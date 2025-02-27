
#include "Components/SphereComponent.h"
#include "Game/GameManager.h"
#include "Sound/SoundBase.h" 
#include "Kismet/GameplayStatics.h"
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
            isClosed = false;

            AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
            GameManager = Cast<AGameManager>(FoundActor);
            UGameplayStatics::PlaySound2D(this, OpenedSound, GameManager->GetSoundVolumeMultiplier(), GameManager->GetSoundPitchMultiplier(), 0);

            FRotator Rotation(0.0f, 0.0f, 0.0f);
            GetWorld()->SpawnActor<ARessource>(RessourceInside.Last(), RessourcePointSpawn->GetComponentLocation(), Rotation);
            RessourceInside.Pop();
        }
    }
}