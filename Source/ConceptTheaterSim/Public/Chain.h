// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chain.generated.h"

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API AChain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void OnConstruction(const FTransform &Transform) override;

    UPROPERTY(VisibleAnywhere)
    USceneComponent *root;

    UPROPERTY(EditDefaultsOnly)
    TArray<UStaticMeshComponent *> links;
    
    UPROPERTY(EditDefaultsOnly)
    UStaticMesh* linkMesh;

    UPROPERTY(EditDefaultsOnly)
    float linkLength = 1.25;

    UPROPERTY(EditDefaultsOnly)
    bool rotateLinks = true;

    UPROPERTY(EditAnywhere, Category="Default")
    float length = 144;

private:
    UStaticMeshComponent* createLinkMesh(int i);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void setLength(float newLength);
};
