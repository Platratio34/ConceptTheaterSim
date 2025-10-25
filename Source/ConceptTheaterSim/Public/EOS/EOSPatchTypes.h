// Copyright Peter Crall 2025

#pragma once

#include "EOS/EOSShowfile.h"

const FName LIGHT_TYPE_UNKNOWN = FName(TEXT("Unknown"));
const FName LIGHT_TYPE_DIMMER = FName(TEXT("Dimmer"));
const FName LIGHT_TYPE_ETC_S4_LUSTR_P_DIRECT = FName(TEXT("ETC_S4_Lustr+_Direct"));
const FName LIGHT_TYPE_ETC_D60_DIRECT = FName(TEXT("ETC_D60_Direct"));
const FName LIGHT_TYPE_ETC_COLOR_SOURCE_CYC_DIRECT = FName(TEXT("ETC_ColorSource_CYC_Direct"));
const FName LIGHT_TYPE_MAVERICK_MK3_WASH_BASIC = FName(TEXT("MaverickMk3Wash_Basic"));
const FName LIGHT_TYPE_MAVERICK_MK3_PROFILE_54CH = FName(TEXT("MaverickMk3Profile_54ch"));

const FName PROPERTY_INTENSITY = FName(TEXT("Intensity"));
const FName PROPERTY_BACKGROUND_INTENSITY = FName(TEXT("BackgroundIntensity"));

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
const FName PROPERTY_CRI = FName(TEXT("CRI"));

const FName PROPERTY_COLOR_MIX = FName(TEXT("ColorMix"));
const FName PROPERTY_BACKGROUND_COLOR_MIX = FName(TEXT("BackgroundColorMix"));
const FName PROPERTY_COLOR_MIX_SPEED = FName(TEXT("ColorMixSpeed"));

const FName PROPERTY_STROBE = FName(TEXT("Strobe"));
const FName PROPERTY_FAN = FName(TEXT("Fan"));

const FName PROPERTY_PAN = FName(TEXT("Pan"));
const FName PROPERTY_TILT = FName(TEXT("Tilt"));
const FName PROPERTY_POSITION_MSPEED = FName(TEXT("PositionMSpeed"));

const FName PROPERTY_LED_EFFECT = FName(TEXT("LEDEffect"));

const FName PROPERTY_COLOR_SELECT = FName(TEXT("ColorSelect"));

const FName PROPERTY_GOBO_SELECT = FName(TEXT("GoboSelect"));
const FName PROPERTY_GOBO_INDEX_SPEED = FName(TEXT("GoboIndex/Speed"));
const FName PROPERTY_GOBO_SELECT_2 = FName(TEXT("GoboSelect2"));
const FName PROPERTY_GOBO_INDEX_SPEED_2 = FName(TEXT("GoboIndex/Speed2"));

const FName PROPERTY_ANIMATION_SELECT = FName(TEXT("AnimationSelect"));
const FName PROPERTY_ANIMATION_INDEX_SPEED = FName(TEXT("AnimationIndex/Speed"));

const FName PROPERTY_SHUTTER_1_POS = FName(TEXT("Shutter1Pos"));
const FName PROPERTY_SHUTTER_1_ROT = FName(TEXT("Shutter1Rot"));

const FName PROPERTY_SHUTTER_2_POS = FName(TEXT("Shutter2Pos"));
const FName PROPERTY_SHUTTER_2_ROT = FName(TEXT("Shutter2Rot"));

const FName PROPERTY_SHUTTER_3_POS = FName(TEXT("Shutter3Pos"));
const FName PROPERTY_SHUTTER_3_ROT = FName(TEXT("Shutter3Rot"));

const FName PROPERTY_SHUTTER_4_POS = FName(TEXT("Shutter4Pos"));
const FName PROPERTY_SHUTTER_4_ROT = FName(TEXT("Shutter4Rot"));

const FName PROPERTY_SHUTTER_1_A = FName(TEXT("Shutter1A"));
const FName PROPERTY_SHUTTER_1_B = FName(TEXT("Shutter1B"));

const FName PROPERTY_SHUTTER_2_A = FName(TEXT("Shutter2A"));
const FName PROPERTY_SHUTTER_2_B = FName(TEXT("Shutter2B"));

const FName PROPERTY_SHUTTER_3_A = FName(TEXT("Shutter3A"));
const FName PROPERTY_SHUTTER_3_B = FName(TEXT("Shutter3B"));

const FName PROPERTY_SHUTTER_4_A = FName(TEXT("Shutter4A"));
const FName PROPERTY_SHUTTER_4_B = FName(TEXT("Shutter4B"));

const FName PROPERTY_SHUTTER_FRAME_ROT = FName(TEXT("ShutterFrameRot"));

const FName PROPERTY_EDGE = FName(TEXT("Edge"));
const FName PROPERTY_EDGE_MODE = FName(TEXT("EdgeMode"));
const FName PROPERTY_ZOOM = FName(TEXT("Zoom"));
const FName PROPERTY_IRIS = FName(TEXT("Iris"));

const FName PROPERTY_BEAM_FX_SELECT = FName(TEXT("BeamFXSelect"));
const FName PROPERTY_BEAM_FX_INDEX_SPEED = FName(TEXT("BeamFXIndex/Speed"));
const FName PROPERTY_BEAM_FX_SELECT_2 = FName(TEXT("BeamFXSelect2"));
const FName PROPERTY_BEAM_FX_INDEX_SPEED_2 = FName(TEXT("BeamFXIndex/Speed2"));

const FName PROPERTY_DIFFUSION = FName(TEXT("Diffusion"));
const FName PROPERTY_DIFFUSION_2 = FName(TEXT("Diffusion2"));

