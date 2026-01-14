// Copyright Peter Crall 2025

#include "EOS/ETCLightBoard.h"
#include "EOS/EOSLightOutputTypes.h"

const FName CMD_UNPARK = FName(TEXT("Unpark"));
const FName CMD_CHAN = FName(TEXT("Chan"));
const FName CMD_FOLLOW = FName(TEXT("Follow"));

// Sets default values
AETCLightBoard::AETCLightBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    networkCard = CreateDefaultSubobject<UDMXNetworkCard>(TEXT("Network Card"));
    interaction = CreateDefaultSubobject<UCInteractionComponent>(TEXT("CInteraction"));
}

void AETCLightBoard::OnConstruction(const FTransform &Transform)
{
    interaction->supportsObjectMode = true;
}

// Called when the game starts or when spawned
void AETCLightBoard::BeginPlay()
{
	Super::BeginPlay();
    networkCard->onNetworkPacket.AddDynamic(this, &AETCLightBoard::onNetworkPacket);
    interaction->onInputOM.AddDynamic(this, &AETCLightBoard::onInputOM);
    interaction->onInputUpOM.AddDynamic(this, &AETCLightBoard::onInputUpOM);
    for (int u = 1; u <= 10; u++)
    {
        networkCard->addUniverse(u);
    }

    if(showfile == nullptr && autoLoad)
    {
        if(showPatch != nullptr)
        {
            showfile = UEOSShowfile::create(showPatch->name);
            TArray<int> chs;
            showPatch->lights.GetKeys(chs);
            for (int i = 0; i < chs.Num(); i++)
            {
                int ch = chs[i];
                FEOSShowPatchLight light = showPatch->lights[ch];
                UEOSPatch *patch = EOSPatchTypes::create(light.type);
                patch->universe = light.universe;
                patch->address = light.address;
                showfile->patchLight(ch, patch);
            }
        }
        else
        {
            showfile = UEOSShowfile::create(TEXT("Testing"));
            for (int i = 1; i <= 100; i++)
            {
                showfile->patchLight(i, EOSPatchTypes::create(LIGHT_TYPE_ETC_S4_LUSTR_P_DIRECT));
            }
        }
    }

    baseButtonMaterial = UMaterialInstanceDynamic::Create(buttonMaterial, this);

    baseButtonMaterial->SetScalarParameterValue("Intensity", 0.5);
    baseButtonMaterial->SetVectorParameterValue("Color", FLinearColor(1.0, 1.0, 1.0, 1.0));

    TMap<const FName, FColor> buttonColors = UEOSButton::getButtonColors();

    TArray<UStaticMeshComponent*> staticMeshComponents;
    GetComponents<UStaticMeshComponent>(staticMeshComponents);
    for(auto& comp : staticMeshComponents)
    {
        FString compName = comp->GetName();
        if(compName.Contains(TEXT("Button-")))
        {
            FName buttonName = FName(compName.Mid(7, compName.Len() - 7));
            buttonsByName.Add(buttonName, comp);
            buttonsByMesh.Add(comp, buttonName);
            setButtonInteractionText(comp, buttonName);
        }
        else if(compName.Contains(TEXT("Indicator-")))
        {
            FName buttonName = FName(compName.Mid(10, compName.Len() - 10));
            indicatorsByName.Add(buttonName, comp);
            UEOSButton* button = NewObject<UEOSButton>();
            button->setup(buttonName, nullptr, FColor(255, 255, 255));
            button->setIndicatorMesh(comp);
            buttonByName.Add(buttonName, button);
            if(FColor* color = buttonColors.Find(buttonName))
            {
                button->makeUnique(UMaterialInstanceDynamic::Create(buttonMaterial, this), UMaterialInstanceDynamic::Create(indicatorMaterial, this));
                button->setActiveColor(*color);
                UE_LOG(LogTemp, Display, TEXT("EOS Button: `%s` (UNIQUE)"), *buttonName.ToString());
            }
            button->updateRender();

            UE_LOG(LogTemp, Display, TEXT("EOS Button Indicator: `%s`"), *buttonName.ToString());
        }
        else if (compName == "Wheel-Scroll")
        {
            scrollWheel = comp;
            setButtonInteractionText(comp, FName("Intensity"));
        }
        else if (compName.Contains(TEXT("Wheel-")))
        {
            FString str = compName.Mid(6, compName.Len() - 6);
            setButtonInteractionText(comp, FName("Wheel "+str));
            int id = FCString::Atoi(*str);
            wheels.Add(id, comp);
            wheelsByMesh.Add(comp, id);
        }
        else if (compName.Contains(TEXT("Dial-")))
        {
            FString str = compName.Mid(5, compName.Len() - 5);
            setButtonInteractionText(comp, FName("Dial "+str));
            int id = FCString::Atoi(*str);
            dials.Add(id, comp);
            dialsByMesh.Add(comp, id);
        }
    }

    TArray<UTextRenderComponent*> textRendererComponents;
    GetComponents<UTextRenderComponent>(textRendererComponents);
    for(auto& comp : textRendererComponents)
    {
        FString compName = comp->GetName();
        if(compName.Contains(TEXT("Text-")))
        {
            FName buttonName = FName(compName.Mid(5, compName.Len() - 5));
            textByName.Add(buttonName, comp);
            comp->SetTextMaterial(baseButtonMaterial);
            UEOSButton* button = nullptr;
            if(UEOSButton** ptr = buttonByName.Find(buttonName))
            {
                button = *ptr;
                button->setTextRenderer(comp);
            }
            else
            {
                button = NewObject<UEOSButton>();
                button->setup(buttonName, comp, FColor(255, 255, 255));
                buttonByName.Add(buttonName, button);
                if(FColor* color = buttonColors.Find(buttonName))
                {
                    button->makeUnique(UMaterialInstanceDynamic::Create(buttonMaterial, this), UMaterialInstanceDynamic::Create(indicatorMaterial, this));
                    button->setActiveColor(*color);
                    UE_LOG(LogTemp, Display, TEXT("EOS Button: `%s` (UNIQUE)"), *buttonName.ToString());
                }
                button->updateRender();
            }
            // UE_LOG(LogTemp, Display, TEXT("EOS Button: `%s`"), *buttonName.ToString());
        }
    }

    // setButtonColor(BUTTON_LIVE, FColor(255, 255, 255), FColor(255, 255, 0));
    // setButtonColor(BUTTON_BLIND, FColor(255, 255, 255), FColor(32, 32, 255));
    // setButtonColor(BUTTON_STAGE, FColor(255, 255, 255), FColor(32, 255, 32));
    
    // setButtonColor(BUTTON_HIGH, FColor(255, 255, 255), FColor(255, 255, 0));
    
    // setButtonColor(BUTTON_CLEAR, FColor(255, 255, 255), FColor(255, 32, 32));
}

