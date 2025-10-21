// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Networking/NetworkTypes.h"
#include "NetworkPacketTypes.generated.h"

#define DMX_NETWORK_PACKET "DMX"
static FName DMX_NETWORK_PACKET_TYPE = FName(DMX_NETWORK_PACKET);
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UDMXNetworkPacket : public UNetworkPacket
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

    static UDMXNetworkPacket* createDMXPacket(FName source, int universe, int priority, TArray<int> data)
    {
        UDMXNetworkPacket* packet = NewObject<UDMXNetworkPacket>();
        packet->dest = getAddress(universe);
        packet->type = DMX_NETWORK_PACKET_TYPE;
        packet->sourceDevice = source;
        packet->universe = universe;
        packet->priority = priority;
        packet->dmxData = data;
        return packet;
    }

    static int getAddress(int universe)
    {
        return 0xe0000000 | universe;
    }
};

#define TIMECODE_NETWORK_PACKET "Timecode"
static FName TIMECODE_NETWORK_PACKET_TYPE = FName(TIMECODE_NETWORK_PACKET);
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UTimecodeNetworkPacket : public UNetworkPacket
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

    static UTimecodeNetworkPacket* createTCPacket(FName source, int frames, double seconds, bool running)
    {
        UTimecodeNetworkPacket* packet = NewObject<UTimecodeNetworkPacket>();
        packet->dest = 0xffffffff;
        packet->type = TIMECODE_NETWORK_PACKET_TYPE;
        packet->timeSource = source;
        packet->frames = frames;
        packet->seconds = seconds;
        packet->running = running;
        return packet;
    }
};