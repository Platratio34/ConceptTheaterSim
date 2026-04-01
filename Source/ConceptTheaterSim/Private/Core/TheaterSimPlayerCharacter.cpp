// Copyright Peter Crall 2025


#include "Core/TheaterSimPlayerCharacter.h"
#include "Core/TheaterSimPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "UI/InteractionWidgetBase.h"
#include "Engine/Engine.h"

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
        if(interactionMouse)
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

void ATheaterSimPlayerCharacter::setCable(ACCable *cable)
{
    if(cCable)
        return;
    cCable = cable;
    cable->putInInventory(this);
}

void ATheaterSimPlayerCharacter::updateInteractionWidget() {
    UInteractionWidgetBase *iw = getPlayerController()->getMainHUD()->interactionWidget;
    iw->mouseMode = interactionMouse;
    iw->updateInteractable(interactable, interactionTarget);
}

void ATheaterSimPlayerCharacter::updateZoom(float deltaTime) {
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

void ATheaterSimPlayerCharacter::updateLadderState(bool newState, AActor *ladder) {
    bool stateChanged = false;
    if(newState) {
        ladders.Add(ladder);
        if(!onLadder) {
            onLadder = true;
            stateChanged = true;
        }
    } else {
        ladders.Remove(ladder);
        if(ladders.IsEmpty() && onLadder) {
            onLadder = false;
            stateChanged = true;
        }
    }
    if(stateChanged) {
        UCharacterMovementComponent* mc = GetCharacterMovement();
        if(onLadder)
            mc->StopMovementImmediately();
        if(!noClip)
            mc->SetMovementMode(onLadder ? EMovementMode::MOVE_Flying : EMovementMode::MOVE_Walking);
    }
}

void ATheaterSimPlayerCharacter::onInteract() {
    widgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
    if(interactable)
    {
        interactable->interact(interactionTarget, interacting);
        interacting = true;
    }
    // otherwise cable?
}

UCInteractionComponent *ATheaterSimPlayerCharacter::onInteractObjectMode() {
    if(!interactable || !interactable->supportsObjectMode)
        return nullptr;
    return interactable;
}

void ATheaterSimPlayerCharacter::onInteractScroll(float scroll) {
    widgetInteraction->ScrollWheel(scroll);
    if(interactable != nullptr)
        interactable->interactScroll(interactionTarget, scroll);
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

void ATheaterSimPlayerCharacter::setInteractionSource(bool mouse) {
    interactionMouse = mouse;
    widgetInteraction->InteractionSource = mouse ? EWidgetInteractionSource::Mouse : EWidgetInteractionSource::CenterScreen;
}

void ATheaterSimPlayerCharacter::cameraInput(FVector2D move)
{
    AddControllerYawInput(move.X);
    AddControllerPitchInput(move.Y);
}

void ATheaterSimPlayerCharacter::movementInput(FVector2D move) {
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

void ATheaterSimPlayerCharacter::onJump(bool ongoing) {
    if(ongoing) {
        if(onLadder)
            ladderMovement = 1;
    } else {
        if(onLadder)
            return;
        Jump();
    }
}

void ATheaterSimPlayerCharacter::setFlashlight(bool on) {
    flashlightOn = on;
    flashlight->SetVisibility(flashlightOn);
}

void ATheaterSimPlayerCharacter::toggleFlashlight() {
    setFlashlight(!flashlightOn);
}

void ATheaterSimPlayerCharacter::toggleZoom() {
    zoomedIn = !zoomedIn;
}

void ATheaterSimPlayerCharacter::setZoom(bool zoomed) {
    zoomedIn = zoomed;
}

void ATheaterSimPlayerCharacter::setCrouched(bool crouched) {
    if(noClip || crouched == isCrouching)
        return;
    isCrouching = crouched;
    if(isCrouching) {
        GetCapsuleComponent()->SetCapsuleHalfHeight(50, true);
        playerCamera->SetRelativeLocation(FVector(2.54, 0, 66.04), false, nullptr, ETeleportType::None);
    } else {
        GetCapsuleComponent()->SetCapsuleHalfHeight(80, true);
        AddActorWorldOffset(FVector(0, 0, 30), false, nullptr, ETeleportType::None);
        playerCamera->SetRelativeLocation(FVector(2.54, 0, 36.04), false, nullptr, ETeleportType::None);
    }
}
void ATheaterSimPlayerCharacter::onCrouch() {
    if(!noClip)
        return;
    AddMovementInput(GetActorUpVector(), -1, true);
}

void ATheaterSimPlayerCharacter::emTp() {
    TArray<AActor *> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), foundActors);
    if(foundActors.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to EM teleport: Could not find start"));
        return;
    }
    SetActorLocation(foundActors[0]->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
}

void ATheaterSimPlayerCharacter::toggleNoClip() {
    noClip = !noClip;
    UCharacterMovementComponent* mc = GetCharacterMovement();
    UCapsuleComponent *capsule = GetCapsuleComponent();
    if(noClip) {
        mc->StopMovementImmediately();
        mc->SetMovementMode(EMovementMode::MOVE_Flying);
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("NoClip Active")));
        capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
        capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
    } else {
        mc->SetMovementMode(EMovementMode::MOVE_Walking);
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("NoClip Inactive")));
        capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
        capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    }
}