// Called every frame
void AETCLightBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    setButtonActive(BUTTON_LIVE, mode == EEOSMode::LIVE);
    setButtonActive(BUTTON_BLIND, mode == EEOSMode::BLIND);
    setButtonActive(BUTTON_STAGE, mode == EEOSMode::STAGE);
    
    setButtonActive(BUTTON_HIGH, highlightMode);

    setButtonActive(BUTTON_PARK, !parkedChannels.IsEmpty());

    setButtonActive(BUTTON_CLEAR, !command.IsEmpty() && !clearCmd);

    setButtonActive(BUTTON_SHIFT, shift);

    if(showfile && followTime != -1)
    {
        followTime -= DeltaTime;
        if(followTime <= 0)
        {
            executeCue(showfile->currentCue + 1, -1);
        }
    }
    if(cueTime > 0)
    {
        cueTime -= DeltaTime;
        if(cueTime < 0)
            cueTime = 0;
    }

    if(showfile)
        showfile->updateFades(DeltaTime);

    if(output)
    {
        if(showfile != nullptr)
            for (int u = 1; u <= 10; u++)
            {
                networkCard->sendData(FName(TEXT("EOS")), 128, u, outputUniverse(u));
            }
    }
    else
    {
        for (int u = 1; u <= 10; u++)
        {
            if(!networkCard->hasChanged(u))
                continue;
            updateUniverse(u, networkCard->getData(u));
            networkCard->clearChanged(u);
        }
    }
}

void AETCLightBoard::updateUniverse(int universe, TArray<int> dmx)
{
    if(showfile == nullptr)
        return;
    for (auto &elem : showfile->patch)
    {
        UEOSPatchSet* patchSet = elem.Value;
        int ch = elem.Key;
        for (int i = 0; i < patchSet->devices.Num(); i++)
        {
            UEOSPatch *patch = patchSet->devices[i];
            if(patch->universe != universe)
            {
                // UE_LOG(LogTemp, Display, TEXT("Skipping channel %d; (u %d)"), ch, patch->universe);
                continue;
            }
            // UE_LOG(LogTemp, Display, TEXT("Updating channel %d"), ch);
            
            EOSLightOutputType* outputType = EOSLightOutputType::getType(patch->type);
            outputType->input(dmx, showfile->getCueChannel(ch), patch->address-1);
        }
    }
}

TArray<int> AETCLightBoard::outputUniverse(int universe)
{
    TArray<int> dmxData;
    dmxData.Init(0, 512);
    if(showfile == nullptr)
        return dmxData;
    for (auto &elem : showfile->patch)
    {
        UEOSPatchSet* patchSet = elem.Value;
        int ch = elem.Key;
        for (int i = 0; i < patchSet->devices.Num(); i++)
        {
            UEOSPatch *patch = patchSet->devices[i];
            if(patch->universe != universe)
            {
                continue;
            }
            // UE_LOG(LogTemp, Display, TEXT("Outputting Ch %d at %d/%d"), ch, patch->universe, patch->address);

            EOSLightOutputType* outputType = EOSLightOutputType::getType(patch->type);
            outputType->output(showfile->getChannel(ch), dmxData, patch->address-1);
        }
    }
    return dmxData;
}

void AETCLightBoard::setButtonColor(FName button, int r, int g, int b)
{
    if(UTextRenderComponent** tr = textByName.Find(button))
    {
        (*tr)->TextRenderColor = FColor(r, g, b);
    }
}

void AETCLightBoard::setButtonColor(FName button, FColor buttonColor, FColor activeColor)
{
    if(UEOSButton** tr = buttonByName.Find(button))
    {
        (*tr)->setButtonColor(buttonColor, activeColor);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No such button: `%s`"), *button.ToString());
    }
}

void AETCLightBoard::setButtonActive(FName button, bool active)
{
    if(UEOSButton** tr = buttonByName.Find(button))
    {
        (*tr)->setActive(active);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No such button: `%s`"), *button.ToString());
    }
}


