// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"
#include "SpawnPoint.h"
#include "SprungWheel.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
}

void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1.f, 1.f);
	DriveTrack(CurrentThrottle);

}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UTankTrack::DriveTrack(float Throttle)
{
	auto ForceApplied = Throttle * TrackMaxDrivingForce;
	
	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();

	for (ASprungWheel* Wheel : Wheels)
	{
		UE_LOG(LogTemp, Warning, TEXT("Applying force to wheel %f"), ForcePerWheel);

		Wheel->AddDrivingForce(ForcePerWheel);
	}
	
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> Wheels;

	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	
	
	for (USceneComponent* Child : Children)
	{
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild) continue;

		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);

		if (!SprungWheel) continue;
		
		Wheels.Add(SprungWheel);
	}

	return Wheels;
}