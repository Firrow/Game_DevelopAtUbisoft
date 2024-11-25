
#include "ObjectInGame/Ressource.h"

// Sets default values
ARessource::ARessource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARessource::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARessource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARessource::RessourceEffect()
{

}