// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flyrail.generated.h"

USTRUCT()
struct FFlyrailLoftBlock
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    FVector position;
    
    UPROPERTY(EditAnywhere)
    FRotator rotation;

    UPROPERTY(EditAnywhere)
    UStaticMesh* loftBlockMesh = nullptr;
};

UENUM(BlueprintType)
enum class EFlyrailWeightType : uint8
{
    SMALL,
    LARGE
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API AFlyrail : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlyrail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    USceneComponent *root;
    
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent *headBlock;
    
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent *arbor;
    
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent *tensionBlock;
    
    
    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent *frontHandline;
    
    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent *arborHandlineLower;
    
    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent *arborHandlineUpper;
    
    UPROPERTY(VisibleDefaultsOnly)
    UStaticMeshComponent *upperBumper;

    UPROPERTY(EditDefaultsOnly)
    TArray<UStaticMeshComponent *> weightBlocks;
    
    UPROPERTY(EditDefaultsOnly)
    TArray<UStaticMeshComponent *> loftBlockComponents;

    virtual void OnConstruction(const FTransform &Transform) override;

    void setRopeLength(UStaticMeshComponent *component, float length);

    UFUNCTION(BlueprintCallable)
    void updatePosition();

    UPROPERTY(EditDefaultsOnly)
    TMap<EFlyrailWeightType, UStaticMesh*> weightMeshes;
    
    UPROPERTY(EditDefaultsOnly)
    UStaticMesh* loftBlockMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly)
    float maxPosition = 444;
    
    UPROPERTY(EditAnywhere)
    float position = 0;

    UPROPERTY(EditDefaultsOnly)
    float arborOffsetHeight = 36;

    UPROPERTY(EditDefaultsOnly)
    float headBlockHeight = 600;

    UPROPERTY(EditDefaultsOnly)
    float tensionBlockHeight = 6;

    UPROPERTY(EditAnywhere)
    TArray<FFlyrailLoftBlock> loftBlocks;

    UPROPERTY(EditAnywhere)
    TArray<EFlyrailWeightType> weights;
};
