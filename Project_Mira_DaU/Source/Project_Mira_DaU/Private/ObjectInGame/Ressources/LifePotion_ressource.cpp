
#include <Characters/PlayerCharacter.h>
#include "ObjectInGame/Ressources/LifePotion_ressource.h"

ALifePotion_ressource::ALifePotion_ressource()
{

}

void ALifePotion_ressource::BeginPlay()
{
    Super::BeginPlay();

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &ALifePotion_ressource::BeginOverlap);
}

void ALifePotion_ressource::RessourceEffect()
{
    //Coder l'effet de la ressource

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Je joue l'effet : LIFE POTION"));
}


void ALifePotion_ressource::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
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