// Copyright Peter Crall 2025.

#include "Cables/CPowerCable.h"
#include "Cables/CPowerCableConnector.h"

ACPowerCable::ACPowerCable()
{
    
}

bool ACPowerCable::canConnectTo(UCCableConnector *connector, bool start)
{
    UCPowerCableConnector *pCon = Cast<UCPowerCableConnector>(connector);
    if(pCon == nullptr)
        return false;
    return pCon->connectorType == (start ? startType : endType);
}