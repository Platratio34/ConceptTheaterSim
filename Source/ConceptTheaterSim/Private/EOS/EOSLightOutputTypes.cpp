// Copyright Peter Crall 2025

#include "EOS/EOSLightOutputTypes.h"

class CONCEPTTHEATERSIM_API Dimmer : public EOSLightOutputType
{
    virtual void output(UEOSPropertySet *parameters, TArray<int>& dmx, int start) override
    {
        dmx[start] = toByte(parameters->get(PROPERTY_INTENSITY));
    }

    virtual void input(TArray<int> &dmx, UEOSPropertySet *parameters, int start) override
    {
        parameters->set(PROPERTY_INTENSITY, fromByte(dmx[start]));
    } 
};

class CONCEPTTHEATERSIM_API ETCLustrDirect : public EOSLightOutputType
{
    virtual void output(UEOSPropertySet *parameters, TArray<int>& dmx, int start) override
    {
        dmx[start+0] = toByte(parameters->get(PROPERTY_RED));
        dmx[start+1] = toByte(parameters->get(PROPERTY_WHITE));
        dmx[start+2] = toByte(parameters->get(PROPERTY_AMBER));
        dmx[start+3] = toByte(parameters->get(PROPERTY_GREEN));
        dmx[start+4] = toByte(parameters->get(PROPERTY_CYAN));
        dmx[start+5] = toByte(parameters->get(PROPERTY_BLUE));
        dmx[start+6] = toByte(parameters->get(PROPERTY_INDIGO));
        dmx[start+7] = toByte(parameters->get(PROPERTY_INTENSITY));
        dmx[start+8] = 0; // strobe
        dmx[start+9] = 0; // intensity
    }

    virtual void input(TArray<int> &dmx, UEOSPropertySet *parameters, int start) override
    {
        parameters->set(PROPERTY_RED, fromByte(dmx[start+0]));
        parameters->set(PROPERTY_WHITE, fromByte(dmx[start+1]));
        parameters->set(PROPERTY_AMBER, fromByte(dmx[start+2]));
        parameters->set(PROPERTY_GREEN, fromByte(dmx[start+3]));
        parameters->set(PROPERTY_CYAN, fromByte(dmx[start+4]));
        parameters->set(PROPERTY_BLUE, fromByte(dmx[start+5]));
        parameters->set(PROPERTY_INDIGO, fromByte(dmx[start+6]));
        parameters->set(PROPERTY_INTENSITY, fromByte(dmx[start+7]));
    } 
};

class CONCEPTTHEATERSIM_API ETCD60LustrDirect : public EOSLightOutputType
{
    virtual void output(UEOSPropertySet *parameters, TArray<int>& dmx, int start) override
    {
        dmx[start+0] = toByte(parameters->get(PROPERTY_RED));
        dmx[start+1] = toByte(parameters->get(PROPERTY_WHITE));
        dmx[start+2] = toByte(parameters->get(PROPERTY_AMBER));
        dmx[start+3] = toByte(parameters->get(PROPERTY_GREEN));
        dmx[start+4] = toByte(parameters->get(PROPERTY_CYAN));
        dmx[start+5] = toByte(parameters->get(PROPERTY_BLUE));
        dmx[start+6] = toByte(parameters->get(PROPERTY_INDIGO));
        dmx[start+7] = toByte(parameters->get(PROPERTY_INTENSITY));
        dmx[start+8] = 0; // strobe
        dmx[start+9] = 0; // intensity
    }

    virtual void input(TArray<int> &dmx, UEOSPropertySet *parameters, int start) override
    {
        parameters->set(PROPERTY_RED, fromByte(dmx[start+0]));
        parameters->set(PROPERTY_WHITE, fromByte(dmx[start+1]));
        parameters->set(PROPERTY_AMBER, fromByte(dmx[start+2]));
        parameters->set(PROPERTY_GREEN, fromByte(dmx[start+3]));
        parameters->set(PROPERTY_CYAN, fromByte(dmx[start+4]));
        parameters->set(PROPERTY_BLUE, fromByte(dmx[start+5]));
        parameters->set(PROPERTY_INDIGO, fromByte(dmx[start+6]));
        parameters->set(PROPERTY_INTENSITY, fromByte(dmx[start+7]));
    } 
};

