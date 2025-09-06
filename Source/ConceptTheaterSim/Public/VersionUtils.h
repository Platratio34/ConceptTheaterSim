// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VersionUtils.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API UVersionUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
    UFUNCTION(BlueprintCallable)
    static FString GetProjectVersion();
};
