// Copyright Peter Crall 2025


#include "CPerson.h"

static void setMeshVisible(UStaticMeshComponent* mesh, bool visible)
{
    mesh->SetVisibility(visible);
    mesh->SetCollisionEnabled(visible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

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

    animationComponent = CreateDefaultSubobject<UCAnimationComponent>(TEXT("Animation Component"));
}

void ACPerson::OnConstruction(const FTransform &Transform)
{
    updateMeshes();
    updateHeight();
}

void ACPerson::updateHeight()
{
    float hScale = height / defaultHeight;
    float wScale = (hScale - 1) * 0.75 + 1;
    bodyRoot->SetRelativeScale3D(FVector(wScale, wScale, hScale));
    float hOffset = 0;
    if(legType == EPersonLegType::FEMALE_HEEL_1)
    {
        hOffset = 2.54;
    }
    else if(legType == EPersonLegType::FEMALE_HEEL_2)
    {
        hOffset = 2.54 * 3;
    }
    hOffset *= hScale;
    bodyRoot->SetRelativeLocation(FVector(0, 0, hOffset));
}

EPersonLegType ACPerson::getLegType()
{
    if(bodyType == EPersonBodyType::MALE)
    {
        return EPersonLegType::MALE;
    }
    EPersonLegType type = EPersonLegType::FEMALE;
    for (int i = 0; i < clothing.Num(); i++)
    {
        FPersonClothing c = clothing[i];
        if(bool*p = hiddenClothing.Find(c.name)) // skipp hidden clothing items
            if(*p == true)
                continue;
        UPersonClothingAsset *asset = c.asset;
        if(asset == nullptr)
            continue;
        if(asset->requiredLeg != EPersonLegType::NONE)
        {
            return asset->requiredLeg;
        }
    }
    switch (bodyType)
    {
    case EPersonBodyType::MALE:
        return EPersonLegType::MALE;
    case EPersonBodyType::FEMALE:
        return EPersonLegType::FEMALE;
    case EPersonBodyType::FEMALE_2:
        return EPersonLegType::FEMALE;

    default:
        return EPersonLegType::MALE;
    }
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
    
    chestCoverage = 0;
    armCoverage = 0;
    legCoverage = 0;
    clothingHides.Empty();
    for(FPersonClothing c : clothing)
    {
        bool hidden = false;
        if(bool*p = hiddenClothing.Find(c.name))
            hidden = *p;
        if(hidden || c.asset == nullptr)
            continue;
        clothingHides.Append(c.asset->hides);
        chestCoverage |= c.asset->chestCoverage;
        armCoverage |= c.asset->armCoverage;
        legCoverage |= c.asset->legCoverage;
    }

    legType = getLegType();
    if(FPersonMeshSet* pLegSet = legMeshes.Find(legType))
    {
        // UE_LOG(LogTemp, Warning, TEXT("Searching for leg %d"), (int)legType);
        UStaticMesh *legMesh = nullptr;
        if(UStaticMesh** p2 = (*pLegSet).meshes.Find(legCoverage))
        {
            legMesh = *p2;
        }
        else if(UStaticMesh** p2d = (*pLegSet).meshes.Find(0))
        {
            legMesh = *p2d;
        }
        if(legMesh != nullptr)
        {
            // UE_LOG(LogTemp, Warning, TEXT("Found %s"), *(legMesh->GetName()));
            legMeshComponent->SetStaticMesh(legMesh);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Missing mesh for leg coverage %d"), legCoverage);
        }
    }
    if(FPersonMeshSet* pChestSet = bodyMeshes.Find(bodyType))
    {
        UStaticMesh *chestMesh = nullptr;
        if(UStaticMesh** p2 = (*pChestSet).meshes.Find(chestCoverage))
        {
            chestMesh = *p2;
        }
        else if(UStaticMesh** p2d = (*pChestSet).meshes.Find(0))
        {
            chestMesh = *p2d;
        }
        if(chestMesh != nullptr)
        {
            chestMeshComponent->SetStaticMesh(chestMesh);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Missing mesh for chest coverage %d"), chestCoverage);
        }
    }

    UStaticMesh *armMesh = nullptr;
    if(UStaticMesh** p3 = armMeshes.meshes.Find(armCoverage))
    {
        armMesh = *p3;
    }
    else if(UStaticMesh** p3d = armMeshes.meshes.Find(0))
    {
        armMesh = *p3d;
    }
    if(armMesh != nullptr)
    {
        armMeshComponent->SetStaticMesh(armMesh);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Missing mesh for arm coverage %d"), armCoverage);
    }
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
        setMeshVisible(hairMeshComponent, true);
        if(hairMaterialInstance == nullptr || !IsValid(hairMaterialInstance))
        {
            hairMaterialInstance = UMaterialInstanceDynamic::Create(hairMaterial, this);
        }
        hairMaterialInstance->SetVectorParameterValue(FName("HairColor"), hairColor);
        hairMeshComponent->SetMaterial(0, hairMaterialInstance);
    }
    else
        setMeshVisible(hairMeshComponent, false);

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
        bool hidden = false;
        if(bool*p = hiddenClothing.Find(c.name))
            hidden = *p;
        if(meshComponent == nullptr || !IsValid(meshComponent))
        {
            meshComponent = NewObject<UStaticMeshComponent>(this);
            clothingMeshComponents[i] = meshComponent;
            meshComponent->AttachToComponent(bodyRoot, FAttachmentTransformRules::KeepRelativeTransform);
            meshComponent->RegisterComponent();
            meshComponent->CreationMethod = EComponentCreationMethod::Instance;
        }
        if(clothingAsset == nullptr || hidden || clothingHides.Contains(clothingAsset))
        {
            setMeshVisible(meshComponent, false);
            clothingMeshComponentsVisible[i] = false;
            continue;
        }
        UStaticMesh *mesh = nullptr;
        if(clothingAsset->hasLegVariants)
        {
            if(clothingAsset->legMeshes.Contains(legType))
                mesh = FPersonMeshSet::GetMesh(clothingAsset->legMeshes[legType], legCoverage);
        }
        else
        {
            mesh = clothingAsset->getMesh(bodyType);
        }
        if(mesh != nullptr)
        {
            meshComponent->SetStaticMesh(mesh);
            setMeshVisible(meshComponent, true);
            clothingMeshComponentsVisible[i] = true;
            for (int index = 0; index < clothingAsset->defaultMaterials.Num(); index++)
            {
                meshComponent->SetMaterial(index, clothingAsset->getMaterial(index, c.materialOverrides, skinMaterialInstance));
            }
        }
        else
        {
            setMeshVisible(meshComponent, false);
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

    animationComponent->dummy = dummy;
    if(!dummy)
    animationComponent->registerWithMaster(name);
    animationComponent->onVisibilityChange.AddDynamic(this, &ACPerson::setVisibility);
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
    setMeshVisible(legMeshComponent, newVisibility);
    setMeshVisible(chestMeshComponent, newVisibility);
    setMeshVisible(armMeshComponent, newVisibility);
    setMeshVisible(headMeshComponent, newVisibility);
    setMeshVisible(eyeMeshComponent, newVisibility);
    setMeshVisible(earMeshComponent, newVisibility);
    if(hairType >= 0)
        setMeshVisible(hairMeshComponent, newVisibility);
    for (int i = 0; i < clothingMeshComponents.Num(); i++)
    {
        bool hidden = false;
        if(clothingMeshComponentsVisible[i])
        {
            setMeshVisible(clothingMeshComponents[i], newVisibility);
        }
    }

    TArray<AActor *> attachedActors;
    GetAttachedActors(attachedActors);
    for(AActor* actor : attachedActors)
    {
        if(actor && Cast<ACPerson>(actor))
        {
            (Cast<ACPerson>(actor))->setVisibility(newVisibility);
        }
    }
}

void ACPerson::hideClothing(FName item, bool hidden)
{
    if(bool *p = hiddenClothing.Find(item))
    {
        if((*p) == hidden) // no change
            return;
    }
    hiddenClothing.Add(item, hidden);
    updateMeshes();
}

void ACPerson::updateClothing() {
    updateMeshes();
    updateHeight();
}

void ACPerson::setBodyType(EPersonBodyType newType) {
    bodyType = newType;
    updateMeshes();
    updateHeight();
}