class CONCEPTTHEATERSIM_API ETCColorSourceDirect : public EOSLightOutputType
{
    virtual void output(UEOSPropertySet *parameters, TArray<int>& dmx, int start) override
    {
        dmx[start+0] = toByte(parameters->get(PROPERTY_INTENSITY));
        dmx[start+1] = toByte(parameters->get(PROPERTY_RED));
        dmx[start+2] = toByte(parameters->get(PROPERTY_GREEN));
        dmx[start+3] = toByte(parameters->get(PROPERTY_BLUE));
        dmx[start+4] = toByte(parameters->get(PROPERTY_INDIGO));
        dmx[start+5] = toByte(parameters->get(PROPERTY_LIME));
        dmx[start+6] = 0; // strobe
    }

    virtual void input(TArray<int> &dmx, UEOSPropertySet *parameters, int start) override
    {
        parameters->set(PROPERTY_INTENSITY, fromByte(dmx[start+0]));
        parameters->set(PROPERTY_RED, fromByte(dmx[start+1]));
        parameters->set(PROPERTY_GREEN, fromByte(dmx[start+2]));
        parameters->set(PROPERTY_BLUE, fromByte(dmx[start+3]));
        parameters->set(PROPERTY_INDIGO, fromByte(dmx[start+4]));
        parameters->set(PROPERTY_LIME, fromByte(dmx[start+5]));
    } 
};

