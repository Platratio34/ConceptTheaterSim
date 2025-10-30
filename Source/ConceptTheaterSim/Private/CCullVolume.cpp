// Copyright Peter Crall 2025.

#include "CCullVolume.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CVisibilityBlocker.h"
#include "Engine/Engine.h"

// #define DEBUG_PRINT

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
    if(firstTick)
    {
        TArray<AActor*> overlappingActors;
        collider->GetOverlappingActors(overlappingActors);
        for(AActor* actor : overlappingActors)
        {
            if(Cast<APawn>(actor))
            {
                pawnInBox = true;
                break;
            }
        }
    }
    if(updateHasPawn()) // if we've updated this:
    {
        if(!hasPawn) // de-activate all outbound connections if we no longer have the pawn
        {
            #ifdef DEBUG_PRINT
            if(GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Exiting %s"), *(name.ToString())));
            #endif
            for(FCCullVolumeConnection connection : connections)
            {
                if(!connection.other || connection.disabled)
                    continue;
                connection.other->setConnectionIn(name, false);
            }
            activeConnectionsOut.Empty();
        }
        else
        {
            #ifdef DEBUG_PRINT
            if(GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Entering %s"), *(name.ToString())));
            #endif
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
    if(!updateActive() && firstTick) // finally update our active status
    {
        // just this once, make sure we broadcast the active state even if it didn't change
        onActiveChange.Broadcast(name, active);
    }
    firstTick = false;
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
    bool n = pawnInBox || forcePawn; // Pawn is in the box collider OR pawn is overlapping with the edge of the volume mesh
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
    if((volumeMesh != nullptr) && !n) // We have a volume mesh, and we don't yet know the pawn *is* in the volume, so we have to do line traces
    {
        // might make sense to cache the pawn? But what if it changes?
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
        bool blocked = GetWorld()->LineTraceSingleByChannel(hit, loc, top, ECollisionChannel::ECC_Visibility, queryParams);
        n &= !blocked;
    }
    if(n == hasPawn) // No change in overall state
        return false;
    hasPawn = n;
    return true;
}

bool ACCullVolume::updateActive()
{
    bool a = hasPawn || forceActive;
    if(!a) // If we have the pawn, no need to check incoming connections
    {
        if(activeConnectionsInDirty) // If the incoming connections have changed since last time we checked
        {
            bool wasActive = connectionInActive;
            connectionInActive = !activeConnectionsIn.IsEmpty();
            if(!connectionInActive && wasActive) // going from active to non-active
            {
                if(!skippedTick) // we skip a tick to prevent flashing
                {
                    skippedTick = true;
                    connectionInActive = false;
                }
                else // we skipped the tick, so clear everything
                {
                    activeConnectionsInDirty = false;
                    skippedTick = false;
                }
            }
            else // Otherwise, we can clear the dirty flag now
            {
                activeConnectionsInDirty = false;
            }
        }
        a |= connectionInActive;
    }
    if(a == active) // No change in overall state
        return false;
    active = a;
    onActiveChange.Broadcast(name, active);
    #ifdef DEBUG_PRINT
    if(GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s active: %s"), *(name.ToString()), active ? TEXT("True") : TEXT("False")));
    #endif
    return true;
}

void ACCullVolume::setConnectionIn(FName other, bool connectionActive)
{
    if(connectionActive)
    {
        activeConnectionsIn.Add(other);
        if (!active) // if we weren't active, update it now
        {
            active = true;
            onActiveChange.Broadcast(name, active);
            connectionInActive = true;
            // We aren't marking dirty, 'cause we just updated all that
            // And just to ensure there are no problems, clear the dirty and skip flags
            activeConnectionsInDirty = false;
            skippedTick = false;
        }
    }
    else
    {
        activeConnectionsIn.Remove(other);
        // We won't broadcast it here, because it might have just changed where it was being activated from
        // But we will mark it as dirty to be re-evaluated on the next tick
        activeConnectionsInDirty = true;
    }
}

void ACCullVolume::updateConnection(FCCullVolumeConnection connection)
{
    if(connection.other == nullptr)
        return;
    bool open = connection.blockers.IsEmpty();
    FName otherName = connection.other->name;
    for(FCVisiblityBlockerSet set : connection.blockers)
    {
        for(AActor* actor : set.blockers)
        {
            if(!actor) // just skip nullptrs
            {
                UE_LOG(LogTemp, Warning, TEXT("Null blocker in connection from %s to %s"), *(name.ToString()), *(otherName.ToString()));
                continue;
            }
            // ICVisibilityBlocker *blocker = Cast<ICVisibilityBlocker>(actor);
            if(!actor->GetClass()->ImplementsInterface(UCVisibilityBlocker::StaticClass())) // just to prevent issues if a non blocker is added to the connection
            {
                UE_LOG(LogTemp, Warning, TEXT("Invalid blocker in connection from %s to %s: %s"), *(name.ToString()), *(otherName.ToString()), *(actor->GetName()));
                continue;
            }
            if(ICVisibilityBlocker::Execute_isVisibilityBlocking(actor)) // if is blocking (aka. is open)
            {
                if(set.andBlockers) // Early exit if blocking for AND
                    break;
            }
            else if(!set.andBlockers) // Early exit if not blocking for OR
            {
                open = true;
                break;
            }
        }
        if(open)
            break;
    }
    if(open)
    {
        if(activeConnectionsOut.Contains(otherName))
            return;
        connection.other->setConnectionIn(name, true);
        activeConnectionsOut.Add(otherName);
    }
    else
    {
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