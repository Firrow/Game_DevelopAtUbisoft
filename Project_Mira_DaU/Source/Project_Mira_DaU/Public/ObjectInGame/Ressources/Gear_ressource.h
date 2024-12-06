// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Ressource.h"
#include "Components/CapsuleComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Gear_ressource.generated.h"


class APlayerCharacter;

UCLASS()
class PROJECT_MIRA_DAU_API AGear_ressource : public ARessource
{
	GENERATED_BODY()
	
private:
	APlayerCharacter* PlayerCharacter = nullptr;

	virtual void RessourceEffect();

public:
	AGear_ressource();
	virtual void BeginPlay() override;

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
};
