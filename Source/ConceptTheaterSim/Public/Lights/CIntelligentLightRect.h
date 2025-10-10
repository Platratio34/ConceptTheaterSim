// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Lights/CIntelligentLight.h"
#include "Components/RectLightComponent.h"
#include "CIntelligentLightRect.generated.h"

class CONCEPTTHEATERSIM_API UCDMXParameter;

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACIntelligentLightRect : public ACIntelligentLight
{
    GENERATED_BODY()

public:
    
	ACIntelligentLightRect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void onLightUpdate() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void OnConstruction(const FTransform &Transform) override;
    
    UPROPERTY(EditDefaultsOnly, Category="Light")
    URectLightComponent* rectLight = nullptr;
};