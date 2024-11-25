// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Ressource.generated.h"

UCLASS()
class PROJECT_MIRA_DAU_API ARessource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARessource();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void RessourceEffect();

	UPROPERTY(VisibleAnywhere, Category = "Capsule")
	class UCapsuleComponent* ObjectCapsule;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
