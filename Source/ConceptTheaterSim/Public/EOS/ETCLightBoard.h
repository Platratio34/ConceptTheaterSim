// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "EOS/EOSButtons.h"
#include "ETCLightBoard.generated.h"

enum EOSMode
{
    LIVE,
    BLIND,
    STAGE
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

    void executeCommand();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    UMaterialInterface *buttonMaterial;

    UPROPERTY(EditAnywhere)
    UMaterialInterface *indicatorMaterial;

    TArray<FName> command;
    FString commandError = TEXT("");
    bool confirmCmd = false;
    bool clearCmd = false;
    bool highlightMode = false;
    EOSMode mode = LIVE;

private:
    bool shift = false;

    TSet<int> parkedChannels;

    TMap<FName, UStaticMeshComponent*> buttonsByName;
    TMap<UStaticMeshComponent*, FName> buttonsByMesh;
    TMap<FName, UStaticMeshComponent*> indicatorsByName;

    TMap<FName, UTextRenderComponent*> textByName;

    TMap<FName, UEOSButton*> buttonByName;

    void setButtonColor(FName button, int r, int g, int b);
    void setButtonColor(FName button, FColor buttonColor, FColor activeColor);
    void setButtonActive(FName button, bool active);

    UMaterialInstanceDynamic* baseButtonMaterial;

    int getCmdNumber(int start);
};
