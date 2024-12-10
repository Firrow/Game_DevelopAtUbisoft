// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Ressource.generated.h"


class UCapsuleComponent;

UCLASS()
class PROJECT_MIRA_DAU_API ARessource : public AActor
{
	GENERATED_BODY()
	
public:	
	ARessource();

protected:
	virtual void RessourceEffect();

	UPROPERTY(VisibleAnywhere, Category = "Capsule")
	UCapsuleComponent* ObjectCapsule;

public:	

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
