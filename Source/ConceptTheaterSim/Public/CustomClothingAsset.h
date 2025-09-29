// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
// #include "IDetailCustomizations.h"
// #include "DetailLayoutBuilder.h"
#include "CustomClothingAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UCustomClothingAsset : public UPrimaryDataAsset/*, public IDetailCustomization*/
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere)
    FText name;

    UPROPERTY(EditAnywhere, Category="Male")
    UStaticMesh* maleMesh = nullptr;

    UPROPERTY(EditAnywhere, Category="Male")
    bool maleOnly;

    UPROPERTY(EditAnywhere, Category="Female")
    UStaticMesh* femaleMesh = nullptr;

    UPROPERTY(EditAnywhere, Category="Female")
    bool femaleOnly;

    UPROPERTY(EditAnywhere)
    TArray<UMaterialInterface*> defaultMaterials;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UMaterialInterface* getMaterial(int index, TArray<UMaterialInterface*> overrideMaterials);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int getNumMaterials();

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UStaticMesh *getMesh(bool female);

    /*void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;*/
};
