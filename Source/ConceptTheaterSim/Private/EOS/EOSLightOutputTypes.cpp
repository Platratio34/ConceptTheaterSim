// Copyright Peter Crall 2025

#include "EOS/EOSLightOutputTypes.h"

class CONCEPTTHEATERSIM_API Dimmer : public EOSLightOutputType
{
    virtual TArray<int> output(TMap<FName, double> parameters) override
    {
        TArray<int> out;
        out.Init(0, 1);
        out[0] = toByte(parameters[PROPERTY_INTENSITY]);
        return out;
    }

    virtual void input(TArray<int> dmx, TMap<FName, double> parameters) override
    {
        parameters[PROPERTY_INTENSITY] = fromByte(dmx[0]);
    } 
};

class CONCEPTTHEATERSIM_API ETCLustrDirect : public EOSLightOutputType
{
    virtual TArray<int> output(TMap<FName, double> parameters) override
    {
        TArray<int> out;
        out.Init(0, 10);
        out[0] = toByte(parameters[PROPERTY_RED]);
        out[1] = toByte(parameters[PROPERTY_WHITE]);
        out[2] = toByte(parameters[PROPERTY_AMBER]);
        out[3] = toByte(parameters[PROPERTY_GREEN]);
        out[4] = toByte(parameters[PROPERTY_CYAN]);
        out[5] = toByte(parameters[PROPERTY_BLUE]);
        out[6] = toByte(parameters[PROPERTY_INDIGO]);
        out[7] = toByte(parameters[PROPERTY_INTENSITY]);
        out[8] = 0; // strobe
        out[9] = 0; // intensity
        return out;
    }

    virtual void input(TArray<int> dmx, TMap<FName, double> parameters) override
    {
        parameters[PROPERTY_RED] = fromByte(dmx[0]);
        parameters[PROPERTY_WHITE] = fromByte(dmx[1]);
        parameters[PROPERTY_AMBER] = fromByte(dmx[2]);
        parameters[PROPERTY_GREEN] = fromByte(dmx[3]);
        parameters[PROPERTY_CYAN] = fromByte(dmx[4]);
        parameters[PROPERTY_BLUE] = fromByte(dmx[5]);
        parameters[PROPERTY_INDIGO] = fromByte(dmx[6]);
        parameters[PROPERTY_INTENSITY] = fromByte(dmx[7]);
    } 
};

class CONCEPTTHEATERSIM_API ETCColorSourceDirect : public EOSLightOutputType
{
    virtual TArray<int> output(TMap<FName, double> parameters) override
    {
        TArray<int> out;
        out.Init(0, 7);
        out[0] = toByte(parameters[PROPERTY_RED]);
        out[1] = toByte(parameters[PROPERTY_GREEN]);
        out[2] = toByte(parameters[PROPERTY_BLUE]);
        out[3] = toByte(parameters[PROPERTY_INDIGO]);
        out[4] = toByte(parameters[PROPERTY_LIME]);
        out[5] = toByte(parameters[PROPERTY_INTENSITY]);
        out[6] = 0; // strobe
        return out;
    }

    virtual void input(TArray<int> dmx, TMap<FName, double> parameters) override
    {
        parameters[PROPERTY_RED] = fromByte(dmx[0]);
        parameters[PROPERTY_GREEN] = fromByte(dmx[1]);
        parameters[PROPERTY_BLUE] = fromByte(dmx[2]);
        parameters[PROPERTY_INDIGO] = fromByte(dmx[3]);
        parameters[PROPERTY_LIME] = fromByte(dmx[4]);
        parameters[PROPERTY_INTENSITY] = fromByte(dmx[5]);
    } 
};

