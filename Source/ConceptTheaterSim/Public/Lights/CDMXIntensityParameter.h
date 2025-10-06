// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Lights/CDMXParameter.h"
#include "CDMXIntensityParameter.generated.h"

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCDMXIntensityParameter : public UCDMXParameter
{
    
    GENERATED_BODY()

public:
    
	UCDMXIntensityParameter() {}

    virtual void onDmxUpdate() override
    {
        if(light == nullptr)
            return;
        if(light->hasParameter(parameter))
            light->setIntensity(light->getParameterNormalized(parameter));
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName parameter = FName("Intensity");
    

private:

};