// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Cables/CCable.h"
#include "CPowerCable.generated.h"

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACPowerCable : public ACCable
{
    GENERATED_BODY()

public:
    
	ACPowerCable();
    

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Power")
    FName startType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Power")
    FName endType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Power")
    double maxAmps = 20;

public:	
	// Called every frame
    virtual bool canConnectTo(UCCableConnector *connector, bool start) override;
};