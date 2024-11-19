// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MIRA_DAU_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
private:
	float Speed = 0.3;

public:

	/*UPROPERTY(EditAnywhere)
	FString name = "Mira";*/

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* MyCamera;*/


	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	class UInputMappingContext* InputMappingContext;


	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Enhance Input")
	class UInputAction* JumpAction;

	void Move(const FInputActionValue& InputValue);
	void Jump(const FInputActionValue& InputValue);
};
