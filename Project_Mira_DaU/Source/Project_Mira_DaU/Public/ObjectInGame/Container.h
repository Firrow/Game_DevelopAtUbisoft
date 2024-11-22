
#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Interactible.h"
#include "ObjectInGame/Ressource.h"
#include "ObjectInGame/Interface/InteractibleInterface.h"
#include "Container.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MIRA_DAU_API AContainer : public AInteractible, public IInteractibleInterface
{
	GENERATED_BODY()

private :

	virtual void Effect() override;

public :

	AContainer();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "Global Informations")
	TSubclassOf<ARessource> RessourceInside;

	UPROPERTY(EditAnywhere, Category = "Global Informations") //temporaire ? vérifier après si le container a toujours son objet ?
	bool isEmpty = false;
};
