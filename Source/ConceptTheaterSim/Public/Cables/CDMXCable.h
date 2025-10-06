// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Cables/CCable.h"
#include "CDMXCable.generated.h"

// class CONCEPTTHEATERSIM_API UCDMXCableConnector;

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACDMXCable : public ACCable
{
    GENERATED_BODY()

public:
    
	ACDMXCable();
    

protected:
    

public:	
	// Called every frame
    virtual bool canConnectTo(UCCableConnector *connector, bool start) override;
};