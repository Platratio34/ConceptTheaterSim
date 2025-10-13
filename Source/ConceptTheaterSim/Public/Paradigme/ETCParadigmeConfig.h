// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ETCParadigmeConfig.generated.h"

USTRUCT(BlueprintType)
struct FETCParadigmeRoomState
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FName, double> states;
};

USTRUCT(BlueprintType)
struct FETCParadigmeRoom
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FETCParadigmeRoomState> states;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int defaultState = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int showState = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSet<int> showInhibit;
};

USTRUCT(BlueprintType)
struct FETCParadigmeCircuit
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int universe = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int address = 1;
};

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UETCParadigmeConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString configName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FName, FETCParadigmeRoom> rooms;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FName, FETCParadigmeCircuit> circuits;
};
