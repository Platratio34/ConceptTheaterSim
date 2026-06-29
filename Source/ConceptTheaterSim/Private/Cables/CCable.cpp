// Copyright Peter Crall 2025.

#include "Cables/CCable.h"
#include "Cables/CCableConnector.h"
#include "Components/ArrowComponent.h" 
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACCable::ACCable()
{
	PrimaryActorTick.bCanEverTick = true;
    
    root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(root);

    startMesh = CreateDefaultSubobject<UStaticMeshComponent>("Start");
    startMesh->SetupAttachment(root);

    endMesh = CreateDefaultSubobject<UStaticMeshComponent>("End");
    endMesh->SetupAttachment(root);
    
    coil = CreateDefaultSubobject<UStaticMeshComponent>("Coil");
    coil->SetupAttachment(root);
    
    spline = CreateDefaultSubobject<USplineMeshComponent>("Spline");
    spline->Mobility = EComponentMobility::Movable;
    spline->SetupAttachment(root);
    
    arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
}

UCCableConnector* ACCable::getConnector(AActor* actor, bool start)
{
    if(actor == nullptr)
        return nullptr;
    TArray<UCCableConnector *> connectors;
    actor->GetComponents<UCCableConnector>(connectors);
    FName portId = start ? startPortID : endPortID;
    for(UCCableConnector * con : connectors)
    {
        if(!portId.IsNone() && con->id != portId)
            continue;
        if(!isValidConnection(con, start))
            continue;
        return con;
    }
    return nullptr;
}

void ACCable::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);

    if(!softStartActor.IsValid() || !softEndActor.IsValid())
        return;

    AActor *startActor = softStartActor.Get();
    AActor *endActor = softEndActor.Get();
    if(!startActor || !endActor)
    {
        coil->SetVisibility(true);
        return;
    }
    coil->SetVisibility(false);
        
    UCCableConnector *startPort = getConnector(startActor, true);
    UCCableConnector *endPort = getConnector(endActor, false);
    
    #if WITH_EDITOR
        FString actorLabel = GetActorLabel();
    #else
        FString actorLabel = GetName();
    #endif
    if(startPort == nullptr || endPort == nullptr)
    {
        #if WITH_EDITOR
            FString targetActorLabel = (startPort == nullptr) ? startActor->GetActorLabel() : endActor->GetActorLabel();
        #else
            FString targetActorLabel = (startPort == nullptr) ? startActor->GetName() : endActor->GetName();
        #endif
        UE_LOG(LogTemp, Warning, TEXT("Unable to connect cable %s, could not find connector on %s"), *actorLabel, *targetActorLabel);
        updateCable(nullptr, nullptr, true);
        return;
    }
        
    if(startPort->connector == nullptr || endPort->connector == nullptr)
    {
        #if WITH_EDITOR
            FString targetActorLabel = (startPort->connector == nullptr) ? startActor->GetActorLabel() : endActor->GetActorLabel();
        #else
            FString targetActorLabel = (startPort->connector == nullptr) ? startActor->GetName() : endActor->GetName();
        #endif
        UE_LOG(LogTemp, Warning, TEXT("Unable to connect cable %s, connector on %s was missing component"), *actorLabel, *targetActorLabel);
        updateCable(nullptr, nullptr, true);
        return;
    }

    FVector sPos = startPort->connector->GetComponentLocation();
    FVector ePos = endPort->connector->GetComponentLocation();
    
    SetActorLocation((sPos + ePos) * 0.5);

    updateCable(startPort, endPort, true);
}

// Called when the game starts or when spawned
void ACCable::BeginPlay()
{
	Super::BeginPlay();
    
}

// Called every frame
void ACCable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(!isFullyConnected() && softStartActor.IsValid() && softEndActor.IsValid())
    {
        AActor *startActor = softStartActor.Get();
        AActor *endActor = softEndActor.Get();
        if(startActor && endActor)
        {
            UCCableConnector *startPort = getConnector(startActor, true);
            UCCableConnector *endPort = getConnector(endActor, false);
            if(startPort != nullptr && endPort != nullptr)
            {
                tryConnect(startPort);
                tryConnect(endPort);
                // UE_LOG(LogTemp, Display, TEXT("Cable connected"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Cable end actors missing connectors"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Cable end actors not loaded"));
        }
    }
    debugConnected = isFullyConnected();

    redrawCable();
}

void ACCable::putInInventory(ACharacter* player_)
{
    inInventory = true;
}

