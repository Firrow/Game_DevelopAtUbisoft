
#include "Game/TriggerEnding.h"

ATriggerEnding::ATriggerEnding()
{
	Tags.Add(TEXT("EndingTrigger"));

	TriggerWithEnding = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleRessource"));
	TriggerWithEnding->SetBoxExtent(FVector(1.f, 1.f, 1.f));
	TriggerWithEnding->SetupAttachment(RootComponent);

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

