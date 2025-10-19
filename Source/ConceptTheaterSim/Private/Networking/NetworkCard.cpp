// Copyright Peter Crall 2025

#include "Networking/NetworkCard.h"

#include "Networking/CNetwork.h"

UNetworkCard::UNetworkCard()
{
    
}

UNetworkCard::~UNetworkCard()
{
    
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
    if(multicast.Contains(multicastAddress))
        return;
    multicast.Add(multicastAddress);
    if(network != nullptr) {
        network->multicastSubscribe(multicastAddress, this);
    }
}
void UNetworkCard::multicastUnSubscribe(int multicastAddress)
{
    if(!multicast.Contains(multicastAddress))
        return;
    multicast.Remove(multicastAddress);
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
    for (int multicastAddress : multicast)
    {
        network->multicastSubscribe(multicastAddress, this);
    }
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

FString UNetworkCard::getHWAddress() {
    return hwAddress;
}