// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectInGame/Interactible.h"

AInteractible::AInteractible()
{
	//PrimaryActorTick.bCanEverTick = true;

	ObjectTags.Add(TEXT("Interactible"));
}

void AInteractible::BeginPlay()
{
	Super::BeginPlay();
	
}

/*void AInteractible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

