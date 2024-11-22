
#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Interactible.h"
#include "ObjectInGame/Interface/InteractibleInterface.h"
#include "Ladder.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MIRA_DAU_API ALadder : public AInteractible, public IInteractibleInterface
{
	GENERATED_BODY()

private:

	virtual void Effect() override;

public:
	ALadder();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};