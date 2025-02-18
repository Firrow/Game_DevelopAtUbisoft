
#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Interactible.h"
#include "ObjectInGame/Ressource.h"
#include "ObjectInGame/Interface/InteractibleInterface.h"
#include "Container.generated.h"

class USphereComponent;

UCLASS()
class PROJECT_MIRA_DAU_API AContainer : public AInteractible, public IInteractibleInterface
{
	GENERATED_BODY()

private:

	virtual void Effect() override;

public:

	UPROPERTY(EditAnywhere, Category = "Global Informations")
	TArray<TSubclassOf<ARessource>> RessourceInside = {nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Global Informations") //temporaire ? vérifier après si le container a toujours son objet ?
	bool isEmpty = false;

	UPROPERTY(EditAnywhere, Category = "CapsuleRessource")
	USphereComponent* RessourcePointSpawn;

	UPROPERTY(EditAnywhere, Category = "CapsuleRessource")
	USphereComponent* CapsuleContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool isClosed = true;

	AContainer();
};

