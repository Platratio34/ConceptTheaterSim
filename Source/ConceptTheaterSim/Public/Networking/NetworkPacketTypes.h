// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Networking/NetworkTypes.h"
#include "NetworkPacketTypes.generated.h"

#define DMX_NETWORK_PACKET "DMX"
static FName DMX_NETWORK_PACKET_TYPE = FName(DMX_NETWORK_PACKET);
USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FDMXNetworkPacket : public FNetworkPacket
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    FName sourceDevice;
    UPROPERTY(BlueprintReadWrite)
    int universe = 1;
    UPROPERTY(BlueprintReadWrite)
    int priority = 128;
    UPROPERTY(BlueprintReadWrite)
    TArray<int> dmxData;
};

#define TIMECODE_NETWORK_PACKET "Timecode"
static FName TIMECODE_NETWORK_PACKET_TYPE = FName(TIMECODE_NETWORK_PACKET);
USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FTimecodeNetworkPacket : public FNetworkPacket
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    FName timeSource;
    
    UPROPERTY(BlueprintReadWrite)
    int frames;
    
    UPROPERTY(BlueprintReadWrite)
    double seconds;
    
    UPROPERTY(BlueprintReadWrite)
    bool running = true;
};