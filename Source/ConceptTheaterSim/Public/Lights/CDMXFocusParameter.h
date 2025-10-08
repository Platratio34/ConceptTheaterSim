// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Lights/CDMXParameter.h"
#include "CDMXFocusParameter.generated.h"

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCDMXFocusParameter : public UCDMXParameter
{
    
    GENERATED_BODY()

public:
    
	UCDMXFocusParameter() {}

    virtual void onDmxUpdate() override
    {
        if(light == nullptr)
            return;
        if(panEnabled && light->hasParameter(panParameter))
            light->setPan(range(light->getParameterNormalized(panParameter), panMin, panMax));
        if(tiltEnabled&& light->hasParameter(tiltParameter))
            light->setTilt(range(light->getParameterNormalized(tiltParameter), tiltMin, tiltMax));
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName panParameter = FName("Pan");
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool panEnabled = true;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double panMin = -180;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double panMax = 180;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName tiltParameter = FName("Tilt");

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool tiltEnabled = true;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double tiltMin = -180;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double tiltMax = 180;
    

private:

};