// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieController.generated.h"

class UBehaviorTree;
class UPawnSensingComponent;

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZombieController : public AAIController
{
	GENERATED_BODY()
	
protected:
	//--------------------------------------------- Functions
	virtual void BeginPlay() override; // start function
	void Tick(float DeltaTime); // update function

	//--------------------------------------------- Variables

	// Reference to the behaviour tree
	UPROPERTY(EditAnywhere)
		UBehaviorTree* ZombieBehaviorTree;

	// Holds a reference for the player pawn
	APawn* PlayerPawn; 

	

public:
	void SetAttack(bool result);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAttacking;
};
