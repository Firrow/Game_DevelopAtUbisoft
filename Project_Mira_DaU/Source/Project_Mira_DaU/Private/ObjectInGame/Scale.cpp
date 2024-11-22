// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectInGame/Scale.h"

// Sets default values
AScale::AScale()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScale::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScale::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScale::Effect()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Applique l'effet : ECHELLE !"));
}

