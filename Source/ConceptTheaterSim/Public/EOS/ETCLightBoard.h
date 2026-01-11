// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "EOS/EOSButtons.h"
#include "EOS/EOSShowfile.h"
#include "EOS/EOSPatchTypes.h"
#include "EOS/EOSShowPatch.h"
#include "Networking/DMXNetworkCard.h"
#include "ETCLightBoard.generated.h"

UENUM(BlueprintType)
enum class EEOSMode : uint8
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

    UFUNCTION(BlueprintCallable)
    TArray<int> outputUniverse(int universe);

    UFUNCTION()
    void executeCommand();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UDMXNetworkCard *networkCard;

    UFUNCTION()
    void executeCue(int cueI, double time = -1);

    UFUNCTION(BlueprintCallable)
    void onTimecode(int frames);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category="Buttons")
    UMaterialInterface *buttonMaterial;

    UPROPERTY(EditAnywhere, Category="Buttons")
    UMaterialInterface *indicatorMaterial;

    UPROPERTY(EditAnywhere, Category="EOS")
    UEOSShowfile* showfile = nullptr;

    UPROPERTY(EditAnywhere, Category="EOS")
    UEOSShowPatch* showPatch = nullptr;

    UPROPERTY(VisibleInstanceOnly, Category="EOS")
    TArray<FName> command;
    UPROPERTY(VisibleInstanceOnly, Category="EOS")
    FString commandError = TEXT("");
    bool confirmCmd = false;
    UPROPERTY(VisibleInstanceOnly, Category="EOS", AdvancedDisplay)
    bool clearCmd = false;
    UPROPERTY(VisibleInstanceOnly, Category="EOS", AdvancedDisplay)
    bool cmdComplete = false;
    UPROPERTY(VisibleInstanceOnly, Category="EOS", AdvancedDisplay)
    bool highlightMode = false;
    
    UPROPERTY(VisibleInstanceOnly, Category="EOS")
    EEOSMode mode = EEOSMode::LIVE;

    UFUNCTION(BlueprintCallable)
    bool loadShowfile(FString filePath);
    
    UFUNCTION(BlueprintCallable)
    void saveShowfile(FString filePath);

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool autoLoad = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool output = false;

    UFUNCTION(BlueprintCallable)
    void cueGo();

    UFUNCTION(BlueprintCallable)
    void cueBack();

    UFUNCTION(BlueprintCallable)
    double getFollowTime() { return followTime; }

    UFUNCTION(BlueprintCallable)
    double getCueTime() { return cueTime; }

private:
    UPROPERTY(VisibleInstanceOnly, Category="EOS", AdvancedDisplay)
    bool shift = false;

    UPROPERTY(VisibleInstanceOnly, Category="EOS", AdvancedDisplay)
    TSet<int> parkedChannels;
    
    UPROPERTY(VisibleInstanceOnly, Category="EOS", AdvancedDisplay)
    FCmdSelection activeSelection;

    UPROPERTY(VisibleInstanceOnly, Category="EOS", AdvancedDisplay)
    double followTime = -1;
    
    UPROPERTY(VisibleInstanceOnly, Category="EOS", AdvancedDisplay)
    double cueTime = 0;

    UPROPERTY(VisibleInstanceOnly, Category="Buttons", AdvancedDisplay)
    TMap<FName, UStaticMeshComponent*> buttonsByName;
    UPROPERTY()
    TMap<UStaticMeshComponent*, FName> buttonsByMesh;
    UPROPERTY()
    TMap<FName, UStaticMeshComponent*> indicatorsByName;

    UPROPERTY()
    TMap<FName, UTextRenderComponent*> textByName;

    UPROPERTY()
    TMap<FName, UEOSButton*> buttonByName;

    UPROPERTY()
    UStaticMeshComponent *scrollWheel;
    UPROPERTY()
    TMap<int, UStaticMeshComponent *> wheels;
    UPROPERTY()
    TMap<int, UStaticMeshComponent *> dials;
    UPROPERTY()
    TMap<UStaticMeshComponent*, int> wheelsByMesh;
    UPROPERTY()
    TMap<UStaticMeshComponent*, int> dialsByMesh;

    void setButtonColor(FName button, int r, int g, int b);
    void setButtonColor(FName button, FColor buttonColor, FColor activeColor);
    void setButtonActive(FName button, bool active);

    UPROPERTY(VisibleInstanceOnly, Category="Buttons", AdvancedDisplay)
    UMaterialInstanceDynamic* baseButtonMaterial;

    int getCmdNumber(int start, int* len);
    double getCmdNumberD(int start, int* len);
    FString getCmdNumberS(int start, int* len);
    FCmdSelection getCmdSelection(int start, int *end);

    UFUNCTION()
    void onNetworkPacket(UNetworkPacket *packet);

    UFUNCTION()
    void finishCommand(bool clear);
};
