// Fill out your copyright notice in the Description page of Project Settings.


#include "Atom.h"
#include "BondBase.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AAtom::AAtom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PhysicsMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Physics Mesh"));
	if (PhysicsMesh) {
		SetRootComponent(PhysicsMesh);
		PhysicsMesh->GetBodyInstance()->bSimulatePhysics = true;
		PhysicsMesh->GetBodyInstance()->bEnableGravity = false;
		PhysicsMesh->SetLinearDamping(0.f);
		PhysicsMesh->SetAngularDamping(0.f);
	}


}

// Called when the game starts or when spawned
void AAtom::BeginPlay()
{
	Super::BeginPlay();
	
}

/*void AAtom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

FVector AAtom::InitialPush(float Strength) {
	float XStrength = FMath::RandRange(-Strength, Strength);
	float YStrength = FMath::RandRange(-Strength, Strength);
	float ZStrength = FMath::RandRange(-Strength, Strength);

	FVector Impulse = FVector(XStrength, YStrength, ZStrength);

	PhysicsMesh->AddImpulse(Impulse, NAME_None, false);

	return Impulse;

	return FVector(0);
}

void AAtom::AddBond(ABondBase* NewBond) {
	Bonds.Add(NewBond);
	AddToLinearDamping(LinearDampingPerBond);
}

void AAtom::RemoveBond(ABondBase* DeadBond) {
	Bonds.Remove(DeadBond);
	AddToLinearDamping(-LinearDampingPerBond);
}

void AAtom::AddToLinearDamping(float DeltaDamping) {
	FBodyInstance* BI = PhysicsMesh->GetBodyInstance();
	if (BI)
	{
		BI->LinearDamping += DeltaDamping;
		BI->UpdateDampingProperties();
	}
}
