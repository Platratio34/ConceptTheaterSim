// Copyright Peter Crall 2025


#include "Core/TheaterSimGameMode.h"

ATheaterSimGameMode::ATheaterSimGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
    // PrimaryActorTick.bTickEvenWhenPause = true;
}

ATheaterSimGameMode::~ATheaterSimGameMode()
{

}

float ATheaterSimGameMode::getAverageTPS()
{
    return tps;
}

void ATheaterSimGameMode::BeginPlay()
{
    Super::BeginPlay();
    tpsArr.Init(0, 60);
}

void ATheaterSimGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    float cTps = 1 / DeltaTime;
    tpsArr[tpsPtr++] = cTps;
    if(tpsPtr >= tpsArr.Num())
        tpsPtr = 0;
    tps = 0;
    for(float t : tpsArr)
        tps += t;
    tps /= tpsArr.Num();
}