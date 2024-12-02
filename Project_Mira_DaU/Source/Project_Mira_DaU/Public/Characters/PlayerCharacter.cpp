// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


APlayerCharacter::APlayerCharacter() 
{
    UCapsuleComponent* ObjectCapsule = GetCapsuleComponent();
    ObjectCapsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::BeginOverlap);

    Tags.Add(TEXT("Player"));
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    Component = this->GetRootComponent();
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Component != nullptr)
    {
        PlayerVelocity = Component->GetComponentVelocity();
    }

    UpdateCurrentState();
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
        Input->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
        Input->BindAction(LadderAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LadderMove);

    }
}

//PLAYER MOVEMENT
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
        // Appeler la m�thode via l'interface
        IInteractibleInterface* InteractibleActor = Cast<IInteractibleInterface>(ActorIsOverlaped);
        if (InteractibleActor)
        {
            InteractibleActor->Effect(); // Appelle la m�thode de l'interface
        }
    }
}

//PLAYER INTERACT
void APlayerCharacter::LadderMove(const FInputActionValue& InputValue)
{
    /*if (ActorIsOverlaped->GetClass()->ImplementsInterface(UInteractibleInterface::StaticClass()))
    {
        // Appeler la m�thode via l'interface
        IInteractibleInterface* InteractibleActor = Cast<IInteractibleInterface>(ActorIsOverlaped);
        if (InteractibleActor)
        {
            InteractibleActor->Effect(); // Appelle la m�thode de l'interface
        }
    }*/

    //TODO : Coder le mouvement du joueur sur l'�chelle (elle n'a pas d'effet)
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
        if (PlayerVelocity.Z > 0)
            CurrentStateMovement = "jumpRise";
        else
            CurrentStateMovement = "fall";
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
    }
}

void APlayerCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // Overlap
    if (Cast<AInteractible>(OtherActor))
    {
        //Stopper LA FONCTION de l'UI
        ActorIsOverlaped = nullptr;
    }
}