// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Cables/CCableConnector.h"
#include "Networking/CNetwork.h"
#include "CNetworkCableConnector.generated.h"

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNetworkSourceUpdate, UCNetworkCableConnector*, source);

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCNetworkCableConnector : public UCCableConnector
{
    GENERATED_BODY()

public:
    
	UCNetworkCableConnector();
    

protected:

    UFUNCTION(BlueprintCallable)
    void setup(UPrimitiveComponent *connectorObject, UCNetworkCableConnector *throughConnector)
    {
        connector = connectorObject;
        through = throughConnector;
    }
    
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

    virtual void onConnect(ACCable* cable) override;

    virtual void onDisconnect(ACCable* cable) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default", meta = (ExposeOnSpawn = true))
    ACNetwork* sourceNetwork;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default", meta = (ExposeOnSpawn = true))
    bool isSource = false;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default", meta = (ExposeOnSpawn = true))
    UCNetworkCableConnector *through = nullptr;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, AdvancedDisplay, Category="Default")
    UCNetworkCableConnector *source = nullptr;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, AdvancedDisplay, Category="Default")
    UCNetworkCableConnector *other = nullptr;

    UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category="Default")
    FOnNetworkSourceUpdate onSourceUpdate;

protected:
    UFUNCTION()
    void updateSource(UCNetworkCableConnector* source_);

private:

};