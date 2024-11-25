
#include "ObjectInGame/Container.h"
#include <Characters/PlayerCharacter.h>


AContainer::AContainer()
{

}

void AContainer::BeginPlay()
{
	Super::BeginPlay();

    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &AContainer::BeginOverlap);
}

void AContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AContainer::Effect()
{
	if (!isEmpty)
	{
        //TODO : Donne l'objet au joueur 
		isEmpty = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Applique l'effet : CONTAINER !"));
	}
}

void AContainer::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{

    // Overlap
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        
    }
}