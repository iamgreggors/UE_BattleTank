// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerContoller.h"

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