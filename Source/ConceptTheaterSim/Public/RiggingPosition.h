// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RiggingPosition.generated.h"

UCLASS()
class CONCEPTTHEATERSIM_API ARiggingPosition : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARiggingPosition();

protected:

    virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    virtual FVector getMountPosition(FVector targetPosition);

private:
    void drawDebugBox(FVector pos, FQuat rot);

protected:
    UPROPERTY()
    USceneComponent* root;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* meshComponent;

    UPROPERTY(EditAnywhere)
    bool xAligned = false;

    UPROPERTY(EditAnywhere)
    UStaticMesh *mesh;
    
    UPROPERTY(EditAnywhere)
    float meshLength;

    UPROPERTY(EditAnywhere, AdvancedDisplay)
    float debugBoxSize = 3;

};
