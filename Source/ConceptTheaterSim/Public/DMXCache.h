// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UDMXNetSource.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DMXCache.generated.h"

/**
 * 
 */
UCLASS()
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
    TMap<int, TArray<int>> cache;

    TMap<FName, UDMXNetSource*> sources;
};