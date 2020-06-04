// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
    //auto Time = GetWorld()->GetTimeSeconds();

    float MaxRelativeSpeed = 1.f;
    float ClampedRelativeSpeed = FMath::Clamp(RelativeSpeed, -MaxRelativeSpeed, MaxRelativeSpeed);

    auto ElevationChange = ClampedRelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
    auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;

    auto ClampedElevation = FMath::Clamp(RawNewElevation, MinElevationDeg, MaxElevationDeg);

    UE_LOG(LogTemp, Warning, TEXT("Setting Elevation to %f"), ClampedElevation);
    SetRelativeRotation(FRotator(ClampedElevation, 0, 0));
   
}