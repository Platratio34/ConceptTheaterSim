// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DMXNetSource.generated.h"

UCLASS()
class CONCEPTTHEATERSIM_API UDMXNetSource : public UObject
{
	GENERATED_BODY()    

public:
    int priority[16];
    int * universes[16];
    bool hasUniverse[16];

    UDMXNetSource();
    ~UDMXNetSource();

    bool set(int universe, int priority, TArray<int> data);

    struct DMXNetSourceUniverse {
        int priority;
        int *data;
        bool valid = false;
    };

    DMXNetSourceUniverse getUniverse(int universe);
};