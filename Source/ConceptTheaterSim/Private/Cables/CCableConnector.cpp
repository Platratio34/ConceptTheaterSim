// Copyright Peter Crall 2025.

#include "Cables/CCableConnector.h"


UCCableConnector::UCCableConnector()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void UCCableConnector::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UCCableConnector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCCableConnector::onConnect(ACCable* cable_)
{
    cable = cable_;
}
void UCCableConnector::onDisconnect(ACCable* cable_)
{
    cable = nullptr;
}