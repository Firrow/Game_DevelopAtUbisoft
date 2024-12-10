// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Ressource.h"
#include "Components/CapsuleComponent.h"

#include "Game/GameManager.h"

#include "Watch_ressource.generated.h"

/**
 * 
 */
class AGameManager;
UCLASS()
class PROJECT_MIRA_DAU_API AWatch_ressource : public ARessource
{
	GENERATED_BODY()
	
private:

	int TIME_IN_WATCH = 5;

	virtual void RessourceEffect();

public:
	AWatch_ressource();
	virtual void BeginPlay() override;

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
};