// Fill out your copyright notice in the Description page of Project Settings.


#include "AimingGameplayAbility.h"
#include "ProjectZCharacter.h"
#include "BaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameplayTagContainer.h"

UAimingGameplayAbility::UAimingGameplayAbility()
{	
	
}

void UAimingGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo)
	{
		if (AProjectZCharacter* PlayerChar = Cast<AProjectZCharacter>(ActorInfo->AvatarActor.Get()))
		{
			if (!PlayerChar->bSprintToggle && !PlayerChar->bReloadToggle)
			{
				PlayerChar->FollowCamera->Deactivate();
				PlayerChar->ADSCamera->Activate();
			}
		}
	}
}

void UAimingGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ActorInfo)
	{
		if (AProjectZCharacter* PlayerChar = Cast<AProjectZCharacter>(ActorInfo->AvatarActor.Get()))
		{
			PlayerChar->ADSCamera->Deactivate();
			PlayerChar->FollowCamera->Activate();
		}
	}
}

void UAimingGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
