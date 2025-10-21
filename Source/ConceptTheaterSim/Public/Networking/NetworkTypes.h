// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkTypes.generated.h"

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UNetworkPacket : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    int dest = -1;

    UPROPERTY(BlueprintReadOnly)
    int source = 0;

    UPROPERTY(BlueprintReadWrite)
    FName type;

    UPROPERTY(BlueprintReadWrite)
    UObject *data = nullptr;

    static UNetworkPacket* createPacket(int dest, FName type)
    {
        UNetworkPacket* packet = NewObject<UNetworkPacket>();
        packet->dest = dest;
        packet->type = type;
        return packet;
    }
};

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNetworkPacket, UNetworkPacket*, packet);


class CONCEPTTHEATERSIM_API ACNetwork;

class CONCEPTTHEATERSIM_API UNetworkCard;