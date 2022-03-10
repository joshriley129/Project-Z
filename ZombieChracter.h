// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieChracter.generated.h"

class AZombieController;
class USphereComponent;
class AProjectZGameMode;

DECLARE_MULTICAST_DELEGATE(FZombieKilledHandle);
DECLARE_MULTICAST_DELEGATE(FAddPointsHandle);

UCLASS()
class PROJECTZ_API AZombieChracter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieChracter();

	FZombieKilledHandle ZombieKilledHandle;
	FAddPointsHandle AddPointHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
		class UAudioComponent* AudioComp;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void ActivateCollisionSphere();

	UFUNCTION()
		void DeactivateCollisionSphere();

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAttacking;*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bPlayerJustHit;

protected:
	//--------------------------------------------- Functions
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void Damage(float damageAmount);

	UFUNCTION(BlueprintCallable)
		void End();

	UFUNCTION(BlueprintCallable)
		void ReadyAttack();

	UFUNCTION()
		void DropPickup();

	//--------------------------------------------- Variables
	// Used when killed before destroying object 
	FTimerHandle DestroyTimerHandle;
	FTimerHandle DelayTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int Health;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DeathLength;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackDelayLength;

	UPROPERTY(EditAnywhere)
		USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere)
		int DropChance;

	UPROPERTY(EditAnywhere)
		float AttackDamage;

	// Game mode reference
	UPROPERTY(EditAnywhere)
		AProjectZGameMode* GameModeRef;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AActor>> PickupClasses;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* IdleSound;

	UPROPERTY()
		int RoundNumWhenSpawned;
};
