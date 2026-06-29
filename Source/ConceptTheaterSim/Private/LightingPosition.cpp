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

    if(positions.Num() == 0) {
        FLightingPositionConfig p0;
        p0.minPosition = minPosition;
        p0.maxPosition = maxPosition;
        p0.radius = radius;
        p0.byAngle = byAngle;
        positions.Add(p0);
    }

    FRotator zero = FRotator(0,0,0);
    for (int index = 0; index < positions.Num(); index++) {
        FLightingPositionConfig &pc = positions[index];
        float step = pc.byAngle ? 5 : 12;
        if (pc.minPosition < 0 && pc.maxPosition > 0)
            drawDebugBox(getHangLocation(index, 0), getHangRotation(index, 0, zero));

        float start = pc.minPosition;
        if(start < 0)
            start = 0;
        start = start + (step - FMath::Fmod(start, step));
        for (float p = start; p < pc.maxPosition; p += step)
        {
            drawDebugBox(getHangLocation(index, p), getHangRotation(index, p, zero));
        }
        drawDebugBox(getHangLocation(index, pc.maxPosition), getHangRotation(index, pc.maxPosition, zero));

        start = pc.maxPosition;
        if(start > -step)
            start = -step;
        start = start - FMath::Fmod(start, step);
        for (float p = start; p > pc.minPosition; p -= step)
        {
            drawDebugBox(getHangLocation(index, p), getHangRotation(index, p, zero));
        }
        drawDebugBox(getHangLocation(index, pc.minPosition), getHangRotation(index, pc.minPosition, zero));
    }
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

FVector ALightingPosition::getHangLocation(int index, float position)
{
    if(positions.Num() == 0)
        return GetActorTransform().TransformPosition(FVector(position * 2.54, 0, 0));
    if(index < 0 || index >= positions.Num())
        index = 0;
    FLightingPositionConfig &pc = positions[index];
    if(pc.radius != 0)
    {
        // float theta = position / (3.141562653 * 2 * radius);
        float theta = pc.byAngle ? (position / 180 * 3.141592653) : (position / pc.radius);
        float x = FMath::Cos(theta);
        float y = FMath::Sin(theta);
        FVector pos = /*GetActorLocation();
        pos += */FVector(y * pc.radius * 2.54, -(1-x) * pc.radius * 2.54, 0);
        return GetActorTransform().TransformPosition(pos + (pc.offset*2.54));
    }
    return GetActorTransform().TransformPosition(FVector(position * 2.54, 0, 0) + (pc.offset*2.54));
}

FQuat ALightingPosition::getHangRotation(int index, float position, FRotator offset)
{
    if(positions.Num() == 0) {
        return (GetActorRotation() + offset).Quaternion();
    }
    if(index < 0 || index >= positions.Num())
        index = 0;
    FLightingPositionConfig &pc = positions[index];
    FRotator r = GetActorRotation();
    if(pc.radius != 0)
    {
        if(pc.byAngle) {
            r += FRotator(0, -position, 0);
        } else {
            r += FRotator(0, -(position / (3.141592653 * 2 * pc.radius)) * 360, 0);
        }
    }
    r += offset;
    return r.Quaternion();
}