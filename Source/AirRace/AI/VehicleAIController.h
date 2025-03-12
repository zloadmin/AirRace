// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "VehicleAIController.generated.h"

/**
 * 
 */
UCLASS()
class AIRRACE_API AVehicleAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AVehicleAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
private:
	void MoveToTarget();
	int TargetSplinePointIndex  = 2;
};
