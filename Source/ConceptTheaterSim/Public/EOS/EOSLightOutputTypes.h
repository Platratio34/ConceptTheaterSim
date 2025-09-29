// Copyright Peter Crall 2025

#pragma once

#include "EOS/EOSPatchTypes.h"

class CONCEPTTHEATERSIM_API EOSLightOutputType
{
public:
    virtual TArray<int> output(TMap<FName, double> parameters) { TArray<int> arr;
        return arr;
    }
    virtual void input(TArray<int> dmx, TMap<FName, double> params) {}

    static EOSLightOutputType *getType(FName type);

    int toByte(double v)
    {
        return (int)(255 * v);
    }
    int toByteCoarse(double v)
    {
        return (int)(65535 * v) >> 8;
    }
    int toByteFine(double v)
    {
        return (int)(65535 * v) & 0xff;
    }

    double fromByte(int b)
    {
        return 255.0 / ((double)b);
    }
    double fromByte16b(int b1, int b2)
    {
        return ((double)((b1 << 8) | b2)) / 65535;
    }
};