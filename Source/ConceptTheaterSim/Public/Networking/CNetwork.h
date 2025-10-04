// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Networking/NetworkTypes.h"
#include "CNetwork.generated.h"

class CONCEPTTHEATERSIM_API MulticastTargetSet {

public:
    MulticastTargetSet(int address);
    ~MulticastTargetSet();

    int address = 0xe0000000;

    UNetworkCard **subscribers = nullptr;

    void addSubscriber(UNetworkCard *subscriber);
    void removeSubscriber(UNetworkCard *subscriber);
    void sendPacket(FNetworkPacket packet);

protected:
    int subSize = 4;
    int subPntr = 0;
};

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UCNetwork : public UObject
{
	GENERATED_BODY()
	
public:
    UCNetwork();
    ~UCNetwork();

    UPROPERTY()
    FOnNetworkPacket onPacketOut;

    UFUNCTION(BlueprintCallable)
    void setup(int subnet, int subnetMask, UCNetwork *upstream);

    UFUNCTION(BlueprintCallable)
    void sendPacket(FNetworkPacket packet);
    UFUNCTION()
    void onUpstreamPacket(FNetworkPacket packet);
    UFUNCTION(BlueprintCallable)
    int requestIP(FString hwAddress);
    UFUNCTION(BlueprintCallable)
    void releaseIP(FString hwAddress);

    UFUNCTION(BlueprintCallable)
    void connect(UNetworkCard *card);
    UFUNCTION(BlueprintCallable)
    void disconnect(UNetworkCard *card);

    UFUNCTION(BlueprintCallable)
    int getSubnet();
    UFUNCTION(BlueprintCallable)
    int getSubnetMask();

    UFUNCTION(BlueprintCallable)
    void setUpstream(UCNetwork *upstream);
    UFUNCTION(BlueprintCallable)
    void clearUpstream();

    UFUNCTION(BlueprintCallable)
    void multicastSubscribe(int address, UNetworkCard *subscriber);
    UFUNCTION(BlueprintCallable)
    void multicastUnSubscribe(int address, UNetworkCard *subscriber);

protected:
    UPROPERTY()
    int subnet = 0x0A000000;
    UPROPERTY()
    int subnetMask = 0xFF000000;

    UPROPERTY()
    TMap<FString, int> assignedAddresses;
    UPROPERTY()
    int nextAddress = 0x00000001;

    UCNetwork *upstream = nullptr;

    UFUNCTION()
    bool isAddressLocal(int addr);
    UFUNCTION()
    void sendPacketInt(FNetworkPacket packet, bool fromUpstream, UCNetwork* sourceNet);

    MulticastTargetSet **multicastSets = nullptr;
    UPROPERTY()
    int multicastSize = 1;
    UPROPERTY()
    int multicastPntr;

    UPROPERTY()
    TArray<UNetworkCard*> cards;
    UPROPERTY()
    TArray<UCNetwork*> childNetworks;
    UPROPERTY()
    UCNetwork *parentNetwork;
};