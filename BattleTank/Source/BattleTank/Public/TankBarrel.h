// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 *  Tank Barrel
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	

public:
	void Elevate(float DegreesPerSecond);

private:
	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 20.f; // Default

	UPROPERTY(EditAnywhere)
	float MinElevationDeg = 0.f; // Default

	UPROPERTY(EditAnywhere)
	float MaxElevationDeg = 40.f; // Default


};
