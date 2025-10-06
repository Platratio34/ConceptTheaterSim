// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Lights/CIntelligentLight.h"
#include "CDMXParameter.generated.h"

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCDMXParameter : public UActorComponent
{
    
    GENERATED_BODY()

public:
    
	UCDMXParameter();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    virtual void onDmxUpdate() {}

    UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
    ACIntelligentLight *light;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    static double range(double v, double min, double max)
    {
        v *= (max - min);
        return v + min;
    }

private:

};