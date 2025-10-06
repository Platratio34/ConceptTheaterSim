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
    spline->SetupAttachment(root);
    
    arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
}

UCCableConnector* ACCable::getConnector(AActor* actor, bool start)
{
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
        return;
        
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
        return;
    }

    FVector sPos = startPort->connector->GetComponentLocation();
    FVector ePos = endPort->connector->GetComponentLocation();
    
    SetActorLocation((sPos + ePos) * 0.5);

    if(drawDebug)
    {
        FTransform transform = GetActorTransform();
        // sPos = transform.InverseTransformPosition(sPos);
        // ePos = transform.InverseTransformPosition(ePos);

        // arrow->RegisterComponent(); 
        // arrow->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);
        arrow->SetWorldLocation(sPos);
        arrow->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(sPos, ePos));
        arrow->ArrowSize = 0.5;
        arrow->ArrowLength = (sPos - ePos).Size() * 2.0;
        arrow->SetArrowColor(debugColor);
    }
    arrow->SetVisibility(drawDebug);
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
                UE_LOG(LogTemp, Display, TEXT("Cable connected"));
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

    updateCable();
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

void ACCable::updateCable()
{
    if(startConnector != nullptr || endConnector != nullptr)
    {
        coil->SetVisibility(false);
        coil->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        spline->SetVisibility(true);

        FVector splineStart;
        FVector splineEnd;

        if(isFullyConnected())
        {
            FTransform startTransform = startConnector->connector->GetComponentTransform();
            startMesh->SetVisibility(true);
            startMesh->SetWorldLocationAndRotation(startTransform.GetLocation(), startTransform.GetRotation().Rotator());
            splineStart = startMesh->GetComponentTransform().TransformPosition(startOffset);
            
            FTransform endTransform = endConnector->connector->GetComponentTransform();
            endMesh->SetVisibility(true);
            endMesh->SetWorldLocationAndRotation(endTransform.GetLocation(), endTransform.GetRotation().Rotator());
            splineEnd = endMesh->GetComponentTransform().TransformPosition(endOffset);
        }
        else
        {
            if(startConnector != nullptr)
            {
                FTransform startTransform = startConnector->connector->GetComponentTransform();
                startMesh->SetVisibility(true);
                startMesh->SetWorldLocationAndRotation(startTransform.GetLocation(), startTransform.GetRotation().Rotator());
                endMesh->SetVisibility(false);
                splineStart = startMesh->GetComponentTransform().TransformPosition(startOffset);
            }
            else
            {
                splineStart = player->GetActorLocation();
            }
            if(endConnector != nullptr)
            {
                FTransform endTransform = endConnector->connector->GetComponentTransform();
                endMesh->SetVisibility(true);
                endMesh->SetWorldLocationAndRotation(endTransform.GetLocation(), endTransform.GetRotation().Rotator());
                startMesh->SetVisibility(false);
                splineEnd = endMesh->GetComponentTransform().TransformPosition(endOffset);
            }
            else
            {
                splineEnd = player->GetActorLocation();
            }
        }

        FTransform splineTransform = spline->GetComponentTransform();

        splineStart = splineTransform.InverseTransformPosition(splineStart);
        splineEnd = splineTransform.InverseTransformPosition(splineEnd);
        FVector startTangent = splineEnd - splineStart;
        FVector endTangent = splineStart - splineEnd;

        spline->SetStartAndEnd(splineStart, startTangent, splineEnd, endTangent);
    }
    else // totally disconnected
    {
        spline->SetVisibility(false);
        coil->SetVisibility(!inInventory);
        coil->SetCollisionEnabled((!inInventory) ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
        startMesh->SetVisibility(false);
        endMesh->SetVisibility(false);
    }
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