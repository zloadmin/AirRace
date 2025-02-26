// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadActor.h"


#include "../Cars/BaseSportsCarPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Engine/TriggerBox.h"

// Sets default values
ARoadActor::ARoadActor()
{
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetClosedLoop(false);
	if (Spline)
	{
		SetRootComponent(Spline);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Plane2.Shape_Plane2'"));
	if (MeshAsset.Object)
	{
		StaticMesh = MeshAsset.Object;
	}
}


void ARoadActor::OnOverlapStartBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ABaseSportsCarPawn::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("IGNORE ONE")));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("ASplineActor::OnOverlapStartBegin")));
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("SPAWN ONE")));
	}
}

void ARoadActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!StaticMesh) return;

	const FRoadActorParams Params;
	Generate(0, Params.SplinePoints, Params);
}

void ARoadActor::GenerateSplinePoints(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params)
{
	const FVector StartPointPos = Spline->GetLocationAtSplinePoint(StartIndex, ESplineCoordinateSpace::World);
	int Index = 0;
	for (int CurrentIndex = StartIndex; CurrentIndex <= FinishIndex + (Params.SplinePointsOffset * 2); CurrentIndex++)
	{
		if (CurrentIndex + 1 > Spline->GetNumberOfSplinePoints())
		// spline always generate with first two points 0 and 100  - don't touch it 
		{
			constexpr double Angle = 180.0f;
			const float Distance = Index * Params.SplineDistanceBetweenPoints;
			const double X = StartPointPos.X + Distance * UKismetMathLibrary::Cos(Params.CircleAngle * PI / Angle);
			const double Y = StartPointPos.Y + Distance * UKismetMathLibrary::Sin(Params.CircleAngle * PI / Angle);

			// FVector Point = FVector(CurrentIndex * DistanceBetweenPoints, 0, 0);
			FVector Point = FVector(X, Y, 0);
			Spline->AddSplinePointAtIndex(Point, CurrentIndex, ESplineCoordinateSpace::World, true);
		}
		Index++;
	}
	Spline->UpdateSpline();
}


