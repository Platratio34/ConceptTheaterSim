// Copyright Peter Crall 2025


#include "Networking/DMXNetworkCard.h"

UDMXNetworkCard::UDMXNetworkCard() : UNetworkCard()
{
    cache = NewObject<UDMXCache>();
}

UDMXNetworkCard::~UDMXNetworkCard()
{
    
}

bool UDMXNetworkCard::onPacketInternal(FNetworkPacket packet)
{
    if(packet.type != DMX_NETWORK_PACKET) {
        return false;
    }
    FDMXNetworkPacket dmxPacket = (FDMXNetworkPacket)packet;
    bool has = false;
    for(auto& universe : activeUniverses)
    {
        if(dmxPacket.universe == universe)
        {
            has = true;
            break;
        }
    }
    if(!has)
        return true;
    
    cache->updateSource(dmxPacket.sourceDevice, dmxPacket.priority, dmxPacket.universe, dmxPacket.dmxData);

    return true;
}

TArray<int> UDMXNetworkCard::getData(int universe) {
    return cache->getData(universe);
}

void UDMXNetworkCard::sendData(FName source, int priority, int universe, TArray<int> data) {
    FDMXNetworkPacket dmxPacket;

    dmxPacket.dest = 0xe0000000 | universe;
    dmxPacket.type = DMX_NETWORK_PACKET;

    dmxPacket.sourceDevice = source;
    dmxPacket.priority = priority;

    dmxPacket.universe = universe;
    dmxPacket.dmxData = data;
    
    send(dmxPacket);
}