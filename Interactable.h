// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class PROJECTZ_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere, Category = "Collision")
		class USphereComponent* CollisionSphere;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		int InteractableType;
	/* 
	   0 = Ammo Pickup Crate
	   1 = Gun Upgrade Workbench
	*/

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		int InteractablePrice;
};
