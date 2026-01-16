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
const FName LIGHT_TYPE_GENERIC_LIGHT_PANEL = FName(TEXT("GenericLightPanel"));

const TSet<FName> LIGHT_TYPES = {
    LIGHT_TYPE_DIMMER,
    LIGHT_TYPE_ETC_S4_LUSTR_P_DIRECT,
    LIGHT_TYPE_ETC_D60_DIRECT,
    LIGHT_TYPE_ETC_COLOR_SOURCE_CYC_DIRECT,
    LIGHT_TYPE_MAVERICK_MK3_WASH_BASIC,
    LIGHT_TYPE_MAVERICK_MK3_PROFILE_54CH,
    LIGHT_TYPE_GENERIC_LIGHT_PANEL};

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

const FName PROPERTY_COLOR_SELECT = FName(TEXT("ColorSelect"));

const FName PROPERTY_STROBE = FName(TEXT("Strobe"));
const FName PROPERTY_FAN = FName(TEXT("Fan"));

const FName PROPERTY_PAN = FName(TEXT("Pan"));
const FName PROPERTY_TILT = FName(TEXT("Tilt"));
const FName PROPERTY_POSITION_MSPEED = FName(TEXT("PositionMSpeed"));

const FName PROPERTY_LED_EFFECT = FName(TEXT("LEDEffect"));

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

const TSet<FName> PROPERTIES_INTENSITY = {
    PROPERTY_INTENSITY,
    PROPERTY_BACKGROUND_INTENSITY,
    PROPERTY_PIXEL_MASK,
    PROPERTY_PIXEL_MASK_2,
    FName("Cell1"),
    FName("Cell2"),
    FName("Cell3"),
    FName("Cell4"),
    FName("Cell5"),
    FName("Cell6"),
    FName("Cell7"),
    FName("Cell8"),
    FName("Cell9"),
    FName("Cell10"),
    FName("Cell11"),
    FName("Cell12"),
    FName("Cell13"),
    FName("Cell14"),
    FName("Cell15"),
    FName("Cell16"),
    FName("Cell17"),
    FName("Cell18"),
    FName("Cell19"),
    FName("Cell20"),
    FName("Cell21"),
    FName("Cell22"),
    FName("Cell23"),
    FName("Cell24"),
    FName("Cell25"),
    FName("Cell26"),
    FName("Cell27"),
    FName("Cell28"),
    FName("Cell29"),
    FName("Cell30"),
    PROPERTY_STROBE};

const TSet<FName> PROPERTIES_COLOR = {
    PROPERTY_RED,
    PROPERTY_GREEN,
    PROPERTY_BLUE,
    PROPERTY_WHITE,
    PROPERTY_CYAN,
    PROPERTY_MAGENTA,
    PROPERTY_YELLOW,
    PROPERTY_AMBER,
    PROPERTY_LIME,
    PROPERTY_INDIGO,
    PROPERTY_HUE,
    PROPERTY_SAT,
    PROPERTY_CT,
    PROPERTY_CRI,
    PROPERTY_COLOR_SELECT,
    PROPERTY_COLOR_MIX,
    PROPERTY_BACKGROUND_COLOR_MIX};

const TSet<FName> PROPERTIES_FOCUS = {
    PROPERTY_PAN,
    PROPERTY_TILT,
    PROPERTY_POSITION_MSPEED};

const TSet<FName> PROPERTIES_FORM = {
    PROPERTY_ZOOM,
    PROPERTY_EDGE,
    PROPERTY_EDGE_MODE,
    PROPERTY_IRIS,
    PROPERTY_DIFFUSION,
    PROPERTY_DIFFUSION_2};

