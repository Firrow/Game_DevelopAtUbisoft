
#include "ObjectInGame/Container.h"


AContainer::AContainer()
{

}

void AContainer::BeginPlay()
{
	Super::BeginPlay();

}

void AContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AContainer::Effect() 
{
	if (!isEmpty)
	{
		isEmpty = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Applique l'effet : CONTAINER !"));
	}
}