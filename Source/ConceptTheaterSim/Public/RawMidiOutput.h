// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "midi/RtMidi.h"
#include <cstdlib>
#include <iostream>

#define WINDOWS_MM 1

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RawMidiOutput.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API URawMidiOutput : public UObject
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void setup(int device);

    UFUNCTION(BlueprintCallable)
    void sendFullFrame(int framerate, int hour, int minute, int second, int frame);

    UFUNCTION(BlueprintCallable)
    void sendQuarterFrame(int qf, int framerate, int hour, int minute, int second, int frame);

    void sendMessage(std::vector<unsigned char> message);

protected:

    RtMidiOut* midiOut;
    bool isSetup;
};
