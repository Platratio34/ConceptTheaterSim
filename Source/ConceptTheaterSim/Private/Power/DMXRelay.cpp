// Copyright Peter Crall 2025

#include "Power/DMXRelay.h"

ADMXRelay::ADMXRelay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    networkCard = CreateDefaultSubobject<UDMXNetworkCard>(TEXT("Network Card"));
}

ADMXRelay::~ADMXRelay()
{
    
}

void ADMXRelay::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);
    if(config != nullptr)
        networkCard->addUniverse(config->universe);
    switched = true;
}

void ADMXRelay::BeginPlay()
{
    Super::BeginPlay();
    
}

void ADMXRelay::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(config != nullptr)
        return;
    if(networkCard->hasChanged(config->universe))
    {
        networkCard->clearChanged(config->universe);
        TArray<int> u = networkCard->getData(config->universe);
        for(const TPair<FName, FBreakerCircuitConfig> &pair : config->circuits)
        {
            FBreakerCircuitConfig cConfig = pair.Value;
            if(cConfig.constantPower)
                continue;
            int dmxV = u[cConfig.address];
            double cState = dmxV > 127 ? 1 : 0;
            if(double* p = state.Find(pair.Key))
            {
                if(cState != (*p))
                    setState(pair.Key, cState);
            }
            else
            {
                setState(pair.Key, cState);
            }
        }
    }
}