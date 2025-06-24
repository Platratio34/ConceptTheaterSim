// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ColorBlendSpace.generated.h"

/**
 * 
 */

struct HSVColor {
    float hue;
    float saturation;
    float value;
};

struct XYYColor {
    float x;
    float y;
    float Y; // luninence
};

UCLASS()
class CONCEPTTHEATERSIM_API UColorBlendSpace : public UObject
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void addColor(TArray<uint8> color);

    UFUNCTION(BlueprintCallable)
    TArray<uint8> blend(TArray<float> weights);

protected:

    TArray<HSVColor> colors;

    float min(float a, float b);
    float max(float a, float b);

    HSVColor rgbToHSV(TArray<uint8> rgb);

    XYYColor rgbToXYY(TArray<uint8> rgb);
    TArray<uint8> xyYToRGB(XYYColor xyY);
};
