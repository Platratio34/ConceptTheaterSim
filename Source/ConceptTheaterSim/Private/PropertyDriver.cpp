// Copyright Peter Crall 2025.


#include "PropertyDriver.h"

void UPropertyDriver::SetValue(float value_) {
    this->value = value_;
    done = false;
}

void UPropertyDriver::SetTarget(float target, float time_)
{
    this->targetValue = target;
    this->time = time_;
    done = false;
}

void UPropertyDriver::UpdateTarget(float target)
{
    this->targetValue = target;
    done = false;
}

float UPropertyDriver::Update(float deltaTime)
{
    float delta = targetValue - value;
    if(abs(delta) < 0.001) {
        value = targetValue;
        time = 0;
        done = true;
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
    move *= deltaTime;
    if(abs(move) > abs(delta))
        move = delta;
    time -= deltaTime;
    value += move;
    lastDelta = delta;
    done = abs(targetValue - value) < 0.001;
    if(done)
    {
        value = targetValue;
        time = 0;
    }
    return value;
}


bool UPropertyDriver::IsDone()
{
    return done;
}