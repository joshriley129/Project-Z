// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectZ.h"
#include "ReloadGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UReloadGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
		UReloadGameplayAbility();

		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
			EProjectZAbilityInputID AbilityInputID = EProjectZAbilityInputID::None;

		FTimerHandle ReloadTimerHandle;

		// Reload animation time
		UPROPERTY(EditAnywhere, Category = "Gameplay")
			float ReloadAnimTime;

		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
		virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
		void ReloadGun(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
};
