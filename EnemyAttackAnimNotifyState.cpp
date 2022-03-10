// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackAnimNotifyState.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "ZombieChracter.h"

void UEnemyAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Cast<AZombieChracter>(Owner))
	{
		Cast<AZombieChracter>(Owner)->ActivateCollisionSphere();
	}
}

void UEnemyAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
}

void UEnemyAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Cast<AZombieChracter>(Owner))
	{
		Cast<AZombieChracter>(Owner)->DeactivateCollisionSphere();
	}
}
