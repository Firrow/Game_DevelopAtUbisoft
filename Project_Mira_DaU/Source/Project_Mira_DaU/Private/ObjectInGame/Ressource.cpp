
#include "ObjectInGame/Ressource.h"
#include "Sound/SoundBase.h" 


ARessource::ARessource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Object's Capsule"));
	ObjectCapsule->InitCapsuleSize(5.f, 5.f);
}

void ARessource::RessourceEffect() {}
void ARessource::PlayRessourceSound() {}

void ARessource::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
	RessourceEffect();
    PlayRessourceSound();

	this->Destroy();
}