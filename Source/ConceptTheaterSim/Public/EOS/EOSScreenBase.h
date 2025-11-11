// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EOS/ETCLightBoard.h"
#include "EOSScreenBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UEOSScreenBase : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    AETCLightBoard* board = nullptr;

protected:
    UFUNCTION(BlueprintCallable)
    FString getCommandString();

    UFUNCTION(BlueprintCallable)
    FLinearColor getCommandColor();

    UFUNCTION(BlueprintCallable)
    TArray<int> getPatchedChannels();

    UFUNCTION(BlueprintCallable)
    double getChannelParameter(int ch, FName parameter);

    UFUNCTION(BlueprintCallable)
    TArray<FName> getChannelParameters(int ch);

    UFUNCTION(BlueprintCallable)
    UEOSChannelView *getChannelView(int ch);

    UFUNCTION(BlueprintCallable)
    FString getCurrentCue();

    static TMap<FName, FString> getNamesToDisplay()
    {
        static bool namesToDisplayInit = false;
        static TMap<FName, FString> namesToDisplay;
        if(namesToDisplayInit)
            return namesToDisplay;
        namesToDisplayInit = true;

        namesToDisplay.Add(BUTTON_DOT, TEXT("."));
        namesToDisplay.Add(BUTTON_SLASH, TEXT("/"));

        namesToDisplay.Add(BUTTON_GO_TO_CUE, TEXT("Go To Cue"));

        namesToDisplay.Add(BUTTON_ADDRESS, TEXT("Address"));

        namesToDisplay.Add(BUTTON_BEAM_PALLET, TEXT("Beam Pallet"));
        namesToDisplay.Add(BUTTON_INT_PALLET, TEXT("Intensity Pallet"));
        namesToDisplay.Add(BUTTON_COLOR_PALLET, TEXT("Color Pallet"));
        namesToDisplay.Add(BUTTON_FOCUS_PALLET, TEXT("Focus Pallet"));
        
        namesToDisplay.Add(BUTTON_SELECT_ACTIVE, TEXT("Select Active"));
        namesToDisplay.Add(BUTTON_SELECT_LAST, TEXT("Select Last"));
        namesToDisplay.Add(BUTTON_SELECT_MANUAL, TEXT("Select Manual"));
        
        namesToDisplay.Add(BUTTON_RECORD_ONLY, TEXT("Record Only"));

        namesToDisplay.Add(BUTTON_RECALL_FROM, TEXT("Recall From"));
        namesToDisplay.Add(BUTTON_COPY_TO, TEXT("Copy To"));

        namesToDisplay.Add(BUTTON_HIGH, TEXT("Highlight"));
        
        namesToDisplay.Add(BUTTON_PLUS, TEXT("+"));
        namesToDisplay.Add(BUTTON_MINUS, TEXT("-"));
        
        namesToDisplay.Add(BUTTON_LABEL, TEXT("Label"));

        return namesToDisplay;
    }
};
