// Fill out your copyright notice in the Description page of Project Settings.


#include "Atom.h"
#include "BondBase.h"
#include "Math/UnrealMathUtility.h"

AAtom::AAtom()
{
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
