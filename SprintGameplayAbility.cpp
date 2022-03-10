// Fill out your copyright notice in the Description page of Project Settings.

#include "SprintGameplayAbility.h"
#include "ProjectZCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameplayTagContainer.h"

USprintGameplayAbility::USprintGameplayAbility()
{
   
}

void USprintGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo)
	{
		if (AProjectZCharacter* PlayerChar = Cast<AProjectZCharacter>(ActorInfo->AvatarActor.Get()))
		{
			if (!PlayerChar->bCrouchToggle)
			{
				PlayerChar->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
				PlayerChar->SwitchSprintToggle(true);
				if (PlayerChar->ADSCamera)
				{
					PlayerChar->ADSCamera->Deactivate();
				}
				if (PlayerChar->FollowCamera)
				{
					PlayerChar->FollowCamera->Activate();
				}
			}
		}
	}
}

void USprintGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ActorInfo)
	{
		if (AProjectZCharacter* PlayerChar = Cast<AProjectZCharacter>(ActorInfo->AvatarActor.Get()))
		{
			PlayerChar->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			PlayerChar->SwitchSprintToggle(false);
		}
	}
}

void USprintGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
