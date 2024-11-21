// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Interactible.generated.h"


/// <summary>
/// This class has all element to create different types of interactible object
/// </summary>
UCLASS()
class PROJECT_MIRA_DAU_API AInteractible : public AActor
{
	GENERATED_BODY()
	
private :	
	
protected:

public:	

	AInteractible();
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Globales Informations")
	TArray<FString> ObjectTags;

	UPROPERTY(VisibleAnywhere, Category = "Capsule")
	class UCapsuleComponent* ObjectCapsule;

};
