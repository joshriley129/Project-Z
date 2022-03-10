// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleAmmoPickup.h"
#include "Components/BoxComponent.h"
#include "ProjectZCharacter.h"

// Sets default values
ARifleAmmoPickup::ARifleAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(RootComponent);
	BoxMesh->OnComponentBeginOverlap.AddDynamic(this, &ARifleAmmoPickup::BeginOverlap);
}

// Called when the game starts or when spawned
void ARifleAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &ARifleAmmoPickup::EndDropLife, AmmoDropLifetime, false);
}

void ARifleAmmoPickup::EndDropLife()
{
	GetWorld()->GetTimerManager().ClearTimer(LifetimeTimerHandle);
	Destroy();
}

// Called every frame
void ARifleAmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARifleAmmoPickup::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AProjectZCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Collected Ammo")); // if missed then output that
		Cast<AProjectZCharacter>(OtherActor)->RefillGuns();
		EndDropLife();
	}
}