const TSet<FName> PROPERTIES_IMAGE = {
    PROPERTY_GOBO_SELECT,
    PROPERTY_GOBO_INDEX_SPEED,
    PROPERTY_GOBO_SELECT_2,
    PROPERTY_GOBO_INDEX_SPEED_2,
    PROPERTY_ANIMATION_SELECT,
    PROPERTY_ANIMATION_INDEX_SPEED,
    PROPERTY_BEAM_FX_SELECT,
    PROPERTY_BEAM_FX_INDEX_SPEED,
    PROPERTY_BEAM_FX_SELECT_2,
    PROPERTY_BEAM_FX_INDEX_SPEED_2,
    PROPERTY_SHUTTER_FRAME_ROT,
    PROPERTY_SHUTTER_1_POS,
    PROPERTY_SHUTTER_1_ROT,
    PROPERTY_SHUTTER_2_POS,
    PROPERTY_SHUTTER_2_ROT,
    PROPERTY_SHUTTER_3_POS,
    PROPERTY_SHUTTER_3_ROT,
    PROPERTY_SHUTTER_4_POS,
    PROPERTY_SHUTTER_4_ROT,
    PROPERTY_SHUTTER_1_A,
    PROPERTY_SHUTTER_1_B,
    PROPERTY_SHUTTER_2_A,
    PROPERTY_SHUTTER_2_B,
    PROPERTY_SHUTTER_3_A,
    PROPERTY_SHUTTER_3_B,
    PROPERTY_SHUTTER_4_A,
    PROPERTY_SHUTTER_4_B};

const TSet<FName> PROPERTIES = {
    PROPERTY_INTENSITY,
    PROPERTY_BACKGROUND_INTENSITY,
    PROPERTY_PIXEL_MASK,
    PROPERTY_PIXEL_MASK_2,
    FName("Cell1"),
    FName("Cell2"),
    FName("Cell3"),
    FName("Cell4"),
    FName("Cell5"),
    FName("Cell6"),
    FName("Cell7"),
    FName("Cell8"),
    FName("Cell9"),
    FName("Cell10"),
    FName("Cell11"),
    FName("Cell12"),
    FName("Cell13"),
    FName("Cell14"),
    FName("Cell15"),
    FName("Cell16"),
    FName("Cell17"),
    FName("Cell18"),
    FName("Cell19"),
    FName("Cell20"),
    FName("Cell21"),
    FName("Cell22"),
    FName("Cell23"),
    FName("Cell24"),
    FName("Cell25"),
    FName("Cell26"),
    FName("Cell27"),
    FName("Cell28"),
    FName("Cell29"),
    FName("Cell30"),
    PROPERTY_RED,
    PROPERTY_GREEN,
    PROPERTY_BLUE,
    PROPERTY_WHITE,
    PROPERTY_CYAN,
    PROPERTY_MAGENTA,
    PROPERTY_YELLOW,
    PROPERTY_AMBER,
    PROPERTY_LIME,
    PROPERTY_INDIGO,
    PROPERTY_HUE,
    PROPERTY_SAT,
    PROPERTY_CT,
    PROPERTY_CRI,
    PROPERTY_COLOR_SELECT,
    PROPERTY_PAN,
    PROPERTY_TILT,
    PROPERTY_POSITION_MSPEED,
    PROPERTY_ZOOM,
    PROPERTY_EDGE,
    PROPERTY_EDGE_MODE,
    PROPERTY_IRIS,
    PROPERTY_DIFFUSION,
    PROPERTY_DIFFUSION_2,
    PROPERTY_GOBO_SELECT,
    PROPERTY_GOBO_INDEX_SPEED,
    PROPERTY_GOBO_SELECT_2,
    PROPERTY_GOBO_INDEX_SPEED_2,
    PROPERTY_ANIMATION_SELECT,
    PROPERTY_ANIMATION_INDEX_SPEED,
    PROPERTY_BEAM_FX_SELECT,
    PROPERTY_BEAM_FX_INDEX_SPEED,
    PROPERTY_BEAM_FX_SELECT_2,
    PROPERTY_BEAM_FX_INDEX_SPEED_2,
    PROPERTY_SHUTTER_FRAME_ROT,
    PROPERTY_SHUTTER_1_POS,
    PROPERTY_SHUTTER_1_ROT,
    PROPERTY_SHUTTER_2_POS,
    PROPERTY_SHUTTER_2_ROT,
    PROPERTY_SHUTTER_3_POS,
    PROPERTY_SHUTTER_3_ROT,
    PROPERTY_SHUTTER_4_POS,
    PROPERTY_SHUTTER_4_ROT,
    PROPERTY_SHUTTER_1_A,
    PROPERTY_SHUTTER_1_B,
    PROPERTY_SHUTTER_2_A,
    PROPERTY_SHUTTER_2_B,
    PROPERTY_SHUTTER_3_A,
    PROPERTY_SHUTTER_3_B,
    PROPERTY_SHUTTER_4_A,
    PROPERTY_SHUTTER_4_B,
    PROPERTY_PIXEL_MASK_EFFECT_TIME,
    PROPERTY_PIXEL_MASK_EFFECT_STEP_TIME,
    PROPERTY_POSITION_BLINK,
    PROPERTY_LED_EFFECT,
    PROPERTY_STROBE,
    PROPERTY_FAN,
    PROPERTY_COLOR_MIX_SPEED,
    PROPERTY_COLOR_MIX,
    PROPERTY_BACKGROUND_COLOR_MIX};
    