void AETCLightBoard::onInteract(UPrimitiveComponent* component)
{
    UStaticMeshComponent *mesh = (UStaticMeshComponent *)component;
    if(!mesh)
        return;
    if(FName *buttonPntr = buttonsByMesh.Find(mesh))
    {
        onButton(*buttonPntr);

        // if(button == BUTTON_PARK)
        // {
        //     if(parkedChannels.IsEmpty())
        //     {
        //         parkedChannels.Add(0);
        //     }
        //     else 
        //     {
        //         parkedChannels.Remove(0);
        //     }

        // }
            
    }
}
void AETCLightBoard::onInteractEnd(UPrimitiveComponent* component, double time)
{
    UStaticMeshComponent *mesh = (UStaticMeshComponent *)component;
    if(!mesh)
        return;
    if(FName *buttonPntr = buttonsByMesh.Find(mesh))
    {

    }
}
void AETCLightBoard::onInteractScroll(UPrimitiveComponent* component, double direction)
{
    UStaticMeshComponent *mesh = (UStaticMeshComponent *)component;
    if(!mesh)
        return;
    if(mesh == scrollWheel)
    {
        if(!activeSelection.chan)
            return;
        
        for (int i = 0; i < activeSelection.values.Num(); i++)
        {
            int ch = activeSelection.values[i];
            UEOSChannelView *chView = showfile->getChannel(ch);
            if(chView == nullptr)
                continue;
            double val = chView->getProperty(PROPERTY_INTENSITY) + (direction * 0.01);
            if(val < 0)
                val = 0;
            else if (val > 1)
                val = 1;
            showfile->setManualProperty(ch, PROPERTY_INTENSITY, val);
        }
    }
    else if (int *p = wheelsByMesh.Find(mesh))
    {
        if(encoderSelection.Num() == 0)
            return;
        int wheelIndex = *p;
        FName property = FName("None");
        double delta = 0.05;
        double min = 0;
        double max = 1;
        int encoderIndex = wheelIndex - 1 + (encoderPage * 6);
        if(encoderProperties.Num() > encoderIndex)
        {
            property = encoderProperties[encoderIndex];
            if(property == PROPERTY_PAN || property == PROPERTY_TILT)
            {
                delta = 0.5;
                min = -90;
                max = 90;
            }
            else if (property == PROPERTY_ZOOM)
            {
                delta = 0.5;
                min = 10;
                max = 50;
            }
        }
        
        UE_LOG(LogTemp, Display, TEXT("Encoder scroll: `%s`"), *property.ToString());
        if(property == FName("None"))
        {
            return;
        }
        
        for (const int& ch : encoderSelection)
        {
            UEOSChannelView *chView = showfile->getChannel(ch);
            if(chView == nullptr)
                continue;
            if(!chView->hasProperty(property))
                continue;
            UEOSPropertyType *propertyType = showfile->patch[ch]->getProperty(property);
            double val = chView->getProperty(property);
            if(propertyType->indexed)
            {
                double step = propertyType->indexStep(val);
                if(step > 0)
                {
                    val += direction * step;
                    if(val < propertyType->min)
                        val = propertyType->max;
                    else if (val > propertyType->max)
                        val = propertyType->min;
                }
                else if(direction > 0)
                {
                    val = propertyType->nextIndex(val);
                }
                else if(direction < 0)
                {
                    val = propertyType->lastIndex(val);
                }
            }
            else
            {
                val += (direction * propertyType->encoderStep);
                if(val < propertyType->min)
                    val = propertyType->min;
                else if (val > propertyType->max)
                    val = propertyType->max;
            }
            UE_LOG(LogTemp, Display, TEXT("- new val: [%d] %f"), ch, val);
            showfile->setManualProperty(ch, property, val);
        }
    }
    else if (int *p2 = dialsByMesh.Find(mesh))
    {
        int wheelIndex = *p2;
        FName property = FName("None");
        double delta = 0.05;
        double min = 0;
        double max = 1;
        if (wheelIndex == 1)
        {
            property = PROPERTY_PAN;
            delta = 0.5;
            min = -90;
            max = 90;
        }
        else if (wheelIndex == 2)
        {
            property = PROPERTY_TILT;
            delta = 0.5;
            min = -90;
            max = 90;
        }
        else if (wheelIndex == 3)
        {
            property = PROPERTY_CYAN;
        }
        else if (wheelIndex == 4)
        {
            property = PROPERTY_YELLOW;
        }
        else if (wheelIndex == 5)
        {
            property = PROPERTY_MAGENTA;
        }
        else if (wheelIndex == 6)
        {
            property = PROPERTY_EDGE;
        }
        else if (wheelIndex == 7)
        {
            property = PROPERTY_ZOOM;
            delta = 0.5;
            min = 10;
            max = 50;
        }
        if(property == FName("None"))
            return;

        if(!activeSelection.chan)
            return;
        
        for (int i = 0; i < activeSelection.values.Num(); i++)
        {
            int ch = activeSelection.values[i];
            UEOSChannelView *chView = showfile->getChannel(ch);
            if(chView == nullptr)
                continue;
            if(!chView->hasProperty(property))
                continue;
            double val = chView->getProperty(property) + (direction * delta);
            if(val < min)
                val = min;
            else if (val > max)
                val = max;
            showfile->setManualProperty(ch, property, val);
        }
    }
}

