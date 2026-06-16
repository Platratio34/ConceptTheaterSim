// Copyright Peter Crall 2025.


#include "Flyrail.h"

// Sets default values
AFlyrail::AFlyrail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(root);

    headBlock = CreateDefaultSubobject<USceneComponent>(TEXT("HeadBlock"));
    headBlock->SetupAttachment(root);

    arbor = CreateDefaultSubobject<USceneComponent>(TEXT("Arbor"));
    arbor->SetupAttachment(root);

    tensionBlock = CreateDefaultSubobject<USceneComponent>(TEXT("TensionBlock"));
    tensionBlock->SetupAttachment(root);
    

    frontHandline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontHandline"));
    frontHandline->SetupAttachment(tensionBlock);

    arborHandlineLower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArborHandlineLower"));
    arborHandlineLower->SetupAttachment(tensionBlock);

    arborHandlineUpper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArborHandlineUpper"));
    arborHandlineUpper->SetupAttachment(arbor);

    upperBumper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpperBumper"));
    upperBumper->SetupAttachment(root);
}

void AFlyrail::setRopeLength(UStaticMeshComponent* component, float length)
{
    component->SetRelativeScale3D( FVector( 1, 1, length / 12 ));
}

void AFlyrail::OnConstruction(const FTransform &Transform)
{
    headBlock->SetRelativeLocation( FVector( 0, 0, 2.54 * headBlockHeight) );
    tensionBlock->SetRelativeLocation( FVector( 0, 0, 2.54 * tensionBlockHeight ) );
    upperBumper->SetRelativeLocation( FVector( 0, 0, 2.54 * ( maxPosition + arborOffsetHeight + 96 ) ) );

    setRopeLength(frontHandline, headBlockHeight - (tensionBlockHeight + 6));

    lastPosition = -1;
    updatePosition();

    if(weightBlocks.Num() < weights.Num())
    {
        for (int i = weightBlocks.Num(); i < weights.Num(); i++)
        {
            weightBlocks.Add(NewObject<UStaticMeshComponent>(this));
        }
    }
    else if(weightBlocks.Num() > weights.Num())
    {
        for (int i = weightBlocks.Num()-1; i >= weights.Num(); i--)
        {
            weightBlocks[i]->DestroyComponent();
            weightBlocks.RemoveAt(i);
        }
    }
    float wH = 0;
    for (int i = 0; i < weights.Num(); i++)
    {
        UStaticMeshComponent *meshComp = weightBlocks[i];
        if(meshComp == nullptr || !IsValid(meshComp))
        {
            meshComp = NewObject<UStaticMeshComponent>(this);
            weightBlocks[i] = meshComp;
            meshComp->RegisterComponent();
            // meshComp->SetupAttachment(arbor);
            meshComp->CreationMethod = EComponentCreationMethod::Instance;
        }
        meshComp->AttachToComponent(arbor, FAttachmentTransformRules::KeepRelativeTransform);
        meshComp->SetRelativeLocation(FVector(0, 8 * 2.54, 2.54 * wH));
        meshComp->SetRelativeScale3D(FVector(((i%2==1) ? -1 : 1), 1, 1));
        wH += (weights[i] == EFlyrailWeightType::LARGE) ? 2 : 1;
        meshComp->SetStaticMesh(weightMeshes[weights[i]]);
    }

    if(loftBlockComponents.Num() < loftBlocks.Num())
    {
        for (int i = loftBlockComponents.Num(); i < loftBlocks.Num(); i++)
        {
            loftBlockComponents.Add(NewObject<UStaticMeshComponent>(this));
        }
    }
    else if(loftBlockComponents.Num() > loftBlocks.Num())
    {
        for (int i = loftBlockComponents.Num()-1; i >= loftBlocks.Num(); i--)
        {
            if(IsValid(loftBlockComponents[i]))
                loftBlockComponents[i]->DestroyComponent();
            loftBlockComponents.RemoveAt(i);
        }
    }
    if(loftBlockRopeComponents.Num() < loftBlocks.Num())
    {
        for (int i = loftBlockRopeComponents.Num(); i < loftBlocks.Num(); i++)
        {
            loftBlockRopeComponents.Add(NewObject<UStaticMeshComponent>(this));
        }
    }
    else if(loftBlockRopeComponents.Num() > loftBlocks.Num())
    {
        for (int i = loftBlockRopeComponents.Num()-1; i >= loftBlocks.Num(); i--)
        {
            if(IsValid(loftBlockRopeComponents[i]))
                loftBlockRopeComponents[i]->DestroyComponent();
            loftBlockRopeComponents.RemoveAt(i);
        }
    }
    for (int i = 0; i < loftBlocks.Num(); i++)
    {
        FFlyrailLoftBlock *block = &loftBlocks[i];
        UStaticMeshComponent *meshComp = loftBlockComponents[i];
        if(meshComp == nullptr || !IsValid(meshComp))
        {
            meshComp = NewObject<UStaticMeshComponent>(this);
            loftBlockComponents[i] = meshComp;
            meshComp->RegisterComponent();
            meshComp->CreationMethod = EComponentCreationMethod::Instance;
        }
        meshComp->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
        meshComp->SetRelativeLocation(block->position * 2.54);
        meshComp->SetRelativeRotation(block->rotation);
        meshComp->SetStaticMesh(block->loftBlockMesh != nullptr ? block->loftBlockMesh : loftBlockMesh);

        UStaticMeshComponent *meshRopeComp = loftBlockRopeComponents[i];
        if(meshRopeComp == nullptr || !IsValid(meshRopeComp))
        {
            meshRopeComp = NewObject<UStaticMeshComponent>(this);
            loftBlockRopeComponents[i] = meshRopeComp;
            meshRopeComp->RegisterComponent();
            meshRopeComp->CreationMethod = EComponentCreationMethod::Instance;
        }
        meshRopeComp->AttachToComponent(meshComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
        // meshComp->SetRelativeLocation(block->position * 2.54);
        // meshComp->SetRelativeRotation(block->rotation);
        meshRopeComp->SetStaticMesh(block->loftBlockRopeMesh != nullptr ? block->loftBlockRopeMesh : loftBlockRopeMesh);
    }

    int numLoftBlocks = loftBlocks.Num();
    for (int i = 0; i < headBlockLines.Num(); i++) {
        if(i >= numLoftBlocks) {
            headBlockLines[i]->SetVisibility(false);
            continue;
        }
        headBlockLines[i]->SetVisibility(true);
    }
    int numArborLineEnds = arborLineEnds.Num();
    for (int i = 0; i < arborLines.Num(); i++) {
        USplineMeshComponent *comp = arborLines[i];
        if(i >= numLoftBlocks || i >= numArborLineEnds) {
            comp->SetVisibility(false);
            continue;
        }
        comp->SetVisibility(true);
        comp->SetEndPosition(comp->GetComponentTransform().InverseTransformPosition(arborLineEnds[i]->GetComponentLocation()), true);
    }

    if(hasRedirectBlock) {
        if(redirectBlockComponent == nullptr || !IsValid(redirectBlockComponent)) {
            redirectBlockComponent = NewObject<UChildActorComponent>(this);
            redirectBlockComponent->RegisterComponent();
            redirectBlockComponent->CreationMethod = EComponentCreationMethod::Instance;
        }

        redirectBlockComponent->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
        redirectBlockComponent->SetRelativeLocation(redirectBlock.position * 2.54);
        redirectBlockComponent->SetRelativeRotation(redirectBlock.rotation);
        redirectBlockComponent->SetChildActorClass(redirectBlockClass);

        redirectBlockActor = Cast<AFlyrailRedirectBlock>(redirectBlockComponent->GetChildActor());
        redirectBlockActor->setup(loftBlockComponents);

        for (int i = 0; i < loftBlockLines.Num(); i++) {
            USplineMeshComponent *comp = loftBlockLines[i];
            if(i >= numLoftBlocks || redirectBlockActor == nullptr || i >= redirectBlockActor->inLineEnds.Num()) {
                comp->SetVisibility(false);
                continue;
            }
            comp->SetVisibility(true);
            FVector ePos = redirectBlockActor->inLineEnds[i]->GetComponentLocation();
            comp->SetEndPosition(comp->GetComponentTransform().InverseTransformPosition(ePos), true);
        }
    } else {
        for (int i = 0; i < loftBlockLines.Num(); i++) {
            USplineMeshComponent *comp = loftBlockLines[i];
            if(i >= numLoftBlocks) {
                comp->SetVisibility(false);
                continue;
            }
            comp->SetVisibility(true);
            FVector ePos = loftBlockComponents[i]->GetComponentTransform().TransformPosition(FVector(0, -3 * 2.54, 7 * 2.54));
            comp->SetEndPosition(comp->GetComponentTransform().InverseTransformPosition(ePos), true);
        }
    }
    // int loftBlockI = 0;
    // int redirectNum = 0;
    // int loftBlockLineI = 0;
    // for (int i = 0; i < loftBlocks.Num(); i++)
    //     FFlyrailLoftBlock &block = loftBlocks[loftBlockI];
    //     // USplineMeshComponent *comp = loftBlockLines[i];
    //     // if(loftBlockNum >= numLoftBlocks) {
    //     //     comp->SetVisibility(false);
    //     //     continue;
    //     // }
    //     // comp->SetVisibility(true);
    //     // if(block.redirectIn.Num() >= redirectNum) {

    //     // } else {

    //     // }
    // }
}

void AFlyrail::updatePosition()
{
    if(position < 0)
        position = 0;
    if(position > maxPosition)
        position = maxPosition;
    
    if((position < (lastPosition + 0.05)) && (position > (lastPosition - 0.05)))
        return;
    lastPosition = position;

    float arborPos = (maxPosition - position) + arborOffsetHeight;
    arbor->SetRelativeLocation( FVector( 0, 0, 2.54 * arborPos ) );
    
    setRopeLength(arborHandlineLower, (arborPos - 6) - (tensionBlockHeight + 6));
    setRopeLength(arborHandlineUpper, headBlockHeight - ( arborPos + 102 ));

    if(battonActor != nullptr) {
        FVector cPos = battonActor->GetActorLocation();
        cPos.Z = (battonMinHeight + position) * 2.54;
        battonActor->SetActorLocation(cPos);
    }

    int numLoftBlocks = loftBlocks.Num();
    int numArborLineEnds = arborLineEnds.Num();
    for (int i = 0; i < arborLines.Num(); i++) {
        if(i >= numLoftBlocks || i >= numArborLineEnds)
            break;
        arborLines[i]->SetEndPosition(arborLines[i]->GetComponentTransform().InverseTransformPosition(arborLineEnds[i]->GetComponentLocation()), true);
    }
}

void AFlyrail::setPosition(float newPosition) {
    position = newPosition;
    if(position < 0)
        position = 0;
    if(position > maxPosition)
        position = maxPosition;

    updatePosition();
}

// Called when the game starts or when spawned
void AFlyrail::BeginPlay()
{
	Super::BeginPlay();
	
    if(hasRedirectBlock) {
        redirectBlockActor = Cast<AFlyrailRedirectBlock>(redirectBlockComponent->GetChildActor());
        redirectBlockActor->setup(loftBlockComponents);
    }
}

// Called every frame
void AFlyrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
AFlyrailRedirectBlock
*/
AFlyrailRedirectBlock::AFlyrailRedirectBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(root);
}

void AFlyrailRedirectBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlyrailRedirectBlock::OnConstruction(const FTransform &Transform)
{

}

void AFlyrailRedirectBlock::setup(TArray<UStaticMeshComponent*>& loftBlocks) {
    int numLoftBlocks = loftBlocks.Num();
    for (int i = 0; i < blockLines.Num(); i++) {
        UStaticMeshComponent *comp = blockLines[i];
        if(!IsValid(comp))
            continue;
        if (i > numLoftBlocks) {
            comp->SetVisibility(false);
            continue;
        }
        comp->SetVisibility(true);
    }
    for (int i = 0; i < outLines.Num(); i++) {
        USplineMeshComponent *comp = outLines[i];
        if(!IsValid(comp))
            continue;
        if (i > numLoftBlocks) {
            comp->SetVisibility(false);
            continue;
        }
        comp->SetVisibility(true);
        FVector ePos = loftBlocks[i]->GetComponentTransform().TransformPosition(FVector(0, -3 * 2.54, 7 * 2.54));
        comp->SetEndPosition(comp->GetComponentTransform().InverseTransformPosition(ePos), true);
    }
}