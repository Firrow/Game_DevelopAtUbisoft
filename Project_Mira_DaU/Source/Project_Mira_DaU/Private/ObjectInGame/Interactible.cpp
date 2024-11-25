
#include "ObjectInGame/Interactible.h"

AInteractible::AInteractible()
{
	//PrimaryActorTick.bCanEverTick = true;

	ObjectTags.Add(TEXT("Interactible"));

	ObjectCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Object's Capsule"));
	ObjectCapsule->InitCapsuleSize(10.f, 10.0f);
}

void AInteractible::BeginPlay()
{
	Super::BeginPlay();
	
}

