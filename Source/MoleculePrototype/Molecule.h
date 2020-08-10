// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Molecule.generated.h"

UCLASS()
class MOLECULEPROTOTYPE_API AMolecule : public AActor
{
	GENERATED_BODY()
	
public:	
	AMolecule();
	//virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite)
	float LaunchStrength = 15000;

};
