
#include "ObjectInGame/Ressource.h"

// Sets default values
ARessource::ARessource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Object's Capsule"));
	ObjectCapsule->InitCapsuleSize(1.f, 1.f);
}

void ARessource::RessourceEffect()
{

}

void ARessource::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
	RessourceEffect();

	this->Destroy();
}