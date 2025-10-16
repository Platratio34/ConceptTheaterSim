// Copyright Peter Crall 2025

#include "Networking/CNetwork.h"

#include "Networking/NetworkCard.h"

ACNetwork::ACNetwork()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ACNetwork::~ACNetwork()
{
    
}


void ACNetwork::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);
    if(upstream != nullptr && upstreamSameNet)
    {
        subnet = upstream->subnet;
        subnetMask = upstream->subnetMask;
    }
}

void ACNetwork::BeginPlay()
{
    Super::BeginPlay();
    if(upstream != nullptr)
    {
        upstream->onPacketOut.AddDynamic(this, &ACNetwork::onUpstreamPacket);
        if(upstreamSameNet)
        {
            subnet = upstream->subnet;
            subnetMask = upstream->subnetMask;
        }
    }
}

void ACNetwork::setup(int _subnet, int _subnetMask, ACNetwork *_upstream, bool sameNet)
{
    subnet = _subnet;
    subnetMask = _subnetMask;
    if (_upstream)
        setUpstream(_upstream, sameNet);
}

bool ACNetwork::isAddressLocal(int addr)
{
    int packetSubnet = addr & subnetMask;
    bool isMulticast = (addr & 0xF0000000) == 0xE0000000;
    return packetSubnet == subnet || addr == -1 || isMulticast;
}

void ACNetwork::sendPacket(FNetworkPacket packet)
{
    sendPacketInt(packet, false);
}

void ACNetwork::onUpstreamPacket(FNetworkPacket packet)
{
    sendPacketInt(packet, true);
}

void ACNetwork::sendPacketInt(FNetworkPacket packet, bool fromUpstream)
{
    int dest = packet.dest;
    int packetSubnet = dest & subnetMask;
    int localBroadcast = subnet | (~subnetMask);

    bool upOnly = !fromUpstream && upstreamSameNet;
    if(!fromUpstream && upstreamSameNet && upstream != nullptr) // we are a sub-switch, so always pass up
    {
        upstream->sendPacketInt(packet, false);
    }
    else if (packetSubnet == subnet || dest == -1) // in our network, or all network broadcast
    {
        onPacketOut.Broadcast(packet);
        bool broadcast = dest == -1 || dest == localBroadcast;
        for(auto& card : cards)
        {
            if(broadcast || dest == card->getIP())
            {
                card->onPacket(packet);
            }
        }
    }
    else if ((dest & 0xF0000000) == 0xE0000000) // multicast
    {
        onPacketOut.Broadcast(packet);
        for (UMulticastTargetSet *set : multicastSets)
        {
            if (set->address == dest)
            {
                set->sendPacket(packet);
                break;
            }
        }
    }
    else if (upstream != nullptr && !fromUpstream) // else send to upstream if present
    {
        upstream->sendPacket(packet);
    }
}

int ACNetwork::requestIP(FString hwAddress)
{
    if(upstreamSameNet && upstream != nullptr)
    {
        return upstream->requestIP(hwAddress);
    }
    if (int *addrPntr = assignedAddresses.Find(hwAddress))
    {
        return *addrPntr;
    }
    int addr = (nextAddress++) | subnet;
    assignedAddresses.Add(hwAddress, addr);
    if (nextAddress >= ~subnetMask)
    {
        // TODO: do something here?
    }
    return addr;
}

void ACNetwork::releaseIP(FString hwAddress)
{
    if(upstreamSameNet && upstream != nullptr)
    {
        upstream->releaseIP(hwAddress);
        return;
    }
    assignedAddresses.Remove(hwAddress);
}

int ACNetwork::getSubnet()
{
    return subnet;
}
int ACNetwork::getSubnetMask()
{
    return subnetMask;
}

void ACNetwork::setUpstream(ACNetwork *newUpstream, bool sameNet)
{
    upstreamSameNet = sameNet;
    if (upstream == newUpstream)
        return;
    if (upstream != nullptr)
    {
        clearUpstream();
    }
    upstream = newUpstream;
    if(upstream == nullptr)
        return;
    upstream->onPacketOut.AddDynamic(this, &ACNetwork::onUpstreamPacket);
    if(sameNet)
    {
        subnet = upstream->subnet;
        subnetMask = upstream->subnetMask;
    }
}
void ACNetwork::clearUpstream()
{
    if (upstream == nullptr)
        return;
    upstream->onPacketOut.RemoveDynamic(this, &ACNetwork::onUpstreamPacket);
    upstream = nullptr;
}

void ACNetwork::multicastSubscribe(int address, UNetworkCard *subscriber)
{
    for(UMulticastTargetSet *set : multicastSets)
    {
        if(set == nullptr)
            continue;
        if (set->address == address)
        {
            set->addSubscriber(subscriber);
            return;
        }
    }
    UMulticastTargetSet *set = NewObject<UMulticastTargetSet>();
    set->address = address;
    multicastSets.Add(set);
    set->addSubscriber(subscriber);
}

void ACNetwork::multicastUnSubscribe(int address, UNetworkCard *subscriber)
{
    for (UMulticastTargetSet *set : multicastSets)
    {
        if (set->address == address)
        {
            set->removeSubscriber(subscriber);
            return;
        }
    }
}

void ACNetwork::connect(UNetworkCard *card) {
    cards.AddUnique(card);
}
void ACNetwork::disconnect(UNetworkCard *card) {
    cards.Remove(card);
    for (UMulticastTargetSet *set : multicastSets)
    {
        set->removeSubscriber(card);
    }
}


UMulticastTargetSet::UMulticastTargetSet()
{
    
}
UMulticastTargetSet::~UMulticastTargetSet()
{
    
}

void UMulticastTargetSet::addSubscriber(UNetworkCard *subscriber)
{
    subscribers.AddUnique(subscriber);
}

void UMulticastTargetSet::removeSubscriber(UNetworkCard *subscriber)
{
    subscribers.Remove(subscriber);
}

void UMulticastTargetSet::sendPacket(FNetworkPacket packet) {
    for (UNetworkCard *subscriber : subscribers)
    {
        subscriber->onPacket(packet);
    }
}