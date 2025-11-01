// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonUtilities.h"
#include "EOSShowfile.generated.h"

/*
JSON:

{
    "version": 1,
    "patch": [
        CHANNEL_PATCH
    ],
    "cues": [
        CUE
    ]
}

CHANNEL_PATCH: {
    "ch": [channel number]
    "type": "[light type]",
    "universe": [universe],
    "address": [address]
}

CUE: {
    "cue": [cue number],
    "time": [duration],
    "timecode"?: "[timestring]",
    "follow"?: [follow time],
    "hang"?: [hang time],
    "actions": [
        { "ch": [channel], "[Property]": [value] ... },
        ...
    ]
}


*/


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
class CONCEPTTHEATERSIM_API UEOSCue : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double cueNumber;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double time;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double hang = -1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double follow = -1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int timecode = -1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<int, UEOSPropertySet*> actions;

    static inline FString JSON_CUE = TEXT("cue");
    static inline FString JSON_TIME = TEXT("time");
    
    static inline FString JSON_TIMECODE = TEXT("timecode");
    static inline FString JSON_FOLLOW = TEXT("follow");
    static inline FString JSON_HANG = TEXT("hang");

    static inline FString JSON_ACTIONS = TEXT("actions");

    static inline FString JSON_ACTIONS_CH = TEXT("ch");

    static UEOSCue *FromJSON(TSharedPtr<FJsonObject> cueJson);
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

    UPROPERTY()
    TArray<UEOSCue*> cues;

    UFUNCTION()
    bool patchLight(int ch, UEOSPatch* light);

    UFUNCTION(BlueprintCallable)
    TArray<int> getPatchedChannels();

    UFUNCTION(BlueprintCallable)
    double getParameter(int ch, FName parameter);

    bool loadFromJson(TSharedPtr<FJsonObject> cueJson);
    
    static inline FString JSON_VERSION = TEXT("version");

    static inline FString JSON_PATCH = TEXT("patch");
    static inline FString JSON_PATCH_CH = TEXT("ch");
    static inline FString JSON_PATCH_TYPE = TEXT("type");
    static inline FString JSON_PATCH_UNIVERSE = TEXT("universe");
    static inline FString JSON_PATCH_ADDRESS = TEXT("address");

    static inline FString JSON_CUES = TEXT("cues");
};