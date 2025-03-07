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
	FRoadActorParams Params = FRoadActorParams::SimpleStraight();

	Generate(0, Params.GetSplinePoints(), Params);
}

void ARoadActor::GenerateSplinePoints(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params)
{
	const FVector StartPointPos = Spline->GetLocationAtSplinePoint(StartIndex, ESplineCoordinateSpace::World);
	int Index = 0;
	for (int CurrentIndex = StartIndex; CurrentIndex <= FinishIndex + (Params.GetSplinePointsOffset() * 2); CurrentIndex++)
	{
		if (CurrentIndex + 1 > Spline->GetNumberOfSplinePoints())
		// spline always generate with first two points 0 and 100  - don't touch it 
		{
			constexpr double Angle = 180.0f;
			const float Distance = Index * Params.GetSplineDistanceBetweenPoints();
			const double X = StartPointPos.X + Distance * UKismetMathLibrary::Cos(CurrentAngle * PI / Angle);
			const double Y = StartPointPos.Y + Distance * UKismetMathLibrary::Sin(CurrentAngle * PI / Angle);

			// FVector Point = FVector(CurrentIndex * DistanceBetweenPoints, 0, 0);
			FVector Point = FVector(X, Y, StartPointPos.Z);
			Spline->AddSplinePointAtIndex(Point, CurrentIndex, ESplineCoordinateSpace::World, true);
		}
		Index++;
	}
	Spline->UpdateSpline();
}

void ARoadActor::GenerateClimb(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params)
{
	if (!Params.GetClimbAngle()) return;

	int Index = 0;
	for (int CurrentIndex = StartIndex; CurrentIndex <= FinishIndex + (Params.GetSplinePointsOffset() * 2); CurrentIndex++)
	{
		// Skip offset before
		if (CurrentIndex - StartIndex < Params.GetSplinePointsOffset())
		{
			continue;
		}


		// // Skip offset after 
		// if (CurrentIndex - (FinishIndex + Params.GetSplinePointsOffset()) > 0)
		// {
		// 	double const AngleBeforeOffset = GetAngleOfSplineIndexes(Params.GetSplinePointsOffset() + FinishIndex - 1, Params.GetSplinePointsOffset() + FinishIndex);
		// 	const FVector PointBefore = Spline->GetLocationAtSplinePoint(CurrentIndex - 1, ESplineCoordinateSpace::World);
		// 	constexpr double Angle = 180.0f;
		// 	const double X = PointBefore.X + Params.GetSplineDistanceBetweenPoints() * UKismetMathLibrary::Cos(AngleBeforeOffset * PI / Angle);
		// 	const double Y = PointBefore.Y + Params.GetSplineDistanceBetweenPoints() * UKismetMathLibrary::Sin(AngleBeforeOffset * PI / Angle);
		// 	Spline->SetLocationAtSplinePoint(CurrentIndex, FVector(X, Y, 0), ESplineCoordinateSpace::World, true);
		// 	continue;
		// }
		const FVector CurrentPoint = Spline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
		Spline->SetLocationAtSplinePoint(CurrentIndex, FVector(CurrentPoint.X, CurrentPoint.Y, CurrentPoint.Z + (Params.GetClimbAngle() * Index)), ESplineCoordinateSpace::World, true);
		Index++;
	}
}


