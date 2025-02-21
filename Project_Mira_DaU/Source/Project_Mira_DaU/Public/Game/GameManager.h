
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.generated.h"

UCLASS()
class PROJECT_MIRA_DAU_API AGameManager : public AActor
{
	GENERATED_BODY()
	
private:	

	int RealTimeValue;
	int GameTimeValue;
	int PlayerGearsQuantity;

	int SECONDS_IN_A_DAY = 86400;
	int TOTAL_PLAYING_TIME = 10; //TODO : Set to 3600 or 1800 when tests are finished

	FTimerHandle timer;
	int translateSecondsIRLTimeToGameTime = SECONDS_IN_A_DAY / TOTAL_PLAYING_TIME;

	UPROPERTY()
	UUserWidget* GameOverWidget;

	UPROPERTY()
	UUserWidget* VictoryWidget;

	AGameManager();
	void TimerManager();
	int GetTimeInGame(const int& RealTime);
	

protected:
	virtual void BeginPlay() override;


public:	
	virtual void Tick(float DeltaTime) override;
	void GameOverEndGame();
	void VictoryEndGame();

	int NUMBER_OF_GEARS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DEBUGING SECTION")
	bool ActiveTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> VictoryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element in Map")
	TMap<int, int> RessourcesQuantity;



	int GetRealTimeValue() const;
	void SetRealTimeValue(int NewValue);

	int GetGameTimeValue() const;
	void SetGameTimeValue(int NewValue);

	int GetPlayerGearsQuantity() const;
	void SetPlayerGearsQuantity(int NewValue);
};
