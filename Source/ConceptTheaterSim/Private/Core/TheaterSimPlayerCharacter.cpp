// Copyright Peter Crall 2025


#include "Core/TheaterSimPlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATheaterSimPlayerCharacter::ATheaterSimPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    playerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
    playerCamera->SetupAttachment(GetRootComponent());
    playerCamera->SetRelativeLocation(FVector(2.54, 0, 66.04));

    widgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction"));
    widgetInteraction->SetupAttachment(playerCamera);
    
    flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
    flashlight->SetupAttachment(playerCamera);
    
    wirelessNetworkCard = CreateDefaultSubobject<UWirelessNetworkCard>(TEXT("Wireless Network Card"));
}

// Called when the game starts or when spawned
void ATheaterSimPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    // Add HUD

    wirelessNetworkCard->connectBySSID(FName("Control"));

    // Find fixed camera
}

// Called every frame
void ATheaterSimPlayerCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

    if(ladderMovement != 0)
    {
        FVector offset;
        float m = ladderMovement;
        if(m > 1)
            m = 1;
        if(m < -1)
            m = -1;
        offset.Z = ladderMovement * 121.92 * deltaTime;
        AddActorWorldOffset(offset, true, nullptr, ETeleportType::TeleportPhysics);
    }

    updateZoom(deltaTime);

    if(interacting && interactable)
    {
        float dist = FVector::Distance(GetActorLocation(), interactionTarget->GetComponentLocation());
        if(dist > (2.54 * maxInteractionDistance))
        {
            endInteract();
            interactable = nullptr;
            interactionTarget = nullptr;
            updateInteractionWidget();
        }
    }
    else
    {
        UCInteractionComponent *newInteractable = nullptr;
        UPrimitiveComponent *newInteractionTarget = nullptr;

        FVector start = playerCamera->GetComponentLocation();
        FVector end = playerCamera->GetForwardVector();
        if(cursorFree)
            getPlayerController()->DeprojectMousePositionToWorld(start, end);
        end *= maxInteractionDistance * 2.54;
        end += start;

        FHitResult hitResult;
        FCollisionQueryParams queryParams;
        if (GetWorld()->LineTraceSingleByChannel(
                hitResult,
                start,
                end,
                ECollisionChannel::ECC_Visibility,
                queryParams
        ))
        {
            if(hitResult.Component->ComponentHasTag(FName("Interactable")))
            {
                newInteractable = hitResult.GetActor()->GetComponentByClass<UCInteractionComponent>();
                newInteractionTarget = hitResult.GetComponent();
            }
            else
            {
                // cable connector?
            }
        }
        if(newInteractable != interactable)
        {
            if(interactable && interacting)
            {
                interactable->endInteract(interactionTarget);
                if(newInteractable)
                {
                    newInteractable->interact(interactionTarget, false);
                }
            }
            interactable = newInteractable;
            updateInteractionWidget();
        }
        if(newInteractionTarget != interactionTarget)
        {
            if(interacting)
            {
                interactable->endInteract(interactionTarget);
                interactable->interact(newInteractionTarget, false);
            }
            interactionTarget = newInteractionTarget;
            updateInteractionWidget();
        }
    }
}

// Called to bind functionality to input
void ATheaterSimPlayerCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);
    if (UEnhancedInputComponent* eI = Cast<UEnhancedInputComponent>(playerInputComponent))
    {
        eI->BindAction(moveCameraAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerCharacter::onCameraMoveInput);
        eI->BindAction(moveAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerCharacter::onMoveInput);
        
        eI->BindAction(jumpAction, ETriggerEvent::Started, this, &ATheaterSimPlayerCharacter::onJumpInputStarted);
        eI->BindAction(jumpAction, ETriggerEvent::Ongoing, this, &ATheaterSimPlayerCharacter::onJumpInputOngoing);
        eI->BindAction(jumpAction, ETriggerEvent::Completed, this, &ATheaterSimPlayerCharacter::onJumpInputCompleted);

        eI->BindAction(interactAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerCharacter::onInteractInputStart);
        eI->BindAction(interactAction, ETriggerEvent::Completed, this, &ATheaterSimPlayerCharacter::onInteractInputEnd);
        eI->BindAction(interactAction, ETriggerEvent::Canceled, this, &ATheaterSimPlayerCharacter::onInteractInputEnd);

        eI->BindAction(interactScrollAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerCharacter::onInteractScrollInput);
        
        eI->BindAction(flashlightAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerCharacter::onFlashlightInput);
        
        eI->BindAction(zoomAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerCharacter::onZoomInputStarted);
        eI->BindAction(zoomAction, ETriggerEvent::Completed, this, &ATheaterSimPlayerCharacter::onZoomInputEnded);
        eI->BindAction(zoomAction, ETriggerEvent::Canceled, this, &ATheaterSimPlayerCharacter::onZoomInputEnded);
        
        eI->BindAction(freeLookAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerCharacter::onFreeLookInputStarted);
        eI->BindAction(freeLookAction, ETriggerEvent::Completed, this, &ATheaterSimPlayerCharacter::onFreeLookInputEnded);
        eI->BindAction(freeLookAction, ETriggerEvent::Canceled, this, &ATheaterSimPlayerCharacter::onFreeLookInputEnded);
        
        eI->BindAction(emTpAction, ETriggerEvent::Started, this, &ATheaterSimPlayerCharacter::onEmTpInputStarted);
        eI->BindAction(emTpAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerCharacter::onEmTpInputTriggered);
        eI->BindAction(emTpAction, ETriggerEvent::Canceled, this, &ATheaterSimPlayerCharacter::onEmTpInputCanceled);
        
        eI->BindAction(crouchAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerCharacter::onCrouchInputStarted);
        eI->BindAction(crouchAction, ETriggerEvent::Completed, this, &ATheaterSimPlayerCharacter::onCrouchInputEnded);
        eI->BindAction(crouchAction, ETriggerEvent::Canceled, this, &ATheaterSimPlayerCharacter::onCrouchInputEnded);
        
        eI->BindAction(viewModeAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerCharacter::onViewModeInput);
    }
}