void AETCLightBoard::onInputUpOM(FKeyEvent event)
{
    FName keyName = event.GetKey().GetFName();
    if (keyName == FName("Z"))
        shift = false;
}
void AETCLightBoard::onInputOM(FKeyEvent event)
{
    FName keyName = event.GetKey().GetFName();
    UE_LOG(LogTemp, Display, TEXT("Key input %s"), *keyName.ToString());
    if (keyName == FName("Z"))
        shift = true;
    else if(keyName == FName("NumPadZero"))
        onButton(BUTTON_0);
    else if(keyName == FName("NumPadOne"))
        onButton(BUTTON_1);
    else if(keyName == FName("NumPadTwo"))
        onButton(BUTTON_2);
    else if(keyName == FName("NumPadThree"))
        onButton(BUTTON_3);
    else if(keyName == FName("NumPadFour"))
        onButton(BUTTON_4);
    else if(keyName == FName("NumPadFive"))
        onButton(BUTTON_5);
    else if(keyName == FName("NumPadSix"))
        onButton(BUTTON_6);
    else if(keyName == FName("NumPadSeven"))
        onButton(BUTTON_7);
    else if(keyName == FName("NumPadEight"))
        onButton(BUTTON_8);
    else if(keyName == FName("NumPadNine"))
        onButton(BUTTON_9);
    else if(keyName == FName("BackSpace"))
        onButton(BUTTON_CLEAR);
    else if(keyName == FName("Enter"))
        onButton(BUTTON_ENTER);
    else if(keyName == FName("Add"))
        onButton(BUTTON_PLUS);
    else if(keyName == FName("T"))
        onButton(BUTTON_THRU);
    else if(keyName == FName("F"))
        onButton(BUTTON_FULL);
    else if(keyName == FName("O"))
        onButton(BUTTON_OUT);
    else if(keyName == FName("A"))
        onButton(BUTTON_AT);
    else if(keyName == FName("N"))
        onButton(BUTTON_SNEAK);
    else if(keyName == FName("U"))
        onButton(BUTTON_UPDATE);
    else if(keyName == FName("E"))
        onButton(BUTTON_RECALL_FROM);
    else if(keyName == FName("One"))
        onButton(BUTTON_SK1);
    else if(keyName == FName("Two"))
        onButton(BUTTON_SK2);
    else if(keyName == FName("Three"))
        onButton(BUTTON_SK3);
    else if(keyName == FName("Four"))
        onButton(BUTTON_SK4);
    else if(keyName == FName("Five"))
        onButton(BUTTON_SK5);
    else if(keyName == FName("Six"))
        onButton(BUTTON_SK6);
    else if(keyName == FName("Seven"))
        onButton(BUTTON_MORE_SK);
    else if(keyName == FName("SpaceBar"))
    {
        if(event.IsControlDown())
            onButton(BUTTON_BACK);
        else
            onButton(BUTTON_GO);
    }
}

void AETCLightBoard::onButton(FName button)
{
    if(button == BUTTON_LIVE)
    {
        mode = EEOSMode::LIVE;
        return;
    }
    else if(button == BUTTON_BLIND)
    {
        mode = EEOSMode::BLIND;
        return;
    }
    else if(button == BUTTON_STAGE)
    {
        mode = EEOSMode::STAGE;
        return;
    }
    else if(button == BUTTON_GO)
    {
        // TODO: update when Qs are added
        if(showfile != nullptr)
        {
            cueGo();
        }
        return;
    }
    else if(button == BUTTON_BACK)
    {
        // TODO: update when Qs are added
        if(showfile != nullptr)
        {
            cueBack();
        }
        return;
    }
    else if (shift && button == BUTTON_SK1)
    {
        setEncoderPage(FName("Intensity"));
        return;
    }
    else if (shift && button == BUTTON_SK2)
    {
        setEncoderPage(FName("Color"));
        return;
    }
    else if (shift && button == BUTTON_SK3)
    {
        setEncoderPage(FName("Focus"));
        return;
    }
    else if (shift && button == BUTTON_SK4)
    {
        setEncoderPage(FName("Form"));
        return;
    }
    else if (shift && button == BUTTON_SK5)
    {
        setEncoderPage(FName("Image"));
        return;
    }
    
    if(clearCmd)
    {
        command.Empty();
        clearCmd = false;
        if((button == BUTTON_FULL || button == BUTTON_OUT || button == BUTTON_AT || PROPERTIES.Contains(button)) && activeSelection.chan) {
            for (int i = 0; i < activeSelection.cmd.Num(); i++)
                command.Add(activeSelection.cmd[i]);
        }
    }
    cmdComplete = false;
    
    if(button == BUTTON_ENTER)
    {
        executeCommand();
        return;
    }
    else if(button == BUTTON_CLEAR)
    {
        if(commandError.Len() > 0)
            commandError = TEXT("");
        else if(command.Num() > 0) {
            if(shift)
                command.Empty();
            else
                command.RemoveAt(command.Num() - 1);
        }
        return;
    }
    else if(button == BUTTON_SHIFT)
    {
        shift = !shift;
        return;
    }
    if(shift && button == BUTTON_UPDATE)
    {
        if(showfile == nullptr)
            return;
        saveShowfile(showfile->fileName);
        return;
    }
    
    if(commandError.Len() > 0)
    {
        commandError = TEXT("");
    }

    if(UEOSButton::isNumeric(button) && command.Num() == 0)
    {
        command.Add(CMD_CHAN);
    }
    else if(button == BUTTON_DELAY)
    {
        if(shift)
        {
            button = CMD_FOLLOW;
        }
    }
    else if(button == BUTTON_FULL && command.Num() > 0 && command[command.Num()-1] == BUTTON_FULL)
    {
        executeCommand();
        return;
    }

    command.Add(button);
    if(button == BUTTON_OUT)
    {
        executeCommand();
        return;
    }
}

void AETCLightBoard::setButtonInteractionText_Implementation(UPrimitiveComponent *component, FName button)
{

}

