// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASplineActor::ASplineActor()
{
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	if (Spline)
	{
		SetRootComponent(Spline);
	}
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

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!StaticMesh) return;
	
	GenerateSplinePoints();
	MoveSplinePointsToCycle();
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
	if(!Radius) return;
	
	const FVector CenterOfRadius(0 - Radius, 0, 0);
	UE_LOG(LogTemp, Warning, TEXT("CenterOfRadius is %s"), *CenterOfRadius.ToString());
	
	for (int SplineCount = 0; SplineCount < Spline->GetNumberOfSplinePoints(); SplineCount++)
	{
		const float Degree = SplineCount * DegreeStep;

		// x=x0+r⋅cos δ
		UE_LOG(LogTemp, Warning, TEXT("X is %f + %i * %f"), CenterOfRadius.X, Radius, UKismetMathLibrary::Cos(Degree * PI / 180.0));
		const int X = CenterOfRadius.X + Radius * UKismetMathLibrary::Cos(Degree * PI / 180.0);
		// y=y0+r⋅sin δ
		UE_LOG(LogTemp, Warning, TEXT("Y is %f + %i * %f"), CenterOfRadius.Y, Radius, UKismetMathLibrary::Sin(Degree*PI/180));
		const int Y = CenterOfRadius.Y + Radius * UKismetMathLibrary::Sin(Degree*PI/180);

		UE_LOG(LogTemp, Warning, TEXT("Degree %f X: %i Y: %i"), Degree, X, Y);
		Spline->SetLocationAtSplinePoint(SplineCount, FVector(X, Y, 0), ESplineCoordinateSpace::Local, true);
	}
}

void ASplineActor::GenerateStaticMesh()
{
	const float RollStep = Roll > 0.0f ? Roll / Spline->GetNumberOfSplinePoints() : 0.0f;
	for (int SplineCount = 0; SplineCount < (Spline->GetNumberOfSplinePoints() - 1); SplineCount++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(
			this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(StaticMesh);

		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);

		FVector StartPoint = Spline->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
		// StartPoint.Y = StartPoint.Y + 200;
		const FVector StartTangent = Spline->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
		FVector EndPoint = Spline->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);
		// EndPoint.Y = EndPoint.Y + 200;
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		SplineMeshComponent->SetForwardAxis(ForwardAxis);

		
		// TODO: disable rolling for first and last point
		// TODO: make double roll
		SplineMeshComponent->SetStartRoll(RollStep * SplineCount);
		SplineMeshComponent->SetEndRoll(RollStep * SplineCount + RollStep);
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

int ASplineActor::GetRandomForm()
{
	return FMath::RandRange(1, 99);
}
