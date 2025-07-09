// Fill out your copyright notice in the Description page of Project Settings.


#include "DMXCache.h"


bool UDMXCache::updateSource(FName name, int priority, int universe, TArray<int> data) {
    UDMXNetSource *source;
    const FName &nPtn = name;
    if(sources.Contains(name)) {
        source = *sources.Find(name);
    } else {
        sources.Add(name, source);
    }
    if(!source->set(universe, priority, data)) {
        return false;
    }
    cache.Remove(universe);
    return true; // test
}

TArray<int> UDMXCache::getData(const int universe) {
    if(cache.Contains(universe)) {
        return cache[universe];
    }
    int *arr = new int[512];
    int *prio = new int[512];
    for(auto& pair : sources) {
        UDMXNetSource *source = pair.Value;
        UDMXNetSource::DMXNetSourceUniverse u = source->getUniverse(universe);
        for (int i = 0; i < 512; i++) {
            if(u.priority < prio[i])
                continue;
            
            if(u.priority > prio[i] || u.data[i] > arr[i]) {
                prio[i] = u.priority;
                arr[i] = u.data[i];
            }
        }
    }
    TArray<int> out;
    for (int i = 0; i < 512; i++) {
        out.Add(arr[i]);
    }
    cache.Add(universe, out);
    delete[] prio;
    delete[] arr;
    return out;
}