// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Networking/NetworkCard.h"
#include "Networking/NetworkPacketTypes.h"
#include "DMXCache.h"
#include "DMXNetworkCard.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UDMXNetworkCard : public UNetworkCard
{
	GENERATED_BODY()

public:
    UDMXNetworkCard();
    ~UDMXNetworkCard();

    UFUNCTION(BlueprintCallable)
    TArray<int> getData(int universe);

    UFUNCTION(BlueprintCallable)
    void sendData(FName source, int priority, int universe, TArray<int> data);
    
    UFUNCTION(BlueprintCallable)
    bool hasChanged(int universe);

    UFUNCTION(BlueprintCallable)
    void clearChanged(int universe);

    UPROPERTY(VisibleInstanceOnly, Category="DMX")
    TArray<int> activeUniverses;

    UFUNCTION(BlueprintCallable)
    void addUniverse(int universe);
    
    UFUNCTION(BlueprintCallable)
    void removeUniverse(int universe);

protected:
    UPROPERTY(VisibleInstanceOnly, Category="DMX")
    UDMXCache *cache = nullptr;

    virtual bool onPacketInternal(FNetworkPacket packet) override;

    UPROPERTY(VisibleInstanceOnly, Category="DMX")
    TMap<int, bool> changedUniverses;
};
