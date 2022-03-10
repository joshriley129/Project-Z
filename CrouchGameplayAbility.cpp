// Fill out your copyright notice in the Description page of Project Settings.

#include "CrouchGameplayAbility.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "ProjectZCharacter.h"
#include "GameplayTagContainer.h"

UCrouchGameplayAbility::UCrouchGameplayAbility()
{
	
}

void UCrouchGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability Activated: Crouch"));
	if (ActorInfo)
	{
		if (AProjectZCharacter* PlayerChar = Cast<AProjectZCharacter>(ActorInfo->AvatarActor.Get()))
		{
			PlayerChar->bCrouchToggle = true;
			PlayerChar->GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		}
	}
}

void UCrouchGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UE_LOG(LogTemp, Warning, TEXT("Ability Ended: Crouch"));
	if (ActorInfo)
	{
		if (AProjectZCharacter* PlayerChar = Cast<AProjectZCharacter>(ActorInfo->AvatarActor.Get()))
		{
			PlayerChar->bCrouchToggle = false;
			PlayerChar->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void UCrouchGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
