// Copyright Peter Crall 2025

#pragma once

#include "EOS/EOSShowfile.h"

const FName LIGHT_TYPE_UNKNOWN = FName(TEXT("Unknown"));
const FName LIGHT_TYPE_DIMMER = FName(TEXT("Dimmer"));
const FName LIGHT_TYPE_ETC_S4_LUSTR_P_DIRECT = FName(TEXT("ETC_S4_Lustr+_Direct"));
const FName LIGHT_TYPE_ETC_D60_DIRECT = FName(TEXT("ETC_D60_Direct"));

const FName PROPERTY_INTENSITY = FName(TEXT("Intensity"));

const FName PROPERTY_RED = FName(TEXT("Red"));
const FName PROPERTY_GREEN = FName(TEXT("Green"));
const FName PROPERTY_BLUE = FName(TEXT("Blue"));
const FName PROPERTY_WHITE = FName(TEXT("White"));

const FName PROPERTY_CYAN = FName(TEXT("Cyan"));
const FName PROPERTY_MAGENTA = FName(TEXT("Magenta"));
const FName PROPERTY_YELLOW = FName(TEXT("Yellow"));

const FName PROPERTY_AMBER = FName(TEXT("Amber"));
const FName PROPERTY_LIME = FName(TEXT("Lime"));
const FName PROPERTY_INDIGO = FName(TEXT("Indigo"));

const FName PROPERTY_HUE = FName(TEXT("Hue"));
const FName PROPERTY_SAT = FName(TEXT("Saturation"));
const FName PROPERTY_CT = FName(TEXT("ColorTemp"));

const FName PROPERTY_STROBE = FName(TEXT("Strobe"));
const FName PROPERTY_FAN = FName(TEXT("Fan"));

class CONCEPTTHEATERSIM_API EOSPatchTypes
{
public:
    static FEOSPatch create(FName type)
    {
        if(type == LIGHT_TYPE_DIMMER)
        {
            FEOSPatch patch(type, 1);
            patch.properties.Add(PROPERTY_INTENSITY);
            return patch;
        }
        if(type == LIGHT_TYPE_ETC_S4_LUSTR_P_DIRECT || type == LIGHT_TYPE_ETC_D60_DIRECT)
        {
            FEOSPatch patch(type, 10);
            patch.properties.Add(PROPERTY_INTENSITY);
            patch.properties.Add(PROPERTY_RED);
            patch.properties.Add(PROPERTY_WHITE);
            patch.properties.Add(PROPERTY_AMBER);
            patch.properties.Add(PROPERTY_GREEN);
            patch.properties.Add(PROPERTY_CYAN);
            patch.properties.Add(PROPERTY_BLUE);
            patch.properties.Add(PROPERTY_INDIGO);
            patch.properties.Add(PROPERTY_STROBE);
            patch.properties.Add(PROPERTY_FAN);
            return patch;
        }
        FEOSPatch def(type, 1);
        return def;
    }
};