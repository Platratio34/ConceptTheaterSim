// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Networking/NetworkTypes.h"
#include "NetworkCard.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UNetworkCard : public UObject
{
    GENERATED_BODY()

protected:
    UNetworkCard();
    ~UNetworkCard();

public:

    UFUNCTION(BlueprintCallable)
    void setup(int address, FString hwAddress);

    UFUNCTION(BlueprintCallable)
    void send(FNetworkPacket packet);

    void onPacket(FNetworkPacket packet);

    UFUNCTION(BlueprintCallable)
    void multicastSubscribe(int address);
    UFUNCTION(BlueprintCallable)
    void multicastUnSubscribe(int address);

    UFUNCTION(BlueprintCallable)
    void connect(UCNetwork *network);
    UFUNCTION(BlueprintCallable)
    void disconnect();

    UFUNCTION(BlueprintCallable)
    int getIP();

    FOnNetworkPacket onNetworkPacket;

protected:
    int address = 0;
    FString hwAddress;
    bool staticIP = false;

    int subnet = 0;
    int subnetMask = 0xffff0000;

    int *multicast = nullptr;
    int multicastPntr = 0;
    int multicastSize = 8;

    UCNetwork *network = nullptr;

    bool virtual onPacketInternal(FNetworkPacket packet) { return false; }
};