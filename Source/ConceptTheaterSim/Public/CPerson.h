// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Animation/CAnimationComponent.h"
#include "CPerson.generated.h"

UENUM(BlueprintType)
enum class EPersonBodyType : uint8
{
    MALE UMETA(DisplayName="Male"),
    FEMALE UMETA(DisplayName="Female")
};

UENUM(BlueprintType)
enum class EPersonLegType : uint8
{
    NONE UMETA(DisplayName="None"),
    MALE UMETA(DisplayName="Male"),
    FEMALE UMETA(DisplayName="Female"),
    FEMALE_HEEL_1 UMETA(DisplayName="Female Heel 1"),
    FEMALE_HEEL_2 UMETA(DisplayName="Female Heel 2")
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UPersonClothingAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FText displayName;

    UPROPERTY(EditAnywhere)
    TMap<EPersonBodyType, UStaticMesh*> meshes;

    UPROPERTY(EditAnywhere)
    TArray<UMaterialInterface *> defaultMaterials;

    UPROPERTY(EditAnywhere, AdvancedDisplay)
    TMap<EPersonLegType, UStaticMesh*> legMeshes;
    
    UPROPERTY(EditAnywhere, AdvancedDisplay)
    bool hasLegVariants = false;

    UPROPERTY(EditAnywhere, AdvancedDisplay)
    EPersonLegType requiredLeg = EPersonLegType::NONE;

    UMaterialInterface* getMaterial(int index, TArray<UMaterialInterface*> materialOverrides)
    {
        if(index >= defaultMaterials.Num())
        {
            if(defaultMaterials.Num())
                return nullptr;
            return defaultMaterials[0];
        }
        if(index < materialOverrides.Num() && materialOverrides[index] != nullptr)
            return materialOverrides[index];
        return defaultMaterials[index];
    }
};

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FPersonClothing
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    UPersonClothingAsset* asset;

    UPROPERTY(EditAnywhere)
    TArray<UMaterialInterface*> materialOverrides;

};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACPerson : public APawn
{
	GENERATED_BODY()

protected:

    UPROPERTY(EditDefaultsOnly, Category="Body")
    float defaultHeight = 66;

public:
	// Sets default values for this pawn's properties
	ACPerson();

    UPROPERTY(EditAnywhere, Category="Animation")
    FName name;

    UPROPERTY(EditAnywhere, Category="Animation")
    bool dummy = false;

    UPROPERTY(EditAnywhere, Category="Body")
    EPersonBodyType bodyType = EPersonBodyType::MALE;

    UPROPERTY(EditAnywhere, Category="Body")
    FLinearColor skinColor = FLinearColor(0.823, 0.597, 0.392, 1.0);

    UPROPERTY(EditInstanceOnly, Category="Body")
    float height = defaultHeight;

    UPROPERTY(EditAnywhere, Category="Head")
    FLinearColor eyeColor = FLinearColor(0.431, 0.816, 0.878, 1.0);

    UPROPERTY(EditAnywhere, Category="Head")
    FLinearColor hairColor = FLinearColor(0.072, 0.016, 0.0, 1.0);

    UPROPERTY(EditAnywhere, Category="Head")
    int hairType = -1;

    UPROPERTY(EditAnywhere, Category="Clothing")
    TArray<FPersonClothing> clothing;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category="Body", AdvancedDisplay)
    UMaterialInterface *skinMaterial;

    UPROPERTY(VisibleInstanceOnly, Category="Body", AdvancedDisplay)
    UMaterialInstanceDynamic *skinMaterialInstance;

    UPROPERTY(EditAnywhere, Category="Head", AdvancedDisplay)
    UMaterialInterface *eyeMaterial;
    
    UPROPERTY(VisibleInstanceOnly, Category="Head", AdvancedDisplay)
    UMaterialInstanceDynamic *eyeMaterialInstance;

    UPROPERTY(EditAnywhere, Category="Head", AdvancedDisplay)
    UMaterialInterface *hairMaterial;
    
    UPROPERTY(VisibleInstanceOnly, Category="Head", AdvancedDisplay)
    UMaterialInstanceDynamic *hairMaterialInstance;

    // Meshes
    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    TMap<EPersonBodyType, UStaticMesh*> bodyMeshes;

    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    TMap<EPersonLegType, UStaticMesh *> legMeshes;

    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    UStaticMesh *armMesh;
    
    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    UStaticMesh *headMesh;

    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    UStaticMesh *eyeMesh;

    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    UStaticMesh *earMesh;

    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    TArray<UStaticMesh*> hairMeshes;

    // components
    UPROPERTY(VisibleAnywhere, Category="Components")
    USceneComponent* rootComponent2;
    UPROPERTY(VisibleAnywhere, Category="Components")
    USceneComponent* bodyRoot;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* legMeshComponent;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* armMeshComponent;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* headMeshComponent;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* eyeMeshComponent;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* earMeshComponent;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* hairMeshComponent;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* chestMeshComponent;

    UPROPERTY(VisibleAnywhere, Category="Components")
    TArray<UStaticMeshComponent*> clothingMeshComponents;

    UPROPERTY(VisibleAnywhere, Category="Components")
    TMap<int, bool> clothingMeshComponentsVisible;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UCAnimationComponent* animationComponent;

    UFUNCTION()
    void setVisibility(bool newVisibility);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void OnConstruction(const FTransform &Transform) override;

private:

    void updateMeshes();
    void updateHeight();

    EPersonLegType getLegType()
    {
        EPersonLegType type = EPersonLegType::FEMALE;
        for (int i = 0; i < clothing.Num(); i++)
        {
            FPersonClothing c = clothing[i];
            UPersonClothingAsset *asset = c.asset;
            if(asset == nullptr)
                continue;
            if(asset->requiredLeg != EPersonLegType::NONE)
            {
                return asset->requiredLeg;
            }
        }
        switch (bodyType)
        {
        case EPersonBodyType::MALE:
            return EPersonLegType::MALE;
        case EPersonBodyType::FEMALE:
            return EPersonLegType::FEMALE;

        default:
            return EPersonLegType::MALE;
        }
        
    }
};
