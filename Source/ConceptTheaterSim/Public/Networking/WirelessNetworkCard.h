// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Networking/NetworkCard.h"
#include "Networking/NetworkPacketTypes.h"
#include "WirelessNetworkCard.generated.h"

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACWirelessNetwork : public AActor
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName ssid;
};

/**
 * 
 */
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UWirelessNetworkCard : public UNetworkCard
{
	GENERATED_BODY()

public:
    UWirelessNetworkCard();
    ~UWirelessNetworkCard();
    
    UFUNCTION(BlueprintCallable)
    bool checkConnection();

    UFUNCTION(BlueprintCallable)
    bool connectBySSID(FName ssid);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual bool onPacketInternal(UNetworkPacket *packet) override;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    FName currentSSID;
};
