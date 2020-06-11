// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

float ATank::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser
)
{
//	Super::TakeDamage(
		 //DamageAmount,
		 //DamageEvent,
		 //EventInstigator,
		 //DamageCauser
	//);

	UE_LOG(LogTemp, Warning, TEXT("caused damage: %f"), DamageAmount);

	// Clamp Damage
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 ClampedDamage = FMath::Clamp(DamagePoints, 0, CurrentHealth);
	
	// Apply Damage then clamp result
	
	CurrentHealth -= DamagePoints;

	if (CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tank Died: %s"), *GetName());
		OnTankDeath.Broadcast();
	}

	return ClampedDamage;
}


float ATank::GetHealthPercent()
{
	return (float)CurrentHealth / (float)StartingHealth;
}