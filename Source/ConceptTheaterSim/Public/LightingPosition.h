// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightingPosition.generated.h"

UCLASS()
class CONCEPTTHEATERSIM_API ALightingPosition : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightingPosition();

protected:

    virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    virtual FVector getHangLocation(float position);

    UFUNCTION(BlueprintCallable)
    virtual FQuat getHangRotation(float position);

private:
    void drawDebugBox(FVector pos, FQuat rot);

protected:
    UPROPERTY()
    USceneComponent* root;

    UPROPERTY(EditAnywhere)
    float minPosition = 120;

    UPROPERTY(EditAnywhere)
    float maxPosition = 120;

    UPROPERTY(EditAnywhere)
    float radius = 0;
    
    UPROPERTY(EditAnywhere)
    bool byAngle = false;
    
    UPROPERTY(EditAnywhere, AdvancedDisplay)
    float debugBoxSize = 3;

};
