// Fill out your copyright notice in the Description page of Project Settings.


#include "DMXPlayback.h"

// Sets default values
ADMXPlayback::ADMXPlayback()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADMXPlayback::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADMXPlayback::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADMXPlayback::LoadFile(FString filename) {
    loaded = false;
    TArray64<uint8> file;
    FString path = FPaths::ProjectContentDir();
    path += "/" + filename;
    UE_LOG(LogTemp, Display, TEXT("Loading DMX recording: %s"), *path);
    FFileHelper::LoadFileToArray(file, *path, 0x00000000);
    uint8 *bytes = file.GetData();
    if(bytes == nullptr || file.Num() == 0) {
        UE_LOG(LogTemp, Error, TEXT("Invalid file; No data"));
        return;
    }
    if(bytes[0] != 0x44 || bytes[1] != 0x4d || bytes[2] != 0x58 || bytes[3] != 0x00) { // invalid file type
        UE_LOG(LogTemp, Error, TEXT("Invalid file type for DMX recording binary;"));
        return;
    }
    uint16 version = (bytes[4] << 8) | (bytes[5]);
    if(version != 0x01) { // un-implemented version
        UE_LOG(LogTemp, Error, TEXT("Unimplemented file version for DMX recording binary; Expected `1`, got `%u`"), version);
        return;
    }
    int pointer = 8;
    TArray<uint16> universesList;
    numUniverses = 0;
    uint16 nextUniverse = (bytes[6] << 8) | (bytes[7]);
    while(nextUniverse > 0) {
        universesList.Add(nextUniverse);
        numUniverses++;

        nextUniverse = (bytes[pointer++] << 8) | (bytes[pointer++]);
    }
    universes = new uint16[numUniverses];
    for (int32 i = 0; i != universesList.Num(); ++i)
    {
        universes[i] = universesList[i];
    }

    numFrames = (bytes[pointer++] << 24) | (bytes[pointer++] << 16) | (bytes[pointer++] << 8) | (bytes[pointer++]);
    TArray<DMXFrame> frameList;
    uint32 frameNumber = (bytes[pointer++] << 24) | (bytes[pointer++] << 16) | (bytes[pointer++] << 8) | (bytes[pointer++]);
    int numActualFrames = 0;
    int skippedFrames = 0;
    DMXFrame lastFrame;
    while(frameNumber != 0xffffffff) {
        DMXFrame frame;
        frame.count = (bytes[pointer++] << 24) | (bytes[pointer++] << 16) | (bytes[pointer++] << 8) | (bytes[pointer++]);
        frame.frameNumber = frameNumber;
        frame.universes = new uint8*[numUniverses];
        int skip = 0;
        for (int i = 0; i < numUniverses; i++)
        {
            frame.universes[i] = new uint8[512];
            int num255 = true;
            for (int a = 0; a < 512; a++)
            {
                frame.universes[i][a] = bytes[pointer++];
                if(frame.universes[i][a] == (uint8)0xff)
                    num255++;
            }
            if(numActualFrames > 0 && (num255 > 200)/* && frame.count <= 1*/) {
                // UE_LOG(LogTemp, Display, TEXT("Flash detected? (frame %i, universe U%i, length %i, num255 %i)"), frame.frameNumber, universes[i], frame.count, num255);
                skip++;
            }
        }
        if(skip > 2) {
            skippedFrames++;
            UE_LOG(LogTemp, Display, TEXT("Skipped frame %i: preview flash"), frame.frameNumber);
            frameList[numActualFrames - 1].count += 1 + frame.count;
        } else {
            frame.isValid = true;
            frameList.Add(frame);
            lastFrame = frame;
            numActualFrames++;
        }

        frameNumber = (bytes[pointer++] << 24) | (bytes[pointer++] << 16) | (bytes[pointer++] << 8) | (bytes[pointer++]);
    }
    if(numFrames != numActualFrames) {
        // something is wrong here, probably just a bad file
        if(numFrames != (numActualFrames + skippedFrames)) // only warn if we didn't skip any those frames
            UE_LOG(LogTemp, Warning, TEXT("File specified %i frames, but %i were found, falling back to number of frames found"), numFrames, numActualFrames)
        numFrames = numActualFrames; // But we'll update this just to avoid issues in playback
    }
    
    frames = new DMXFrame[frameList.Num()];
    for (int32 i = 0; i != frameList.Num(); ++i)
    {
        frames[i] = frameList[i];
    }
    cFrame = frames[0];
    frameIndex = 0;
    loaded = true;
    UE_LOG(LogTemp, Display, TEXT("DMX recording loaded (%i universes, %i frames)"), numUniverses, numFrames);
}

bool ADMXPlayback::UpdateTime(int newFrames) {
    int last = cFrameNumber;
    cFrameNumber = newFrames;

    if(newFrames == last)
        return false; // repeated frame
    
    if(newFrames < last) { // We went back in time, so we may need to start over
        cFrameNumber = newFrames;
        for (int i = 0; i < numFrames; i++) { // search for last defined frame
            DMXFrame f = frames[i];
            if((int)f.frameNumber > cFrameNumber) {
                if(frameIndex == i-1)
                    return false; // No actual update
                frameIndex = i - 1;
                break;
            }
        }
        cFrame = frames[frameIndex];
        return true;
    }

    if(frameIndex == numFrames - 1)
        return false; // No more frames defined
    
    DMXFrame nextFrame = frames[frameIndex + 1];
    bool updated = false;
    while((frameIndex + 1 < numFrames) && ((int)nextFrame.frameNumber <= newFrames)) {
        cFrame = nextFrame;
        frameIndex++;
        updated = true;
        
        nextFrame = frames[frameIndex + 1];
    }

    return updated;
}

void ADMXPlayback::NextFrame() {
    if(frameIndex == numFrames - 1)
        return; // No more frames defined
    cFrame = frames[frameIndex + 1];
    frameIndex++;
}

int ADMXPlayback::SetDMXFrame(int frame) {
    if(frame < 0) {
        frame = 0;
    } else if (frame >= numFrames) {
        frame = numFrames - 1;
    }
    frameIndex = frame;
    cFrame = frames[frameIndex];
    return frameIndex;
}

TArray<int> ADMXPlayback::GetUniverse(int universe) {
    TArray<int> outArr;
    outArr.Init(0, 512);
    
    try
    {
        if (frameIndex < 0 || !loaded || !cFrame.isValid)
        {
            UE_LOG(LogTemp, Warning, TEXT("Tried to get data from DMX recording for universe %i, but no frame was loaded (frameIndex=%i, loaded=%s, frameValid=%s)"), universe, frameIndex, loaded?TEXT("true"):TEXT("false"),cFrame.isValid?TEXT("true"):TEXT("false"));
            return outArr;
        }
        bool universeFound = false;
        for (int i = 0; i < numUniverses; i++) {
            if(universes[i] == universe) {
                uint8 *data = cFrame.universes[i];
                for(int a = 0; a < 512; a++) {
                    outArr[a] = (int)data[a];
                }
                universeFound = true;
                break;
            }
        }
        if(!universeFound) {
            UE_LOG(LogTemp, Warning, TEXT("Tried to get data from DMX recording for universe %i, universe could not be found, (%i total universes recorded)"), universe, numUniverses);
        }
    }
    catch(const std::exception& e)
    {
        UE_LOG(LogTemp, Error, TEXT("Error in GetUniverse(%i): %hs"), universe, e.what());
    }
    return outArr;
}

int ADMXPlayback::GetDMXFrame() {
    return frameIndex;
}