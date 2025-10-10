// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Networking/NetworkTypes.h"
#include "NetworkCard.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UNetworkCard : public UActorComponent
{
    GENERATED_BODY()

protected:
    UNetworkCard();
    ~UNetworkCard();

public:

    UFUNCTION(BlueprintCallable)
    void setup(int address, FString hwAddress, int subnet_, int subnetMask_);

    UFUNCTION(BlueprintCallable)
    void send(FNetworkPacket packet);

    UFUNCTION()
    void onPacket(FNetworkPacket packet);

    UFUNCTION(BlueprintCallable)
    void multicastSubscribe(int address);
    UFUNCTION(BlueprintCallable)
    void multicastUnSubscribe(int address);

    UFUNCTION(BlueprintCallable)
    void connect(ACNetwork *network);
    UFUNCTION(BlueprintCallable)
    void disconnect();

    UFUNCTION(BlueprintCallable)
    int getIP();

    UPROPERTY(VisibleInstanceOnly, BlueprintAssignable, Category="Network")
    FOnNetworkPacket onNetworkPacket;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Network")
    int address = 0;
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Network")
    FString hwAddress;
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Network")
    bool staticIP = false;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Network")
    int subnet = 0;
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Network")
    int subnetMask = 0xffff0000;

    int *multicast = nullptr;
    int multicastPntr = 0;
    int multicastSize = 8;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Network")
    ACNetwork *network = nullptr;

    UFUNCTION()
    virtual bool onPacketInternal(FNetworkPacket packet) { return false; };

    UFUNCTION()
    void connectInternal();
};