void ARoadActor::GenerateStaticMesh(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(
	// 	                                 TEXT("ASplineActor::GenerateStaticMesh Start: %d Finish: %d"), StartIndex,
	// 	                                 FinishIndex));

	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	// const float RollStep = Params.GetRoll() > 0.0f ? Params.GetRoll() / Spline->GetNumberOfSplinePoints() : 0.0f;
	int StartIndexWithOffset = StartIndex == 0 ? StartIndex : StartIndex - 1;
	int Index = 0;
	for (int CurrentIndex = StartIndexWithOffset; CurrentIndex < FinishIndex + (Params.GetSplinePointsOffset() * 2);
	     CurrentIndex++)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Current index %d. Is prefix %d"), CurrentIndex, IsOffsetSplineIndex(CurrentIndex, Params));

		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(
			this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(StaticMesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(World);
		SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);

		FVector StartPoint = Spline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
		FVector EndPoint = Spline->GetLocationAtSplinePoint(CurrentIndex + 1, ESplineCoordinateSpace::World);


		const FVector StartTangent = Spline->GetTangentAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(CurrentIndex + 1, ESplineCoordinateSpace::World);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%d  - StartPoint is %s, EndPoint is %s "), SplineCount, *StartPoint.ToString(), *EndPoint.ToString()));
		FVector SplinePoint = Spline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
		// UE_LOG(LogTemp, Warning, TEXT("%d - SplinePoint is %s"), CurrentIndex, *SplinePoint.ToString());
		//SplineMeshComponent->SetForwardAxis(ForwardAxis);

		GenerateStaticMeshRoll(Index, Params, SplineMeshComponent);

		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		++Index;
	}
}

void ARoadActor::GenerateStaticMeshRoll(const int Index, const FRoadActorParams& Params, USplineMeshComponent* SplineMeshComponent)
{
	// TODO: make as another function GenerateRoll. 
	// Roll linear type. first 0.0 and RollDegrees, Last RollDrees and zero
	if (IsNotOffsetSplineIndex(Index, Params) && Params.HasRoll())
	{
		auto const [StartRollDegree, EndRollDegree] = GetRollDegreeByIndex(Index, Params);
		UE_LOG(LogTemp, Warning, TEXT("%d - SplinePoint. Degree %f"), Index, StartRollDegree);
		UE_LOG(LogTemp, Warning, TEXT("%d - SplinePoint. Degree %f"), Index, EndRollDegree);
		SplineMeshComponent->SetStartRoll(FMath::DegreesToRadians(StartRollDegree));
		SplineMeshComponent->SetEndRoll(FMath::DegreesToRadians(EndRollDegree));
		// if (IsFirstNotOffsetSplineIndex(Index, Params))
		// {
		// 	// UE_LOG(LogTemp, Warning, TEXT("%d - SplinePoint. First"), Index);
		// 	SplineMeshComponent->SetStartRoll(FMath::DegreesToRadians(0.0f));
		// 	SplineMeshComponent->SetEndRoll(FMath::DegreesToRadians(Params.GetRoll()));
		// 	return;
		// }
		// if (IsLastNotOffsetSplineIndex(Index, Params))
		// {
		// 	// UE_LOG(LogTemp, Warning, TEXT("%d - SplinePoint. Last"), Index);
		// 	SplineMeshComponent->SetStartRoll(FMath::DegreesToRadians(Params.GetRoll()));
		// 	SplineMeshComponent->SetEndRoll(FMath::DegreesToRadians(0.0f));
		// 	return;
		// }
		// // UE_LOG(LogTemp, Warning, TEXT("%d - SplinePoint. Not First or Last"), Index);
		// SplineMeshComponent->SetStartRoll(FMath::DegreesToRadians(Params.GetRoll()));
		// SplineMeshComponent->SetEndRoll(FMath::DegreesToRadians(Params.GetRoll()));
		// return;

		// SplineMeshComponent->SetStartRoll(FMath::DegreesToRadians(Degree));
		// SplineMeshComponent->SetEndRoll(FMath::DegreesToRadians(Degree));
	}
}

void ARoadActor::GenerateTurn(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params)
{
	if (!Params.GetCircleRadius()) return;

	// 1 take startPoint
	const FVector StartPointPos = Spline->GetLocationAtSplinePoint(StartIndex + Params.GetSplinePointsOffset(), ESplineCoordinateSpace::World);
	// 2 add radius by
	// const FVector CenterOfRadius = FVector(StartPointPos.X, StartPointPos.Y - Radius, StartPointPos.Z);
	const FVector CenterOfRadius = GetCenterOfTurn(StartIndex + Params.GetSplinePointsOffset(), Params);

	UWorld* const World = GetWorld();
	DrawDebugSphere(
		World, // World context
		CenterOfRadius, // Location of the sphere
		Params.GetCircleRadius(), // Radius of the sphere
		12, // Segments for smoothness
		FColor::Red, // Color of the sphere
		true, // Whether the sphere should persist
		999, // Time to display the sphere (0 means one frame)
		0, // Depth priority (default is 0)
		2.0f // Thickness of the lines
	);
	//Spline->GetLocationAtSplinePoint(StartIndex + 2, ESplineCoordinateSpace::World);
	// const FVector CenterOfRadius(0, 0, 0);
	// UE_LOG(LogTemp, Warning, TEXT("CenterOfRadius is %s"), *CenterOfRadius.ToString());

	int Index = 0;
	for (int CurrentIndex = StartIndex; CurrentIndex <= FinishIndex + (Params.GetSplinePointsOffset() * 2); CurrentIndex++)
	{
		// offset before - skip
		if (CurrentIndex - StartIndex < Params.GetSplinePointsOffset())
		{
			continue;
		}
		const FVector CurrentPoint = Spline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
		// offset after - add points in same angle & skip
		if (CurrentIndex - (FinishIndex + Params.GetSplinePointsOffset()) > 0)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,FString::Printf(TEXT("Offset after %d"), CurrentIndex));
			double AngleBeforeOffset = GetAngleOfSplineIndexes(Params.GetSplinePointsOffset() + FinishIndex - 1, Params.GetSplinePointsOffset() + FinishIndex);
			if (Params.GetCircleAngleAfterRound() > 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("AngleBeforeOffset before %f"), AngleBeforeOffset));
				AngleBeforeOffset = FMath::RoundToDouble(AngleBeforeOffset / Params.GetCircleAngleAfterRound()) * Params.GetCircleAngleAfterRound();
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("AngleBeforeOffset after %f"), AngleBeforeOffset));
			}

			constexpr double Angle = 180.0f;
			const FVector PointBefore = Spline->GetLocationAtSplinePoint(CurrentIndex - 1, ESplineCoordinateSpace::World);
			const double X = PointBefore.X + Params.GetSplineDistanceBetweenPoints() * UKismetMathLibrary::Cos(AngleBeforeOffset * PI / Angle);
			const double Y = PointBefore.Y + Params.GetSplineDistanceBetweenPoints() * UKismetMathLibrary::Sin(AngleBeforeOffset * PI / Angle);
			Spline->SetLocationAtSplinePoint(CurrentIndex, FVector(X, Y, CurrentPoint.Z), ESplineCoordinateSpace::World, true); // TODO: change Z location
			continue;
		}

		const double DegreeStep = Params.GetCircleDegree() / (Params.GetSplinePoints());
		const double Degree = (CurrentAngle + (90.0f * Params.GetCircleTurnSide())) - (Params.GetCircleTurnSide() * Index) * DegreeStep;
		constexpr double Angle = 180.0f;
		const double X = CenterOfRadius.X + Params.GetCircleRadius() * UKismetMathLibrary::Cos(Degree * PI / Angle);
		const double Y = CenterOfRadius.Y + Params.GetCircleRadius() * UKismetMathLibrary::Sin(Degree * PI / Angle);

		Spline->SetLocationAtSplinePoint(CurrentIndex, FVector(X, Y, CurrentPoint.Z), ESplineCoordinateSpace::World, true);
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
		Add(FRoadActorParams::SimpleStraight());
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

