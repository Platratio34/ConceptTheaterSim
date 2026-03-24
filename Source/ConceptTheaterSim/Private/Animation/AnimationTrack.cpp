// Copyright Peter Crall 2025.

#include "Animation/AnimationTrack.h"
#include "Json.h"


double UAnimationTrackPropertyTrack::getValue(int frame) {
    double lastV = 0;
    int lastF = 0;
    for(int i = 0; i < keyframes.Num(); i++) {
        const FAnimationTrackPropertyKeyframe &keyframe = keyframes[i];
        // UE_LOG(LogTemp, Display, TEXT("Keyframe #%d at: %d"), i, keyframe.frame);
        if(keyframe.frame == frame) {
            return keyframe.block ? lastV : keyframe.value;
        } else if(keyframe.frame < frame) {
            if(keyframe.block) {
                lastF = keyframe.frame;
                continue;
            }
            lastV = keyframe.value;
            lastF = keyframe.frame;
        } else { // the first keyframe in the future
            if(keyframe.block) {
                return lastV;
            }
            double dT = (double)(keyframe.frame - lastF);
            double t = ((double)(frame - lastF)) / dT;
            return lastV + ((keyframe.value - lastV) * t);
        }
    }
    return lastV;
}

double UAnimationTrackPropertyTrack::getValueTracked(int frame) {
    bool reset = lastFrame > frame;
    if(reset) { // we went back
        lastKeyframe = 0;
    }
    reset |= (lastFrame + 1 < frame);
    lastFrame = frame;
    double lastV = 0;
    int lastF = 0;
    blocked = false;
    for (int i = lastKeyframe; i < keyframes.Num(); i++) {
        const FAnimationTrackPropertyKeyframe &keyframe = keyframes[i];

        if(keyframe.frame == frame) {
            if(keyframe.block) {
                blocked = !reset;
                return lastV;
            }
            lastKeyframe = i;
            return keyframe.value;
        } else if(keyframe.frame < frame) {
            if(keyframe.block) {
                lastF = keyframe.frame;
                continue;
            }
            lastV = keyframe.value;
            lastF = keyframe.frame;
            lastKeyframe = i;
        } else { // the first keyframe in the future
            if(keyframe.block) {
                blocked = !reset;
                return lastV;
            }
            double dT = keyframe.frame - lastF;
            double t = ((double)(frame - lastF)) / dT;
            return lastV + ((keyframe.value - lastV) * t);
        }
    }
    blocked = !reset;
    return lastV;
}

UAnimationTrack* UAnimationTrack::fromJSON(TSharedPtr<FJsonObject> json) {
    UAnimationTrack *track = NewObject<UAnimationTrack>();

    /*
    keyframes: [
        {
            "frame": [frames|"[timecode]"],
            "[property]": [value|"B"],
            ...
            "clothing"?: {"[item]": [visible]}
        }
    ]
    */
    TArray<TSharedPtr<FJsonValue>> keyframesJson = json->GetArrayField(JSON_KEYFRAMES);
    for (int i = 0; i < keyframesJson.Num(); i++) {
        TSharedPtr<FJsonObject> keyframeJson = keyframesJson[i]->AsObject();
        // track.keyframes.Add(UAnimationTrackKeyframe::fromJSON(keyframesJson[i]->AsObject()));
        int frame = -1;
        if(keyframeJson->HasTypedField(JSON_FRAME, EJson::String)) {
            frame = UAnimationTrack::parseTimeString(keyframeJson->GetStringField(JSON_FRAME));
        } else if(keyframeJson->HasTypedField(JSON_FRAME, EJson::Number)) {
            frame = keyframeJson->GetIntegerField(JSON_FRAME);
        }
        if(frame == -1) {
            UE_LOG(LogTemp, Warning, TEXT("Keyframe #%d had a malformed frame number, skipping it"), i);
            continue;
        }
        TArray<FString> keys;
        keyframeJson->Values.GetKeys(keys);
        for(const FString& key : keys) {
            if(key == JSON_FRAME) {
                continue;
            } else if(key == JSON_CLOTHING) {
                // TODO: add clothing later
            } else {
                double v = 0;
                if(keyframeJson->HasTypedField(key, EJson::String)) {
                    FString str = keyframeJson->GetStringField(key);
                    if(str == TEXT("B")) {
                        track->addKeyframe(key, { frame, 0, true });
                        continue;
                    }
                    v = UAnimationTrack::parseLengthString(keyframeJson->GetStringField(key));
                } else {
                    v = keyframeJson->GetNumberField(key);
                }
                track->addKeyframe(key, { frame, v, false });
            }
        }
    }

    return track;
}

