// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Cables/CCableConnector.h"
#include "Power/CBreaker.h"
#include "CPowerCableConnector.generated.h"

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPowerSourceUpdate, UCPowerCableConnector*, source);

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPowerChange, double, volts, double, amps);

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCPowerCableConnector : public UCCableConnector
{
    GENERATED_BODY()

public:
    
	UCPowerCableConnector();
    

protected:

    UFUNCTION(BlueprintCallable)
    void setup(UPrimitiveComponent *connectorObject, UCPowerCableConnector *throughConnector)
    {
        connector = connectorObject;
        through = throughConnector;
    }
    
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

    virtual void onConnect(ACCable* cable) override;

    virtual void onDisconnect(ACCable* cable) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Power", meta = (ExposeOnSpawn = true))
    FName connectorType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Power", meta = (ExposeOnSpawn = true))
    bool isSource = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default", meta = (ExposeOnSpawn = true))
    UCPowerCableConnector *through = nullptr;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, AdvancedDisplay, Category="Default")
    UCPowerCableConnector *source = nullptr;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, AdvancedDisplay, Category="Default")
    UCPowerCableConnector *other = nullptr;

    UFUNCTION(BlueprintCallable)
    void updatePower(double newVolts, double newAmps);

    UFUNCTION(BlueprintCallable)
    double getVolts();

    UFUNCTION(BlueprintCallable)
    double getAmps();

    UFUNCTION(BlueprintCallable)
    double getWatts();

    UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category="Default")
    FOnPowerSourceUpdate onSourceUpdate;

    UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category="Power")
    FOnPowerChange onPowerChange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Power", meta = (ExposeOnSpawn = true))
    ACBreaker *breaker;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Power", meta = (ExposeOnSpawn = true))
    FName breakerCircuit;

    UFUNCTION(BlueprintCallable)
    void updateBreaker(ACBreaker *newBreaker, FName circuit);

protected:
    UFUNCTION()
    void updateSource(UCPowerCableConnector* source_);

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Power")
    double volts;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Power")
    double amps;

private:
    UFUNCTION()
    void onCircuitUpdate(FName circuit, double state);
};