// Copyright Peter Crall 2025


#include "Core/TheaterSimPlayerController.h"
#include "Core/TheaterSimPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ATheaterSimPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if(UEnhancedInputLocalPlayerSubsystem* inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        inputSubsystem->AddMappingContext(defaultMappingContext, 0);
    }
}

void ATheaterSimPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* eI = Cast<UEnhancedInputComponent>(InputComponent)) {
        eI->BindAction(moveCameraAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerController::onCameraMoveInput);
        eI->BindAction(moveAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerController::onMoveInput);
        
        eI->BindAction(jumpAction, ETriggerEvent::Started, this, &ATheaterSimPlayerController::onJumpInputStarted);
        eI->BindAction(jumpAction, ETriggerEvent::Ongoing, this, &ATheaterSimPlayerController::onJumpInputOngoing);
        eI->BindAction(jumpAction, ETriggerEvent::Completed, this, &ATheaterSimPlayerController::onJumpInputCompleted);

        eI->BindAction(interactAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerController::onInteractInputStart);
        eI->BindAction(interactAction, ETriggerEvent::Completed, this, &ATheaterSimPlayerController::onInteractInputEnd);
        eI->BindAction(interactAction, ETriggerEvent::Canceled, this, &ATheaterSimPlayerController::onInteractInputEnd);

        eI->BindAction(interactScrollAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerController::onInteractScrollInput);
        
        eI->BindAction(flashlightAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerController::onFlashlightInput);
        
        eI->BindAction(zoomAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerController::onZoomInputStarted);
        eI->BindAction(zoomAction, ETriggerEvent::Completed, this, &ATheaterSimPlayerController::onZoomInputEnded);
        eI->BindAction(zoomAction, ETriggerEvent::Canceled, this, &ATheaterSimPlayerController::onZoomInputEnded);
        
        eI->BindAction(freeLookAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerController::onFreeLookInputStarted);
        eI->BindAction(freeLookAction, ETriggerEvent::Completed, this, &ATheaterSimPlayerController::onFreeLookInputEnded);
        eI->BindAction(freeLookAction, ETriggerEvent::Canceled, this, &ATheaterSimPlayerController::onFreeLookInputEnded);
        
        eI->BindAction(emTpAction, ETriggerEvent::Started, this, &ATheaterSimPlayerController::onEmTpInputStarted);
        eI->BindAction(emTpAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerController::onEmTpInputTriggered);
        eI->BindAction(emTpAction, ETriggerEvent::Canceled, this, &ATheaterSimPlayerController::onEmTpInputCanceled);
        
        eI->BindAction(crouchAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerController::onCrouchInputStarted);
        eI->BindAction(crouchAction, ETriggerEvent::Completed, this, &ATheaterSimPlayerController::onCrouchInputEnded);
        eI->BindAction(crouchAction, ETriggerEvent::Canceled, this, &ATheaterSimPlayerController::onCrouchInputEnded);
        
        eI->BindAction(viewModeAction, ETriggerEvent::Triggered, this, &ATheaterSimPlayerController::onViewModeInput);
    }
}

void ATheaterSimPlayerController::Tick(float deltaTime) {
    if(!IsValid(pc))
        pc = nullptr;
    if(pc == nullptr) {
        getCharacter();
    }
}

ATheaterSimPlayerCharacter *ATheaterSimPlayerController::getCharacter()
{
    if(pc != nullptr)
        return pc;
    if(ACharacter* character = GetCharacter())
    {
        pc = Cast<ATheaterSimPlayerCharacter>(character);
        return pc;
    }
    return nullptr;
}

void ATheaterSimPlayerController::freeCursor(bool newFree)
{
    if(cursorFree == newFree)
        return;
    cursorFree = newFree;
    bShowMouseCursor = cursorFree;

    pc->setInteractionSource(cursorFree);
    if(cursorFree)
    {
        FInputModeGameAndUI inputMode;
        inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        inputMode.SetHideCursorDuringCapture(false);
        SetInputMode(inputMode);
        int x = 0;
        int y = 0;
        GetViewportSize(x, y);
        SetMouseLocation(x * 0.5, y * 0.5);
    }
    else
    {
        FInputModeGameOnly inputMode;
        SetInputMode(inputMode);
    }
}

void ATheaterSimPlayerController::onCameraMoveInput(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr) || cursorFree)
        return;
    pc->cameraInput(value.GetValue().Get<FVector2D>());
}

void ATheaterSimPlayerController::onMoveInput(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    FVector2D move = value.GetValue().Get<FVector2D>();
    pc->movementInput(value.GetValue().Get<FVector2D>());
}

void ATheaterSimPlayerController::onJumpInputStarted(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    pc->onJump(false);
}

void ATheaterSimPlayerController::onJumpInputOngoing(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    pc->onJump(true);
}

void ATheaterSimPlayerController::onJumpInputCompleted(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    pc->StopJumping();
}

void ATheaterSimPlayerController::onInteractInputStart(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    pc->onInteract();
}

void ATheaterSimPlayerController::onInteractInputEnd(const FInputActionInstance &value)
{
    if(pc == nullptr)
        return;
    pc->endInteract();
}

void ATheaterSimPlayerController::onInteractScrollInput(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    pc->onInteractScroll(value.GetValue().Get<float>());
}

void ATheaterSimPlayerController::onFlashlightInput(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    pc->toggleFlashlight();
}

void ATheaterSimPlayerController::onZoomInputStarted(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    pc->toggleZoom();
}

void ATheaterSimPlayerController::onZoomInputEnded(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    if(value.GetElapsedTime() > 0.5)
        pc->setZoom(false);
}

void ATheaterSimPlayerController::onFreeLookInputStarted(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    freeCursor(!cursorFree);
}

void ATheaterSimPlayerController::onFreeLookInputEnded(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr) || value.GetElapsedTime() < 0.5)
        return;
    freeCursor(false);
}

void ATheaterSimPlayerController::onEmTpInputStarted(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    hud->setEmTpWarning(true);
}

void ATheaterSimPlayerController::onEmTpInputTriggered(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    hud->setEmTpWarning(false);
    pc->emTp();
}

void ATheaterSimPlayerController::onEmTpInputCanceled(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    hud->setEmTpWarning(false);
}

void ATheaterSimPlayerController::onCrouchInputStarted(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    pc->setCrouched(!pc->getCrouched());
}

void ATheaterSimPlayerController::onCrouchInputEnded(const FInputActionInstance &value)
{
    if(locked || (pc == nullptr))
        return;
    if(value.GetElapsedTime() > 0.25)
        pc->setCrouched(false);
}

void ATheaterSimPlayerController::onViewModeInput(const FInputActionInstance &value)
{
    if(!fixedCamera)
        return;
    if(isSelfCam)
    {
        SetViewTargetWithBlend(fixedCamera, 0, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
        returnPos = pc->GetActorLocation();
        pc->SetActorLocation(FVector(0, 0, -1117.6), false, nullptr, ETeleportType::TeleportPhysics);
    }
    else
    {
        SetViewTargetWithBlend(this, 0, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
        pc->SetActorLocation(returnPos, false, nullptr, ETeleportType::TeleportPhysics);
    }
    isSelfCam = !isSelfCam;
    // fixedCamera->updateActive(!isSelfCam);
    freeCursor(!isSelfCam);
    locked = !isSelfCam;
    hud->setMinimapVisibility(isSelfCam);
}