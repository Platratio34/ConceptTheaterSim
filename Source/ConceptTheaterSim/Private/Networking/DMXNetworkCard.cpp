// Copyright Peter Crall 2025


#include "Networking/DMXNetworkCard.h"

UDMXNetworkCard::UDMXNetworkCard() : UNetworkCard()
{
    
}

UDMXNetworkCard::~UDMXNetworkCard()
{
    
}

void UDMXNetworkCard::BeginPlay()
{
	Super::BeginPlay();

    cache = NewObject<UDMXCache>();
}


bool UDMXNetworkCard::onPacketInternal(UNetworkPacket *packet)
{
    if(packet->type != DMX_NETWORK_PACKET_TYPE) {
        return false;
    }
    UDMXNetworkPacket *dmxPacket = Cast<UDMXNetworkPacket>(packet);
    bool has = false;
    for(auto& universe : activeUniverses)
    {
        if(dmxPacket->universe == universe)
        {
            has = true;
            break;
        }
    }
    if(!has)
        return true;
    
    if(dmxPacket->dmxData.Num() != 512)
    {
        UE_LOG(LogTemp, Error, TEXT("DMX data incorrect size in onPacketInternal: Expected 512 elements, found %d"), dmxPacket->dmxData.Num());
        return true;
    }
    cache->updateSource(dmxPacket->sourceDevice, dmxPacket->priority, dmxPacket->universe, dmxPacket->dmxData);
    changedUniverses.Add(dmxPacket->universe, true);

    return true;
}

TArray<int> UDMXNetworkCard::getData(int universe) {
    return cache->getData(universe);
}

void UDMXNetworkCard::sendData(FName source, int priority, int universe, TArray<int> data) {
    if(data.Num() != 512)
    {
        UE_LOG(LogTemp, Error, TEXT("DMX data incorrect size in sendData: Expected 512 elements, found %d"), data.Num());
        return;
    }
    send(UDMXNetworkPacket::createDMXPacket(source, universe, priority, data));
}

bool UDMXNetworkCard::hasChanged(int universe)
{
    if(bool* changed = changedUniverses.Find(universe))
        return *changed;
    return false;
}

void UDMXNetworkCard::clearChanged(int universe)
{
    if(universe <= 0)
        changedUniverses.Empty();
    changedUniverses.Remove(universe);
}

void UDMXNetworkCard::addUniverse(int universe)
{
    if(activeUniverses.Contains(universe))
        return;
    activeUniverses.Add(universe);
    multicastSubscribe(UDMXNetworkPacket::getAddress(universe));
}

void UDMXNetworkCard::removeUniverse(int universe)
{
    if(!activeUniverses.Contains(universe))
        return;
    activeUniverses.Remove(universe);
    multicastUnSubscribe(UDMXNetworkPacket::getAddress(universe));
}