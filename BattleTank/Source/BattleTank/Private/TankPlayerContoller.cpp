// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "TankPlayerContoller.h"
#include "Math/Vector2D.h"


ATank* ATankPlayerContoller::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}


void ATankPlayerContoller::BeginPlay()
{
    Super::BeginPlay();

    if(GetControlledTank())
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerContoller Begin Play %s"), *GetControlledTank()->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Didn't get a Tank!"));
    }
    

}

void ATankPlayerContoller::Tick(float DeltaTime)
{
    Super::Tick( DeltaTime );

    AimTowardCrosshair();
}


void ATankPlayerContoller::AimTowardCrosshair()
{
    if(!GetControlledTank())
        return;

    FVector HitLocation; // Out Parameter
       
    // Get World Location through Crosshair
    if( GetSightRayHitLocation( HitLocation ) )
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitLocation.ToString());
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