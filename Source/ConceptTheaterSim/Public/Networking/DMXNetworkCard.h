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
UCLASS()
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

    UPROPERTY(VisibleAnywhere)
    TArray<int> activeUniverses;

protected:
    UDMXCache *cache = nullptr;

    bool onPacketInternal(FNetworkPacket packet);

    TMap<int, bool> changedUniverses;
};
