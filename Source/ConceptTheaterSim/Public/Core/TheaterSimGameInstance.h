// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TheaterSimGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API UTheaterSimGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    FName currentShow = FName(TEXT("TLT"));
};
