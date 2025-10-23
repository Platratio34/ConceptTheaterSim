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

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor="true"))
enum class EChestCoverage : uint8
{
    NONE UMETA(Hidden),
    LOWER = 1 UMETA(DisplayName="Lower Body"),
    MIDDEL = 2 UMETA(DisplayName="Mid Body"),
    SHOULDERS = 4 UMETA(DisplayName="Shoulders")
};
/*
0000 0 - None
0001 1 - only lower missing
0010 2 - only mid missing
0011 3 - missing lower & mid
0100 4 - only shoulders missing
0101 5 - shoulders and lower missing, mid present
0110 6 - shoulders & mid missing, lower present
0111 7 - all missing
*/

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor="true"))
enum class ELegCoverage : uint8
{
    NONE = 0 UMETA(Hidden),
    UPPER = 1 UMETA(DisplayName="Upper Legs"),
    LOWER = 2 UMETA(DisplayName="Lower Legs"),
    FEET = 4 UMETA(DisplayName="Feet")
};
/*
0000 0 - None
0001 1 - upper legs missing
0010 2 - lower legs missing
0011 3 - upper & lower legs missing (ankle & foot)
0100 4 - foot missing
0101 5 - foot & upper leg missing
0110 6 - foot & lower leg missing (upper only)
0111 7 - all missing (only ankle)
*/

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor="true"))
enum class EArmCoverage : uint8
{
    NONE UMETA(Hidden),
    UPPER = 1 UMETA(DisplayName="Upper Arms"),
    LOWER = 2 UMETA(DisplayName="Lower Arms"),
    HANDS = 4 UMETA(DisplayName="Hands")
};
/*
0000 0 - None
0001 1 - upper arms missing
0010 2 - only lower arms missing
0011 3 - lower & upper arms missing
0100 4 - hands missing
0101 5 - hands & upper arms missing
0110 6 - hands & lower arms missing (upper only)
0111 7 - all missing (only wrist)
*/

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UPersonClothingAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Default")
    FText displayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Default")
    TMap<EPersonBodyType, UStaticMesh*> meshes;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Default")
    TArray<UMaterialInterface *> defaultMaterials;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Legs")
    TMap<EPersonLegType, UStaticMesh*> legMeshes;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Legs")
    bool hasLegVariants = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Legs")
    EPersonLegType requiredLeg = EPersonLegType::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Metadata")
    EClothingType clothingType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Optimization")
    TSet<UPersonClothingAsset*> hides;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Optimization", Meta = (Bitmask, BitmaskEnum = "/Script/ConceptTheaterSim.EChestCoverage"))
    uint8 chestCoverage;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Optimization", Meta = (Bitmask, BitmaskEnum = "/Script/ConceptTheaterSim.EArmCoverage"))
    uint8 armCoverage;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Optimization", Meta = (Bitmask, BitmaskEnum = "/Script/ConceptTheaterSim.ELegCoverage"))
    uint8 legCoverage;

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

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FPersonMeshSet 
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TMap<int, UStaticMesh *> meshes;
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
    TMap<EPersonBodyType, FPersonMeshSet> bodyMeshes;

    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    TMap<EPersonLegType, FPersonMeshSet> legMeshes;

    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    FPersonMeshSet armMeshes;
    
    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    UStaticMesh *headMesh;

    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    UStaticMesh *eyeMesh;

    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    UStaticMesh *earMesh;

    UPROPERTY(EditDefaultsOnly, Category="Meshes")
    TArray<UStaticMesh*> hairMeshes;

    UPROPERTY(VisibleInstanceOnly, Category="Debug")
    uint8 chestCoverage = 0;
    UPROPERTY(VisibleInstanceOnly, Category="Debug")
    uint8 armCoverage = 0;
    UPROPERTY(VisibleInstanceOnly, Category="Debug")
    uint8 legCoverage = 0;
    UPROPERTY(VisibleInstanceOnly, Category="Debug")
    EPersonLegType legType = EPersonLegType::MALE;

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

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Clothing")
    TSet<UPersonClothingAsset*> clothingHides;

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
