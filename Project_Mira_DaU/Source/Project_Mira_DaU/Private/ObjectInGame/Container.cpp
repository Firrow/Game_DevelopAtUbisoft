// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectInGame/Container.h"

// Sets default values
AContainer::AContainer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AContainer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AContainer::Effect() 
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("APPLIQUE L'EFFET !!!!"));
}