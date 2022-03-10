// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectZ.h"
#include "SprintGameplayAbility.generated.h"

class AProjectZCharacter;
/**
 * 
 */
UCLASS()
class PROJECTZ_API USprintGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	USprintGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		EProjectZAbilityInputID AbilityInputID = EProjectZAbilityInputID::None;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
		float SprintSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
		float WalkSpeed;
};
