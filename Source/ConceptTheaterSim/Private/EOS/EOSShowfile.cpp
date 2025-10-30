// Copyright Peter Crall 2025

#include "EOS/EOSShowfile.h"

UEOSShowfile::UEOSShowfile()
{

}
UEOSShowfile::~UEOSShowfile()
{

}

bool UEOSShowfile::patchLight(int ch, UEOSPatch* light)
{
    UEOSPatchSet **p = patch.Find(ch);
    UEOSPatchSet *set = nullptr;
    if(!p)
    {
        UEOSPatchSet *s = NewObject<UEOSPatchSet>();
        s->channel = ch;
        set = s;
        patch.Add(ch, s);
        UEOSPropertySet* propSet = UEOSPropertySet::Create();
        for(FName &name : light->properties)
        {
            propSet->add(name, 0);
        }
        channels.Add(ch, propSet);
    }
    else
    {
        set = *p;
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
    if(UEOSPropertySet** set = channels.Find(ch))
    {
        return (*set)->get(parameter);
    }
    return 0;
}