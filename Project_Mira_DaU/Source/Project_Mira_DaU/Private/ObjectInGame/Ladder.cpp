
#include "ObjectInGame/Ladder.h"

ALadder::ALadder()
{

}

void ALadder::BeginPlay()
{
	Super::BeginPlay();

}

void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALadder::Effect()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Applique l'effet : LADDER !"));
	//TODO : fonction utile ?
}