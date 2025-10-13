// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
class CONCEPTTHEATERSIM_API ACNetwork : public AActor
{
	GENERATED_BODY()
	
public:
    ACNetwork();
    ~ACNetwork();

    void OnConstruction(const FTransform &Transform) override;

    UPROPERTY(VisibleInstanceOnly, BlueprintAssignable, Category="Network")
    FOnNetworkPacket onPacketOut;

    UFUNCTION(BlueprintCallable)
    void setup(int subnet, int subnetMask, ACNetwork *upstream, bool sameNet);

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
    void setUpstream(ACNetwork *upstream, bool sameNet);
    UFUNCTION(BlueprintCallable)
    void clearUpstream();

    UFUNCTION(BlueprintCallable)
    void multicastSubscribe(int address, UNetworkCard *subscriber);
    UFUNCTION(BlueprintCallable)
    void multicastUnSubscribe(int address, UNetworkCard *subscriber);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Network")
    int subnet = 0x0A000000;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Network")
    int subnetMask = 0xFF000000;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Network")
    TMap<FString, int> assignedAddresses;
    UPROPERTY()
    int nextAddress = 0x00000001;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Upstream")
    ACNetwork *upstream = nullptr;
    
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Upstream")
    bool upstreamSameNet = false;

    UFUNCTION()
    bool isAddressLocal(int addr);
    UFUNCTION()
    void sendPacketInt(FNetworkPacket packet, bool fromUpstream);

    MulticastTargetSet **multicastSets = nullptr;
    UPROPERTY()
    int multicastSize = 1;
    UPROPERTY()
    int multicastPntr;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Network")
    TArray<UNetworkCard*> cards;
};