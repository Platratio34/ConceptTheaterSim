// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Json.h"
#include "CAutomationShowFile.generated.h"

/*
JSON spec:

File: {
    cues: {
        Cue
        ...
    }
    metadata: {
        "[key]": "[value]"
    }
}

Cue: {
    time: [TimeString]
    duration: [seconds]
    properties: {
        "[device].[property]": [value]
        ...
    }
    subCues?: {
        {
            offset?: [seconds]
            duration?: [seconds]
            properties: {
                "[device].[property]": [value]
                ...
            }
        }
    }
}

TimeString: "[seconds]" | "[hour]:[minutes]:[seconds]:[frames]"

*/

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FAutomationPropertySet
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Automation")
    TMap<FName, double> properties;

    static TMap<FName, FAutomationPropertySet> ParseJSON(TSharedPtr<FJsonObject> properties);
};

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FAutomationSubCue
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Automation")
    double offset = 0;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Automation")
    double duration = -1;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Automation")
    TMap<FName, FAutomationPropertySet> properties;

    static FAutomationSubCue ParseJSON(TSharedPtr<FJsonObject> subCueJSON);
};

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FAutomationCue
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Automation")
    double time;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Automation")
    double duration;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Automation")
    TMap<FName, FAutomationPropertySet> properties;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Automation")
    TArray<FAutomationSubCue> subCues;

    static FAutomationCue ParseJSON(TSharedPtr<FJsonObject> cueJSON);
};