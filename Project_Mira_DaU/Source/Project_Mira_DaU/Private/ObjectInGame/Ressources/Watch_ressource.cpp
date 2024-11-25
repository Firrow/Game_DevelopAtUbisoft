
#include <Characters/PlayerCharacter.h>
#include "ObjectInGame/Ressources/Watch_ressource.h"


AWatch_ressource::AWatch_ressource()
{

}

void AWatch_ressource::BeginPlay()
{
    Super::BeginPlay();

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &AWatch_ressource::BeginOverlap);
}

void AWatch_ressource::RessourceEffect()
{
    //Coder l'effet de la ressource

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Je joue l'effet"));
}


void AWatch_ressource::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
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
        //Jouer effet + Détruire l'objet
        Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    }
}