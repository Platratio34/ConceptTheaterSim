// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Lights/CDMXParameter.h"
#include "CDMXShutterParameter.generated.h"

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCDMXShutterParameter : public UCDMXParameter
{
    
    GENERATED_BODY()

public:
    
	UCDMXShutterParameter() {}

    virtual void onDmxUpdate() override
    {
        if(light == nullptr)
            return;
        
        if(useABPos)
        {
            double posA = light->getParameterNormalized(posAParameter);
            double posB = light->getParameterNormalized(posBParameter);
            light->shutterPositions[shutterIndex] = FShutterPosition::CreateAB(posA, posB);
        }
        else
        {
            double pos = light->getParameterNormalized(posParameter);
            double rot = light->getParameterNormalized(rotParameter);
            rot = (rot - 0.5) * (rotRange * 2);
            light->shutterPositions[shutterIndex] = FShutterPosition::Create(pos, rot);
        }
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int shutterIndex = 0;


    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName posParameter = FName("Shutter A Pos");

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName rotParameter = FName("Shutter A Rot");

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double rotRange = 45;
    
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool useABPos = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName posAParameter = FName("Shutter A Pos");

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName posBParameter = FName("Shutter A Rot");
    

private:

};