class CONCEPTTHEATERSIM_API MaverickMk3Profile54Ch : public EOSLightOutputType
{
    virtual TArray<int> output(TMap<FName, double> parameters) override
    {
        TArray<int> out;
        out.Init(0, 54);
        out[0] = toByteCoarse(parameters[PROPERTY_PAN]);
        out[1] = toByteFine(parameters[PROPERTY_PAN]);
        out[2] = toByteCoarse(parameters[PROPERTY_TILT]);
        out[3] = toByteFine(parameters[PROPERTY_TILT]);
        out[4] = toByte(parameters[PROPERTY_POSITION_MSPEED]);
        out[5] = toByteCoarse(parameters[PROPERTY_INTENSITY]);
        out[6] = toByteFine(parameters[PROPERTY_INTENSITY]);
        out[7] = (int)(parameters[PROPERTY_STROBE]);
        out[8] = 0; // virtual strobe
        out[9] = toByte(parameters[PROPERTY_CYAN]);
        out[10] = toByte(parameters[PROPERTY_MAGENTA]);
        out[11] = toByte(parameters[PROPERTY_YELLOW]);
        out[12] = toByte(parameters[PROPERTY_CT]);
        out[13] = (int)parameters[PROPERTY_COLOR_SELECT];
        out[14] = (int)(parameters[PROPERTY_GOBO_SELECT]);
        out[15] = (int)(parameters[PROPERTY_GOBO_INDEX_SPEED]) >> 8;
        out[16] = (int)(parameters[PROPERTY_GOBO_INDEX_SPEED]) & 0xff;
        out[17] = (int)(parameters[PROPERTY_GOBO_SELECT_2]);
        out[18] = (int)(parameters[PROPERTY_ANIMATION_SELECT]);
        out[19] = (int)(parameters[PROPERTY_ANIMATION_INDEX_SPEED]);
        out[20] = toByteCoarse(parameters[PROPERTY_SHUTTER_3_B]);
        out[21] = toByteFine(parameters[PROPERTY_SHUTTER_3_B]);
        out[22] = toByteCoarse(parameters[PROPERTY_SHUTTER_3_A]);
        out[23] = toByteFine(parameters[PROPERTY_SHUTTER_3_A]);
        out[24] = toByteCoarse(parameters[PROPERTY_SHUTTER_2_B]);
        out[25] = toByteFine(parameters[PROPERTY_SHUTTER_2_B]);
        out[26] = toByteCoarse(parameters[PROPERTY_SHUTTER_2_A]);
        out[27] = toByteFine(parameters[PROPERTY_SHUTTER_2_A]);
        out[28] = toByteCoarse(parameters[PROPERTY_SHUTTER_1_B]);
        out[29] = toByteFine(parameters[PROPERTY_SHUTTER_1_B]);
        out[30] = toByteCoarse(parameters[PROPERTY_SHUTTER_1_A]);
        out[31] = toByteFine(parameters[PROPERTY_SHUTTER_1_A]);
        out[32] = toByteCoarse(parameters[PROPERTY_SHUTTER_4_B]);
        out[33] = toByteFine(parameters[PROPERTY_SHUTTER_4_B]);
        out[34] = toByteCoarse(parameters[PROPERTY_SHUTTER_4_A]);
        out[35] = toByteFine(parameters[PROPERTY_SHUTTER_4_A]);
        out[36] = toByteCoarse(parameters[PROPERTY_SHUTTER_FRAME_ROT]);
        out[37] = toByteFine(parameters[PROPERTY_SHUTTER_FRAME_ROT]);
        out[38] = toByteCoarse(parameters[PROPERTY_EDGE]);
        out[39] = toByteFine(parameters[PROPERTY_EDGE]);
        out[40] = (int)(parameters[PROPERTY_EDGE_MODE]);
        out[41] = toByteCoarse(parameters[PROPERTY_ZOOM]);
        out[42] = toByteFine(parameters[PROPERTY_ZOOM]);
        out[43] = (int)(parameters[PROPERTY_BEAM_FX_SELECT]);
        out[44] = (int)(parameters[PROPERTY_BEAM_FX_INDEX_SPEED]);
        out[45] = (int)(parameters[PROPERTY_BEAM_FX_SELECT_2]);
        out[46] = (int)(parameters[PROPERTY_BEAM_FX_INDEX_SPEED_2]);
        out[47] = (int)(parameters[PROPERTY_IRIS]);
        out[48] = toByte(parameters[PROPERTY_DIFFUSION]);
        out[49] = toByte(parameters[PROPERTY_DIFFUSION_2]);
        out[50] = (int)(parameters[PROPERTY_CRI]);
        out[51] = (int)(parameters[PROPERTY_COLOR_MIX]);
        out[52] = toByte(parameters[PROPERTY_COLOR_MIX_SPEED]);
        out[53] = 0x0; // control
        return out;
    }

