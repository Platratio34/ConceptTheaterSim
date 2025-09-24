// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Networking/NetworkCard.h"
#include "DMXCache.h"
#include "DMXNetworkCard.generated.h"

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FDMXNetworkPacket : public FNetworkPacket
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    FName sourceDevice;
    UPROPERTY(BlueprintReadWrite)
    int universe = 1;
    UPROPERTY(BlueprintReadWrite)
    int priority = 128;
    UPROPERTY(BlueprintReadWrite)
    TArray<int> dmxData;
};

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API UDMXNetworkCard : public UNetworkCard
{
	GENERATED_BODY()

protected:
    UDMXNetworkCard();
    ~UDMXNetworkCard();

    UFUNCTION(BlueprintCallable)
    TArray<int> getData(int universe);

    UFUNCTION(BlueprintCallable)
    void sendData(FName source, int priority, int universe, TArray<int> data);

protected:
    UDMXCache *cache = nullptr;

    bool onPacketInternal(FNetworkPacket packet);
};
