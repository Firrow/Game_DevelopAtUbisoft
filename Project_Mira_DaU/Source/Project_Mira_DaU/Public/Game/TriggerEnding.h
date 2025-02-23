
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TriggerEnding.generated.h"

class AGameManager;

UCLASS()
class PROJECT_MIRA_DAU_API ATriggerEnding : public AActor
{
	GENERATED_BODY()
	
private:
	AGameManager* GameManager = nullptr;

	ATriggerEnding();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void CheckEnding();

	UPROPERTY(EditAnywhere, Category = "BoxComponent")
	class UBoxComponent* TriggerWithEnding;

	UPROPERTY(BlueprintReadOnly, Category = "Sprite")
	bool isOnMapLeftSide = false;
};
