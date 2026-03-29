// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NetworkUtils.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API UNetworkUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network Util")
    static int IPToNumber(uint8 o1, uint8 o2, uint8 o3, uint8 o4);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network Util")
    static FString NumberToIPV4(int address);
};