void ARoadActor::GenerateStaticMesh(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(
	// 	                                 TEXT("ASplineActor::GenerateStaticMesh Start: %d Finish: %d"), StartIndex,
	// 	                                 FinishIndex));

	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	// const float RollStep = Roll > 0.0f ? Roll / Spline->GetNumberOfSplinePoints() : 0.0f;
	int StartIndexWithOffset = StartIndex == 0 ? StartIndex : StartIndex - 1;
	for (int CurrentIndex = StartIndexWithOffset; CurrentIndex < FinishIndex + (Params.SplinePointsOffset * 2);
	     CurrentIndex++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(
			this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(StaticMesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(World);
		SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);

		FVector StartPoint = Spline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
		FVector EndPoint = Spline->GetLocationAtSplinePoint(CurrentIndex + 1, ESplineCoordinateSpace::World);
		// UE_LOG(LogTemp, Warning, TEXT("%d - Start is %s. End is %s"), CurrentIndex, *StartPoint.ToString(),
		//        *EndPoint.ToString());

		const FVector StartTangent = Spline->GetTangentAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(CurrentIndex + 1, ESplineCoordinateSpace::World);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%d  - StartPoint is %s, EndPoint is %s "), SplineCount, *StartPoint.ToString(), *EndPoint.ToString()));
		FVector SplinePoint = Spline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
		// UE_LOG(LogTemp, Warning, TEXT("%d - SplinePoint is %s"), CurrentIndex, *SplinePoint.ToString());
		//SplineMeshComponent->SetForwardAxis(ForwardAxis);


		// TODO: disable rolling for first and last point
		// TODO: make double roll
		// SplineMeshComponent->SetStartRoll(RollStep * SplineCount);
		// SplineMeshComponent->SetEndRoll(RollStep * SplineCount + RollStep);

		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ARoadActor::GenerateTurn(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params)
{
	if (!Params.CircleRadius) return;
	// TODO: make as method getCenterOfRadius
	// 1 take startPoint

	const FVector StartPointPos = Spline->GetLocationAtSplinePoint(StartIndex + Params.SplinePointsOffset,
	                                                               ESplineCoordinateSpace::World);
	// 2 add radius by 

	// const FVector CenterOfRadius = FVector(StartPointPos.X, StartPointPos.Y - Radius, StartPointPos.Z);
	const FVector CenterOfRadius = GetCenterOfTurn(StartIndex + Params.SplinePointsOffset, Params);
	// UWorld* const World = GetWorld();
	// DrawDebugSphere(
	// 	World, // World context
	// 	CenterOfRadius, // Location of the sphere
	// 	Radius, // Radius of the sphere
	// 	12, // Segments for smoothness
	// 	FColor::Red, // Color of the sphere
	// 	true, // Whether the sphere should persist
	// 	999, // Time to display the sphere (0 means one frame)
	// 	0, // Depth priority (default is 0)
	// 	2.0f // Thickness of the lines
	// );
	//Spline->GetLocationAtSplinePoint(StartIndex + 2, ESplineCoordinateSpace::World);
	// const FVector CenterOfRadius(0, 0, 0);
	// UE_LOG(LogTemp, Warning, TEXT("CenterOfRadius is %s"), *CenterOfRadius.ToString());

	int Index = 0;
	for (int CurrentIndex = StartIndex; CurrentIndex <= FinishIndex + (Params.SplinePointsOffset * 2); CurrentIndex++)
	{
		// offset before - skip
		if (CurrentIndex - StartIndex < Params.SplinePointsOffset)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Turn offset before %d"), CurrentIndex);
			continue;
		}
		// offset after - add points in same angle & skip
		if (CurrentIndex - (FinishIndex + Params.SplinePointsOffset) > 0)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Turn offset after %d"), CurrentIndex);
			double const AngleBeforeOffset = GetAngleOfSplineIndexes(Params.SplinePointsOffset + FinishIndex - 1,
			                                                         Params.SplinePointsOffset + FinishIndex);
			const FVector PointBefore = Spline->GetLocationAtSplinePoint(
				CurrentIndex - 1, ESplineCoordinateSpace::World);
			constexpr double Angle = 180.0f;
			const double X = PointBefore.X + Params.SplineDistanceBetweenPoints * UKismetMathLibrary::Cos(
				AngleBeforeOffset * PI / Angle);
			const double Y = PointBefore.Y + Params.SplineDistanceBetweenPoints * UKismetMathLibrary::Sin(
				AngleBeforeOffset * PI / Angle);
			Spline->SetLocationAtSplinePoint(CurrentIndex, FVector(X, Y, 0), ESplineCoordinateSpace::World, true);
			continue;
		}
		// get start degree 

		// make a turn 
		const double Degree = (CurrentAngle + (90.0f * Params.CircleTurnSide)) - (Params.CircleTurnSide * Index) * Params.CircleDegreeStep;
		constexpr double Angle = 180.0f; // 1° × π/180 = 0.01745rad
		// x=x0+r⋅cos δ
		// UE_LOG(LogTemp, Warning, TEXT("X is %f + %i * %f"), CenterOfRadius.X, Radius, UKismetMathLibrary::Cos(Degree * PI / Angle));
		const double X = CenterOfRadius.X + Params.CircleRadius * UKismetMathLibrary::Cos(Degree * PI / Angle);
		// y=y0+r⋅sin δ
		// UE_LOG(LogTemp, Warning, TEXT("Y is %f + %i * %f"), CenterOfRadius.Y, Radius, UKismetMathLibrary::Sin(Degree*PI/Angle));
		const double Y = CenterOfRadius.Y + Params.CircleRadius * UKismetMathLibrary::Sin(Degree * PI / Angle);

		// if (IndexWithoutOffset == 0)
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Degree %f"), Degree);
		// 	UE_LOG(LogTemp, Warning, TEXT("Y is %f + %i * %f"), CenterOfRadius.Y, Radius,
		// 	       UKismetMathLibrary::Sin(Degree*PI/Angle));
		// 	UE_LOG(LogTemp, Warning, TEXT("Y is %f + %i * %f"), CenterOfRadius.Y, Radius, Degree*PI/Angle);
		// 	// UE_LOG(LogTemp, Warning, TEXT("X is %f + %i * %f"), CenterOfRadius.X, Radius, UKismetMathLibrary::Cos(Degree * PI / Angle));
		// 	// UE_LOG(LogTemp, Warning, TEXT("X is %f + %i * %f"), CenterOfRadius.X, Radius, Degree * PI / Angle);
		// }

		// UE_LOG(LogTemp, Warning, TEXT("Degree %f X: %i Y: %i"), Degree, X, Y);
		Spline->SetLocationAtSplinePoint(CurrentIndex, FVector(X, Y, 0), ESplineCoordinateSpace::World, true);
		// if (CurrentIndex == FinishIndex)
		// {
		// 	Spline->SetLocationAtSplinePoint(CurrentIndex, CenterOfRadius, ESplineCoordinateSpace::World, true);
		// } else
		// {
		// 	Spline->SetLocationAtSplinePoint(CurrentIndex, FVector(X, Y  + Radius, 0), ESplineCoordinateSpace::World, true);
		// }
		Index++;
	}
}

double ARoadActor::GetAngleOfSplineIndexes(const int FirstIndex, const int SecondIndex)
{
	const FVector FirstPoint = Spline->GetLocationAtSplinePoint(FirstIndex, ESplineCoordinateSpace::World);
	const FVector SecondPoint = Spline->GetLocationAtSplinePoint(SecondIndex, ESplineCoordinateSpace::World);
	const FVector Direction = SecondPoint - FirstPoint;
	const double AngleRadians = FMath::Atan2(Direction.Y, Direction.X);
	const double AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	return FMath::Fmod(AngleDegrees + 360.0, 360.0);
}

void ARoadActor::OnTriggerBoxBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->IsA(ABaseSportsCarPawn::StaticClass()))
	{
		// Radius = 10000;
		// SplinePoints = 10;
		// DegreeStep = 90 / SplinePoints;
		// TurnSide = TurnSide == -1.0f ? 1.0f : -1.0f;
		Add(FRoadActorParams());
	}
}

