
#include "Game/GameManager.h"
#include "Components/WidgetComponent.h"


AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	TimerManager();

	GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	VictoryWidget = CreateWidget<UUserWidget>(GetWorld(), VictoryWidgetClass);
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
	GetWorldTimerManager().ClearTimer(timer);
	GameOverWidget->AddToViewport();
}

void AGameManager::VictoryEndGame()
{
	GetWorldTimerManager().ClearTimer(timer);
	VictoryWidget->AddToViewport();
}
