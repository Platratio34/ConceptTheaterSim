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
            if(double* p2 = light->defaultValues.Find(name))
                propSet->add(name, *p2);
            else
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
    UEOSChannelView *view = getChannel(ch);
    if(view != nullptr)
    {
        return view->getProperty(parameter);
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
            p->address = (int)lightJson->GetNumberField(JSON_PATCH_ADDRESS);
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
    if(showJson->HasField(JSON_CURRENT_CUE))
        currentCue = showJson->GetNumberField(JSON_CURRENT_CUE);
    return true;
}

TSharedPtr<FJsonObject> UEOSShowfile::toJson()
{
    TSharedPtr<FJsonObject> showJson = MakeShared<FJsonObject>();
    showJson->SetNumberField(JSON_VERSION, 1);

    TArray<TSharedPtr<FJsonValue>> patchJson;
    for(TPair<int, UEOSPatchSet*> patchPair : patch)
    {
        int ch = patchPair.Key;
        for (UEOSPatch *device : patchPair.Value->devices)
        {
            TSharedPtr<FJsonObject> deviceJson = MakeShared<FJsonObject>();
            deviceJson->SetNumberField(JSON_PATCH_CH, ch);
            deviceJson->SetStringField(JSON_PATCH_TYPE, device->type.ToString());
            deviceJson->SetNumberField(JSON_PATCH_UNIVERSE, device->universe);
            deviceJson->SetNumberField(JSON_PATCH_ADDRESS, device->address);
            patchJson.Add(MakeShared<FJsonValueObject>(deviceJson));
        }
    }
    showJson->SetArrayField(JSON_PATCH, patchJson);

    TArray<TSharedPtr<FJsonValue>> cuesJson;
    for(UEOSCue* cue : cues)
    {
        cuesJson.Add(MakeShared<FJsonValueObject>(cue->toJson()));
    }
    showJson->SetArrayField(JSON_CUES, cuesJson);
    showJson->SetNumberField(JSON_CURRENT_CUE, currentCue);

    return showJson;
}

void UEOSShowfile::addFade(int channel, FName property, double target, double time, bool manual, bool sneak)
{
    if(!channels.Contains(channel))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid channel %d in fade add, skipping"), channel);
        return;
    }
    if(fades.Num() < 200) // first time init to prevent repeat allocation
        fades.Init(nullptr, 200);
    int firstNull = -1;
    for (int i = 0; i < fades.Num(); i++)
    {
        UEOSFade *f = fades[i];
        if(f == nullptr)
        {
            firstNull = i;
            continue;
        }
        if(f->channel == channel && f->property == property && f->manual == manual)
        {
            f->target = target;
            f->time = time;
            f->sneak = sneak;
            return;
        }
    }
    UEOSFade *fade = NewObject<UEOSFade>();
    fade->channel = channel;
    fade->property = property;
    fade->target = target;
    fade->time = time;
    fade->manual = manual;
    fade->sneak = sneak;
    if(firstNull != -1)
    {
        fades[firstNull] = fade;
    }
    else
    {
        fades.Add(fade);
    }
}

void UEOSShowfile::updateFades(double deltaTime)
{
    for (int i = 0; i < fades.Num(); i++)
    {
        UEOSFade *fade = fades[i];
        if(fade == nullptr)
            continue;
        if(fade->update(this, deltaTime))
            fades[i] = nullptr;
    }
}

void UEOSShowfile::clearFade(int channel, FName property, bool manual)
{
    for (int i = 0; i < fades.Num(); i++)
    {
        UEOSFade *fade = fades[i];
        if(fade == nullptr)
            continue;
        if(fade->channel == channel && fade->property == property && fade->manual == manual)
        {
            fades[i] = nullptr;
            return;
        }
    }
}

UEOSChannelView* UEOSShowfile::getChannel(int ch)
{
    if(!patch.Contains(ch))
        return nullptr;
    if(UEOSChannelView** p = channelViews.Find(ch))
    {
        return *p;
    }
    UEOSChannelView *view = UEOSChannelView::create(this, ch);
    channelViews.Add(ch, view);
    return view;
}

void UEOSShowfile::setManualProperty(int ch, FName property, float value)
{
    if(!patch.Contains(ch))
        return;
    if(!manualChannels.Contains(ch))
        manualChannels.Add(ch, UEOSPropertySet::Create());
    manualChannels[ch]->add(property, value);
    clearFade(ch, property, true);
}
void UEOSShowfile::setManualProperties(int ch, UEOSPropertySet* properties)
{
    if(!patch.Contains(ch))
        return;
    if(!manualChannels.Contains(ch))
        manualChannels.Add(ch, UEOSPropertySet::Create());
    for(TPair<FName, float> pair : properties->properties)
    {
        manualChannels[ch]->add(pair.Key, pair.Value);
    }
    for (int i = 0; i < fades.Num(); i++)
    {
        UEOSFade *fade = fades[i];
        if(fade != nullptr && fade->channel == ch && fade->manual)
        {
            if(properties->has(fade->property))
                fades[i] = nullptr;
        }
    }
}
void UEOSShowfile::clearManualProperty(int ch, FName property, bool sneak)
{
    if(!manualChannels.Contains(ch))
        return;
    manualChannels[ch]->remove(property);
    if(sneak)
    {
        addFade(ch, property, 0, 5, true, true);
    }
    else
    {
        clearFade(ch, property, true);
    }
}

