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

UENUM(BlueprintType)
enum class EClothingType : uint8
{
    OTHER UMETA(DisplayName="Other"),
    SHOE UMETA(DisplayName="Shoe"),
    BOTTOM UMETA(DisplayName="Bottom"),
    TOP UMETA(DisplayName="Top"),
    HEAD UMETA(DisplayName="Head"),
    DRESS UMETA(DisplayName="Dress"),
    UNDERWEAR UMETA(DisplayName="Underwear (Lower)"),
    UNDERWEAR_UPPER UMETA(DisplayName="Underwear (Upper)")
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UPersonClothingAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FText displayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<EPersonBodyType, UStaticMesh*> meshes;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<UMaterialInterface *> defaultMaterials;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay)
    TMap<EPersonLegType, UStaticMesh*> legMeshes;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay)
    bool hasLegVariants = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay)
    EPersonLegType requiredLeg = EPersonLegType::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Metadata")
    EClothingType clothingType;

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

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName name;
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACPerson : public APawn
{
	GENERATED_BODY()

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Body")
    float defaultHeight = 66;

public:
	// Sets default values for this pawn's properties
	ACPerson();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
    FName name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
    bool dummy = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Body")
    EPersonBodyType bodyType = EPersonBodyType::MALE;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Body")
    FLinearColor skinColor = FLinearColor(0.823, 0.597, 0.392, 1.0);

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Body")
    float height = defaultHeight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Head")
    FLinearColor eyeColor = FLinearColor(0.431, 0.816, 0.878, 1.0);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Head")
    FLinearColor hairColor = FLinearColor(0.072, 0.016, 0.0, 1.0);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Head")
    int hairType = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Clothing")
    TArray<FPersonClothing> clothing;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Clothing")
    TMap<FName, bool> hiddenClothing;

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
    UPROPERTY(BlueprintReadOnly, Category="Components")
    USceneComponent* rootComponent2;
    UPROPERTY(BlueprintReadOnly, Category="Components")
    USceneComponent* bodyRoot;

    UPROPERTY(BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* legMeshComponent;

    UPROPERTY(BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* armMeshComponent;

    UPROPERTY(BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* headMeshComponent;

    UPROPERTY(BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* eyeMeshComponent;

    UPROPERTY(BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* earMeshComponent;

    UPROPERTY(BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* hairMeshComponent;

    UPROPERTY(BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* chestMeshComponent;

    UPROPERTY(BlueprintReadOnly, Category="Components")
    TArray<UStaticMeshComponent*> clothingMeshComponents;

    UPROPERTY(BlueprintReadOnly, Category="Components")
    TMap<int, bool> clothingMeshComponentsVisible;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UCAnimationComponent* animationComponent;

    UFUNCTION(BlueprintCallable)
    void setVisibility(bool newVisibility);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void OnConstruction(const FTransform &Transform) override;

    UFUNCTION(BlueprintCallable)
    void hideClothing(FName item, bool hidden);

private:

    UFUNCTION()
    void updateMeshes();
    UFUNCTION()
    void updateHeight();

    UFUNCTION()
    EPersonLegType getLegType();
};
