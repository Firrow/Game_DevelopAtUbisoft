
#include "Components/TextBlock.h"
#include "Game/GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/PauseUI.h"

void UPauseUI::NativeConstruct()
{
    Super::NativeConstruct();

    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
    GameManager = Cast<AGameManager>(FoundActor);

    if (GameManager)
    {
        GameManager->OnSeedValueDetermined.AddDynamic(this, &UPauseUI::DisplayMapSeed);

        DisplayMapSeed(GameManager->GetSeedCurrentMap());
    }
}

void UPauseUI::DisplayMapSeed(int32 Value)
{
    if (SeedValue)
    {
        SeedValue->SetText(FText::AsNumber(Value));
    }
}
