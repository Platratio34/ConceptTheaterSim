// Copyright Peter Crall 2025

#include "Networking/NetworkCard.h"

#include "Networking/CNetwork.h"

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
        multicastSize += 8;
        int *nArr = new int[multicastSize];
        for (int i = 0; i < multicastPntr; i++)
        {
            nArr[i] = multicast[i];
        }
        delete[] multicast;
        multicast = nArr;
    }
    multicast[multicastPntr++] = multicastAddress;
    if(network != nullptr) {
        network->multicastSubscribe(multicastAddress, this);
    }
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
    if(network != nullptr) {
        network->multicastUnSubscribe(multicastAddress, this);
    }
}

void UNetworkCard::onPacket(FNetworkPacket packet)
{
    if(packet.type == FName(TEXT("Ping"))) { // automatically respond to Ping messages and comsume them
        FNetworkPacket rsp;
        rsp.dest = packet.source;
        rsp.source = address;
        rsp.type = FName(TEXT("Pong"));
        send(rsp);
        return;
    }
    if(!onPacketInternal(packet)) // if the internal function did not consume it send it to listeners
        onNetworkPacket.Broadcast(packet);
}

void UNetworkCard::connect(ACNetwork *newNetwork) {
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
    network->connect(this);
    for (int i = 0; i < multicastPntr; i++)
        network->multicastSubscribe(multicast[i], this);
}

void UNetworkCard::disconnect() {
    if(network == nullptr)
        return;
    if(!staticIP)
        network->releaseIP(hwAddress);
    network->disconnect(this);
    for (int i = 0; i < multicastPntr; i++)
        network->multicastUnSubscribe(multicast[i], this);
}

int UNetworkCard::getIP() {
    return address;
}