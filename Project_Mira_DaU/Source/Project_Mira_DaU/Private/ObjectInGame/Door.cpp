
#include "ObjectInGame/Door.h"

ADoor::ADoor()
{
    
}

void ADoor::Effect()
{
    //TODO : Load Level in building
    //TODO : Launch open animation (change sprite)
    //TODO : Launch sound animation
    if (!IsOpened)
    {
        IsOpened = true;
        //TODO : Load Level in building
        //TODO : Launch open animation (change sprite)
        //TODO : Launch sound opened animation
        GEngine->AddOnScreenDebugMessage(-1, 50000.f, FColor::Yellow, FString::Printf(TEXT("C'EST OUVERT ENTREZ !!!!")));
    }
    else
    {
        //TODO : Load map generated
        //TODO : Launch close animation (change sprite)
        //TODO : Launch sound closed animation
        IsOpened = false;
    }
}