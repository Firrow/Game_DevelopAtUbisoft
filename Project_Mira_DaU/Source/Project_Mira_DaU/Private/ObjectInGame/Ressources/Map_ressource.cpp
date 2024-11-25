// Fill out your copyright notice in the Description page of Project Settings.

#include <Characters/PlayerCharacter.h>
#include "ObjectInGame/Ressources/Map_ressource.h"

AMap_ressource::AMap_ressource()
{

}

void AMap_ressource::BeginPlay()
{
    Super::BeginPlay();

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMap_ressource::BeginOverlap);
}

void AMap_ressource::RessourceEffect()
{
    //Coder l'effet de la ressource

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Je joue l'effet : MAP"));
}


void AMap_ressource::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // Overlap
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    }
}