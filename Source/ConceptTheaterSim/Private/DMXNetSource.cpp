// Fill out your copyright notice in the Description page of Project Settings.


#include "DMXNetSource.h"

UDMXNetSource::UDMXNetSource() {
    for (int i = 0; i < DMX_NET_SOURCE_MAX_UNIVERSE; i++) {
        priority[i] = 127;
        hasUniverse[i] = false;
        universes[i] = nullptr;
    }
}
UDMXNetSource::~UDMXNetSource() {
    for (int i = 0; i < DMX_NET_SOURCE_MAX_UNIVERSE; i++) {
        if(hasUniverse[i]) {
            delete[] universes[i];
            universes[i] = nullptr;
            hasUniverse[i] = false;
        }
    }
}

bool UDMXNetSource::set(int universe, int prio, TArray<int> data) {
    if(universe < 0 || universe >= DMX_NET_SOURCE_MAX_UNIVERSE) {
        return false;
    }
    priority[universe] = prio;
    bool changed = !hasUniverse[universe];
    hasUniverse[universe] = true;
    if(universes[universe] == nullptr) {
        universes[universe] = new uint8[512];
    }
    if(data.Num() != 512) {
        UE_LOG(LogTemp, Error, TEXT("DMX data incorrect size: Expected 512 elements, found %d"), data.Num());
        return false;
    }
    for (int i = 0; i < 512; i++) {
        uint8 l = universes[universe][i];
        uint8 n = data[i];
        universes[universe][i] = n;
        if(!changed) {
            changed |= (n != l);
        }
    }
    return changed;
}

UDMXNetSource::DMXNetSourceUniverse UDMXNetSource::getUniverse(int universe) {
    DMXNetSourceUniverse out;
    if(universe < 0 || universe >= DMX_NET_SOURCE_MAX_UNIVERSE) {
        return out;
    }
    if(!hasUniverse[universe]) {
        return out;
    }
    out.data = universes[universe];
    out.priority = priority[universe];
    out.valid = true;
    return out;
}