// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyDriver.h"

void UPropertyDriver::SetValue(float value_) {
    this->value = value_;
}

void UPropertyDriver::SetTarget(float target, float time_)
{
    this->targetValue = target;
    this->time = time_;
}

float UPropertyDriver::Update(float deltaTime)
{
    float delta = targetValue - value;
    if(abs(delta) < 0.001) {
        value = targetValue;
        time = 0;
        return targetValue;
    }
    if(time <= 0) {
        time = deltaTime;
    }
    if(maxAccl > 0) {
        float accl = delta - lastDelta;
        if(accl > maxAccl) {
            delta = lastDelta + maxAccl;
        }
    }
    float remTime = time;
    if(decelTime > 0 && time < decelTime) {
        float invPrcRem = 1 - (remTime / decelTime);
        remTime *= 1 + invPrcRem;
    }
    float move = delta / remTime;
    time -= deltaTime;
    value += (move * deltaTime);
    lastDelta = delta;
    return value;
}


bool UPropertyDriver::IsDone()
{
    float delta = targetValue - value;
    return abs(delta) < 0.001;
}