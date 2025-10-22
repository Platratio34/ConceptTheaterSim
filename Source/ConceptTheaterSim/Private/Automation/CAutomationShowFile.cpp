// Copyright Peter Crall 2025.

#include "Automation/CAutomationShowFile.h"
#include "Automation/CAutomationProcessor.h"
#include "TimeUtil.h"

TMap<FName, FAutomationPropertySet> FAutomationPropertySet::ParseJSON(TSharedPtr<FJsonObject> properties)
{
    TMap<FName, FAutomationPropertySet> sets;
    TArray<FString> keys;
    properties->Values.GetKeys(keys);
    TArray<FString> parts;
    for(const FString& key : keys)
    {
        int i = key.Find(TEXT("."));
        if(i <= 0)
        {
            UE_LOG(AutomationLog, Warning, TEXT("Could not parse device ID from property in file: `%s`; skipping it"), *key);
            continue;
        }
        FName deviceID = FName(*key.Left(i));
        FName propertyID = FName(*key.Mid(i + 1, key.Len() - i - 1));
        if(!sets.Contains(deviceID))
        {
            FAutomationPropertySet newSet;
            sets.Add(deviceID, newSet);
        }
        sets[deviceID].properties.Add(propertyID, properties->GetNumberField(key));
    }
    return sets;
}

FAutomationSubCue FAutomationSubCue::ParseJSON(TSharedPtr<FJsonObject> subCueJSON)
{
    FAutomationSubCue subCue;
    subCue.offset = subCueJSON->GetNumberField(TEXT("offset"));
    subCue.duration = subCueJSON->GetNumberField(TEXT("duration"));
    if(subCueJSON->HasField(TEXT("properties")))
    {
        subCue.properties = FAutomationPropertySet::ParseJSON(subCueJSON->GetObjectField(TEXT("properties")));
    }
    return subCue;
}

FAutomationCue FAutomationCue::ParseJSON(TSharedPtr<FJsonObject> cueJSON)
{
    FAutomationCue cue;
    cue.time = UTimeUtil::parseTimeString(cueJSON->GetStringField(TEXT("time")));
    cue.duration = cueJSON->GetNumberField(TEXT("duration"));
    if(cueJSON->HasField(TEXT("properties")))
    {
        cue.properties = FAutomationPropertySet::ParseJSON(cueJSON->GetObjectField(TEXT("properties")));
    }
    if(cueJSON->HasField(TEXT("subCues")))
    {
        TArray<TSharedPtr<FJsonValue>> subCuesJSON = cueJSON->GetArrayField(TEXT("subCues"));
        for (int i = 0; i < subCuesJSON.Num(); i++)
        {
            cue.subCues.Add(FAutomationSubCue::ParseJSON(subCuesJSON[i]->AsObject()));
        }
    }
    return cue;
}