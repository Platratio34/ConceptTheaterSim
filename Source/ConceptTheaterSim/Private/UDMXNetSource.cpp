// Fill out your copyright notice in the Description page of Project Settings.


#include "UDMXNetSource.h"

bool UDMXNetSource::set(int universe, int prio, TArray<int> data) {
    priority.Add(universe, prio);
    int* d2 = new int[512];
    bool had = universes.Contains(universe);
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
    universes.Add(universe, d2);
    return changed;
}

UDMXNetSource::DMXNetSourceUniverse UDMXNetSource::getUniverse(int universe) {
    DMXNetSourceUniverse out;
    if(!universes.Contains(universe)) {
        return out;
    }
    out.data = universes[universe];
    out.priority = priority[universe];
    out.valid = true;
    return out;
}