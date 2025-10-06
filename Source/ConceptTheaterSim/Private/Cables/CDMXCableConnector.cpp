// Copyright Peter Crall 2025.

#include "Cables/CDMXCableConnector.h"
#include "Cables/CCable.h"


UCDMXCableConnector::UCDMXCableConnector()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCDMXCableConnector::onConnect(ACCable* cable_)
{
    Super::onConnect(cable_);
    if(input)
    {
        if(cable->startConnector)
        {
            UCDMXCableConnector *prev = Cast<UCDMXCableConnector>(cable->startConnector);
            if(!prev)
                return;
            other = prev;

            prev->onSourceUpdate.AddDynamic(this, &UCDMXCableConnector::updateSource);
            if(!(prev->source || prev->isSource))
                return;

            updateSource(prev->isSource? prev : prev->source);
        }
    }
}

void UCDMXCableConnector::onDisconnect(ACCable* cable_)
{
    Super::onDisconnect(cable_);
    if(other != nullptr)
        other->onSourceUpdate.RemoveDynamic(this, &UCDMXCableConnector::updateSource);
    updateSource(nullptr);
}
void UCDMXCableConnector::sendDMX(TArray<int> dmx)
{
    onDMXData.Broadcast(dmx);
}
void UCDMXCableConnector::updateSource(UCDMXCableConnector* src)
{
    source = src;
    if(through != nullptr)
    {
        through->updateSource(src);
    }
    onSourceUpdate.Broadcast(src);
}

            