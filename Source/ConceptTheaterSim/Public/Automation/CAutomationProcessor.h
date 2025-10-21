// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Automation/CAutomationNetworking.h"
#include "Networking/NetworkCard.h"
#include "JsonUtilities.h"
#include "CAutomationProcessor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AutomationLog, Log, All);

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACAutomationProcessor : public AActor
{
    GENERATED_BODY()

public:
    
	ACAutomationProcessor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Network")
    UNetworkCard *networkCard;

    UFUNCTION()
    void onNetworkPacket(UNetworkPacket *packet);

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Automation")
    bool eStopActive;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Automation")
    TSet<FName> eStopSources;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Automation")
    TMap<FName, int> deviceIPsByName;

    UFUNCTION()
    void sendEvent(FName device, TMap<FName, double> properties, double duration);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

    UFUNCTION()
    void onEStopPacket(UAutomationEStopPacket *packet);

    UFUNCTION(BlueprintCallable)
    void onTimeUpdate(int frames, float seconds, bool running);
    
};