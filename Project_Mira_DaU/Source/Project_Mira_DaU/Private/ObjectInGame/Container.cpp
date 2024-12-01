
#include "ObjectInGame/Container.h"
#include <Characters/PlayerCharacter.h>


AContainer::AContainer()
{
    CapsuleColliderRessource = CreateDefaultSubobject<USphereComponent>(TEXT("CapsuleRessource"));
    CapsuleColliderRessource->InitSphereRadius(5.f);
    CapsuleColliderRessource->SetupAttachment(RootComponent);

    RessourcePointSpawn = CreateDefaultSubobject<USphereComponent>(TEXT("RessourcePointSpawn"));
    RessourcePointSpawn->InitSphereRadius(1.f);
    RessourcePointSpawn->SetupAttachment(CapsuleColliderRessource);
}

void AContainer::Effect()
{
    if (!isEmpty)
    {
        
        if (RessourceInside.IsEmpty())
        {
            isEmpty = true;
        }
        else
        {
            //TODO : Lancer l'animation du coffre en BP

            FRotator Rotation(0.0f, 0.0f, 0.0f);
            GetWorld()->SpawnActor<ARessource>(RessourceInside.Last(), RessourcePointSpawn->GetComponentLocation(), Rotation);
            RessourceInside.Pop();
        }
    }
}