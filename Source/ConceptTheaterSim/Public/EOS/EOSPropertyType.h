// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonUtilities.h"
#include "EOSPropertyType.generated.h"

UCLASS()
class CONCEPTTHEATERSIM_API UEOSPropertyIndex : public UObject
{
    GENERATED_BODY()

public:
    FName name;
    double min;
    double max;
    bool variable = false;

    static UEOSPropertyIndex *Create(FName name, double min, double max);
    static UEOSPropertyIndex *Create(FName name, double min, double max, bool variable);
};

UCLASS()
class CONCEPTTHEATERSIM_API UEOSPropertyType : public UObject
{
    GENERATED_BODY()

public:
    FName property;
    double min = 0;
    double max = 1;
    double def = 0;
    double encoderStep = 0.05;

    bool indexed;
    TArray<UEOSPropertyIndex*> indexes;

    static UEOSPropertyType *Create(FName name);
    static UEOSPropertyType *Create(FName name, double def);
    static UEOSPropertyType *Create(FName name, double def, double min, double max, double step);
    static UEOSPropertyType *Create(FName name, TArray<UEOSPropertyIndex*> indexes);

    bool isValid(double value);

    FName getIndex(double value);
    double getValue(FName index);
    double nextIndex(double value);
    double lastIndex(double value);
    double indexStep(double value);
};