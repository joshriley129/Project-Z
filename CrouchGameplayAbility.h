// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectZ.h"
#include "CrouchGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UCrouchGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCrouchGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		EProjectZAbilityInputID AbilityInputID = EProjectZAbilityInputID::None;	

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float CrouchSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float WalkSpeed;
};
