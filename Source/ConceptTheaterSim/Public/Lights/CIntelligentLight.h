// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Lights/CDMXProfile.h"
#include "Lights/CTheatricalLight.h"
#include "Cables/CDMXCableConnector.h"
#include "CIntelligentLight.generated.h"

class CONCEPTTHEATERSIM_API UCDMXParameter;

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACIntelligentLight : public ACTheatricalLight
{
    GENERATED_BODY()

public:
    
	ACIntelligentLight();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DMX")
    int startAddress = 1;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="DMX")
    TMap<FName, int> parameterValues;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DMX")
    UCDMXProfile *profile;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Components")
    UCDMXCableConnector *dmxInput;

    UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category="DMX")
    UCDMXCableConnector *cSource;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleInstanceOnly, Category="DMX");
    TArray<UCDMXParameter *> params;

    virtual double getPower() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void OnConstruction(const FTransform &Transform) override;

    UFUNCTION(BlueprintCallable)
    void updateDmx(TArray<int> universe);

    UFUNCTION(BlueprintCallable)
    int getParameter(FName id);

    UFUNCTION(BlueprintCallable)
    double getParameterNormalized(FName id);

    UFUNCTION(BlueprintCallable)
    FName getParameterIndexed(FName id);

    UFUNCTION(BlueprintCallable)
    bool hasParameter(FName id)
    {
        if(profile == nullptr)
            return false;
        return profile->hasParameter(id);
    }

    UFUNCTION(BlueprintCallable)
    void registerParameter(UCDMXParameter* p)
    {
        params.AddUnique(p);
    }

    UFUNCTION()
    void onSourceUpdate(UCDMXCableConnector *source);
};