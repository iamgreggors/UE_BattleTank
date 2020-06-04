// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if(!Barrel) { return; }

	FVector OutLaunchVelocity(0);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	TArray < AActor * > ActorsToIgnore = TArray<AActor*>();
	
	bool FoundSolution = UGameplayStatics::SuggestProjectileVelocity(
											this,
											OutLaunchVelocity,
											StartLocation,
											HitLocation,
											LaunchSpeed,
											false,
											0.f,
											0.f,
											ESuggestProjVelocityTraceOption::DoNotTrace,
											FCollisionResponseParams::DefaultResponseParam,
											ActorsToIgnore,
											false  // Debug lines toggle
											);

	if(FoundSolution)
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();

		FString OurTankName = GetOwner()->GetName();
		auto Time = GetWorld()->GetTimeSeconds();
		
		MoveBarrelTowards( AimDirection );
		MoveTurretTowards( AimDirection );
	}
	else
	{
		
	}

}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// Compute current barrel rotation and aim direction
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotation;

	Barrel->Elevate(DeltaRotator.Pitch);  
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	Turret = TurretToSet;
}

void UTankAimingComponent::MoveTurretTowards(FVector AimDirection)
{
	// Compute current barrel rotation and aim direction
	auto TurretRotation = Turret->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - TurretRotation;

	Turret->Rotate(DeltaRotator.Yaw);  
}