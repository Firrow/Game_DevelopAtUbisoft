// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/TriggerEnding.h"
#include "InputActionValue.h"


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

    GetCharacterMovement()->MaxStepHeight = MaxStepHeightPlayer;

    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
    GameManager = Cast<AGameManager>(FoundActor);
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Component != nullptr)
        PlayerVelocity = Component->GetComponentVelocity();

    UpdateCurrentState();
}




void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //Add input mapping context
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

        Input->BindAction(MoveFBAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndMoveFB);
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
void APlayerCharacter::MoveFB(const FInputActionValue& InputValue)
{
    const float ValueInput = InputValue.Get<float>();

    if (bIsOnLadder && Controller != nullptr && ValueInput != 0.0f)
    {
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
        AddMovementInput(FVector(0.0f, 0.0f, 1.0f) * Speed, ValueInput);
    }
}
void APlayerCharacter::EndMoveFB(const FInputActionValue& InputValue)
{
    if (bIsOnLadder && Controller != nullptr)
    {
        PlayerVelocity.X = 0.0f;
        PlayerVelocity.Y = 0.0f;
        PlayerVelocity.Z = 0.0f;

        GetCharacterMovement()->Velocity = PlayerVelocity;
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

    if (ActorIsOverlaped != nullptr && ActorIsOverlaped->GetClass()->ImplementsInterface(UInteractibleInterface::StaticClass()))
    {
        IInteractibleInterface* InteractibleActor = Cast<IInteractibleInterface>(ActorIsOverlaped);
        if (InteractibleActor)
        {
            InteractibleActor->Effect();
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
        if (bIsOnLadder)
        {
            if (PlayerVelocity.Z != 0)
                CurrentStateMovement = "ladder";
            else 
                CurrentStateMovement = "onPause";
        }
        else 
        {
            if (PlayerVelocity.Z < 0)
                CurrentStateMovement = "fall";
            else
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
    if (Cast<AInteractible>(OtherActor))
    {
        //APPELER LA FONCTION de l'UI

        ActorIsOverlaped = OtherActor;

        if (OtherActor->ActorHasTag("Ladder"))
        {
            if (!OverlappingLadders.Contains(OtherActor))
                OverlappingLadders.Add(OtherActor);

            if (!bIsOnLadder)
                bIsOnLadder = true;
        }
    }
    else 
    {
        if (Cast<ATriggerEnding>(OtherActor))
        {
            //UE_LOG(LogTemp, Display, TEXT("This is the end... My only friend, the end."));
            if (GearsNumber == GameManager->NUMBER_OF_GEARS)
            {
                GameManager->VictoryEndGame();
            }
            else
            {
                GameManager->GameOverEndGame();
            }
        }
    }
}

void APlayerCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex)
{
    if (Cast<AInteractible>(OtherActor))
    {
        //Stopper LA FONCTION de l'UI
        ActorIsOverlaped = nullptr;

        if (OtherActor->ActorHasTag("Ladder"))
        {
            OverlappingLadders.Remove(OtherActor);

            if (OverlappingLadders.Num() == 0)
            {
                bIsOnLadder = false;
                GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
            }
        }
    }
}