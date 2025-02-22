
#include "Game/GameManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"


AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (ActiveTimer)
	{
		TimerManager();
	}

	PlayerInformationsWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerInformationsWidgetClass);
	GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	VictoryWidget = CreateWidget<UUserWidget>(GetWorld(), VictoryWidgetClass);

	NUMBER_OF_GEARS = RessourcesQuantity[0];
}

void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AGameManager::TimerManager()
{
	GetWorldTimerManager().SetTimer(timer, FTimerDelegate::CreateLambda([this]() {

		SetRealTimeValue(GetRealTimeValue() + 1);
		SetGameTimeValue(GetTimeInGame(GetRealTimeValue()));
		UE_LOG(LogTemp, Warning, TEXT("REAL TIME (second) : %i"), RealTimeValue);
		UE_LOG(LogTemp, Warning, TEXT("Time in game (second) : %i"), GameTimeValue);
		UE_LOG(LogTemp, Warning, TEXT("----------------------------------------------------------------------------"));

		if (GetRealTimeValue() >= TOTAL_PLAYING_TIME)
		{
			GetWorldTimerManager().ClearTimer(timer);
			GameOverEndGame();
			UE_LOG(LogTemp, Warning, TEXT("STOP GAME !!!!! : %i"), GameTimeValue);
		}

	}), 1, true);
}

int AGameManager::GetTimeInGame(const int &RealTime)
{
	return RealTime * translateSecondsIRLTimeToGameTime;
}



void AGameManager::DisplayUI_PlayerInformations()
{
	PlayerInformationsWidget->AddToViewport();
	UIIsDisplay = true;
}

void AGameManager::GameOverEndGame()
{
	//TODO : display screen (avec les causes de la mort)
	FinishedGame();
	GameOverWidget->AddToViewport();
}

void AGameManager::VictoryEndGame()
{
	FinishedGame();
	VictoryWidget->AddToViewport();
}

void AGameManager::FinishedGame()
{
	GetWorldTimerManager().ClearTimer(timer);

	if (UIIsDisplay)
	{
		PlayerInformationsWidget->RemoveFromParent();
	}
}


int AGameManager::GetRealTimeValue() const
{
	return RealTimeValue;
}
void AGameManager::SetRealTimeValue(int NewValue)
{
	RealTimeValue = NewValue;
}

int AGameManager::GetGameTimeValue() const
{
	return GameTimeValue;
}

void AGameManager::SetGameTimeValue(int NewValue)
{
	GameTimeValue = NewValue;
	OnGameHourChanged.Broadcast(GameTimeValue);
}

int AGameManager::GetPlayerGearsQuantity() const
{
	return PlayerGearsQuantity;
}
void AGameManager::SetPlayerGearsQuantity(int NewValue)
{
	PlayerGearsQuantity = NewValue;
	OnGearsQuantityChanged.Broadcast(PlayerGearsQuantity);
}