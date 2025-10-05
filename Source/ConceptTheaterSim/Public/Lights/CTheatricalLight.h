// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include <cmath>
#include "GameFramework/Actor.h"
#include "Components/SpotLightComponent.h"
#include "CTheatricalLight.generated.h"

const double DEG_TO_RAD = 3.14592653589793 / 180;

USTRUCT(BlueprintType)
struct FShutterPosition
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    double position;
    
    UPROPERTY(EditAnywhere)
    double angleOffset;

    static FShutterPosition Create(double position, double angle)
    {
        FShutterPosition p;
        double angleOff = 
        p.position = position;
        p.angleOffset = tan(angle * DEG_TO_RAD);
        return p;
    }
    static FShutterPosition CreateRot(double positionA, double positionB)
    {
        FShutterPosition p;
        p.position = (positionA + positionB) * 0.5;
        p.angleOffset = positionA - positionB;
        return p;
    }
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACTheatricalLight : public AActor
{
    GENERATED_BODY()

public:
    
	ACTheatricalLight();

    UPROPERTY(EditAnywhere, Category="Light")
    double intensity = 0;

    UPROPERTY(EditDefaultsOnly, Category="Light")
    double maxIntensity = 1000;

    UPROPERTY(EditAnywhere, Category="Light")
    FLinearColor color = FColor(255, 255, 255);

    UPROPERTY(EditDefaultsOnly, Category="Light")
    UCurveFloat* intensityCurve = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="Light")
    USpotLightComponent* light = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="Light")
    UMaterialInterface* lightFunction = nullptr;

    UPROPERTY(VisibleInstanceOnly, Category="Light")
    UMaterialInstanceDynamic *lightFunctionInstance = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="Lense")
    UMaterialInterface* lenseMaterial = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="Lense")
    int lenseMaterialIndex = 0;
    
    UPROPERTY(EditDefaultsOnly, Category="Lense")
    UStaticMeshComponent* lenseMesh = nullptr;

    UPROPERTY(VisibleInstanceOnly, Category="Lense")
    UMaterialInstanceDynamic *lenseMaterialInstance = nullptr;

    UPROPERTY(EditAnywhere, Category="Zoom & Focus")
    double pan = 0;

    UPROPERTY(EditAnywhere, Category="Zoom & Focus")
    double tilt = 0;
    
    UPROPERTY(EditAnywhere, Category="Zoom & Focus")
    double edge = 1;
    
    UPROPERTY(EditAnywhere, Category="Zoom & Focus")
    double zoom = 20;

    UPROPERTY(EditAnywhere, Category="Shutters")
    TArray<FShutterPosition> shutterPositions;

    UPROPERTY(EditDefaultsOnly, Category="Shutters")
    TMap<int, USceneComponent*> shutterHandles;

    UPROPERTY(EditDefaultsOnly, Category="Shutters")
    double shutterHandleTravel = 1;

    UPROPERTY(EditInstanceOnly)
    TSoftObjectPtr<AActor> parentActor = nullptr;

    UPROPERTY(EditInstanceOnly)
    bool focusMode = false;

    UFUNCTION(BlueprintCallable)
    void setIntensity(double newIntensity);

    UFUNCTION(BlueprintCallable)
    void setColor(FLinearColor newColor);

    UFUNCTION(BlueprintCallable)
    void setPan(double newPan);

    UFUNCTION(BlueprintCallable)
    void setTilt(double newTilt);
    
    UFUNCTION(BlueprintCallable)
    void setEdge(double newEdge);
    
    UFUNCTION(BlueprintCallable)
    void setZoom(double newZoom);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* root;
    
    UPROPERTY(VisibleAnywhere)
    USceneComponent* yoke;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* body;

    UFUNCTION(BlueprintNativeEvent)
    double getIntensityScale();

private:
    virtual double getIntensityScale_Implementation() { return 1; }

    void updateBeam();

    void updateShutters();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void OnConstruction(const FTransform &Transform) override;
};