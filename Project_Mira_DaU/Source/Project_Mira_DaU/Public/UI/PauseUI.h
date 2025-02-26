
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseUI.generated.h"


UCLASS()
class PROJECT_MIRA_DAU_API UPauseUI : public UUserWidget
{
	GENERATED_BODY()
	
    protected:
        UPROPERTY(meta = (BindWidget))
        UTextBlock* SeedValue;

    private:
        AGameManager* GameManager;

    public:
        virtual void NativeConstruct() override;

        UFUNCTION()
        void DisplayMapSeed(int32 Value);
};
