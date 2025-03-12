// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleAIController.h"

#include "ChaosVehicleMovementComponent.h"
#include "NavigationSystem.h"
#include "AirRace/AirRaceGameState.h"
#include "AirRace/Cars/BaseSportsCarPawn.h"

AVehicleAIController::AVehicleAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVehicleAIController::BeginPlay()
{
	Super::BeginPlay();
	MoveToTarget();
}

void AVehicleAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MoveToTarget();
}

void AVehicleAIController::MoveToTarget()
{
	ABaseSportsCarPawn* VehiclePawn = Cast<ABaseSportsCarPawn>(GetPawn());
	if (!VehiclePawn) return;
	AAirRaceGameState* GameState = Cast<AAirRaceGameState>(GetWorld()->GetGameState());
	if (!GameState) return;
	ARoadActor* RoadActor = GameState->GetRoadActor();
	if (!RoadActor || !RoadActor->IsExists()) return;
	FVector TargetSplinePointLocation = RoadActor->Spline->GetLocationAtSplinePoint(TargetSplinePointIndex, ESplineCoordinateSpace::World);
	FVector Difference = VehiclePawn->GetActorLocation() - TargetSplinePointLocation;
	// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("MoveToTarget Difference Length %f"), Difference.Length()));
	FVector TargetLocation;
	 
	if (Difference.Length() < 300.0f)
	{
		TargetSplinePointIndex += 1;
		TargetLocation = GameState->GetRoadSplinePointLocation(TargetSplinePointIndex);
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("MoveToTarget %s"), *TargetLocation.ToString()));
	}
	else
	{
		TargetLocation = TargetSplinePointLocation;
	}
	TargetLocation.Z = 1017.0f;

	// ChatGPT code
	FVector MoveDirection = (TargetLocation - VehiclePawn->GetActorLocation()).GetSafeNormal();
	float ForwardValue = FVector::DotProduct(VehiclePawn->GetActorForwardVector(), MoveDirection);
	float SteeringValue = FVector::CrossProduct(VehiclePawn->GetActorForwardVector(), MoveDirection).Z;

	// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("MoveToTarget %s"), *TargetLocation.ToString()));

	UChaosVehicleMovementComponent* UChaosVehicleMovementComponent = VehiclePawn->GetVehicleMovement(); //->SetThrottleInput(ForwardValue);
	UChaosVehicleMovementComponent->SetThrottleInput(ForwardValue);
	UChaosVehicleMovementComponent->SetSteeringInput(SteeringValue);

	// VehiclePawn->GetVehicleMovement()->SetSteeringInput(SteeringValue);
	// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("MoveToTarget")));
	// APawn* ControlledPawn = GetPawn();
	// if (!ControlledPawn) return;
	// UWorld* World = GetWorld();
	// if (!World) return;
	// UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
	// if (NavSystem)
	// {
	// 	// x 5000.f y -18.0f z 1017.0f 
	// 	// FVector Destination = NavSystem->GetRandomReachablePointInRadius(ControlledPawn->GetActorLocation(), 2000.0f);
	// 	// MoveToLocation(Destination);
	// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("MoveToTarget")));
	// }
}
