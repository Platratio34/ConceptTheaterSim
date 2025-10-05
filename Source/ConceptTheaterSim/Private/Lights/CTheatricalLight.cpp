// Copyright Peter Crall 2025.

#include "Lights/CTheatricalLight.h"

// Sets default values
ACTheatricalLight::ACTheatricalLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(root);

    yoke = CreateDefaultSubobject<USceneComponent>(TEXT("Yoke"));
    yoke->SetupAttachment(root);

    body = CreateDefaultSubobject<USceneComponent>(TEXT("Body"));
    body->SetupAttachment(yoke);

    light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
    light->SetupAttachment(body);
}

void ACTheatricalLight::OnConstruction(const FTransform &Transform)
{
    for (int i = shutterPositions.Num(); i < 4; i++)
    {
        shutterPositions.Add(FShutterPosition::Create(0, 0));
    }

    if (lenseMaterial != nullptr && (lenseMaterialInstance == nullptr || !IsValid(lenseMaterialInstance)))
    {
        lenseMaterialInstance = UMaterialInstanceDynamic::Create(lenseMaterial, this);
        if (lenseMesh != nullptr)
            lenseMesh->SetMaterial(lenseMaterialIndex, lenseMaterialInstance);
    }
    if(lightFunction != nullptr && (lightFunctionInstance == nullptr || !IsValid(lightFunctionInstance)))
    {
        lightFunctionInstance = UMaterialInstanceDynamic::Create(lightFunction, this);
        if(light != nullptr)
            light->SetLightFunctionMaterial(lightFunctionInstance);
    }

    if(focusMode)
    {
        if(light != nullptr)
        {
            light->SetIntensity(maxIntensity);
            light->SetLightColor(FLinearColor(1.0, 1.0, 1.0, 1.0));
        }
    }
    else
    {
        setIntensity(intensity);
        setColor(color);
    }

    setPan(pan);
    setTilt(tilt);
    
    setEdge(edge);
    setZoom(zoom);
    updateShutters();
}

// Called when the game starts or when spawned
void ACTheatricalLight::BeginPlay()
{
	Super::BeginPlay();
    
}

// Called every frame
void ACTheatricalLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
}


void ACTheatricalLight::setIntensity(double newIntensity)
{
    if(newIntensity < 0)
        newIntensity = 0;
    if(newIntensity > 1)
        newIntensity = 1;
    intensity = newIntensity;
    double actualIntensity = intensity * getIntensityScale();
    if(intensityCurve != nullptr)
        actualIntensity = intensityCurve->GetFloatValue(actualIntensity);
    if(light != nullptr)
        light->SetIntensity(actualIntensity * maxIntensity);
    if(lenseMaterialInstance != nullptr)
        lenseMaterialInstance->SetScalarParameterValue(FName("Intensity"), actualIntensity * 10);
}

void ACTheatricalLight::setColor(FLinearColor newColor)
{
    color = newColor;
    if(light != nullptr)
        light->SetLightColor(color);
    if(lenseMaterialInstance != nullptr)
        lenseMaterialInstance->SetVectorParameterValue(FName("Color"), color);
}

void ACTheatricalLight::setPan(double newPan)
{
    pan = newPan;
    yoke->SetRelativeRotation(FRotator(0, pan, 0));
}

void ACTheatricalLight::setTilt(double newTilt)
{
    tilt = newTilt;
    body->SetRelativeRotation(FRotator(tilt, 0, 0));
}

void ACTheatricalLight::setEdge(double newEdge)
{
    if(newEdge < 0)
        newEdge = 0;
    if(newEdge > 1)
        newEdge = 1;
    edge = newEdge;
    updateBeam();
}

void ACTheatricalLight::setZoom(double newZoom)
{
    if(newZoom < 0)
        newZoom = 0;
    if(newZoom > 180)
        newZoom = 180;
    zoom = newZoom;
    if(light != nullptr)
        light->SetOuterConeAngle(zoom * 0.5);
    updateBeam();
}

void ACTheatricalLight::updateBeam()
{
    if(light == nullptr)
        return;
    
    double inner = (edge * 0.6) + 0.4;
    inner *= zoom * 0.5;
    light->SetInnerConeAngle(inner);
}

void ACTheatricalLight::updateShutters()
{
    if(lightFunctionInstance == nullptr)
        return;

    FName posNames[4] = {FName("ShutterAPos"), FName("ShutterBPos"), FName("ShutterCPos"), FName("ShutterDPos")};
    FName angleOffNames[4] = {FName("ShutterAAngleOff"), FName("ShutterBAngleOff"), FName("ShutterCAngleOff"), FName("ShutterDAngleOff")};
    for (int i = 0; i < 4; i++)
    {
        lightFunctionInstance->SetScalarParameterValue(posNames[i], shutterPositions[i].position);
        lightFunctionInstance->SetScalarParameterValue(angleOffNames[i], shutterPositions[i].angleOffset);
        if(USceneComponent** p = shutterHandles.Find(i))
        {
            USceneComponent *handle = *p;
            handle->SetRelativeLocation(FVector(0, shutterPositions[i].position*shutterHandleTravel*2.54, 0));
            handle->SetRelativeRotation(FRotator(0, atan(shutterPositions[i].angleOffset) / DEG_TO_RAD, 0));
        }
    }
}