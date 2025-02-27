
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Game/GameManager.h"
#include "ObjectInGame/Ressources/Gear_ressource.h"


void AGear_ressource::BeginPlay()
{
    Super::BeginPlay();

    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
    GameManager = Cast<AGameManager>(FoundActor);

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGear_ressource::BeginOverlap);
}

void AGear_ressource::RessourceEffect()
{
    if (GameManager != nullptr)
    {
        GameManager->SetPlayerGearsQuantity(GameManager->GetPlayerGearsQuantity() + 1);
    }
}

void AGear_ressource::PlayRessourceSound()
{
    UGameplayStatics::PlaySound2D(this, RessourceSound, GameManager->GetSoundVolumeMultiplier(), GameManager->GetSoundPitchMultiplier(), 0);
}

void AGear_ressource::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // Overlap
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (PlayerCharacter != nullptr)
    {
        AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
        GameManager = Cast<AGameManager>(FoundActor);

        Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    }
}