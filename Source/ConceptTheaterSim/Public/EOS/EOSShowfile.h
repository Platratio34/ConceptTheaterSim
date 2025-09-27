// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSSHowfile.generated.h"

USTRUCT()
struct CONCEPTTHEATERSIM_API FEOSPatch
{
    GENERATED_BODY()

public:
    FEOSPatch() {}
    FEOSPatch(FName type_, int size_)
    {
        type = type_;
        size = size_;
    }

    UPROPERTY()
    int universe = 1;
    UPROPERTY()
    int address = 1;
    UPROPERTY()
    int size = 1;

    UPROPERTY()
    FName type;

    UPROPERTY()
    FString label;

    UPROPERTY()
    TSet<FName> properties;
};

USTRUCT()
struct CONCEPTTHEATERSIM_API FEOSPatchSet
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FEOSPatch> devices;

    bool canAdd(FEOSPatch device)
    {
        if(devices.Num() == 0)
            return true;
        return devices[0].type == device.type;
    }
};

USTRUCT()
struct CONCEPTTHEATERSIM_API FEOSPropertySet
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TMap<FName, double> properties;

    bool has(FName property)
    {
        return properties.Contains(property);
    }

    double get(FName property)
    {
        if(double* v = properties.Find(property))
        {
            return *v;
        }
        return 0;
    }

    void set(FName property, double value)
    {
        if(double* v = properties.Find(property))
        {
            properties.Add(property, value);
        }
    }

    void add(FName property, double value)
    {
        properties.Add(property, value);
    }

    double operator[](FName property)
    {
        return get(property);
    }
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UEOSShowfile : public UObject
{
    GENERATED_BODY()

public:
    UEOSShowfile();
    ~UEOSShowfile();

    UFUNCTION()
    static UEOSShowfile* create(FString fileName)
    {
        UEOSShowfile* file = NewObject<UEOSShowfile>();
        file->fileName;
        return file;
    }

    UPROPERTY()
    FString fileName;

    UPROPERTY()
    TMap<int, FEOSPatchSet> patch;

    UPROPERTY()
    TMap<int, FEOSPropertySet> channels;

    UFUNCTION()
    bool patchLight(int ch, FEOSPatch light);

    UFUNCTION(BlueprintCallable)
    TArray<int> getPatchedChannels();

    UFUNCTION(BlueprintCallable)
    double getParameter(int ch, FName parameter);
};