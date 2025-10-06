// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Lights/CDMXParameter.h"
#include "CDMXColorParameter.generated.h"

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FColorParameter
{
    
    
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName parameterId;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor color;
};

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCDMXColorParameter : public UCDMXParameter
{
    
    GENERATED_BODY()

public:
    
	UCDMXColorParameter() {}

    virtual void onDmxUpdate() override
    {
        if(light == nullptr)
            return;
        FLinearColor c = subtractive ? FLinearColor(1.0, 1.0, 1.0, 1.0) : FLinearColor(0.0, 0.0, 0.0, 1.0);
        for(FColorParameter p : parameters)
        {
            double v = light->getParameterNormalized(p.parameterId);
            if(subtractive)
            {
                c.R -= (1.0-p.color.R) * v;
                c.G -= (1.0-p.color.G) * v;
                c.B -= (1.0-p.color.B) * v;
            }
            else
            {
                c.R += p.color.R * v;
                c.G += p.color.G * v;
                c.B += p.color.B * v;
            }
        }
        double max = FMath::Max(c.R, FMath::Max(c.G, c.B));
        c.R /= max;
        c.G /= max;
        c.B /= max;
        light->setColor(c);
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FColorParameter> parameters;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool subtractive = false;

private:

};