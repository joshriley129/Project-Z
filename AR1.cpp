// Fill out your copyright notice in the Description page of Project Settings.


#include "AR1.h"
#include "Kismet/GameplayStatics.h"

void AAR1::ActivateWeapon(AController* ControllerRef)
{
	Super::ActivateWeapon(ControllerRef);

	// Raycast work
	FVector CamLoc;
	FRotator CamRot;
	ControllerRef->GetPlayerViewPoint(CamLoc, CamRot); // get camera viewpoint

	FVector CastEnd = CamLoc + CamRot.Vector() * CastRange; // gets cast ray finish point from camera for the cast range
	FHitResult Hit; // sets up hit target
	bool bDidHit = GetWorld()->LineTraceSingleByChannel(Hit, CamLoc, CastEnd, ECC_Visibility); // fires ray 
	if (bDidHit)
	{
		if (Hit.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit something: %s"), *GetNameSafe(Hit.GetActor())); // if hits, output name
			float ImpForce = 1000; // add an impulse of 1000 to the object
			UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Hit.GetActor()->GetRootComponent());
			if (RootComp->IsSimulatingPhysics()) // checks if the object is simulating physics, error checking
			{
				RootComp->AddImpulse(CamRot.Vector() * ImpForce * RootComp->GetMass()); // adds the impulse
			}
			UGameplayStatics::ApplyDamage(Hit.GetActor(), WeaponDamage, ControllerRef, this, UDamageType::StaticClass()); // calls apply damage function to whatever is hit
		}
	}
}