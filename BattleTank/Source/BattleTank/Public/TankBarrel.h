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
	void Elevate(float RelativeSpeed);

private:
	UPROPERTY(EditAnywhere, Category = "Setup")
	float MaxDegreesPerSecond = 5.f; // Default

	UPROPERTY(EditAnywhere, Category = "Setup")
	float MinElevationDeg = 0.f; // Default

	UPROPERTY(EditAnywhere, Category = "Setup")
	float MaxElevationDeg = 40.f; // Default

};
