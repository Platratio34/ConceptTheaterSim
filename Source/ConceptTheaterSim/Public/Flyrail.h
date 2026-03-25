// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flyrail.generated.h"

USTRUCT(BlueprintType)
struct FFlyrailLoftBlock
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector position;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRotator rotation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* loftBlockMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* loftBlockRopeMesh = nullptr;
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

    UPROPERTY()
    TArray<UStaticMeshComponent *> weightBlocks;
    
    UPROPERTY()
    TArray<UStaticMeshComponent *> loftBlockComponents;
    
    UPROPERTY()
    TArray<UStaticMeshComponent *> loftBlockRopeComponents;

    virtual void OnConstruction(const FTransform &Transform) override;

    void setRopeLength(UStaticMeshComponent *component, float length);

    UFUNCTION(BlueprintCallable)
    void updatePosition();

    UPROPERTY(EditDefaultsOnly)
    TMap<EFlyrailWeightType, UStaticMesh*> weightMeshes;
    
    UPROPERTY(EditDefaultsOnly)
    UStaticMesh* loftBlockMesh;
    
    UPROPERTY(EditDefaultsOnly)
    UStaticMesh* loftBlockRopeMesh;
    
    UPROPERTY(EditAnywhere, Category = "Batton")
    AActor* battonActor;
    
    UPROPERTY(EditAnywhere, Category = "Batton")
    float battonMinHeight;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float maxPosition = 444;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float position = 0;

    UPROPERTY(EditDefaultsOnly)
    float arborOffsetHeight = 36;

    UPROPERTY(EditDefaultsOnly)
    float headBlockHeight = 600;

    UPROPERTY(EditDefaultsOnly)
    float tensionBlockHeight = 6;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FFlyrailLoftBlock> loftBlocks;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<EFlyrailWeightType> weights;

    UFUNCTION(BlueprintCallable)
    void setPosition(float newPosition);

private:
    
    UPROPERTY(VisibleInstanceOnly)
    float lastPosition = -1;
};
