// Copyright Peter Crall 2025

#include "EOS/ETCLightBoard.h"

// Sets default values
AETCLightBoard::AETCLightBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AETCLightBoard::BeginPlay()
{
	Super::BeginPlay();

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
            UEOSButton* button = NewObject<UEOSButton>();
            button->setup(buttonName, comp, FColor(255, 255, 255));
            buttonByName.Add(buttonName, button);
            if(FColor* color = buttonColors.Find(buttonName))
            {
                button->makeUnique(UMaterialInstanceDynamic::Create(buttonMaterial, this));
                button->setActiveColor(*color);
                // UE_LOG(LogTemp, Display, TEXT("EOS Button: `%s` (UNIQUE)"), *buttonName.ToString());
            }
            // UE_LOG(LogTemp, Display, TEXT("EOS Button: `%s`"), *buttonName.ToString());
        }
    }

    setButtonColor(BUTTON_LIVE, FColor(255, 255, 255), FColor(255, 255, 0));
    setButtonColor(BUTTON_BLIND, FColor(255, 255, 255), FColor(32, 32, 255));
    setButtonColor(BUTTON_STAGE, FColor(255, 255, 255), FColor(32, 255, 32));
    
    setButtonColor(BUTTON_HIGH, FColor(255, 255, 255), FColor(255, 255, 0));
    
    setButtonColor(BUTTON_CLEAR, FColor(255, 255, 255), FColor(255, 32, 32));
}

// Called every frame
void AETCLightBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    setButtonActive(BUTTON_LIVE, mode == LIVE);
    setButtonActive(BUTTON_BLIND, mode == BLIND);
    setButtonActive(BUTTON_STAGE, mode == STAGE);
    
    setButtonActive(BUTTON_HIGH, highlightMode);

    setButtonActive(BUTTON_PARK, !parkedChannels.IsEmpty());

    setButtonActive(BUTTON_CLEAR, !command.IsEmpty());
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
            mode = LIVE;
            return;
        }
        else if(button == BUTTON_BLIND)
        {
            mode = BLIND;
            return;
        }
        else if(button == BUTTON_STAGE)
        {
            mode = STAGE;
            return;
        }
        else if(button == BUTTON_ENTER)
        {
            executeCommand();
            return;
        }
        else if(button == BUTTON_CLEAR)
        {
            commandError = TEXT("");
            if(clearCmd)
            {
                command.Empty();
                clearCmd = false;
            }
            else if(command.Num() > 0)
                command.RemoveAt(command.Num() - 1);
            return;
        }
        else if(button == BUTTON_SHIFT)
        {
            shift = !shift;
            return;
        }
        if(clearCmd)
        {
            command.Empty();
            clearCmd = false;
        }
        if(commandError.Len() > 0)
        {
            commandError = TEXT("");
        }

        if(UEOSButton::isNumeric(button) && command.Num() == 0)
        {
            command.Add(FName(TEXT("Chan")));
        }

        command.Add(button);

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
        command.Add(FName(TEXT("Unpark")));
        confirmCmd = true;
        return;
    }
    else if(command.Num() == 1 && command[0] == FName(TEXT("Unpark")) && confirmCmd)
    {
        confirmCmd = false;
        clearCmd = true;
        parkedChannels.Empty();
        return;
    }
    else if(command[command.Num()-1] == BUTTON_PARK)
    {
        if(command[0] != FName(TEXT("Chan")))
        {
            commandError = TEXT("Can only park channels");
            return;
        }
        int c = getCmdNumber(1);
        if(parkedChannels.Contains(c))
        {
            command.RemoveAt(command.Num() - 1);
            command.Add(FName(TEXT("Unpark")));
            confirmCmd = true;
            return;
        }
        parkedChannels.Add(c);
        clearCmd = true;
        return;
    }
    else if(command[command.Num()-1] == FName(TEXT("Unpark")))
    {
        int c = getCmdNumber(1);
        parkedChannels.Remove(c);
        clearCmd = true;
        confirmCmd = false;
        return;
    }
    commandError = TEXT("Unknown command");
}

int AETCLightBoard::getCmdNumber(int start)
{
    int c = 0;
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
    }
    return c;
}