// Fill out your copyright notice in the Description page of Project Settings.


#include "RawMidiOutput.h"

RtMidi::Api chooseMidiApi()
{
    std::vector< RtMidi::Api > apis;
    RtMidi::getCompiledApi(apis);

    if (apis.size() <= 1)
        return RtMidi::Api::UNSPECIFIED;

    return static_cast<RtMidi::Api>(0);
}

void URawMidiOutput::setup(int device) {
    

    try {
        midiOut = new RtMidiOut(chooseMidiApi());
        midiOut->openPort( device );
    } catch (RtMidiError &error) {
        UE_LOG(LogTemp, Fatal, TEXT("Error seting up raw Midi"));
        error.printMessage();
    }

    isSetup = true;
}

void URawMidiOutput::sendFullFrame(int framerate, int hour, int minute, int second, int frame){
    if(!isSetup)
        return;
    std::vector<unsigned char> message;
    message.push_back(0xf0); // System exclusive
    message.push_back(0x7f); // ??
    message.push_back(0x7f); // ??
    message.push_back(0x01); // ??
    message.push_back(0x01); // ??
    message.push_back((framerate << 5) | (hour & 0b0001111));
    message.push_back(minute & 0b00111111);
    message.push_back(second & 0b00111111);
    message.push_back(frame & 0b00111111);
    message.push_back(0xf7); // Special System Exclusive
    this->sendMessage(message);
}

void URawMidiOutput::sendQuarterFrame(int qf, int framerate, int hour, int minute, int second, int frame){
    if(!isSetup) return;
    std::vector<unsigned char> message;
    message.push_back(0xf1); // MTC
    unsigned char data;
    if(qf == 0) {
        data = frame & 0x0f;
    } else if (qf == 1) {
        data = (frame & 0x10) >> 4;
    } else if(qf == 2) {
        data = second & 0x0f;
    } else if (qf == 3) {
        data = (second & 0x30) >> 4;
    } else if(qf == 4) {
        data = minute & 0x0f;
    } else if (qf == 5) {
        data = (minute & 0x30) >> 4;
    } else if(qf == 6) {
        data = hour & 0x0f;
    } else if (qf == 3) {
        data = ((hour & 0x10) >> 4) | (framerate << 1);
    }
    message.push_back(data);
    message.push_back(0x00); // unused
    this->sendMessage(message);
    // 0x00
}

void URawMidiOutput::sendMessage(std::vector<unsigned char> message) {
    try {
        midiOut->sendMessage(&message);
    } catch (RtMidiError &error) {
        UE_LOG(LogTemp, Fatal, TEXT("Error sending Midi Message"));
        error.printMessage();
    }
}