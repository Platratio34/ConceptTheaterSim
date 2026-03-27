// Copyright Peter Crall 2025.


#include "TimeCodeSourceC.h"

ATimeCodeSourceC::ATimeCodeSourceC() {

}

void ATimeCodeSourceC::BeginPlay() {
    Super::BeginPlay();

    if(midiEnabled) {
        GetWorld()->GetTimerManager().SetTimer(timer, this, &ATimeCodeSourceC::TimerUpdate, 1.0f / ((float)frameRate*4), true);
    } else {
        GetWorld()->GetTimerManager().SetTimer(timer, this, &ATimeCodeSourceC::TimerUpdate, 1.0f / ((float)frameRate), true);
    }
}

void ATimeCodeSourceC::TimerUpdate() {
    if(!running)
        return;
    if(midiEnabled) {
        qf++;
        if(qf >= 7)
            qf = 0;

        int s = frames / 30;
        int m = s / 60;
        int h = m / 60;
        if(midiOutput != nullptr)
            midiOutput->sendQuarterFrame(qf, MTC_30FPS, h, m, s, frames % 30);
        
        if(!(qf == 0 || qf == 4)) {
            return;
        }
    }
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
    qf = 0;
    running = true;
    onTimeChangeEvent.Broadcast(frames, GetSeconds());
    sendTimePacket();
    if(midiOutput != nullptr) {
        int s = frames / 30;
        int m = s / 60;
        int h = m / 60;
        if(midiOutput != nullptr)
            midiOutput->sendFullFrame(MTC_30FPS, h, m, s, frames % 30);
    }
}

void ATimeCodeSourceC::Stop() {
    if(!running)
        return;
    running = false;
    onTimeStopEvent.Broadcast();
    sendTimePacket();
    if(midiOutput != nullptr) {
        int s = frames / 30;
        int m = s / 60;
        int h = m / 60;
        if(midiOutput != nullptr)
            midiOutput->sendFullFrame(MTC_30FPS, h, m, s, frames % 30);
    }
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