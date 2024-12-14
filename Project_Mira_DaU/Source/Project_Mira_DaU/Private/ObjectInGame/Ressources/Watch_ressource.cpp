
#include <Characters/PlayerCharacter.h>
#include "Game/GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectInGame/Ressources/Watch_ressource.h"

void AWatch_ressource::BeginPlay()
{
    Super::BeginPlay();

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &AWatch_ressource::BeginOverlap);
}

void AWatch_ressource::RessourceEffect()
{
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
    AGameManager* GameManager = Cast<AGameManager>(FoundActor);

    if (GameManager != nullptr)
    {
        GameManager->RealTimeValue -= TIME_IN_WATCH; //Update RealTime to change TimeInGame
    }
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
        Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    }
}