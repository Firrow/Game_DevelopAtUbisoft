
#include <Characters/PlayerCharacter.h>
#include "ObjectInGame/Ressources/Computer_ressource.h"

AComputer_ressource::AComputer_ressource()
{

}

void AComputer_ressource::BeginPlay()
{
    Super::BeginPlay();

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &AComputer_ressource::BeginOverlap);
}

void AComputer_ressource::RessourceEffect()
{
    //Coder l'effet de la ressource

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Je joue l'effet : COMPUTER"));
}


void AComputer_ressource::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
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