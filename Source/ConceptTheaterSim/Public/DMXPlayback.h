// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DMXPlayback.generated.h"

UCLASS()
class CONCEPTTHEATERSIM_API ADMXPlayback : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADMXPlayback();

    UFUNCTION(BlueprintCallable)
    bool UpdateTime(int frames);

    UFUNCTION(BlueprintCallable)
    TArray<int> GetUniverse(int universe);

    UFUNCTION(BlueprintCallable)
    int GetDMXFrame();

    UFUNCTION(BlueprintCallable)
    void LoadFile(FString filename);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    struct DMXFrame {
        uint32 frameNumber;
        uint32 count;
        uint8 **universes;
        bool isValid = false;
    };

private:
    int numFrames;
    DMXFrame *frames;
    int numUniverses = 0;
    uint16 *universes;

    DMXFrame cFrame;
    int frameIndex = -1;
    int cFrameNumber = -1;
    bool loaded = false;
};
