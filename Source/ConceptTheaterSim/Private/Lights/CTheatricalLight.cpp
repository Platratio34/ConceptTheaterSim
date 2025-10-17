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

    powerInput = CreateDefaultSubobject<UCPowerCableConnector>(TEXT("Power Input"));
    powerInput->input = true;
}

void ACTheatricalLight::OnConstruction(const FTransform &Transform)
{
    powerInput->input = true;
    for (int i = shutterPositions.Num(); i < 4; i++)
    {
        shutterPositions.Add(FShutterPosition::Create(0, 0));
    }

    if (lenseMaterial != nullptr)
    {
        lenseMaterialInstance = UMaterialInstanceDynamic::Create(lenseMaterial, this);
    }
    else
    {
        lenseMaterialInstance = nullptr;
    }
    if (lenseMesh != nullptr)
        lenseMesh->SetMaterial(lenseMaterialIndex, lenseMaterialInstance);
    if(lightFunction != nullptr)
    {
        lightFunctionInstance = UMaterialInstanceDynamic::Create(lightFunction, this);
    }
    else
    {
        lightFunctionInstance = nullptr;
    }
    if(light != nullptr)
        light->SetLightFunctionMaterial(lightFunctionInstance);

    if(focusMode)
    {
        if(light != nullptr)
        {
            light->SetIntensity(maxIntensity);
            if(lenseMaterialInstance != nullptr)
                lenseMaterialInstance->SetScalarParameterValue(FName("Intensity"), 10);
            light->SetLightColor(FLinearColor(1.0, 1.0, 1.0, 1.0));
            if(lenseMaterialInstance != nullptr)
                lenseMaterialInstance->SetVectorParameterValue(FName("Color"), FLinearColor(1.0, 1.0, 1.0, 1.0));
        }
        actualIntensity = 1;
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

    setGobo(gobo);
    setGoboRotation(goboRotation);
    
    if(lightFunctionInstance != nullptr)
        lightFunctionInstance->SetVectorParameterValue(FName("LightPosition"), GetActorLocation());
    onLightUpdate();
}

// Called when the game starts or when spawned
void ACTheatricalLight::BeginPlay()
{
	Super::BeginPlay();
    
    if(parentActor != nullptr)
    {
        root->AttachToComponent(parentActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
    }
    if(dummy && light != nullptr)
        light->SetIntensity(0);
    if(cullVolume.IsValid())
    {
        ACCullVolume *v = cullVolume.Get();
        if(v)
            v->onActiveChange.AddDynamic(this, &ACTheatricalLight::updateCull);
    }
}

// Called every frame
void ACTheatricalLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(lightFunctionInstance != nullptr)
        lightFunctionInstance->SetVectorParameterValue(FName("LightPosition"), GetActorLocation());
}

void ACTheatricalLight::setup(UStaticMeshComponent* lenseMesh_)
{
    if(lenseMesh_ == nullptr)
        return;
    lenseMesh = lenseMesh_;

    if (lenseMaterial != nullptr && (lenseMaterialInstance == nullptr || !IsValid(lenseMaterialInstance)))
    {
        lenseMaterialInstance = UMaterialInstanceDynamic::Create(lenseMaterial, this);
            lenseMesh->SetMaterial(lenseMaterialIndex, lenseMaterialInstance);
    }
    lenseMesh->SetMaterial(lenseMaterialIndex, lenseMaterialInstance);
    if(lightFunction != nullptr && (lightFunctionInstance == nullptr || !IsValid(lightFunctionInstance)))
    {
        lightFunctionInstance = UMaterialInstanceDynamic::Create(lightFunction, this);
    }
    light->SetLightFunctionMaterial(lightFunctionInstance);
}

void ACTheatricalLight::setIntensity(double newIntensity)
{
    if(newIntensity < 0)
        newIntensity = 0;
    if(newIntensity > 1)
        newIntensity = 1;
    intensity = newIntensity;
    actualIntensity = intensity * getIntensityScale() * getPower();
    if(actualIntensity > 1)
    {
        actualIntensity = 1;
    }
    else if(actualIntensity < 0)
    {
        actualIntensity = 0;
    }
    if(intensityCurve != nullptr)
        actualIntensity = intensityCurve->GetFloatValue(actualIntensity);
    if(light != nullptr && !dummy)
        light->SetIntensity(actualIntensity * maxIntensity);
    if(lenseMaterialInstance != nullptr)
        lenseMaterialInstance->SetScalarParameterValue(FName("Intensity"), actualIntensity * 10);
    onLightUpdate();
}

void ACTheatricalLight::setColor(FLinearColor newColor)
{
    color = newColor;
    onLightUpdate();
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
    body->SetRelativeRotation(FRotator(0, 0, tilt));
}

void ACTheatricalLight::setEdge(double newEdge)
{
    if(newEdge < 0)
        newEdge = 0;
    // if(newEdge > 1)
    //     newEdge = 1;
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
    if(simpleEdge)
    {
        if(light != nullptr)
            light->SetInnerConeAngle(zoom * 0.5 * edge);
    }
    else
    {
        if(light != nullptr)
            light->SetInnerConeAngle(zoom * 0.5);
        if(lightFunctionInstance != nullptr)
            lightFunctionInstance->SetScalarParameterValue(FName("Focus"), edge);
    }
    onLightUpdate();
}


void ACTheatricalLight::setShutter(int index, FShutterPosition position)
{
    shutterPositions[index] = position;

    FName posNames[4] = {FName("Shutter A In"), FName("Shutter B In"), FName("Shutter C In"), FName("Shutter D In")};
    FName angleOffNames[4] = {FName("Shutter A AngleOff"), FName("Shutter B AngleOff"), FName("Shutter C AngleOff"), FName("Shutter D AngleOff")};
    
    lightFunctionInstance->SetScalarParameterValue(posNames[index], position.position);
    lightFunctionInstance->SetScalarParameterValue(angleOffNames[index], position.angleOffset);
    if(USceneComponent** p = shutterHandles.Find(index))
    {
        USceneComponent *handle = *p;
        handle->SetRelativeLocation(FVector(0, position.position*shutterHandleTravel*2.54, 0));
        handle->SetRelativeRotation(FRotator(0, atan(position.angleOffset) / DEG_TO_RAD, 0));
    }
}

void ACTheatricalLight::updateShutters()
{
    if(lightFunctionInstance == nullptr)
        return;

    FName posNames[4] = {FName("Shutter A In"), FName("Shutter B In"), FName("Shutter C In"), FName("Shutter D In")};
    FName angleOffNames[4] = {FName("Shutter A AngleOff"), FName("Shutter B AngleOff"), FName("Shutter C AngleOff"), FName("Shutter D AngleOff")};
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

void ACTheatricalLight::setShutterFrame(double position)
{
    shutterFrame = position;
    if(lightFunctionInstance == nullptr)
        return;
    lightFunctionInstance->SetScalarParameterValue(FName("Frame Rotate"), position);
}

void ACTheatricalLight::addShutterHandle(int index, UStaticMeshComponent* handle)
{
    if(handle == nullptr)
        shutterHandles.Remove(index);
    shutterHandles.Add(index, handle);
}

void ACTheatricalLight::setGobo(UTexture2D* newGobo)
{
    gobo = newGobo;
    if(lightFunctionInstance == nullptr)
        return;

    if(gobo == nullptr)
    {
        lightFunctionInstance->SetScalarParameterValue(FName("Gobo Active"), 0);
    }
    else
    {
        lightFunctionInstance->SetScalarParameterValue(FName("Gobo Active"), 1);
        lightFunctionInstance->SetTextureParameterValue(FName("Gobo"), gobo);
    }
}

void ACTheatricalLight::setGoboRotation(double newGoboRotation)
{
    goboRotation = newGoboRotation;
    if(lightFunctionInstance == nullptr)
        return;

    lightFunctionInstance->SetScalarParameterValue(FName("Gobo Rotation"), goboRotation);
}

double ACTheatricalLight::getPower()
{
    return powerInput->getWatts() / requiredPower;
}

void ACTheatricalLight::updateCull(FName volume, bool active)
{
    if(light != nullptr)
        light->SetVisibility(active);
}