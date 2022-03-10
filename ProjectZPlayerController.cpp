// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectZPlayerController.h"
#include "Blueprint/UserWidget.h"

void AProjectZPlayerController::BeginPlay()  // Begin play adds the crosshair to the screen
{
	Super::BeginPlay();
	
	CrosshairWidget = CreateWidget(this, CrosshairClass);  // creates widget
	HUDWidget = CreateWidget(this, HUDClass);  // creates widget
	BloodSplatterWidget = CreateWidget(this, BloodSplatterClass);
	MessageWidget = CreateWidget(this, MessageClass);

	if (CrosshairWidget) 
	{
		CrosshairWidget->AddToViewport(); // adds widget to screen
	}

	if (HUDWidget) 
	{
		HUDWidget->AddToViewport(); // adds widget to screen
	}

	if (MessageWidget) 
	{
		MessageWidget->AddToViewport(); // adds widget to screen
		MessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AProjectZPlayerController::ActivateBloodSplatterHUD()
{
	if (BloodSplatterWidget) 
	{
		if (!BloodSplatterWidget->IsVisible())
		{
			BloodSplatterWidget->AddToViewport(); // adds widget to screen
		}
	}

	GetWorld()->GetTimerManager().SetTimer(RemoveBloodTimerHandle, this, &AProjectZPlayerController::RemoveBloodSplatterHUD, 2, false);
}

void AProjectZPlayerController::RemoveBloodSplatterHUD()
{
	if (BloodSplatterWidget) 
	{
		if (BloodSplatterWidget->IsVisible())
		{
			BloodSplatterWidget->RemoveFromViewport(); // adds widget to screen
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(RemoveBloodTimerHandle);
}

void AProjectZPlayerController::ActivateMessageHUD()
{
	if (MessageWidget) 
	{
		MessageWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AProjectZPlayerController::RemoveMessageHUD()
{
	if (MessageWidget) 
	{
		MessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

