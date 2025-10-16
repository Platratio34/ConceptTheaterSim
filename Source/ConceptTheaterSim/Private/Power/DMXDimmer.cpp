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
    lData.Init(-1, 512);
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
        updateDMX(networkCard->getData(config->universe));
    }
}

void ADMXDimmer::updateDMX(TArray<int> data)
{
    for(const TPair<FName, FBreakerCircuitConfig> &pair : config->circuits)
    {
        FBreakerCircuitConfig cConfig = pair.Value;
        if(cConfig.constantPower)
            continue;
        int addr = cConfig.address - 1;
        int lDmxV = lData[addr];
        int dmxV = data[addr];
        lData[addr] = dmxV;
        if(dmxV == lDmxV)
            continue;
        double nState = dmxV / 255.0;
        if(nState > 1)
            nState = 1;
        if(nState < 0)
            nState = 0;
        setState(pair.Key, nState * nState);
    }
}