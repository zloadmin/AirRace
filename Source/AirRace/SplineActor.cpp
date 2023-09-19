// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"

#include "AirRaceCharacter.h"
#include "AirRaceGameState.h"
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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (MeshAsset.Object)
	{
		StaticMesh = MeshAsset.Object;
	}

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	BoxComponent->SetBoxExtent(FVector(10, 300, 300), true);
	BoxComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	// BoxComponent->SetRelativeLocation(FVector(10, 0, 320));
	BoxComponent->SetRelativeLocation(FVector(10, 0, 0));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASplineActor::OnOverlapStartBegin);

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

void ASplineActor::OnOverlapStartBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if(!OtherActor->IsA(AAirRaceCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("IGNORE ONE")));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("ASplineActor::OnOverlapStartBegin")));
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("SPAWN ONE")));
		AAirRaceGameState* AirRaceGameState = World->GetGameState<AAirRaceGameState>();
		AirRaceGameState->GenerateNextRoad();

		// AirRaceGameState-
	}
}

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!StaticMesh) return;
	MoveSplinePointsToCycle();
	ClimbSplinePoints();
	GenerateStaticMesh();
}

void ASplineActor::GenerateSplinePoints()
{
	for (int i = 0; i < SplinePoints; i++)
	{
		Spline->AddSplineLocalPoint(FVector(i * DistanceBetweenPoints + (DistanceBetweenPoints * 2), 0, 0));
	}
}


void ASplineActor::MoveSplinePointsToCycle()
{
	if (!Radius) return;

	const FVector CenterOfRadius(0 - Radius, 0, 0);
	UE_LOG(LogTemp, Warning, TEXT("CenterOfRadius is %s"), *CenterOfRadius.ToString());

	for (int SplineCount = 0; SplineCount < Spline->GetNumberOfSplinePoints(); SplineCount++)
	{
		const float Degree = SplineCount * DegreeStep;

		// x=x0+r⋅cos δ
		UE_LOG(LogTemp, Warning, TEXT("X is %f + %i * %f"), CenterOfRadius.X, Radius,
		       UKismetMathLibrary::Cos(Degree * PI / 180.0));
		const int X = CenterOfRadius.X + Radius * UKismetMathLibrary::Cos(Degree * PI / 180.0);
		// y=y0+r⋅sin δ
		UE_LOG(LogTemp, Warning, TEXT("Y is %f + %i * %f"), CenterOfRadius.Y, Radius,
		       UKismetMathLibrary::Sin(Degree*PI/180));
		const int Y = CenterOfRadius.Y + Radius * UKismetMathLibrary::Sin(Degree * PI / 180);

		UE_LOG(LogTemp, Warning, TEXT("Degree %f X: %i Y: %i"), Degree, X, Y);
		Spline->SetLocationAtSplinePoint(SplineCount, FVector(X, Y, 0), ESplineCoordinateSpace::World, true);
	}
}

void ASplineActor::ClimbSplinePoints()
{
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
			FVector CurrentPoint = Spline->GetLocationAtSplineInputKey(SplineCount, ESplineCoordinateSpace::Local);
			FVector CurrentPointWithClimb = FVector(CurrentPoint.X, CurrentPoint.Y, CurrentPoint.Z + Angle);
			Spline->SetLocationAtSplinePoint(SplineCount, CurrentPointWithClimb, ESplineCoordinateSpace::Local, true);
		}
	}
}

void ASplineActor::GenerateStaticMesh()
{
	UWorld* const World = GetWorld();

	if (World == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("WORD NOT EXISTS")));
		return;
	}
	if (!Spline->GetNumberOfSplinePoints())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("SPLINE NOT EXISTS")));
		return;
	}
	const float RollStep = Roll > 0.0f ? Roll / Spline->GetNumberOfSplinePoints() : 0.0f;
	for (int SplineCount = 0; SplineCount < (Spline->GetNumberOfSplinePoints() - 1); SplineCount++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(
			this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(StaticMesh);

		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(World);
		SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);

		FVector StartPoint = Spline->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::World);
		// StartPoint.Y = StartPoint.Y - 200; // TODO: Make minus half of static mesh
		const FVector StartTangent = Spline->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::World);
		FVector EndPoint = Spline->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::World);
		// EndPoint.Y = EndPoint.Y - 200; // TODO: Make minus half of static mesh
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::World);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
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

FVector ASplineActor::GetFinish()
{
	return Spline->GetLocationAtSplinePoint(Spline->GetSplineLength() - 1, ESplineCoordinateSpace::World);
}
