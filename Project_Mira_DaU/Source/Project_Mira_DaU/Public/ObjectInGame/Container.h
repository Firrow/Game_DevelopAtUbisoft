// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Interactible.h"
#include "ObjectInGame/Ressource.h"
#include "Container.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MIRA_DAU_API AContainer : public AInteractible
{
	GENERATED_BODY()

private :

public :

	AContainer();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "Global Informations")
	TSubclassOf<ARessource> RessourceInside;
};