void UEOSShowfile::recordOnly(int cueI, TArray<int> *sel)
{
    UEOSCue *cue = cues[cueI];
    if(sel != nullptr)
    {
        for (int ch : *sel)
        {
            if(!manualChannels.Contains(ch))
                continue;
            UEOSPropertySet *set = nullptr;
            if(cue->actions.Contains(ch))
                set = cue->actions[ch];
            else
                set = UEOSPropertySet::Create();
            
            set->addSet(manualChannels[ch]);
            manualChannels.Remove(ch);
        }
    }
    else
    {
        for (TPair<int, UEOSPropertySet *> pair : manualChannels)
        {
            UEOSPropertySet *set = nullptr;
            if(cue->actions.Contains(pair.Key))
                set = cue->actions[pair.Key];
            else
                set = UEOSPropertySet::Create();
            set->addSet(pair.Value);
            manualChannels.Remove(pair.Key);
        }
    }
}

void UEOSShowfile::setCueProperty(int ch, FName property, float value)
{
    if(!channels.Contains(ch))
        return;
    channels[ch]->set(property, value);
    clearFade(ch, property, false);
}
void UEOSShowfile::setCueProperties(int ch, UEOSPropertySet* properties)
{
    if(!channels.Contains(ch))
        channels.Add(ch, UEOSPropertySet::Create());
    for(TPair<FName, float> pair : properties->properties)
    {
        channels[ch]->set(pair.Key, pair.Value);
    }
    for (int i = 0; i < fades.Num(); i++)
    {
        UEOSFade *fade = fades[i];
        if(fade != nullptr && fade->channel == ch && !fade->manual)
        {
            if(properties->has(fade->property))
                fades[i] = nullptr;
        }
    }
}

UEOSPropertySet *UEOSShowfile::getCueChannel(int ch)
{
    if(!channels.Contains(ch))
        return nullptr;
    return channels[ch];
}
UEOSPropertySet *UEOSShowfile::getManualChannel(int ch)
{
    if(!manualChannels.Contains(ch))
        return nullptr;
    return manualChannels[ch];
}

float UEOSChannelView::getProperty(FName property)
{
    if(UEOSPropertySet* set = showfile->getManualChannel(channel))
    {
        if(set->has(property))
        {
            return set->get(property);
        }
    }
    return showfile->getCueChannel(channel)->get(property);
}
FName UEOSChannelView::propertySource(FName property)
{
    if(UEOSPropertySet* set = showfile->getManualChannel(channel))
    {
        if(set->has(property))
        {
            return FName("Manual");
        }
    }
    return FName("Cue");
}
void UEOSChannelView::getKeys(TArray<FName> outKeys)
{
    showfile->getCueChannel(channel)->properties.GetKeys(outKeys);
}

UEOSCue* UEOSCue::FromJSON(TSharedPtr<FJsonObject> cueJson)
{
    UEOSCue *cue = NewObject<UEOSCue>();
    if(!cueJson->HasField(JSON_CUE))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid Cue: Missing cue number"));
        return nullptr;
    }
    cue->cueNumber = cueJson->GetStringField(JSON_CUE);
    if(!cueJson->HasField(JSON_TIME))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid Cue: Missing cue time; (Cue %s)"), *(cue->cueNumber));
        return nullptr;
    }
    cue->time = cueJson->GetNumberField(JSON_TIME);
    if(cueJson->HasField(JSON_TIMECODE))
        cue->timecode = UTimeUtil::parseTimeStringFrames(cueJson->GetStringField(JSON_TIMECODE));
    if(cueJson->HasField(JSON_FOLLOW))
        cue->follow = cueJson->GetNumberField(JSON_FOLLOW);
    if(cueJson->HasField(JSON_HANG))
        cue->hang = cueJson->GetNumberField(JSON_HANG);
    if(!cueJson->HasField(JSON_TIME))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid show file: Invalid Cue: Missing cue actions; (Cue %s)"), *(cue->cueNumber));
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

TSharedPtr<FJsonObject> UEOSCue::toJson()
{
    TSharedPtr<FJsonObject> cueJson = MakeShared<FJsonObject>();
    cueJson->SetStringField(JSON_CUE, cueNumber);
    cueJson->SetNumberField(JSON_TIME, time);
    if(hang >= 0)
        cueJson->SetNumberField(JSON_HANG, hang);
    if(follow >= 0)
        cueJson->SetNumberField(JSON_FOLLOW, follow);
    if(timecode >= 0)
        cueJson->SetStringField(JSON_TIMECODE, UTimeUtil::createTimeStringFromFrames(timecode));
    TArray<TSharedPtr<FJsonValue>> actionsJson;
    for(TPair<int, UEOSPropertySet*> pair : actions)
    {
        TSharedPtr<FJsonObject> chJson = MakeShared<FJsonObject>();
        chJson->SetNumberField(JSON_ACTIONS_CH, pair.Key);
        for(TPair<FName, double> propertyPair : pair.Value->properties)
        {
            chJson->SetNumberField(propertyPair.Key.ToString(), propertyPair.Value);
        }
        actionsJson.Add(MakeShared<FJsonValueObject>(chJson));
    }
    cueJson->SetArrayField(JSON_ACTIONS, actionsJson);
    return cueJson;
}

bool UEOSFade::update(UEOSShowfile* showfile, double deltaTime)
{
    UEOSPropertySet *set = showfile->getCueChannel(channel);
    if(manual)
    {
        if(sneak)
            target = set->get(property);
        set = showfile->getManualChannel(channel);
    }
    double value = set->get(property);
    double diff = target - value;
    if(time < deltaTime)
        time = deltaTime;
    double delta = (diff / time) * deltaTime;
    time -= deltaTime;
    if(abs(diff) < abs(delta) || time <= 0.01)
    {
        if(manual && sneak)
        {
            set->remove(property);
        }
        else
        {
            set->set(property, target);
        }
        return true;
    }
    set->set(property, value + delta);
    return false;
}