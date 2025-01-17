
#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Ressource.h"
#include "Gear_ressource.generated.h"


class APlayerCharacter;

UCLASS()
class PROJECT_MIRA_DAU_API AGear_ressource : public ARessource
{
	GENERATED_BODY()
	
private:
	APlayerCharacter* PlayerCharacter = nullptr;

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