class CONCEPTTHEATERSIM_API EOSPatchTypes
{
public:
    static UEOSPatch* create(FName type)
    {
        if(type == LIGHT_TYPE_DIMMER)
        {
            UEOSPatch *patch = UEOSPatch::Create(type, 1);
            patch->addProperty(PROPERTY_INTENSITY, 0);
            return patch;
        }
        if(type == LIGHT_TYPE_ETC_S4_LUSTR_P_DIRECT || type == LIGHT_TYPE_ETC_D60_DIRECT)
        {
            UEOSPatch *patch = UEOSPatch::Create(type, 10);
            patch->addProperty(PROPERTY_INTENSITY, 0);
            patch->addProperty(PROPERTY_RED, 1);
            patch->addProperty(PROPERTY_WHITE, 1);
            patch->addProperty(PROPERTY_AMBER, 1);
            patch->addProperty(PROPERTY_GREEN, 1);
            patch->addProperty(PROPERTY_CYAN, 1);
            patch->addProperty(PROPERTY_BLUE, 1);
            patch->addProperty(PROPERTY_INDIGO, 1);
            patch->addProperty(PROPERTY_STROBE);
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_FAN, {
                UEOSPropertyIndex::Create(FName("Auto"), 0, 0),
                UEOSPropertyIndex::Create(FName("On"), 1, 254, true),
                UEOSPropertyIndex::Create(FName("Full"), 255, 255)
            }));
            return patch;
        }
        if(type == LIGHT_TYPE_ETC_COLOR_SOURCE_CYC_DIRECT)
        {
            UEOSPatch *patch = UEOSPatch::Create(type, 10);
            patch->addProperty(PROPERTY_INTENSITY, 0);
            patch->addProperty(PROPERTY_RED, 1);
            patch->addProperty(PROPERTY_GREEN, 1);
            patch->addProperty(PROPERTY_BLUE, 1);
            patch->addProperty(PROPERTY_INDIGO, 1);
            patch->addProperty(PROPERTY_LIME, 1);
            patch->addProperty(PROPERTY_STROBE);
            return patch;
        }
        if(type == LIGHT_TYPE_MAVERICK_MK3_PROFILE_54CH)
        {
            UEOSPatch *patch = UEOSPatch::Create(type, 54);
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_PAN, 0, -270, 270, 1));
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_TILT, 0, -135, 135, 1));
            patch->addProperty(PROPERTY_POSITION_MSPEED);
            patch->addProperty(PROPERTY_INTENSITY);
            patch->addProperty(PROPERTY_STROBE);
            patch->addProperty(PROPERTY_CYAN, 0);
            patch->addProperty(PROPERTY_MAGENTA, 0);
            patch->addProperty(PROPERTY_YELLOW, 0);
            patch->addProperty(PROPERTY_CT);
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_COLOR_SELECT, {
                UEOSPropertyIndex::Create(FName("Open"), 0, 7),
                UEOSPropertyIndex::Create(FName("Red"), 8, 15),
                UEOSPropertyIndex::Create(FName("Orange"), 16, 23),
                UEOSPropertyIndex::Create(FName("Green"), 24, 31),
                UEOSPropertyIndex::Create(FName("Yellow"), 32, 39),
                UEOSPropertyIndex::Create(FName("Dark Blue"), 40, 47),
                UEOSPropertyIndex::Create(FName("CTB"), 48, 59),
                UEOSPropertyIndex::Create(FName("Indexed"), 60, 187, true),
                UEOSPropertyIndex::Create(FName("Scroll"), 118, 2198, true),
                UEOSPropertyIndex::Create(FName("Stop"), 220, 223),
                UEOSPropertyIndex::Create(FName("Scroll Reverse"), 224, 255, true),
            }));
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_GOBO_SELECT, {
                UEOSPropertyIndex::Create(FName("Open"), 0, 7),
                UEOSPropertyIndex::Create(FName("Gobo 1"), 8, 15),
                UEOSPropertyIndex::Create(FName("Gobo 2"), 16, 23),
                UEOSPropertyIndex::Create(FName("Gobo 3"), 24, 31),
                UEOSPropertyIndex::Create(FName("Gobo 4"), 32, 39),
                UEOSPropertyIndex::Create(FName("Gobo 5"), 40, 47),
                UEOSPropertyIndex::Create(FName("Gobo 6"), 48, 55),
                UEOSPropertyIndex::Create(FName("Gobo 7"), 56, 63),
                UEOSPropertyIndex::Create(FName("Gobo 7 Shaking"), 64, 71),
                UEOSPropertyIndex::Create(FName("Gobo 6 Shaking"), 72, 79),
                UEOSPropertyIndex::Create(FName("Gobo 5 Shaking"), 80, 87),
                UEOSPropertyIndex::Create(FName("Gobo 4 Shaking"), 88, 95),
                UEOSPropertyIndex::Create(FName("Gobo 3 Shaking"), 96, 103),
                UEOSPropertyIndex::Create(FName("Gobo 2 Shaking"), 104, 111),
                UEOSPropertyIndex::Create(FName("Gobo 1 Shaking"), 112, 119),
                UEOSPropertyIndex::Create(FName("Open"), 120, 127),
                UEOSPropertyIndex::Create(FName("Scroll"), 128, 191, true),
                UEOSPropertyIndex::Create(FName("Scroll Reverse"), 192, 255, true),
            }));
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_GOBO_INDEX_SPEED, {
                UEOSPropertyIndex::Create(FName("Index"), 0, 63, true),
                UEOSPropertyIndex::Create(FName("Rotate"), 64, 145, true),
                UEOSPropertyIndex::Create(FName("Stop"), 146, 149),
                UEOSPropertyIndex::Create(FName("Rotate Reverse"), 150, 231, true),
                UEOSPropertyIndex::Create(FName("Bounce"), 232, 255, true)
            }));
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_GOBO_SELECT_2, {
                UEOSPropertyIndex::Create(FName("Open"), 0, 5),
                UEOSPropertyIndex::Create(FName("Gobo 1"), 6, 11),
                UEOSPropertyIndex::Create(FName("Gobo 2"), 12, 17),
                UEOSPropertyIndex::Create(FName("Gobo 3"), 18, 23),
                UEOSPropertyIndex::Create(FName("Gobo 4"), 24, 29),
                UEOSPropertyIndex::Create(FName("Gobo 5"), 30, 35),
                UEOSPropertyIndex::Create(FName("Gobo 6"), 36, 41),
                UEOSPropertyIndex::Create(FName("Gobo 7"), 42, 47),
                UEOSPropertyIndex::Create(FName("Gobo 8"), 48, 53),
                UEOSPropertyIndex::Create(FName("Gobo 9"), 54, 63),
                UEOSPropertyIndex::Create(FName("Gobo 9 Shaking"), 64, 69),
                UEOSPropertyIndex::Create(FName("Gobo 8 Shaking"), 70, 75),
                UEOSPropertyIndex::Create(FName("Gobo 7 Shaking"), 76, 81),
                UEOSPropertyIndex::Create(FName("Gobo 6 Shaking"), 82, 87),
                UEOSPropertyIndex::Create(FName("Gobo 5 Shaking"), 88, 93),
                UEOSPropertyIndex::Create(FName("Gobo 4 Shaking"), 94, 99),
                UEOSPropertyIndex::Create(FName("Gobo 3 Shaking"), 100, 105),
                UEOSPropertyIndex::Create(FName("Gobo 2 Shaking"), 106, 111),
                UEOSPropertyIndex::Create(FName("Gobo 1 Shaking"), 112, 117),
                UEOSPropertyIndex::Create(FName("Open"), 118, 127),
                UEOSPropertyIndex::Create(FName("Scroll"), 128, 191, true),
                UEOSPropertyIndex::Create(FName("Scroll Reverse"), 192, 255, true),
            }));
            patch->addProperty(PROPERTY_ANIMATION_SELECT);
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_ANIMATION_INDEX_SPEED, {
                UEOSPropertyIndex::Create(FName("Rotate"), 0, 124, true),
                UEOSPropertyIndex::Create(FName("Stop"), 125, 130),
                UEOSPropertyIndex::Create(FName("Rotate Reverse"), 131, 255, true),
            }));
            patch->addProperty(PROPERTY_SHUTTER_3_B);
            patch->addProperty(PROPERTY_SHUTTER_3_A);
            patch->addProperty(PROPERTY_SHUTTER_2_B);
            patch->addProperty(PROPERTY_SHUTTER_2_A);
            patch->addProperty(PROPERTY_SHUTTER_1_B);
            patch->addProperty(PROPERTY_SHUTTER_1_A);
            patch->addProperty(PROPERTY_SHUTTER_4_B);
            patch->addProperty(PROPERTY_SHUTTER_4_A);
            patch->addProperty(PROPERTY_SHUTTER_FRAME_ROT);
            patch->addProperty(PROPERTY_EDGE);
            patch->addProperty(PROPERTY_EDGE_MODE);
            patch->addProperty(PROPERTY_ZOOM);
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_BEAM_FX_SELECT, {
                UEOSPropertyIndex::Create(FName("Open"), 0, 4),
                UEOSPropertyIndex::Create(FName("Prism"), 5, 255)
            }));
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_BEAM_FX_INDEX_SPEED, {
                UEOSPropertyIndex::Create(FName("Index"), 0, 127, true),
                UEOSPropertyIndex::Create(FName("Rotate"), 128, 189, true),
                UEOSPropertyIndex::Create(FName("Stop"), 190, 193),
                UEOSPropertyIndex::Create(FName("Rotate Reverse"), 194, 255, true)
            }));
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_BEAM_FX_SELECT_2, {
                UEOSPropertyIndex::Create(FName("Open"), 0, 4),
                UEOSPropertyIndex::Create(FName("Prism"), 5, 255)
            }));
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_BEAM_FX_INDEX_SPEED_2, {
                UEOSPropertyIndex::Create(FName("Index"), 0, 127, true),
                UEOSPropertyIndex::Create(FName("Rotate"), 128, 189, true),
                UEOSPropertyIndex::Create(FName("Stop"), 190, 193),
                UEOSPropertyIndex::Create(FName("Rotate Reverse"), 194, 255, true)
            }));
            patch->addProperty(PROPERTY_IRIS);
            patch->addProperty(PROPERTY_DIFFUSION);
            patch->addProperty(PROPERTY_DIFFUSION_2);
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_CRI, {
                UEOSPropertyIndex::Create(FName("Open"), 0, 4),
                UEOSPropertyIndex::Create(FName("CRI Filter"), 5, 189)
            }));
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_COLOR_MIX, {
                UEOSPropertyIndex::Create(FName("Open"), 0, 4),
                UEOSPropertyIndex::Create(FName("Macro"), 5, 189)
            }));
            patch->addProperty(PROPERTY_COLOR_MIX_SPEED);
            patch->addProperty(PROPERTY_POSITION_BLINK);
            return patch;
        }
        if(type == LIGHT_TYPE_MAVERICK_MK3_WASH_BASIC)
        {
            UEOSPatch *patch = UEOSPatch::Create(type, 21);
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_PAN, 0, -270, 270, 1));
            patch->addProperty(UEOSPropertyType::Create(PROPERTY_TILT, 0, -135, 135, 1));
            patch->addProperty(PROPERTY_POSITION_MSPEED);
            patch->addProperty(PROPERTY_CT);
            patch->addProperty(PROPERTY_COLOR_MIX);
            patch->addProperty(PROPERTY_PIXEL_MASK_2);
            patch->addProperty(PROPERTY_PIXEL_MASK);
            patch->addProperty(PROPERTY_PIXEL_MASK_EFFECT_TIME);
            patch->addProperty(PROPERTY_PIXEL_MASK_EFFECT_STEP_TIME);
            patch->addProperty(PROPERTY_BACKGROUND_COLOR_MIX);
            patch->addProperty(PROPERTY_BACKGROUND_INTENSITY);
            patch->addProperty(PROPERTY_INTENSITY);
            patch->addProperty(PROPERTY_STROBE);
            patch->addProperty(PROPERTY_ZOOM);
            patch->addProperty(PROPERTY_POSITION_BLINK);
            patch->addProperty(PROPERTY_RED, 1);
            patch->addProperty(PROPERTY_GREEN, 1);
            patch->addProperty(PROPERTY_BLUE, 1);
            patch->addProperty(PROPERTY_WHITE, 1);
            return patch;
        }
        if(type == LIGHT_TYPE_GENERIC_LIGHT_PANEL)
        {
            UEOSPatch *patch = UEOSPatch::Create(type, 21);
            patch->addProperty(PROPERTY_INTENSITY, 0);
            patch->addProperty(PROPERTY_RED, 1);
            patch->addProperty(PROPERTY_GREEN, 1);
            patch->addProperty(PROPERTY_BLUE, 1);
            
            patch->addProperty(FName(TEXT("Cell1")), 1);
            patch->addProperty(FName(TEXT("Cell2")), 1);
            patch->addProperty(FName(TEXT("Cell3")), 1);
            patch->addProperty(FName(TEXT("Cell4")), 1);
            patch->addProperty(FName(TEXT("Cell5")), 1);
            patch->addProperty(FName(TEXT("Cell6")), 1);
            patch->addProperty(FName(TEXT("Cell7")), 1);
            patch->addProperty(FName(TEXT("Cell8")), 1);
            patch->addProperty(FName(TEXT("Cell9")), 1);
            patch->addProperty(FName(TEXT("Cell10")), 1);
            patch->addProperty(FName(TEXT("Cell11")), 1);
            patch->addProperty(FName(TEXT("Cell12")), 1);
            patch->addProperty(FName(TEXT("Cell13")), 1);
            patch->addProperty(FName(TEXT("Cell14")), 1);
            patch->addProperty(FName(TEXT("Cell15")), 1);
            patch->addProperty(FName(TEXT("Cell16")), 1);
            patch->addProperty(FName(TEXT("Cell17")), 1);
            patch->addProperty(FName(TEXT("Cell18")), 1);
            patch->addProperty(FName(TEXT("Cell19")), 1);
            patch->addProperty(FName(TEXT("Cell20")), 1);
            patch->addProperty(FName(TEXT("Cell21")), 1);
            patch->addProperty(FName(TEXT("Cell22")), 1);
            patch->addProperty(FName(TEXT("Cell23")), 1);
            patch->addProperty(FName(TEXT("Cell24")), 1);
            patch->addProperty(FName(TEXT("Cell25")), 1);
            return patch;
        }
        UE_LOG(LogTemp, Warning, TEXT("Unknown patch type: %s"), *(type.ToString()));
        UEOSPatch *def = UEOSPatch::Create(type, 1);
        def->addProperty(PROPERTY_INTENSITY);
        return def;
    }
};