FVector ARoadActor::GetCenterOfTurn(const int StartTurnIndex, const FRoadActorParams& Params)
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
	AngleLastTwoPoints = FMath::Fmod(AngleLastTwoPoints + 360.0, 360.0);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(TEXT("AngleLastTwoPoints + 90 (360) %f"), AngleLastTwoPoints));
	constexpr double Angle = 180.0f;
	const FVector FirstPoint = Spline->GetLocationAtSplinePoint(StartTurnIndex, ESplineCoordinateSpace::World);
	const double X = FirstPoint.X + (Params.GetCircleRadius() * Params.GetCircleTurnSide()) * UKismetMathLibrary::Cos(AngleLastTwoPoints * PI / Angle);
	const double Y = FirstPoint.Y + (Params.GetCircleRadius() * Params.GetCircleTurnSide()) * UKismetMathLibrary::Sin(AngleLastTwoPoints * PI / Angle);
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
	const int FinishIndex = GetSplineLastIndex() + Params.GetSplinePoints();
	Generate(StartIndex, FinishIndex, Params);
	//float Angle = GetAngleOfSplineIndexes(FinishIndex - 1, FinishIndex);
}


void ARoadActor::Generate(const int StartIndex, const int FinishIndex, const FRoadActorParams& Params)
{
	if (StartIndex > 0) SetCurrentAngle();

	GenerateSplinePoints(StartIndex, FinishIndex, Params);
	GenerateTurn(StartIndex, FinishIndex, Params);
	GenerateClimb(StartIndex, FinishIndex, Params);
	GenerateStaticMesh(StartIndex, FinishIndex, Params);
	GenerateTriggerBox(StartIndex, FinishIndex);
}

void ARoadActor::Generate(const int StartIndex, const int FinishIndex, TArray<const FRoadActorParams>& Params)
{
	int AddedIndex = 0;
	for (FRoadActorParams Param : Params)
	{
		Generate(StartIndex + AddedIndex, FinishIndex + AddedIndex, Param);
		AddedIndex = AddedIndex + Param.GetSplinePoints();
	}
}

FORCEINLINE bool ARoadActor::IsNotOffsetSplineIndex(const int Index, const FRoadActorParams& Params)
{
	return Index >= Params.GetSplinePointsOffset() && Index < (Params.GetSplinePointsOffset() + Params.GetSplinePoints());
}

FORCEINLINE bool ARoadActor::IsFirstNotOffsetSplineIndex(const int Index, const FRoadActorParams& Params)
{
	return Params.GetSplinePointsOffset() == Index;
}

FORCEINLINE bool ARoadActor::IsLastNotOffsetSplineIndex(const int Index, const FRoadActorParams& Params)
{
	return Params.GetSplinePointsOffset() + Params.GetSplinePoints() - 1 == Index;
}

FORCEINLINE bool ARoadActor::IsOffsetSplineIndex(const int Index, const FRoadActorParams& Params)
{
	return !IsNotOffsetSplineIndex(Index, Params);
}

