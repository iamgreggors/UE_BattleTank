// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

}

void ATankAIController::Tick(float DeltaTime)
{
    auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
    auto ControlledTank = Cast<ATank>(GetPawn());

    Super::Tick( DeltaTime );

    // Find Player Tank and aim at it

    if(PlayerTank)
    {
        // Move Towards Player
        MoveToActor(
            PlayerTank,
            AcceptanceRadius
        );

        // Aim at Player
        FVector PlayerTankLocation = PlayerTank->GetActorLocation();
        ControlledTank->AimAt(PlayerTankLocation);

        // Fire if ready
        ControlledTank->Fire();


    }

}