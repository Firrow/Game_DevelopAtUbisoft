
#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Interactible.h"
#include "ObjectInGame/Interface/InteractibleInterface.h"
#include "Components/BoxComponent.h"
#include "Ladder.generated.h"

/**
 * 
 */
class UBoxComponent;
UCLASS()
class PROJECT_MIRA_DAU_API ALadder : public AInteractible, public IInteractibleInterface
{
	GENERATED_BODY()

private:

	virtual void Effect() override;

public:
	ALadder();

	UPROPERTY(VisibleAnywhere, Category = "Box Collider")
	UBoxComponent* BoxCollider;
};