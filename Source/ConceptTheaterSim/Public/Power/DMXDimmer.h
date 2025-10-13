// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Power/CBreaker.h"
#include "Networking/DMXNetworkCard.h"
#include "DMXDimmer.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ADMXDimmer : public ACBreaker
{
	GENERATED_BODY()
	
public:
    ADMXDimmer();
    ~ADMXDimmer();

    void OnConstruction(const FTransform &Transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Network")
    UDMXNetworkCard* networkCard;

private:

};