
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
        
        if (RessourceInside.IsEmpty())
        {
            isEmpty = true;
        }
        else
        {
            //TODO : Lancer l'animation du coffre en BP
            FRotator Rotation(0.0f, 0.0f, 0.0f);
            GetWorld()->SpawnActor<ARessource>(RessourceInside.Last(), RessourcePointSpawn->GetComponentLocation(), Rotation);
            RessourceInside.Pop();
            //UE_LOG(LogTemp, Display, TEXT("Taille tableau : %i"), RessourceInside.Num());
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