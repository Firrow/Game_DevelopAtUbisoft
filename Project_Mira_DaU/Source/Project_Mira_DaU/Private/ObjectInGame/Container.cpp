
#include "Components/SphereComponent.h"
#include "ObjectInGame/Container.h"


AContainer::AContainer()
{
    CapsuleContainer = CreateDefaultSubobject<USphereComponent>(TEXT("CapsuleContainer"));
    CapsuleContainer->InitSphereRadius(25.f);
    CapsuleContainer->SetupAttachment(RootComponent);

    RessourcePointSpawn = CreateDefaultSubobject<USphereComponent>(TEXT("RessourcePointSpawn"));
    RessourcePointSpawn->InitSphereRadius(1.f);
    RessourcePointSpawn->AddLocalOffset(FVector(0, 0, 80));
    RessourcePointSpawn->SetupAttachment(CapsuleContainer);
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
            // Start opening animation
            isClosed = false;

            FRotator Rotation(0.0f, 0.0f, 0.0f);
            GetWorld()->SpawnActor<ARessource>(RessourceInside.Last(), RessourcePointSpawn->GetComponentLocation(), Rotation);
            RessourceInside.Pop();
        }
    }
}