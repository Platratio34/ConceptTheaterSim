// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Character.h"
#include "CCable.generated.h"

class CONCEPTTHEATERSIM_API UCCableConnector;

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACCable : public AActor
{
    GENERATED_BODY()

public:
    
	ACCable();
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void OnConstruction(const FTransform &Transform) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cable")
    float maxLength = 9e10;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cable")
    bool directional = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Inventory")
    bool inInventory = false;
    ACharacter *player = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Connectors")
    FVector startOffset;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Connectors")
    FVector endOffset;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Cable")
    TSoftObjectPtr<AActor> softStartActor;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Cable")
    FName startPortID;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Cable")
    TSoftObjectPtr<AActor> softEndActor;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Cable")
    FName endPortID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
    FLinearColor debugColor = FLinearColor(1.0, 1.0, 0.0, 1.0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
    bool drawDebug = true;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Debug")
    bool debugConnected = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Cable", AdvancedDisplay)
    UCCableConnector *startConnector = nullptr;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Cable", AdvancedDisplay)
    UCCableConnector *endConnector = nullptr;

    UFUNCTION(BlueprintCallable)
    void putInInventory(ACharacter* player);

    UFUNCTION(BlueprintCallable)
    virtual bool canConnectTo(UCCableConnector *connector, bool start);

    UFUNCTION(BlueprintCallable)
    bool tryConnect(UCCableConnector *connector);

    UFUNCTION(BlueprintCallable)
    virtual void onConnect(UCCableConnector *connector, bool start);

    UFUNCTION(BlueprintCallable)
    virtual void onDisconnect(bool start);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool isFullyConnected();

    UFUNCTION(BlueprintCallable)
    virtual void updateCable();

    UFUNCTION(BlueprintCallable)
    virtual bool isValidConnection(UCCableConnector *connector, bool start);

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    USceneComponent *root;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent *startMesh;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent *endMesh;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent *coil;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    USplineMeshComponent *spline;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
    UArrowComponent *arrow;

    UFUNCTION()
    UCCableConnector *getConnector(AActor *actor, bool start);

private:
    
};