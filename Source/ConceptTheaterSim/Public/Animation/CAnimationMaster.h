// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeCodeSourceC.h"
#include "JsonUtilities.h"
#include "CAnimationMaster.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AnimationLog, Log, All);

USTRUCT(BlueprintType)
struct FAnimationFileTrackEvent
{
    GENERATED_BODY()
public:

    UPROPERTY(VisibleAnywhere)
    FString time;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double timeSeconds;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double duration = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double x;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool xKey = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double y;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool yKey = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double z;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool zKey = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double xRot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool xRotKey = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double yRot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool yRotKey = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    double zRot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool zRotKey = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName parent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool clearParent = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool parentKey = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool visible;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool visibleKey = false;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FName> showClothing;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FName> hideClothing;
    
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
    }

    static double parseLengthString(FString str)
    {
        bool neg = false;
        double feet = 0;
        double l = 0;
        double next = 0.1;
        bool dec = false;
        for (int i = 0; i < str.Len(); i++)
        {
            TCHAR c = str[i];
            switch (c)
            {
            case '-':
                neg = true;
                break;
            case '.':
                dec = true;
                next = 0.1;
                break;
            case '\'':
                feet = l;
                l  = 0;
                dec = false;
                break;
            case '0':
                if(!dec)
                    l *= 10;
                else
                    next /= 10;
                break;
            case '1':
                if(!dec)
                {
                    l *= 10;
                    l += 1;
                }
                else
                {
                    l += next;
                    next /= 10;
                }
                break;
            case '2':
                if(!dec)
                {
                    l *= 10;
                    l += 2;
                }
                else
                {
                    l += next * 2;
                    next /= 10;
                }
                break;
            case '3':
                if(!dec)
                {
                    l *= 10;
                    l += 3;
                }
                else
                {
                    l += next * 3;
                    next /= 10;
                }
                break;
            case '4':
                if(!dec)
                {
                    l *= 10;
                    l += 4;
                }
                else
                {
                    l += next * 4;
                    next /= 10;
                }
                break;
            case '5':
                if(!dec)
                {
                    l *= 10;
                    l += 5;
                }
                else
                {
                    l += next * 5;
                    next /= 10;
                }
                break;
            case '6':
                if(!dec)
                {
                    l *= 10;
                    l += 6;
                }
                else
                {
                    l += next * 6;
                    next /= 10;
                }
                break;
            case '7':
                if(!dec)
                {
                    l *= 10;
                    l += 7;
                }
                else
                {
                    l += next * 7;
                    next /= 10;
                }
                break;
            case '8':
                if(!dec)
                {
                    l *= 10;
                    l += 8;
                }
                else
                {
                    l += next * 8;
                    next /= 10;
                }
                break;
            case '9':
                if(!dec)
                {
                    l *= 10;
                    l += 9;
                }
                else
                {
                    l += next * 9;
                    next /= 10;
                }
                break;

            default:
                break;
            }
        }
        return (l + ( feet * 12 ) ) * (neg ? -1 : 1);
    }

    static FAnimationFileTrackEvent CreateFrom(TSharedPtr<FJsonObject> eventJson)
    {
        FAnimationFileTrackEvent event;
        event.time = eventJson->GetStringField(JSON_TIME);
        event.timeSeconds = parseTimeString(event.time);
        if(eventJson->HasField(JSON_DURATION))
        {
            if(eventJson->HasTypedField(JSON_DURATION, EJson::String))
                event.duration = parseTimeString(eventJson->GetStringField(JSON_DURATION));
            else
                event.duration = eventJson->GetNumberField(JSON_DURATION);
        }
        if(eventJson->HasField(JSON_END))
        {
            double end = parseTimeString(eventJson->GetStringField(JSON_END));
            event.duration = end - event.timeSeconds;
        }

        if(eventJson->HasField(JSON_X))
        {
            if(eventJson->HasTypedField(JSON_X, EJson::String))
                event.x = parseLengthString(eventJson->GetStringField(JSON_X));
            else
                event.x = eventJson->GetNumberField(JSON_X);
            event.xKey = true;
        }
        if(eventJson->HasField(JSON_Y))
        {
            if(eventJson->HasTypedField(JSON_Y, EJson::String))
                event.y = parseLengthString(eventJson->GetStringField(JSON_Y));
            else
                event.y = eventJson->GetNumberField(JSON_Y);
            event.yKey = true;
        }
        if(eventJson->HasField(JSON_Z))
        {
            if(eventJson->HasTypedField(JSON_Z, EJson::String))
                event.z = parseLengthString(eventJson->GetStringField(JSON_Z));
            else
                event.z = eventJson->GetNumberField(JSON_Z);
            event.zKey = true;
        }
        
        if(eventJson->HasField(JSON_X_ROT))
        {
            event.xRot = eventJson->GetNumberField(JSON_X_ROT);
            event.xRotKey = true;
        }
        if(eventJson->HasField(JSON_Y_ROT))
        {
            event.yRot = eventJson->GetNumberField(JSON_Y_ROT);
            event.yRotKey = true;
        }
        if(eventJson->HasField(JSON_Z_ROT))
        {
            event.zRot = eventJson->GetNumberField(JSON_Z_ROT);
            event.zRotKey = true;
        }

        if(eventJson->HasField(JSON_PARENT))
        {
            FString p = eventJson->GetStringField(JSON_PARENT);
            if(p.Len() > 0)
                event.parent = FName(p);
            else
                event.clearParent = true;
            event.parentKey = true;
        }

        if(eventJson->HasField(JSON_VISIBILITY))
        {
            event.visible = eventJson->GetBoolField(JSON_VISIBILITY);
            event.visibleKey = true;
        }

        if(eventJson->HasField(JSON_SHOW_CLOTHING))
        {
            TArray<TSharedPtr<FJsonValue>> showJson = eventJson->GetArrayField(JSON_SHOW_CLOTHING);
            for (int i = 0; i < showJson.Num(); i++)
            {
                event.showClothing.Add(FName(showJson[i]->AsString()));
            }
        }

        if(eventJson->HasField(JSON_HIDE_CLOTHING))
        {
            TArray<TSharedPtr<FJsonValue>> hideJson = eventJson->GetArrayField(JSON_HIDE_CLOTHING);
            for (int i = 0; i < hideJson.Num(); i++)
            {
                event.hideClothing.Add(FName(hideJson[i]->AsString()));
            }
        }
        
        return event;
    }
    
    static inline FString JSON_TIME = TEXT("time");
    static inline FString JSON_DURATION = TEXT("duration");
    static inline FString JSON_END = TEXT("end");

    static inline FString JSON_X = TEXT("x");
    static inline FString JSON_Y = TEXT("y");
    static inline FString JSON_Z = TEXT("z");
    static inline FString JSON_X_ROT = TEXT("xRot");
    static inline FString JSON_Y_ROT = TEXT("yRot");
    static inline FString JSON_Z_ROT = TEXT("zRot");
    static inline FString JSON_PARENT = TEXT("parent");
    static inline FString JSON_VISIBILITY = TEXT("visibility");

    static inline FString JSON_SHOW_CLOTHING = TEXT("showClothing");
    static inline FString JSON_HIDE_CLOTHING = TEXT("hideClothing");
};

