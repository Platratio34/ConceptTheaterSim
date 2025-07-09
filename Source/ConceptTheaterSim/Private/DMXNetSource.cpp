// Fill out your copyright notice in the Description page of Project Settings.


#include "DMXNetSource.h"

UDMXNetSource::UDMXNetSource() {
    for (int i = 0; i < 16; i++) {
        priority[i] = 127;
        hasUniverse[i] = false;
    }
}
UDMXNetSource::~UDMXNetSource() {
    for (int i = 0; i < 16; i++) {
        if(hasUniverse[i]) {
            delete[] universes[i];
        }
    }
}

bool UDMXNetSource::set(int universe, int prio, TArray<int> data) {
    if(universe < 0 || universe >= 16) {
        return false;
    }
    priority[universe] = prio;
    int* d2 = new int[512];
    bool had = hasUniverse[universe];
    int *last;
    if(had) {
        last = universes[universe];
    } else {
        last = new int[512];
    }
    bool changed = !had;
    for (int i = 0; i < data.Num(); i++) {
        d2[i] = data[i];
        if(had) {
            changed |= (last[i] != d2[i]);
        }
    }
    if(changed) {
        universes[universe] = d2;
        hasUniverse[universe] = true;
        delete[] last;
    } else {
        delete[] d2;
    }
    return changed;
}

UDMXNetSource::DMXNetSourceUniverse UDMXNetSource::getUniverse(int universe) {
    DMXNetSourceUniverse out;
    if(universe < 0 || universe >= 16) {
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