// Copyright Peter Crall 2025.

#pragma once

// #include "midi/RtMidi.h"
// #include <cstdlib>
// #include <iostream>

// #define WINDOWS_MM 1

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RawMidiOutput.generated.h"

#define MTC_24FPS 0b00 
#define MTC_25FPS 0b01
#define MTC_29FPS 0b10
#define MTC_30FPS 0b11

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API URawMidiOutput : public UObject
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    bool setup(const int32 device);
    
    UFUNCTION(BlueprintCallable)
    void close();

    UFUNCTION(BlueprintCallable)
    void sendFullFrame(int framerate, int hour, int minute, int second, int frame);
    
    UFUNCTION(BlueprintCallable)
    void sendFullFrameByFrames(int frames);

    UFUNCTION(BlueprintCallable)
    void sendQuarterFrame(int qf, int framerate, int hour, int minute, int second, int frame);

    UFUNCTION(BlueprintCallable)
    void sendQuarterFrameByFrames(int qf, int frames);

    UFUNCTION(BlueprintCallable)
    FString getDeviceName() { return deviceName; }

protected:

    virtual void BeginDestroy() override;

    // RtMidiOut* midiOut;
    UPROPERTY()
    bool isSetup;

    UPROPERTY()
    int32 deviceId = -1;

    void *pmMIDIStream = nullptr;

    UPROPERTY()
    FString deviceName;
};
