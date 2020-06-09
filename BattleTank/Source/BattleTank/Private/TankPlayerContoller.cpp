// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerContoller.h"
#include "Engine/World.h"
#include "Math/Vector2D.h"
#include "TankAimingComponent.h"


void ATankPlayerContoller::BeginPlay()
{
    Super::BeginPlay();

    if (!GetPawn()) return;

    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (!ensure(AimingComponent)) { return; }
    FoundAimingComponent(AimingComponent);
}

void ATankPlayerContoller::Tick(float DeltaTime)
{
    Super::Tick( DeltaTime );

    AimTowardCrosshair();
}


void ATankPlayerContoller::AimTowardCrosshair()
{
    FVector HitLocation; // Out Parameter
    
    if (!GetPawn()) { return; }

    // Get World Location through Crosshair
    if( GetSightRayHitLocation( HitLocation ) )
    {
        auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
        AimingComponent->AimAt(HitLocation);
    }

    return;
}

bool ATankPlayerContoller::GetSightRayHitLocation(FVector& OutHitLocation) const
{
    // Get UI Location (Crosshair)
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);

    FVector2D ScreenLocation;
    ScreenLocation.X = ViewportSizeX * CrosshairXLocation;
    ScreenLocation.Y = ViewportSizeY * CrosshairYLocation;

    // De-project the crosshair

    FVector LookDirection;
    bool FoundLookDirection = GetLookDirection(ScreenLocation, LookDirection);

    // Raytrace along that line
    if (FoundLookDirection)
    {
        GetLookVectorHitLocation(LookDirection, OutHitLocation);
    }
    
    // See what we hit
    return true;
}


bool ATankPlayerContoller::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
    FVector WorldLocationTemp;
    return DeprojectScreenPositionToWorld(
        ScreenLocation.X,
        ScreenLocation.Y,
        WorldLocationTemp,
        LookDirection
    );
}

bool ATankPlayerContoller::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
    FHitResult HitResult;

    FVector StartLoc = PlayerCameraManager->GetCameraLocation();
    FVector EndLoc = StartLoc + ( LookDirection * LineTraceRange );

    if( GetWorld()->LineTraceSingleByChannel(
            HitResult,
            StartLoc,
            EndLoc,
            ECollisionChannel::ECC_Visibility)
      )
    {
        // Set HitLocation 
        HitLocation = HitResult.Location;
        return true;
    }
    HitLocation = FVector(0);
    return false;
    
}