void AETCLightBoard::executeCommand()
{

    activeSelection = FCmdSelection();
    if(command.Num() == 0)
        return;
    
    FString cmdStr = command[0].ToString();
    for(int i = 1; i < command.Num(); i++)
    {
        cmdStr += TEXT(" ") + command[i].ToString();
    }
    UE_LOG(LogTemp, Display, TEXT("Cmd execution: `%s`"), *cmdStr);

    if(command.Num() == 1 && command[0] == BUTTON_HIGH)
    {
        highlightMode = !highlightMode;
        finishCommand(true);
        return;
    }
    else if(command.Num() == 1 && command[0] == BUTTON_PARK)
    {
        if(parkedChannels.IsEmpty())
        {
            commandError = TEXT("No parked channels");
            finishCommand(true);
            return;
        }
        command.Empty();
        command.Add(CMD_UNPARK);
        confirmCmd = true;
        return;
    }
    else if(command.Num() == 1 && command[0] == CMD_UNPARK && confirmCmd)
    {
        finishCommand(true);
        parkedChannels.Empty();
        return;
    }

    if(command[0] == CMD_CHAN)
    {
        int next = 0;
        activeSelection = getCmdSelection(0, &next);
        if(next == 0)
        {
            commandError = TEXT("Invalid selection: ") + activeSelection.error;
            return;
        }
        updateEncoderPage(false);
        next++;
        UE_LOG(LogTemp, Display, TEXT("- Channel selection start"));
        UE_LOG(LogTemp, Display, TEXT("- next is %d (of %d)"), next, command.Num());
        if(next == command.Num())
        {
            finishCommand(false);
            return;
        }

        if(command[next] == BUTTON_PARK)
        {
            bool unpark = true;
            for (int i = 0; i < activeSelection.values.Num(); i++)
            {
                if(!parkedChannels.Contains(activeSelection.values[i])) {
                    unpark = false;
                    break;
                }
            }
            if(unpark)
            {
                command.RemoveAt(command.Num() - 1);
                command.Add(CMD_UNPARK);
                confirmCmd = true;
                return;
            }
            for (int i = 0; i < activeSelection.values.Num(); i++)
            {
                parkedChannels.Add(activeSelection.values[i]);
            }
            finishCommand(true);
            return;
        }
        else if(command[next] == CMD_UNPARK)
        {
            for (int i = 0; i < activeSelection.values.Num(); i++)
            {
                parkedChannels.Remove(activeSelection.values[i]);
            }
            finishCommand(true);
            return;
        }
        else if(command[next] == BUTTON_FULL || command[next] == BUTTON_OUT || command[next] == BUTTON_AT || PROPERTIES.Contains(command[next]))
        {
            FName action = command[next++];
            double v = (action == BUTTON_FULL) ? 1 : 0;
            FName property = PROPERTY_INTENSITY;
            if(PROPERTIES.Contains(action))
                property = action;
            if(action == BUTTON_AT || PROPERTIES.Contains(action))
            {
                if(command[next] == BUTTON_RECALL_FROM)
                {
                    FCmdSelection sel2 = getCmdSelection(next, &next);
                    if(!sel2.chan)
                    {
                        commandError = TEXT("Must select a channel");
                        return;
                    }
                    UEOSChannelView *view = showfile->getChannel(sel2.values[0]);
                    if(view == nullptr)
                    {
                        commandError = TEXT("Must select a valid channel");
                        return;
                    }
                    if(!view->hasProperty(property))
                    {
                        commandError = TEXT("Channel does not have parameter");
                        return;
                    }
                    v = view->getProperty(property);
                    UE_LOG(LogTemp, Display, TEXT("- Recall from: %f"), v);
                }
                else
                {
                    v = getCmdNumberD(next, &next);
                    next++;
                    if((v < 0 || v > 100) && property == PROPERTY_INTENSITY)
                    {
                        commandError = TEXT("Value must be between 0-100");
                        return;
                    }
                    UE_LOG(LogTemp, Display, TEXT("- At: %f"), v);
                }
            }
            float sneakTime = 0;
            if(command.Num() > next && command[next] == BUTTON_SNEAK)
            {
                sneakTime = 5;
                next++;
                if(command.Num() > next)
                {
                    sneakTime = getCmdNumberD(next, &next);
                    next++;
                    if (sneakTime == 0)
                    {
                        commandError = TEXT("Invalid sneak time");
                        return;
                    }
                }
            }
            for (int i = 0; i < activeSelection.values.Num(); i++)
            {
                int ch = activeSelection.values[i];
                if(sneakTime == 0)
                    showfile->setManualProperty(ch, property, v);
                else
                    showfile->addFade(ch, property, v, sneakTime, true, false);
            }
            finishCommand(true);
            return;
        }
        else if (command[next] == BUTTON_SNEAK)
        {
            next++;
            double time = 5;
            if(command.Num() > next)
            {
                time = getCmdNumberD(next, &next);
                if (time == 0)
                {
                    commandError = TEXT("Invalid sneak time");
                    return;
                }
            }
            UE_LOG(LogTemp, Display, TEXT("- Sneak"));
            for (int i = 0; i < activeSelection.values.Num(); i++)
            {
                int ch = activeSelection.values[i];
                UE_LOG(LogTemp, Display, TEXT("- Chan %d"), ch);
                showfile->addFade(ch, PROPERTY_INTENSITY, 0, time, true, true);
            }
            finishCommand(true);
            return;
        }
    }
    else if (command[0] == BUTTON_SNEAK)
    {
        int next = 1;
        double time = 5;
        if(command.Num() > next)
        {
            time = getCmdNumberD(next, &next);
            if (time == 0)
            {
                commandError = TEXT("Invalid sneak time");
                return;
            }
        }
        for(const TPair<int, UEOSPatchSet*>& pair : showfile->patch)
        {
            const int& ch = pair.Key;
            UEOSPropertySet *set = showfile->getManualChannel(ch);
            if(set == nullptr)
                continue;
            for (const TPair<FName, double> p2 : set->properties)
            {
                showfile->addFade(ch, p2.Key, 0, time, true, true);
            }
        }
        finishCommand(true);
        return;
    }
    else if(command[0] == BUTTON_GO_TO_CUE)
    {
        FString q = getCmdNumberS(1, nullptr);
        for (int i = 0; i < showfile->cues.Num(); i++)
        {
            if(showfile->cues[i]->cueNumber == q)
            {
                executeCue(i, 1);
            }
        }
        finishCommand(true);
        return;
    }
    
    for (int i = 0; i < command.Num(); i++)
    {
        if(command[i] == BUTTON_RECORD_ONLY) 
        {
            if(showfile == nullptr)
            {
                commandError = TEXT("No showfile exists");
                return;
            }
            int targetCue = -1;
            FCmdSelection *sel = nullptr;
            if(i > 0)
            {
                FCmdSelection selection = getCmdSelection(0, nullptr);
                sel = &selection;
            }

            if(command.Num() == i+1)
            {
                command.Add(BUTTON_CUE);
                UEOSButton::addNumberString(showfile->cues[showfile->currentCue]->cueNumber, &command);
                confirmCmd = true;
                return;
            }
            else if(command.Num() == i+3 && command[i+1] == BUTTON_CUE)
            {
                UEOSButton::addNumberString(showfile->cues[showfile->currentCue]->cueNumber, &command);
                confirmCmd = true;
                return;
            }
            else if(command.Num() < i+4)
            {
                commandError = TEXT("Invalid record command");
                return;
            }
            else if(command[i+1] != BUTTON_CUE)
            {
                commandError = TEXT("Invalid record target (only Cue supported)");
                return;
            }
            FString q = getCmdNumberS(i+2, nullptr);
            for (int j = 0; j < showfile->cues.Num(); j++)
            {
                if(showfile->cues[j]->cueNumber == q)
                {
                    targetCue, j;
                    break;
                }
            }

            if(targetCue == -1)
            {
                commandError = TEXT("Invalid record target: no such cue");
                return;
            }
            showfile->recordOnly(targetCue, (sel == nullptr) ? nullptr : &(sel->values));
        }
    }
    commandError = TEXT("Unknown command");
}
void AETCLightBoard::finishCommand(bool clear)
{
    clearCmd = clear;
    cmdComplete = true;
    confirmCmd = false;
}

