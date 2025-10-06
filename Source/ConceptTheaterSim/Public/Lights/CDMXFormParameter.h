// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Lights/CDMXParameter.h"
#include "CDMXFormParameter.generated.h"

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCDMXFormParameter : public UCDMXParameter
{
    
    GENERATED_BODY()

public:
    
	UCDMXFormParameter() {}

    virtual void onDmxUpdate() override
    {
        if(light == nullptr)
            return;
        if(zoomEnabled && light->hasParameter(zoomParameter))
            light->setZoom(range(light->getParameterNormalized(zoomParameter), zoomMin, zoomMax));
        if(edgeEnabled&& light->hasParameter(edgeParameter))
            light->setEdge(light->getParameterNormalized(edgeParameter));
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName zoomParameter = FName("Zoom");
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool zoomEnabled = true;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double zoomMin = 5;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double zoomMax = 50;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName edgeParameter = FName("Edge");

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool edgeEnabled = true;
    

private:

};