void ARoadActor::GenerateTriggerBox(const int FirstIndex, const int SecondIndex)
{
	UWorld* World = GetWorld();
	if (!World) return;
	const FVector LastPointPosition = Spline->GetLocationAtSplinePoint(SecondIndex, ESplineCoordinateSpace::World);
	const FRotator LastPointRotator = Spline->GetRotationAtSplinePoint(SecondIndex, ESplineCoordinateSpace::World);

	ATriggerBox* TriggerBox = World->SpawnActor<ATriggerBox>(ATriggerBox::StaticClass(), LastPointPosition,
	                                                         LastPointRotator);
	if (TriggerBox)
	{
		TriggerBox->SetActorScale3D(FVector(10, 10000, 10000));
		TriggerBox->OnActorBeginOverlap.AddDynamic(this, &ARoadActor::OnTriggerBoxBeginOverlap);
	}
}

void ARoadActor::SetCurrentAngle()
{
	CurrentAngle = GetAngleOfSplineIndexes(GetSplineLastIndex() - 1, GetSplineLastIndex());
}

FVector ARoadActor::GetCenterOfTurn(const int StartTurnIndex, const FRoadActorParams &Params)
{
	// FVector StartTurnPoint = Spline->GetLocationAtSplinePoint(StartTurnIndex, ESplineCoordinateSpace::World);
	// FVector BeforeStartTurnPoint = Spline->GetLocationAtSplinePoint(StartTurnIndex - 1, ESplineCoordinateSpace::World);
	double AngleLastTwoPoints = GetAngleOfSplineIndexes(StartTurnIndex - 1, StartTurnIndex);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(TEXT("AngleLastTwoPoints %f"), AngleLastTwoPoints));
	// AngleLastTwoPoints = TurnSide ? AngleLastTwoPoints - 90.0f : AngleLastTwoPoints + 90.0f;	
	AngleLastTwoPoints -= 90.0f;
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(TEXT("AngleLastTwoPoints + 90 %f"), AngleLastTwoPoints));
	AngleLastTwoPoints = FMath::Fmod(AngleLastTwoPoints + 360.f, 360.f);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(TEXT("AngleLastTwoPoints + 90 (360) %f"), AngleLastTwoPoints));
	constexpr double Angle = 180.0f;
	const FVector FirstPoint = Spline->GetLocationAtSplinePoint(StartTurnIndex, ESplineCoordinateSpace::World);
	const double X = FirstPoint.X + (Params.CircleRadius * Params.CircleTurnSide) * UKismetMathLibrary::Cos(AngleLastTwoPoints * PI / Angle);
	const double Y = FirstPoint.Y + (Params.CircleRadius * Params.CircleTurnSide) * UKismetMathLibrary::Sin(AngleLastTwoPoints * PI / Angle);
	return FVector(X, Y, FirstPoint.Z);
}


int ARoadActor::GetSplineLastIndex() const
{
	return Spline->GetNumberOfSplinePoints() - 1;
}

FVector ARoadActor::GetFinish()
{
	return Spline->GetLocationAtSplinePoint(GetSplineLastIndex(), ESplineCoordinateSpace::World);
}

FRotator ARoadActor::GetRotation()
{
	return Spline->GetRotationAtSplinePoint(GetSplineLastIndex(), ESplineCoordinateSpace::World);
}


void ARoadActor::Add(const FRoadActorParams& Params)
{
	const int StartIndex = GetSplineLastIndex() + 1;
	const int FinishIndex = GetSplineLastIndex() + Params.SplinePoints;
	Generate(StartIndex, FinishIndex, Params);
	//float Angle = GetAngleOfSplineIndexes(FinishIndex - 1, FinishIndex);
}


void ARoadActor::Generate(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params)
{
	if (StartIndex > 0) SetCurrentAngle();

	GenerateSplinePoints(StartIndex, FinishIndex, Params);
	GenerateTurn(StartIndex, FinishIndex, Params);
	GenerateStaticMesh(StartIndex, FinishIndex, Params);
	GenerateTriggerBox(StartIndex, FinishIndex);
}
