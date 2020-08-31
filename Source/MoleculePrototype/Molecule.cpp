// Fill out your copyright notice in the Description page of Project Settings.


#include "Molecule.h"
#include "Atom.h"
#include "Components/ChildActorComponent.h"
#include "Math/UnrealMathUtility.h"

AMolecule::AMolecule()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AMolecule::BeginPlay()
{

	Super::BeginPlay();

	if (!FMath::IsNearlyZero(LaunchStrength)) {
		AAtom* FirstAtom = nullptr;
		TArray<UChildActorComponent*> ChildActors;
		GetComponents<UChildActorComponent>(ChildActors, false);
		for (UChildActorComponent* ChildActor : ChildActors) {
			FirstAtom = Cast<AAtom>(ChildActor->GetChildActor());
			if (FirstAtom) {
				FirstAtom->InitialPush(LaunchStrength);
				break;
			}
		}
	}
	
}

/*void AMolecule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

