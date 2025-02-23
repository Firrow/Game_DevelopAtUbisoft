
#include <Characters/PlayerCharacter.h>
#include "Kismet/GameplayStatics.h"
#include "ObjectInGame/Ressources/Map_ressource.h"

void AMap_ressource::BeginPlay()
{
    Super::BeginPlay();

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMap_ressource::BeginOverlap);
}

void AMap_ressource::RessourceEffect()
{
    // Add UI Map on screen
    if (GameManager != nullptr)
    {
        GameManager->DisplayUI_Map();
    }
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
        AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
        GameManager = Cast<AGameManager>(FoundActor);

        Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    }
}