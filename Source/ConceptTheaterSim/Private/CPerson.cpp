// Copyright Peter Crall 2025


#include "CPerson.h"

// Sets default values
ACPerson::ACPerson()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    rootComponent2 = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent2"));
    SetRootComponent(rootComponent2);
    bodyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BodyRoot"));
    bodyRoot->SetupAttachment(rootComponent2);

    legMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Leg Mesh Component"));
    legMeshComponent->SetupAttachment(bodyRoot);
    chestMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chest Mesh Component"));
    chestMeshComponent->SetupAttachment(bodyRoot);

    armMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arm Mesh Component"));
    armMeshComponent->SetupAttachment(bodyRoot);
    headMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head Mesh Component"));
    headMeshComponent->SetupAttachment(bodyRoot);
    eyeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eye Mesh Component"));
    eyeMeshComponent->SetupAttachment(bodyRoot);
    earMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ear Mesh Component"));
    earMeshComponent->SetupAttachment(bodyRoot);
    hairMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair Mesh Component"));
    hairMeshComponent->SetupAttachment(bodyRoot);
}

void ACPerson::OnConstruction(const FTransform &Transform)
{
    updateMeshes();
    updateHeight();
}

void ACPerson::updateHeight()
{
    float hScale = height / defaultHeight;
    rootComponent2->SetRelativeScale3D(FVector(hScale, hScale, hScale));
}
void ACPerson::updateMeshes()
{
    if(skinMaterialInstance == nullptr || !IsValid(skinMaterialInstance))
    {
        skinMaterialInstance = UMaterialInstanceDynamic::Create(skinMaterial, this);
    }
    skinMaterialInstance->SetVectorParameterValue(FName("Skin Color"), skinColor);
    legMeshComponent->SetMaterial(0, skinMaterialInstance);
    chestMeshComponent->SetMaterial(0, skinMaterialInstance);
    armMeshComponent->SetMaterial(0, skinMaterialInstance);
    headMeshComponent->SetMaterial(0, skinMaterialInstance);
    earMeshComponent->SetMaterial(0, skinMaterialInstance);

    EPersonLegType legType = getLegType();
    if(UStaticMesh** mesh = legMeshes.Find(legType))
        legMeshComponent->SetStaticMesh(*mesh);
    if(UStaticMesh** mesh = bodyMeshes.Find(bodyType))
        chestMeshComponent->SetStaticMesh(*mesh);
    float hOffset = 0;
    if(legType == EPersonLegType::FEMALE_HEEL_1)
    {
        hOffset = 2.54;
    }
    else if(legType == EPersonLegType::FEMALE_HEEL_2)
    {
        hOffset = 2.54 * 3;
    }
    bodyRoot->SetRelativeLocation(FVector(0, 0, hOffset));

    if(armMesh != nullptr)
        armMeshComponent->SetStaticMesh(armMesh);
    if(headMesh != nullptr)
        headMeshComponent->SetStaticMesh(headMesh);
    if(eyeMesh != nullptr)
        eyeMeshComponent->SetStaticMesh(eyeMesh);
    if(eyeMaterial != nullptr)
    {
        if(eyeMaterialInstance == nullptr || !IsValid(eyeMaterialInstance))
        {
            eyeMaterialInstance = UMaterialInstanceDynamic::Create(eyeMaterial, this);
        }
        eyeMeshComponent->SetMaterial(0, eyeMaterialInstance);
        eyeMaterialInstance->SetVectorParameterValue(FName("EyeColor"), eyeColor);
    }
    if(earMesh != nullptr)
        earMeshComponent->SetStaticMesh(earMesh);

    hairType = FMath::Clamp(hairType, -1, hairMeshes.Num() - 1);
    if(hairType >= 0)
    {
        if(hairMeshes.Num() > 0 && hairMeshes[hairType] != nullptr)
            hairMeshComponent->SetStaticMesh(hairMeshes[hairType]);
        hairMeshComponent->SetVisibility(true);
        if(hairMaterialInstance == nullptr || !IsValid(hairMaterialInstance))
        {
            hairMaterialInstance = UMaterialInstanceDynamic::Create(hairMaterial, this);
        }
        hairMaterialInstance->SetVectorParameterValue(FName("HairColor"), hairColor);
        hairMeshComponent->SetMaterial(0, hairMaterialInstance);
    }
    else
        hairMeshComponent->SetVisibility(false);

    // clothing update
    // First make sure our arrays are the same size
    if(clothingMeshComponents.Num() > clothing.Num())
    {
        for (int i = clothingMeshComponents.Num()-1; i >= clothing.Num(); i--)
        {
            clothingMeshComponents[i]->DestroyComponent();
            clothingMeshComponents.RemoveAt(i);
        }
    }
    else if(clothingMeshComponents.Num() < clothing.Num())
    {
        for (int i = clothingMeshComponents.Num(); i < clothing.Num(); i++)
        {
            UStaticMeshComponent *component = NewObject<UStaticMeshComponent>(this);
            clothingMeshComponents.Add(component);
            component->AttachToComponent(bodyRoot, FAttachmentTransformRules::KeepRelativeTransform);
            component->RegisterComponent();
            component->CreationMethod = EComponentCreationMethod::Instance;
        }
    }
    if(clothingMeshComponentsVisible.Num() > clothing.Num())
    {
        for (int i = clothingMeshComponentsVisible.Num()-1; i >= clothing.Num(); i--)
        {
            clothingMeshComponentsVisible.Remove(i);
        }
    }
    else if(clothingMeshComponentsVisible.Num() < clothing.Num())
    {
        for (int i = clothingMeshComponentsVisible.Num(); i < clothing.Num(); i++)
        {
            clothingMeshComponentsVisible.Add(i, false);
        }
    }
    // then actually update the clothing meshes
    for (int i = 0; i < clothing.Num(); i++)
    {
        FPersonClothing c = clothing[i];
        UPersonClothingAsset* clothingAsset = c.asset;
        UStaticMeshComponent *meshComponent = clothingMeshComponents[i];
        if(meshComponent == nullptr || !IsValid(meshComponent))
        {
            meshComponent = NewObject<UStaticMeshComponent>(this);
            clothingMeshComponents[i] = meshComponent;
            meshComponent->AttachToComponent(bodyRoot, FAttachmentTransformRules::KeepRelativeTransform);
            meshComponent->RegisterComponent();
            meshComponent->CreationMethod = EComponentCreationMethod::Instance;
        }
        if(clothingAsset == nullptr)
        {
            meshComponent->SetVisibility(false);
            clothingMeshComponentsVisible[i] = false;
            continue;
        }
        UStaticMesh *mesh = nullptr;
        if(clothingAsset->hasLegVariants)
        {
            if(clothingAsset->legMeshes.Contains(legType))
                mesh = clothingAsset->legMeshes[legType];
        }
        else
        {
            if(clothingAsset->meshes.Contains(bodyType))
                mesh = clothingAsset->meshes[bodyType];
        }
        if(mesh != nullptr)
        {
            meshComponent->SetStaticMesh(mesh);
            meshComponent->SetVisibility(true);
            clothingMeshComponentsVisible[i] = true;
            for (int index = 0; index < clothingAsset->defaultMaterials.Num(); index++)
            {
                meshComponent->SetMaterial(index, clothingAsset->getMaterial(index, c.materialOverrides));
            }
        }
        else
        {
            meshComponent->SetVisibility(false);
            clothingMeshComponentsVisible[i] = false;
        }
    }
}

// Called when the game starts or when spawned
void ACPerson::BeginPlay()
{
	Super::BeginPlay();

    updateMeshes();
    updateHeight();
	
}

// Called every frame
void ACPerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPerson::setVisibility(bool newVisibility)
{
    legMeshComponent->SetVisibility(newVisibility);
    chestMeshComponent->SetVisibility(newVisibility);
    armMeshComponent->SetVisibility(newVisibility);
    headMeshComponent ->SetVisibility(newVisibility);
    eyeMeshComponent->SetVisibility(newVisibility);
    earMeshComponent->SetVisibility(newVisibility);
    if(hairType >= 0)
        hairMeshComponent->SetVisibility(newVisibility);
    for (int i = 0; i < clothingMeshComponents.Num(); i++)
    {
        if(clothingMeshComponentsVisible[i])
        {
            clothingMeshComponents[i]->SetVisibility(newVisibility);
        }
    }
}