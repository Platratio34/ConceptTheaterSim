// Copyright Peter Crall 2025.

#include "Lights/CDMXParameter.h"

UCDMXParameter::UCDMXParameter()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void UCDMXParameter::BeginPlay()
{
	Super::BeginPlay();
    AActor *ow = GetOwner();
    ACIntelligentLight *l = Cast<ACIntelligentLight>(ow);
    if(l)
    {
        light = l;
        light->registerParameter(this);
    }
}

// Called every frame
void UCDMXParameter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}