// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightingPosition.generated.h"

USTRUCT()
struct FLightingPositionConfig {
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    FVector offset;

    UPROPERTY(EditAnywhere)
    float minPosition = 120;

    UPROPERTY(EditAnywhere)
    float maxPosition = 120;

    UPROPERTY(EditAnywhere)
    float radius = 0;
    
    UPROPERTY(EditAnywhere)
    bool byAngle = false;
};

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
    virtual FVector getHangLocation(int index, float position);

    UFUNCTION(BlueprintCallable)
    virtual FQuat getHangRotation(int index, float position, float roll);

private:
    void drawDebugBox(FVector pos, FQuat rot);

protected:

    UPROPERTY()
    USceneComponent* root;

    UPROPERTY(EditAnywhere)
    TArray<FLightingPositionConfig> positions;

    UPROPERTY(EditAnywhere, AdvancedDisplay)
    float minPosition = 120;

    UPROPERTY(EditAnywhere, AdvancedDisplay)
    float maxPosition = 120;

    UPROPERTY(EditAnywhere, AdvancedDisplay)
    float radius = 0;
    
    UPROPERTY(EditAnywhere, AdvancedDisplay)
    bool byAngle = false;
    
    UPROPERTY(EditAnywhere, AdvancedDisplay)
    float debugBoxSize = 3;
};
