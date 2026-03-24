// Copyright Peter Crall 2025


#include "LightingPosition.h"

#include "DrawDebugHelpers.h"

// Sets default values
ALightingPosition::ALightingPosition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(root);
}

void ALightingPosition::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // Clear any previous persistent debug lines for this actor in the editor
    FlushPersistentDebugLines(GetWorld());

    float step = byAngle ? 5 : 12;

    if(minPosition < 0 && maxPosition > 0)
        drawDebugBox(getHangLocation(0), getHangRotation(0));

    float start = minPosition;
    if(start < 0)
        start = 0;
    start = start + (step - FMath::Fmod(start, step));
    for (float p = start; p < maxPosition; p += step)
    {
        drawDebugBox(getHangLocation(p), getHangRotation(p));
    }
    drawDebugBox(getHangLocation(maxPosition), getHangRotation(maxPosition));

    start = maxPosition;
    if(start > -step)
        start = -step;
    start = start - FMath::Fmod(start, step);
    for (float p = start; p > minPosition; p -= step)
    {
        drawDebugBox(getHangLocation(p), getHangRotation(p));
    }
    drawDebugBox(getHangLocation(minPosition), getHangRotation(minPosition));
}

void ALightingPosition::drawDebugBox(FVector pos, FQuat rot)
{
    DrawDebugBox(
        GetWorld(),                                        // World context
        pos,                                // Center of the box
        FVector(debugBoxSize, debugBoxSize, debugBoxSize), // Extent (half size)
        rot,                         // Rotation
        FColor::Red,                                       // Color
        true,                                              // bPersistentLines (visible in editor, not just PIE)
        -1.0f,                                             // LifeTime (seconds, -1 means infinite)
        0,                                                 // DepthPriority
        1.0f                                              // Thickness
    );
}

// Called when the game starts or when spawned
void ALightingPosition::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightingPosition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ALightingPosition::getHangLocation(float position)
{
    if(radius != 0)
    {
        // float theta = position / (3.141562653 * 2 * radius);
        float theta = byAngle ? (position / 180 * 3.141592653) : (position / radius);
        float x = FMath::Cos(theta);
        float y = FMath::Sin(theta);
        FVector pos = GetActorLocation();
        pos += FVector(y * radius * 2.54, -(1-x) * radius * 2.54, 0);
        return pos;
    }
    return GetActorLocation() + FVector(position * 2.54, 0, 0);
}

FQuat ALightingPosition::getHangRotation(float position)
{
    if(radius != 0)
    {
        FRotator r = GetActorRotation();
        if(byAngle) {
            r += FRotator(0, -position, 0);
        } else {
            r += FRotator(0, -(position / (3.141592653 * 2 * radius)) * 360, 0);
        }
        return r.Quaternion();
    }
    return GetActorRotation().Quaternion();
}