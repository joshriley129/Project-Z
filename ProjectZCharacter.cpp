// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectZCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "ProjectZPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "BaseWeapon.h"
#include "Interactable.h"
#include "ProjectZAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "ProjectZAttributeSet.h"
#include "SprintGameplayAbility.h"
#include "ReloadGameplayAbility.h"
#include "CrouchGameplayAbility.h"
#include "AimingGameplayAbility.h"
#include "ShootingGameplayAbility.h"
#include <GameplayEffectTypes.h>

//////////////////////////////////////////////////////////////////////////
// AProjectZCharacter

AProjectZCharacter::AProjectZCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AProjectZCharacter::BeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AProjectZCharacter::OnOverlapEnd);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), TEXT("head")); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	// Init ability system
	AbilitySystemComponent = CreateDefaultSubobject<UProjectZAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Create attributes
	Attributes = CreateDefaultSubobject<UProjectZAttributeSet>("Attributes");

	// Setup hidden gun mesh for ADS
	HiddenGunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HiddenGunMesh"));
	HiddenGunMesh->SetupAttachment(GetMesh(), TEXT("weapon_placement"));

	// Setup ADS Camera
	ADSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ADSCamera"));
	ADSCamera->SetupAttachment(HiddenGunMesh); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation

	// Setup gun spawnpoint
	GunSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("MissileSpawnPoint"));
	GunSpawnPoint->SetupAttachment(FollowCamera);

	// Set base bools
	bIsAlive = true;
	bIsInteracting = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectZCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


	PlayerInputComponent->BindAxis("MoveForward", this, &AProjectZCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectZCharacter::MoveRight);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProjectZCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProjectZCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AProjectZCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AProjectZCharacter::TouchStopped);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AProjectZCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AProjectZCharacter::StopInteracting);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EProjectZAbilityInputID", static_cast<int32>(EProjectZAbilityInputID::Confirm), static_cast<int32>(EProjectZAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

//////////////////////////////////////////////////////////////////////////
// Gameplay Functions

// Called when the game starts or when spawned
void AProjectZCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Gun setup
	FVector SpawnLocation = GunSpawnPoint->GetComponentLocation();
	AController* ControllerRef = GetController();
	FVector CamLoc;
	FRotator CamRot;
	if (ControllerRef)
	{
		ControllerRef->GetPlayerViewPoint(CamLoc, CamRot);
	}
	Gun = GetWorld()->SpawnActor<ABaseWeapon>(GunClass, SpawnLocation, CamRot);

	if (Gun)
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("weapon_placement"));
		Gun->DisableCollision();
		Gun->bIsEquipped = true;
	}

	// Setting gameplay effect handles
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	TakeDamageSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageHealthEffect, 1, ContextHandle);
	PassiveHealingSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(HealEffect, 1, ContextHandle);
	AddPointsSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AddPointsEffect, 1, ContextHandle);
	BuyAmmoSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(BuyAmmoEffect, 1, ContextHandle);
}

void AProjectZCharacter::Tick(float DeltaTime)
{
	// Passive Health Regen
	if (bIsAlive)
	{
		if (Attributes)
		{
			if (Attributes->GetHealth() > 1)
			{
				if (HealthRegenTimer >= HealthRegenRate)
				{
					if (PassiveHealingSpecHandle.IsValid())
					{
						ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PassiveHealingSpecHandle.Data.Get());
					}

					HealthRegenTimer = 0;
				}
				else
				{
					HealthRegenTimer += 1 * DeltaTime;
				}
			}
		}
	}

	// Interacting with overlapped objects
	if (bIsOverlapping)
	{
		if (Cast<ABaseWeapon>(OverlappedActor))
		{
			if (bIsInteracting)
			{
				UE_LOG(LogTemp, Warning, TEXT("Gun"));
				if (Gun)
				{
					Gun->Destroy();
				}

				Gun = Cast<ABaseWeapon>(OverlappedActor);
				OverlappedActor->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("weapon_placement"));
				Cast<ABaseWeapon>(OverlappedActor)->bIsEquipped = true;
				bIsInteracting = false;
			}
		}
		if (Cast<AInteractable>(OverlappedActor))
		{
			if (bIsInteracting)
			{
				if (Cast<AInteractable>(OverlappedActor)->InteractableType == 0 && Cast<AInteractable>(OverlappedActor)->InteractablePrice <= Attributes->GetPoints()) // Ammo Crate
				{
					UE_LOG(LogTemp, Warning, TEXT("Ammo Crate"));

					RefillGuns();
					if (BuyAmmoSpecHandle.IsValid())
					{
						ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*BuyAmmoSpecHandle.Data.Get());
					}
					bIsInteracting = false;
				}
			}
		}
	}
}

void AProjectZCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AProjectZCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AProjectZCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProjectZCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProjectZCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AProjectZCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AProjectZCharacter::StopAimDownSights()
{
	if (ADSCamera)
	{
		ADSCamera->Deactivate();
	}
	if (FollowCamera)
	{
		FollowCamera->Activate();
	}
}

void AProjectZCharacter::AddPoints()
{
	if (AddPointsSpecHandle.IsValid())
	{
		ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*AddPointsSpecHandle.Data.Get());
	}
}

void AProjectZCharacter::SwitchSprintToggle(bool Switch)
{
	bSprintToggle = Switch;
}

void AProjectZCharacter::PlayerTakeDamage(float Damage)
{
	// Call gameplay effect
	if (TakeDamageSpecHandle.IsValid())
	{
		ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*TakeDamageSpecHandle.Data.Get());
	}

	// Activate blood splatter
	AController* CurrentController = GetController();
	Cast<AProjectZPlayerController>(CurrentController)->ActivateBloodSplatterHUD();

	// Check if dead
	if (Attributes)
	{
		if (Attributes->GetHealth() <= 0)
		{
			GameOverHandle.Broadcast();
			bIsAlive = false;

			if (CurrentController) 
			{
				CurrentController->StopMovement();
				CurrentController->UnPossess();
			}
			GetMesh()->SetSimulatePhysics(true);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			UCapsuleComponent* Capsule = GetCapsuleComponent();
			if (Capsule)
			{
				Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void AProjectZCharacter::RefillGuns()
{
	if (Gun)
	{
		Gun->RefillMags();
	}
}

void AProjectZCharacter::Interact()
{
	bIsInteracting = true;
}

void AProjectZCharacter::StopInteracting()
{
	bIsInteracting = false;
}

void AProjectZCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsOverlapping = true;
	if (OtherActor)
	{
		OverlappedActor = OtherActor;
	}

	AController* CurrentController = GetController();
	if (CurrentController)
	{
		Cast<AProjectZPlayerController>(CurrentController)->ActivateMessageHUD();
	}	
}

void AProjectZCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlapping = false;
	OverlappedActor = nullptr;

	AController* CurrentController = GetController();
	if (CurrentController)
	{
		Cast<AProjectZPlayerController>(CurrentController)->RemoveMessageHUD();
	}
}

//////////////////////////////////////////////////////////////////////////
// Gameplay Ability

UAbilitySystemComponent* AProjectZCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
void AProjectZCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}
void AProjectZCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(SprintAbility, 1, static_cast<int32>(SprintAbility.GetDefaultObject()->AbilityInputID), this));
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ReloadAbility, 1, static_cast<int32>(ReloadAbility.GetDefaultObject()->AbilityInputID), this));
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(CrouchAbility, 1, static_cast<int32>(CrouchAbility.GetDefaultObject()->AbilityInputID), this));
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AimingAbility, 1, static_cast<int32>(AimingAbility.GetDefaultObject()->AbilityInputID), this));
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ShootingAbility, 1, static_cast<int32>(ShootingAbility.GetDefaultObject()->AbilityInputID), this));
	}
}
void AProjectZCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveAbilities();
}
void AProjectZCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAttributes();

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EProjectZAbilityInputID", static_cast<int32>(EProjectZAbilityInputID::Confirm), static_cast<int32>(EProjectZAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}
