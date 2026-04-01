// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/InteractionWidgetBase.h"
#include "CPerson.h"
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

    UFUNCTION(BlueprintImplementableEvent)
    void setLoading(bool loading, const FText& showName);
    
    UFUNCTION(BlueprintImplementableEvent)
    void clearLoading();

    UFUNCTION(BlueprintImplementableEvent)
    void openPersonMenu(ACPerson *person);

    UPROPERTY(BlueprintReadWrite)
    UInteractionWidgetBase *interactionWidget;

    UFUNCTION(BlueprintNativeEvent)
    UUserWidget* getWidget();

protected:

    UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
    bool emTpWarningVisible = false;

    UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
    bool minimapVisible = true;

    UUserWidget* getWidget_Implementation() {
        return nullptr;
    }
};