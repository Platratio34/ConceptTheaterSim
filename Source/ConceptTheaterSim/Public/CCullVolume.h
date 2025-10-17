// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "CCullVolume.generated.h"

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCullVolumeChange, FName, volume, bool, active);

class CONCEPTTHEATERSIM_API ACCullVolume;

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FCVisiblityBlockerSet
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<AActor *> blockers;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool andBlockers;
};

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FCCullVolumeConnection
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ACCullVolume *other;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCVisiblityBlockerSet> blockers;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool disabled;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool oneDirection;
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACCullVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCullVolume();
    ~ACCullVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void OnConstruction(const FTransform &Transform) override;
    
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Cull Volume")
    FName name;
    
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Cull Volume")
    TArray<ACCullVolume*> subVolumes;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Cull Volume")
    TArray<FCCullVolumeConnection> connections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cull Volume")
    bool requireSky = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cull Volume")
    FVector boxSize = FVector(120, 120, 120);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cull Volume")
    bool outOnly = false;

    UFUNCTION(BlueprintCallable)
    bool isActive()
    {
        return active;
    }

    UPROPERTY(VisibleInstanceOnly, BlueprintAssignable, Category="Cull Volume")
    FOnCullVolumeChange onActiveChange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cull Volume Mesh")
    UStaticMesh *volumeMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cull Volume Mesh")
    FVector meshOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cull Volume Mesh")
    bool meshVisibleInEditor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cull Volume Mesh")
    FVector checkOffset = FVector(0, 0, 2000);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    USceneComponent* rootObj;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UBoxComponent* collider;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* meshCollider;

    UFUNCTION()
    void onBeginBoxOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    UFUNCTION()
    void onBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    bool updateHasPawn();

    UFUNCTION()
    bool updateActive();

    UFUNCTION()
    void updateConnection(FCCullVolumeConnection connection);

    UFUNCTION(BlueprintCallable)
    void setConnectionIn(FName other, bool connectionActive);

    UFUNCTION()
    void updateArrows(bool notify);

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Cull Volume")
    bool hasPawn = false;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Cull Volume", AdvancedDisplay)
    bool pawnInBox = false;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Cull Volume")
    bool connectionInActive = false;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Cull Volume", AdvancedDisplay)
    bool active = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Cull Volume", AdvancedDisplay)
    TSet<FName> activeConnectionsIn;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Cull Volume", AdvancedDisplay)
    TSet<FName> activeConnectionsOut;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Debug")
    TMap<FName, UArrowComponent*> arrows;

private:
    UPROPERTY()
    bool activeConnectionsInDirty = false;

};