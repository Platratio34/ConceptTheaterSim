// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Framework/Application/SlateApplication.h"
#include "CInteractionComponent.generated.h"


UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteract, UPrimitiveComponent*, component, bool, repeated);

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractEnd, UPrimitiveComponent*, component, float, interactionTime);

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractScroll, UPrimitiveComponent*, component, float, scrollDirection);

USTRUCT(BlueprintType)
struct FOptionalText {
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    bool present = false;
    
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    FText text;
};

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(FOptionalText, FOnGetText, UPrimitiveComponent *, component);

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputOM, FKeyEvent, event);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CONCEPTTHEATERSIM_API UCInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void interact(UPrimitiveComponent *targetComponent, bool repeated);

    UFUNCTION(BlueprintCallable)
    void endInteract(UPrimitiveComponent *targetComponent);

    UFUNCTION(BlueprintCallable)
    void interactScroll(UPrimitiveComponent *targetComponent, float scroll);

    UFUNCTION(BlueprintCallable)
    FText getAction(UPrimitiveComponent *targetComponent);

    UFUNCTION(BlueprintCallable)
    FText getName(UPrimitiveComponent *targetComponent);

    UFUNCTION(BlueprintCallable)
    void addComponent(UPrimitiveComponent *component, FText action);
    
    UFUNCTION(BlueprintCallable)
    void updateAction(UPrimitiveComponent *targetComponent, FText action);

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, BlueprintAssignable)
    FOnInteract onInteract;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, BlueprintAssignable)
    FOnInteractEnd onInteractEnd;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, BlueprintAssignable)
    FOnInteractScroll onInteractScroll;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FOnGetText onGetAction;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FOnGetText onGetName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool supportsObjectMode = false;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    bool objectMode = true;

    UFUNCTION(BlueprintCallable)
    void setObjectMode(bool active);

    UFUNCTION(BlueprintCallable)
    void inputOM(FKeyEvent event);
    
    UFUNCTION(BlueprintCallable)
    void inputUpOM(FKeyEvent event);
    
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, BlueprintAssignable)
    FOnInputOM onInputOM;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, BlueprintAssignable)
    FOnInputOM onInputUpOM;

protected:
    UPROPERTY(VisibleInstanceOnly)
    TMap<UPrimitiveComponent*, FText> actions;
    
    UPROPERTY(VisibleInstanceOnly)
    TMap<UPrimitiveComponent*, float> interactionTimes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText name;
};
