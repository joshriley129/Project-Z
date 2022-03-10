// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"


UCLASS()
class PROJECTZ_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// RayCast Range
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float CastRange;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* GunSound;

	UPROPERTY(EditAnywhere, Category = "Pickup")
		class USphereComponent* CollisionSphere;
public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void ActivateWeapon(AController* ControllerRef);

	UFUNCTION()
		void RefillMags();

	UFUNCTION()
		void Despawn();

	UFUNCTION()
		void DisableCollision();

	UFUNCTION()
		void EnableCollision();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		int Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		int ReserveAmmo;

	UPROPERTY(EditAnywhere, Category = "Ammo")
		int MaxClipAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		int WeaponDamage;

	UPROPERTY()
		int MaxReserveAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		bool bIsEquipped;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
		UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "VFX")
		FVector MuzzleFlashScale;

	UPROPERTY(EditAnywhere, Category = "Pickup")
		float GunDropLifetime;

	FTimerHandle LifetimeTimerHandle;
};
