// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Power/CBreakerConfig.h"
#include "CBreaker.generated.h"


UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCircuitChange, FName, circuit, double, state);

/**
 * 
 */
UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API ACBreaker : public AActor
{
	GENERATED_BODY()
	
public:
    ACBreaker();
    ~ACBreaker();

    void OnConstruction(const FTransform &Transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void setState(FName circuit, double newState);
    
    UFUNCTION(BlueprintCallable)
    double getState(FName circuit);

    UPROPERTY(VisibleInstanceOnly, BlueprintAssignable, Category="Breaker")
    FOnCircuitChange onCircuitChange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breaker")
    FName name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breaker")
    UCBreakerConfig *config;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Breaker")
    TMap<FName, double> state;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breaker")
    bool switched = true;

private:

};