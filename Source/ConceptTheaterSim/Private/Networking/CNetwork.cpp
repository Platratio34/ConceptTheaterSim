// Copyright Peter Crall 2025

#include "Networking/CNetwork.h"

#include "Networking/NetworkCard.h"

ACNetwork::ACNetwork()
{
    multicastSets = new MulticastTargetSet *[multicastSize];
}

ACNetwork::~ACNetwork()
{
    for (int i = 0; i < multicastPntr; i++)
    {
        delete multicastSets[i];
    }
    delete[] multicastSets;
}

void ACNetwork::setup(int _subnet, int _subnetMask, ACNetwork *_upstream)
{
    subnet = _subnet;
    subnetMask = _subnetMask;
    if (_upstream)
        setUpstream(_upstream);
}

bool ACNetwork::isAddressLocal(int addr)
{
    int packetSubnet = addr & subnetMask;
    bool isMulticast = (addr & 0xF0000000) == 0xE0000000;
    return packetSubnet == subnet || addr == -1 || isMulticast;
}

void ACNetwork::sendPacket(FNetworkPacket packet)
{
    sendPacketInt(packet, false, nullptr);
}

void ACNetwork::onUpstreamPacket(FNetworkPacket packet)
{
    sendPacketInt(packet, true, nullptr);
}

void ACNetwork::sendPacketInt(FNetworkPacket packet, bool fromUpstream, ACNetwork* sourceNet)
{
    int dest = packet.dest;
    int packetSubnet = dest & subnetMask;
    int localBroadcast = subnet | (~subnetMask);
    
    if (packetSubnet == subnet || dest == -1) // in our network, or all network broadcast
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
        for (int i = 0; i < multicastPntr; i++)
        {
            MulticastTargetSet *set = multicastSets[i];
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

void ACNetwork::setUpstream(ACNetwork *newUpstream)
{
    if (upstream == newUpstream)
        return;
    if (upstream != nullptr)
    {
        clearUpstream();
    }
    upstream = newUpstream;
    upstream->onPacketOut.AddDynamic(this, &ACNetwork::onUpstreamPacket);
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
    for (int i = 0; i < multicastPntr; i++)
    {
        MulticastTargetSet *set = multicastSets[i];
        if (set->address == address)
        {
            set->addSubscriber(subscriber);
            return;
        }
    }
    if (multicastPntr == multicastSize)
    {
        multicastSize *= 2;
        MulticastTargetSet **newArr = new MulticastTargetSet *[multicastSize];
        for (int i = 0; i < multicastPntr; i++)
        {
            newArr[i] = multicastSets[i];
        }
        delete[] multicastSets;
        multicastSets = newArr;
    }
    MulticastTargetSet *set = new MulticastTargetSet(address);
    multicastSets[multicastPntr++] = set;
    set->addSubscriber(subscriber);
}

void ACNetwork::multicastUnSubscribe(int address, UNetworkCard *subscriber)
{
    for (int i = 0; i < multicastPntr; i++)
    {
        MulticastTargetSet *set = multicastSets[i];
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
}


MulticastTargetSet::MulticastTargetSet(int address_)
{
    address = address_;
    subscribers = new UNetworkCard *[subSize];
}
MulticastTargetSet::~MulticastTargetSet()
{
    delete[] subscribers;
}

void MulticastTargetSet::addSubscriber(UNetworkCard *subscriber)
{
    for (int i = 0; i < subPntr; i++)
    {
        if (subscribers[i] == subscriber)
            return;
    }
    if (subPntr == subSize)
    {
        subSize += 4;
        UNetworkCard **newArr = new UNetworkCard *[subSize];
        for (int i = 0; i < subPntr; i++)
        {
            newArr[i] = subscribers[i];
        }
        delete[] subscribers;
        subscribers = newArr;
    }
    subscribers[subPntr++] = subscriber;
}

void MulticastTargetSet::removeSubscriber(UNetworkCard *subscriber)
{
    bool after = false;
    for (int i = 0; i < subPntr; i++)
    {
        if (subscribers[i] == subscriber)
        {
            after = true;
        }
        else if (after)
        {
            subscribers[i - 1] = subscribers[i];
        }
    }
    if (after)
        subPntr--;
}

void MulticastTargetSet::sendPacket(FNetworkPacket packet) {
    for (int i = 0; i < subPntr; i++)
    {
        subscribers[i]->onPacket(packet);
    }
}