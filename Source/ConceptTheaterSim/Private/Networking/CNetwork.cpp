// Copyright Peter Crall 2025


#include "Networking/CNetwork.h"

void UCNetwork::setup(int _subnet, int _subnetMask, UCNetwork *_upstream) {
    subnet = _subnet;
    subnetMask = _subnetMask;
    if(_upstream)
        setUpstream(_upstream);
}

bool UCNetwork::isAddressLocal(int addr) {
    int packetSubnet = addr & subnetMask;
    bool isMulticast = (addr & 0xF0000000) == 0xE0000000;
    return packetSubnet == subnet || addr == -1 || isMulticast;
}

void UCNetwork::sendPacket(FNetworkPacket packet)
{
    if(isAddressLocal(packet.dest)) { // in our network, all network broadcast, or multicast
        onPacketOut.Broadcast(packet);
    } else if(upstream != nullptr) { // else send to upstream if present
        upstream->sendPacket(packet);
    }
}

void UCNetwork::onUpstreamPacket(FNetworkPacket packet)
{
    // we arn't using sendPacket because we don't want to accidentally re-send it up the chain to the upstream
    if(isAddressLocal(packet.dest)) {
        onPacketOut.Broadcast(packet);
    }
}

int UCNetwork::requestIP(FString hwAddress)
{
    if(int* addrPntr = assignedAddresses.Find(hwAddress)) {
        return *addrPntr;
    }
    int addr = (nextAddress++) | subnet;
    assignedAddresses.Add(hwAddress, addr);
    if(nextAddress >= ~subnetMask) {
        // TODO: do something here?
    }
    return addr;
}

void UCNetwork::releaseIP(FString hwAddress) {
    assignedAddresses.Remove(hwAddress);
}

int UCNetwork::getSubnet() {
    return subnet;
}
int UCNetwork::getSubnetMask() {
    return subnetMask;
}

void UCNetwork::setUpstream(UCNetwork* newUpstream)
{
    if(upstream == newUpstream)
        return;
    if(upstream != nullptr) {
        clearUpstream();
    }
    upstream = newUpstream;
    upstream->onPacketOut.AddDynamic(this, &UCNetwork::onUpstreamPacket);
}
void UCNetwork::clearUpstream()
{
    if(upstream == nullptr)
        return;
    upstream->onPacketOut.RemoveDynamic(this, &UCNetwork::onUpstreamPacket);
    upstream = nullptr;
}