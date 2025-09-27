// Copyright Peter Crall 2025

#include "EOS/EOSShowfile.h"

UEOSShowfile::UEOSShowfile()
{

}
UEOSShowfile::~UEOSShowfile()
{

}

bool UEOSShowfile::patchLight(int ch, FEOSPatch light)
{
    FEOSPatchSet *set = patch.Find(ch);
    if(!set)
    {
        FEOSPatchSet s;
        set = &s;
        patch.Add(ch, s);
        FEOSPropertySet propSet;
        for(FName &name : light.properties)
        {
            propSet.add(name, 0);
        }
        channels.Add(ch, propSet);
    }
    else
    {
        if(!set->canAdd(light))
        {
            return false;
        }
    }
    set->devices.Add(light);
    return true;
}

TArray<int> UEOSShowfile::getPatchedChannels()
{
    TArray<int> chs;
    patch.GetKeys(chs);
    return chs;
}

double UEOSShowfile::getParameter(int ch, FName parameter)
{
    if(FEOSPropertySet* set = channels.Find(ch))
    {
        return set->get(parameter);
    }
    return 0;
}