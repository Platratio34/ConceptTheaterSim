// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EOS/ETCLightBoard.h"
#include "EOSScreenBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UEOSScreenBase : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    AETCLightBoard* board = nullptr;

protected:
    UFUNCTION(BlueprintCallable)
    FString getCommandString();

    UFUNCTION(BlueprintCallable)
    FLinearColor getCommandColor();
};
