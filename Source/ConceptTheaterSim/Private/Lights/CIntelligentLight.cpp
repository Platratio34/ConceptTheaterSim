// Copyright Peter Crall 2025.

#include "Lights/CIntelligentLight.h"
#include "Lights/CDMXParameter.h"

// Sets default values
ACIntelligentLight::ACIntelligentLight()
{
    dmxInput = CreateDefaultSubobject<UCDMXCableConnector>(TEXT("DMX Input"));
}

void ACIntelligentLight::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);
    dmxInput->input = true;
}

// Called when the game starts or when spawned
void ACIntelligentLight::BeginPlay()
{
	Super::BeginPlay();
    dmxInput->onSourceUpdate.AddDynamic(this, &ACIntelligentLight::onSourceUpdate);
}

// Called every frame
void ACIntelligentLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
}

void ACIntelligentLight::updateDmx(TArray<int> universe)
{
    dmxCache = universe;
    if(profile == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Missing profile"));
        return;
    }
    profile->updateParameters(&parameterValues, universe, startAddress - 1);
    for(UCDMXParameter* p : params)
    {
        if(p != nullptr)
            p->onDmxUpdate();
    }
}

int ACIntelligentLight::getParameter(FName id)
{
    int *p = parameterValues.Find(id);
    if(!p)
        return 0;
    return *p;
}

double ACIntelligentLight::getParameterNormalized(FName id)
{
    int *p = parameterValues.Find(id);
    if(!p)
        return 0;
    double v = (double)(*p);
    if(profile == nullptr)
    {
        return v / 255.0;
    }
    if(profile->getType(id) == ECDMXProfileParameterType::COARSE)
    {
        return v / 65535.0;
    }
    return v / 255.0;
}

FName ACIntelligentLight::getParameterIndexed(FName id)
{
    int *p = parameterValues.Find(id);
    if(!p)
        return FName("");
    if(profile == nullptr)
        return FName("");
    return profile->getRange(id, *p);
}

TArray<bool> ACIntelligentLight::getParameterBitmask(FName id)
{
    TArray<bool> arr;
    arr.Init(false, 8);
    int *p = parameterValues.Find(id);
    if(!p)
        return arr;
    int v = *p;
    arr[0] = (v & 0x01) != 0;
    arr[1] = (v & 0x02) != 0;
    arr[2] = (v & 0x04) != 0;
    arr[3] = (v & 0x08) != 0;
    arr[4] = (v & 0x10) != 0;
    arr[5] = (v & 0x20) != 0;
    arr[6] = (v & 0x40) != 0;
    arr[7] = (v & 0x80) != 0;
    return arr;
}

void ACIntelligentLight::onSourceUpdate(UCDMXCableConnector *source)
{
    if(source == nullptr)
    {
        if(cSource != nullptr)
            cSource->onDMXData.RemoveDynamic(this, &ACIntelligentLight::updateDmx);
    }
    else
    {
        source->onDMXData.AddDynamic(this, &ACIntelligentLight::updateDmx);
    }
    cSource = source;
}

double ACIntelligentLight::getPower()
{
    return powerInput->getWatts() >= requiredPower ? 1 : 0;
}

void ACIntelligentLight::refreshDMX()
{
    if(dmxCache.Num() < 512)
        return;
    updateDmx(dmxCache);
}