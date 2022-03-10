// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnergyProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class PROJECTZ_API AEnergyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnergyProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void EndProjectileLife();

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Collision")
		USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		float ProjectileLifetime;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		int ProjectileDamage;

	FTimerHandle LifetimeTimerHandle;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY()
		AController* ControllerRef;
};
