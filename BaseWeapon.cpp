// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	RootComponent = GunMesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);

	bIsEquipped = false;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	MaxReserveAmmo = ReserveAmmo;	
	GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &ABaseWeapon::Despawn, GunDropLifetime, false);
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsEquipped)
	{
		CollisionSphere->SetGenerateOverlapEvents(false);
	}
}

// Called when weapon activated
void ABaseWeapon::ActivateWeapon(AController* ControllerRef)
{
	UGameplayStatics::PlaySound2D(GetWorld(), GunSound, 1.0f, 1.0f, 0.0f); // play gun sound from gun
	const FVector Loc = GunMesh->GetSocketLocation("Muzzle");
	FRotator Rot = GunMesh->GetSocketRotation("Muzzle");
    UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleFlash, Loc, Rot, MuzzleFlashScale);
	Ammo--;
}

void ABaseWeapon::RefillMags()
{
	ReserveAmmo = MaxReserveAmmo;
}

void ABaseWeapon::Despawn()
{
	if (!bIsEquipped)
	{
		Destroy();
	}
}

void ABaseWeapon::DisableCollision()
{
	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseWeapon::EnableCollision()
{
	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}