// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieChracter.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "ZombieController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "ProjectZCharacter.h"
#include "ProjectZGameMode.h"
#include "Components/AudioComponent.h"

// Sets default values
AZombieChracter::AZombieChracter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(GetMesh(), TEXT("AttackCollision"));

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));

}

// Called when the game starts or when spawned
void AZombieChracter::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionSphere)
	{
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AZombieChracter::BeginOverlap);
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (AudioComp)
	{
		AudioComp->SetSound(IdleSound);
		AudioComp->Play();
	}


	GameModeRef = Cast<AProjectZGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameModeRef)
	{
		if (GameModeRef->CurrentRound <= 10)
		{
			Health = (GameModeRef->CurrentRound * 100);
		}
		else
		{
			Health = 1000 + (GameModeRef->CurrentRound * 10);
		}
	}
}

// Called every frame
void AZombieChracter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombieChracter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AZombieChracter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bPlayerJustHit)
	{
		if (Cast<AProjectZCharacter>(OtherActor))
		{
			Cast<AProjectZCharacter>(OtherActor)->PlayerTakeDamage(AttackDamage);
			bPlayerJustHit = true;
			GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AZombieChracter::ReadyAttack, AttackDelayLength, false);
		}
	}
}

void AZombieChracter::ActivateCollisionSphere()
{
	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AZombieChracter::DeactivateCollisionSphere()
{
	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AZombieChracter::Damage(float DamageAmount)
{
	Health -= DamageAmount;
	AddPointHandle.Broadcast();

	if (Health <= 0)
	{
		if (AudioComp)
		{
			AudioComp->Stop();
		}

		AController* CurrentController = GetController();
		if (CurrentController)
		{
			CurrentController->StopMovement();
			CurrentController->UnPossess();
			CurrentController->Destroy();
		}

		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		UCapsuleComponent* Capsule = GetCapsuleComponent();
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ZombieKilledHandle.Broadcast();

		DropPickup();

		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AZombieChracter::End, DeathLength, false);
	}
}

void AZombieChracter::End()
{
	Destroy();
}

void AZombieChracter::ReadyAttack()
{
	bPlayerJustHit = false;
	GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
}

void AZombieChracter::DropPickup()
{
	int Index = FMath::RandRange(0, 100);
	if (Index > DropChance)
	{
		FVector SpawnLocation = GetActorLocation(); // gets location and rotation
		FRotator SpawnRotation = GetActorRotation();

		Index = FMath::RandRange(0, PickupClasses.Num() - 1);
		AActor* TempEnemy = GetWorld()->SpawnActor<AActor>(PickupClasses[Index], SpawnLocation, SpawnRotation); // spawns actor
	}
}
