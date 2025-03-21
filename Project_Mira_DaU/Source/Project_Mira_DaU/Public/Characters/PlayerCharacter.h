
#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ObjectInGame/Interactible.h"
#include "ObjectInGame/Container.h"
#include "Game/GameManager.h"

#include "PlayerCharacter.generated.h"


class UInputAction;
class UInputMappingContext;

UCLASS()
class PROJECT_MIRA_DAU_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
private:
	
	int MAX_HEALTH = 10;
	USceneComponent* Component;
	FVector PlayerVelocity;
	AActor* ActorIsOverlaped = nullptr;
	bool bIsOnLadder = false;
	float MaxStepHeightPlayer = 5.0f;
	TArray<AActor*> OverlappingLadders;
	TArray<AActor*> OverlappingObjects;
	class AGameManager* GameManager = nullptr;

	APlayerCharacter();
	void MoveRL(const FInputActionValue& InputValue);
	void MoveFB(const FInputActionValue& InputValue);
	void EndMoveFB(const FInputActionValue& InputValue);
	void Jump(const FInputActionValue& InputValue);
	void Interact(const FInputActionValue& InputValue);
	void PauseGame(const FInputActionValue& InputValue);
	void UpdateCurrentState();
	void UpdateIsFacingLeft();

public:
	int LifePlayer = MAX_HEALTH;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpHeight = 300.0f;


	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	UInputAction* MoveRLAction;

	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	UInputAction* MoveFBAction;

	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	UInputAction* PauseAction;


	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	FString CurrentStateMovement = "Idle";

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool isFacingLeft = false;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	bool isArmed;



	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);
};

