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

    FOnNetworkPacket onPacketOut;

    UFUNCTION(BlueprintCallable)
    void setup(int subnet, int subnetMask, UCNetwork *upstream);

    void sendPacket(FNetworkPacket packet);
    void onUpstreamPacket(FNetworkPacket packet);
    int requestIP(FString hwAddress);
    void releaseIP(FString hwAddress);

    void connect(UNetworkCard *card);
    void disconnect(UNetworkCard *card);

    int getSubnet();
    int getSubnetMask();

    UFUNCTION(BlueprintCallable)
    void setUpstream(UCNetwork *upstream);
    UFUNCTION(BlueprintCallable)
    void clearUpstream();

    void multicastSubscribe(int address, UNetworkCard *subscriber);
    void multicastUnSubscribe(int address, UNetworkCard *subscriber);

protected:
    int subnet = 0x0A000000;
    int subnetMask = 0xFF000000;

    TMap<FString, int> assignedAddresses;
    int nextAddress = 0x00000001;

    UCNetwork *upstream = nullptr;

    bool isAddressLocal(int addr);
    void sendPacketInt(FNetworkPacket packet, bool fromUpstream, UCNetwork* sourceNet);

    MulticastTargetSet **multicastSets = nullptr;
    int multicastSize = 1;
    int multicastPntr;

    TArray<UNetworkCard*> cards;
    TArray<UCNetwork*> childNetworks;
    UCNetwork *parentNetwork;
};