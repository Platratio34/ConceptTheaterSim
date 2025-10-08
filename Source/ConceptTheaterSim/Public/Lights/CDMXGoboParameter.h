// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Lights/CDMXParameter.h"
#include "CDMXGoboParameter.generated.h"

UENUM(BlueprintType)
enum class FGoboRotateAction : uint8
{
    INDEX,
    ROTATE,
    STOP
};

USTRUCT(BlueprintType)
struct FGoboRotateMode
{
    
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FGoboRotateAction action;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int minAddress = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int maxAddress = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double dir = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double minValue = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double maxValue = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool fine = false;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName fineParameter;
};

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCDMXGoboParameter : public UCDMXParameter
{
    
    GENERATED_BODY()

public:
    
	UCDMXGoboParameter() {}

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override
    {
        if(abs(rotSpeed) > 0.001)
        {
            cIndex += rotSpeed * DeltaTime;
            while(cIndex > 1)
            {
                cIndex -= 1;
            }
            light->setGoboRotation(cIndex);
        }
    }

    virtual void onDmxUpdate() override
    {
        if(light == nullptr)
            return;
        // if(zoomEnabled && light->hasParameter(zoomParameter))
        //     light->setZoom(range(light->getParameterNormalized(zoomParameter), zoomMin, zoomMax));
        // if(edgeEnabled&& light->hasParameter(edgeParameter))
        //     light->setEdge(light->getParameterNormalized(edgeParameter));

        FName goboId = light->getParameterIndexed(selectParameter);
        if(UTexture2D** p = gobos.Find(goboId))
        {
            UTexture2D *gobo = *p;
            if(gobo != lastGobo)
            {
                lastGobo = gobo;
                light->setGobo(gobo);
            }
        }

        if(rotateEnabled)
        {
            rotateModeName = light->getParameterIndexed(rotateParameter);
            if(FGoboRotateMode *p = rotateModes.Find(rotateModeName))
            {
                rotateMode = *p;

                double v = light->getParameter(rotateParameter) - rotateMode.minAddress;
                double max = (rotateMode.maxAddress - rotateMode.minAddress);
                if(rotateMode.fine)
                {
                    v *= 256;
                    v += light->getParameter(rotateMode.fineParameter);
                    max = (max * 256) + 255;
                }
                v /= max;
                cValue = v;

                if(rotateMode.action == FGoboRotateAction::INDEX)
                {
                    cIndex = (v * (rotateMode.maxValue - rotateMode.minValue) + rotateMode.minValue);
                    rotSpeed = 0;
                    light->setGoboRotation(cIndex);
                }
                if(rotateMode.action == FGoboRotateAction::STOP)
                {
                    rotSpeed = 0;
                }
                else if(rotateMode.action == FGoboRotateAction::ROTATE)
                {
                    rotSpeed = rotateMode.dir * (v * (rotateMode.maxValue - rotateMode.minValue) + rotateMode.minValue);
                }
            }
            else
            {
                rotSpeed = 0;
            }
        }
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName selectParameter = FName("Gobo Select");
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TMap<FName, UTexture2D*> gobos;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName rotateParameter = FName("Gobo Rotate");

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool rotateEnabled = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TMap<FName, FGoboRotateMode> rotateModes;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    double cIndex = 0;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    double rotSpeed = 0;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    double cValue = 0;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    UTexture2D *lastGobo = nullptr;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    FName rotateModeName;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    FGoboRotateMode rotateMode;

private:
};