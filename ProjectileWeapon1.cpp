// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon1.h"
#include "EnergyProjectile.h"

void AProjectileWeapon1::ActivateWeapon(AController* ControllerRef)
{
	Super::ActivateWeapon(ControllerRef);

	FVector CamLoc;
	FRotator CamRot;

	if (ControllerRef)
	{
		ControllerRef->GetPlayerViewPoint(CamLoc, CamRot); // get camera viewpoint
	}

	FVector SpawnLocation = GunMesh->GetSocketLocation("Muzzle");
	AActor* TempProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, CamRot); // spawns actor

	if (TempProjectile)
	{
		TempProjectile = Cast<AEnergyProjectile>(TempProjectile)->ControllerRef = ControllerRef;
	}
}