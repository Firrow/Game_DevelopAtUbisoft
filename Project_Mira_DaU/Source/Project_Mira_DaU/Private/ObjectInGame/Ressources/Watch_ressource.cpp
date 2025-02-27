
#include <Characters/PlayerCharacter.h>
#include "Game/GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectInGame/Ressources/Watch_ressource.h"

void AWatch_ressource::BeginPlay()
{
    Super::BeginPlay();

    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
    GameManager = Cast<AGameManager>(FoundActor);

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &AWatch_ressource::BeginOverlap);
}

void AWatch_ressource::RessourceEffect()
{
    if (GameManager != nullptr)
    {
        GameManager->SetRealTimeValue(GameManager->GetRealTimeValue() - TIME_IN_WATCH);
    }
}

void AWatch_ressource::PlayRessourceSound()
{
    UGameplayStatics::PlaySound2D(this, RessourceSound, GameManager->GetSoundVolumeMultiplier(), GameManager->GetSoundPitchMultiplier(), 0);
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