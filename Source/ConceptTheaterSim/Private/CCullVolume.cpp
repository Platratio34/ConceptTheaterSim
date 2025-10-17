// Copyright Peter Crall 2025.

#include "CCullVolume.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CVisibilityBlocker.h"

ACCullVolume::ACCullVolume()
{
    rootObj = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(rootObj);

    collider = CreateDefaultSubobject<UBoxComponent>("Box");
    collider->SetupAttachment(rootObj);

    meshCollider = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    meshCollider->SetupAttachment(rootObj);
    meshCollider->SetHiddenInGame(true);
}
ACCullVolume::~ACCullVolume()
{

}

void ACCullVolume::OnConstruction(const FTransform &Transform)
{
    if(volumeMesh)
    {
        meshCollider->SetVisibility(meshVisibleInEditor);
        meshCollider->SetStaticMesh(volumeMesh);
        meshCollider->SetRelativeLocation(meshOffset*2.54);
    }
    else
    {
        meshCollider->SetVisibility(false);
        collider->SetBoxExtent(boxSize * 2.54 * 0.5);
    }
    updateArrows(true);
}

void ACCullVolume::BeginPlay()
{
    Super::BeginPlay();
    if (volumeMesh) // if we are mesh-based, register them instead;
    {
        meshCollider->OnComponentBeginOverlap.AddDynamic(this, &ACCullVolume::onBeginBoxOverlap);
        meshCollider->OnComponentEndOverlap.AddDynamic(this, &ACCullVolume::onBoxOverlapEnd);
    }
    else
    {
        collider->OnComponentBeginOverlap.AddDynamic(this, &ACCullVolume::onBeginBoxOverlap);
        collider->OnComponentEndOverlap.AddDynamic(this, &ACCullVolume::onBoxOverlapEnd);
    }
}

void ACCullVolume::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(updateHasPawn()) // if we've updated this:
    {
        if(!hasPawn) // de-activate all outbound connections if we no longer have the pawn
        {
            for(FCCullVolumeConnection connection : connections)
            {
                if(!connection.other || connection.disabled)
                    continue;
                connection.other->setConnectionIn(name, false);
            }
            activeConnectionsOut.Empty();
        }
    }
    if(hasPawn) // else update all outbound connections
    {
        for(FCCullVolumeConnection connection : connections)
        {
            if(connection.disabled)
                continue;
            updateConnection(connection);
        }
    }
    updateActive(); // finally update our active status
}

void ACCullVolume::onBeginBoxOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if(!Cast<APawn>(OtherActor))
        return;
    pawnInBox = true;
}

void ACCullVolume::onBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(!Cast<APawn>(OtherActor))
        return;
    if(!pawnInBox)
        return;
    TArray<AActor*> overlappingActors;
    collider->GetOverlappingActors(overlappingActors);

    for(AActor* actor : overlappingActors)
    {
        if(Cast<APawn>(actor))
            return;
    }
    pawnInBox = false;
}

bool ACCullVolume::updateHasPawn()
{
    bool n = pawnInBox; // Pawn is in the box collider OR pawn is overlapping with the edge of the volume mesh
    if(!n) // Only bother checking sub-volumes if the pawn is not overlapping the box or mesh right now
    {
        for(ACCullVolume* sub : subVolumes)
        {
            if(sub && sub->hasPawn)
            {
                n = true;
                break;
            }
        }
    }
    if(volumeMesh && !n) // We have a volume mesh, and we don't yet know the pawn *is* in the volume, so we have to do line traces
    {
        // might make sense of cache the pawn? But what if it changes?
        // TODO: Also probably should do something to get local pawn/camera rather than P0, but that is a future Peter problem
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        FVector loc = PlayerPawn->GetActorLocation();
        FVector top = loc + checkOffset;
        FCollisionQueryParams queryParams;
        FHitResult hit;
        if(meshCollider->LineTraceComponent(hit, loc, top, queryParams)) // only do second trace if we *could* be in the volume
        {
            FVector bottom = loc - checkOffset;
            FHitResult hit2;
            n = meshCollider->LineTraceComponent(hit2, loc, bottom, queryParams);
        }
    }
    if(n && requireSky) // only check sky if we *could* be in the volume
    {
        APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        FVector loc = playerPawn->GetActorLocation();
        FVector top = loc + checkOffset;
        FCollisionQueryParams queryParams;
        queryParams.AddIgnoredActor(playerPawn);
        FHitResult hit;
        bool sky = GetWorld()->LineTraceSingleByChannel(hit, loc, top, ECollisionChannel::ECC_Visibility, queryParams);
        n &= sky;
    }
    if(n == hasPawn) // No change in overall state
        return false;
    hasPawn = n;
    return true;
}

bool ACCullVolume::updateActive()
{
    bool a = hasPawn;
    if(!a) // If we have the pawn, no need to check incoming connections
    {
        if(activeConnectionsInDirty) // If the incoming connections have changed since last time we checked
        {
            connectionInActive = !activeConnectionsIn.IsEmpty();
            activeConnectionsInDirty = false;
        }
        a |= connectionInActive;
    }
    if(a == active) // No change in overall state
        return false;
    onActiveChange.Broadcast(name, active);
    active = a;
    return true;
}

