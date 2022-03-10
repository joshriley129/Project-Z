// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectZAttributeSet.h"
#include "Net/UnrealNetwork.h"

UProjectZAttributeSet::UProjectZAttributeSet()
{

}

void UProjectZAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UProjectZAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectZAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
}

void UProjectZAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectZAttributeSet, Health, OldHealth);
}

void UProjectZAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectZAttributeSet, Stamina, OldStamina);
}

void UProjectZAttributeSet::OnRep_Points(const FGameplayAttributeData& OldPoints)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectZAttributeSet, Points, OldPoints);
}
