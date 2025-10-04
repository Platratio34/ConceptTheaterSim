// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/CAnimationMaster.h"
#include "PropertyDriver.h"
#include "CAnimationComponent.generated.h"

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVisibilityChange, bool, newVisibility);

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCAnimationComponent : public UActorComponent
{
    
    GENERATED_BODY()

public:
    
	UCAnimationComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void onEvent(FAnimationFileTrackEvent event);

    UFUNCTION(BlueprintCallable)
    void registerWithMaster(FName ID);

    UPROPERTY(BlueprintAssignable)
    FOnVisibilityChange onVisibilityChange;

    UPROPERTY(EditAnywhere)
    bool dummy = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    UPROPERTY()
    UPropertyDriver *xPosDriver;
    UPROPERTY()
    UPropertyDriver *yPosDriver;
    UPROPERTY()
    UPropertyDriver *zPosDriver;
    
    UPROPERTY()
    UPropertyDriver *xRotDriver;
    UPROPERTY()
    UPropertyDriver *yRotDriver;
    UPROPERTY()
    UPropertyDriver *zRotDriver;

    UPROPERTY()
    USceneComponent *cParent = nullptr;

    void updatePosRot();

    UPROPERTY()
    ACAnimationMaster *master;
};