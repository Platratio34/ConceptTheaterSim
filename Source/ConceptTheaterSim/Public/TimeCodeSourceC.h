// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeCodeSourceC.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API ATimeCodeSourceC : public AActor
{
	GENERATED_BODY()

public:
    ATimeCodeSourceC();

    void TimerUpdate();

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintCallable)
    int GetFrames();

    UFUNCTION(BlueprintCallable)
    float GetSeconds();

    UFUNCTION(BlueprintCallable)
    void Start(int frames);

    UFUNCTION(BlueprintCallable)
    void Stop();

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool IsRunning();

    UDELEGATE(BlueprintCallable)
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeChange, int, Frames, float, Seconds);
    
    UDELEGATE(BlueprintCallable)
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStop);

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    FOnTimeChange onTimeChangeEvent;
    UPROPERTY(VisibleAnywhere)
    FOnStop onTimeStopEvent;

protected:
    int frames = 0;
    int frameRate = 30;

    FTimerHandle timer;

    bool running = false;

    virtual void BeginPlay() override;
};
