// Copyright Peter Crall 2025

#include "Paradigme/ETCParadigmeProcessor.h"

AETCParadigmeProcessor::AETCParadigmeProcessor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    networkCard = CreateDefaultSubobject<UDMXNetworkCard>(TEXT("Network Card"));
}

AETCParadigmeProcessor::~AETCParadigmeProcessor()
{
    
}

void AETCParadigmeProcessor::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);
}

void AETCParadigmeProcessor::BeginPlay()
{
    Super::BeginPlay();

    if(config != nullptr)
    {
        for(const TPair<FName, FETCParadigmeRoom> &pair : config->rooms)
        {
            activeStateInternal(pair.Key, pair.Value, pair.Value.defaultState);
        }
    }
}

void AETCParadigmeProcessor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TArray<FName> doneStates;
    for(const TPair<FName, double> &pair : targetOutState)
    {
        if(processStateUpdate(pair.Key, DeltaTime, pair.Value))
            doneStates.Add(pair.Key);
    }
    for(const FName &circuit : doneStates)
    {
        targetOutState.Remove(circuit);
    }
}

bool AETCParadigmeProcessor::processStateUpdate(FName circuit, double deltaTime, double target)
{
    double cState = 0;
    if(double *p = outState.Find(circuit))
    {
        cState = *p;
    }
    double step = deltaTime * 0.3;
    double aDelta = abs(cState - target);
    if(aDelta < step)
    {
        outState.Add(circuit, target);
        return true;
    }
    if(step > aDelta)
        step = aDelta;
    step *= (cState < target) ? 1 : -1;
    cState += step;
    if(abs(cState - target) < 0.001)
    {
        outState.Add(circuit, target);
        return true;
    }
    outState.Add(circuit, cState);
    return false;
}

bool AETCParadigmeProcessor::requestState(FName room, int state)
{
    if(!validRoom(room)) // Unknown room
        return false;
    FETCParadigmeRoom roomConfig = config->rooms[room];
    if(roomConfig.states.Num() <= state || state < 0) // invalid state
        return false;
    
    bool showInhibit = showMode && (roomConfig.showState != -1 || roomConfig.showInhibit.Contains(state));
    if(showInhibit)
        return false;
    
    if(int* p = activeStates.Find(room))
    {
        if((*p) == state)
        {
            activeStateInternal(room, roomConfig, (state == 0) ? 1 : 0);
            return true;
        }
    }
    activeStateInternal(room, roomConfig, state);
    return true;
}

void AETCParadigmeProcessor::activateState(FName room, int state)
{
    if(!validRoom(room)) // Unknown room
        return;
    if(int* p = activeStates.Find(room))
    {
        if((*p) == state)
            return;
    }
    activeStates.Add(room, state);
    FETCParadigmeRoom roomConfig = config->rooms[room];
    if(roomConfig.states.Num() <= state || state < 0) // invalid state
        return;
    activeStateInternal(room, roomConfig, state);
}

void AETCParadigmeProcessor::activeStateInternal(FName room, FETCParadigmeRoom roomConfig, int state)
{
    if(roomConfig.states.Num() <= state || state < 0) // invalid state
        return;
    FETCParadigmeRoomState newState = roomConfig.states[state];
    for (const TPair<FName, double> &pair : newState.states)
    {
        targetOutState.Add(pair.Key, pair.Value);
    }
    onRoomStateUpdate.Broadcast(room, state);
}

int AETCParadigmeProcessor::getState(FName room)
{
    if(int* p = activeStates.Find(room))
    {
        return *p;
    }
    return 0;
}

bool AETCParadigmeProcessor::validRoom(FName room)
{
    if(config == nullptr)
        return false;
    return config->rooms.Contains(room);
}

bool AETCParadigmeProcessor::toggleShowMode()
{
    setShowMode(!showMode);
    return showMode;
}

void AETCParadigmeProcessor::setShowMode(bool active)
{
    showMode = active;
    if(!showMode || config == nullptr)
        return;
    
    for(const TPair<FName, FETCParadigmeRoom> &pair : config->rooms)
    {
        FName roomId = pair.Key;
        FETCParadigmeRoom room = pair.Value;
        if(room.showState != -1)
        {
            activeStateInternal(roomId, room, room.showState);
        }
        else if(room.showInhibit.Num() > 0)
        {
            if(int *p = activeStates.Find(roomId))
            {
                if(room.showInhibit.Contains(*p))
                {
                    activeStateInternal(roomId, room, 0);
                }
            }
        }
    }
}
bool AETCParadigmeProcessor::getShowMode()
{
    return showMode;
}

struct FOutUniverse
{
    TArray<int> data;
};

void AETCParadigmeProcessor::sendDMX()
{
    TMap<int, FOutUniverse> universes;
    if(config == nullptr)
        return;
    for(const TPair<FName, double> &pair : outState)
    {
        FName circuitId = pair.Key;
        FETCParadigmeCircuit *circuitConfig = config->circuits.Find(circuitId);
        if(!circuitConfig)
            continue;
        FOutUniverse universe;
        if(FOutUniverse *p = universes.Find(circuitConfig->universe))
        {
            universe = *p;
            universe.data.Init(0, 512);
        }
        universe.data[circuitConfig->address] = (int)(255.0 * pair.Value);
    }
    for(const TPair<int, FOutUniverse> &pair : universes)
    {
        networkCard->sendData(sourceName, dmxPriority, pair.Key, pair.Value.data);
    }
}