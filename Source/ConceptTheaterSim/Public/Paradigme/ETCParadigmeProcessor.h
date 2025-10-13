// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking/DMXNetworkCard.h"
#include "Paradigme/ETCParadigmeConfig.h"
#include "ETCParadigmeProcessor.generated.h"


UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoomStateUpdate, FName, room, int, state);

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API AETCParadigmeProcessor : public AActor
{
	GENERATED_BODY()
	
public:
    AETCParadigmeProcessor();
    ~AETCParadigmeProcessor();

    void OnConstruction(const FTransform &Transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    
    UFUNCTION(BlueprintCallable)
    bool requestState(FName room, int state);

    void activateState(FName room, int state);

    UFUNCTION(BlueprintCallable)
    int getState(FName room);

    UFUNCTION(BlueprintCallable)
    bool validRoom(FName room);

    UFUNCTION(BlueprintCallable)
    bool toggleShowMode();
    
    UFUNCTION(BlueprintCallable)
    void setShowMode(bool active);
    
    UFUNCTION(BlueprintCallable)
    bool getShowMode();

    UPROPERTY(VisibleInstanceOnly, BlueprintAssignable, Category="Paradigme")
    FOnRoomStateUpdate onRoomStateUpdate;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Paradigme")
    FName sourceName = FName(TEXT("Paradigme Processor"));

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Paradigme")
    int dmxPriority = 127;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Network")
    UDMXNetworkCard* networkCard;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Paradigme")
    bool showMode = false;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Paradigme")
    TMap<FName, int> activeStates;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Paradigme")
    TMap<FName, double> outState;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Paradigme")
    TMap<FName, double> targetOutState;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Paradigme")
    UETCParadigmeConfig *config = nullptr;

    UFUNCTION()
    void sendDMX();

private:

    UFUNCTION()
    void activeStateInternal(FName room, FETCParadigmeRoom roomConfig, int state);

    UFUNCTION()
    bool processStateUpdate(FName circuit, double deltaTime, double target);
};