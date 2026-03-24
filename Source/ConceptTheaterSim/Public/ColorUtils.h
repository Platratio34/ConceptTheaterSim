// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ColorUtils.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API UColorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

    // TArray<int> BlendHSV();

    UFUNCTION(BlueprintCallable, Category = "Color")
    static FColor HexToColor(const FString& hex) {
        return FColor::FromHex(hex);
    }
};
