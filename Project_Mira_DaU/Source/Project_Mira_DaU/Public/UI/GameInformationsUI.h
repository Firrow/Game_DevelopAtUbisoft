
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
        // Associe cette variable � l'�l�ment texte dans le Blueprint
        UPROPERTY(meta = (BindWidget))
        UTextBlock* GearQuantity;

    private:
        AGameManager* GameManager;

    public:
        virtual void NativeConstruct() override;

        UFUNCTION()
        void UpdateGearQuantity(int32 NewQuantity);
};
