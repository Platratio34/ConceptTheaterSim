// Copyright Peter Crall 2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CVisibilityBlocker.generated.h"

UINTERFACE(Blueprintable, BlueprintType)
class CONCEPTTHEATERSIM_API UCVisibilityBlocker : public UInterface
{
    GENERATED_BODY()
};

class CONCEPTTHEATERSIM_API ICVisibilityBlocker
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Visibility")
    bool isVisibilityBlocking();

private:
    virtual bool isVisibilityBlocking_Implementation() { return false; };
};