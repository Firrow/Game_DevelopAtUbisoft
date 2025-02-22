
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameInformationsUI.generated.h"

class UTextBlock;
class AGameManager;

UCLASS()
class PROJECT_MIRA_DAU_API UGameInformationsUI : public UUserWidget
{
	GENERATED_BODY()

    protected:
        UPROPERTY(meta = (BindWidget))
        UTextBlock* GearQuantity;

        UPROPERTY(meta = (BindWidget))
        UTextBlock* GameHour;

    private:
        AGameManager* GameManager;

    public:
        virtual void NativeConstruct() override;

        UFUNCTION()
        void UpdateGearQuantity(int32 NewQuantity);

        UFUNCTION()
        void UpdateGameHour(int32 NewQuantity);
};
