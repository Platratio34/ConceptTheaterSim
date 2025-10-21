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

void ACNetwork::sendPacket(UNetworkPacket *packet)
{
    sendPacketInt(packet, false);
}

void ACNetwork::onUpstreamPacket(UNetworkPacket *packet)
{
    sendPacketInt(packet, true);
}

void ACNetwork::sendPacketInt(UNetworkPacket *packet, bool fromUpstream)
{
    int dest = packet->dest;
    int packetSubnet = dest & subnetMask;
    int localBroadcast = subnet | (~subnetMask);

    bool upOnly = !fromUpstream && upstreamSameNet;
    if(!fromUpstream && upstreamSameNet && upstream != nullptr) // we are a sub-switch, so always pass up
    {
        upstream->sendPacketInt(packet, false);
    }
    else if (packetSubnet == subnet || dest == -1 || dest == 0xffffffff) // in our network, or all network broadcast
    {
        numUnicastPackets++;
        onPacketOut.Broadcast(packet);
        bool broadcast = dest == -1 || dest == localBroadcast || dest == 0xffffffff;
        if(!broadcast)
        {
            bool sent = false;
            if (UNetworkCard** p = cardsByIP.Find(dest))
            {
                UNetworkCard *card = *p;
                if(card != nullptr)
                {
                    card->onPacket(packet);
                    sent = true;
                }
            }
            if(!sent) // this is a backup, incase the cards-by-IP map is wrong
            {
                for(auto& card : cards)
                {
                    if(card == nullptr)
                        continue;
                    if(card->getIP() == dest)
                    {
                        card->onPacket(packet);
                        sent = true;
                        break;
                    }
                }
            }
            if(!sent)
            {
                UE_LOG(LogTemp, Warning, TEXT("Network %s did not have device for IP %s"), *GetName(), *ipToString(dest));
            }
        }
        else
        {
            for(auto& card : cards)
            {
                if(card == nullptr)
                    continue;
                card->onPacket(packet);
            }
        }
    }
    else if ((dest & 0xF0000000) == 0xE0000000) // multicast
    {
        onPacketOut.Broadcast(packet);
        UMulticastTargetSet **p = multicastSets.Find(dest);
        if (p != nullptr && *p != nullptr)
        {
            UMulticastTargetSet *set = *p;
            set->sendPacket(packet);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Network %s did not have any listeners for multicast address %s"), *GetName(), *ipToString(dest));
        }
        numMulticastPackets++;
    }
    else if (upstream != nullptr && !fromUpstream) // else send to upstream if present
    {
        upstream->sendPacket(packet);
        packetsUp++;
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
    }
    else
    {
        assignedAddresses.Remove(hwAddress);
    }
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
    UMulticastTargetSet **p = multicastSets.Find(address);
    UMulticastTargetSet *set = nullptr;
    if(p && (*p) != nullptr)
    {
        set = *p;
    }
    else
    {
        set = NewObject<UMulticastTargetSet>();
        set->address = address;
        multicastSets.Add(address, set);
    }
    set->addSubscriber(subscriber);
}

void ACNetwork::multicastUnSubscribe(int address, UNetworkCard *subscriber)
{
    if(UMulticastTargetSet **p = multicastSets.Find(address))
    {
        UMulticastTargetSet *set = *p;
        if(set != nullptr)
        {
            set->removeSubscriber(subscriber);
        }
    }
}

void ACNetwork::connect(UNetworkCard *card) {
    if(card == nullptr)
        return;
    cards.AddUnique(card);
    if(card->getIP() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Card %s had no IP address when connecting to network %s"), *(card->getHWAddress()), *GetName());
        return;
    }
    if(cardsByIP.Contains(card->getIP()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Duplicate IP address in network %s for card %s. It will not receive unicast messages"), *GetName(), *(card->getHWAddress()));
    }
    else
    {
        cardsByIP.Add(card->getIP(), card);
    }
}

void ACNetwork::disconnect(UNetworkCard *card) {
    cards.Remove(card);
    for (TPair<int, UMulticastTargetSet*> pair : multicastSets)
    {
        UMulticastTargetSet *set = pair.Value;
        if(set != nullptr)
            set->removeSubscriber(card);
    }
    for(TPair<int, UNetworkCard*> pair : cardsByIP)
    {
        if(card == pair.Value)
        {
            cardsByIP.Remove(pair.Key);
            break;
        }
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

void UMulticastTargetSet::sendPacket(UNetworkPacket *packet) {
    subscribers.RemoveAll([](const auto *el)
                            { return el == nullptr; });
    for (UNetworkCard *subscriber : subscribers)
    {
        subscriber->onPacket(packet);
    }
}