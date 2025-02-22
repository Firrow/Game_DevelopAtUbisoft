
#include "Components/TextBlock.h"
#include "Game/GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GameInformationsUI.h"

void UGameInformationsUI::NativeConstruct()
{
    Super::NativeConstruct();

    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
    GameManager = Cast<AGameManager>(FoundActor);

    if (GameManager)
    {
        // Lier la fonction UpdateGearQuantity au delegate du GameManager
        GameManager->OnGearsQuantityChanged.AddDynamic(this, &UGameInformationsUI::UpdateGearQuantity);
        GameManager->OnGameHourChanged.AddDynamic(this, &UGameInformationsUI::UpdateGameHour);

        UpdateGearQuantity(GameManager->GetPlayerGearsQuantity());
    }
}

void UGameInformationsUI::UpdateGearQuantity(int32 NewQuantity)
{
    if (GearQuantity)
    {
        GearQuantity->SetText(FText::AsNumber(NewQuantity));
    }
}

void UGameInformationsUI::UpdateGameHour(FString NewHour)
{
    if (GameHour)
    {
        GameHour->SetText(FText::FromString(NewHour));
    }
}