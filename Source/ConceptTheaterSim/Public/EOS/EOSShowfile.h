// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSShowfile.generated.h"

UCLASS()
class CONCEPTTHEATERSIM_API UEOSPatch : public UObject
{
    GENERATED_BODY()

public:
    UEOSPatch() {}
    static UEOSPatch* Create(FName lightType, int dmxSize)
    {
        UEOSPatch *patch = NewObject<UEOSPatch>();
        patch->type = lightType;
        patch->size = dmxSize;
        return patch;
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

    void addProperty(FName property)
    {
        properties.Add(property);
    }
};

UCLASS()
class CONCEPTTHEATERSIM_API UEOSPatchSet : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY()
    int channel;
    
    UPROPERTY()
    TArray<UEOSPatch*> devices;

    bool canAdd(UEOSPatch* device)
    {
        if(devices.Num() == 0)
            return true;
        return devices[0]->type == device->type;
    }
};

UCLASS()
class CONCEPTTHEATERSIM_API UEOSPropertySet : public UObject
{
    GENERATED_BODY()

public:
    static UEOSPropertySet* Create()
    {
        return NewObject<UEOSPropertySet>();
    }

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
        file->fileName = fileName;
        return file;
    }

    UPROPERTY()
    FString fileName;

    UPROPERTY()
    TMap<int, UEOSPatchSet*> patch;

    UPROPERTY()
    TMap<int, UEOSPropertySet*> channels;

    UFUNCTION()
    bool patchLight(int ch, UEOSPatch* light);

    UFUNCTION(BlueprintCallable)
    TArray<int> getPatchedChannels();

    UFUNCTION(BlueprintCallable)
    double getParameter(int ch, FName parameter);
};