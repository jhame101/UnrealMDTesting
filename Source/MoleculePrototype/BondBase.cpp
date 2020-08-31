// Fill out your copyright notice in the Description page of Project Settings.


#include "BondBase.h"
#include "Atom.h"
#include "Components/ChildActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ABondBase::ABondBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BondConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Bond Constraint"));
	SetRootComponent(BondConstraint);
	if (BondConstraint) {
		//Setting which axes to lock, limit, or free
		BondConstraint->SetLinearXLimit(LCM_Limited, 0);
		BondConstraint->SetLinearYLimit(LCM_Locked, 0);
		BondConstraint->SetLinearZLimit(LCM_Locked, 0);
		BondConstraint->SetAngularSwing1Limit(ACM_Limited, 0);
		BondConstraint->SetAngularSwing2Limit(ACM_Limited, 0);
		BondConstraint->SetAngularTwistLimit((bTwistLocked) ? ACM_Locked : ACM_Free, 0);

		//make it a soft constraint so that it acts like a spring
		BondConstraint->ConstraintInstance.ProfileInstance.LinearLimit.bSoftConstraint = true;
		BondConstraint->ConstraintInstance.ProfileInstance.ConeLimit.bSoftConstraint = true;

		BondConstraint->ConstraintInstance.ProfileInstance.LinearLimit.Stiffness = LinearStiffness;
		BondConstraint->ConstraintInstance.ProfileInstance.ConeLimit.Stiffness = AngularStiffness;
		BondConstraint->ConstraintInstance.ProfileInstance.LinearLimit.Damping = 0;
		BondConstraint->ConstraintInstance.ProfileInstance.ConeLimit.Damping = 0;
	}
}

void ABondBase::BeginPlay()
{
	Super::BeginPlay();

}

void ABondBase::Destroyed() {
	Super::Destroyed();

	ReleaseAtoms();
}

/*void ABondBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

void ABondBase::CreateBond(AAtom* Atom1, AAtom* Atom2, const TSubclassOf<ABondBase> BondClass, ABondBase*& OutBond)
{
	if (!Atom1 || !Atom2) { return; }
	UWorld* World = GEngine->GetWorldFromContextObject(Atom1);
	OutBond = Cast<ABondBase>(World->SpawnActor(BondClass));
	OutBond->AttachToAtoms(Atom1, Atom2);
}

void ABondBase::CreateBond(const UChildActorComponent* ChildAtomComponent1, const UChildActorComponent* ChildAtomComponent2, const TSubclassOf<ABondBase> BondClass, AAtom*& OutAtom1, AAtom*& OutAtom2, ABondBase*& OutBond) {
	OutAtom1 = Cast<AAtom>(ChildAtomComponent1->GetChildActor());
	OutAtom2 = Cast<AAtom>(ChildAtomComponent2->GetChildActor());
	CreateBond(OutAtom1, OutAtom2, BondClass, OutBond);		//There is null checking in the other override
}

bool ABondBase::AttachToAtoms(AAtom* NewAtom1, AAtom* NewAtom2) {
	if (!ensure(BondConstraint) || !NewAtom1 || !NewAtom2) return false;
	UPrimitiveComponent* ConstrainedComponent1 = Cast<UPrimitiveComponent>(NewAtom1->GetRootComponent());
	UPrimitiveComponent* ConstrainedComponent2 = Cast<UPrimitiveComponent>(NewAtom2->GetRootComponent());
	if (!ConstrainedComponent1 || !ConstrainedComponent2) return false;

	ReleaseAtoms();
	BondedAtom1 = NewAtom1;
	BondedAtom2 = NewAtom2;
	SetActorTransform(GetBondWorldTransform(NewAtom1->GetActorLocation(), NewAtom2->GetActorLocation()));
	BondConstraint->ConstraintActor1 = NewAtom1;
	BondConstraint->ConstraintActor2 = NewAtom2;
	BondConstraint->SetConstrainedComponents(ConstrainedComponent1, FName::FName(), ConstrainedComponent2, FName::FName());

	BondConstraint->InitComponentConstraint();

	// Make it follow the atoms it's bonded to
	AttachToActor(NewAtom1, FAttachmentTransformRules::KeepWorldTransform);
	NewAtom1->AddBond(this);
	NewAtom2->AddBond(this);

	return true;

}

FTransform ABondBase::GetBondWorldTransform(const FVector& Position1, const FVector& Position2) const {
	FVector Position = (Position1 + Position2) / 2;
	FRotator Rotation = (Position2 - Position1).Rotation();
	FVector Scale = FVector(BondLength/BondStaticMeshLength, 1, 1);
	return FTransform(Rotation, Position, Scale);
}

void ABondBase::ReleaseAtoms()
{
	if (BondedAtom1) {
		BondedAtom1->RemoveBond(this);
	}
	if (BondedAtom2) {
		BondedAtom2->RemoveBond(this);
	}

	BondedAtom1 = nullptr;
	BondedAtom2 = nullptr;

	BondConstraint->BreakConstraint();
}
