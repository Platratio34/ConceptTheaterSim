// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking/DMXNetworkCard.h"
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
    int SetDMXFrame(int frame);

    UFUNCTION(BlueprintCallable)
    void LoadFile(FString filename);

    UFUNCTION(BlueprintCallable)
    void NextFrame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UDMXNetworkCard *networkCard;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    struct DMXFrame {
        uint32 frameNumber;
        uint32 count;
        uint8 **universes;
        bool isValid = false;
    };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DMX")
    FName dmxSourceName = FName(TEXT("DMXPlayback"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DMX")
    int dmxPriority = 127;

protected:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="DMX", AdvancedDisplay)
    int numFrames;
    DMXFrame *frames;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="DMX", AdvancedDisplay)
    int numUniverses = 0;
    uint16 *universes;

    DMXFrame cFrame;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="DMX")
    int frameIndex = -1;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="DMX")
    int cFrameNumber = -1;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="DMX", AdvancedDisplay)
    bool loaded = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="DMX", AdvancedDisplay)
    int lastSentFrame = -1;

private:
    UFUNCTION()
    void sendFramePackets();
};
