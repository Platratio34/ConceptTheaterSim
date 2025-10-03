// Copyright Peter Crall 2025.

#include "Animation/CAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

#define IN_TO_CM 2.54

UCAnimationComponent::UCAnimationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCAnimationComponent::registerWithMaster(FName ID)
{
    TArray<AActor*> found;
    UGameplayStatics::GetAllActorsOfClass(GetOwner()->GetWorld(), ACAnimationMaster::StaticClass(), found);
    if(found.Num() <= 0)
    {
        return;
    }
    master = Cast<ACAnimationMaster>(found[0]);
    master->registerAnimatedObject(ID, this);
}

// Called when the game starts or when spawned
void UCAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

    AActor *ow = GetOwner();
    FVector pos = ow->GetActorLocation();
    xPosDriver = NewObject<UPropertyDriver>();
    xPosDriver->SetValue(pos.X);
    yPosDriver = NewObject<UPropertyDriver>();
    yPosDriver->SetValue(pos.Y);
    zPosDriver = NewObject<UPropertyDriver>();
    zPosDriver->SetValue(pos.Z);

    FRotator rot = ow->GetActorRotation();
    xRotDriver = NewObject<UPropertyDriver>();
    xRotDriver->SetValue(rot.Roll);
    yRotDriver = NewObject<UPropertyDriver>();
    yRotDriver->SetValue(rot.Pitch);
    zRotDriver = NewObject<UPropertyDriver>();
    zRotDriver->SetValue(rot.Yaw);
}

// Called every frame
void UCAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(parentInvalid)
    {

    }

    AActor *ow = GetOwner();
    // FEulerTransform transform = ow->GetRootComponent()->GetRelativeTransform();
    FVector cPos/* = transform.Location*/;
    cPos.X = xPosDriver->Update(DeltaTime) * IN_TO_CM;
    cPos.Y = yPosDriver->Update(DeltaTime) * IN_TO_CM;
    cPos.Z = zPosDriver->Update(DeltaTime) * IN_TO_CM;
    ow->SetActorRelativeLocation(cPos);
    FRotator cRot/* = transform.Rotation*/;
    cRot.Roll = xRotDriver->Update(DeltaTime);
    cRot.Pitch = yRotDriver->Update(DeltaTime);
    cRot.Yaw = zRotDriver->Update(DeltaTime);
    ow->SetActorRelativeRotation(cRot);
}

void UCAnimationComponent::onEvent(FAnimationFileTrackEvent event)
{   
    if(event.parentKey)
    {
        if(event.clearParent)
        {
            if(cParent != nullptr)
            {
                GetOwner()->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
                cParent = nullptr;
                updatePosRot();
            }
        }
        else
        {
            AActor *newParent = master->getParent(event.parent);
            if(newParent != cParent)
            {
                USceneComponent *targetComponent = newParent->GetRootComponent();
                // TODO: change for animation parent component here?
                FAttachmentTransformRules attachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
                GetOwner()->GetRootComponent()->AttachToComponent(targetComponent, attachmentRules);
                cParent = newParent;
                updatePosRot();
            }
        }
    }

    if(event.xKey)
        xPosDriver->SetTarget(event.x, event.duration);
    if(event.yKey)
        yPosDriver->SetTarget(event.y, event.duration);
    if(event.zKey)
        zPosDriver->SetTarget(event.z, event.duration);
        
    if(event.xRotKey)
        xRotDriver->SetTarget(event.xRot, event.duration);
    if(event.yRotKey)
        yRotDriver->SetTarget(event.yRot, event.duration);
    if(event.zRotKey)
        zRotDriver->SetTarget(event.zRot, event.duration);
    
    if(event.visibleKey)
        onVisibilityChange.Broadcast(event.visible);
}

void UCAnimationComponent::updatePosRot()
{
    AActor *ow = GetOwner();
    FTransform transform = ow->GetRootComponent()->GetRelativeTransform();
    
    FVector pos = transform.GetLocation();
    xPosDriver->SetValue(pos.X / IN_TO_CM);
    yPosDriver->SetValue(pos.Y / IN_TO_CM);
    zPosDriver->SetValue(pos.Z / IN_TO_CM);

    FRotator rot = transform.GetRotation().Rotator();
    xRotDriver->SetValue(rot.Roll);
    yRotDriver->SetValue(rot.Pitch);
    zRotDriver->SetValue(rot.Yaw);
}