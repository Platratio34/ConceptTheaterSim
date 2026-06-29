// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Cables/CCable.h"
#include "CNetworkCable.generated.h"

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACNetworkCable : public ACCable
{
    GENERATED_BODY()

public:
    
	ACNetworkCable();
    

protected:
    

public:	
	// Called every frame
    virtual bool canConnectTo(UCCableConnector *connector, bool start) override;
};