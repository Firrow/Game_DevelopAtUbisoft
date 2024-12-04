
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

UCLASS()
class PROJECT_MIRA_DAU_API AGameManager : public AActor
{
	GENERATED_BODY()
	
private:	
	int SECONDS_IN_A_DAY = 86400;
	int TOTAL_PLAYING_TIME = 10; //TODO : Set to 3600 or 1800 when tests are finished

	FTimerHandle timer;
	int translateSecondsIRLTimeToGameTime = SECONDS_IN_A_DAY / TOTAL_PLAYING_TIME;

	AGameManager();
	void TimerManager();
	int GetTimeInGame(int& RealTime);
	

protected:
	virtual void BeginPlay() override;


public:	
	int RealTimeValue;
	int GameTimeValue;

	virtual void Tick(float DeltaTime) override;
	void GameOverEndGame();
	void VictoryEndGame();
};
