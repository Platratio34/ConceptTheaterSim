// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ColorUtils.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPTTHEATERSIM_API UColorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

    TArray<int> BlendHSV();
};
