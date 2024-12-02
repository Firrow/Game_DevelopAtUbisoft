
#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Interactible.h"
#include "ObjectInGame/Ressource.h"
#include "ObjectInGame/Interface/InteractibleInterface.h"
#include "Components/SphereComponent.h"
#include "Container.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MIRA_DAU_API AContainer : public AInteractible, public IInteractibleInterface
{
	GENERATED_BODY()

private:

	virtual void Effect() override;

public:

	AContainer();

	UPROPERTY(EditAnywhere, Category = "Global Informations")
	TArray<TSubclassOf<ARessource>> RessourceInside;

	UPROPERTY(VisibleAnywhere, Category = "Global Informations") //temporaire ? vérifier après si le container a toujours son objet ?
	bool isEmpty = false;

	UPROPERTY(EditAnywhere, Category = "CapsuleRessource")
	class USphereComponent* RessourcePointSpawn;

	UPROPERTY(EditAnywhere, Category = "CapsuleRessource")
	class USphereComponent* CapsuleColliderRessource;

};