void ATheaterSimPlayerCharacter::setCable(ACCable *cable)
{
    if(cCable)
        return;
    cCable = cable;
    cable->putInInventory(this);
}

void ATheaterSimPlayerCharacter::updateInteractionWidget()
{
    if(interactable)
    {
        // interactionWidget->setInteraction(interactable->getName(interactionTarget), interactable->getAction(interactionTarget))
    }
    else
    {
        // interactionWidget->clearInteraction();
    }
}

void ATheaterSimPlayerCharacter::updateZoom(float deltaTime)
{
    if(cursorFree || locked)
        return;
    deltaTime *= 300;
    if(zoomedIn)
    {
        if(fov == fovIn)
            return;
        fov -= deltaTime;
        if(fov < fovIn)
            fov = fovIn;
    }
    else
    {
        if(fov == fovOut)
            return;
        fov += deltaTime;
        if(fov > fovOut)
            fov = fovOut;
    }
    playerCamera->SetFieldOfView(fov);
}

void ATheaterSimPlayerCharacter::updateLadderState(bool newState, AActor *ladder)
{
    bool stateChanged = false;
    if(newState)
    {
        ladders.Add(ladder);
        if(!onLadder)
        {
            onLadder = true;
            stateChanged = true;
        }
    }
    else
    {
        ladders.Remove(ladder);
        if(ladders.IsEmpty() && onLadder)
        {
            onLadder = false;
            stateChanged = true;
        }
    }
    if(stateChanged)
    {
        UCharacterMovementComponent* mc = GetCharacterMovement();
        if(onLadder)
        {
            mc->StopMovementImmediately();
        }
        mc->SetMovementMode(onLadder ? EMovementMode::MOVE_Flying : EMovementMode::MOVE_Walking);
    }
}

void ATheaterSimPlayerCharacter::onInteract()
{
    widgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
    if(interactable)
    {
        interactable->interact(interactionTarget, interacting);
        interacting = true;
    }
    // otherwise cable?
}

void ATheaterSimPlayerCharacter::endInteract()
{
    interacting = false;
    widgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
    if(!interactable)
        return;
    interactable->endInteract(interactionTarget);
}

ATheaterSimPlayerController *ATheaterSimPlayerCharacter::getPlayerController()
{
    if(AController* controller = GetController())
    {
        return Cast<ATheaterSimPlayerController>(controller);
    }
    return nullptr;
}

void ATheaterSimPlayerCharacter::freeCursor(bool newFree)
{
    cursorFree = newFree;
    ATheaterSimPlayerController *controller = getPlayerController();
    controller->bShowMouseCursor = cursorFree;

    widgetInteraction->InteractionSource = cursorFree ? EWidgetInteractionSource::Mouse : EWidgetInteractionSource::CenterScreen;
    if(cursorFree)
    {
        FInputModeGameAndUI inputMode;
        inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        inputMode.SetHideCursorDuringCapture(false);
        controller->SetInputMode(inputMode);
        int x = 0;
        int y = 0;
        controller->GetViewportSize(x, y);
        controller->SetMouseLocation(x * 0.5, y * 0.5);
    }
    else
    {
        FInputModeGameOnly inputMode;
        controller->SetInputMode(inputMode);
    }
}

void ATheaterSimPlayerCharacter::onCameraMoveInput(const FInputActionInstance &value)
{
    if(locked || cursorFree)
        return;
    FVector2D move = value.GetValue().Get<FVector2D>();
    AddControllerYawInput(move.X);
    AddControllerPitchInput(move.Y);
}

