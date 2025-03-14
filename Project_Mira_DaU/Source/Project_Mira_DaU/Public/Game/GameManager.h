
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGearsQuantityChanged, int, PlayerGearsQuantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameHourChanged, FString, ConvertedGameTimeValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeedValueDetermined, int, SeedCurrentMap);


UCLASS()
class PROJECT_MIRA_DAU_API AGameManager : public AActor
{
	GENERATED_BODY()
	
private:	
	int SECONDS_IN_A_DAY = 86400;
	int TOTAL_PLAYING_TIME = 300; //TODO : Set to 3600 or 1800 or 900 or 600 or 300 when tests are finished

	int RealTimeValue = 0;
	int GameTimeValue = 0;
	FTimerHandle timer;
	int translateSecondsIRLTimeToGameTime = SECONDS_IN_A_DAY / TOTAL_PLAYING_TIME;
	FString ConvertedGameTimeValue = "";
	int NUMBER_OF_GEARS = 0;
	int PlayerGearsQuantity = 0;
	int SeedCurrentMap = 0;
	bool UIIsDisplay = false;
	bool MapIsDisplay = false;
	float SoundVolumeMultiplier = 1.0f;
	float SoundPitchMultiplier = 1.0f;

	UPROPERTY()
	UUserWidget* PlayerInformationsWidget;

	UPROPERTY()
	UUserWidget* MapWidget;

	UPROPERTY()
	UUserWidget* GameOverWidget;

	UPROPERTY()
	UUserWidget* VictoryWidget;

	UPROPERTY()
	UUserWidget* PauseWidget;

	AGameManager();
	void TimerManager();
	int GetTimeInGame(const int& RealTime);
	FString ConvertGameTimeValue();
	void FinishedGame();

	

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void DisplayUI_PlayerInformations();
	void DisplayUI_Map();
	void GameOverEndGame();
	void VictoryEndGame();
	void PauseGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DEBUGING SECTION")
	bool ActiveTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> VictoryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerInformationsWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MapWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element in Map")
	TMap<int, int> RessourcesQuantity;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGearsQuantityChanged OnGearsQuantityChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameHourChanged OnGameHourChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSeedValueDetermined OnSeedValueDetermined;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ResetGame();

	int GetRealTimeValue() const;
	void SetRealTimeValue(int NewValue);

	int GetGameTimeValue() const;
	void SetGameTimeValue(int NewValue);

	int GetTotalGearQuantity() const;
	void SetTotalGearQuantity(int NewValue);

	int GetPlayerGearsQuantity() const;
	void SetPlayerGearsQuantity(int NewValue);

	int GetSeedCurrentMap() const;
	void SetSeedCurrentMap(int NewValue);




	int GetSoundVolumeMultiplier() const;
	void SetSoundVolumeMultiplier(int NewValue);

	int GetSoundPitchMultiplier() const;
	void SetSoundPitchMultiplier(int NewValue);
};
