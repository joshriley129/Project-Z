// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ZombieChracter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
AEnergyProjectile::AEnergyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = 1500.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
}

// Called when the game starts or when spawned
void AEnergyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (CollisionSphere)
	{
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnergyProjectile::BeginOverlap);
	}

	GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &AEnergyProjectile::EndProjectileLife, ProjectileLifetime, false);
}

void AEnergyProjectile::EndProjectileLife()
{
	GetWorld()->GetTimerManager().ClearTimer(LifetimeTimerHandle);
	Destroy();
}

// Called every frame
void AEnergyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AEnergyProjectile::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (Cast<AZombieChracter>(OtherActor))
		{
			UGameplayStatics::ApplyDamage(OtherActor, ProjectileDamage, ControllerRef, this, UDamageType::StaticClass()); // calls apply damage function to whatever is hit
		}
	}
}


