// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "Networking/NetworkTypes.h"
#include "CNetwork.generated.h"

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UMulticastTargetSet : public UObject
{

	GENERATED_BODY()
	
public:
    UMulticastTargetSet();
    ~UMulticastTargetSet();

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    int address = 0xe0000000;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    TArray<UNetworkCard *> subscribers;

    UFUNCTION()
    void addSubscriber(UNetworkCard *subscriber);
    UFUNCTION()
    void removeSubscriber(UNetworkCard *subscriber);
    UFUNCTION()
    void sendPacket(FNetworkPacket packet);
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

    UFUNCTION()
    void connect(UNetworkCard *card);
    UFUNCTION()
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

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Network")
    TArray<UMulticastTargetSet *> multicastSets;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Network")
    TArray<UNetworkCard*> cards;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Network")
    TMap<int, UNetworkCard *> cardsByIP;
};