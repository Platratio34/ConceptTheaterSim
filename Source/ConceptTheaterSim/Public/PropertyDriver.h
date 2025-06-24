// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PropertyDriver.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UPropertyDriver : public UObject
{
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void SetValue(float value);
    UFUNCTION(BlueprintCallable)
    void SetTarget(float target, float time);
    UFUNCTION(BlueprintCallable)
    float Update(float deltaTime);
    UFUNCTION(BlueprintCallable)
    bool IsDone();

protected:
    float value;
    float targetValue;
    float time;
};
