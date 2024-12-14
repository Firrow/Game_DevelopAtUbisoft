
#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Ressource.h"
#include "Map_ressource.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MIRA_DAU_API AMap_ressource : public ARessource
{
	GENERATED_BODY()
	
private:

	virtual void RessourceEffect() override;

public:
	virtual void BeginPlay() override;

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
};
