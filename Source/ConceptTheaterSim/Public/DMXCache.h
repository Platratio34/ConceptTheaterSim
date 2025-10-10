// Copyright Peter Crall 2025.

#pragma once

#include "DMXNetSource.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DMXCache.generated.h"

USTRUCT()
struct FUniverse {
    GENERATED_BODY()

public:
    FUniverse() {
        
    }
    FUniverse(TArray<int> dmx) : data(dmx) {
        
    }

    UPROPERTY(VisibleInstanceOnly)
    TArray<int> data;
};

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UDMXCache : public UObject
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    bool updateSource(FName name, int priority, int universe, TArray<int> data);

    UFUNCTION(BlueprintCallable)
    TArray<int> getData(int universe);

protected:
    // source cache?
    //
    UPROPERTY(VisibleInstanceOnly, Category="DMX Cache")
    TMap<int, FUniverse> cache;

    UPROPERTY(VisibleInstanceOnly, Category="DMX Cache")
    TMap<FName, UDMXNetSource*> sources;
};