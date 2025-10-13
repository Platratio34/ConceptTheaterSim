// Copyright Peter Crall 2025

#include "Power/CBreaker.h"

ACBreaker::ACBreaker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
}

ACBreaker::~ACBreaker()
{
    
}

void ACBreaker::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);
}

void ACBreaker::BeginPlay()
{
    Super::BeginPlay();
    if(config != nullptr)
    {
        for(const TPair<FName, FBreakerCircuitConfig> &pair : config->circuits)
        {
            FBreakerCircuitConfig cConfig = pair.Value;
            setState(pair.Key, cConfig.constantPower ? 1 : cConfig.defaultState);
        }
    }
}

void ACBreaker::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

void ACBreaker::setState(FName circuit, double newState)
{
    bool cSwitched = false;
    if(config != nullptr)
    {
        if(FBreakerCircuitConfig* cfg = config->circuits.Find(circuit))
        {
            if(cfg->constantPower)
                return;
            cSwitched = cfg->switched;
        }
    }
    if(cSwitched || switched)
        newState = (newState >= 0.5) ? 1 : 0;
    state.Add(circuit, newState);
    onCircuitChange.Broadcast(circuit, newState);
}

double ACBreaker::getState(FName circuit)
{
    if(double* p = state.Find(circuit))
        return *p;
    return 0;
}