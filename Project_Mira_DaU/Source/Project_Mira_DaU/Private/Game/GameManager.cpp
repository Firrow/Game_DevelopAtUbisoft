
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
	GetWorldTimerManager().SetTimer(timer1, FTimerDelegate::CreateLambda([this]() {

		RealTimeValue++;
		GameTimeValue = GetTimeInGame(RealTimeValue);
		UE_LOG(LogTemp, Display, TEXT("Time in game : %i"), GameTimeValue);

	}), 1, true);

	GetWorldTimerManager().SetTimer(timer2, FTimerDelegate::CreateLambda([this]() {
		GetWorldTimerManager().ClearTimer(timer1);

		GameOverEndGame();
		UE_LOG(LogTemp, Display, TEXT("STOP GAME !!!!! : %i"), GameTimeValue);

	}), TOTAL_PLAYING_TIME, true);
}

int AGameManager::GetTimeInGame(int &RealTime)
{
	return RealTime * translateSecondsIRLTimeToGameTime;
}




void AGameManager::GameOverEndGame()
{
	//TODO : display screen (avec les causes de la mort)
}

void AGameManager::VictoryEndGame()
{
	//TODO : display screen
}
