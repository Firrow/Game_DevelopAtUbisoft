
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

	int RealTimeValue; //créer getter et setter
	int GameTimeValue; //créer getter et setter

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
	int NUMBER_OF_GEARS = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> VictoryWidgetClass;

	virtual void Tick(float DeltaTime) override;
	void GameOverEndGame();
	void VictoryEndGame();



	int GetRealTimeValue() const;
	void SetRealTimeValue(int NewValue);

	int GetGameTimeValue() const;
	void SetGameTimeValue(int NewValue);
};
