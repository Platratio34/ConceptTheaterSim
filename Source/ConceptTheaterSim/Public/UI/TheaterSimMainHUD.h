// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TheaterSimMainHUD.generated.h"

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ATheaterSimMainHUD : public AHUD {
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void setEmTpWarning(bool visible) {
        emTpWarningVisible = visible;
    }

    UFUNCTION(BlueprintCallable)
    void setMinimapVisibility(bool visible) {
        minimapVisible = visible;
    }

protected:

    UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
    bool emTpWarningVisible = false;

    UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
    bool minimapVisible = true;
};