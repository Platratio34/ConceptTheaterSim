// Copyright Peter Crall 2025


#include "CustomClothingAsset.h"

UMaterialInterface* UCustomClothingAsset::getMaterial(int index, TArray<UMaterialInterface*> overrideMaterials)
{
    if(index >= defaultMaterials.Num())
    {
        return defaultMaterials[0];
    }
    else if(index >= overrideMaterials.Num())
    {
        return defaultMaterials[index];
    }
    return overrideMaterials[index];
}

int UCustomClothingAsset::getNumMaterials()
{
    return defaultMaterials.Num();
}

UStaticMesh* UCustomClothingAsset::getMesh(bool female)
{
    if(female)
    {
        if(maleOnly)
        {
            return nullptr;
        }
        else if(femaleMesh == nullptr)
        {
            return maleMesh;
        }
        return femaleMesh;
    }
    else
    {
        if(femaleOnly)
        {
            return nullptr;
        }
        else if(maleMesh == nullptr)
        {
            return femaleMesh;
        }
        return maleMesh;
    }
}

// void UCustomClothingAsset::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
// {
//     if(maleOnly)
//         DetailBuilder.HideCategory (FName(TEXT("Female")));
//     if(femaleOnly)
//         DetailBuilder.HideCategory (FName(TEXT("Male")));
// }