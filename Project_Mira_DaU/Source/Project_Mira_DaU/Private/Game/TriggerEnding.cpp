
#include "Game/TriggerEnding.h"

ATriggerEnding::ATriggerEnding()
{
	Tags.Add(TEXT("EndingTrigger"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATriggerEnding::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATriggerEnding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

