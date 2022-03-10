// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectZPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AProjectZPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> CrosshairClass; // Reference to the crosshair class

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> HUDClass; // Reference to the HUD class

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> BloodSplatterClass; // Reference to the BloodSplatter class

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> MessageClass; // Reference to the BloodSplatter class

	UPROPERTY()
		UUserWidget* CrosshairWidget; // crosshair widget

	UPROPERTY()
		UUserWidget* HUDWidget; // HUD widget

	UPROPERTY()
		UUserWidget* BloodSplatterWidget; // BloodSplatter widget

	UPROPERTY()
		UUserWidget* MessageWidget; // Message widget

public:

	UFUNCTION()
		void ActivateBloodSplatterHUD();

	UFUNCTION()
		void RemoveBloodSplatterHUD();

	UFUNCTION()
		void ActivateMessageHUD();

	UFUNCTION()
		void RemoveMessageHUD();

	FTimerHandle RemoveBloodTimerHandle;
};
