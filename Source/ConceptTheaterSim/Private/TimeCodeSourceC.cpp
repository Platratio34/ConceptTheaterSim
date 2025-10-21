// Copyright Peter Crall 2025.


#include "TimeCodeSourceC.h"

ATimeCodeSourceC::ATimeCodeSourceC() {

}

void ATimeCodeSourceC::BeginPlay() {
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(timer, this, &ATimeCodeSourceC::TimerUpdate, 1.0f / ((float)frameRate), true);
}

void ATimeCodeSourceC::TimerUpdate() {
    if(!running)
        return;
    frames++;
    onTimeChangeEvent.Broadcast(frames, GetSeconds());
    sendTimePacket();
}

void ATimeCodeSourceC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    
    GetWorld()->GetTimerManager().ClearTimer(timer);
}

void ATimeCodeSourceC::Start(int startFrames) {
    if(running)
        return;
    frames = startFrames;
    running = true;
    onTimeChangeEvent.Broadcast(frames, GetSeconds());
    sendTimePacket();
}

void ATimeCodeSourceC::Stop() {
    if(!running)
        return;
    running = false;
    onTimeStopEvent.Broadcast();
    sendTimePacket();
}

bool ATimeCodeSourceC::IsRunning() {
    return running;
}

float ATimeCodeSourceC::GetSeconds() {
    return ((float)frames) / ((float)frameRate);
}

int ATimeCodeSourceC::GetFrames() {
    return frames;
}

void ATimeCodeSourceC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimeCodeSourceC::sendTimePacket()
{
    if(networkCard == nullptr)
        return;
    networkCard->send(UTimecodeNetworkPacket::createTCPacket(timeSourceName, frames, GetSeconds(), running));
}