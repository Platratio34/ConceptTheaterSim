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

    if(dummy)
    {
        return;
    }
    if(track != nullptr) {
        int frame = master->lastFrame;
        AActor *ow = GetOwner();
        FVector cPos;
        cPos.X = xPosTrack->getValueTracked(frame) * IN_TO_CM;
        cPos.Y = yPosTrack->getValueTracked(frame) * IN_TO_CM;
        cPos.Z = zPosTrack->getValueTracked(frame) * IN_TO_CM;
        if(!(xPosTrack->blocked && yPosTrack->blocked && zPosTrack->blocked)) {
            ow->SetActorRelativeLocation(cPos);
        }

        FRotator cRot;
        cRot.Roll = xRotTrack->getValueTracked(frame);
        cRot.Pitch = yRotTrack->getValueTracked(frame);
        cRot.Yaw = zRotTrack->getValueTracked(frame);
        if(!(xRotTrack->blocked && yRotTrack->blocked && zRotTrack->blocked)) {
            ow->SetActorRelativeRotation(cRot);
        }
        return;
    }

    AActor *ow = GetOwner();
    // FEulerTransform transform = ow->GetRootComponent()->GetRelativeTransform();
    FVector cPos/* = transform.Location*/;
    bool posChange = !(xPosDriver->IsDone() && yPosDriver->IsDone() && zPosDriver->IsDone());
    cPos.X = xPosDriver->Update(DeltaTime) * IN_TO_CM;
    cPos.Y = yPosDriver->Update(DeltaTime) * IN_TO_CM;
    cPos.Z = zPosDriver->Update(DeltaTime) * IN_TO_CM;
    if(posChange)
        ow->SetActorRelativeLocation(cPos);
    FRotator cRot /* = transform.Rotation */;
    bool rotChange = !(xRotDriver->IsDone() && yRotDriver->IsDone() && zRotDriver->IsDone());
    cRot.Roll = xRotDriver->Update(DeltaTime);
    cRot.Pitch = yRotDriver->Update(DeltaTime);
    cRot.Yaw = zRotDriver->Update(DeltaTime);
    if(rotChange)
        ow->SetActorRelativeRotation(cRot);
}

void UCAnimationComponent::setTrack(UAnimationTrack *newTrack) {
    track = newTrack;
    if(track == nullptr)
        return;
    
    xPosTrack = track->properties[FName("x")];
    yPosTrack = track->properties[FName("y")];
    zPosTrack = track->properties[FName("z")];
    
    xRotTrack = track->properties[FName("xRot")];
    yRotTrack = track->properties[FName("yRot")];
    zRotTrack = track->properties[FName("zRot")];
}

void UCAnimationComponent::onEvent(FAnimationFileTrackEvent event)
{   
    if(track != nullptr)
        return;
    
    if(event.visibleKey)
    {
        if(!event.visible)
            updateVisibility(false);
    }
    if(event.parentKey)
    {
        if(event.clearParent)
        {
            if(cParent != nullptr)
            {
                GetOwner()->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
                updatePosRot(nullptr);
            }
        }
        else
        {
            USceneComponent *newParent = master->getParent(event.parent);
            if(newParent != cParent)
            {
                FAttachmentTransformRules attachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
                GetOwner()->GetRootComponent()->AttachToComponent(newParent, attachmentRules);
                updatePosRot(newParent);
            }
        }
    }

    bool isInstant = event.duration < (1 / 30);

    AActor *ow = GetOwner();
    FTransform transform = ow->GetRootComponent()->GetRelativeTransform();
    FVector cPos = transform.GetLocation();
    if(event.xKey)
    {
        if(isInstant)
            cPos.X = event.x * IN_TO_CM;
        xPosDriver->SetTarget(event.x, event.duration);
    }
    if(event.yKey)
    {
        if(isInstant)
            cPos.Y = event.y * IN_TO_CM;
        yPosDriver->SetTarget(event.y, event.duration);
    }
    if(event.zKey)
    {
        if(isInstant)
            cPos.Z = event.z * IN_TO_CM;
        zPosDriver->SetTarget(event.z, event.duration);
    }
    if(isInstant)
        ow->SetActorRelativeLocation(cPos);
    
    FRotator cRot = transform.GetRotation().Rotator();
    if(event.xRotKey)
    {
        if(isInstant)
            cRot.Roll = event.xRot;
        xRotDriver->SetTarget(event.xRot, event.duration);
    }
    if(event.yRotKey)
    {
        if(isInstant)
            cRot.Pitch = event.yRot;
        yRotDriver->SetTarget(event.yRot, event.duration);
    }
    if(event.zRotKey)
    {
        if(isInstant)
            cRot.Yaw = event.zRot;
        zRotDriver->SetTarget(event.zRot, event.duration);
    }
    if(isInstant)
        ow->SetActorRelativeRotation(cRot);
    
    if(event.visibleKey)
    {
        if(event.visible)
            updateVisibility(true);
    }

    for(const FName& key : event.showClothing) {
        onClothingVisibilityChange.Broadcast(key, false);
    }
    for(const FName& key : event.hideClothing) {
        onClothingVisibilityChange.Broadcast(key, true);
    }
}

void UCAnimationComponent::updateVisibility(bool newVisibility)
{
    onVisibilityChange.Broadcast(newVisibility);
    for(UStaticMeshComponent* mesh : meshes)
    {
        mesh->SetVisibility(newVisibility);
        mesh->SetCollisionEnabled(newVisibility ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    }
}

void UCAnimationComponent::updatePosRot(USceneComponent* newParent)
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

    FVector cTgtPos = FVector(xPosDriver->GetValue(), yPosDriver->GetValue(), zPosDriver->GetValue());
    FQuat cTgtRot = FRotator(yRotDriver->GetValue(), zRotDriver->GetValue(), xRotDriver->GetValue()).Quaternion();
    if(cParent != nullptr)
    {
        FTransform oldParentTransform = cParent->GetComponentTransform();
        cTgtPos = oldParentTransform.TransformPosition(cTgtPos);
        cTgtRot = oldParentTransform.TransformRotation(cTgtRot);
    }
    if(newParent != nullptr)
    {
        FTransform newParentTransform = newParent->GetComponentTransform();
        cTgtPos = newParentTransform.InverseTransformPosition(cTgtPos);
        cTgtRot = newParentTransform.InverseTransformRotation(cTgtRot);
    }
    FRotator cTgtRotator = cTgtRot.Rotator();
    xPosDriver->UpdateTarget(cTgtPos.X / IN_TO_CM);
    yPosDriver->UpdateTarget(cTgtPos.Y / IN_TO_CM);
    zPosDriver->UpdateTarget(cTgtPos.Z / IN_TO_CM);
    xRotDriver->UpdateTarget(cTgtRotator.Roll);
    yRotDriver->UpdateTarget(cTgtRotator.Pitch);
    zRotDriver->UpdateTarget(cTgtRotator.Yaw);

    cParent = newParent;
}