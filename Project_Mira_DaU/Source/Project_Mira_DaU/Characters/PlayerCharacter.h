// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MIRA_DAU_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
private:

public:

	UPROPERTY(EditAnywhere)
	FString name = "Mira";

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
