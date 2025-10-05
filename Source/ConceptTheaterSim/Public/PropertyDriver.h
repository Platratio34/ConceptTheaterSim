// Copyright Peter Crall 2025.

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
    void UpdateTarget(float target);
    UFUNCTION(BlueprintCallable)
    float Update(float deltaTime);
    UFUNCTION(BlueprintCallable)
    bool IsDone();

    UPROPERTY(EditAnywhere)
    float maxAccl = -1;
    UPROPERTY(EditAnywhere)
    float decelTime = 0;

    float GetValue()
    {
        return value;
    }
    float GetTarget()
    {
        return targetValue;
    }

protected:
    float value;
    float targetValue;
    float time;
    float lastDelta = 0;

    bool done;
};
