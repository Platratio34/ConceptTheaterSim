// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TimeUtil.generated.h"

UCLASS()
class CONCEPTTHEATERSIM_API UTimeUtil : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="TimeUtil")
    static double parseTimeString(FString timeString)
    {
        double seconds = 0;
        int segment = 0;
        double lastSegment = 0;
        double nextBase = 1;
        for (int i = timeString.Len() - 1; i >= 0; i--)
        {
            TCHAR c = timeString[i];
            switch (c)
            {
            case ':':
                switch (segment)
                {
                case 0: // frames
                    seconds += lastSegment / 30.0;
                    break;
                case 1: // seconds
                    seconds += lastSegment;
                    break;
                case 2: // minutes
                    seconds += lastSegment*60;
                    break;
                case 3: // hours
                    seconds += lastSegment*60*60;
                    break;
                }
                segment++;
                lastSegment = 0;
                nextBase = 1;
                break;

            case '.':
                if(segment > 1)
                {
                    // error?
                    break;
                }
                lastSegment /= (nextBase);
                nextBase = 1;
                break;

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
            seconds += lastSegment / 30.0;
            break;
        case 1: // seconds
            seconds += lastSegment;
            break;
        case 2: // minutes
            seconds += lastSegment*60;
            break;
        case 3: // hours
            seconds += lastSegment*60*60;
            break;
        }
        return seconds;
    };

    static int parseTimeStringFrames(FString timeString)
    {
        int frames = 0;
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
                    frames += lastSegment;
                    break;
                case 1: // seconds
                    frames += lastSegment*30;
                    break;
                case 2: // minutes
                    frames += lastSegment*30*60;
                    break;
                case 3: // hours
                    frames += lastSegment*30*60*60;
                    break;
                }
                segment++;
                lastSegment = 0;
                nextBase = 1;
                break;

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
            frames += lastSegment;
            break;
        case 1: // seconds
            frames += lastSegment*30;
            break;
        case 2: // minutes
            frames += lastSegment*30*60;
            break;
        case 3: // hours
            frames += lastSegment*30*60*60;
            break;
        }
        return frames;
    };

    static FString createTimeStringFromFrames(int frames)
    {
        int f = frames % 30;
        int s = frames / 30;
        int m = s / 60;
        int h = m / 60;
        return FString::Printf(TEXT("%02d:%02d:%02d:%02d"), h % 60, m % 60, s % 60, f);
    };

    static FString createTimeStringFromSeconds(double seconds)
    {
        int f = FMath::FloorToInt(seconds * 30.0);
        int s = FMath::FloorToInt(seconds / 30.0);
        int m = s / 60;
        int h = m / 60;
        return FString::Printf(TEXT("%02d:%02d:%02d:%02d"), h % 60, m % 60, s, f);
    };
};