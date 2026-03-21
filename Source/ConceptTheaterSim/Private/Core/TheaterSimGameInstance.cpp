// Copyright Peter Crall 2025


#include "Core/TheaterSimGameInstance.h"
#include "Core/TheaterSimSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void UTheaterSimGameInstance::save(FString slot)
{
    if(UTheaterSimSaveGame* instance = Cast<UTheaterSimSaveGame>(UGameplayStatics::CreateSaveGameObject(UTheaterSimSaveGame::StaticClass())))
    {
        instance->lastShow = currentShow;

        if(UGameplayStatics::SaveGameToSlot(instance, slot, 0))
        {
            UE_LOG(LogTemp, Display, TEXT("Game state saved"));
            GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("Game state saved"));
            return;
        }
    }
    UE_LOG(LogTemp, Error, TEXT("Error saving game state"));
    GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, TEXT("Error saving game state"));
}

void UTheaterSimGameInstance::load(FString slot)
{
    if(UTheaterSimSaveGame* loadedState = Cast<UTheaterSimSaveGame>(UGameplayStatics::LoadGameFromSlot(slot, 0)))
    {
        currentShow = loadedState->lastShow;
        UE_LOG(LogTemp, Display, TEXT("Game state loaded"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("Game state loaded"));
        loaded.Add(slot, true);
        return;
    }
    UE_LOG(LogTemp, Error, TEXT("Error loading game state"));
    GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, TEXT("Error loading game state"));
}

void UTheaterSimGameInstance::loadIfNot(FString slot)
{   
    if(!isLoaded(slot))
        load(slot);
}

void UTheaterSimGameInstance::LoadShow(FString name) {
    currentShow = FName(name);
    forceLoad = true;
    FName CurrentLevelName = FName(*GetWorld()->GetName());
    UGameplayStatics::OpenLevel(GetWorld(), CurrentLevelName);
}