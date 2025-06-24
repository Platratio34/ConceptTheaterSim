// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UDMXNetSource.generated.h"

UCLASS()
class CONCEPTTHEATERSIM_API UDMXNetSource : public UObject
{
	GENERATED_BODY()

public:
    TMap<int, int> priority;
    TMap<int, int*> universes;

    bool set(int universe, int priority, TArray<int> data);

    struct DMXNetSourceUniverse {
        int priority;
        int *data;
        bool valid = false;
    };

    DMXNetSourceUniverse getUniverse(int universe);
};