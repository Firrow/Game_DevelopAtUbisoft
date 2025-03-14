
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
	MapWidget = CreateWidget<UUserWidget>(GetWorld(), MapWidgetClass);
	PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
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

		if (GetRealTimeValue() >= TOTAL_PLAYING_TIME)
		{
			GetWorldTimerManager().ClearTimer(timer);
			GameOverEndGame();
		}

	}), 1, true);
}

int AGameManager::GetTimeInGame(const int &RealTime)
{
	return RealTime * translateSecondsIRLTimeToGameTime;
}

FString AGameManager::ConvertGameTimeValue()
{
	int hours = GameTimeValue / 3600;
	int minutes = ((GameTimeValue % 3600) / 60);

	if (minutes == 0)
	{
		return FString::Printf(TEXT("%i h 00"), hours);
	}
	else
	{
		return FString::Printf(TEXT("%i h %i"), hours, minutes);
	}
}


// UIs
void AGameManager::DisplayUI_PlayerInformations()
{
	PlayerInformationsWidget->AddToViewport();
	UIIsDisplay = true;
}

void AGameManager::DisplayUI_Map()
{
	MapWidget->AddToViewport();
	MapIsDisplay = true;
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

void AGameManager::PauseGame()
{
	PauseWidget->AddToViewport();
}

/// <summary>
/// Stop timer and undisplay UI if it was on screen
/// </summary>
void AGameManager::FinishedGame()
{
	ResetGame();

	if (UIIsDisplay)
	{
		PlayerInformationsWidget->RemoveFromParent();
	}
	if (MapIsDisplay)
	{
		MapWidget->RemoveFromParent();
	}
}

void AGameManager::ResetGame()
{
	GetWorldTimerManager().ClearTimer(timer);
	timer.Invalidate();
	SetRealTimeValue(0);
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
	ConvertedGameTimeValue = ConvertGameTimeValue();
	OnGameHourChanged.Broadcast(ConvertedGameTimeValue);
}

int AGameManager::GetTotalGearQuantity() const
{
	return NUMBER_OF_GEARS;
}
void AGameManager::SetTotalGearQuantity(int NewValue)
{
	NUMBER_OF_GEARS = NewValue;
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

int AGameManager::GetSeedCurrentMap() const
{
	return SeedCurrentMap;
}
void AGameManager::SetSeedCurrentMap(int NewValue)
{
	SeedCurrentMap = NewValue;
	OnSeedValueDetermined.Broadcast(SeedCurrentMap);
}




// SOUND
int AGameManager::GetSoundVolumeMultiplier() const
{
	return SoundVolumeMultiplier;
}
void AGameManager::SetSoundVolumeMultiplier(int NewValue)
{
	SoundVolumeMultiplier = NewValue;
}

int AGameManager::GetSoundPitchMultiplier() const
{
	return SoundPitchMultiplier;
}
void AGameManager::SetSoundPitchMultiplier(int NewValue)
{
	SoundPitchMultiplier = NewValue;
}