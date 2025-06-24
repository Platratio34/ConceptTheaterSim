// Fill out your copyright notice in the Description page of Project Settings.


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
    onTimeChangeEvent.Broadcast(frames, this->GetSeconds());
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
    onTimeChangeEvent.Broadcast(frames, this->GetSeconds());
}

void ATimeCodeSourceC::Stop() {
    if(!running)
        return;
    running = false;
    onTimeStopEvent.Broadcast();
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