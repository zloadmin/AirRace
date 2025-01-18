// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SplineActor.h"
#include "GameFramework/GameStateBase.h"
#include "AirRaceGameState.generated.h"

/**
 * 
 */
UCLASS()
class AIRRACE_API AAirRaceGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AAirRaceGameState();
	void GenerateNextRoad();
	void InitRoad();
	void AddStraightRoad();
	void GenerateRoad(int Points);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds);

public:
	UPROPERTY(VisibleAnywhere, Category = "Road")
	FVector RoadFinish = FVector(0, 0, 0);
	UPROPERTY(VisibleAnywhere, Category = "Road")
	FRotator RoadRotation = FRotator(0, 0, 0);
	UPROPERTY(VisibleAnywhere, Category = "Road")
	TArray<ASplineActor*> Roads;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 100, ClampMax = 10000))
	int DistanceBetweenPoints = 1000;

private:
	ASplineActor* Road; // TODO: delete
	USplineComponent* RoadSplineComponent;
	UStaticMesh* RoadStaticMesh;
	FVector GetRoadFinish();
	FRotator GetRoadRotation();
	void SetRoadFinish(ASplineActor* Actor);
	void SetRoadRotation(ASplineActor* Actor);
	TArray<UClass*> Actors;
	void SetActors();
	UClass* GetRandomActor();

	void GenerateRoadSplinePoints(int Points);
	void GenerateRoadSplineStaticMesh(int Points);
};
