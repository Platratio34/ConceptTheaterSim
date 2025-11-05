// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "TheaterSimUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API UTheaterSimUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
    bool isTouchscreenMode()
    {
        return touchscreenMode;
    }
    void setTouchscreenMode(bool newMode)
    {
        touchscreenMode = newMode;
    }
protected:
    UPROPERTY(Config)
    bool touchscreenMode = false;
};
