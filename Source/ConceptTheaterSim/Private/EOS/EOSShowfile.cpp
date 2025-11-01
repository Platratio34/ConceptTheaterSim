// Copyright Peter Crall 2025

#include "EOS/EOSShowfile.h"
#include "TimeUtil.h"
#include "EOS/EOSPatchTypes.h"

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

bool UEOSShowfile::loadFromJson(TSharedPtr<FJsonObject> showJson)
{
    if(!showJson->HasField(JSON_VERSION))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid show file: missing version"));
        return false;
    }
    double v = showJson->GetNumberField(JSON_VERSION);
    if(v != 1)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid show file: missing version"));
        return false;
    }
    if(showJson->HasField(JSON_PATCH))
    {
        TArray<TSharedPtr<FJsonValue>> patchJson = showJson->GetArrayField(JSON_PATCH);
        for(TSharedPtr<FJsonValue> lightJsonV : patchJson)
        {
            TSharedPtr<FJsonObject> lightJson = lightJsonV->AsObject();
            if(!lightJson->HasField(JSON_PATCH_CH))
            {
                UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid patch: Missing channel number"));
                return false;
            }
            int ch = (int)lightJson->GetNumberField(JSON_PATCH_CH);
            if(!lightJson->HasField(JSON_PATCH_TYPE))
            {
                UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid patch: Missing type (Chan %d)"), ch);
                return false;
            }
            UEOSPatch *p = EOSPatchTypes::create(FName(*(lightJson->GetStringField(JSON_PATCH_TYPE))));
            if(!lightJson->HasField(JSON_PATCH_UNIVERSE))
            {
                UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid patch: Missing DMX universe (Chan %d)"), ch);
                return false;
            }
            p->universe = (int)lightJson->GetNumberField(JSON_PATCH_UNIVERSE);
            if(!lightJson->HasField(JSON_PATCH_ADDRESS))
            {
                UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid patch: Missing DMX address (Chan %d)"), ch);
                return false;
            }
            p->address = (int)lightJson->GetNumberField(JSON_PATCH_UNIVERSE);
            if(!patchLight(ch, p))
            {
                UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid patch: Could not patch light (Chan %d)"), ch);
                return false;
            }
        }
    }
    if(showJson->HasField(JSON_CUES))
    {
        TArray<TSharedPtr<FJsonValue>> cuesJson = showJson->GetArrayField(JSON_CUES);
        for(TSharedPtr<FJsonValue> lightJsonV : cuesJson)
        {
            UEOSCue *cue = UEOSCue::FromJSON(lightJsonV->AsObject());
            if(cue == nullptr)
                return false;
            cues.Add(cue);
        }
    }
    return true;
}

UEOSCue* UEOSCue::FromJSON(TSharedPtr<FJsonObject> cueJson)
{
    UEOSCue *cue = NewObject<UEOSCue>();
    if(!cueJson->HasField(JSON_CUE))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid Cue: Missing cue number"));
        return nullptr;
    }
    cue->cueNumber = cueJson->GetNumberField(JSON_CUE);
    if(!cueJson->HasField(JSON_TIME))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid Cue: Missing cue time; (Cue %f)"), cue->cueNumber);
        return nullptr;
    }
    cue->time = cueJson->GetNumberField(JSON_TIME);
    if(cueJson->HasField(JSON_TIMECODE))
        cue->timecode = UTimeUtil::parseTimeStringFrames(cueJson->GetStringField(JSON_TIMECODE));
    if(cueJson->HasField(JSON_FOLLOW))
        cue->timecode = cueJson->GetNumberField(JSON_FOLLOW);
    if(cueJson->HasField(JSON_HANG))
        cue->timecode = cueJson->GetNumberField(JSON_HANG);
    if(!cueJson->HasField(JSON_TIME))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid Cue: Missing cue actions; (Cue %f)"), cue->cueNumber);
        return nullptr;
    }
    TArray<TSharedPtr<FJsonValue>> actionsJson = cueJson->GetArrayField(JSON_ACTIONS);
    for(TSharedPtr<FJsonValue> val : actionsJson)
    {
        TSharedPtr<FJsonObject> actionJson = val->AsObject();
        int ch = actionJson->GetNumberField(JSON_ACTIONS_CH);
        UEOSPropertySet *action = UEOSPropertySet::Create();
        for(const auto& pair : actionJson->Values)
        {
            if(pair.Key == JSON_ACTIONS_CH)
                continue;
            action->add(FName(*pair.Key), pair.Value->AsNumber());
        }
        cue->actions.Add(ch, action);
    }
    return cue;
}