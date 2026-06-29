// Copyright Peter Crall 2025.

#include "Cables/CNetworkCableConnector.h"
#include "Cables/CCable.h"


UCNetworkCableConnector::UCNetworkCableConnector()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCNetworkCableConnector::BeginPlay()
{
    
}

void UCNetworkCableConnector::onConnect(ACCable* cable_)
{
    Super::onConnect(cable_);
    if(input)
    {
        if(cable->startConnector)
        {
            UCNetworkCableConnector *prev = Cast<UCNetworkCableConnector>(cable->startConnector);
            if(!prev)
                return;
            other = prev;

            prev->onSourceUpdate.AddDynamic(this, &UCNetworkCableConnector::updateSource);
            if(!(prev->source || prev->isSource))
                return;

            updateSource(prev->isSource? prev : prev->source);
        }
    }
}

void UCNetworkCableConnector::onDisconnect(ACCable* cable_)
{
    Super::onDisconnect(cable_);
    if(other != nullptr)
        other->onSourceUpdate.RemoveDynamic(this, &UCNetworkCableConnector::updateSource);
    updateSource(nullptr);
}

void UCNetworkCableConnector::updateSource(UCNetworkCableConnector* src)
{
    source = src;
    if(through != nullptr) {
        through->updateSource(src);
    }
    onSourceUpdate.Broadcast(src);
}