const FName PROPERTY_POSITION_BLINK = FName(TEXT("PositionBlink"));

const FName PROPERTY_PIXEL_MASK = FName(TEXT("PixelMask"));
const FName PROPERTY_PIXEL_MASK_2 = FName(TEXT("PixelMask2"));
const FName PROPERTY_PIXEL_MASK_EFFECT_TIME = FName(TEXT("PixelMaskEffectTime"));
const FName PROPERTY_PIXEL_MASK_EFFECT_STEP_TIME = FName(TEXT("PixelMaskEffectStepTime"));


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
        if(type == LIGHT_TYPE_ETC_COLOR_SOURCE_CYC_DIRECT)
        {
            FEOSPatch patch(type, 10);
            patch.properties.Add(PROPERTY_INTENSITY);
            patch.properties.Add(PROPERTY_RED);
            patch.properties.Add(PROPERTY_GREEN);
            patch.properties.Add(PROPERTY_BLUE);
            patch.properties.Add(PROPERTY_INDIGO);
            patch.properties.Add(PROPERTY_LIME);
            patch.properties.Add(PROPERTY_STROBE);
            return patch;
        }
        if(type == LIGHT_TYPE_MAVERICK_MK3_PROFILE_54CH)
        {
            FEOSPatch patch(type, 54);
            patch.properties.Add(PROPERTY_PAN);
            patch.properties.Add(PROPERTY_TILT);
            patch.properties.Add(PROPERTY_POSITION_MSPEED);
            patch.properties.Add(PROPERTY_INTENSITY);
            patch.properties.Add(PROPERTY_STROBE);
            patch.properties.Add(PROPERTY_CYAN);
            patch.properties.Add(PROPERTY_MAGENTA);
            patch.properties.Add(PROPERTY_YELLOW);
            patch.properties.Add(PROPERTY_CT);
            patch.properties.Add(PROPERTY_COLOR_SELECT);
            patch.properties.Add(PROPERTY_GOBO_SELECT);
            patch.properties.Add(PROPERTY_GOBO_INDEX_SPEED);
            patch.properties.Add(PROPERTY_GOBO_SELECT_2);
            patch.properties.Add(PROPERTY_ANIMATION_SELECT);
            patch.properties.Add(PROPERTY_ANIMATION_INDEX_SPEED);
            patch.properties.Add(PROPERTY_SHUTTER_3_B);
            patch.properties.Add(PROPERTY_SHUTTER_3_A);
            patch.properties.Add(PROPERTY_SHUTTER_2_B);
            patch.properties.Add(PROPERTY_SHUTTER_2_A);
            patch.properties.Add(PROPERTY_SHUTTER_1_B);
            patch.properties.Add(PROPERTY_SHUTTER_1_A);
            patch.properties.Add(PROPERTY_SHUTTER_4_B);
            patch.properties.Add(PROPERTY_SHUTTER_4_A);
            patch.properties.Add(PROPERTY_SHUTTER_FRAME_ROT);
            patch.properties.Add(PROPERTY_EDGE);
            patch.properties.Add(PROPERTY_EDGE_MODE);
            patch.properties.Add(PROPERTY_ZOOM);
            patch.properties.Add(PROPERTY_BEAM_FX_SELECT);
            patch.properties.Add(PROPERTY_BEAM_FX_INDEX_SPEED);
            patch.properties.Add(PROPERTY_BEAM_FX_SELECT_2);
            patch.properties.Add(PROPERTY_BEAM_FX_INDEX_SPEED_2);
            patch.properties.Add(PROPERTY_IRIS);
            patch.properties.Add(PROPERTY_DIFFUSION);
            patch.properties.Add(PROPERTY_DIFFUSION_2);
            patch.properties.Add(PROPERTY_CRI);
            patch.properties.Add(PROPERTY_COLOR_MIX);
            patch.properties.Add(PROPERTY_POSITION_BLINK);
            return patch;
        }
        if(type == LIGHT_TYPE_MAVERICK_MK3_WASH_BASIC)
        {
            FEOSPatch patch(type, 21);
            patch.properties.Add(PROPERTY_PAN);
            patch.properties.Add(PROPERTY_TILT);
            patch.properties.Add(PROPERTY_POSITION_MSPEED);
            patch.properties.Add(PROPERTY_CT);
            patch.properties.Add(PROPERTY_COLOR_MIX);
            patch.properties.Add(PROPERTY_PIXEL_MASK_2);
            patch.properties.Add(PROPERTY_PIXEL_MASK);
            patch.properties.Add(PROPERTY_PIXEL_MASK_EFFECT_TIME);
            patch.properties.Add(PROPERTY_PIXEL_MASK_EFFECT_STEP_TIME);
            patch.properties.Add(PROPERTY_BACKGROUND_COLOR_MIX);
            patch.properties.Add(PROPERTY_BACKGROUND_INTENSITY);
            patch.properties.Add(PROPERTY_INTENSITY);
            patch.properties.Add(PROPERTY_STROBE);
            patch.properties.Add(PROPERTY_ZOOM);
            patch.properties.Add(PROPERTY_POSITION_BLINK);
            patch.properties.Add(PROPERTY_RED);
            patch.properties.Add(PROPERTY_GREEN);
            patch.properties.Add(PROPERTY_BLUE);
            patch.properties.Add(PROPERTY_WHITE);
            return patch;
        }
        UE_LOG(LogTemp, Warning, TEXT("Unknown patch type: %s"), *(type.ToString()));
        FEOSPatch def(type, 1);
        return def;
    }
};