// Copyright Peter Crall 2025.

#include "Cables/CPowerCableConnector.h"
#include "Cables/CCable.h"


UCPowerCableConnector::UCPowerCableConnector()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCPowerCableConnector::BeginPlay()
{
    if(breaker != nullptr)
        breaker->onCircuitChange.AddDynamic(this, &UCPowerCableConnector::onCircuitUpdate);
}

void UCPowerCableConnector::onConnect(ACCable* cable_)
{
    Super::onConnect(cable_);
    if(input)
    {
        if(cable->startConnector)
        {
            UCPowerCableConnector *prev = Cast<UCPowerCableConnector>(cable->startConnector);
            if(!prev)
                return;
            other = prev;

            prev->onSourceUpdate.AddDynamic(this, &UCPowerCableConnector::updateSource);
            if(!(prev->source || prev->isSource))
                return;

            updateSource(prev->isSource? prev : prev->source);
        }
    }
}

void UCPowerCableConnector::onDisconnect(ACCable* cable_)
{
    Super::onDisconnect(cable_);
    if(other != nullptr)
        other->onSourceUpdate.RemoveDynamic(this, &UCPowerCableConnector::updateSource);
    updateSource(nullptr);
}

void UCPowerCableConnector::updateSource(UCPowerCableConnector* src)
{
    source = src;
    if(through != nullptr)
    {
        through->updateSource(src);
    }
    onSourceUpdate.Broadcast(src);
}

void UCPowerCableConnector::updatePower(double newVolts, double newAmps)
{
    volts = newVolts;
    amps = newAmps;
    onPowerChange.Broadcast(volts, amps);
}

double UCPowerCableConnector::getVolts()
{
    if(source != nullptr)
        return source->getVolts();
    return volts;
}

double UCPowerCableConnector::getAmps()
{
    if(source != nullptr)
        return source->getAmps();
    return amps;
}

double UCPowerCableConnector::getWatts()
{
    if(source != nullptr)
        return source->getWatts();
    return volts * amps;
}

void UCPowerCableConnector::updateBreaker(ACBreaker *newBreaker, FName circuit)
{
    breakerCircuit = circuit;
    if(newBreaker == breaker)
    {
        if(breaker != nullptr)
            updatePower(120, 20 * breaker->getState(circuit));
        return;
    }
    if(breaker != nullptr)
        breaker->onCircuitChange.RemoveDynamic(this, &UCPowerCableConnector::onCircuitUpdate);
    breaker = newBreaker;
    if(breaker != nullptr)
        breaker->onCircuitChange.AddDynamic(this, &UCPowerCableConnector::onCircuitUpdate);
}

void UCPowerCableConnector::onCircuitUpdate(FName circuit, double state)
{
    if(circuit != breakerCircuit)
        return;
    updatePower(120, 20 * state);
}