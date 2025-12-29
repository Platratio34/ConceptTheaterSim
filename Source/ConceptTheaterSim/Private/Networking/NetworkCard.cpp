// Copyright Peter Crall 2025

#include "Networking/NetworkCard.h"

#include "Networking/CNetwork.h"
#include "Networking/NetworkSocket.h"

UNetworkCard::UNetworkCard()
{
    PrimaryComponentTick.bCanEverTick = true;
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

void UNetworkCard::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    for(TPair<int, UNetworkSocket*> pair : sockets)
    {
        pair.Value->update();
    }
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

void UNetworkCard::send(UNetworkPacket *packet)
{
    packet->source = address;
    if(network != nullptr) {
        network->sendPacket(packet);
    }
    packetsOut++;
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

void UNetworkCard::onPacket(UNetworkPacket *packet)
{
    if(packet->type == FName(TEXT("Ping"))) { // automatically respond to Ping messages and comsume them
        UNetworkPacket *rsp = UNetworkPacket::createPacket(packet->source, FName(TEXT("Pong")));
        rsp->source = address;
        send(rsp);
        return;
    }
    packetsIn++;
    if(UNetworkSocket** p = sockets.Find(packet->destPort))
    {
        UNetworkSocket *socket = *p;
        int tgtIP = socket->getIP();
        if(socket->getType() == packet->type && (tgtIP == 0 || tgtIP == packet->source))
        {
            socket->onPacket(packet);
            return; // comsume socket packets as well
        }
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

void UNetworkCard::sendBroadcast(UNetworkPacket *packet)
{
    packet->dest = subnet | (~subnetMask);
    send(packet);
}

UNetworkSocket *UNetworkCard::createSocket(int ip, FName type, int port)
{
    UNetworkSocket *socket = NewObject<UNetworkSocket>();
    int outPort = nextPortOut++;
    while(sockets.Contains(outPort))
    {
        outPort++;
    }
    socket->setup(this, ip, type, outPort, port);
    sockets.Add(outPort, socket);
    return socket;
}

void UNetworkCard::closeSocket(int port)
{
    if(UNetworkSocket** p = sockets.Find(port))
    {
        sockets.Remove(port);
    }
}

UNetworkSocket *UNetworkCard::openSocket(FName type, int port)
{
    if(sockets.Contains(port))
    {
        return nullptr;
    }
    UNetworkSocket *socket = NewObject<UNetworkSocket>();
    int outPort = nextPortOut++;
    while(sockets.Contains(outPort))
    {
        outPort++;
    }
    socket->setup(this, 0, type, outPort, port);
    sockets.Add(outPort, socket);
    return socket;
}