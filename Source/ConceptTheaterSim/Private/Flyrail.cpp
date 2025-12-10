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
            loftBlockComponents[i]->DestroyComponent();
            loftBlockComponents.RemoveAt(i);
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
    }
}

void AFlyrail::updatePosition()
{
    if(position < 0)
        position = 0;
    if(position > maxPosition)
        position = maxPosition;

    arbor->SetRelativeLocation( FVector( 0, 0, 2.54 * ( position + arborOffsetHeight ) ) );
    
    setRopeLength(arborHandlineLower, (position + arborOffsetHeight - 6) - (tensionBlockHeight + 6));
    setRopeLength(arborHandlineUpper, headBlockHeight - ( arborOffsetHeight + position + 102 ));
}

// Called when the game starts or when spawned
void AFlyrail::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlyrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