class CONCEPTTHEATERSIM_API MaverickMk3Profile54Ch : public EOSLightOutputType
{
    virtual void output(UEOSPropertySet *parameters, TArray<int>& dmx, int start) override
    {
        dmx[start+0] = toByteCoarseRanged(parameters->get(PROPERTY_PAN), -100, 100);
        dmx[start+1] = toByteFineRanged(parameters->get(PROPERTY_PAN), -100, 100);
        dmx[start+2] = toByteCoarseRanged(parameters->get(PROPERTY_TILT), -135, 135);
        dmx[start+3] = toByteFineRanged(parameters->get(PROPERTY_TILT), -135, 135);
        dmx[start+4] = toByte(parameters->get(PROPERTY_POSITION_MSPEED));
        dmx[start+5] = toByteCoarse(parameters->get(PROPERTY_INTENSITY));
        dmx[start+6] = toByteFine(parameters->get(PROPERTY_INTENSITY));
        dmx[start+7] = (int)(parameters->get(PROPERTY_STROBE));
        dmx[start+8] = 0; // virtual strobe
        dmx[start+9] = toByte(parameters->get(PROPERTY_CYAN));
        dmx[start+10] = toByte(parameters->get(PROPERTY_MAGENTA));
        dmx[start+11] = toByte(parameters->get(PROPERTY_YELLOW));
        dmx[start+12] = toByte(parameters->get(PROPERTY_CT));
        dmx[start+13] = (int)parameters->get(PROPERTY_COLOR_SELECT);
        dmx[start+14] = (int)(parameters->get(PROPERTY_GOBO_SELECT));
        dmx[start+15] = (int)(parameters->get(PROPERTY_GOBO_INDEX_SPEED)) >> 8;
        dmx[start+16] = (int)(parameters->get(PROPERTY_GOBO_INDEX_SPEED)) & 0xff;
        dmx[start+17] = (int)(parameters->get(PROPERTY_GOBO_SELECT_2));
        dmx[start+18] = (int)(parameters->get(PROPERTY_ANIMATION_SELECT));
        dmx[start+19] = (int)(parameters->get(PROPERTY_ANIMATION_INDEX_SPEED));
        dmx[start+20] = toByteCoarse(parameters->get(PROPERTY_SHUTTER_3_B));
        dmx[start+21] = toByteFine(parameters->get(PROPERTY_SHUTTER_3_B));
        dmx[start+22] = toByteCoarse(parameters->get(PROPERTY_SHUTTER_3_A));
        dmx[start+23] = toByteFine(parameters->get(PROPERTY_SHUTTER_3_A));
        dmx[start+24] = toByteCoarse(parameters->get(PROPERTY_SHUTTER_2_B));
        dmx[start+25] = toByteFine(parameters->get(PROPERTY_SHUTTER_2_B));
        dmx[start+26] = toByteCoarse(parameters->get(PROPERTY_SHUTTER_2_A));
        dmx[start+27] = toByteFine(parameters->get(PROPERTY_SHUTTER_2_A));
        dmx[start+28] = toByteCoarse(parameters->get(PROPERTY_SHUTTER_1_B));
        dmx[start+29] = toByteFine(parameters->get(PROPERTY_SHUTTER_1_B));
        dmx[start+30] = toByteCoarse(parameters->get(PROPERTY_SHUTTER_1_A));
        dmx[start+31] = toByteFine(parameters->get(PROPERTY_SHUTTER_1_A));
        dmx[start+32] = toByteCoarse(parameters->get(PROPERTY_SHUTTER_4_B));
        dmx[start+33] = toByteFine(parameters->get(PROPERTY_SHUTTER_4_B));
        dmx[start+34] = toByteCoarse(parameters->get(PROPERTY_SHUTTER_4_A));
        dmx[start+35] = toByteFine(parameters->get(PROPERTY_SHUTTER_4_A));
        dmx[start+36] = toByteCoarseRanged(parameters->get(PROPERTY_SHUTTER_FRAME_ROT), -1, 1);
        dmx[start+37] = toByteFineRanged(parameters->get(PROPERTY_SHUTTER_FRAME_ROT), -1, 1);
        dmx[start+38] = toByteCoarseRanged(parameters->get(PROPERTY_EDGE), 0, 1);
        dmx[start+39] = toByteFineRanged(parameters->get(PROPERTY_EDGE), 0, 1);
        dmx[start+40] = (int)(parameters->get(PROPERTY_EDGE_MODE));
        dmx[start+41] = toByteCoarseRanged(parameters->get(PROPERTY_ZOOM), 50, 10);
        dmx[start+42] = toByteFineRanged(parameters->get(PROPERTY_ZOOM), 50, 10);
        dmx[start+43] = (int)(parameters->get(PROPERTY_BEAM_FX_SELECT));
        dmx[start+44] = (int)(parameters->get(PROPERTY_BEAM_FX_INDEX_SPEED));
        dmx[start+45] = (int)(parameters->get(PROPERTY_BEAM_FX_SELECT_2));
        dmx[start+46] = (int)(parameters->get(PROPERTY_BEAM_FX_INDEX_SPEED_2));
        dmx[start+47] = (int)(parameters->get(PROPERTY_IRIS));
        dmx[start+48] = toByte(parameters->get(PROPERTY_DIFFUSION));
        dmx[start+49] = toByte(parameters->get(PROPERTY_DIFFUSION_2));
        dmx[start+50] = (int)(parameters->get(PROPERTY_CRI));
        dmx[start+51] = (int)(parameters->get(PROPERTY_COLOR_MIX));
        dmx[start+52] = toByte(parameters->get(PROPERTY_COLOR_MIX_SPEED));
        dmx[start+53] = 0x0; // control
    }

