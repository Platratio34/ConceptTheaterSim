// Copyright Peter Crall 2025.

#include "Cables/CNetworkCable.h"
#include "Cables/CNetworkCableConnector.h"

ACNetworkCable::ACNetworkCable()
{
    
}

bool ACNetworkCable::canConnectTo(UCCableConnector *connector, bool start)
{
    return Cast<UCNetworkCableConnector>(connector) != nullptr && (!start == connector->input);
}