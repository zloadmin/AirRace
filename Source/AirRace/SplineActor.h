// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
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
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Box")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 0.0f, ClampMax = 1.0f))
	float Roll = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 10, ClampMax = 100))
	int SplinePoints = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 100, ClampMax = 10000))
	int DistanceBetweenPoints = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 0, ClampMax = 10000))
	int ClimbAngle = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 0, ClampMax = 100))
	int SplinePointClimbStart = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 1, ClampMax = 100))
	int SplinePointClimbEnd = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 1000, ClampMax = 1000000))
	int Radius = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 1.0f, ClampMax = 45.0f))
	float DegreeStep = 0.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spline", meta=(ClampMin = 1, ClampMax = 100))
	int PreSetRandomForm;

	UPROPERTY(VisibleAnywhere)
	float RandomForm;

	UFUNCTION()
	void OnOverlapStartBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// overlap end function
	// UFUNCTION()
	// void OnOverlapStartEnd(class AActor* OverlappedActor, class AActor* OtherActor);


	virtual void OnConstruction(const FTransform& Transform) override;

	void GenerateSplinePoints();
	void MoveSplinePointsToCycle();
	void ClimbSplinePoints();
	void GenerateStaticMesh();
	float GetRandomForm();
	FVector GetFinish();
	FRotator GetRotation();
};
