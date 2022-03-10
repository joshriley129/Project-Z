// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectZGameMode.h"
#include "ProjectZCharacter.h"
#include "ZombieChracter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnPoint.h"

AProjectZGameMode::AProjectZGameMode()
{

}

void AProjectZGameMode::BeginPlay()   // Spawns original enemies
{
	Super::BeginPlay();

	CurrentRound = 1; // Sets start of game
	MaxRoundEnemies = (CurrentRound * 0.15) * MaxSpawnedEnemies;
	bMaxEnemiesOnField = false;
	bIsGameOver = false;

	Player = Cast<AProjectZCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->GameOverHandle.AddUObject(this, &AProjectZGameMode::GameOver);
	}

	UGameplayStatics::PlaySound2D(GetWorld(), BkgrdMusic, 1.0f, 1.0f, 0.0f); // play gun sound from gun
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), SpawnPoints);   // Adds spawn points to an array
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AProjectZGameMode::SpawnEnemies, EnemySpawnWaitTime, false);
}

void AProjectZGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMaxEnemiesOnField)
	{
		if (EnemiesSpawnedThisRound < MaxRoundEnemies && EnemiesActive < MaxSpawnedEnemies)
		{
			GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AProjectZGameMode::SpawnEnemies, 1, false);
		}
	}
}

AActor* AProjectZGameMode::ChooseSpawnpoint()
{
	int index = FMath::RandRange(0, SpawnPoints.Num() - 1);
	return SpawnPoints[index]; // returns an actor
}

void AProjectZGameMode::SpawnEnemies()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	bMaxEnemiesOnField = false;

	AActor* SpwnPnt = ChooseSpawnpoint();  // gets a random spawn point
	FVector SpawnLocation = SpwnPnt->GetActorLocation(); // gets location and rotation
	FRotator SpawnRotation = SpwnPnt->GetActorRotation();
	AActor* TempEnemy = GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation); // spawns actor
	EnemiesActive++; // increases enemy count
	EnemiesSpawnedThisRound++;
	
	AZombieChracter* Zombie = Cast<AZombieChracter>(TempEnemy);	
	if (Zombie) 
	{
		Zombie->ZombieKilledHandle.AddUObject(this, &AProjectZGameMode::EnemyKilled);
	}
	if (Zombie) 
	{
		Zombie->AddPointHandle.AddUObject(this, &AProjectZGameMode::IncreaseScore);
	}

	if (!bIsGameOver)
	{
		if (EnemiesSpawnedThisRound < MaxRoundEnemies && EnemiesActive < MaxSpawnedEnemies)
		{
			GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AProjectZGameMode::SpawnEnemies, 1, false);
		}
		else
		{
			bMaxEnemiesOnField = true;
		}
	}
}

void AProjectZGameMode::EnemyKilled()
{
	EnemiesActive--; // Reduce enemy count

	if (EnemiesActive <= 0) // If still enemies
	{
		CurrentRound++; // Increase round
		EnemiesActive = 0;
		EnemiesSpawnedThisRound = 0;
		MaxRoundEnemies = (CurrentRound * 0.15) * MaxSpawnedEnemies;

		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AProjectZGameMode::SpawnEnemies, EnemySpawnWaitTime, false); // spawn enemies after 5 seconds		
	}
}


void AProjectZGameMode::IncreaseScore()
{
	TotalGameScore += PointAmountPerEnemy;
	if (Player)
	{
		Player->AddPoints();
	}
}

void AProjectZGameMode::GameOver()
{
	bIsGameOver = true;
}
