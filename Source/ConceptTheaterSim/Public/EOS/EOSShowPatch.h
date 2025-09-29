// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSShowPatch.generated.h"

USTRUCT()
struct CONCEPTTHEATERSIM_API FEOSShowPatchLight
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    FName type;

    UPROPERTY(EditAnywhere)
    int universe;
    
    UPROPERTY(EditAnywhere)
    int address;
};

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API UEOSShowPatch : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    FString name;

    UPROPERTY(EditAnywhere)
    TMap<int, FEOSShowPatchLight> lights;
};
