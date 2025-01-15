
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TriggerEnding.generated.h"


UCLASS()
class PROJECT_MIRA_DAU_API ATriggerEnding : public AActor
{
	GENERATED_BODY()
	
public:	
	ATriggerEnding();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "BoxComponent")
	class UBoxComponent* TriggerWithEnding;
};
