// Copyright Peter Crall 2025.

#include "Lights/CIntelligentLight.h"
#include "Lights/CDMXParameter.h"

// Sets default values
ACIntelligentLight::ACIntelligentLight()
{
    
}

void ACIntelligentLight::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void ACIntelligentLight::BeginPlay()
{
	Super::BeginPlay();
    
}

// Called every frame
void ACIntelligentLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
}

void ACIntelligentLight::updateDmx(TArray<int> universe)
{
    if(profile == nullptr)
        return;
    profile->updateParameters(parameterValues, universe, startAddress - 1);
    for(UCDMXParameter* p : params)
    {
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