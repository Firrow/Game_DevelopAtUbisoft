// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Ressource.h"
#include "Components/CapsuleComponent.h"
#include "Computer_ressource.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MIRA_DAU_API AComputer_ressource : public ARessource
{
	GENERATED_BODY()
	
private:

	virtual void RessourceEffect();

public:
	AComputer_ressource();
	virtual void BeginPlay() override;

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
};
