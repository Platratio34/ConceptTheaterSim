// Fill out your copyright notice in the Description page of Project Settings.


#include "DMXCache.h"


bool UDMXCache::updateSource(FName name, int priority, int universe, TArray<int> data) {
    // UE_LOG(LogTemp, Display, TEXT("Updating DMX Net Source: %s"), *(name.ToString()));
    UDMXNetSource *source = nullptr;
    const FName &nPtn = name;
    if(sources.Contains(name)) {
        source = *sources.Find(name);
    } else {
        source = NewObject<UDMXNetSource>();
        sources.Add(name, source);
        source->name = name;
        // UE_LOG(LogTemp, Display, TEXT("- Creating new source"));
    }
    if(!source->set(universe, priority, data)) {
        return false;
    }
    cache.Remove(universe);
    return true;
}

TArray<int> UDMXCache::getData(const int universe) {
    if(cache.Contains(universe)) {
        return cache[universe].data;
    }
    // UE_LOG(LogTemp, Display, TEXT("Geting DMX data: U%d"), universe);
    int prio[512] = {};
    for(int i = 0; i < 512; i++) {
        prio[i] = 0;
    }
    TArray<int> outData;
    outData.Init(0, 512);
    for(auto& pair : sources) {
        UDMXNetSource *source = pair.Value;
        UDMXNetSource::DMXNetSourceUniverse sourceUniverse = source->getUniverse(universe);
        if(!sourceUniverse.valid) // this source doesn't have this universe
            continue;
        // UE_LOG(LogTemp, Display, TEXT("- Source: %s"), *(source->name.ToString()));
        for (int i = 0; i < 512; i++) {
            if(sourceUniverse.priority < prio[i])
                continue;
            
            if(sourceUniverse.priority > prio[i] || sourceUniverse.data[i] > outData[i]) {
                prio[i] = sourceUniverse.priority;
                outData[i] = sourceUniverse.data[i];
            }
        }
    }
    FUniverse uni(outData);
    cache.Add(universe, uni);
    return outData;
}