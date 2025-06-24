// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrayUtils.h"

TArray<FName> UArrayUtils::sortNames(TArray<FName> input) {
    TArray<FName> out;
    int length = input.Num();
    for (int i = 0; i < length; i++) {
        out[i] = input[i];
    }
    for (int i = 0; i < length - 1; i++) {
        for (int j = i + 1; j < length; j++) {
            int v = out[i].Compare(out[j]);
            if(v > 0) {
                FName temp = out[i];
                out[i] = out[j];
                out[j] = temp;
            }
        }
    }

    return out;
}