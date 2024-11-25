
#include "ObjectInGame/Container.h"
#include <Characters/PlayerCharacter.h>


AContainer::AContainer()
{
    RessourcePointSpawn = CreateDefaultSubobject<USphereComponent>(TEXT("RessourcePointSpawn"));
    RessourcePointSpawn->InitSphereRadius(1.f);
    RessourcePointSpawn->SetupAttachment(ObjectCapsule);
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
        //TODO : Lancer l'animation du coffre en BP
        if (sizeof(RessourceInside) == 0)
        {
            isEmpty = true;
        }
        else
        {
            //TODO : Fait apparaitre l'objet
            FRotator Rotation(0.0f, 0.0f, 0.0f);
            //FActorSpawnParameters SpawnParams;


            //ATTENTION UE CRASH QUAND SPAWNACTOR
            //AActor* SpawnedActor = GetWorld()->SpawnActor<ARessource>(RessourceInside[0], RessourcePointSpawn->GetComponentLocation(), Rotation);
            //GetWorld()->SpawnActor<ARessource>(RessourceInside[0], RessourcePointSpawn->GetComponentLocation(), Rotation); //RessourceInside[0]
        }

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
    /*APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {

    }*/
}