// Fill out your copyright notice in the Description page of Project Settings.


#include "AirRaceGameState.h"

#include "ClimbRoadActor.h"
#include "LongTurnRoadActor.h"
#include "StraightRoadActor.h"
#include "Math/UnrealMathUtility.h"

AAirRaceGameState::AAirRaceGameState()
{
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
	// 	TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Plane2.Shape_Plane2'"));
	// if (MeshAsset.Object)
	// {
	// 	RoadStaticMesh = MeshAsset.Object;
	// }


	// SetActors();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();


	// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("AAirRaceGameState::AAirRaceGameState")));
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
}

void AAirRaceGameState::GenerateNextRoad()
{
	// UWorld* const World = GetWorld();
	// if (World != nullptr)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Add road")));
	//
	// 	ASplineActor* SplineActor = World->SpawnActor<ASplineActor>(
	// 		GetRandomActor(),
	// 		GetRoadFinish(),
	// 		GetRoadRotation()
	// 	);
	// 	// Roads.Add(ResultPawn);
	// 	SetRoadFinish(SplineActor);
	// 	SetRoadRotation(SplineActor);
	// 	// SplineActor->LogSplinePointsLocations();
	// }
}

void AAirRaceGameState::InitRoad()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Init road")));

		Road = World->SpawnActor<ASplineActor>(
			ASplineActor::StaticClass(),
			FVector(0, 0, 0),
			FRotator(0, 0, 0)
		);
		// Roads.Add(ResultPawn);
		// SetRoadFinish(SplineActor);
		// SetRoadRotation(SplineActor);
		// SplineActor->LogSplinePointsLocations();
	}
}

void AAirRaceGameState::AddStraightRoad()
{
}


void AAirRaceGameState::BeginPlay()
{
	Super::BeginPlay();
	SetActors();
	InitRoad();
	Road->Add();
	Road->Add();
	Road->Add();
	Road->Add();
	Road->Add();
	
	// Road->Radius = 10000;
	// Road->SplinePoints = 10;
	// Road->DegreeStep = 90 / Road->SplinePoints;
	// Road->TurnSide = 1.0f;
	// Road->Add();
	//
	// Road->Radius = 10000;
	// Road->SplinePoints = 10;
	// Road->DegreeStep = 90 / Road->SplinePoints;
	// Road->TurnSide = 1.0f;
	// Road->Add();

	

	
	


	// for (int i = 0; i <= 10; i++)
	// {
	// 	Road->Radius = 10000;
	// 	Road->SplinePoints = 10;
	// 	Road->DegreeStep = 90 / Road->SplinePoints;
	// 	Road->TurnSide = i % 2 == 0 ? 1.0f : -1.0f;
	// 	Road->Add();
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Angle: %f"), Road->CurrentAngle));
	// 	Road->Radius = 0;
	// 	Road->SplinePoints = 10;
	// 	Road->DegreeStep = 0;
	// 	Road->Add();
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Angle: %f"), Road->CurrentAngle));
	// }

	
	
	// for (int i = 0; i <= 10; i++) {
	// 	Road->Radius = 0;
	// 	Road->SplinePoints = 10;
	// 	Road->DegreeStep = 0;
	// 	Road->Add();
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Angle: %f"), Road->CurrentAngle));
	//
	// 	Road->Radius = 10000 + (i * 10000);
	// 	Road->SplinePoints = 10;
	// 	Road->DegreeStep = 90 / Road->SplinePoints;
	// 	Road->Add();
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Angle: %f"), Road->CurrentAngle));
	// }
	// Road->Add();
	// Road->Add();
	// Road->Add();
	// Road->Add();
	// Road->Add();
	// Road->Add();
	// Road->Add();
	GEngine->Exec(GetWorld(), TEXT("stat fps"));
}

void AAirRaceGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FVector AAirRaceGameState::GetRoadFinish()
{
	return RoadFinish;
}

FRotator AAirRaceGameState::GetRoadRotation()
{
	return RoadRotation;
}

// void AAirRaceGameState::SetRoadFinish(ASplineActor* Actor)
// {
// 	RoadFinish = Actor->GetFinish();
// 	UE_LOG(LogTemp, Warning, TEXT("Set road finish %s"), *Actor->GetFinish().ToString());
// }
//
// void AAirRaceGameState::SetRoadRotation(ASplineActor* Actor)
// {
// 	RoadRotation = Actor->GetRotation();
// 	// RoadRotation = FRotator(Actor->GetRotation().Pitch, Actor->GetRotation().Yaw, Actor->GetRotation().Roll - 90.f);
// }

void AAirRaceGameState::SetActors()
{
	// Actors.Add(AStraightRoadActor::StaticClass());
	Actors.Add(AStraightRoadActor::StaticClass());
	// Actors.Add(ALongTurnRoadActor::StaticClass());
}

UClass* AAirRaceGameState::GetRandomActor()
{
	return Actors[FMath::RandRange(0, Actors.Num() - 1)];
}

void AAirRaceGameState::GenerateRoad(int Points)
{
	// first add points
	GenerateRoadSplinePoints(Points);

	// add static mesh
	// GenerateRoadSplineStaticMesh(Points);
}

void AAirRaceGameState::GenerateRoadSplinePoints(int Points)
{
	for (int i = 0; i < Points; i++)
	{
		FVector Point = FVector(i * DistanceBetweenPoints, 0, 0);
		RoadSplineComponent->AddSplineLocalPoint(Point);
	}
}

void AAirRaceGameState::GenerateRoadSplineStaticMesh(int Points)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	for (int Point = 0; Point < Points; Point++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(
			this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(RoadStaticMesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(World);
		SplineMeshComponent->AttachToComponent(RoadSplineComponent, FAttachmentTransformRules::KeepWorldTransform);
		FVector StartPoint = RoadSplineComponent->GetLocationAtSplinePoint(Point, ESplineCoordinateSpace::World);
		FVector EndPoint = RoadSplineComponent->GetLocationAtSplinePoint(Point + 1, ESplineCoordinateSpace::World);
		const FVector StartTangent = RoadSplineComponent->GetTangentAtSplinePoint(Point, ESplineCoordinateSpace::World);
		const FVector EndTangent = RoadSplineComponent->GetTangentAtSplinePoint(
			Point + 1, ESplineCoordinateSpace::World);
		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
