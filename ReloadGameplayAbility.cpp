// Fill out your copyright notice in the Description page of Project Settings.

#include "ReloadGameplayAbility.h"
#include "ProjectZCharacter.h"
#include "BaseWeapon.h"
#include "Camera/CameraComponent.h"

UReloadGameplayAbility::UReloadGameplayAbility()
{	

}

void UReloadGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FTimerDelegate ReloadTimerDel = FTimerDelegate::CreateUObject(this, &UReloadGameplayAbility::ReloadGun, Handle, ActorInfo, ActivationInfo);

	if (ActorInfo)
	{
		if (AProjectZCharacter* PlayerChar = Cast<AProjectZCharacter>(ActorInfo->AvatarActor.Get()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability Activated: Reload"));
			if (!PlayerChar->bSprintToggle)
			{
				if (PlayerChar->Gun->Ammo < PlayerChar->Gun->MaxClipAmmo && PlayerChar->Gun->ReserveAmmo > 0)
				{
					PlayerChar->bReloadToggle = true;

					if (PlayerChar->ADSCamera)
					{
						PlayerChar->ADSCamera->Deactivate();
					}
					if (PlayerChar->FollowCamera)
					{
						PlayerChar->FollowCamera->Activate();
					}
					GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, ReloadTimerDel, ReloadAnimTime, false);
				}
				else
				{
					EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
				}
			}
			else
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			}
		}
	}
}

void UReloadGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AProjectZCharacter* PlayerChar = Cast<AProjectZCharacter>(ActorInfo->AvatarActor.Get()))
	{
		PlayerChar->bReloadToggle = false;
	}
}

void UReloadGameplayAbility::ReloadGun(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (AProjectZCharacter* PlayerChar = Cast<AProjectZCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (PlayerChar->bReloadToggle)
		{
			int Leftover = PlayerChar->Gun->MaxClipAmmo - PlayerChar->Gun->Ammo;
			if (PlayerChar->Gun->ReserveAmmo >= PlayerChar->Gun->MaxClipAmmo)
			{

				PlayerChar->Gun->Ammo = PlayerChar->Gun->MaxClipAmmo;
				PlayerChar->Gun->ReserveAmmo -= Leftover;
			}
			else if (PlayerChar->Gun->ReserveAmmo < PlayerChar->Gun->MaxClipAmmo && PlayerChar->Gun->ReserveAmmo > 0)
			{
				PlayerChar->Gun->Ammo += PlayerChar->Gun->ReserveAmmo;
				PlayerChar->Gun->ReserveAmmo = 0;
			}
		}

		PlayerChar->bReloadToggle = false;
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