TTuple<double, double> ARoadActor::GetRollDegreeByIndex(const int Index, const FRoadActorParams& Params)
{
	const int IndexWithoutOffset = Index - Params.GetSplinePointsOffset();
	const int HalfOfSplines = Params.GetSplinePoints() / 2;
	double DegreeStart = 0.0f;
	double DegreeEnd = 0.0f;
	const double DegreePerStep = Params.GetRoll() / HalfOfSplines;
	const int IndexFromZero = IndexWithoutOffset - HalfOfSplines;
	const int NegativeIndex = IndexWithoutOffset - (IndexFromZero * 2);
	
	if (Params.GetSplinePoints() % 2 == 0)
	{
		// First half
		if (IndexWithoutOffset < HalfOfSplines)
		{
			DegreeStart = IndexWithoutOffset * DegreePerStep;
			DegreeEnd = IndexWithoutOffset * DegreePerStep + DegreePerStep;
		}
		else if (IndexWithoutOffset >= HalfOfSplines)
		{
			DegreeStart = NegativeIndex * DegreePerStep;
			DegreeEnd = NegativeIndex * DegreePerStep - DegreePerStep;
		}
		
	} else
	{
		// First half
		if (IndexWithoutOffset < HalfOfSplines)
		{
			DegreeStart = IndexWithoutOffset * DegreePerStep;
			DegreeEnd = IndexWithoutOffset * DegreePerStep + DegreePerStep;
		}
		else if (IndexWithoutOffset > HalfOfSplines)
		{
			DegreeStart = NegativeIndex * DegreePerStep + DegreePerStep;
			DegreeEnd = NegativeIndex * DegreePerStep;
		}
		else
			// Center
		{
			DegreeStart = NegativeIndex * DegreePerStep;
			DegreeEnd = NegativeIndex * DegreePerStep;
		}
	}


	// if (IndexWithoutOffset < HalfOfSplines)
	// const int HalfOfSplines = Params.GetSplinePoints() / 2;
	// const double DegreePerStep = Params.GetRoll() / HalfOfSplines;
	// const int IndexFromZero = IndexWithoutOffset - HalfOfSplines;
	// const int NegativeIndex = IndexWithoutOffset - (IndexFromZero * 2);
	//
	// // if first half
	// if (IndexWithoutOffset < HalfOfSplines)
	// {
	// 	const double StartRollDegree = DegreePerStep * IndexWithoutOffset;
	// 	const double EndRollDegree =  DegreePerStep * IndexWithoutOffset + DegreePerStep;
	// 	return TTuple<double, double>(StartRollDegree, EndRollDegree);
	// }

	//
	// // const int HalfOfSplines = Params.GetSplinePoints() / 2;
	// // const int IndexWithoutOffset = Index - Params.GetSplinePointsOffset();
	//
	// // const int IndexFromZero = IndexWithoutOffset - HalfOfSplines;
	// // const int NegativeIndex = IndexWithoutOffset - (IndexFromZero * 2);
	// // const int MultiplyIndex = IndexWithoutOffset < HalfOfSplines ? IndexWithoutOffset : NegativeIndex;
	// // double DegreeStart = DegreePerStep * MultiplyIndex;
	// const double Degree = DegreePerStep * IndexWithoutOffset;
	// const int IndexWithoutOffset = (Index - Params.GetSplinePointsOffset()) * 2;
	// const double DegreePerStep = Params.GetRoll() / (Params.GetSplinePoints() - 1);
	// double DegreeStart;
	// double DegreeEnd;
	// const int IndexStart = IndexWithoutOffset;
	// const int IndexEnd = IndexWithoutOffset + 1;
	// const int IndexFromZero = IndexWithoutOffset - Params.GetSplinePoints();
	// const int NegativeIndex = IndexWithoutOffset - (IndexFromZero * 2 + 1);
	//
	//
	// if (IndexWithoutOffset < Params.GetSplinePoints())
	// {
	// 	// DegreeStart = (IndexStart * DegreePerStep) - DegreePerStep;
	// 	DegreeStart = IndexStart;
	// }
	// else
	// {
	// 	// DegreeStart = (NegativeIndex * DegreePerStep) - DegreePerStep;
	// 	DegreeStart = NegativeIndex;
	// }
	//
	// if (IndexWithoutOffset + 1 < Params.GetSplinePoints())
	// {
	// 	// DegreeEnd = IndexEnd * DegreePerStep;
	// 	DegreeEnd = IndexEnd;
	// }
	// else
	// {
	// 	// DegreeEnd = (NegativeIndex - 1) * DegreePerStep;
	// 	DegreeEnd = NegativeIndex - 1;
	// }

	return TTuple<double, double>(DegreeStart, DegreeEnd);
}
