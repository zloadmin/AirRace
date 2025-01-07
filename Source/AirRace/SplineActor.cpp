// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"

#include "AirRaceCharacter.h"
#include "AirRaceGameState.h"
#include "BaseSportsCarPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ASplineActor::ASplineActor()
{
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	if (Spline)
	{
		SetRootComponent(Spline);
	}
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Plane2.Shape_Plane2'"));

	if (MeshAsset.Object)
	{
		StaticMesh = MeshAsset.Object;
	}

	// BoxComponent = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	// BoxComponent->SetBoxExtent(FVector(10, 1000, 1000), true);
	// BoxComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	// // BoxComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	// // BoxComponent->SetRelativeLocation(FVector(10, 0, 320));
	// BoxComponent->SetRelativeLocation(FVector(10, 0, 0));
	// BoxComponent->SetGenerateOverlapEvents(true);
	// BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASplineActor::OnOverlapStartBegin);

	RandomForm = GetRandomForm();
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// void ASplineActor::OnOverlapStartBegin(AActor* Other)
// {
// }

// void ASplineActor::OnOverlapStartEnd(AActor* OverlappedActor, AActor* OtherActor)
// {
// }

// void ASplineActor::OnOverlapStartBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
//                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
//                                        const FHitResult& SweepResult)
// {
// 	if(!OtherActor->IsA(ABaseSportsCarPawn::StaticClass()))
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("IGNORE ONE")));
// 		return;
// 	}
// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("ASplineActor::OnOverlapStartBegin")));
// 	UWorld* const World = GetWorld();
// 	if (World != nullptr)
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("SPAWN ONE")));
// 		AAirRaceGameState* AirRaceGameState = World->GetGameState<AAirRaceGameState>();
// 		AirRaceGameState->GenerateNextRoad();
// 	}
// }

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!StaticMesh) return;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ASplineActor::OnConstruction")));
	// MoveSplinePointsToCycle();
	// ClimbSplinePoints();
	Generate(0, SplinePoints);
}

void ASplineActor::GenerateSplinePoints(const int StartIndex, const int FinishIndex)
{
	// TArray<FVector> Points;
	// Spline->ClearSplinePoints(true); // TODO: do we need it?
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	                                 FString::Printf(
		                                 TEXT("ASplineActor::GenerateSplinePoints Start: %d Finish: %d"), StartIndex,
		                                 FinishIndex));

	for (int CurrentIndex = StartIndex; CurrentIndex <= FinishIndex; CurrentIndex++)
	{
		FVector Point = FVector(CurrentIndex * DistanceBetweenPoints, 0, 0);
		UE_LOG(LogTemp, Warning, TEXT("Spline is %d - %s"), CurrentIndex, *Point.ToString());
		Spline->AddSplineLocalPoint(Point);
	}
}


void ASplineActor::MoveSplinePointsToCycle()
{
	if (!Radius) return;

	const FVector CenterOfRadius(0, 0, 0);
	// UE_LOG(LogTemp, Warning, TEXT("CenterOfRadius is %s"), *CenterOfRadius.ToString());

	for (int SplineCount = 0; SplineCount < Spline->GetNumberOfSplinePoints(); SplineCount++)
	{
		const float Degree = SplineCount * DegreeStep - 90.0f;
		float Angle = 180.0f;
		// x=x0+r⋅cos δ
		UE_LOG(LogTemp, Warning, TEXT("X is %f + %i * %f"), CenterOfRadius.X, Radius,
		       UKismetMathLibrary::Cos(Degree * PI / Angle));
		const int X = CenterOfRadius.X + Radius * UKismetMathLibrary::Cos(Degree * PI / Angle);
		// y=y0+r⋅sin δ
		UE_LOG(LogTemp, Warning, TEXT("Y is %f + %i * %f"), CenterOfRadius.Y, Radius,
		       UKismetMathLibrary::Sin(Degree*PI/Angle));
		const int Y = CenterOfRadius.Y + Radius * UKismetMathLibrary::Sin(Degree * PI / Angle);

		// UE_LOG(LogTemp, Warning, TEXT("Degree %f X: %i Y: %i"), Degree, X, Y);
		Spline->SetLocationAtSplinePoint(SplineCount, FVector(X, Y + Radius, 0), ESplineCoordinateSpace::Local, true);
	}
}

