// Copyright Peter Crall 2025


#include "RiggingPosition.h"

#include "DrawDebugHelpers.h"

// Sets default values
ARiggingPosition::ARiggingPosition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(root);
    
    meshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    meshComponent->SetupAttachment(root);
}

void ARiggingPosition::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    meshComponent->SetStaticMesh(mesh);
    if(xAligned)
    {
        meshComponent->SetRelativeScale3D(FVector(meshLength, 1, 1));
    }
    else
    {
        meshComponent->SetRelativeScale3D(FVector(1, meshLength, 1));
    }
}

void ARiggingPosition::drawDebugBox(FVector pos, FQuat rot)
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
void ARiggingPosition::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARiggingPosition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ARiggingPosition::getMountPosition(FVector targetPosition)
{
    FTransform transform = root->GetComponentTransform();
    FVector localTPos = transform.InverseTransformPosition(targetPosition);
    FVector localUp = transform.InverseTransformVector(FVector(0,0,1));

    float a = localTPos.Z / localUp.Z;

    FVector outPos = FVector(0, 0, 0);
    if(xAligned)
    { // position will be ?, 0, 0 because we will move along the X-axis
        outPos.X = localTPos.X - (localUp.X * a);
    }
    else
    { // position will be 0, ?, 0 because we will move along the Y-axis
        outPos.Y = localTPos.Y - (localUp.Y * a);
    }
    return transform.TransformPosition(outPos);
}