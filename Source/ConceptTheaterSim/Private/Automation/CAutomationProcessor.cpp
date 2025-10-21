// Copyright Peter Crall 2025.

#include "Automation/CAutomationProcessor.h"
#include "Networking/NetworkPacketTypes.h"
#include "Json.h"

DEFINE_LOG_CATEGORY(AutomationLog);

// Sets default values
ACAutomationProcessor::ACAutomationProcessor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    networkCard = CreateDefaultSubobject<UNetworkCard>("Network Card");
}

// Called when the game starts or when spawned
void ACAutomationProcessor::BeginPlay()
{
	Super::BeginPlay();
    networkCard->onNetworkPacket.AddDynamic(this, &ACAutomationProcessor::onNetworkPacket);
}

// Called every frame
void ACAutomationProcessor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
}

void ACAutomationProcessor::onNetworkPacket(UNetworkPacket *packet)
{
    if(packet->type == AUTOMATION_NETWORK_PACKET_TYPE)
    {
        UAutomationPacket *autoPacket = Cast<UAutomationPacket>(packet);
        switch (autoPacket->action)
        {
        case EAutomationPacketType::PING: // just respond to it immediately
            networkCard->send(UAutomationPacket::Pong(autoPacket));
            break;
        case EAutomationPacketType::PONG:
            // nothing here at the moment
            break;

        case EAutomationPacketType::EVENT:
            // nothing here at the moment
            // And why would *we* get this
            break;

        case EAutomationPacketType::QUERY:
            // nothing here at the moment
            // And why would *we* get this
            break;

        case EAutomationPacketType::QUERY_RESPONSE:
            // Keep track of the responses here?
            break;

        case EAutomationPacketType::STATUS:
            // Keep track of the responses here?
            break;

        case EAutomationPacketType::E_STOP:
            onEStopPacket((UAutomationEStopPacket*)autoPacket);
            break;
        
        default:
            break;
        }
    }
    else if(packet->type == TIMECODE_NETWORK_PACKET_TYPE)
    {
        UTimecodeNetworkPacket *timePacket = Cast<UTimecodeNetworkPacket>(packet);
        onTimeUpdate(timePacket->frames, timePacket->seconds, timePacket->running);
    }
}

void ACAutomationProcessor::onEStopPacket(UAutomationEStopPacket *packet)
{
    if(packet->request)
    {
        UAutomationEStopPacket *rsp = UAutomationEStopPacket::EStopSet(eStopSources, eStopActive);
        rsp->dest = packet->source;
        networkCard->send(rsp);
        return;
    }
    if(packet->stopSource == FName("None"))
    {
        return;
    }
    
    if(packet->active)
    {
        eStopSources.Add(packet->stopSource);
    }
    else
    {
        eStopSources.Remove(packet->stopSource);
    }
    bool newActive = eStopSources.IsEmpty();
    // If wasn't active and isn't being activated, we don't need to broadcast the 'new' state
    // On the other hand, even if we were active, we should re-broadcast the set in case it has changed
    if(!(newActive || eStopActive))
        return;
    eStopActive = newActive;
    // We are doing a local broadcast here so that non-automation devices can find out about the update
    networkCard->sendBroadcast(UAutomationEStopPacket::EStopSet(eStopSources, eStopActive));
}

void ACAutomationProcessor::onTimeUpdate(int frames, float seconds, bool running)
{
    
}

void ACAutomationProcessor::sendEvent(FName device, TMap<FName, double> properties, double duration)
{
    int *p = deviceIPsByName.Find(device);
    if(!p) // we don't know it's IP
    {
        UE_LOG(AutomationLog, Warning, TEXT("Automation event for device %s, but no IP was known"), *(device.ToString()));
        return;
    }
    UAutomationEventPacket *outPacket = UAutomationEventPacket::Event(properties, duration);
    outPacket->dest = *p;
    networkCard->send(outPacket);
}