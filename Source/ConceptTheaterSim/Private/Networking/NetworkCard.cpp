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

// Called when the game starts or when spawned
void UNetworkCard::BeginPlay()
{
	Super::BeginPlay();
    if(hwAddress.Len() == 0)
        hwAddress = GetOwner()->GetName() + TEXT(":") + GetName();
    if(network != nullptr)
        connectInternal();
}

void UNetworkCard::setup(int ip, FString hw, int subnet_, int subnetMask_)
{
    if (ip > 0)
    {
        address = ip;
        staticIP = true;
        subnet = subnet_;
        subnetMask = subnetMask_;
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
    connectInternal();
}
void UNetworkCard::connectInternal() {
    if(network == nullptr)
        return;
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
    network = nullptr;
}

int UNetworkCard::getIP() {
    return address;
}