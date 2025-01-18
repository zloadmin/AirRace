// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AirRaceHUD.generated.h"

/**
 * 
 */
UCLASS()
class AIRRACE_API AAirRaceHUD : public AHUD
{
	GENERATED_BODY()
public:
	AAirRaceHUD();
	virtual void DrawHUD() override;
	// virtual void ShowDebugInfo(float& YL, float& YPos) override;
};
