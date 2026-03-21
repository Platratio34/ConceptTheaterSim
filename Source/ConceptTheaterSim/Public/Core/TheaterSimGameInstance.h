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
    
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    bool forceLoad = false;

    UFUNCTION(BlueprintCallable)
    void save(FString slot);
    
    UFUNCTION(BlueprintCallable)
    void load(FString slot);
    
    UFUNCTION(BlueprintCallable)
    void loadIfNot(FString slot);
    
    UFUNCTION(BlueprintCallable)
    bool isLoaded(FString slot) { return loaded.Contains(slot); }

    UFUNCTION(BlueprintCallable, Exec)
    void LoadShow(FString name);

protected:
    UPROPERTY(VisibleAnywhere)
    TMap<FString, bool> loaded;
};
