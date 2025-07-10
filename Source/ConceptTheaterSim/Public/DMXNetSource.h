// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DMXNetSource.generated.h"

#define DMX_NET_SOURCE_MAX_UNIVERSE 16

UCLASS()
class CONCEPTTHEATERSIM_API UDMXNetSource : public UObject
{
	GENERATED_BODY()    

public:
    FName name;
    int priority[DMX_NET_SOURCE_MAX_UNIVERSE];
    uint8 * universes[DMX_NET_SOURCE_MAX_UNIVERSE];
    bool hasUniverse[DMX_NET_SOURCE_MAX_UNIVERSE];

    UDMXNetSource();
    ~UDMXNetSource();

    bool set(int universe, int priority, TArray<int> data);

    struct DMXNetSourceUniverse {
        int priority;
        uint8 *data;
        bool valid = false;
    };

    DMXNetSourceUniverse getUniverse(int universe);
};