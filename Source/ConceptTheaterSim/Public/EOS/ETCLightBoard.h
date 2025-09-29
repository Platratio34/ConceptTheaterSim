// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "EOS/EOSButtons.h"
#include "EOS/EOSShowfile.h"
#include "EOS/EOSPatchTypes.h"
#include "EOS/EOSShowPatch.h"
#include "ETCLightBoard.generated.h"

enum EOSMode
{
    LIVE,
    BLIND,
    STAGE
};

USTRUCT()
struct FCmdSelection
{
	GENERATED_BODY()
public:
    UPROPERTY()
    TArray<int> values;
    UPROPERTY()
    bool chan;
    UPROPERTY()
    bool cue;
    UPROPERTY()
    bool sub;
    UPROPERTY()
    bool group;

    UPROPERTY()
    FString error;
};

UCLASS()
class CONCEPTTHEATERSIM_API AETCLightBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AETCLightBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void onInteract(UPrimitiveComponent* component);

    UFUNCTION(BlueprintCallable)
    void onInteractEnd(UPrimitiveComponent* component, double time);

    UFUNCTION(BlueprintCallable)
    void onInteractScroll(UPrimitiveComponent* component, double direction);

    UFUNCTION(BlueprintNativeEvent)
    void setButtonInteractionText(UPrimitiveComponent *component, FName button);

    UFUNCTION(BlueprintCallable)
    void updateUniverse(int universe, TArray<int> dmx);

    void executeCommand();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    UMaterialInterface *buttonMaterial;

    UPROPERTY(EditAnywhere)
    UMaterialInterface *indicatorMaterial;

    UPROPERTY(VisibleAnywhere)
    TArray<FName> command;
    UPROPERTY(VisibleAnywhere)
    FString commandError = TEXT("");
    bool confirmCmd = false;
    UPROPERTY(VisibleAnywhere)
    bool clearCmd = false;
    UPROPERTY(VisibleAnywhere)
    bool highlightMode = false;
    
    EOSMode mode = LIVE;

    UPROPERTY(EditAnywhere)
    UEOSShowfile* showfile = nullptr;

    UPROPERTY(EditAnywhere)
    UEOSShowPatch* showPatch = nullptr;

private:
    bool shift = false;

    TSet<int> parkedChannels;

    UPROPERTY()
    TMap<FName, UStaticMeshComponent*> buttonsByName;
    UPROPERTY()
    TMap<UStaticMeshComponent*, FName> buttonsByMesh;
    UPROPERTY()
    TMap<FName, UStaticMeshComponent*> indicatorsByName;

    UPROPERTY()
    TMap<FName, UTextRenderComponent*> textByName;

    UPROPERTY()
    TMap<FName, UEOSButton*> buttonByName;

    void setButtonColor(FName button, int r, int g, int b);
    void setButtonColor(FName button, FColor buttonColor, FColor activeColor);
    void setButtonActive(FName button, bool active);

    UPROPERTY()
    UMaterialInstanceDynamic* baseButtonMaterial;

    int getCmdNumber(int start, int* len);
    FCmdSelection getCmdSelection(int start, int *end);
};
