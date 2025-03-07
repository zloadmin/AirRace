// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoadActorParams.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"
#include "RoadActor.generated.h"


UCLASS()
class AIRRACE_API ARoadActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoadActor();
	UPROPERTY(VisibleAnywhere, Category= "Spline")
	USplineComponent* Spline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline")
	UStaticMesh* StaticMesh;

public:
	UFUNCTION()
	void OnOverlapStartBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerBoxBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	// TODO: rename to with prefix Generate and make all private
	// int SplinePoints = 5;
	// const int SplinePointsOffset = 2;
	double CurrentAngle = 0.0f;
	// int DistanceBetweenPoints = 1000;
	// int Radius = 0;
	// float TurnSide = 1.0f; // positive left, negative right; 
	// float DegreeStep = 0.0f;
	//
	//
	virtual void OnConstruction(const FTransform& Transform) override;
	void Add(const FRoadActorParams& Params);

	static bool IsNotOffsetSplineIndex(const int Index, const FRoadActorParams& Params);
	static bool IsFirstNotOffsetSplineIndex(const int Index, const FRoadActorParams& Params);
	static bool IsLastNotOffsetSplineIndex(const int Index, const FRoadActorParams& Params);
	static bool IsOffsetSplineIndex(const int Index, const FRoadActorParams& Params);
	static TTuple<double, double> GetRollDegreeByIndex(const int Index, const FRoadActorParams& Params);

private:
	// Generation
	void Generate(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params);
	void Generate(const int StartIndex, const int FinishIndex, TArray<const FRoadActorParams>& Params);
	void GenerateSplinePoints(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params);
	void GenerateClimb(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params);
	void GenerateStaticMesh(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params);
	void GenerateStaticMeshRoll(const int Index, const FRoadActorParams& Params, USplineMeshComponent* SplineMeshComponent);
	void GenerateTurn(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params);
	void GenerateTriggerBox(const int FirstIndex, const int SecondIndex);

	void SetCurrentAngle();
	double GetAngleOfSplineIndexes(const int FirstIndex, const int SecondIndex);
	FVector GetCenterOfTurn(const int StartTurnIndex, const FRoadActorParams& Params);

	// Spline 
	int GetSplineLastIndex() const;
	FVector GetFinish();
	FRotator GetRotation();
};
