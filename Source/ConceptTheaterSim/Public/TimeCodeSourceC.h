// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking/NetworkCard.h"
#include "Networking/NetworkPacketTypes.h"
#include "RawMidiOutput.h"
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

    UPROPERTY(VisibleAnywhere)
    UNetworkCard *networkCard = nullptr;

    UPROPERTY(VisibleAnywhere)
    FName timeSourceName = FName(TEXT("TimeCodeSourceC"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool midiEnabled = false;

protected:
    UPROPERTY(VisibleInstanceOnly)
    int qf = 0;

    UPROPERTY(VisibleInstanceOnly)
    int frames = 0;

    UPROPERTY()
    int frameRate = 30;

    UPROPERTY()
    FTimerHandle timer;

    UPROPERTY(VisibleInstanceOnly)
    bool running = false;

    virtual void BeginPlay() override;

    void sendTimePacket();

    UPROPERTY(BlueprintReadOnly)
    URawMidiOutput *midiOutput = nullptr;
};