void ATheaterSimPlayerCharacter::onMoveInput(const FInputActionInstance &value)
{
    if(locked)
        return;
    FVector2D move = value.GetValue().Get<FVector2D>();
    AddMovementInput(GetActorRightVector(), move.X, false);
    if(onLadder && move.Y > 0)
    {
        ladderMovement = move.Y;
    }
    else
    {
        AddMovementInput(GetActorForwardVector(), move.Y, false);
    }
}

void ATheaterSimPlayerCharacter::onJumpInputStarted(const FInputActionInstance &value)
{
    if(locked || onLadder)
        return;
    Jump();
}

void ATheaterSimPlayerCharacter::onJumpInputOngoing(const FInputActionInstance &value)
{
    if(locked)
        return;
    if(onLadder)
        ladderMovement = 1;
}

void ATheaterSimPlayerCharacter::onJumpInputCompleted(const FInputActionInstance &value)
{
    if(locked)
        return;
    StopJumping();
}

void ATheaterSimPlayerCharacter::onInteractInputStart(const FInputActionInstance &value)
{
    if(locked)
        return;
    onInteract();
}

void ATheaterSimPlayerCharacter::onInteractInputEnd(const FInputActionInstance &value)
{
    endInteract();
}

void ATheaterSimPlayerCharacter::onInteractScrollInput(const FInputActionInstance &value)
{
    if(locked || interactable == nullptr)
        return;
    float scroll = value.GetValue().Get<float>();
    widgetInteraction->ScrollWheel(scroll);
    if(interactable != nullptr)
        interactable->interactScroll(interactionTarget, scroll);
}

void ATheaterSimPlayerCharacter::onFlashlightInput(const FInputActionInstance &value)
{
    if(locked)
        return;
    flashlightOn = !flashlightOn;
    flashlight->SetVisibility(flashlightOn);
}

void ATheaterSimPlayerCharacter::onZoomInputStarted(const FInputActionInstance &value)
{
    if(locked)
        return;
    zoomedIn = !zoomedIn;
}

void ATheaterSimPlayerCharacter::onZoomInputEnded(const FInputActionInstance &value)
{
    if(locked)
        return;
    if(value.GetElapsedTime() > 0.5)
        zoomedIn = false;
}

void ATheaterSimPlayerCharacter::onFreeLookInputStarted(const FInputActionInstance &value)
{
    if(locked)
        return;
    freeCursor(!cursorFree);
}

void ATheaterSimPlayerCharacter::onFreeLookInputEnded(const FInputActionInstance &value)
{
    if(locked || value.GetElapsedTime() < 0.5)
        return;
    freeCursor(false);
}

void ATheaterSimPlayerCharacter::onEmTpInputStarted(const FInputActionInstance &value)
{
    if(locked)
        return;
    // mainHud->hideEmTpWarning();
    TArray<AActor *> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), foundActors);
    if(foundActors.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to EM teleport: Could not find start"));
        return;
    }
    SetActorLocation(foundActors[0]->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
}

void ATheaterSimPlayerCharacter::onEmTpInputTriggered(const FInputActionInstance &value)
{
    if(locked)
        return;
    // mainHud->showEmTpWarning();
}

void ATheaterSimPlayerCharacter::onEmTpInputCanceled(const FInputActionInstance &value)
{
    if(locked)
        return;
    // mainHud->hideEmTpWarning();
}

void ATheaterSimPlayerCharacter::onCrouchInputStarted(const FInputActionInstance &value)
{
    if(locked)
        return;
    GetCapsuleComponent()->SetCapsuleHalfHeight(50, true);
    playerCamera->SetRelativeLocation(FVector(2.54, 0, 66.04), false, nullptr, ETeleportType::None);
}

void ATheaterSimPlayerCharacter::onCrouchInputEnded(const FInputActionInstance &value)
{
    if(locked)
        return;
    GetCapsuleComponent()->SetCapsuleHalfHeight(80, true);
    AddActorWorldOffset(FVector(0, 0, 30), false, nullptr, ETeleportType::None);
    playerCamera->SetRelativeLocation(FVector(2.54, 0, 36.04), false, nullptr, ETeleportType::None);
}

void ATheaterSimPlayerCharacter::onViewModeInput(const FInputActionInstance &value)
{
    ATheaterSimPlayerController *controller = getPlayerController();
    if(!fixedCamera)
        return;
    if(isSelfCam)
    {
        controller->SetViewTargetWithBlend(fixedCamera, 0, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
        returnPos = GetActorLocation();
        SetActorLocation(FVector(0, 0, -1117.6), false, nullptr, ETeleportType::TeleportPhysics);
    }
    else
    {
        controller->SetViewTargetWithBlend(this, 0, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
        SetActorLocation(returnPos, false, nullptr, ETeleportType::TeleportPhysics);
    }
    isSelfCam = !isSelfCam;
    // fixedCamera->updateActive(!isSelfCam);
    freeCursor(!isSelfCam);
    locked = !isSelfCam;
    // mainHud->setMinimapVisibility(isSelfCam);
}