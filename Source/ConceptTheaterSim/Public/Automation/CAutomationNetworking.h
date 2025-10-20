// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Networking/NetworkTypes.h"
#include "CAutomationNetworking.generated.h"

#define AUTOMATION_NETWORK_PACKET "Automation"
static FName AUTOMATION_NETWORK_PACKET_TYPE = FName(AUTOMATION_NETWORK_PACKET);

UENUM(BlueprintType)
enum class EAutomationPacketType : uint8
{
    PING UMETA(DisplayName="Ping"),
    PONG UMETA(DisplayName="Pong"),
    EVENT UMETA(DisplayName="Event"),
    QUERY UMETA(DisplayName="Query"),
    QUERY_RESPONSE UMETA(DisplayName="Query Response"),
    STATUS UMETA(DisplayName="Status"),
    E_STOP UMETA(DisplayName="E-Stop")
};

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FAutomationPacket : public FNetworkPacket
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    EAutomationPacketType action;

    static FAutomationPacket Ping()
    {
        FAutomationPacket packet;
        packet.type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet.action = EAutomationPacketType::PING;
        return packet;
    };

    static FAutomationPacket Pong(FAutomationPacket src)
    {
        FAutomationPacket packet;
        packet.type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet.action = EAutomationPacketType::PONG;
        packet.dest = src.source;
        return packet;
    };
};

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FAutomationEventPacket : public FAutomationPacket
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    TMap<FName, double> properties;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    double duration;

    static FAutomationEventPacket Event(TMap<FName, double> properties, double duration)
    {
        FAutomationEventPacket packet;
        packet.type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet.action = EAutomationPacketType::EVENT;
        packet.properties = properties;
        packet.duration = duration;
        return packet;
    };
};

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FAutomationEStopPacket : public FAutomationPacket
{
    GENERATED_BODY()

public:
    // The global set of active e-stops
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    TSet<FName> sources;

    // The local e-stop ID if broadcasting local state
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    FName stopSource = FName("None");
    
    // e-stop active (either local or global depending on `stopSource`)
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    bool active;

    // Request for the global e-stop state
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    bool request = false;

    static FAutomationEStopPacket EStopActive(FName source)
    {
        FAutomationEStopPacket packet;
        packet.type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet.action = EAutomationPacketType::E_STOP;
        packet.stopSource = source;
        packet.active = true;
        return packet;
    };

    static FAutomationEStopPacket EStopReleased(FName source)
    {
        FAutomationEStopPacket packet;
        packet.type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet.action = EAutomationPacketType::E_STOP;
        packet.stopSource = source;
        packet.active = false;
        return packet;
    };

    static FAutomationEStopPacket EStopSet(TSet<FName> sources, bool active)
    {
        FAutomationEStopPacket packet;
        packet.type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet.action = EAutomationPacketType::E_STOP;
        packet.sources = sources;
        packet.active = active;
        return packet;
    };

    static FAutomationEStopPacket Request()
    {
        FAutomationEStopPacket packet;
        packet.request = true;
        return packet;
    };
};