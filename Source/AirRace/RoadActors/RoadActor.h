// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	int SplinePoints = 5;
	const int SplinePointsOffset = 2;
	float CurrentAngle = 0.0f;
	int DistanceBetweenPoints = 1000;
	int Radius = 0;
	float TurnSide = 1.0f; // positive left, negative right; 
	float DegreeStep = 0.0f;
	
	
	virtual void OnConstruction(const FTransform& Transform) override;
	void Add();
private:
	// Generation
	void Generate(const int StartIndex, const int FinishIndex);
	void GenerateSplinePoints(const int StartIndex, const int FinishIndex);
	void GenerateStaticMesh(const int StartIndex, const int FinishIndex);
	void GenerateTurn(const int StartIndex, const int FinishIndex);
	void GenerateTriggerBox(const int FirstIndex, const int SecondIndex);

	
	void SetCurrentAngle();
	float GetAngleOfSplineIndexes(const int FirstIndex, const int SecondIndex);
	FVector GetCenterOfTurn(const int StartTurnIndex);
	
	// Spline 
	int GetSplineLastIndex();
	FVector GetFinish();
	FRotator GetRotation();
};