    virtual void input(TArray<int> &dmx, UEOSPropertySet *parameters, int start) override
    {
        parameters->set(PROPERTY_PAN, fromByte16bRanged(dmx[start+0], dmx[start+1], -100, 100));
        parameters->set(PROPERTY_TILT, fromByte16bRanged(dmx[start+2], dmx[start+3], -135, 135));
        parameters->set(PROPERTY_POSITION_MSPEED, fromByte(dmx[start+4]));
        parameters->set(PROPERTY_INTENSITY, fromByte16b(dmx[start+5], dmx[start+6]));
        parameters->set(PROPERTY_STROBE, dmx[start+7]);
        // parameters->set(PROPERTY_STROBE, dmx[start+8]); // virtual strobe
        parameters->set(PROPERTY_CYAN, fromByte(dmx[start+9]));
        parameters->set(PROPERTY_MAGENTA, fromByte(dmx[start+10]));
        parameters->set(PROPERTY_YELLOW, fromByte(dmx[start+11]));
        parameters->set(PROPERTY_CT, fromByte(dmx[start+12]));
        parameters->set(PROPERTY_COLOR_SELECT, dmx[start+13]);
        parameters->set(PROPERTY_GOBO_SELECT, dmx[start+14]);
        parameters->set(PROPERTY_GOBO_INDEX_SPEED, (dmx[start+15] << 8) | dmx[start+16]);
        parameters->set(PROPERTY_GOBO_SELECT_2, dmx[start+17]);
        parameters->set(PROPERTY_ANIMATION_SELECT, dmx[start+18]);
        parameters->set(PROPERTY_ANIMATION_INDEX_SPEED, dmx[start+19]);
        parameters->set(PROPERTY_SHUTTER_3_B, fromByte16b(dmx[start+20], dmx[start+21]));
        parameters->set(PROPERTY_SHUTTER_3_A, fromByte16b(dmx[start+22], dmx[start+23]));
        parameters->set(PROPERTY_SHUTTER_2_B, fromByte16b(dmx[start+24], dmx[start+25]));
        parameters->set(PROPERTY_SHUTTER_2_A, fromByte16b(dmx[start+26], dmx[start+27]));
        parameters->set(PROPERTY_SHUTTER_1_B, fromByte16b(dmx[start+28], dmx[start+29]));
        parameters->set(PROPERTY_SHUTTER_1_A, fromByte16b(dmx[start+30], dmx[start+31]));
        parameters->set(PROPERTY_SHUTTER_4_B, fromByte16b(dmx[start+32], dmx[start+33]));
        parameters->set(PROPERTY_SHUTTER_4_A, fromByte16b(dmx[start+34], dmx[start+35]));
        parameters->set(PROPERTY_SHUTTER_FRAME_ROT, fromByte16bRanged(dmx[start+36], dmx[start+37], -1, 1));
        parameters->set(PROPERTY_EDGE, fromByte16b(dmx[start+38], dmx[start+39]));
        parameters->set(PROPERTY_EDGE_MODE, dmx[start+40]);
        parameters->set(PROPERTY_ZOOM, fromByte16bRanged(dmx[start+41], dmx[start+42], 50, 10));
        parameters->set(PROPERTY_BEAM_FX_SELECT, dmx[start+43]);
        parameters->set(PROPERTY_BEAM_FX_INDEX_SPEED, dmx[start+44]);
        parameters->set(PROPERTY_BEAM_FX_SELECT_2, dmx[start+45]);
        parameters->set(PROPERTY_BEAM_FX_INDEX_SPEED_2, dmx[start+46]);
        parameters->set(PROPERTY_IRIS, dmx[start+47]);
        parameters->set(PROPERTY_DIFFUSION, fromByte(dmx[start+48]));
        parameters->set(PROPERTY_DIFFUSION_2, fromByte(dmx[start+49]));
        parameters->set(PROPERTY_CRI, dmx[start+50]);
        parameters->set(PROPERTY_COLOR_MIX, dmx[start+51]);
        parameters->set(PROPERTY_COLOR_MIX_SPEED, fromByte(dmx[start+52]));
        // dmx[start+53] = 0x0; // control
    } 
};

class CONCEPTTHEATERSIM_API MaverickMk3WashBasic : public EOSLightOutputType
{
    virtual void output(UEOSPropertySet *parameters, TArray<int>& dmx, int start) override
    {
        dmx[start+0] = toByteCoarse(parameters->get(PROPERTY_PAN));
        dmx[start+1] = toByteFine(parameters->get(PROPERTY_PAN));
        dmx[start+2] = toByteCoarse(parameters->get(PROPERTY_TILT));
        dmx[start+3] = toByteFine(parameters->get(PROPERTY_TILT));
        dmx[start+4] = toByte(parameters->get(PROPERTY_POSITION_MSPEED));
        dmx[start+5] = (int)(parameters->get(PROPERTY_CT));
        dmx[start+6] = (int)(parameters->get(PROPERTY_COLOR_MIX));
        dmx[start+7] = (int)(parameters->get(PROPERTY_GOBO_SELECT));
        dmx[start+8] = (int)(parameters->get(PROPERTY_PIXEL_MASK_2));
        dmx[start+9] = (int)(parameters->get(PROPERTY_PIXEL_MASK_EFFECT_TIME));
        dmx[start+10] = (int)(parameters->get(PROPERTY_PIXEL_MASK_EFFECT_STEP_TIME));
        dmx[start+11] = (int)(parameters->get(PROPERTY_BACKGROUND_COLOR_MIX));
        dmx[start+12] = toByte(parameters->get(PROPERTY_BACKGROUND_INTENSITY));
        dmx[start+13] = toByte(parameters->get(PROPERTY_INTENSITY));
        dmx[start+14] = (int)(parameters->get(PROPERTY_STROBE));
        dmx[start+15] = toByte(parameters->get(PROPERTY_ZOOM));
        dmx[start+16] = 0x0; // control
        dmx[start+17] = toByte(parameters->get(PROPERTY_RED));
        dmx[start+18] = toByte(parameters->get(PROPERTY_GREEN));
        dmx[start+19] = toByte(parameters->get(PROPERTY_BLUE));
        dmx[start+20] = toByte(parameters->get(PROPERTY_WHITE));
    }

