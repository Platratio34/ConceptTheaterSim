// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"
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
class CONCEPTTHEATERSIM_API AFlyrailRedirectBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlyrailRedirectBlock();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent *root;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void OnConstruction(const FTransform &Transform) override;

    UPROPERTY(BlueprintReadWrite)
    TArray<UStaticMeshComponent*> blockLines;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<USplineMeshComponent*> outLines;

public:
    UPROPERTY(BlueprintReadWrite)
    TArray<USceneComponent*> inLineEnds;

    void setup(TArray<UStaticMeshComponent*>& loftBlocks);
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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
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

    UPROPERTY(BlueprintReadOnly)
    TArray<UStaticMeshComponent *> weightBlocks;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<UStaticMeshComponent *> loftBlockComponents;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<UStaticMeshComponent *> loftBlockRopeComponents;

    UPROPERTY(BlueprintReadOnly)
    UChildActorComponent *redirectBlockComponent;
    
    UPROPERTY(BlueprintReadOnly)
    AFlyrailRedirectBlock *redirectBlockActor;

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
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Batton")
    AActor* battonActor;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Batton")
    float battonMinHeight;

    UPROPERTY(BlueprintReadWrite)
    TArray<UStaticMeshComponent*> headBlockLines;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<USplineMeshComponent*> loftBlockLines;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<USplineMeshComponent*> arborLines;

    UPROPERTY(BlueprintReadWrite)
    TArray<USceneComponent*> arborLineEnds;

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
    bool hasRedirectBlock;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FFlyrailLoftBlock redirectBlock;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AFlyrailRedirectBlock> redirectBlockClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<EFlyrailWeightType> weights;

    UFUNCTION(BlueprintCallable)
    void setPosition(float newPosition);

private:
    
    UPROPERTY(VisibleInstanceOnly)
    float lastPosition = -1;
};