void ASplineActor::ClimbSplinePoints()
{
	return; // TODO: to back 
	if (!ClimbAngle) return;
	double Angle = 0;
	for (int SplineCount = 0; SplineCount < Spline->GetNumberOfSplinePoints(); SplineCount++)
	{
		if (SplineCount >= SplinePointClimbStart)
		{
			// If steel in period between points start and end add angle else keep current angle 
			if (SplineCount <= SplinePointClimbEnd)
			{
				Angle += ClimbAngle;
			}
			FVector CurrentPoint = Spline->GetLocationAtSplineInputKey(SplineCount, ESplineCoordinateSpace::World);
			FVector CurrentPointWithClimb = FVector(CurrentPoint.X, CurrentPoint.Y, CurrentPoint.Z + Angle);
			Spline->SetLocationAtSplinePoint(SplineCount, CurrentPointWithClimb, ESplineCoordinateSpace::World, true);
		}
	}
}

void ASplineActor::GenerateStaticMesh(const int StartIndex, const int FinishIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	                                 FString::Printf(
		                                 TEXT("ASplineActor::GenerateStaticMesh Start: %d Finish: %d"), StartIndex,
		                                 FinishIndex));

	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	// const float RollStep = Roll > 0.0f ? Roll / Spline->GetNumberOfSplinePoints() : 0.0f;
	for (int CurrentIndex = StartIndex; CurrentIndex <= FinishIndex; CurrentIndex++)
	{
		if (CurrentIndex == 0) continue;
		
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(
			this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(StaticMesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(World);
		SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);

		FVector StartPoint = Spline->GetLocationAtSplinePoint(CurrentIndex + 1, ESplineCoordinateSpace::World);
		FVector EndPoint = Spline->GetLocationAtSplinePoint(CurrentIndex + 2, ESplineCoordinateSpace::World);
		UE_LOG(LogTemp, Warning, TEXT("%d - Start is %s. End is %s"), CurrentIndex, *StartPoint.ToString(),
		       *EndPoint.ToString());

		const FVector StartTangent = Spline->GetTangentAtSplinePoint(CurrentIndex + 1, ESplineCoordinateSpace::World);
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(CurrentIndex + 2, ESplineCoordinateSpace::World);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%d  - StartPoint is %s, EndPoint is %s "), SplineCount, *StartPoint.ToString(), *EndPoint.ToString()));
		FVector SplinePoint = Spline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
		UE_LOG(LogTemp, Warning, TEXT("%d - SplinePoint is %s"), CurrentIndex, *SplinePoint.ToString());
		SplineMeshComponent->SetForwardAxis(ForwardAxis);


		// TODO: disable rolling for first and last point
		// TODO: make double roll
		// SplineMeshComponent->SetStartRoll(RollStep * SplineCount);
		// SplineMeshComponent->SetEndRoll(RollStep * SplineCount + RollStep);

		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

float ASplineActor::GetRandomForm()
{
	// if (PreSetRandomForm)
	// {
	// 	return PreSetRandomForm;
	// }
	return FMath::RandRange(0.0f, 1.0f);
}

int ASplineActor::GetSplineLastIndex()
{
	return Spline->GetNumberOfSplinePoints() - 3;
}

FVector ASplineActor::GetFinish()
{
	return Spline->GetLocationAtSplinePoint(GetSplineLastIndex(), ESplineCoordinateSpace::World);
}

FRotator ASplineActor::GetRotation()
{
	return Spline->GetRotationAtSplinePoint(GetSplineLastIndex(), ESplineCoordinateSpace::World);
}

void ASplineActor::LogSplinePointsLocations()
{
	// for (int SplineCount = 0; SplineCount < Spline->GetNumberOfSplinePoints(); SplineCount++)
	// 	FVector Location = Spline->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::World);
	// 	UE_LOG(LogTemp, Warning, TEXT("Spline %d location: %s"), SplineCount, *Location.ToString());
	// }
}

void ASplineActor::Add()
{
	const int StartIndex = GetSplineLastIndex() + 1;
	const int FinishIndex = GetSplineLastIndex() + SplinePoints;
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ASplineActor::Add Start: %d Finish: %d"), StartIndex, FinishIndex));
	Generate(GetSplineLastIndex() + 1, GetSplineLastIndex() + SplinePoints);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ADD")));
	// int addPoints = 10;
	// // add splines
	// FVector FinishPoint = GetFinish();
	// int FinishIndex = GetSplineLastIndex();
	// for (int i = 0; i < addPoints; i++)
	// {
	// 	FVector Point = FVector(FinishPoint.X + (i * DistanceBetweenPoints), 0, 0); // TODO: fix
	// 	Spline->AddSplineLocalPoint(Point);
	// }
	// // set meshes
	// GenerateStaticMesh(FinishIndex + 1, addPoints);
}

void ASplineActor::Generate(const int StartIndex, const int FinishIndex)
{
	GenerateSplinePoints(StartIndex, FinishIndex);
	GenerateStaticMesh(StartIndex, FinishIndex);
}
