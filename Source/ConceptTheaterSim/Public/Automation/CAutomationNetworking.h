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
    E_STOP UMETA(DisplayName="E-Stop"),
    PROCESSOR_QUERY UMETA(DisplayName="Processor Query"),
    REGISTER UMETA(DisplayName="Device Register")
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UAutomationPacket : public UNetworkPacket
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    EAutomationPacketType action;

    static UAutomationPacket* Ping()
    {
        UAutomationPacket* packet = NewObject<UAutomationPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->action = EAutomationPacketType::PING;
        return packet;
    };

    static UAutomationPacket* Pong(UAutomationPacket* src)
    {
        UAutomationPacket* packet = NewObject<UAutomationPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->action = EAutomationPacketType::PONG;
        packet->dest = src->source;
        return packet;
    };
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UAutomationEventPacket : public UAutomationPacket
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    TMap<FName, double> properties;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    double duration;

    static UAutomationEventPacket* Event(TMap<FName, double> properties, double duration)
    {
        UAutomationEventPacket* packet = NewObject<UAutomationEventPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->action = EAutomationPacketType::EVENT;
        packet->properties = properties;
        packet->duration = duration;
        return packet;
    };
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UAutomationEStopPacket : public UAutomationPacket
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

    static UAutomationEStopPacket* EStopActive(FName source)
    {
        UAutomationEStopPacket* packet = NewObject<UAutomationEStopPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->action = EAutomationPacketType::E_STOP;
        packet->stopSource = source;
        packet->active = true;
        return packet;
    };

    static UAutomationEStopPacket* EStopReleased(FName source)
    {
        UAutomationEStopPacket* packet = NewObject<UAutomationEStopPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->action = EAutomationPacketType::E_STOP;
        packet->stopSource = source;
        packet->active = false;
        return packet;
    };

    static UAutomationEStopPacket* EStopSet(TSet<FName> sources, bool active)
    {
        UAutomationEStopPacket* packet = NewObject<UAutomationEStopPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->action = EAutomationPacketType::E_STOP;
        packet->sources = sources;
        packet->active = active;
        return packet;
    };

    static UAutomationEStopPacket* Request()
    {
        UAutomationEStopPacket* packet = NewObject<UAutomationEStopPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->action = EAutomationPacketType::E_STOP;
        packet->request = true;
        return packet;
    };
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UAutomationProcessorQueryPacket : public UAutomationPacket
{
    GENERATED_BODY()

public:
    // If this is request for processor. Should can a broadcast message
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    bool request;

    static UAutomationProcessorQueryPacket* Request()
    {
        UAutomationProcessorQueryPacket* packet = NewObject<UAutomationProcessorQueryPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->action = EAutomationPacketType::PROCESSOR_QUERY;
        packet->request = true;
        return packet;
    };

    // Map of devices the processor
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    TMap<FName, int> devices;

    static UAutomationProcessorQueryPacket* Response(UAutomationPacket* req, TMap<FName, int> devices)
    {
        UAutomationProcessorQueryPacket* packet = NewObject<UAutomationProcessorQueryPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->dest = req->source;
        packet->action = EAutomationPacketType::PROCESSOR_QUERY;
        packet->request = false;
        packet->devices = devices;
        return packet;
    };
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UAutomationRegistrationPacket : public UAutomationPacket
{
    GENERATED_BODY()

public:
    // If the is a registration or un-registration message
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    bool isRegister = true;

    // The ID of the devices being registered/un-registered
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    FName deviceID;

    // The set of device property IDs (registration only)
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Automation")
    TSet<FName> properties;

    static UAutomationRegistrationPacket* Register(FName deviceID, TSet<FName> properties)
    {
        UAutomationRegistrationPacket* packet = NewObject<UAutomationRegistrationPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->action = EAutomationPacketType::REGISTER;
        packet->isRegister = true;
        packet->deviceID = deviceID;
        packet->properties = properties;
        return packet;
    };

    static UAutomationRegistrationPacket* Unregister(FName deviceID)
    {
        UAutomationRegistrationPacket* packet = NewObject<UAutomationRegistrationPacket>();
        packet->type = AUTOMATION_NETWORK_PACKET_TYPE;
        packet->action = EAutomationPacketType::REGISTER;
        packet->isRegister = false;
        packet->deviceID = deviceID;
        return packet;
    };
};