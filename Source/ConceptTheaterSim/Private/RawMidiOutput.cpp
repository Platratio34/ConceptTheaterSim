// Copyright Peter Crall 2025.


#include "RawMidiOutput.h"
#include "portmidi.h"

#define FRAMERATE_30FPS 0b110
#define FRAMES_TO_SECONDS 30
#define FRAMES_TO_MINUTES 1800
#define FRAMES_TO_HOURS 108000

static FString ParsePmError(const PmError& InError) {
    FString ErrorText = ANSI_TO_TCHAR(Pm_GetErrorText(InError));
    if(InError == pmHostError) {
        char ErrorTextBuffer[1024];
        Pm_GetHostErrorText(ErrorTextBuffer, 1024);
        ErrorText = ANSI_TO_TCHAR(ErrorTextBuffer);
    }
    return ErrorText;
}

bool URawMidiOutput::setup(const int32 device) {
    if(isSetup)
        return deviceId == device;
    if(device < 0) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to bind to MIDI device: ID was invalid"));
        return false;
    }
    deviceId = device;
    UE_LOG(LogTemp, Display, TEXT("Attempting to bind to MIDI device %i"), deviceId);
    const PmDeviceID pmDeviceId = deviceId;
    const PmDeviceInfo *pmDeviceInfo = Pm_GetDeviceInfo(pmDeviceId);
    if(pmDeviceInfo != nullptr) {
        if(pmDeviceInfo->opened != 0) {
            UE_LOG(LogTemp, Warning, TEXT("Failed to bind to MIDI device '%s' (ID: %i): Device is already in use"), ANSI_TO_TCHAR(pmDeviceInfo->name), pmDeviceId);
            return false;
        }

        if(pmDeviceInfo->output == 0) {
            UE_LOG(LogTemp, Warning, TEXT("Failed to bind to MIDI device '%s' (ID: %i): Device not setup to receive MIDI"), ANSI_TO_TCHAR(pmDeviceInfo->name), pmDeviceId);
            return false;
        }

        const PmError pmError = Pm_OpenOutput(&pmMIDIStream, pmDeviceId, nullptr, 1, nullptr, nullptr, 0);
        if(pmError == pmNoError) {
            check(pmMIDIStream != nullptr);
            deviceName = ANSI_TO_TCHAR(pmDeviceInfo->name);
        } else {
			pmMIDIStream = nullptr;
			const FString errorText = ParsePmError(pmError);
			UE_LOG(LogTemp, Error, TEXT("Unable to open output connection to MIDI device ID %i (%s) (PortMidi error: %s)."), pmDeviceId, ANSI_TO_TCHAR(pmDeviceInfo->name), *errorText);
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("Unable to query information about MIDI device (PortMidi device ID: %i)."), pmDeviceId);
	}
    UE_LOG(LogTemp, Display, TEXT("Bound to MIDI device '%s' (ID: %i)"), ANSI_TO_TCHAR(pmDeviceInfo->name), deviceId);

    isSetup = true;
    return true;
}

void URawMidiOutput::close() {
    if(!isSetup || pmMIDIStream == nullptr)
        return;

    const PmError pmError = Pm_Close(pmMIDIStream);
    if (pmError != pmNoError)
    {
        const FString errorText = ParsePmError(pmError);
        UE_LOG(LogTemp, Error, TEXT("Encounter an error when closing the output connection to MIDI device ID %i (%s) (PortMidi error: %s)."), deviceId, *deviceName, *errorText);
    }
    pmMIDIStream = nullptr;

    isSetup = false;
}

void URawMidiOutput::BeginDestroy() {
    close();
    Super::BeginDestroy();
}

