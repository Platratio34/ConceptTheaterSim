// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkTypes.generated.h"

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


class CONCEPTTHEATERSIM_API ACNetwork;

class CONCEPTTHEATERSIM_API UNetworkCard;