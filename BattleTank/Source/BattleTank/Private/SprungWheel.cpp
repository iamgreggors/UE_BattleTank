// Fill out your copyright notice in the Description page of Project Settings.


#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "GameFramework/Actor.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;


	MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheelConstraint"));
	SetRootComponent(MassWheelConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(MassWheelConstraint);

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle);

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
	AxleWheelConstraint->SetupAttachment(Axle);

}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	
	Wheel->SetNotifyRigidBodyCollision(true);

	UE_LOG(LogTemp, Warning, TEXT("Spung Wheel %s Called Begin Play"), *GetName());
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);
	


	// Setup Constraints

	if (!GetAttachParentActor()) return;

	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
	if (!BodyRoot) return;

	MassWheelConstraint->SetConstrainedComponents
	(
		BodyRoot, NAME_None,
		Axle, NAME_None
	);

	AxleWheelConstraint->SetConstrainedComponents
	(
		Axle, NAME_None,
		Wheel, NAME_None
	);


}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		//UE_LOG(LogTemp, Warning, TEXT("In the right rate group"));
		ForceToApply = 0;
	}

}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	ForceToApply += ForceMagnitude;
	//ApplyForce = true;

}



void ASprungWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("T: %f -- Driving Wheel %s with Force %f ::: Collision Other Actor %s"), GetWorld()->RealTimeSeconds, *GetName(), ForceToApply, *OtherActor->GetName());

	//if (ApplyForce)
	//{
		Wheel->AddForce(Axle->GetForwardVector() * ForceToApply);
	//	ForceToApply = 0;
	//	ApplyForce = false;
	//}

}