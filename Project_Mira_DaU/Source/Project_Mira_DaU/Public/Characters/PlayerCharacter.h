// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "ObjectInGame/Interactible.h"
#include "ObjectInGame/Container.h"


#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MIRA_DAU_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
private:
	APlayerCharacter();

	USceneComponent* Component;
	FVector PlayerVelocity;
	AActor* ActorIsOverlaped;

	void Move(const FInputActionValue& InputValue);
	void Jump(const FInputActionValue& InputValue);
	void Interact(const FInputActionValue& InputValue);
	void LadderMove(const FInputActionValue& InputValue);
	void UpdateCurrentState();
	void UpdateIsFacingLeft();

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpHeight = 300.0f;


	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	class UInputAction* LadderAction;


	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	FString CurrentStateMovement = "Idle";

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool isFacingLeft = false;



	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 gears;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	bool isArmed;




	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};

