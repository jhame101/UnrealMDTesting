// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Atom.generated.h"

class ABondBase;

UCLASS()
class MOLECULEPROTOTYPE_API AAtom : public AActor
{
	GENERATED_BODY()
	
public:	
	AAtom();

	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FVector InitialPush(float Strength);

	void AddBond(ABondBase* NewBond);

	void RemoveBond(ABondBase* DeadBond);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PhysicsMesh = nullptr;

	void AddToLinearDamping(float DeltaDamping);

	TSet<ABondBase*> Bonds;

private:
	UPROPERTY(EditDefaultsOnly)
	float LinearDampingPerBond = -0.10;		//When an atom is bonded, it seems to constantly lose energy to the PhysicsConstraint that makes the bond. This negative 

};
