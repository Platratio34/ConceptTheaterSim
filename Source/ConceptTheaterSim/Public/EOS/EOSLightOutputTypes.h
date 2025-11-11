// Copyright Peter Crall 2025

#pragma once

#include "EOS/EOSPatchTypes.h"
#include "EOS/EOSShowfile.h"

class CONCEPTTHEATERSIM_API EOSLightOutputType
{
public:
    virtual void output(UEOSChannelView *channel, TArray<int>& dmx, int start) {}
    virtual void input(TArray<int> &dmx, UEOSPropertySet *parameters, int start) {}

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

    int toByteRanged(double v, double min, double max)
    {
        return (int)((v - min) / (max - min) * 255);
    }
    int toByteCoarseRanged(double v, double min, double max)
    {
        return (int)((v - min) / (max - min) * 65535) >> 8;
    }
    int toByteFineRanged(double v, double min, double max)
    {
        return (int)((v - min) / (max - min) * 65535) & 0xff;
    }

    double fromByte(int b)
    {
        return ((double)b) / 255.0;
    }
    double fromByte16b(int bCoarse, int bFine)
    {
        return ((double)((bCoarse * 256) + bFine)) / 65535.0;
    }

    double fromByteRanged(int b, double min, double max)
    {
        return (((double)b) / 255.0) * (max - min) + min;
    }
    double fromByte16bRanged(int bCoarse, int bFine, double min, double max)
    {
        return ((((bCoarse * 256.0) + (double)bFine)) / 65535.0) * (max - min) + min;
    }
};