void ACCullVolume::setConnectionIn(FName other, bool connectionActive)
{
    activeConnectionsInDirty = true;
    if(active)
    {
        activeConnectionsIn.Add(other);
    }
    else
    {
        activeConnectionsIn.Remove(other);
    }
}

void ACCullVolume::updateConnection(FCCullVolumeConnection connection)
{
    if(connection.other == nullptr)
        return;
    bool open = false;
    for(FCVisiblityBlockerSet set : connection.blockers)
    {
        for(AActor* actor : set.blockers)
        {
            ICVisibilityBlocker *blocker = Cast<ICVisibilityBlocker>(actor);
            if(!blocker) // just to prevent issues if a non blocker is added to the connection
                continue;
            if(blocker->isVisibilityBlocking())
            {
                if(set.andBlockers) // Early exit if blocking for AND
                    break;
            }
            else if(!set.andBlockers) // Early exit if not blocking for OR
            {
                open |= true;
                break;
            }
        }
        if(!open)
            break;
    }
    if(open)
    {
        FName otherName = connection.other->name;
        if(activeConnectionsOut.Contains(otherName))
            return;
        connection.other->setConnectionIn(name, true);
        activeConnectionsOut.Add(otherName);
    }
    else
    {
        FName otherName = connection.other->name;
        if(!activeConnectionsOut.Contains(otherName))
            return;
        connection.other->setConnectionIn(name, false);
        activeConnectionsOut.Remove(otherName);
    }
}

void ACCullVolume::updateArrows(bool notify)
{
    TSet<FName> usedArrows;
    FVector sPos = GetActorLocation();
    for(FCCullVolumeConnection connection : connections)
    {
        ACCullVolume *other = connection.other;
        if(!other)
            continue;
        FName otherName = other->name;
        if(usedArrows.Contains(otherName))
        {
            UE_LOG(LogTemp, Warning, TEXT("Duplicate connection to %s from %s"), *(otherName.ToString()), *(name.ToString()));
            continue;
        }
        usedArrows.Add(otherName);
        UArrowComponent *arrow = nullptr;
        if(UArrowComponent** p = arrows.Find(otherName))
            arrow = *p;
        if(arrow == nullptr || !IsValid(arrow))
        {
            arrow = NewObject<UArrowComponent>(this);
            arrow->AttachToComponent(rootObj, FAttachmentTransformRules::SnapToTargetIncludingScale);
            arrow->ArrowSize = 1.0;
            arrows.Add(otherName, arrow);
        }
        FVector endPos = other->GetActorLocation();
        arrow->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(sPos, endPos));
        double len = (endPos - sPos).Size();
        if(connection.disabled)
        {
            arrow->SetArrowColor(FLinearColor(0.1, 0.1, 0.1, 1.0));
            arrow->ArrowLength = len * 0.5;
        }
        else 
        {
            bool oneDir = connection.oneDirection || outOnly;
            bool hasSelf = oneDir;
            if(!hasSelf)
            {
                for(FCCullVolumeConnection con2 : other->connections)
                {
                    if(con2.other != this)
                        continue;
                    hasSelf = true;
                    break;
                }
            }
            bool isConditional = connection.blockers.Num() > 0;
            arrow->ArrowLength = (hasSelf && !oneDir) ? (len * 0.5) : (len - 15);
            if(!hasSelf)
            {
                arrow->SetArrowColor(FLinearColor(1.0, 0.1, isConditional ? 0.3 : 0.1, 1.0));
            }
            else if(isConditional)
            {
                arrow->SetArrowColor(FLinearColor(0.1, 0.1, 1.0, 1.0));
            }
            else
            {
                arrow->SetArrowColor(FLinearColor(0.1, 1.0, 0.1, 1.0));
            }
        }
        if(notify)
        {
            other->updateArrows(false);
        }
    }
    for(ACCullVolume* sub : subVolumes)
    {
        if(!sub)
            continue;
        FName otherName = sub->name;
        usedArrows.Add(otherName);
        UArrowComponent *arrow = nullptr;
        if(UArrowComponent** p = arrows.Find(otherName))
            arrow = *p;
        if(arrow == nullptr || !IsValid(arrow))
        {
            arrow = NewObject<UArrowComponent>(this);
            arrow->AttachToComponent(rootObj, FAttachmentTransformRules::SnapToTargetIncludingScale);
            arrow->ArrowSize = 1.0;
            arrows.Add(otherName, arrow);
        }
        FVector endPos = sub->GetActorLocation();
        arrow->SetWorldLocation(endPos);
        arrow->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(endPos, sPos));
        arrow->SetArrowColor(FLinearColor(0.75, 0.75, 0.75, 1.0));
        arrow->ArrowLength = (endPos - sPos).Size() - 15;
    }
    TArray<FName> arrowsToRemove;
    for(TPair<FName, UArrowComponent*> pair : arrows)
    {
        if(usedArrows.Contains(pair.Key))
            continue;
        arrowsToRemove.Add(pair.Key);
        pair.Value->DestroyComponent();
    }
    for(FName nameToRemove : arrowsToRemove)
    {
        arrows.Remove(nameToRemove);
    }
}