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
}

// Called when the game starts or when spawned
void AETCLightBoard::BeginPlay()
{
	Super::BeginPlay();
    networkCard->onNetworkPacket.AddDynamic(this, &AETCLightBoard::onNetworkPacket);
    for (int u = 0; u < 10; u++)
    {
        networkCard->addUniverse(u);
    }

    if(showfile == nullptr)
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
                FEOSPatch patch = EOSPatchTypes::create(light.type);
                patch.universe = light.universe;
                patch.address = light.address;
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

    for (int u = 0; u < 10; u++)
    {
        if(!networkCard->hasChanged(u))
            continue;
        updateUniverse(u, networkCard->getData(u));
        networkCard->clearChanged(u);
    }
}

void AETCLightBoard::updateUniverse(int universe, TArray<int> dmx)
{
    if(showfile == nullptr)
        return;
    for (auto &elem : showfile->patch)
    {
        FEOSPatchSet patchSet = elem.Value;
        int ch = elem.Key;
        for (int i = 0; i < patchSet.devices.Num(); i++)
        {
            FEOSPatch patch = patchSet.devices[i];
            if(patch.universe == universe)
            {
                EOSLightOutputType* outputType = EOSLightOutputType::getType(patch.type);
                TArray<int> d2;
                d2.Init(0, patch.size);
                for (int j = 0; j < patch.size; j++)
                {
                    d2[j] = dmx[j+patch.address];
                }
                outputType->input(d2, showfile->channels[ch].properties);
            }
        }
    }
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
        FName button = *buttonPntr;
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
            return;
        }
        else if(button == BUTTON_BACK)
        {
            // TODO: update when Qs are added
            return;
        }
        
        if(clearCmd)
        {
            command.Empty();
            clearCmd = false;
        }
        
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
}

void AETCLightBoard::setButtonInteractionText_Implementation(UPrimitiveComponent *component, FName button)
{

}

void AETCLightBoard::executeCommand()
{
    if(command.Num() == 0)
        return;
    else if(command.Num() == 1 && command[0] == BUTTON_HIGH)
    {
        highlightMode = !highlightMode;
        clearCmd = true;
        return;
    }
    else if(command.Num() == 1 && command[0] == BUTTON_PARK)
    {
        if(parkedChannels.IsEmpty())
        {
            commandError = TEXT("No parked channels");
            clearCmd = true;
            return;
        }
        command.Empty();
        command.Add(CMD_UNPARK);
        confirmCmd = true;
        return;
    }
    else if(command.Num() == 1 && command[0] == CMD_UNPARK && confirmCmd)
    {
        confirmCmd = false;
        clearCmd = true;
        parkedChannels.Empty();
        return;
    }
    else if(command[command.Num()-1] == BUTTON_PARK)
    {
        if(command[0] != CMD_CHAN)
        {
            commandError = TEXT("Can only park channels");
            return;
        }
        int c = getCmdNumber(1, nullptr);
        if(parkedChannels.Contains(c))
        {
            command.RemoveAt(command.Num() - 1);
            command.Add(CMD_UNPARK);
            confirmCmd = true;
            return;
        }
        parkedChannels.Add(c);
        clearCmd = true;
        return;
    }
    else if(command[command.Num()-1] == CMD_UNPARK)
    {
        int c = getCmdNumber(1, nullptr);
        parkedChannels.Remove(c);
        clearCmd = true;
        confirmCmd = false;
        return;
    }
    else if(command[command.Num()-1] == BUTTON_FULL || command[command.Num()-1] == BUTTON_OUT)
    {
        int p;
        FCmdSelection selection = getCmdSelection(0, &p);
        if(p == 0 || selection.cue)
        {
            if(selection.cue)
                commandError = TEXT("Invalid selection: Can not set intensity of cue");
            else
                commandError = TEXT("Invalid selection: ") + selection.error;
            return;
        }
        double v = command[command.Num() - 1] == BUTTON_FULL ? 1.0 : 0.0;
        for (int i = 0; i < selection.values.Num(); i++)
        {
            int c = selection.values[i];
            if(parkedChannels.Contains(c))
                continue;
            if(FEOSPropertySet *set = showfile->channels.Find(c))
            {
                set->set(PROPERTY_INTENSITY, v);
            }
        }
        clearCmd = true;
        return;
    }
    else if(command.Num() > 1 && command[command.Num()-2] == BUTTON_AT)
    {
        int p;
        FCmdSelection selection = getCmdSelection(0, &p);
        if(p == 0 || selection.cue)
        {
            if(selection.cue)
                commandError = TEXT("Invalid selection: Can not set intensity of cue");
            else
                commandError = TEXT("Invalid selection: ") + selection.error;
            return;
        }
        int n = getCmdNumber(p, nullptr);
        double v = (double)n / 100.0;
        for (int i = 0; i < selection.values.Num(); i++)
        {
            int c = selection.values[i];
            if(parkedChannels.Contains(c))
                continue;
            if(FEOSPropertySet *set = showfile->channels.Find(c))
            {
                set->set(PROPERTY_INTENSITY, v);
            }
        }
        clearCmd = true;
        return;
    }
    commandError = TEXT("Unknown command");
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
            int n = getCmdNumber(i, &p);
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
            int n = getCmdNumber(i, &p);
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
            int n = getCmdNumber(i, &p);
            i += p;
            last = n;
            selection.values.Remove(n);
            minus = true;
        }
        else
        {
            int p;
            int n = getCmdNumber(i, &p);
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