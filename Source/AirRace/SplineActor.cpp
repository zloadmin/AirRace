// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"

#include "AirRaceCharacter.h"
#include "AirRaceGameState.h"
#include "BaseSportsCarPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMesh.h"
#include "Logging/StructuredLog.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Engine/TriggerBox.h"

// Sets default values
ASplineActor::ASplineActor()
{
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetClosedLoop(false);
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
	static ConstructorHelpers::FObjectFinder<UMaterial> DebugMaterialAssert(
		TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_AssetPlatform.M_AssetPlatform'"));
	if (DebugMaterialAssert.Object)
	{
		DebugMaterial = DebugMaterialAssert.Object;
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

void ASplineActor::OnOverlapStartBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
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

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!StaticMesh) return;
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ASplineActor::OnConstruction")));
	// MoveSplinePointsToCycle();
	// ClimbSplinePoints();
	Generate(0, SplinePoints);
}

void ASplineActor::GenerateSplinePoints(const int StartIndex, const int FinishIndex)
{
	// TArray<FVector> Points;
	// Spline->ClearSplinePoints(true); // TODO: do we need it?
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(
	// 	                                 TEXT("ASplineActor::GenerateSplinePoints Start: %d Finish: %d"), StartIndex,
	// 	                                 FinishIndex));

	const FVector StartPointPos = Spline->GetLocationAtSplinePoint(StartIndex, ESplineCoordinateSpace::World);
	int Index = 0;
	for (int CurrentIndex = StartIndex; CurrentIndex <= FinishIndex + (SplinePointsOffset * 2); CurrentIndex++)
	{
		if (CurrentIndex + 1 > Spline->GetNumberOfSplinePoints())
		// spline always generate with first two points 0 and 100  - don't touch it 
		{
			constexpr float Angle = 180.0f;
			const float Distance = Index * DistanceBetweenPoints;
			const float X = StartPointPos.X + Distance * UKismetMathLibrary::Cos(CurrentAngle * PI / Angle);
			const float Y = StartPointPos.Y + Distance * UKismetMathLibrary::Sin(CurrentAngle * PI / Angle);

			// FVector Point = FVector(CurrentIndex * DistanceBetweenPoints, 0, 0);
			FVector Point = FVector(X, Y, 0);

			UE_LOG(LogTemp, Warning, TEXT("ASplineActor::GenerateSplinePoints is %d - %s"), CurrentIndex,
			       *Point.ToString());
			Spline->AddSplinePointAtIndex(Point, CurrentIndex, ESplineCoordinateSpace::World, true);
		}
		Index++;
	}
	Spline->UpdateSpline();
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
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(
	// 	                                 TEXT("ASplineActor::GenerateStaticMesh Start: %d Finish: %d"), StartIndex,
	// 	                                 FinishIndex));

	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	// const float RollStep = Roll > 0.0f ? Roll / Spline->GetNumberOfSplinePoints() : 0.0f;
	int StartIndexWithOffset = StartIndex == 0 ? StartIndex : StartIndex - 1;
	for (int CurrentIndex = StartIndexWithOffset; CurrentIndex < FinishIndex + (SplinePointsOffset * 2); CurrentIndex++)
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
		UE_LOG(LogTemp, Warning, TEXT("%d - Start is %s. End is %s"), CurrentIndex, *StartPoint.ToString(),
		       *EndPoint.ToString());

		const FVector StartTangent = Spline->GetTangentAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(CurrentIndex + 1, ESplineCoordinateSpace::World);

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

void ASplineActor::GenerateTurn(const int StartIndex, const int FinishIndex)
{
	if (!Radius) return;
	// TODO: make as method getCenterOfRadius
	// 1 take startPoint

	const FVector StartPointPos = Spline->GetLocationAtSplinePoint(StartIndex + SplinePointsOffset,
	                                                               ESplineCoordinateSpace::World);
	// 2 add radius by 

	// const FVector CenterOfRadius = FVector(StartPointPos.X, StartPointPos.Y - Radius, StartPointPos.Z);
	const FVector CenterOfRadius = GetCenterOfTurn(StartIndex + SplinePointsOffset);
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
	for (int CurrentIndex = StartIndex; CurrentIndex <= FinishIndex + (SplinePointsOffset * 2); CurrentIndex++)
	{
		// offset before - skip
		if (CurrentIndex - StartIndex < SplinePointsOffset)
		{
			UE_LOG(LogTemp, Warning, TEXT("Turn offset before %d"), CurrentIndex);
			continue;
		}
		// offset after - add points in same angle & skip
		if (CurrentIndex - (FinishIndex + SplinePointsOffset) > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Turn offset after %d"), CurrentIndex);
			float AngleBeforeOffset = GetAngleOfSplineIndexes(SplinePointsOffset + FinishIndex - 1,
			                                                  SplinePointsOffset + FinishIndex);
			const FVector PointBefore = Spline->GetLocationAtSplinePoint(
				CurrentIndex - 1, ESplineCoordinateSpace::World);
			constexpr float Angle = 180.0f;
			const float X = PointBefore.X + DistanceBetweenPoints * UKismetMathLibrary::Cos(
				AngleBeforeOffset * PI / Angle);
			const float Y = PointBefore.Y + DistanceBetweenPoints * UKismetMathLibrary::Sin(
				AngleBeforeOffset * PI / Angle);
			Spline->SetLocationAtSplinePoint(CurrentIndex, FVector(X, Y, 0), ESplineCoordinateSpace::World, true);
			continue;
		}
		// get start degree 

		// make a turn 
		const float Degree = (CurrentAngle + (90.0f * TurnSide)) - (TurnSide * Index) * DegreeStep;
		constexpr float Angle = 180.0f; // 1° × π/180 = 0.01745rad
		// x=x0+r⋅cos δ
		// UE_LOG(LogTemp, Warning, TEXT("X is %f + %i * %f"), CenterOfRadius.X, Radius, UKismetMathLibrary::Cos(Degree * PI / Angle));
		const float X = CenterOfRadius.X + Radius * UKismetMathLibrary::Cos(Degree * PI / Angle);
		// y=y0+r⋅sin δ
		// UE_LOG(LogTemp, Warning, TEXT("Y is %f + %i * %f"), CenterOfRadius.Y, Radius, UKismetMathLibrary::Sin(Degree*PI/Angle));
		const float Y = CenterOfRadius.Y + Radius * UKismetMathLibrary::Sin(Degree * PI / Angle);

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

float ASplineActor::GetAngleOfSplineIndexes(const int FirstIndex, const int SecondIndex)
{
	const FVector FirstPoint = Spline->GetLocationAtSplinePoint(FirstIndex, ESplineCoordinateSpace::World);
	const FVector SecondPoint = Spline->GetLocationAtSplinePoint(SecondIndex, ESplineCoordinateSpace::World);
	const FVector Direction = SecondPoint - FirstPoint;
	const float AngleRadians = FMath::Atan2(Direction.Y, Direction.X);
	const float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	return FMath::Fmod(AngleDegrees + 360.f, 360.f);
}

void ASplineActor::OnTriggerBoxBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor->IsA(ABaseSportsCarPawn::StaticClass()))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("IGNORE ONE")));
		return;
	}
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Generate TURN")));
	Radius = 10000;
	SplinePoints = 10;
	DegreeStep = 90 / SplinePoints;
	TurnSide = TurnSide == -1.0f ? 1.0f : -1.0f;
	Add();
	// if ( FMath::RandRange(0.0f, 1.0f) > 0.5)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Generate TURN left")));
	// 	Radius = 10000;
	// 	SplinePoints = 10;
	// 	DegreeStep = 90 / SplinePoints;
	// 	TurnSide = 1.0f;
	// 	Add();
	// } else {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Generate TURN left")));
	// 	Radius = 10000;
	// 	SplinePoints = 10;
	// 	DegreeStep = 90 / SplinePoints;
	// 	TurnSide = -1.0f;
	// 	Add();
	// }
}

void ASplineActor::GenerateTriggerBox(const int FirstIndex, const int SecondIndex)
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
		TriggerBox->OnActorBeginOverlap.AddDynamic(this, &ASplineActor::OnTriggerBoxBeginOverlap);
	}


	// UBoxComponent* TriggerBox = NewObject<UBoxComponent>(World, TEXT("TriggerBox"));
	// if (!TriggerBox) return;
	//
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Triger BOX!!!")));
	// const FVector LastPointPosition = Spline->GetLocationAtSplinePoint(SecondIndex, ESplineCoordinateSpace::World);
	// TriggerBox->RegisterComponent();
	// TriggerBox->SetWorldLocation(FVector(LastPointPosition.X, LastPointPosition.Y, LastPointPosition.Z));
	// // Set desired position
	// TriggerBox->SetBoxExtent(FVector(10, 10000, 10000), true); // Set size
	// TriggerBox->SetHiddenInGame(false);
	// TriggerBox->SetVisibility(true);
	// TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// TriggerBox->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	// TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASplineActor::OnOverlapStartBegin);
	// if (DebugMaterial)
	// {
	// 	TriggerBox->SetMaterial(0, DebugMaterial);
	// }


	// TriggerBox->SetBoxExtent(FVector(10, 1000, 1000), true);
	// TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASplineActor::OnOverlapStartBegin);
	// TriggerBox->SetHiddenInGame(false); // Ensure it's not hidden in the game
	// TriggerBox->bHiddenInGame = false;  // Deprecated, but sometimes useful in older versions
	//
	// const FVector LastPointPosition = Spline->GetLocationAtSplinePoint(SecondIndex, ESplineCoordinateSpace::World);
	// const FRotator LastPointRotator = Spline->GetRotationAtSplinePoint(SecondIndex, ESplineCoordinateSpace::World);
	// TriggerBox->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//
	// UWorld* World = GetWorld();
	// if (World)
	// {
	// 	const FActorSpawnParameters SpawnInfo;
	// 	World->SpawnActor<UBoxComponent>(TriggerBox->GetClass(), LastPointPosition, LastPointRotator, SpawnInfo);
	// } else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("NO Word")));
	// }

	// BoxComponent->SetBoxExtent(FVector(10, 1000, 1000), true);
	// BoxComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	// BoxComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	// BoxComponent->SetRelativeLocation(FVector(10, 0, 320));
	// FVector LastPointPosition = Spline->GetLocationAtSplinePoint(SecondIndex, ESplineCoordinateSpace::World);
	// TODO: odd offset of LastPointPosition;
	// BoxComponent->SetWorldLocation(LastPointPosition);
	// BoxComponent->SetGenerateOverlapEvents(true);
	// BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASplineActor::OnOverlapStartBegin);
}

