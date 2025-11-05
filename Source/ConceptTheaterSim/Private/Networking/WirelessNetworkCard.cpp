// Copyright Peter Crall 2025


#include "Networking/WirelessNetworkCard.h"
#include "Kismet/GameplayStatics.h"

UWirelessNetworkCard::UWirelessNetworkCard() : UNetworkCard()
{
    
}

UWirelessNetworkCard::~UWirelessNetworkCard()
{
    
}

void UWirelessNetworkCard::BeginPlay()
{
	Super::BeginPlay();
}


bool UWirelessNetworkCard::onPacketInternal(UNetworkPacket *packet)
{
    return false;
}

bool UWirelessNetworkCard::checkConnection()
{
    return false;
}

bool UWirelessNetworkCard::connectBySSID(FName ssid)
{
    if(ssid == currentSSID && network != nullptr)
        return true;

    currentSSID = ssid;
    return true;
}