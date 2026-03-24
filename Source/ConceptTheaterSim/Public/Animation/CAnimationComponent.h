// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/CAnimationMaster.h"
#include "PropertyDriver.h"
#include "CAnimationComponent.generated.h"

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVisibilityChange, bool, newVisibility);

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClothingVisibilityChange, FName, key, bool, newHidden);

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCAnimationComponent : public UActorComponent
{
    
    GENERATED_BODY()

public:
    
	UCAnimationComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION()
    void onEvent(FAnimationFileTrackEvent event);

    UFUNCTION(BlueprintCallable)
    void registerWithMaster(FName ID);

    UPROPERTY(BlueprintAssignable)
    FOnVisibilityChange onVisibilityChange;

    UPROPERTY(BlueprintAssignable)
    FOnClothingVisibilityChange onClothingVisibilityChange;

    UPROPERTY(EditAnywhere)
    bool dummy = false;

    void setTrack(UAnimationTrack *track);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void addMesh(UStaticMeshComponent* mesh)
    {
        meshes.AddUnique(mesh);
    }

    UFUNCTION(BlueprintCallable)
    void removeMesh(UStaticMeshComponent* mesh)
    {
        meshes.Remove(mesh);
    }

    UPROPERTY()
    UAnimationTrack *track = nullptr;
    
    UPROPERTY()
    UAnimationTrackPropertyTrack *xPosTrack;
    UPROPERTY()
    UAnimationTrackPropertyTrack *yPosTrack;
    UPROPERTY()
    UAnimationTrackPropertyTrack *zPosTrack;
    
    UPROPERTY()
    UAnimationTrackPropertyTrack *xRotTrack;
    UPROPERTY()
    UAnimationTrackPropertyTrack *yRotTrack;
    UPROPERTY()
    UAnimationTrackPropertyTrack *zRotTrack;

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

    UFUNCTION()
    void updatePosRot(USceneComponent* newParent);

    UPROPERTY()
    ACAnimationMaster *master;

    UFUNCTION()
    void updateVisibility(bool newVisibility);

    UPROPERTY(EditInstanceOnly)
    TArray<UStaticMeshComponent *> meshes;
};