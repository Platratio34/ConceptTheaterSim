// Copyright Peter Crall 2025

#pragma once

#include "GameFramework/SaveGame.h"
#include "TheaterSimSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API UTheaterSimSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

public:
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FName lastShow;

    UTheaterSimSaveGame();
};
