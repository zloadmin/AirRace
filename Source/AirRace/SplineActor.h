// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"
#include "SplineActor.generated.h"


UCLASS()
class AIRRACE_API ASplineActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category= "Spline")
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline")
	UStaticMesh* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline")
	TEnumAsByte<ESplineMeshAxis::Type>ForwardAxis;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 0.0f, ClampMax = 1.0f))
	float Roll = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 10, ClampMax = 100))
	int SplinePoints = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 100, ClampMax = 10000))
	int DistanceBetweenPoints = 100;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 1.0f, ClampMax = 10000.0f))
	// float TurnAngle = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 1000, ClampMax = 1000000))
	int Radius = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 1.0f, ClampMax = 45.0f))
	float DegreeStep = 10.0f;
	
	
	virtual void OnConstruction(const FTransform& Transform) override;

	void GenerateSplinePoints();
	void MoveSplinePointsToCycle();
	void GenerateStaticMesh();
	int GetRandomForm();
	int RandomForm;
};
