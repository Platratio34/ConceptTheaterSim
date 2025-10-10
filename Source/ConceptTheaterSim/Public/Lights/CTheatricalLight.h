// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include <cmath>
#include "GameFramework/Actor.h"
#include "Components/SpotLightComponent.h"
#include "Cables/CPowerCableConnector.h"
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
    static FShutterPosition CreateAB(double positionA, double positionB)
    {
        FShutterPosition p;
        p.position = (positionA + positionB) * 0.5;
        p.angleOffset = (positionA - positionB) * 0.5;
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

    UPROPERTY(EditAnywhere, Category="Light")
    bool dummy = false;

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
    
    UPROPERTY(EditAnywhere, Category="Zoom & Focus")
    bool simpleEdge = false;

    UPROPERTY(EditAnywhere, Category="Shutters")
    TArray<FShutterPosition> shutterPositions;
    
    UPROPERTY(EditAnywhere, Category="Zoom & Focus")
    double shutterFrame = 0;

    UPROPERTY(EditDefaultsOnly, Category="Shutters")
    TMap<int, USceneComponent*> shutterHandles;

    UPROPERTY(EditDefaultsOnly, Category="Shutters")
    double shutterHandleTravel = 1;

    UPROPERTY(EditInstanceOnly, Category="Default")
    TSoftObjectPtr<AActor> parentActor = nullptr;
    
    UPROPERTY(EditAnywhere, Category="Image")
    UTexture2D* gobo = nullptr;

    UPROPERTY(EditAnywhere, Category="Image")
    double goboRotation = 0;

    UPROPERTY(EditInstanceOnly, Category="Default")
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
    
    UFUNCTION(BlueprintCallable)
    void setGobo(UTexture2D* newGobo);

    UFUNCTION(BlueprintCallable)
    void setGoboRotation(double newGoboRotation);

    UFUNCTION(BlueprintCallable)
    void setShutter(int index, FShutterPosition position);

    UFUNCTION(BlueprintCallable)
    void setShutterFrame(double position);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Components")
    USceneComponent* root;
    
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Components")
    USceneComponent* yoke;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Components")
    USceneComponent* body;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Components")
    UCPowerCableConnector *powerInput;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Power")
    double requiredPower = 575;

    UFUNCTION(BlueprintNativeEvent)
    double getIntensityScale();

    UFUNCTION(BlueprintCallable)
    void setup(UStaticMeshComponent* lenseMesh_);

    UFUNCTION(BlueprintCallable)
    void addShutterHandle(int index, UStaticMeshComponent* handle);

    UFUNCTION()
    virtual double getPower();

    UFUNCTION()
    virtual void onLightUpdate() {};

    UPROPERTY()
    double actualIntensity = 0;

private:
    virtual double getIntensityScale_Implementation() { return 1; }

    UFUNCTION()
    void updateBeam();

    UFUNCTION()
    void updateShutters();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void OnConstruction(const FTransform &Transform) override;
};