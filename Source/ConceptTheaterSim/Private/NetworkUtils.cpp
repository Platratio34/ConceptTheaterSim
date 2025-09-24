// Copyright Peter Crall 2025


#include "NetworkUtils.h"

int UNetworkUtils::IPToNumber(uint8 o1, uint8 o2, uint8 o3, uint8 o4) {
    return (o1 << 24) | (o2 << 16) | (o3 << 8) | o4;
}