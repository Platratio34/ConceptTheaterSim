// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonUtilities.h"
#include "EOS/EOSPropertyType.h"
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

class CONCEPTTHEATERSIM_API UEOSShowfile;

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
    TMap<FName, UEOSPropertyType *> properties;

    void addProperty(FName property)
    {
        properties.Add(property, UEOSPropertyType::Create(property));
    }
    void addProperty(FName property, double def)
    {
        properties.Add(property, UEOSPropertyType::Create(property, def));
    }
    void addProperty(UEOSPropertyType *propertyType)
    {
        properties.Add(propertyType->property, propertyType);
    }
    UEOSPropertyType* getProperty(FName property)
    {
        if(UEOSPropertyType** p = properties.Find(property))
        {
            return *p;
        }
        return nullptr;
    }
    bool hasProperty(FName property)
    {
        return properties.Contains(property);
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

    bool hasProperty(FName property)
    {
        if (devices.Num() == 0)
            return false;
        return devices[0]->hasProperty(property);
    }

    UEOSPropertyType* getProperty(FName property)
    {
        if (devices.Num() == 0)
            return nullptr;
        return devices[0]->getProperty(property);
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
        UE_LOG(LogTemp, Warning, TEXT("Tried to get property %s from set, but it was not present"), *property.ToString());
        return 0;
    }

    void set(FName property, double value)
    {
        if(!properties.Contains(property))
            return;
        properties.Add(property, value);
    }

    void apply(UEOSPropertySet* set)
    {
        for(TPair<FName, double> pair : set->properties)
        {
            if(!properties.Contains(pair.Key))
                continue;
            properties.Add(pair.Key, pair.Value);
        }
    }

    void add(FName property, double value)
    {
        properties.Add(property, value);
    }

    void addSet(UEOSPropertySet* set)
    {
        for(TPair<FName, double> pair : set->properties)
        {
            properties.Add(pair.Key, pair.Value);
        }
    }

    void remove(FName property)
    {
        properties.Remove(property);
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
    FString cueNumber;
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

    TSharedPtr<FJsonObject> toJson();
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UEOSChannelView : public UObject
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    float getProperty(FName property);
    
    UFUNCTION(BlueprintCallable)
    FName propertySource(FName property);

    static UEOSChannelView* create(UEOSShowfile *showfile, int channel)
    {
        UEOSChannelView *view = NewObject<UEOSChannelView>();
        view->showfile = showfile;
        view->channel = channel;
        return view;
    }

    void getKeys(TArray<FName> outKeys);

    UFUNCTION(BlueprintCallable)
    bool hasProperty(FName property);

protected:
    UEOSShowfile *showfile;
    int channel;
};

UCLASS()
class CONCEPTTHEATERSIM_API UEOSFade : public UObject
{
	GENERATED_BODY()
	
public:
    int channel;
    FName property;
    double target;
    double time;
    bool manual;
    bool sneak;

    bool update(UEOSShowfile *showfile, double deltaTime);
};

UCLASS()
class CONCEPTTHEATERSIM_API UEOSGroup : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY()
    FString name;

    UPROPERTY()
    TSet<int> channels;

    static inline FString JSON_CHS = TEXT("chs");
    static inline FString JSON_NAME = TEXT("name");

    static UEOSGroup *FromJSON(TSharedPtr<FJsonObject> groupJson);
    
    TSharedPtr<FJsonObject> toJson();
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
    TArray<UEOSCue*> cues;

    UPROPERTY()
    TMap<FString, UEOSGroup*> groups;

    UPROPERTY()
    int currentCue;

    UFUNCTION()
    bool patchLight(int ch, UEOSPatch* light);

    UFUNCTION(BlueprintCallable)
    TArray<int> getPatchedChannels();

    UFUNCTION(BlueprintCallable)
    double getParameter(int ch, FName parameter);

    UFUNCTION(BlueprintCallable)
    UEOSChannelView *getChannel(int ch);

    UFUNCTION()
    void addFade(int channel, FName property, double target, double time, bool manual, bool sneak);

    UFUNCTION()
    void clearFade(int channel, FName property, bool manual);

    UFUNCTION()
    void updateFades(double deltaTime);

    bool loadFromJson(TSharedPtr<FJsonObject> cueJson);

    TSharedPtr<FJsonObject> toJson();
    
    static inline FString JSON_VERSION = TEXT("version");

    static inline FString JSON_PATCH = TEXT("patch");
    static inline FString JSON_PATCH_CH = TEXT("ch");
    static inline FString JSON_PATCH_TYPE = TEXT("type");
    static inline FString JSON_PATCH_UNIVERSE = TEXT("universe");
    static inline FString JSON_PATCH_ADDRESS = TEXT("address");

    static inline FString JSON_CUES = TEXT("cues");
    static inline FString JSON_GROUPS = TEXT("groups");

    static inline FString JSON_CURRENT_CUE = TEXT("currentCue");

    void setManualProperty(int ch, FName property, float value);
    void setManualProperties(int ch, UEOSPropertySet* properties);
    void clearManualProperty(int ch, FName property, bool sneak);

    void recordOnly(int cueI, TArray<int> *sel);

    void setCueProperty(int ch, FName property, float value);
    void setCueProperties(int ch, UEOSPropertySet* properties);
    
    UEOSPropertySet *getManualChannel(int ch);
    UEOSPropertySet *getCueChannel(int ch);

protected:
    UPROPERTY()
    TMap<int, UEOSPropertySet*> channels;

    UPROPERTY()
    TMap<int, UEOSPropertySet*> manualChannels;
    
    UPROPERTY()
    TMap<int, UEOSChannelView*> channelViews;

    UPROPERTY()
    TArray<UEOSFade *> fades;
};