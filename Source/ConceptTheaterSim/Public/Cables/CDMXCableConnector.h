// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Cables/CCableConnector.h"
#include "CDMXCableConnector.generated.h"

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDMXSourceUpdate, UCDMXCableConnector*, source);

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDMXData, TArray<int>, dmx);

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCDMXCableConnector : public UCCableConnector
{
    GENERATED_BODY()

public:
    
	UCDMXCableConnector();
    

protected:

    UFUNCTION(BlueprintCallable)
    void setup(UPrimitiveComponent *connectorObject, UCDMXCableConnector *throughConnector)
    {
        connector = connectorObject;
        through = throughConnector;
    }
    

public:

    virtual void onConnect(ACCable* cable) override;

    virtual void onDisconnect(ACCable* cable) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default", meta = (ExposeOnSpawn = true))
    UCDMXCableConnector *through = nullptr;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, AdvancedDisplay, Category="Default")
    UCDMXCableConnector *source = nullptr;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, AdvancedDisplay, Category="Default")
    UCDMXCableConnector *other = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DMX", meta = (ExposeOnSpawn = true))
    bool isSource = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DMX", meta = (ExposeOnSpawn = true))
    bool isReceiver = true;

    UFUNCTION(BlueprintCallable)
    void sendDMX(TArray<int> dmx);

    UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category="Default")
    FOnDMXSourceUpdate onSourceUpdate;

    UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category="DMX")
    FOnDMXData onDMXData;

protected:
    UFUNCTION()
    void updateSource(UCDMXCableConnector* source_);

    UFUNCTION()
    void onData(TArray<int> dmx);

private:
};