void ASplineActor::SetCurrentAngle()
{
	CurrentAngle = GetAngleOfSplineIndexes(GetSplineLastIndex() - 1, GetSplineLastIndex());
}

FVector ASplineActor::GetCenterOfTurn(const int StartTurnIndex)
{
	// FVector StartTurnPoint = Spline->GetLocationAtSplinePoint(StartTurnIndex, ESplineCoordinateSpace::World);
	// FVector BeforeStartTurnPoint = Spline->GetLocationAtSplinePoint(StartTurnIndex - 1, ESplineCoordinateSpace::World);
	float AngleLastTwoPoints = GetAngleOfSplineIndexes(StartTurnIndex - 1, StartTurnIndex);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(TEXT("AngleLastTwoPoints %f"), AngleLastTwoPoints));
	// AngleLastTwoPoints = TurnSide ? AngleLastTwoPoints - 90.0f : AngleLastTwoPoints + 90.0f;	
	AngleLastTwoPoints -= 90.0f;
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(TEXT("AngleLastTwoPoints + 90 %f"), AngleLastTwoPoints));
	AngleLastTwoPoints = FMath::Fmod(AngleLastTwoPoints + 360.f, 360.f);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	//                                  FString::Printf(TEXT("AngleLastTwoPoints + 90 (360) %f"), AngleLastTwoPoints));
	constexpr float Angle = 180.0f;
	FVector FirstPoint = Spline->GetLocationAtSplinePoint(StartTurnIndex, ESplineCoordinateSpace::World);
	const float X = FirstPoint.X + (Radius * TurnSide) * UKismetMathLibrary::Cos(AngleLastTwoPoints * PI / Angle);
	const float Y = FirstPoint.Y + (Radius * TurnSide) * UKismetMathLibrary::Sin(AngleLastTwoPoints * PI / Angle);
	return FVector(X, Y, FirstPoint.Z);
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
	return Spline->GetNumberOfSplinePoints() - 1;
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
	// const int StartIndex = GetSplineLastIndex() + 1;
	// const int FinishIndex = GetSplineLastIndex() + SplinePoints;
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ASplineActor::Add Start: %d Finish: %d"), StartIndex, FinishIndex));
	const int StartIndex = GetSplineLastIndex() + 1;
	const int FinishIndex = GetSplineLastIndex() + SplinePoints;
	Generate(StartIndex, FinishIndex);

	float Angle = GetAngleOfSplineIndexes(FinishIndex - 1, FinishIndex);

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Added ANGLE - %f"), Angle));
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
	if (StartIndex > 0) SetCurrentAngle();

	GenerateSplinePoints(StartIndex, FinishIndex);
	GenerateTurn(StartIndex, FinishIndex);
	GenerateStaticMesh(StartIndex, FinishIndex);
	GenerateTriggerBox(StartIndex, FinishIndex);
}
