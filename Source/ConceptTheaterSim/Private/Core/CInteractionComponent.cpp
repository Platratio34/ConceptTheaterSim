// Copyright Peter Crall 2025


#include "Core/CInteractionComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCInteractionComponent::UCInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
    
	
}


// Called every frame
void UCInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // !! Tick disabled in constructor !!
}

void UCInteractionComponent::interact(UPrimitiveComponent *targetComponent, bool repeated)
{
    if(!interactionTimes.Contains(targetComponent))
        interactionTimes.Add(targetComponent, UGameplayStatics::GetTimeSeconds(GetWorld()));
    onInteract.Broadcast(targetComponent, repeated);
}

void UCInteractionComponent::endInteract(UPrimitiveComponent *targetComponent)
{
    float time = 0;
    if(interactionTimes.Contains(targetComponent))
        time = interactionTimes[targetComponent];
    interactionTimes.Remove(targetComponent);
    onInteractEnd.Broadcast(targetComponent, time);
}

void UCInteractionComponent::interactScroll(UPrimitiveComponent *targetComponent, float scroll)
{
    onInteractScroll.Broadcast(targetComponent, scroll);
}

FText UCInteractionComponent::getAction(UPrimitiveComponent *targetComponent)
{
    if(FText* p = actions.Find(targetComponent))
    {
        return *p;
    }
    return FText::FromString("Interact");
}

FText UCInteractionComponent::getName(UPrimitiveComponent *targetComponent)
{
    return name;
}


void UCInteractionComponent::addComponent(UPrimitiveComponent *component, FText action)
{
    actions.Add(component, action);
    if(!component->ComponentHasTag(FName("Interactable")))
        UE_LOG(LogTemp, Warning, TEXT("Component added to interactable missing tag: %s -> %s"), *component->GetOwner()->GetName(), *component->GetName());
}

void UCInteractionComponent::updateAction(UPrimitiveComponent *targetComponent, FText action)
{
    actions.Add(targetComponent, action);
}