void UAnimationTrack::addKeyframe(FString property, FAnimationTrackPropertyKeyframe keyframe) {
    FName key = FName(property);
    if(!properties.Contains(key)) {
        properties.Add(key, NewObject<UAnimationTrackPropertyTrack>());
    }
    properties[key]->keyframes.Add(keyframe);
}

double UAnimationTrack::parseLengthString(FString str) {
    bool neg = false;
    double feet = 0;
    double l = 0;
    double next = 0.1;
    bool dec = false;
    for (int i = 0; i < str.Len(); i++) {
        TCHAR c = str[i];
        double v = 0;
        switch (c) {
        case '-':
            neg = true;
            continue;
        case '.':
            dec = true;
            next = 0.1;
            continue;
        case '\'':
            feet = l;
            l  = 0;
            dec = false;
            continue;
        case '0':
            v = 0;
            break;
        case '1':
            v = 1;
            break;
        case '2':
            v = 2;
            break;
        case '3':
            v = 3;
            break;
        case '4':
            v = 4;
            break;
        case '5':
            v = 5;
            break;
        case '6':
            v = 6;
            break;
        case '7':
            v = 7;
            break;
        case '8':
            v = 8;
            break;
        case '9':
            v = 9;
            break;

        default:
            continue;
        }
        
        if(!dec) {
            l *= 10;
            l += v;
        } else {
            l += next * v;
            next /= 10;
        }
    }
    return (l + ( feet * 12 ) ) * (neg ? -1 : 1);
}

int UAnimationTrack::parseTimeString(FString timeString) {
    int frames = -1;
    
    int segment = 0;
    int lastSegment = 0;
    int nextBase = 1;
    for (int i = timeString.Len() - 1; i >= 0; i--)
    {
        TCHAR c = timeString[i];
        switch (c)
        {
        case ':':
            switch (segment)
            {
            case 0: // frames
                frames = lastSegment;
                break;
            case 1: // seconds
                frames += lastSegment * 30;
                break;
            case 2: // minutes
                frames += lastSegment * 30 * 60;
                break;
            case 3: // hours
                frames += lastSegment * 30 * 60 * 60;
                break;
            }
            segment++;
            lastSegment = 0;
            nextBase = 1;
            break;

        case '.':
            return -1;
            // if(segment > 1)
            // {
            //     // error?
            //     break;
            // }
            // lastSegment /= (nextBase);
            // nextBase = 1;
            // break;

        case '0':
            nextBase *= 10;
            break;

        case '1':
            lastSegment += nextBase;
            nextBase *= 10;
            break;

        case '2':
            lastSegment += nextBase * 2;
            nextBase *= 10;
            break;

        case '3':
            lastSegment += nextBase * 3;
            nextBase *= 10;
            break;

        case '4':
            lastSegment += nextBase * 4;
            nextBase *= 10;
            break;

        case '5':
            lastSegment += nextBase * 5;
            nextBase *= 10;
            break;

        case '6':
            lastSegment += nextBase * 6;
            nextBase *= 10;
            break;

        case '7':
            lastSegment += nextBase * 7;
            nextBase *= 10;
            break;

        case '8':
            lastSegment += nextBase * 8;
            nextBase *= 10;
            break;

        case '9':
            lastSegment += nextBase * 9;
            nextBase *= 10;
            break;

        }
    }
    switch (segment)
    {
    case 0: // frames
        frames = lastSegment;
        break;
    case 1: // seconds
        frames += lastSegment * 30;
        break;
    case 2: // minutes
        frames += lastSegment * 30 * 60;
        break;
    case 3: // hours
        frames += lastSegment * 30 * 60 * 60;
        break;
    }
    return frames;
}