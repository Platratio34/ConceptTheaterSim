// Copyright Peter Crall 2025

#include "Power/DMXDimmer.h"

ADMXDimmer::ADMXDimmer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    networkCard = CreateDefaultSubobject<UDMXNetworkCard>(TEXT("Network Card"));
}

ADMXDimmer::~ADMXDimmer()
{
    
}

void ADMXDimmer::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);
    if(config != nullptr)
        networkCard->addUniverse(config->universe);
    switched = false;
}

void ADMXDimmer::BeginPlay()
{
    Super::BeginPlay();
    
}

void ADMXDimmer::Tick(float DeltaTime)
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
            double nState = 255.0 / dmxV;
            setState(pair.Key, nState * nState);
        }
    }
}