// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
// #include "IDetailCustomizations.h"
// #include "DetailLayoutBuilder.h"
#include "ClothingMaterial.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UClothingMaterial : public UPrimaryDataAsset/*, public IDetailCustomization*/
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText displayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool isColorable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName colorParameter = FName("Color");
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UMaterialInterface* material;
};


USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FClothingMaterialInstance
{
    GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UClothingMaterial* base;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool isColored;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FColor color;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UMaterialInterface* material;
};
