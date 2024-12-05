
#include "Game/GameManager.h"


AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	TimerManager();
}

void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AGameManager::TimerManager()
{
	GetWorldTimerManager().SetTimer(timer, FTimerDelegate::CreateLambda([this]() {

		RealTimeValue++;
		GameTimeValue = GetTimeInGame(RealTimeValue);
		UE_LOG(LogTemp, Display, TEXT("REAL TIME (second) : %i"), RealTimeValue);
		UE_LOG(LogTemp, Display, TEXT("Time in game (second) : %i"), GameTimeValue);
		UE_LOG(LogTemp, Display, TEXT("----------------------------------------------------------------------------"));

		if (RealTimeValue >= TOTAL_PLAYING_TIME)
		{
			GetWorldTimerManager().ClearTimer(timer);
			GameOverEndGame();
			UE_LOG(LogTemp, Display, TEXT("STOP GAME !!!!! : %i"), GameTimeValue);
		}

	}), 1, true);
}

int AGameManager::GetTimeInGame(int &RealTime)
{
	return RealTime * translateSecondsIRLTimeToGameTime;
}



void AGameManager::GameOverEndGame()
{
	//TODO : display screen (avec les causes de la mort)
	// Si timer est encore en cours -> arrêter le timer
	UE_LOG(LogTemp, Display, TEXT("BAD ENDING"));
}

void AGameManager::VictoryEndGame()
{
	//TODO : display screen
	// Stop timer
	UE_LOG(LogTemp, Display, TEXT("GOOD ENDING"));
}
