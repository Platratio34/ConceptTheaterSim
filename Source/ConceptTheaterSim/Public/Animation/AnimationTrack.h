// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "JsonUtilities.h"
#include "AnimationTrack.generated.h"


USTRUCT(BlueprintType)
struct FAnimationTrackPropertyKeyframe {
    GENERATED_BODY()

public:
    int frame;
    double value;
    bool block;
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UAnimationTrackPropertyTrack : public UObject {
    GENERATED_BODY()

public:
    TArray<FAnimationTrackPropertyKeyframe> keyframes;

    UFUNCTION(BlueprintCallable)
    double getValue(int frame);
    
    UFUNCTION(BlueprintCallable)
    double getValueTracked(int frame);
    
    bool blocked;

protected:
    int lastFrame;
    int lastKeyframe;
};

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UAnimationTrack : public UObject
{
    GENERATED_BODY()

public:
    TMap<FName, UAnimationTrackPropertyTrack*> properties;

    static UAnimationTrack *fromJSON(TSharedPtr<FJsonObject> json);

    static double parseLengthString(FString str);
    static int parseTimeString(FString timeString);

    static inline FString JSON_KEYFRAMES = TEXT("keyframes");

    static inline FString JSON_FRAME = TEXT("frame");
    static inline FString JSON_CLOTHING = TEXT("clothing");

private:
    void addKeyframe(FString property, FAnimationTrackPropertyKeyframe keyframe);
};