void URawMidiOutput::sendFullFrame(int framerate, int hour, int minute, int second, int frame) {
    if(!isSetup) return;
    unsigned char msg[10];
    msg[0] = 0xf0;     // System exclusive
    msg[1] = 0x7f; // Manufacturer: real-time universal message
    msg[2] = 0x7f; // Channel: Global broadcast
    msg[3] = 0x01; // Type: Timecode
    msg[4] = 0x01; // Full frame
    msg[5] = (framerate << 5) | (hour & 0b0001111);
    msg[6] = minute & 0b00111111;
    msg[7] = second & 0b00111111;
    msg[8] = frame & 0b00111111;
    msg[9] = 0xf7; // Special System Exclusive

    Pm_WriteSysEx(pmMIDIStream, 0, msg);
}

void URawMidiOutput::sendFullFrameByFrames(int frames) {
    if(!isSetup) return;
    unsigned char msg[10];
    msg[0] = 0xf0;     // System exclusive
    msg[1] = 0x7f; // Manufacturer: real-time universal message
    msg[2] = 0x7f; // Channel: Global broadcast
    msg[3] = 0x01; // Type: Timecode
    msg[4] = 0x01; // Full frame
    msg[5] = (FRAMERATE_30FPS << 4) | ((frames / FRAMES_TO_HOURS) & 0b0001111);
    msg[6] = (frames / FRAMES_TO_MINUTES) & 0b00111111;
    msg[7] = (frames / FRAMES_TO_SECONDS) & 0b00111111;
    msg[8] = (frames % 30) & 0b00111111;
    msg[9] = 0xf7; // Special System Exclusive

    Pm_WriteSysEx(pmMIDIStream, 0, msg);
}

void URawMidiOutput::sendQuarterFrame(int qf, int framerate, int hour, int minute, int second, int frame){
    if(!isSetup) return;
    unsigned char data = 0;
    switch (qf) {
    case 0:
        data = frame & 0x0f;
        break;
    case 1:
        data = (frame & 0x10) >> 4;
        break;
    case 2:
        data = second & 0x0f;
        break;
    case 3:
        data = (second & 0x30) >> 4;
        break;
    case 4:
        data = minute & 0x0f;
        break;
    case 5:
        data = (minute & 0x30) >> 4;
        break;
    case 6:
        data = hour & 0x0f;
        break;
    case 7:
        data = ((hour & 0x10) >> 4) | (framerate << 1);
        break;
    default:
        break;
    }
    unsigned char msg[2];
    msg[0] = 0xf1;
    msg[1] = (qf << 4) | data;
    Pm_WriteSysEx(pmMIDIStream, 0, msg);
    // msg = (msg << 8) | 0xf1;
    // Pm_WriteShort(pmMIDIStream, 0, msg);
}

void URawMidiOutput::sendQuarterFrameByFrames(int qf, int frames){
    if(!isSetup) return;
    unsigned char data = 0;
    switch (qf) {
    case 0:
        data = (frames % 30) & 0x0f;
        break;
    case 1:
        data = ((frames % 30) & 0x10) >> 4;
        break;
    case 2:
        data = (frames / FRAMES_TO_SECONDS) & 0x0f;
        break;
    case 3:
        data = ((frames / FRAMES_TO_SECONDS) & 0x30) >> 4;
        break;
    case 4:
        data = (frames / FRAMES_TO_MINUTES) & 0x0f;
        break;
    case 5:
        data = ((frames / FRAMES_TO_MINUTES) & 0x30) >> 4;
        break;
    case 6:
        data = (frames / FRAMES_TO_HOURS) & 0x0f;
        break;
    case 7:
        data = (((frames / FRAMES_TO_HOURS) & 0x10) >> 4) | FRAMERATE_30FPS;
        break;
    default:
        break;
    }
    unsigned char msg[2];
    msg[0] = 0xf1;
    msg[1] = (qf << 4) | data;
    Pm_WriteSysEx(pmMIDIStream, 0, msg);
    // msg = (msg << 8) | 0xf1;
    // Pm_WriteShort(pmMIDIStream, 0, msg);
}