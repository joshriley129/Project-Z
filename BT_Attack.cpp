// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Attack.h"
#include "AIController.h"
#include "ZombieController.h"

EBTNodeResult::Type UBT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory); // Call the super as this is an override

	AAIController* ZombieAIController = OwnerComp.GetAIOwner(); // Return ai controller
	if (ZombieAIController)
	{
		if (Cast<AZombieController>(ZombieAIController))
		{
			Cast<AZombieController>(ZombieAIController)->SetAttack(true);
		}
	}
	return EBTNodeResult::Succeeded;
}