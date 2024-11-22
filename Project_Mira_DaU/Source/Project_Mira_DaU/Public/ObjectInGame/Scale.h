// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectInGame/Interface/InteractibleInterface.h"
#include "Scale.generated.h"

UCLASS()
class PROJECT_MIRA_DAU_API AScale : public AActor, public IInteractibleInterface
{
	GENERATED_BODY()
	
private:	
	virtual void Effect() override;


public:	

	AScale();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