    virtual void input(TArray<int> &dmx, UEOSPropertySet *parameters, int start) override
    {
        parameters->set(PROPERTY_PAN, fromByte16b(dmx[start+0], dmx[start+1]));
        parameters->set(PROPERTY_TILT, fromByte16b(dmx[start+2], dmx[start+3]));
        parameters->set(PROPERTY_POSITION_MSPEED, fromByte(dmx[start+4]));
        parameters->set(PROPERTY_CT, dmx[start+5]);
        parameters->set(PROPERTY_COLOR_MIX, dmx[start+6]);
        parameters->set(PROPERTY_GOBO_SELECT, dmx[start+7]);
        parameters->set(PROPERTY_PIXEL_MASK_2, dmx[start+8]);
        parameters->set(PROPERTY_PIXEL_MASK_EFFECT_TIME, dmx[start+9]);
        parameters->set(PROPERTY_PIXEL_MASK_EFFECT_STEP_TIME, dmx[start+10]);
        parameters->set(PROPERTY_BACKGROUND_COLOR_MIX, dmx[start+11]);
        parameters->set(PROPERTY_BACKGROUND_INTENSITY, fromByte(dmx[start+12]));
        parameters->set(PROPERTY_INTENSITY, fromByte(dmx[start+13]));
        parameters->set(PROPERTY_STROBE, dmx[start+14]);
        parameters->set(PROPERTY_ZOOM, fromByte(dmx[start+15]));
        // dmx[start+16] = 0x0; // control
        parameters->set(PROPERTY_RED, fromByte(dmx[start+17]));
        parameters->set(PROPERTY_GREEN, fromByte(dmx[start+18]));
        parameters->set(PROPERTY_BLUE, fromByte(dmx[start+19]));
        parameters->set(PROPERTY_WHITE, fromByte(dmx[start+20]));
    } 
};

EOSLightOutputType* EOSLightOutputType::getType(FName type)
{
    static TMap < FName, EOSLightOutputType*> types;
    if(EOSLightOutputType** p = types.Find(type))
    {
        return *p;
    }
    EOSLightOutputType *typeI = nullptr;
    if(type == LIGHT_TYPE_DIMMER)
    {
        typeI = new Dimmer();
    }
    else if(type == LIGHT_TYPE_ETC_S4_LUSTR_P_DIRECT)
    {
        typeI = new ETCLustrDirect();
    }
    else if(type == LIGHT_TYPE_ETC_D60_DIRECT)
    {
        typeI = new ETCD60LustrDirect();
    }
    else if(type == LIGHT_TYPE_ETC_COLOR_SOURCE_CYC_DIRECT)
    {
        typeI = new ETCColorSourceDirect();
    }
    else if(type == LIGHT_TYPE_MAVERICK_MK3_WASH_BASIC)
    {
        typeI = new MaverickMk3WashBasic();
    }
    else if(type == LIGHT_TYPE_MAVERICK_MK3_PROFILE_54CH)
    {
        typeI = new MaverickMk3Profile54Ch();
    }
    else
    {
        typeI = new Dimmer();
    }
    types.Add(type, typeI);
    return typeI;
}