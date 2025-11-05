// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "TheaterSimPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API ATheaterSimPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly)
    UInputMappingContext *defaultMappingContext;
};
