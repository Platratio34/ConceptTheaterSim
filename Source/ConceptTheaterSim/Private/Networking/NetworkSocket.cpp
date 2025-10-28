// Copyright Peter Crall 2025

#include "Networking/NetworkSocket.h"
#include "Kismet/GameplayStatics.h"

#include "Networking/NetworkCard.h"

UNetworkSocket::UNetworkSocket()
{
    
}

UNetworkSocket::~UNetworkSocket()
{
    
}

void UNetworkSocket::setup(UNetworkCard *card, int otherIP, FName msgType, int srcPort, int destPort)
{
    networkCard = card;
    tgtIP = otherIP;
    type = msgType;
    localPort = srcPort;
    remotePort = destPort;

    if(networkCard != nullptr && tgtIP > 0)
    {
        networkCard->send(USocketPacket::Packet(this));
    }
}

bool UNetworkSocket::isConnected()
{
    return connected;
}

void UNetworkSocket::close()
{
    networkCard->send(USocketPacket::ClosePacket(this));
    closing = true;
    connected = false;
}

bool UNetworkSocket::send(UObject *data)
{
    if(!connected)
        return false;
    networkCard->send(USocketPacket::DataPacket(this, data));
    return true;
}

void UNetworkSocket::onPacket(UNetworkPacket *packet)
{
    lastPacket = UGameplayStatics::GetRealTimeSeconds(GetWorld());
    USocketPacket *socketPacket = Cast<USocketPacket>(packet);
    if(!closing) // don't re-mark connected on closing
        connected = true;
    if(tgtIP == 0 && !closing)
    {
        tgtIP = packet->source;
        networkCard->send(USocketPacket::Packet(this));
    }
    if(socketPacket->keepAlive != 0)
    {
        if(socketPacket->keepAlive == 1)
            networkCard->send(USocketPacket::KeepAlivePacket(this, true));
    }
    else if(socketPacket->close)
    {
        if(closing) // we initiated the close
        {
            networkCard->closeSocket(localPort);
            closing = false;
        }
        else // other end initiated the closure
        {
            // so make sure they know we know
            networkCard->send(USocketPacket::ClosePacket(this));
            networkCard->closeSocket(localPort);
            connected = false;
        }
    }
    else if(socketPacket->hasData)
    {
        // on data packet;
    }
}

void UNetworkSocket::update()
{
    float time = UGameplayStatics::GetRealTimeSeconds(GetWorld());
    time -= lastPacket;
    if(time > 10)
    {
        close();
    }
    else if(time > 5 && keepAlive)
    {
        networkCard->send(USocketPacket::KeepAlivePacket(this, false));
    }
}

// 
// USocketPacket
//
USocketPacket* USocketPacket::Packet(UNetworkSocket *socket)
{
    USocketPacket *packet = NewObject<USocketPacket>();
    packet->dest = socket->getIP();
    packet->type = socket->getType();
    packet->destPort = socket->getLocalPort();
    packet->sourcePort = socket->getRemotePort();
    return packet;
}

USocketPacket* USocketPacket::DataPacket(UNetworkSocket *socket, UObject *data)
{
    USocketPacket *packet = USocketPacket::Packet(socket);
    packet->data = data;
    packet->hasData = true;
    return packet;
}

USocketPacket* USocketPacket::KeepAlivePacket(UNetworkSocket *socket, bool rsp)
{
    USocketPacket *packet = USocketPacket::Packet(socket);
    packet->keepAlive = rsp ? 2 : 1;
    return packet;
}

USocketPacket* USocketPacket::ClosePacket(UNetworkSocket *socket)
{
    USocketPacket *packet = USocketPacket::Packet(socket);
    packet->close = true;
    return packet;
}