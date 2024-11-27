
#include "ObjectInGame/Ladder.h"

ALadder::ALadder()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxLadder"));
	BoxCollider->InitBoxExtent(FVector(10.f, 10.f, 30.f));
	BoxCollider->SetupAttachment(RootComponent);

	Tags.Add(TEXT("Ladder"));
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