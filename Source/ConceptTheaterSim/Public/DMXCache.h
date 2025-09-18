// Fill out your copyright notice in the Description page of Project Settings.

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

    UPROPERTY()
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
    UPROPERTY()
    TMap<int, FUniverse> cache;

    UPROPERTY()
    TMap<FName, UDMXNetSource*> sources;
};