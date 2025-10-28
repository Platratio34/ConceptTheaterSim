// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Networking/NetworkTypes.h"
#include "NetworkSocket.generated.h"

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSocketPacket, UObject*, data);

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UNetworkSocket : public UObject
{
    GENERATED_BODY()

protected:
    UNetworkSocket();
    ~UNetworkSocket();

public:
    UFUNCTION()
    void setup(UNetworkCard *card, int otherIP, FName msgType, int srcPort, int destPort);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool isConnected();

    UFUNCTION(BlueprintCallable)
    bool send(UObject *data);

    UFUNCTION(BlueprintCallable)
    void close();

    UFUNCTION(BlueprintCallable)
    void onPacket(UNetworkPacket *packet);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int getLocalPort()
    {
        return localPort;
    }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int getRemotePort()
    {
        return remotePort;
    }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    FName getType()
    {
        return type;
    }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int getIP()
    {
        return tgtIP;
    }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool isClosing()
    {
        return closing;
    }
    
    UFUNCTION()
    void update();

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    FOnSocketPacket onDataPacket;

protected:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    bool connected = false;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    bool keepAlive = true;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    bool closing = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    UNetworkCard *networkCard;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    int tgtIP;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    FName type;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    int localPort;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    int remotePort;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    float lastPacket = 0;
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API USocketPacket : public UNetworkPacket
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    int keepAlive = 0;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    bool hasData = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    bool close = false;

    static USocketPacket *Packet(UNetworkSocket *socket);

    static USocketPacket *DataPacket(UNetworkSocket *socket, UObject *data);

    static USocketPacket *KeepAlivePacket(UNetworkSocket *socket, bool rsp);

    static USocketPacket *ClosePacket(UNetworkSocket *socket);
};