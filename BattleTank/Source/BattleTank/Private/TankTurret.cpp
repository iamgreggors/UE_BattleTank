// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
    float MaxRelativeSpeed = 1.f;
    float ClampedRelativeSpeed = FMath::Clamp(RelativeSpeed, -MaxRelativeSpeed, MaxRelativeSpeed);

    auto RotationChange = ClampedRelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
    auto RawNewRotation = RelativeRotation.Yaw + RotationChange;   

    SetRelativeRotation(FRotator(0, RawNewRotation, 0));
}