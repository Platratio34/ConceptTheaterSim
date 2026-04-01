// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidgetBase.generated.h"

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UInteractionWidgetBase : public UUserWidget {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
    bool mouseMode;

    UFUNCTION(BlueprintImplementableEvent)
    void updateInteractable(UCInteractionComponent *interactable, USceneComponent *component);
};