void AETCLightBoard::setEncoderPage(FName category)
{
    if(!activeSelection.chan || showfile == nullptr)
    {
        encoderProperties.Empty();
        encoderPage = 0;
        encoderMaxPage = 0;
        encoderSelection.Empty();
        encoderCategory = FName("None");
        UE_LOG(LogTemp, Display, TEXT("Clearing encoder page"));
        return;
    }
    TSet<int> newSel;
    bool changed = false;
    for (int i = 0; i < activeSelection.values.Num(); i++)
    {
        int ch = activeSelection.values[i];
        if(showfile->patch.Contains(ch))
        {
            newSel.Add(ch);
            if (!encoderSelection.Contains(ch))
                changed = true;
        }
    }
    changed |= encoderSelection.Num() != newSel.Num();

    if (category == encoderCategory && !changed)
    {
        encoderPage++;
        if (encoderPage > encoderMaxPage)
            encoderPage = 0;
        UE_LOG(LogTemp, Display, TEXT("Set encoder page to %s, %d of %d"), *encoderCategory.ToString(), encoderPage, encoderMaxPage);
    }
    else
    {
        encoderCategory = category;
        updateEncoderPage(true);
        UE_LOG(LogTemp, Display, TEXT("Reset encoder page to %s, %d of %d"), *encoderCategory.ToString(), encoderPage, encoderMaxPage);
    }
}

void AETCLightBoard::updateEncoderPage(bool changed)
{
    if(encoderCategory == FName("None"))
        return;
    if(!activeSelection.chan || showfile == nullptr)
    {
        encoderProperties.Empty();
        encoderPage = 0;
        encoderMaxPage = 0;
        encoderSelection.Empty();
        encoderCategory = FName("None");
        UE_LOG(LogTemp, Display, TEXT("Clearing encoder page"));
        return;
    }
    TSet<int> newSel;
    for (int i = 0; i < activeSelection.values.Num(); i++)
    {
        int ch = activeSelection.values[i];
        if(showfile->patch.Contains(ch))
        {
            newSel.Add(ch);
            if (!encoderSelection.Contains(ch))
                changed = true;
        }
    }
    changed |= encoderSelection.Num() != newSel.Num();
    encoderSelection = newSel;
    if (!changed)
        return;
    encoderPage = 0;
    encoderMaxPage = 0;
    encoderProperties.Empty();
    TSet<FName> propSet;
    if (encoderCategory == FName("Intensity"))
    {
        propSet = PROPERTIES_INTENSITY;
    }
    else if (encoderCategory == FName("Color"))
    {
        propSet = PROPERTIES_COLOR;
    }
    else if (encoderCategory == FName("Focus"))
    {
        propSet = PROPERTIES_FOCUS;
    }
    else if (encoderCategory == FName("Form"))
    {
        propSet = PROPERTIES_FORM;
    }
    else if (encoderCategory == FName("Image"))
    {
        propSet = PROPERTIES_IMAGE;
    }

    int b = 0;
    for(const FName& property : propSet)
    {
        bool has = true;
        for(const int& ch : encoderSelection)
        {
            if(!showfile->patch[ch]->hasProperty(property)) {
                has = false;
                break;
            }
        }
        if(has)
        {
            encoderProperties.Add(property);
            b++;
            if(b > 6) {
                b = 1;
                encoderMaxPage++;
            }
        }
    }
    UE_LOG(LogTemp, Display, TEXT("New encoder pages generated, %d properties"), encoderProperties.Num());
}

