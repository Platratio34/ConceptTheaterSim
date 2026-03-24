// Copyright Peter Crall 2025.

#include "Animation/CAnimationMaster.h"
#include "Animation/CAnimationComponent.h"
#include "Json.h"

DEFINE_LOG_CATEGORY(AnimationLog);

// Sets default values
ACAnimationMaster::ACAnimationMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACAnimationMaster::BeginPlay()
{
	Super::BeginPlay();
    reloadFile();
    if(timecodeSource != nullptr)
    {
        timecodeSource->onTimeChangeEvent.AddDynamic(this, &ACAnimationMaster::onTimeUpdate);
        timecodeSource->onTimeStopEvent.AddDynamic(this, &ACAnimationMaster::onTimeStop);
    }
}

// Called every frame
void ACAnimationMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(first && animationFileLoaded)
    {
        TArray<FName> trackKeys;
        animationFile.tracks.GetKeys(trackKeys);
        for(const FName& key : trackKeys)
        {
            FAnimationFileTrack track = animationFile.tracks[key];
            if(UCAnimationComponent** component = animatedObjects.Find(key))
            {
                for (int i = 0; i < track.events.Num(); i++)
                {
                    FAnimationFileTrackEvent event = track.events[i];
                    if (event.timeSeconds > lastSeconds && i > 0)
                        break;
                    (*component)->onEvent(event);
                }
                UE_LOG(AnimationLog, Display, TEXT("Setting default position for animated object: %s"), *key.ToString());
            }
            else
            {
                UE_LOG(AnimationLog, Warning, TEXT("Missing animated object: %s"), *key.ToString());
            }
        }
        animatedObjects.GetKeys(trackKeys);
        for(const FName& key : trackKeys)
        {
            if(!animationFile.tracks.Find(key))
            {
                UE_LOG(AnimationLog, Warning, TEXT("Missing track for animated object: %s"), *key.ToString());
            }
        }
        first = false;
    }
}

void ACAnimationMaster::reloadFile()
{
    animationFileLoaded = false;
    if(filePath.Len() <= 0)
    {
        return;
    }
    FString path = FPaths::ProjectContentDir();
    path += "/" + filePath;
    UE_LOG(AnimationLog, Display, TEXT("Loading Animation: %s"), *path);
    FString jsonString;
    if(!FFileHelper::LoadFileToString(jsonString, *path))
    {
        UE_LOG(AnimationLog, Warning, TEXT("Error Loading Animation file: %s; Error loading file"), *path);
        return;
    }

    TSharedPtr<FJsonObject> jsonObject;
    TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(jsonString);
    if(!FJsonSerializer::Deserialize(reader, jsonObject) || !jsonObject.IsValid())
    {
        UE_LOG(AnimationLog, Warning, TEXT("Error Loading Animation file: %s; Error parsing JSON"), *path);
        return;
    }

    FAnimationFile animFile;
    TSharedPtr<FJsonObject> tracksField = jsonObject->GetObjectField(TEXT("tracks"));
    TArray<FString> trackKeys;
    tracksField->Values.GetKeys(trackKeys);
    for(const FString& key : trackKeys)
    {
        FAnimationFileTrack track = FAnimationFileTrack::CreateFrom(tracksField->GetObjectField(key));
        animFile.tracks.Add(FName(*key), track);
        nextEvent.Add(FName(*key), 0);
    }

    if(jsonObject->HasField(TEXT("keyTracks"))) {
        TSharedPtr<FJsonObject> keyTracksField = jsonObject->GetObjectField(TEXT("keyTracks"));
        TArray<FString> keyTrackKeys;
        keyTracksField->Values.GetKeys(keyTrackKeys);
        for(const FString& key : keyTrackKeys) {
            FName name = FName(*key);
            UAnimationTrack *t = UAnimationTrack::fromJSON(keyTracksField->GetObjectField(key));
            keyTracks.Add(name, t);
            if(animatedObjects.Contains(name)) {
                animatedObjects[name]->setTrack(t);
            }
        }
    }

    // reset/setup
    animationFile = animFile;
    animationFileLoaded = true;
    first = true;
    UE_LOG(AnimationLog, Display, TEXT("Animation loaded: %s"), *path);
}

void ACAnimationMaster::onTimeUpdate(int frames, float seconds)
{
    lastFrame = frames;
    if(!animationFileLoaded)
        return;
    
    bool jump = abs(seconds - lastSeconds) > 0.5;
    if(jump)
        UE_LOG(AnimationLog, Display, TEXT("Timecode Jump"));
    lastSeconds = seconds;
    
    TArray<FName> trackKeys;
    animationFile.tracks.GetKeys(trackKeys);
    for(const FName& key : trackKeys)
    {
        FAnimationFileTrack track = animationFile.tracks[key];
        if(jump)
        {
            nextEvent.Add(key, 0);
        }
        for (int i = nextEvent[key]; i < track.events.Num(); i++)
        {
            FAnimationFileTrackEvent event = track.events[i];
            if(event.timeSeconds > seconds)
                break;
            if(event.timeSeconds < seconds)
            {
                event.duration = event.duration - (seconds - event.timeSeconds);
                if(event.duration < 0)
                    event.duration = 0;
            }
            nextEvent.Add(key, i+1);
            if(UCAnimationComponent** component = animatedObjects.Find(key))
            {
                // UE_LOG(AnimationLog, Display, TEXT("Animation event (@ %f, %f)"), event.timeSeconds, seconds);
                (*component)->onEvent(event);
            }
        }
    }
}

void ACAnimationMaster::onTimeStop()
{
    
}

void ACAnimationMaster::registerAnimatedObject(FName id, UCAnimationComponent* component)
{
    animatedObjects.Add(id, component);
    registerParentObject(id, component->GetOwner()->GetRootComponent());
    first = true;
    if(keyTracks.Contains(id)) {
        component->setTrack(keyTracks[id]);
    }
}

void ACAnimationMaster::registerParentObject(FName id, USceneComponent* parent)
{
    animationParents.Add(id, parent);
}