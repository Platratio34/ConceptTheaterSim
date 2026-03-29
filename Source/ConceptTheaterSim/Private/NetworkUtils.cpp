// Copyright Peter Crall 2025


#include "NetworkUtils.h"

int UNetworkUtils::IPToNumber(uint8 o1, uint8 o2, uint8 o3, uint8 o4) {
    return (o1 << 24) | (o2 << 16) | (o3 << 8) | o4;
}
FString UNetworkUtils::NumberToIPV4(int address) {
    FString str = FString::Printf(TEXT("%i.%i.%i.%i"), (address >> 24) & 0xff, (address >> 16) & 0xff, (address >> 8) & 0xff, address & 0xff);
    return str;
}