    virtual void input(TArray<int> dmx, TMap<FName, double> parameters) override
    {
        parameters[PROPERTY_PAN] = fromByte16b(dmx[0], dmx[1]);
        parameters[PROPERTY_TILT] = fromByte16b(dmx[2], dmx[3]);
        parameters[PROPERTY_POSITION_MSPEED] = fromByte(dmx[4]);
        parameters[PROPERTY_INTENSITY] = fromByte16b(dmx[5], dmx[6]);
        parameters[PROPERTY_STROBE] = dmx[7];
        // parameters[PROPERTY_STROBE] = dmx[8]; // virtual strobe
        parameters[PROPERTY_CYAN] = fromByte(dmx[9]);
        parameters[PROPERTY_MAGENTA] = fromByte(dmx[10]);
        parameters[PROPERTY_YELLOW] = fromByte(dmx[11]);
        parameters[PROPERTY_CT] = fromByte(dmx[12]);
        parameters[PROPERTY_COLOR_SELECT] = dmx[13];
        parameters[PROPERTY_GOBO_SELECT] = dmx[14];
        parameters[PROPERTY_GOBO_INDEX_SPEED] = (dmx[15] << 8) | dmx[16];
        parameters[PROPERTY_GOBO_SELECT_2] = dmx[17];
        parameters[PROPERTY_ANIMATION_SELECT] = dmx[18];
        parameters[PROPERTY_ANIMATION_INDEX_SPEED] = dmx[19];
        parameters[PROPERTY_SHUTTER_3_B] = fromByte16b(dmx[20], dmx[21]);
        parameters[PROPERTY_SHUTTER_3_A] = fromByte16b(dmx[22], dmx[23]);
        parameters[PROPERTY_SHUTTER_2_B] = fromByte16b(dmx[24], dmx[25]);
        parameters[PROPERTY_SHUTTER_2_A] = fromByte16b(dmx[26], dmx[27]);
        parameters[PROPERTY_SHUTTER_1_B] = fromByte16b(dmx[28], dmx[29]);
        parameters[PROPERTY_SHUTTER_1_A] = fromByte16b(dmx[30], dmx[31]);
        parameters[PROPERTY_SHUTTER_4_B] = fromByte16b(dmx[32], dmx[33]);
        parameters[PROPERTY_SHUTTER_4_A] = fromByte16b(dmx[34], dmx[35]);
        parameters[PROPERTY_SHUTTER_FRAME_ROT] = fromByte16b(dmx[36], dmx[37]);
        parameters[PROPERTY_EDGE] = fromByte16b(dmx[38], dmx[39]);
        parameters[PROPERTY_EDGE_MODE] = dmx[40];
        parameters[PROPERTY_ZOOM] = fromByte16b(dmx[41], dmx[42]);
        parameters[PROPERTY_BEAM_FX_SELECT] = dmx[43];
        parameters[PROPERTY_BEAM_FX_INDEX_SPEED] = dmx[44];
        parameters[PROPERTY_BEAM_FX_SELECT_2] = dmx[45];
        parameters[PROPERTY_BEAM_FX_INDEX_SPEED_2] = dmx[46];
        parameters[PROPERTY_IRIS] = dmx[47];
        parameters[PROPERTY_DIFFUSION] = fromByte(dmx[48]);
        parameters[PROPERTY_DIFFUSION_2] = fromByte(dmx[49]);
        parameters[PROPERTY_CRI] = dmx[50];
        parameters[PROPERTY_COLOR_MIX] = dmx[51];
        parameters[PROPERTY_COLOR_MIX_SPEED] = fromByte(dmx[52]);
        // out[53] = 0x0; // control
    } 
};

