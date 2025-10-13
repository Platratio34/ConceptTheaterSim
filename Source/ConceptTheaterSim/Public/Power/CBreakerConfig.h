// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CBreakerConfig.generated.h"

USTRUCT(BlueprintType)
struct FBreakerCircuitConfig
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool constantPower = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    double defaultState = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int address = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool switched = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int slot = 0;
};

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UCBreakerConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FName, FBreakerCircuitConfig> circuits;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int universe = 1;
};
