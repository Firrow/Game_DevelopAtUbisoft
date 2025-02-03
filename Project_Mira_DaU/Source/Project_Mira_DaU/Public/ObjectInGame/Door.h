
#pragma once

#include "CoreMinimal.h"
#include "ObjectInGame/Interactible.h"
#include "ObjectInGame/Interface/InteractibleInterface.h"
#include "Door.generated.h"


UCLASS()
class PROJECT_MIRA_DAU_API ADoor : public AInteractible, public IInteractibleInterface
{
	GENERATED_BODY()

private:
	virtual void Effect() override;

public:
	ADoor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool IsOpened = false;
};
