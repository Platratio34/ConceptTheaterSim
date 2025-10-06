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
    

public:

    virtual void onConnect(ACCable* cable) override;

    virtual void onDisconnect(ACCable* cable) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCDMXCableConnector *through = nullptr;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, AdvancedDisplay)
    UCDMXCableConnector *source = nullptr;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, AdvancedDisplay)
    UCDMXCableConnector *other = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool isSource = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool isReceiver = true;

    UFUNCTION(BlueprintCallable)
    void sendDMX(TArray<int> dmx);

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FOnDMXSourceUpdate onSourceUpdate;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FOnDMXData onDMXData;

protected:
    UFUNCTION()
    void updateSource(UCDMXCableConnector* source_);

private:
    
};