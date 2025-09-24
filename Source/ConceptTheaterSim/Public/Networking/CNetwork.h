// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CNetwork.generated.h"

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FNetworkPacket
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int dest = -1;

    UPROPERTY(BlueprintReadOnly)
    int source = 0;

    UPROPERTY(BlueprintReadWrite)
    FName type;

    UPROPERTY(BlueprintReadWrite)
    UObject *data = nullptr;
};

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNetworkPacket, FNetworkPacket, packet);

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UCNetwork : public UObject
{
	GENERATED_BODY()
	
public:
    FOnNetworkPacket onPacketOut;

    UFUNCTION(BlueprintCallable)
    void setup(int subnet, int subnetMask, UCNetwork *upstream);

    void sendPacket(FNetworkPacket packet);
    void onUpstreamPacket(FNetworkPacket packet);
    int requestIP(FString hwAddress);
    void releaseIP(FString hwAddress);

    int getSubnet();
    int getSubnetMask();

    UFUNCTION(BlueprintCallable)
    void setUpstream(UCNetwork *upstream);
    UFUNCTION(BlueprintCallable)
    void clearUpstream();

protected:
    int subnet = 0x0A000000;
    int subnetMask = 0xFF000000;

    TMap<FString, int> assignedAddresses;
    int nextAddress = 0x00000001;

    UCNetwork *upstream = nullptr;

    bool isAddressLocal(int addr);
};
