// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "ProjectZCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FGameOverHandle);

class ABaseWeapon;

UCLASS(config = Game)
class AProjectZCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
		class UProjectZAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		class UProjectZAttributeSet* Attributes;

public:
	AProjectZCharacter();

	FGameOverHandle GameOverHandle;

	UFUNCTION()
		void PlayerTakeDamage(float Damage);

	UFUNCTION()
		void SwitchSprintToggle(bool Switch);

	UFUNCTION()
		void RefillGuns();

	UFUNCTION()
		void StopAimDownSights();

	UFUNCTION()
		void AddPoints();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* ADSCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// Whether crouching or not
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		bool bCrouchToggle;

	// Whether sprinting or not
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		bool bSprintToggle;

	// Whether reloading or not
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		bool bReloadToggle;

	// Whether shooting or not
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		bool bShootToggle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool bIsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		bool bIsOverlapping;

	UPROPERTY(BlueprintReadWrite)
		ABaseWeapon* Gun;

	UPROPERTY(BlueprintReadWrite, Category = "Collision")
		bool bIsInteracting;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void Interact();
	void StopInteracting();

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Attribute Functions
	virtual void InitializeAttributes();
	virtual void GiveAbilities();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Gun class
	UPROPERTY(EditAnywhere, Category = "Gun")
		TSubclassOf<ABaseWeapon> GunClass;

	// Gun Spawn Point
	UPROPERTY(EditAnywhere, Category = "Gun")
		USceneComponent* GunSpawnPoint;

	// Skeletal Mesh for aim down sights
	UPROPERTY(EditAnywhere, Category = "Aiming")
		USkeletalMeshComponent* HiddenGunMesh;

	// Crouching Speed
	UPROPERTY(EditAnywhere, Category = "Gameplay")
		float CrouchSpeed;

	// Walking Speed
	UPROPERTY(EditAnywhere, Category = "Gameplay")
		float WalkSpeed;

	// Sprinting Speed
	UPROPERTY(EditAnywhere, Category = "Gameplay")
		float SprintSpeed;

	UPROPERTY()
		AActor* OverlappedActor;

	UPROPERTY()
		float HealthRegenTimer;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;

	struct FGameplayEffectSpecHandle TakeDamageSpecHandle;
	struct FGameplayEffectSpecHandle PassiveHealingSpecHandle;
	struct FGameplayEffectSpecHandle AddPointsSpecHandle;
	struct FGameplayEffectSpecHandle BuyAmmoSpecHandle;
	struct FActiveGameplayEffectHandle ActiveGameplayEffectHandle;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// GAS Variables
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UGameplayEffect> DamageHealthEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UGameplayEffect> HealEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UGameplayEffect> AddPointsEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UGameplayEffect> BuyAmmoEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class USprintGameplayAbility> SprintAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UReloadGameplayAbility> ReloadAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UCrouchGameplayAbility> CrouchAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UAimingGameplayAbility> AimingAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UShootingGameplayAbility> ShootingAbility;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

