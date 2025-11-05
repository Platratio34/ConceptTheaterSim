// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInteractionComponent.generated.h"


UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteract, UPrimitiveComponent*, component, bool, repeated);

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractEnd, UPrimitiveComponent*, component, float, interactionTime);

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractScroll, UPrimitiveComponent*, component, float, scrollDirection);

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

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FOnInteract onInteract;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FOnInteractEnd onInteractEnd;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FOnInteractScroll onInteractScroll;

protected:
    UPROPERTY(VisibleInstanceOnly)
    TMap<UPrimitiveComponent*, FText> actions;
    
    UPROPERTY(VisibleInstanceOnly)
    TMap<UPrimitiveComponent*, float> interactionTimes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText name;
};