bool ACCable::canConnectTo(UCCableConnector *connector, bool start)
{
    return false;
}

bool ACCable::tryConnect(UCCableConnector *connector)
{
    if(connector == nullptr)
        return false;
    if(directional)
    {
        bool start = !connector->input;
        if((start ? startConnector : endConnector) != nullptr)
            return false;
        if(!canConnectTo(connector, start))
            return false;
        onConnect(connector, start);
        return true;
    }
    else
    {
        bool start = false;
        if(startConnector == nullptr)
        {
            start = true;
        }
        else if(endConnector != nullptr)
        {
            return false;
        }
        if(!canConnectTo(connector, start))
            return false;
        onConnect(connector, start);
        return true;
    }
}

void ACCable::onConnect(UCCableConnector *connector, bool start)
{
    if(start)
    {
        startConnector = connector;
        if(endConnector != nullptr)
            connector->onConnect(this);
    }
    else
    {
        endConnector = connector;
        if(startConnector != nullptr)
            connector->onConnect(this);
    }
}

void ACCable::onDisconnect(bool start)
{
    if(start)
    {
        startConnector = nullptr;
    }
    else
    {
        endConnector = nullptr;
    }
    if(endConnector != nullptr)
        endConnector->onDisconnect(this);
    if(startConnector != nullptr)
        startConnector->onDisconnect(this);
}

bool ACCable::isFullyConnected()
{
    return startConnector != nullptr && endConnector != nullptr;
}

void ACCable::updateCable(UCCableConnector *startPort, UCCableConnector *endPort, bool withDebug) {
    bool hasBoth = startPort != nullptr && endPort != nullptr;
    bool hasOne = startPort != nullptr || endPort != nullptr;
    if(hasBoth || (hasOne && player != nullptr)) {
        coil->SetVisibility(false, true);
        coil->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        FVector splineStart;
        FVector splineEnd;

        bool notConnected = false;

        if(hasBoth) {
            FTransform startTransform = startPort->connector->GetComponentTransform();
            startMesh->SetVisibility(true, true);
            startMesh->SetWorldLocationAndRotation(startTransform.GetLocation(), startTransform.GetRotation().Rotator());
            splineStart = startMesh->GetComponentTransform().TransformPosition(startOffset);
            
            FTransform endTransform = endPort->connector->GetComponentTransform();
            endMesh->SetVisibility(true, true);
            endMesh->SetWorldLocationAndRotation(endTransform.GetLocation(), endTransform.GetRotation().Rotator());
            splineEnd = endMesh->GetComponentTransform().TransformPosition(endOffset);
        } else {
            if(startPort != nullptr) {
                FTransform startTransform = startPort->connector->GetComponentTransform();
                startMesh->SetVisibility(true, true);
                startMesh->SetWorldLocationAndRotation(startTransform.GetLocation(), startTransform.GetRotation().Rotator());
                endMesh->SetVisibility(false, true);
                splineStart = startMesh->GetComponentTransform().TransformPosition(startOffset);
            } else {
                splineStart = player->GetActorLocation();
            }
            if(endPort != nullptr) {
                FTransform endTransform = endPort->connector->GetComponentTransform();
                endMesh->SetVisibility(true, true);
                endMesh->SetWorldLocationAndRotation(endTransform.GetLocation(), endTransform.GetRotation().Rotator());
                startMesh->SetVisibility(false, true);
                splineEnd = endMesh->GetComponentTransform().TransformPosition(endOffset);
            } else {
                splineEnd = player->GetActorLocation();
            }
        }

        if(drawDebug && withDebug) {
            FTransform transform = GetActorTransform();
            
            arrow->SetWorldLocation(splineStart);
            arrow->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(splineStart, splineEnd));
            arrow->ArrowSize = 0.5;
            arrow->ArrowLength = (splineStart - splineEnd).Size() * 2.0;
            arrow->SetArrowColor(debugColor);
            arrow->SetVisibility(true, true);

            spline->SetVisibility(false, true);
            for (int i = 0; i < splines.Num(); i++) {
                if(splines[i] && IsValid(splines[i]))
                    splines[i]->SetVisibility(false, true);
            }
            startMesh->SetVisibility(false, true);
            endMesh->SetVisibility(false, true);
        } else {
            arrow->SetVisibility(false, true);
            spline->SetVisibility(true, true);
            FTransform splineTransform = spline->GetComponentTransform();

            splineStart = splineTransform.InverseTransformPosition(splineStart);
            splineEnd = splineTransform.InverseTransformPosition(splineEnd);
            FVector startTangent = splineEnd - splineStart;

            if(redirectPoints.IsEmpty()) {
                spline->SetStartAndEnd(splineStart, startTangent, splineEnd, startTangent);
                for (int i = 0; i < splines.Num(); i++) {
                    if(splines[i]) {
                        splines[i]->DestroyComponent();
                        splines[i] = nullptr;
                    }
                }
                return;
            }

            FVector lastStart = splineStart;
            FAttachmentTransformRules transformRules(
                EAttachmentRule::SnapToTarget, // Location
                EAttachmentRule::SnapToTarget, // Rotation
                EAttachmentRule::SnapToTarget,    // Scale
                true                           // bWeldSimulatedBodies
            );
            USplineMeshComponent *comp;

            for (int i = 0; i < redirectPoints.Num(); i++) {
                AActor *point = redirectPoints[i];
                if(point == nullptr)
                    continue;
                FVector end = splineTransform.InverseTransformPosition(point->GetActorLocation());
                FVector tangent = end - lastStart;
                if(i == 0) {
                    spline->SetStartAndEnd(lastStart, tangent, end, tangent);
                } else {
                    comp = comp = getSpline(i - 1, transformRules);
                    comp->SetStartAndEnd(lastStart, tangent, end, tangent);
                    comp->SetVisibility(true, true);
                }
                
                lastStart = end;
            }

            comp = getSpline(redirectPoints.Num() - 1, transformRules);

            FVector tangent = splineEnd - lastStart;
            comp->SetStartAndEnd(lastStart, tangent, splineEnd, tangent);
            comp->SetVisibility(true, true);

            for (int i = redirectPoints.Num(); i < splines.Num(); i++) {
                if(splines[i]) {
                    splines[i]->DestroyComponent();
                    splines[i] = nullptr;
                }
            }
        }
    } else { // totally disconnected
        spline->SetVisibility(false, true);
        for (int i = 0; i < splines.Num(); i++) {
            if(splines[i] && IsValid(splines[i]))
                splines[i]->SetVisibility(false, true);
        }
        coil->SetVisibility(!inInventory, true);
        coil->SetCollisionEnabled((!inInventory) ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
        startMesh->SetVisibility(false, true);
        endMesh->SetVisibility(false, true);
        arrow->SetVisibility(false, true);
    }
}

