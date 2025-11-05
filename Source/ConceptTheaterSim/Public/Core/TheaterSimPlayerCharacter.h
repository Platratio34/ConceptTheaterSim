// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/SpotLightComponent.h"

#include "Cables/CCable.h"

#include "Core/CInteractionComponent.h"
#include "Core/TheaterSimPlayerController.h"

#include "Networking/WirelessNetworkCard.h"

#include "InputAction.h"
#include "InputActionValue.h"

#include "TheaterSimPlayerCharacter.generated.h"

UCLASS()
class CONCEPTTHEATERSIM_API ATheaterSimPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATheaterSimPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void setCable(ACCable *cable);

    ATheaterSimPlayerController *getPlayerController();

protected:
    void updateInteractionWidget();

    void updateZoom(float deltaTime);

    void updateLadderState(bool newState, AActor *ladder);

    void onInteract();

    void endInteract();

    void freeCursor(bool newFree);

    // Components

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UCameraComponent *playerCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UWidgetInteractionComponent *widgetInteraction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    USpotLightComponent *flashlight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UWirelessNetworkCard *wirelessNetworkCard;

    // Interaction
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Interaction")
    UCInteractionComponent *interactable = nullptr;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Interaction")
    UPrimitiveComponent *interactionTarget = nullptr;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Interaction")
    bool interacting = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Interaction")
    UObject *interactionWidget; // InteractionWidget

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    float maxInteractionDistance = 180;

    // Cables
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Inventory")
    ACCable *cCable = nullptr;

    // Screen
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Camera")
    bool zoomedIn = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Camera")
    float fovOut = 90;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Camera")
    float fovIn = 40;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Camera")
    float fov = fovOut;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Camera")
    bool cursorFree = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Camera")
    AActor *fixedCamera = nullptr; // FixedCamera

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Camera")
    bool isSelfCam = true;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Camera")
    FVector returnPos;

    // Other
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Default")
    bool locked = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Default")
    bool flashlightOn = true;

    // HUD
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Default")
    UObject *hud = nullptr; // MainHUD

    // Ladders
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ladder")
    bool onLadder = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ladder")
    TSet<AActor *> ladders;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ladder")
    float ladderMovement = 0.0;
    
    // Input
    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *moveCameraAction;
    UFUNCTION()
    void onCameraMoveInput(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *moveAction;
    UFUNCTION()
    void onMoveInput(const FInputActionInstance &value);
    
    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *jumpAction;
    UFUNCTION()
    void onJumpInputStarted(const FInputActionInstance &value);
    UFUNCTION()
    void onJumpInputOngoing(const FInputActionInstance &value);
    UFUNCTION()
    void onJumpInputCompleted(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *interactAction;
    UFUNCTION()
    void onInteractInputStart(const FInputActionInstance &value);
    UFUNCTION()
    void onInteractInputEnd(const FInputActionInstance &value);
    
    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *interactScrollAction;
    UFUNCTION()
    void onInteractScrollInput(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *flashlightAction;
    UFUNCTION()
    void onFlashlightInput(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *zoomAction;
    UFUNCTION()
    void onZoomInputStarted(const FInputActionInstance &value);
    UFUNCTION()
    void onZoomInputEnded(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *freeLookAction;
    UFUNCTION()
    void onFreeLookInputStarted(const FInputActionInstance &value);
    UFUNCTION()
    void onFreeLookInputEnded(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *emTpAction;
    UFUNCTION()
    void onEmTpInputStarted(const FInputActionInstance &value);
    UFUNCTION()
    void onEmTpInputTriggered(const FInputActionInstance &value);
    UFUNCTION()
    void onEmTpInputCanceled(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *crouchAction;
    UFUNCTION()
    void onCrouchInputStarted(const FInputActionInstance &value);
    UFUNCTION()
    void onCrouchInputEnded(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputAction *viewModeAction;
    UFUNCTION()
    void onViewModeInput(const FInputActionInstance &value);
};
