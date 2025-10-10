// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCableConnector.generated.h"

class CONCEPTTHEATERSIM_API ACCable;

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class CONCEPTTHEATERSIM_API UCCableConnector : public UActorComponent
{
    GENERATED_BODY()

public:
    
	UCCableConnector();
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
    bool input;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
    FText name;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
    FName id;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Default")
    ACCable *cable = nullptr;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UPrimitiveComponent *connector = nullptr;

    bool isConnected()
    {
        return cable != nullptr;
    }

    UFUNCTION()
    virtual void onConnect(ACCable* cable_);

    UFUNCTION()
    virtual void onDisconnect(ACCable* cable_);

private:
    
};