USplineMeshComponent* ACCable::getSpline(int i, FAttachmentTransformRules& transformRules) {
    USplineMeshComponent *comp;
    if(splines.Num() <= i) {
        comp = NewObject<USplineMeshComponent>(this);
        splines.Add(comp);
    } else {
        comp = splines[i];
        if(!comp) {
            comp = NewObject<USplineMeshComponent>(this);
            splines[i] = comp;
        }
    }
    comp->SetMobility(EComponentMobility::Movable);
    comp->SetStaticMesh(spline->GetStaticMesh());
    comp->ForwardAxis = spline->ForwardAxis;
    comp->SetStartScale(spline->GetStartScale(), true);
    comp->SetEndScale(spline->GetEndScale(), true);
    comp->AttachToComponent(root, transformRules, NAME_None);
    comp->SetMaterial(0, spline->GetMaterial(0));

    return comp;
}

void ACCable::redrawCable() {
    if(startConnector != nullptr || endConnector != nullptr) { // assuming in play not in editor
        updateCable(startConnector, endConnector, false);
        return;
    }

    updateCable(getConnector(softStartActor.Get(), true), getConnector(softEndActor.Get(), false), true);
}

bool ACCable::isValidConnection(UCCableConnector *connector, bool start)
{
    if(connector == nullptr)
        return false;
    if(directional && (!start) != connector->input)
        return false;

    if((start ? startConnector : endConnector) != nullptr)
        return false;
    if(!canConnectTo(connector, start))
        return false;
    return true;
}

void ACCable::swapEnds() {
    // if(startConnector || end)
    TSoftObjectPtr<AActor> tempSoftActor = softStartActor;
    FName tempPortID = startPortID;
    UCCableConnector* tempConnector = startConnector;

    softStartActor = softEndActor;
    startPortID = endPortID;
    startConnector = endConnector;

    softEndActor = tempSoftActor;
    endPortID = tempPortID;
    endConnector = tempConnector;

    redrawCable();
}