USTRUCT(BlueprintType)
struct FAnimationFileTrack
{
    GENERATED_BODY()
public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FAnimationFileTrackEvent> events;

    UPROPERTY(VisibleAnywhere)
    int nextEvent = 0;

    static FAnimationFileTrack CreateFrom(TSharedPtr<FJsonObject> trackJson)
    {
        FAnimationFileTrack track;
        TArray<TSharedPtr<FJsonValue>> eventsJson = trackJson->GetArrayField(TEXT("Events"));
        for (int i = 0; i < eventsJson.Num(); i++)
        {
            track.events.Add(FAnimationFileTrackEvent::CreateFrom(eventsJson[i]->AsObject()));
        }
        return track;
    }
};

USTRUCT(BlueprintType)
struct FAnimationFile
{
    GENERATED_BODY()
public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<FName, FAnimationFileTrack> tracks;
};

class CONCEPTTHEATERSIM_API UCAnimationComponent;

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACAnimationMaster : public AActor
{
    GENERATED_BODY()

public:
    
	ACAnimationMaster();

    UPROPERTY(EditAnywhere)
    FString filePath = "";

    UFUNCTION(BlueprintCallable, CallInEditor)
    void reloadFile();

    UFUNCTION(BlueprintCallable)
    void loadFile(FString path)
    {
        filePath = path;
        reloadFile();
    }

    UPROPERTY(EditAnywhere)
    ATimeCodeSourceC* timecodeSource = nullptr;

    UFUNCTION(BlueprintCallable)
    void registerAnimatedObject(FName id, UCAnimationComponent* component);

    UFUNCTION(BlueprintCallable)
    void registerParentObject(FName id, USceneComponent* parent);

    UFUNCTION(BlueprintCallable)
    USceneComponent *getParent(FName id)
    {
        if(USceneComponent** component = animationParents.Find(id))
        {
            return *component;
        }
        return nullptr;
    }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void onTimeUpdate(int Frames, float Seconds);
    
    UFUNCTION(BlueprintCallable)
    void onTimeStop();

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    FAnimationFile animationFile;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    bool animationFileLoaded = false;
    
    UPROPERTY(VisibleInstanceOnly)
    TMap<FName, int> nextEvent;

    UPROPERTY(VisibleInstanceOnly)
    TMap<FName, UCAnimationComponent *> animatedObjects;

    UPROPERTY(VisibleInstanceOnly)
    TMap<FName, USceneComponent*> animationParents;

private:
    UPROPERTY()
    double lastSeconds = -1;

    bool first = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};