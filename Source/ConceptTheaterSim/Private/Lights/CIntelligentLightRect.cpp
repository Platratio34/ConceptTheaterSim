// Copyright Peter Crall 2025.

#include "Lights/CIntelligentLightRect.h"

// Sets default values
ACIntelligentLightRect::ACIntelligentLightRect()
{
    rectLight = CreateDefaultSubobject<URectLightComponent>(TEXT("RectLight"));
    rectLight->SetupAttachment(body);
}

void ACIntelligentLightRect::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);

    light->SetVisibility(false);
}

// Called when the game starts or when spawned
void ACIntelligentLightRect::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACIntelligentLightRect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
}
void ACIntelligentLightRect::onLightUpdate()
{
    rectLight->SetIntensity(actualIntensity * maxIntensity);
    rectLight->SetLightColor(color);
}