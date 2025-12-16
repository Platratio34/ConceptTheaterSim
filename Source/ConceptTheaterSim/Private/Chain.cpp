// Copyright Peter Crall 2025.


#include "Chain.h"

// Sets default values
AChain::AChain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(root);
}

void AChain::OnConstruction(const FTransform &Transform)
{
    if(length < 0)
        length = 0;
    float l = 0;
    int linkI = 0;
    if(linkLength < 0.1) // This is just a safety to prevent infinite loop
        linkLength = 0.1;
    while(l < length)
    {
        UStaticMeshComponent *mesh = nullptr;
        if(links.Num() <= linkI)
        {
            mesh = NewObject<UStaticMeshComponent>(this);
            mesh->RegisterComponent();
            mesh->CreationMethod = EComponentCreationMethod::Instance;
            links.Add(mesh);
        }
        else
        {
            mesh = links[linkI];
            if(mesh == nullptr || !IsValid(mesh))
            {
                mesh = NewObject<UStaticMeshComponent>(this);
                links[linkI] = mesh;
                mesh->RegisterComponent();
                mesh->CreationMethod = EComponentCreationMethod::Instance;
            }
        }

        mesh->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
        mesh->SetRelativeLocation(FVector(0, 0, 2.54 * l));
        mesh->SetRelativeRotation(FRotator(0, (linkI % 2 == 1) ? 90 : 0, 0));
        mesh->SetStaticMesh(linkMesh);

        l += linkLength;
        linkI++;
    }
    for (int i = links.Num()-1; i >= linkI; i--)
    {
        if(links[i] != nullptr && IsValid(links[i]))
            links[i]->DestroyComponent();
        links.RemoveAt(i);
    }
}

// Called when the game starts or when spawned
void AChain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChain::setLength(float newLength)
{
    if(newLength < 0)
        newLength = 0;
    length = newLength;
    float l = 0;
    int linkI = 0;
    if(linkLength < 0.1) // This is just a safety to prevent infinite loop
        linkLength = 0.1;
    while(l < length)
    {
        UStaticMeshComponent *mesh = nullptr;
        if(links.Num() <= linkI)
        {
            mesh = NewObject<UStaticMeshComponent>(this);
            mesh->RegisterComponent();
            mesh->CreationMethod = EComponentCreationMethod::Instance;
            links.Add(mesh);
        }
        else
        {
            mesh = links[linkI];
            if(mesh == nullptr || !IsValid(mesh))
            {
                mesh = NewObject<UStaticMeshComponent>(this);
                links[linkI] = mesh;
                mesh->RegisterComponent();
                mesh->CreationMethod = EComponentCreationMethod::Instance;
            }
        }

        mesh->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
        mesh->SetRelativeLocation(FVector(0, 0, 2.54 * l));
        mesh->SetRelativeRotation(FRotator(0, (linkI % 2 == 1) ? 90 : 0, 0));
        mesh->SetStaticMesh(linkMesh);

        l += linkLength;
        linkI++;
    }
    for (int i = links.Num()-1; i >= linkI; i--)
    {
        if(links[i] != nullptr && IsValid(links[i]))
            links[i]->DestroyComponent();
        links.RemoveAt(i);
    }
}