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
    UPROPERTY(VisibleInstanceOnly)
    float value;
    UPROPERTY(VisibleInstanceOnly)
    float targetValue;
    UPROPERTY(VisibleInstanceOnly)
    float time;
    UPROPERTY(VisibleInstanceOnly)
    float lastDelta = 0;

    UPROPERTY(VisibleInstanceOnly)
    bool done;
};
