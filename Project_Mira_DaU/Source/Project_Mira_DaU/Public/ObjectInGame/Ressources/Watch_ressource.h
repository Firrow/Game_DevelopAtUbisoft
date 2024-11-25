// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Ressource.h"
#include "Watch_ressource.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MIRA_DAU_API AWatch_ressource : public ARessource
{
	GENERATED_BODY()
	
private:
	virtual void RessourceEffect() override;
};
