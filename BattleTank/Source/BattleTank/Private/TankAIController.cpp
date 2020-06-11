// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"
#include "Tank.h"  // Inmplement OnDeath
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

}

void ATankAIController::Tick(float DeltaTime)
{
    auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
    auto ControlledTank = GetPawn();

    Super::Tick( DeltaTime );

    // Find Player Tank and aim at it

    if (!ensure(PlayerTank && ControlledTank)) { return; }
    // Move Towards Player
    
    MoveToActor(
        PlayerTank,
        AcceptanceRadius
    );

    // Aim at Player
    FVector PlayerTankLocation = PlayerTank->GetActorLocation();

    auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
    AimingComponent->AimAt(PlayerTankLocation);

    // Fire if ready and aiming or locked  

    if (AimingComponent->GetFiringState() == EFiringStatus::Locked)
    {
        AimingComponent->Fire();
    }
 }



void ATankAIController::SetPawn(APawn* InPawn)
{
    
    Super::SetPawn(InPawn);

    if (InPawn)
    {
        auto PossessedTank = Cast<ATank>(InPawn);

        if (!ensure(PossessedTank)) { return; }

        // Subscribe our local method to tank's death event
        PossessedTank->OnTankDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
    }
}

void ATankAIController::OnTankDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("Tank AI Controller responding to OnDeath Broadcast"));
}