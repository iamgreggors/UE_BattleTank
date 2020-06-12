// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if(!ensure(Barrel)) { return; }

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
		AimDirection = OutLaunchVelocity.GetSafeNormal();

		FString OurTankName = GetOwner()->GetName();
		auto Time = GetWorld()->GetTimeSeconds();
		
		MoveBarrelTowards( AimDirection );
		MoveTurretTowards( AimDirection );
	}

}


void UTankAimingComponent::MoveBarrelTowards(FVector TargetAimDirection)
{
	if (!ensure(Barrel)) { return; }
	// Compute current barrel rotation and aim direction
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotation;

	Barrel->Elevate(DeltaRotator.Pitch);  
}


void UTankAimingComponent::MoveTurretTowards(FVector TargetAimDirection)
{
	if (!ensure(Turret)) { return; }

	// Compute current barrel rotation and aim direction
	auto TurretRotation = Turret->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - TurretRotation;

	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}


void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Turret = TurretToSet;
	Barrel = BarrelToSet;
}

EFiringStatus UTankAimingComponent::GetFiringState() const
{
	return FiringStatus;
}



void UTankAimingComponent::Fire()
{

	if (!ensure(Barrel && ProjectileBlueprint)) { return; }

	// This shouldn't happen for the player, but the AI Tank will try to fire when empty
	if (AmmoCount <= 0) { return; }

	// Spawn a project at the barrel's projectile socket location

	if (FiringStatus != EFiringStatus::Reloading)
	{

		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		AmmoCount--;
	}

}

int32 UTankAimingComponent::GetAmmoCount()
{
	return AmmoCount;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AmmoCount <= 0)
	{
		FiringStatus = EFiringStatus::Empty;
		return;
	}

	if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (!IsBarrelMoving())
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else
	{
		FiringStatus = EFiringStatus::Locked;
	}
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds();
}


bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	FVector ForwardDirection = Barrel->GetForwardVector();

	return AimDirection.Equals(ForwardDirection, 0.01f);
		
}