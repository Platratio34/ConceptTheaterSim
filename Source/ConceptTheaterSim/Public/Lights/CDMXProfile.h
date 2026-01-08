// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CDMXProfile.generated.h"

UENUM(BlueprintType)
enum class ECDMXProfileParameterType : uint8
{
    STRAIGHT,
    FINE,
    COARSE,
    INDEXED,
    CONTROL,
    BITMASK
};

USTRUCT(BlueprintType)
struct FCDMXProfileParameterRange
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName id;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int min = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int max = 0;
};

USTRUCT(BlueprintType)
struct FCDMXProfileParameter
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName id;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ECDMXProfileParameterType type = ECDMXProfileParameterType::STRAIGHT;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCDMXProfileParameterRange> ranges;
};

UCLASS(BlueprintType)
class UCDMXProfile : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName name;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCDMXProfileParameter> parameters;

    void updateParameters(TMap<FName, int> *outParameters, TArray<int> dmx, int start)
    {
        TMap<FName, int> p;
        for (int address = 0; address < parameters.Num(); address++)
        {
            FCDMXProfileParameter parameter = parameters[address];
            FName id = parameter.id;
            if(!p.Find(id))
            {
                p.Add(id, 0);
            }

            if(parameter.type == ECDMXProfileParameterType::COARSE)
            {
                p[id] += dmx[start + address] * 256;
            }
            else if(parameter.type == ECDMXProfileParameterType::FINE)
            {
                p[id] += dmx[start + address];
            }
            else
            {
                p[id] = dmx[start + address];
            }
        }
        TArray<FName> keys;
        p.GetKeys(keys);
        for(FName key : keys)
        {
            outParameters->Add(key, p[key]);
        }
    }

    ECDMXProfileParameterType getType(FName id)
    {
        if(ECDMXProfileParameterType *type = typeCache.Find(id))
        {
            return *type;
        }
        ECDMXProfileParameterType o;
        for(FCDMXProfileParameter parameter: parameters)
        {
            if(parameter.id != id)
                continue;
            if(parameter.type == ECDMXProfileParameterType::FINE)
            {
                o = ECDMXProfileParameterType::COARSE;
            }
            else
            {
                o = parameter.type;
            }
            break;
        }
        typeCache.Add(id, o);
        return o;
    }

    FName getRange(FName id, int value)
    {
        for(FCDMXProfileParameter parameter: parameters)
        {
            if(parameter.id != id)
                continue;
            if(parameter.type != ECDMXProfileParameterType::INDEXED)
                return FName("");
            
            for(FCDMXProfileParameterRange range : parameter.ranges)
            {
                if(range.min <= value && range.max >= value)
                    return range.id;
            }
            return FName("");
        }
        return FName("");
    }

    bool hasParameter(FName id)
    {
        for(FCDMXProfileParameter parameter: parameters)
        {
            if(parameter.id == id)
                return true;
        }
        return false;
    }

private:
    TMap<FName, ECDMXProfileParameterType> typeCache;
};