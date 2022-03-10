// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EProjectZAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Sprint,
	Crouch,
	Reload,
	Aim,
	Fire
};