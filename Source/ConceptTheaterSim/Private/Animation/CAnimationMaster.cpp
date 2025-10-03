// Copyright Peter Crall 2025.

#include "Animation/CAnimationMaster.h"
#include "Animation/CAnimationComponent.h"
#include "Json.h"

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
                (*component)->onEvent(track.events[0]);
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
    UE_LOG(LogTemp, Display, TEXT("Loading Animation: %s"), *path);
    FString jsonString;
    if(!FFileHelper::LoadFileToString(jsonString, *path))
    {
        UE_LOG(LogTemp, Warning, TEXT("Error Loading Animation file: %s; Error loading file"), *path);
        return;
    }

    TSharedPtr<FJsonObject> jsonObject;
    TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(jsonString);
    if(!FJsonSerializer::Deserialize(reader, jsonObject) || !jsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Error Loading Animation file: %s; Error parsing JSON"), *path);
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
    animationFile = animFile;
    animationFileLoaded = true;
    lastSeconds = -1;
    first = true;
}

void ACAnimationMaster::onTimeUpdate(int frames, float seconds)
{
    if(!animationFileLoaded)
        return;
    
    bool jump = abs(seconds - lastSeconds) > 0.5;
    if(jump)
        UE_LOG(LogTemp, Display, TEXT("Jump"));
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
                if(event.duration < 0.5)
                    event.duration = 0.5;
            }
            nextEvent.Add(key, i+1);
            if(UCAnimationComponent** component = animatedObjects.Find(key))
            {
                UE_LOG(LogTemp, Display, TEXT("Animation event (@ %f, %f)"), event.timeSeconds, seconds);
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
    registerParentObject(id, component->GetOwner());
}

void ACAnimationMaster::registerParentObject(FName id, AActor* object)
{
    animationParents.Add(id, object);
}