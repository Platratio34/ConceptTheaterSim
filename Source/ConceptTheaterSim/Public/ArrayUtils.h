// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArrayUtils.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API UArrayUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
    UFUNCTION(BlueprintCallable)
    TArray<FName> sortNames(TArray<FName> input);
};
