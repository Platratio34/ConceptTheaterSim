// Copyright Peter Crall 2025

#pragma once

#include "TheaterSimCore.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "UI/TheaterSimMainHUD.h"
#include "TheaterSimPlayerController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ATheaterSimPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    ATheaterSimPlayerCharacter *getCharacter();
    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void clearObjectInteractionMode();

    UFUNCTION(BlueprintCallable)
    ATheaterSimMainHUD* getMainHUD() {
        return hud;
    }

protected:

    void freeCursor(bool newFree);

    ATheaterSimPlayerCharacter *pc = nullptr;

    // INPUT
    virtual void SetupInputComponent() override;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    UInputMappingContext *defaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category="Input - Camera")
    UInputAction *moveCameraAction;
    UFUNCTION()
    void onCameraMoveInput(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input - Camera")
    UInputAction *zoomAction;
    UFUNCTION()
    void onZoomInputStarted(const FInputActionInstance &value);
    UFUNCTION()
    void onZoomInputEnded(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input - Camera")
    UInputAction *freeLookAction;
    UFUNCTION()
    void onFreeLookInputStarted(const FInputActionInstance &value);
    UFUNCTION()
    void onFreeLookInputEnded(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input - Canera")
    UInputAction *viewModeAction;
    UFUNCTION()
    void onViewModeInput(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input - Movement")
    UInputAction *moveAction;
    UFUNCTION()
    void onMoveInput(const FInputActionInstance &value);
    
    UPROPERTY(EditDefaultsOnly, Category="Input - Movement")
    UInputAction *jumpAction;
    UFUNCTION()
    void onJumpInputStarted(const FInputActionInstance &value);
    UFUNCTION()
    void onJumpInputOngoing(const FInputActionInstance &value);
    UFUNCTION()
    void onJumpInputCompleted(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input - Movement")
    UInputAction *crouchAction;
    UFUNCTION()
    void onCrouchInputStarted(const FInputActionInstance &value);
    UFUNCTION()
    void onCrouchInputEnded(const FInputActionInstance &value);
    UFUNCTION()
    void onCrouchInputTriggered(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input - Interaction")
    UInputAction *interactAction;
    UFUNCTION()
    void onInteractInputStart(const FInputActionInstance &value);
    UFUNCTION()
    void onInteractInputEnd(const FInputActionInstance &value);
    
    UPROPERTY(EditDefaultsOnly, Category="Input - Interaction")
    UInputAction *interactScrollAction;
    UFUNCTION()
    void onInteractScrollInput(const FInputActionInstance &value);
    
    UPROPERTY(EditDefaultsOnly, Category="Input - Interaction")
    UInputAction *interactObjectMode;
    UFUNCTION()
    void onInteractObjectMode(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input - Tools")
    UInputAction *flashlightAction;
    UFUNCTION()
    void onFlashlightInput(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input - Debug")
    UInputAction *emTpAction;
    UFUNCTION()
    void onEmTpInputStarted(const FInputActionInstance &value);
    UFUNCTION()
    void onEmTpInputTriggered(const FInputActionInstance &value);
    UFUNCTION()
    void onEmTpInputCanceled(const FInputActionInstance &value);

    UPROPERTY(EditDefaultsOnly, Category="Input - Debug")
    UInputAction *noClipAction;
    UFUNCTION()
    void onNoClipInputTriggered(const FInputActionInstance &value);

    // Camera
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
    UCInteractionComponent *objectInteractionTarget = nullptr;

    // HUD
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Default")
    ATheaterSimMainHUD *hud = nullptr; // MainHUD
};
