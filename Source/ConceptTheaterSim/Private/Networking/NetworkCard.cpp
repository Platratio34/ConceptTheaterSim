// Copyright Peter Crall 2025

#include "Networking/NetworkCard.h"

UNetworkCard::UNetworkCard()
{
    multicast = new int[multicastSize];
}

UNetworkCard::~UNetworkCard()
{
    delete[] multicast;
}

void UNetworkCard::setup(int ip, FString hw)
{
    if (ip > 0)
    {
        address = ip;
        staticIP = true;
    }
    hwAddress = hw;
}

void UNetworkCard::send(FNetworkPacket packet)
{
    packet.source = address;
    if(network != nullptr) {
        network->sendPacket(packet);
    }
    // send it here
}

void UNetworkCard::multicastSubscribe(int multicastAddress)
{
    for (int i = 0; i < multicastPntr; i++)
    {
        if (multicast[i] == multicastAddress) // already subscribed
            return;
    }
    if (multicastPntr == multicastSize)
    {
        int *nArr = new int[multicastSize + 8];
        for (int i = 0; i < multicastSize; i++)
        {
            nArr[i] = multicast[i];
        }
        multicastSize += 8;
    }
    multicast[multicastPntr++] = multicastAddress;
}
void UNetworkCard::multicastUnSubscribe(int multicastAddress)
{
    int after = -1;
    for (int i = 0; i < multicastPntr; i++)
    {
        if (multicast[i] == multicastAddress)
        {
            after = true;
        }
        else if (after)
        {
            multicast[i - 1] = multicast[i];
        }
    }
    if (after)
        multicastPntr--;
}

void UNetworkCard::onPacket(FNetworkPacket packet)
{
    bool valid = false;
    int localBroadcast = subnet | (~subnetMask);
    if (packet.dest == address || packet.dest == -1 || packet.dest == localBroadcast)
    {
        valid = true;
    }
    else
    {
        for (int i = 0; i < multicastPntr; i++)
        {
            if (packet.dest == multicast[i])
            {
                valid = true;
                break;
            }
        }
    }
    if(!valid)
        return;
    if(packet.type == FName(TEXT("Ping"))) {
        FNetworkPacket rsp;
        rsp.dest = packet.source;
        rsp.source = address;
        rsp.type = FName(TEXT("Pong"));
        send(rsp);
        return;
    }
    if(!onPacketInternal(packet)) // if the internal function did not consume it
        onNetworkPacket.Broadcast(packet);
}

void UNetworkCard::connect(UCNetwork *newNetwork) {
    if(network == newNetwork)
        return;
    if(network != nullptr) {
        disconnect();
    }
    network = newNetwork;
    subnet = network->getSubnet();
    subnetMask = network->getSubnetMask();
    if(!staticIP)
        address = network->requestIP(hwAddress);
    network->onPacketOut.AddDynamic(this, &UNetworkCard::onPacket);
}

void UNetworkCard::disconnect() {
    if(network == nullptr)
        return;
    if(!staticIP)
        network->releaseIP(hwAddress);
    network->onPacketOut.RemoveDynamic(this, &UNetworkCard::onPacket);
}

int UNetworkCard::getIP() {
    return address;
}