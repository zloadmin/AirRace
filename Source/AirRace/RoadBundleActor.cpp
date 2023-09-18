// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadBundleActor.h"

#include "StraightRoadActor.h"

// Sets default values
ARoadBundleActor::ARoadBundleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// for(int i = 0; i < Amount; i++)
	// {
	
	AStraightRoadActor* StraightRoadActor = CreateDefaultSubobject<AStraightRoadActor>("Road1");
	StraightRoadActor->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		
	// }

}

// Called when the game starts or when spawned
void ARoadBundleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoadBundleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

