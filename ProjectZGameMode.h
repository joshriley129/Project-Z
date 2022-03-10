// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectZGameMode.generated.h"

class ASpawnPoint;

UCLASS(minimalapi)
class AProjectZGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	//////////////////////////////////////////////////////////////////////////
	// Functions
	virtual void BeginPlay() override;  // Start function
	virtual void Tick(float DeltaTime) override;  // Update function  

	// Spawning Functions
	AActor* ChooseSpawnpoint();         // Function chooses a spawnpoint
	void SpawnEnemies();                // Function spawns enemies

	//////////////////////////////////////////////////////////////////////////
	// Variables
	// Music Variables 
	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* BkgrdMusic;

	// Spawn Enemies Variables

	UPROPERTY(EditAnywhere, Category = "Enemy Spawning")
		TArray<AActor*> SpawnPoints;    // Array of spawn points

	UPROPERTY(EditAnywhere, Category = "Enemy Spawning")
		float EnemySpawnWaitTime; // Time between spawns

	UPROPERTY(EditAnywhere, Category = "Enemy Spawning")
		TSubclassOf<AActor> EnemyClass; // Enemy class (for spawning)

	UPROPERTY()
		FTimerHandle SpawnTimerHandle; // Timer for spawning


		// Round Variables

	UPROPERTY()
		int EnemiesActive; // Stores how many enemies are alive in the world, less expensive than storing references to the enemy

	UPROPERTY()
		int MaxRoundEnemies; // Stores how many enemies can be alive this round

	UPROPERTY(EditAnywhere)
		int MaxSpawnedEnemies; // Stores how many enemies can be in map at one time

	UPROPERTY()
		int EnemiesSpawnedThisRound; // Stores how many enemies have been spawned this round

	UPROPERTY()
		bool bMaxEnemiesOnField; // bool that holds whether any more enemies can be added to the field

	UPROPERTY()
		bool bIsGameOver;

	// Scorekeeping Variables

	UPROPERTY(EditAnywhere)
		int TotalGameScore;

	UPROPERTY()
		class AProjectZCharacter* Player;

public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor
	AProjectZGameMode();

	//////////////////////////////////////////////////////////////////////////
	// Functions
	// Callable from other classes

	UFUNCTION()
		void EnemyKilled(); // Called when enemy killed from zombie character

	UFUNCTION()
		void IncreaseScore(); // Called when enemy hit from zombie character

	UFUNCTION()
		void GameOver(); // Called from player when dead

	//////////////////////////////////////////////////////////////////////////
	// Variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int CurrentRound;

	UPROPERTY(EditAnywhere)
		int PointAmountPerEnemy;
};