int AETCLightBoard::getCmdNumber(int start, int* len)
{
    int c = 0;
    int l = 0;
    for (int i = start; i < command.Num(); i++)
    {
        FName n = command[i];
        if(n == BUTTON_0)
        {
            c = (c * 10) + 0;
        }
        else if(n == BUTTON_1)
        {
            c = (c * 10) + 1;
        }
        else if(n == BUTTON_2)
        {
            c = (c * 10) + 2;
        }
        else if(n == BUTTON_3)
        {
            c = (c * 10) + 3;
        }
        else if(n == BUTTON_4)
        {
            c = (c * 10) + 4;
        }
        else if(n == BUTTON_5)
        {
            c = (c * 10) + 5;
        }
        else if(n == BUTTON_6)
        {
            c = (c * 10) + 6;
        }
        else if(n == BUTTON_7)
        {
            c = (c * 10) + 7;
        }
        else if(n == BUTTON_8)
        {
            c = (c * 10) + 8;
        }
        else if(n == BUTTON_9)
        {
            c = (c * 10) + 9;
        }
        else if(n == BUTTON_MINUS)
        {
            c *= -1;
        }
        else
        {
            break;
        }
        l++;
    }
    if(len != nullptr)
        *len = l;
    return c;
}
double AETCLightBoard::getCmdNumberD(int start, int* len)
{
    double c = 0;
    int l = 0;
    bool dec = false;
    bool neg = false;
    double nextBase = 0.1;
    for (int i = start; i < command.Num(); i++)
    {
        FName n = command[i];
        int nx = -1;
        if(n == BUTTON_DOT)
        {
            dec = true;
        }
        else if(n == BUTTON_MINUS)
        {
            neg = true;
        }
        else if(n == BUTTON_0)
        {
            nx =  0;
        }
        else if(n == BUTTON_1)
        {
            nx =  1;
        }
        else if(n == BUTTON_2)
        {
            nx =  2;
        }
        else if(n == BUTTON_3)
        {
            nx =  3;
        }
        else if(n == BUTTON_4)
        {
            nx =  4;
        }
        else if(n == BUTTON_5)
        {
            nx =  5;
        }
        else if(n == BUTTON_6)
        {
            nx =  6;
        }
        else if(n == BUTTON_7)
        {
            nx =  7;
        }
        else if(n == BUTTON_8)
        {
            nx =  8;
        }
        else if(n == BUTTON_9)
        {
            nx =  9;
        }
        else
        {
            break;
        }
        if(nx != -1)
        {
            if(dec)
            {
                c += (nx * nextBase);
                nextBase /= 10;
            }
            else
            {
                c = (c * 10) + nx;
            }
        }
        l++;
    }
    if(len != nullptr)
        *len = l;
    if (neg)
        c *= -1;
    return c;
}
FString AETCLightBoard::getCmdNumberS(int start, int* len)
{
    FString out;
    int l = 0;
    for (int i = start; i < command.Num(); i++)
    {
        FName n = command[i];
        if(n == BUTTON_DOT)
        {
            out += TEXT(".");
        }
        else if(n == BUTTON_0)
        {
            out += TEXT("0");
        }
        else if(n == BUTTON_1)
        {
            out += TEXT("1");
        }
        else if(n == BUTTON_2)
        {
            out += TEXT("2");
        }
        else if(n == BUTTON_3)
        {
            out += TEXT("3");
        }
        else if(n == BUTTON_4)
        {
            out += TEXT("4");
        }
        else if(n == BUTTON_5)
        {
            out += TEXT("5");
        }
        else if(n == BUTTON_6)
        {
            out += TEXT("6");
        }
        else if(n == BUTTON_7)
        {
            out += TEXT("7");
        }
        else if(n == BUTTON_8)
        {
            out += TEXT("8");
        }
        else if(n == BUTTON_9)
        {
            out += TEXT("9");
        }
        else
        {
            break;
        }
        l++;
    }
    if(len != nullptr)
        *len = l;
    return out;
}

FCmdSelection AETCLightBoard::getCmdSelection(int start, int *end)
{
    FCmdSelection selection;
    int i = start;
    int last = -1;
    bool minus = false;
    selection.chan = command[i] == CMD_CHAN;
    selection.cue = command[i] == BUTTON_CUE;
    selection.sub = command[i] == BUTTON_SUB;
    selection.group = command[i] == BUTTON_GROUP;
    if(!(selection.chan || selection.cue || selection.sub || selection.group))
    {
        if(end != nullptr)
            *end = i-2;
        selection.error = TEXT("No valid selector type");
        return selection;
    }
    selection.cmd.Add(command[i]);
    i++;
    int itCount = 0;
    int cmdLength = command.Num();
    while (i < cmdLength)
    {
        itCount++;
        if(itCount > cmdLength*2)
        {
            selection.error = TEXT("Iteration count {");
            for (int j = 0; j < selection.values.Num(); j++)
            {
                if(j > 0)
                    selection.error += TEXT(",");
                selection.error += FString::FromInt(selection.values[j]);
            }
            selection.error += TEXT("}");
            break;
        }
        FName val = command[i];
        if(val == BUTTON_THRU)
        {
            if(last == -1)
            {
                selection.error = TEXT("Found tru first");
                break;
            }
            i++;
            int p;
            selection.cmd.Add(val);
            int n = getCmdNumber(i, &p);
            for (int j = i; j < i + p; j++) 
                selection.cmd.Add(command[j]);
            i += p;
            if(n > last)
            {
                for (int j = last; j <= n; j++)
                {
                    if(minus)
                        selection.values.Remove(j);
                    else
                        selection.values.AddUnique(j);
                }
            }
            else
            {
                for (int j = last; j >= n; j--)
                {
                    if(minus)
                        selection.values.Remove(j);
                    else
                        selection.values.AddUnique(j);
                }
            }
            last = n;
            minus = false;
        }
        else if(val == BUTTON_PLUS)
        {
            if(last == -1)
            {
                selection.error = TEXT("Found + first");
                break;
            }
            i++;
            int p;
            selection.cmd.Add(val);
            int n = getCmdNumber(i, &p);
            for (int j = i; j < i + p; j++) 
                selection.cmd.Add(command[j]);
            i += p;
            last = n;
            selection.values.AddUnique(n);
            minus = false;
        }
        else if(val == BUTTON_MINUS)
        {
            if(last == -1)
            {
                selection.error = TEXT("Found + first");
                break;
            }
            i++;
            int p;
            selection.cmd.Add(val);
            int n = getCmdNumber(i, &p);
            for (int j = i; j < i + p; j++) 
                selection.cmd.Add(command[j]);
            i += p;
            last = n;
            selection.values.Remove(n);
            minus = true;
        }
        else
        {
            int p;
            int n = getCmdNumber(i, &p);
            for (int j = i; j < i + p; j++) 
                selection.cmd.Add(command[j]);
            if(p == 0)
            {
                selection.error = TEXT("No values");
                break;
            }
            i += p;
            last = n;
            selection.values.AddUnique(n);
        }
    }
    if(end != nullptr)
        *end = i-1;
    return selection;
}

