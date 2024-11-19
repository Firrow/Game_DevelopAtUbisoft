// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"



void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    /*FString* myName;
    myName = &name;
    FString nameValue = *myName;

    UE_LOG(LogTemp, Display, TEXT("L'adresse de name est : %u"), myName);
    UE_LOG(LogTemp, Display, TEXT("La valeur de name est : %s"), *nameValue);*/


    //POINTEUR
    /*float myFloat = 5;
    float* yourFloat = &myFloat;
    float floatValue = *yourFloat;

    UE_LOG(LogTemp, Display, TEXT("La valeur de myFloat est : %f"), floatValue);*/



    //REFERENCE
    /*float damage = 0;
    float& damageRef = damage;
    damageRef = 5;

    UE_LOG(LogTemp, Display, TEXT("damageRef : %f, damage : %f"), damageRef, damage)*/
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //Add input mapping context
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
        Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
    }
}

void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
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

        UE_LOG(LogTemp, Display, TEXT("ForwardDirection.X : %f, ForwardDirection.Y : %f"), ForwardDirection.X, ForwardDirection.Y);
        UE_LOG(LogTemp, Display, TEXT("RightDirection.X : %f, RightDirection.Y : %f"), RightDirection.X, RightDirection.Y);
    }
}

void APlayerCharacter::Jump(const FInputActionValue& InputValue)
{
    
}