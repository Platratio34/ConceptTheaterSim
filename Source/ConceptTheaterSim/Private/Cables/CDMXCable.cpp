// Copyright Peter Crall 2025.

#include "Cables/CDMXCable.h"
#include "Cables/CDMXCableConnector.h"

ACDMXCable::ACDMXCable()
{
    
}

bool ACDMXCable::canConnectTo(UCCableConnector *connector, bool start)
{
    return Cast<UCDMXCableConnector>(connector) != nullptr;
}