// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerContoller.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerContoller : public APlayerController
{
	GENERATED_BODY()

public:	

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float CrosshairXLocation = 0.5;

	UPROPERTY(EditAnywhere)
	float CrosshairYLocation = 0.3333333;

	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000.f;

private:
	ATank* GetControlledTank() const;
	void AimTowardCrosshair();
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

};