void AETCLightBoard::onNetworkPacket(UNetworkPacket *packet)
{
    
}

bool AETCLightBoard::loadShowfile(FString filePath)
{
    if(filePath.Len() <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load showfile: empty path"));
        return false;
    }
    UE_LOG(LogTemp, Display, TEXT("Loading EOS showfile: %s"), *filePath);

    FString fullPathContent = FPaths::ProjectContentDir() + "/" + filePath;
    FString fullPathSaved = FPaths::ProjectSavedDir() + "/SavedGames/EOS/" + filePath;
    FString fullPath = fullPathSaved;
    FString jsonString;
    if(!FFileHelper::LoadFileToString(jsonString, *fullPath)) // try saved first, then default from content
    {
        fullPath = fullPathContent;
        if(!FFileHelper::LoadFileToString(jsonString, *fullPath))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load showfile: Could not open file (%s)"), *fullPath);
            return false;
        }
    }

    TSharedPtr<FJsonObject> jsonObject;
    if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(jsonString), jsonObject) || !jsonObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load showfile: Could not parse json (%s)"), *fullPath);
        return false;
    }

    UEOSShowfile *newShowfile = UEOSShowfile::create(filePath);
    if(!newShowfile->loadFromJson(jsonObject))
    {
        return false;
    }
    showfile = newShowfile;
    for (int i = 0; i <= showfile->currentCue; i++)
    {
        UEOSCue *cue = showfile->cues[i];
        for(TPair<int, UEOSPropertySet*> chPair : cue->actions)
        {
            showfile->setCueProperties(chPair.Key, chPair.Value);
        }
    }

    return true;
}

void AETCLightBoard::saveShowfile(FString filePath)
{
    if(showfile == nullptr)
    {
        return;
    }
    if(filePath.Len() <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save showfile: Empty path"));
        return;
    }
    FString fullPath = FPaths::ProjectSavedDir() + "/SavedGames/EOS/" + filePath;

    FString jsonString;
    TSharedPtr<FJsonObject> jsonObject = showfile->toJson();
    if(!FJsonSerializer::Serialize(jsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&jsonString, 0)))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save showfile: Could serialize json (%s)"), *fullPath);
        return;
    }

    if(!FFileHelper::SaveStringToFile(jsonString, *fullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save showfile: Could not write file (%s)"), *fullPath);
        return;
    }
    UE_LOG(LogTemp, Display, TEXT("Saved EOS showfile: %s"), *fullPath);
}

void AETCLightBoard::executeCue(int cueI, double time)
{
    if(showfile == nullptr)
        return;
    if(cueI >= showfile->cues.Num() || cueI < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid cue index: %d"), cueI);
        return;
    }
    UEOSCue *cue = showfile->cues[cueI];
    if(time == -1)
    {
        time = cue->time;
    }
    UE_LOG(LogTemp, Display, TEXT("Executing cue %s (#%d)"), *(cue->cueNumber), cueI);
    if(cueI < showfile->currentCue) // we went back, so make sure we deal with tracking
    {
        for (int i = 0; i < cueI; i++)
        {
            UEOSCue *cueB = showfile->cues[i];
            for(TPair<int, UEOSPropertySet*> chPair : cueB->actions)
            {
                if(cue->actions.Contains(chPair.Key))
                    continue;
                if(time == 0)
                {
                    showfile->setCueProperties(chPair.Key, chPair.Value);
                }
                else
                {
                    for(TPair<FName, double> paramPair : chPair.Value->properties)
                        showfile->addFade(chPair.Key, paramPair.Key, paramPair.Value, time, false, false);
                }
            }
        }
    }
    for(TPair<int, UEOSPropertySet*> chPair : cue->actions)
    {
        if(time == 0)
        {
            showfile->setCueProperties(chPair.Key, chPair.Value);
        }
        else
        {
            for(TPair<FName, double> paramPair : chPair.Value->properties)
                showfile->addFade(chPair.Key, paramPair.Key, paramPair.Value, time, false, false);
        }
    }
    if(cueI == showfile->currentCue + 1)
    {
        if(cue->follow != -1)
        {
            followTime = cue->follow;
            UE_LOG(LogTemp, Display, TEXT("Follow for cue %s (#%d)"), *(cue->cueNumber), cueI);
        }
        else if(cue->hang != -1)
        {
            followTime = time + cue->hang;
            UE_LOG(LogTemp, Display, TEXT("Hang for cue %s (#%d)"), *(cue->cueNumber), cueI);
        }
        else
        {
            followTime = -1;
        }
    }

    showfile->currentCue = cueI;
    cueTime = time;

    if(followTime == 0)
    {
        executeCue(cueI + 1, -1);
    }
}

void AETCLightBoard::cueGo()
{
    if(showfile == nullptr)
        return;
    executeCue(showfile->currentCue + 1, -1);
}

void AETCLightBoard::cueBack()
{
    if(showfile == nullptr)
        return;
    executeCue(showfile->currentCue - 1, 1);
}

void AETCLightBoard::onTimecode(int frames)
{
    if(showfile == nullptr)
        return;
    int nextI = showfile->currentCue + 1;
    if(nextI >= showfile->cues.Num())
        return;
    UEOSCue *nextCue = showfile->cues[nextI];
    if(nextCue->timecode != frames)
        return;
    executeCue(nextI);
}

FName AETCLightBoard::getEncoderProperty(int index)
{
    index += encoderPage * 6;
    if(encoderProperties.Num() <= index)
        return FName("None");
    return encoderProperties[index];
}