// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"

ATank* ATankAIController::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}


void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

    if(!GetControlledTank())
    {
        UE_LOG(LogTemp, Warning, TEXT("AI Controller didn't get a Tank!"));
    }
    
    if (GetPlayerTank())
    {
        UE_LOG(LogTemp, Warning, TEXT("AI_Contoller Identified Player Tank %s"), *GetPlayerTank()->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AI_Controller didn't get a Player Tank!"));
    }
    


}

ATank* ATankAIController::GetPlayerTank() const
{
    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

    return Cast<ATank>(PlayerPawn);
}