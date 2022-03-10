// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "AR2.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AAR2 : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	virtual void ActivateWeapon(AController* ControllerRef) override;
};
