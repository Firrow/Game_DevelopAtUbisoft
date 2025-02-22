
#include <Characters/PlayerCharacter.h>
#include "Kismet/GameplayStatics.h"
#include "ObjectInGame/Ressources/Computer_ressource.h"

void AComputer_ressource::BeginPlay()
{
    Super::BeginPlay();

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &AComputer_ressource::BeginOverlap);
}

void AComputer_ressource::RessourceEffect()
{
    // Add UI with informations on screen
    if (GameManager != nullptr) 
    {
        GameManager->DisplayUI_PlayerInformations();
    }
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
        AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
        GameManager = Cast<AGameManager>(FoundActor);

        Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    }
}