// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Networking/NetworkTypes.h"
#include "NetworkCard.generated.h"

/**
 *
 */
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UNetworkCard : public UActorComponent
{
    GENERATED_BODY()

protected:
    UNetworkCard();
    ~UNetworkCard();

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void setup(int address, FString hwAddress, int subnet_, int subnetMask_);

    UFUNCTION(BlueprintCallable)
    void send(UNetworkPacket *packet);

    UFUNCTION(BlueprintCallable)
    void sendBroadcast(UNetworkPacket *packet);

    UFUNCTION()
    void onPacket(UNetworkPacket *packet);

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

    UFUNCTION(BlueprintCallable)
    FString getHWAddress();

    UFUNCTION(BlueprintCallable)
    UNetworkSocket *createSocket(int ip, FName type, int port);

    UFUNCTION(BlueprintCallable)
    void closeSocket(int port);

    UFUNCTION(BlueprintCallable)
    UNetworkSocket *openSocket(FName type, int port);

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

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Network")
    TArray<int> multicast;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Network")
    ACNetwork *network = nullptr;

    UFUNCTION()
    virtual bool onPacketInternal(UNetworkPacket *packet) { return false; };

    UFUNCTION()
    void connectInternal();

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Debug")
    int packetsIn;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Debug")
    int packetsOut;

    int nextPortOut = 0x1001;
    TMap<int, UNetworkSocket*> sockets;
};