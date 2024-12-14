
#include "Characters/PlayerCharacter.h"
#include "ObjectInGame/Ressources/Gear_ressource.h"

void AGear_ressource::BeginPlay()
{
    Super::BeginPlay();

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGear_ressource::BeginOverlap);
}

void AGear_ressource::RessourceEffect()
{
    PlayerCharacter->GearsNumber++;
}


void AGear_ressource::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // Overlap
    PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (PlayerCharacter != nullptr)
    {
        Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    }
}