// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"


void AZombieController::BeginPlay()
{
	Super::BeginPlay();

	if (ZombieBehaviorTree)
	{
		RunBehaviorTree(ZombieBehaviorTree); // starts behaviour tree
	}
}

void AZombieController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // gets refernce to player pawn
	if (PlayerPawn)
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation()); // sets players location
	}
}

void AZombieController::SetAttack(bool result)
{
	bAttacking = result;
}