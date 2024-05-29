// Fill out your copyright notice in the Description page of Project Settings.


#include "AirRaceHUD.h"

#include "BaseSportsCarPawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Engine/Canvas.h"

AAirRaceHUD::AAirRaceHUD()
{
	
}

void AAirRaceHUD::DrawHUD()
{
	Super::DrawHUD();
	const float HUDXRatio = Canvas->SizeX / 1280.f;
	const float HUDYRatio = Canvas->SizeY / 720.f;
	
	ABaseSportsCarPawn* Vehicle = Cast<ABaseSportsCarPawn>(GetOwningPawn());
	if(Vehicle)
	{
		FVector2D ScaleVec(HUDYRatio * 1.4f, HUDYRatio * 1.4f);
		FCanvasTextItem SpeedTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 455), Vehicle->SpeedDisplayString, GEngine->GetLargeFont(), FLinearColor::Red);
		SpeedTextItem.Scale = ScaleVec;
		Canvas->DrawItem(SpeedTextItem);
		// Vehicle->DisplayDebug(Canvas, )
		// float YL = 0.0f;
		// float YPos = 0.0f;
		// Vehicle->GetWheeledVehicleMovementComponent()->DrawDebug(Canvas, YL, YPos);
	}
}

// void AAirRaceHUD::ShowDebugInfo(float& YL, float& YPos)
// {
// 	Super::ShowDebugInfo(YL, YPos);
// 	ABaseSportsCarPawn* Vehicle = Cast<ABaseSportsCarPawn>(GetOwningPawn());
// 	if(Vehicle)
// 	{
// 		Vehicle->DisplayDebug(Canvas, GetDebugD, YL, YPos);
// 	}
// }
