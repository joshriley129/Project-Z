// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RifleAmmoPickup.generated.h"

class AProjectZCharacter;

UCLASS()
class PROJECTZ_API ARifleAmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARifleAmmoPickup();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* BoxMesh;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void EndDropLife();

	UPROPERTY(EditAnywhere, Category = Gameplay)
		float AmmoDropLifetime;

	FTimerHandle LifetimeTimerHandle;
};