class CONCEPTTHEATERSIM_API MaverickMk3WashBasic : public EOSLightOutputType
{
    virtual TArray<int> output(TMap<FName, double> parameters) override
    {
        TArray<int> out;
        out.Init(0, 21);
        out[0] = toByteCoarse(parameters[PROPERTY_PAN]);
        out[1] = toByteFine(parameters[PROPERTY_PAN]);
        out[2] = toByteCoarse(parameters[PROPERTY_TILT]);
        out[3] = toByteFine(parameters[PROPERTY_TILT]);
        out[4] = toByte(parameters[PROPERTY_POSITION_MSPEED]);
        out[5] = (int)(parameters[PROPERTY_CT]);
        out[6] = (int)(parameters[PROPERTY_COLOR_MIX]);
        out[7] = (int)(parameters[PROPERTY_GOBO_SELECT]);
        out[8] = (int)(parameters[PROPERTY_PIXEL_MASK_2]);
        out[9] = (int)(parameters[PROPERTY_PIXEL_MASK_EFFECT_TIME]);
        out[10] = (int)(parameters[PROPERTY_PIXEL_MASK_EFFECT_STEP_TIME]);
        out[11] = (int)(parameters[PROPERTY_BACKGROUND_COLOR_MIX]);
        out[12] = toByte(parameters[PROPERTY_BACKGROUND_INTENSITY]);
        out[13] = toByte(parameters[PROPERTY_INTENSITY]);
        out[14] = (int)(parameters[PROPERTY_STROBE]);
        out[15] = toByte(parameters[PROPERTY_ZOOM]);
        out[16] = 0x0; // control
        out[17] = toByte(parameters[PROPERTY_RED]);
        out[18] = toByte(parameters[PROPERTY_GREEN]);
        out[19] = toByte(parameters[PROPERTY_BLUE]);
        out[20] = toByte(parameters[PROPERTY_WHITE]);
        return out;
    }

    virtual void input(TArray<int> dmx, TMap<FName, double> parameters) override
    {
        parameters[PROPERTY_PAN] = fromByte16b(dmx[0], dmx[1]);
        parameters[PROPERTY_TILT] = fromByte16b(dmx[2], dmx[3]);
        parameters[PROPERTY_POSITION_MSPEED] = fromByte(dmx[4]);
        parameters[PROPERTY_CT] = dmx[5];
        parameters[PROPERTY_COLOR_MIX] = dmx[6];
        parameters[PROPERTY_GOBO_SELECT] = dmx[7];
        parameters[PROPERTY_PIXEL_MASK_2] = dmx[8];
        parameters[PROPERTY_PIXEL_MASK_EFFECT_TIME] = dmx[9];
        parameters[PROPERTY_PIXEL_MASK_EFFECT_STEP_TIME] = dmx[10];
        parameters[PROPERTY_BACKGROUND_COLOR_MIX] = dmx[11];
        parameters[PROPERTY_BACKGROUND_INTENSITY] = fromByte(dmx[12]);
        parameters[PROPERTY_INTENSITY] = fromByte(dmx[13]);
        parameters[PROPERTY_STROBE] = dmx[14];
        parameters[PROPERTY_ZOOM] = fromByte(dmx[15]);
        // out[16] = 0x0; // control
        parameters[PROPERTY_RED] = fromByte(dmx[17]);
        parameters[PROPERTY_GREEN] = fromByte(dmx[18]);
        parameters[PROPERTY_BLUE] = fromByte(dmx[19]);
        parameters[PROPERTY_WHITE] = fromByte(dmx[20]);
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
    else if(type == LIGHT_TYPE_ETC_S4_LUSTR_P_DIRECT || type == LIGHT_TYPE_ETC_D60_DIRECT)
    {
        typeI = new ETCLustrDirect();
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