// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


APlayerCharacter::APlayerCharacter() 
{
    Tags.Add(TEXT("Player"));
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    Component = this->GetRootComponent();

    UCapsuleComponent* ObjectCapsule = GetCapsuleComponent();
    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::BeginOverlap);
    ObjectCapsule->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::EndOverlap);

    GetCharacterMovement()->MaxStepHeight = 5.0f;
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Component != nullptr)
    {
        PlayerVelocity = Component->GetComponentVelocity();
    }

    UpdateCurrentState();

    /*EMovementMode CurrentMode = GetCharacterMovement()->MovementMode;

    FString MovementModeString;

    switch (CurrentMode)
    {
    case MOVE_Walking:
        MovementModeString = "Walking";
        break;
    case MOVE_NavWalking:
        MovementModeString = "NavWalking";
        break;
    case MOVE_Falling:
        MovementModeString = "Falling";
        break;
    case MOVE_Swimming:
        MovementModeString = "Swimming";
        break;
    case MOVE_Flying:
        MovementModeString = "Flying";
        break;
    case MOVE_Custom:
        MovementModeString = "Custom";
        break;
    case MOVE_None:
        MovementModeString = "None";
        break;
    default:
        MovementModeString = "Unknown";
        break;
    }

    //UE_LOG(LogTemp, Warning, TEXT("Current Movement Mode: %s"), *MovementModeString);*/
}




void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //Add input mapping context (PAS TOUT COMPRIS)
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        //Get local player subsystem
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            //Add input context
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }

    //Bind inputs to corresponding actions
    if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        Input->BindAction(MoveRLAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRL);
        Input->BindAction(MoveFBAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveFB);
        Input->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
        Input->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);

    }
}

//PLAYER MOVEMENT
void APlayerCharacter::MoveRL(const FInputActionValue& InputValue)
{
    const float ValueInput = InputValue.Get<float>();
    if (Controller != nullptr && ValueInput != 0.0f)
    {
        AddMovementInput(FVector(1.0f, 0.0f, 0.0f) * Speed, ValueInput);
    }
}

//PLAYER INTERACT
void APlayerCharacter::MoveFB(const FInputActionValue& InputValue)
{
    const float ValueInput = InputValue.Get<float>();
    if (bIsOnLadder && Controller != nullptr && ValueInput != 0.0f)
    {
        AddMovementInput(FVector(0.0f, 0.0f, 1.0f) * Speed, ValueInput);
    }
}

//PLAYER JUMPING
void APlayerCharacter::Jump(const FInputActionValue& InputValue)
{
    GetCharacterMovement()->JumpZVelocity = JumpHeight;
    ACharacter::Jump();
}

//PLAYER INTERACT
void APlayerCharacter::Interact(const FInputActionValue& InputValue)
{
    if (ActorIsOverlaped->GetClass()->ImplementsInterface(UInteractibleInterface::StaticClass()))
    {
        // Appeler la méthode via l'interface
        IInteractibleInterface* InteractibleActor = Cast<IInteractibleInterface>(ActorIsOverlaped);
        if (InteractibleActor)
        {
            InteractibleActor->Effect(); // Appelle la méthode de l'interface
        }
    }
}




//UPDATE ANIMATION
void APlayerCharacter::UpdateCurrentState() 
{
    if (GetCharacterMovement()->IsMovingOnGround()) 
    {
        if (PlayerVelocity.X != 0)
            CurrentStateMovement = "move";
        else
            CurrentStateMovement = "idle";
    }
    else
    {
        if (PlayerVelocity.Z < 0)
        {
            CurrentStateMovement = "fall";
        }
        else
        {
            CurrentStateMovement = "jumpRise";
        }
    }

    UpdateIsFacingLeft();
}

void APlayerCharacter::UpdateIsFacingLeft()
{
    if (PlayerVelocity.X > 0)
        isFacingLeft = false;
    else if (PlayerVelocity.X < 0)
        isFacingLeft = true;
}





void APlayerCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // Overlap
    if (Cast<AInteractible>(OtherActor))
    {
        //APPELER LA FONCTION de l'UI
        ActorIsOverlaped = OtherActor;


        if (OtherActor->ActorHasTag("Ladder"))
        {
            if (!OverlappingLadders.Contains(OtherActor))
            {
                OverlappingLadders.Add(OtherActor);
            }

            if (!bIsOnLadder)
            {
                bIsOnLadder = true;
                GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
            }

        }
    }
}

void APlayerCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex)
{
    // Overlap
    if (Cast<AInteractible>(OtherActor))
    {
        //Stopper LA FONCTION de l'UI
        ActorIsOverlaped = nullptr;

        OverlappingLadders.Remove(OtherActor);

        if (OverlappingLadders.Num() == 0)
        {
            bIsOnLadder = false;
            GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        }
    }
}