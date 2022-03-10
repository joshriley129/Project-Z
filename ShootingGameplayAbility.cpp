// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameplayAbility.h"
#include "ProjectZCharacter.h"
#include "BaseWeapon.h"
#include "GameFramework/Controller.h"
#include "GameplayTagContainer.h"

UShootingGameplayAbility::UShootingGameplayAbility()
{

}

void UShootingGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FTimerDelegate FireTimerDel = FTimerDelegate::CreateUObject(this, &UShootingGameplayAbility::ActivateAbility, Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (ActorInfo)
	{
		if (AProjectZCharacter* PlayerChar = Cast<AProjectZCharacter>(ActorInfo->AvatarActor.Get()))
		{
			if (PlayerChar->bIsAlive)
			{
				if (!PlayerChar->bSprintToggle && !PlayerChar->bReloadToggle)
				{
					if (ABaseWeapon* CurrentGun = Cast<ABaseWeapon>(PlayerChar->Gun))
					{
						if (CurrentGun->Ammo > 0)
						{
							AController* ControllerRef = PlayerChar->GetController();
							CurrentGun->ActivateWeapon(ControllerRef);

							// set timer for recursion
							GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, FireTimerDel, CurrentGun->FireRate, false);
						}
					}
				}
			}
		}
	}
}

void UShootingGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void UShootingGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}