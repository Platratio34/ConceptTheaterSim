// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheaterSimGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API ATheaterSimGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
    ATheaterSimGameMode();
    ~ATheaterSimGameMode();

public:
    UFUNCTION(BlueprintPure)
    float getAverageTPS();

	virtual void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleInstanceOnly)
    float tps;
    UPROPERTY(VisibleInstanceOnly, AdvancedDisplay)
    TArray<float> tpsArr;
    UPROPERTY(VisibleInstanceOnly, AdvancedDisplay)
    int tpsPtr = 0;
};
