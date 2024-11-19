// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"



void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //UE_LOG(LogTemp, Display, TEXT("CurrentState : %s"), *CurrentStateMovement);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //Add input mapping context (PAS TOUT COMPRIS)
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        //Get local player subsystem
        if (class UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            //Add input context
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }

    //Bind inputs to corresponding actions
    if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        Input->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);

        Input->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::ResetCurrentState);
        Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::ResetCurrentState);
    }
}

//PLAYER MOVEMENT
void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
    SetCurrentState("move");

    FVector2D InputVector = InputValue.Get<FVector2D>();

    if (IsValid(Controller))
    {
        //Get Forward direction
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * Speed;
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * Speed;

        //Add movement
        AddMovementInput(ForwardDirection, InputVector.Y);
        AddMovementInput(RightDirection, InputVector.X);
    }
}

//PLAYER JUMPING
void APlayerCharacter::Jump(const FInputActionValue& InputValue)
{
    SetCurrentState("jump"); //TODO : Upgrade jump animation system (anim up -> anim down -> ResetCurrentState ?)
    GetCharacterMovement()->JumpZVelocity = JumpHeight;
    ACharacter::Jump();
}

//UPDATE ANIMATION
void APlayerCharacter::SetCurrentState(FString currentState)
{
    CurrentStateMovement = currentState;
}

void APlayerCharacter::ResetCurrentState(const FInputActionValue& InputValue) 
{
    CurrentStateMovement = "idle";
}