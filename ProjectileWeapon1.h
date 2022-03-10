// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "ProjectileWeapon1.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AProjectileWeapon1 : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	virtual void ActivateWeapon(AController* ControllerRef) override;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		TSubclassOf<AActor> ProjectileClass; 


};
