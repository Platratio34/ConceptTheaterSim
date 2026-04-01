// Copyright Peter Crall 2025

#pragma once

#include "TheaterSimCore.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/SpotLightComponent.h"

#include "Cables/CCable.h"

#include "Core/CInteractionComponent.h"

#include "Networking/WirelessNetworkCard.h"

#include "InputAction.h"
#include "InputActionValue.h"

#include "TheaterSimPlayerCharacter.generated.h"

UCLASS(BlueprintType)
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

    void setCable(ACCable *cable);

    UFUNCTION(BlueprintCallable)
    ATheaterSimPlayerController *getPlayerController();

    UFUNCTION(BlueprintCallable)
    void setFlashlight(bool on);
    UFUNCTION(BlueprintCallable)
    void toggleFlashlight();

    UFUNCTION(BlueprintCallable)
    void setCrouched(bool crouched);

    UFUNCTION(BlueprintCallable)
    void onCrouch();

    UFUNCTION(BlueprintCallable)
    bool getCrouched() {
        return isCrouching;
    }

    UFUNCTION()
    void setInteractionSource(bool mouse);

    UFUNCTION()
    void cameraInput(FVector2D move);

    UFUNCTION()
    void movementInput(FVector2D move);

    UFUNCTION()
    void onJump(bool ongoing);

    UFUNCTION()
    void onInteract();

    UFUNCTION()
    UCInteractionComponent *onInteractObjectMode();

    UFUNCTION()
    void onInteractScroll(float scroll);

    UFUNCTION()
    void endInteract();

    UFUNCTION()
    void toggleZoom();

    UFUNCTION()
    void setZoom(bool zoomed);

    UFUNCTION(BlueprintCallable)
    void emTp();

    UFUNCTION(BlueprintCallable)
    void toggleNoClip();

protected:

    void updateInteractionWidget();

    void updateZoom(float deltaTime);

    void updateLadderState(bool newState, AActor *ladder);

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
    bool interactionMouse = false;

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

    // Ladders
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ladder")
    bool onLadder = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ladder")
    TSet<AActor *> ladders;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ladder")
    float ladderMovement = 0.0;
    
    // Other
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Default")
    bool flashlightOn = true;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Default")
    bool isCrouching = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Default")
    bool noClip = false;
};
