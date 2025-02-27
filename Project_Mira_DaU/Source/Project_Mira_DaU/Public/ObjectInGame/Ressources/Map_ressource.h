
#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Ressource.h"
#include "Map_ressource.generated.h"

class AGameManager;

UCLASS()
class PROJECT_MIRA_DAU_API AMap_ressource : public ARessource
{
	GENERATED_BODY()
	
private:
	AGameManager* GameManager = nullptr;

	virtual void RessourceEffect() override;
	virtual void PlayRessourceSound() override;

public:
	virtual void BeginPlay() override;

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, Category = "Animation")
	USoundBase* RessourceSound;
};
