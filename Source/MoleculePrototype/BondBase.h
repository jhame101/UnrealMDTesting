// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BondBase.generated.h"

class AAtom;
class UChildActorComponent;
class UPhysicsConstraintComponent;
class UPrimitiveComponent;

UCLASS()
class MOLECULEPROTOTYPE_API ABondBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ABondBase();

	//virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
	static void CreateBond(const UChildActorComponent* ChildAtomComponent1, const UChildActorComponent* ChildAtomComponent2, const TSubclassOf<ABondBase> BondClass, AAtom*& OutAtom1, AAtom*& OutAtom2, ABondBase*& OutBond);

	static void CreateBond(AAtom* Atom1, AAtom* Atom2, const TSubclassOf<ABondBase> BondClass, ABondBase*& OutBond);

	UFUNCTION(BlueprintCallable)
	bool AttachToAtoms(AAtom* NewAtom1, AAtom* NewAtom2);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* BondConstraint = nullptr;

	// TODO: include static mesh here

	UPROPERTY(VisibleAnywhere)
	AAtom* BondedAtom1 = nullptr;

	UPROPERTY(VisibleAnywhere)
	AAtom* BondedAtom2 = nullptr;

private:
	FTransform GetBondWorldTransform(const FVector& Position1, const FVector& Position2) const;

	UPROPERTY(EditDefaultsOnly)
	float BondLength = 100;

	UPROPERTY(EditDefaultsOnly)
	float BondStaticMeshLength = 100;

	const bool bTwistLocked = false;
	const float LinearStiffness = 300;
	const float AngularStiffness = 200;

};
