// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TextRenderComponent.h"
#include "EOSButtons.generated.h"

const FName BUTTON_LIVE = FName(TEXT("Live"));
const FName BUTTON_BLIND = FName(TEXT("Blind"));
const FName BUTTON_STAGE = FName(TEXT("Stage"));

const FName BUTTON_DISPLAYS = FName(TEXT("Displays"));
const FName BUTTON_SYSTEM = FName(TEXT("System"));

const FName BUTTON_CLEAR = FName(TEXT("Clear"));
const FName BUTTON_ENTER = FName(TEXT("Enter"));

const FName BUTTON_ABOUT = FName(TEXT("About"));
const FName BUTTON_HELP = FName(TEXT("Help"));

const FName BUTTON_SHIFT = FName(TEXT("Shift"));
const FName BUTTON_DELETE = FName(TEXT("Delete"));
const FName BUTTON_UNDO = FName(TEXT("Undo"));
const FName BUTTON_SCROLL_LOCK = FName(TEXT("ScrollLock"));
const FName BUTTON_ESCAPE = FName(TEXT("Escape"));
const FName BUTTON_SELECT = FName(TEXT("Select"));

const FName BUTTON_TAB = FName(TEXT("Tab"));
const FName BUTTON_FORMAT = FName(TEXT("Format"));
const FName BUTTON_EXPAND = FName(TEXT("Expand"));
const FName BUTTON_DATA = FName(TEXT("Data"));
const FName BUTTON_FIND = FName(TEXT("Find"));
// const FName BUTTON_TIME_B = FName(TEXT("TimeB"));

const FName BUTTON_LEFT = FName(TEXT("Left"));
const FName BUTTON_RIGHT = FName(TEXT("Right"));
const FName BUTTON_UP = FName(TEXT("Up"));
const FName BUTTON_DOWN = FName(TEXT("Down"));

const FName BUTTON_GO = FName(TEXT("Go"));
const FName BUTTON_BACK = FName(TEXT("Back"));

const FName BUTTON_0 = FName(TEXT("0"));
const FName BUTTON_1 = FName(TEXT("1"));
const FName BUTTON_2 = FName(TEXT("2"));
const FName BUTTON_3 = FName(TEXT("3"));
const FName BUTTON_4 = FName(TEXT("4"));
const FName BUTTON_5 = FName(TEXT("5"));
const FName BUTTON_6 = FName(TEXT("6"));
const FName BUTTON_7 = FName(TEXT("7"));
const FName BUTTON_8 = FName(TEXT("8"));
const FName BUTTON_9 = FName(TEXT("9"));

const FName BUTTON_DOT = FName(TEXT("Dot"));
const FName BUTTON_PLUS = FName(TEXT("Plus"));
const FName BUTTON_MINUS = FName(TEXT("Minus"));
const FName BUTTON_THRU = FName(TEXT("Thru"));

const FName BUTTON_AT = FName(TEXT("At"));
const FName BUTTON_FULL = FName(TEXT("Full"));
const FName BUTTON_OUT = FName(TEXT("Out"));
const FName BUTTON_PLUS_PERCENT = FName(TEXT("PlusPercent"));
const FName BUTTON_MINUS_PERCENT = FName(TEXT("MinusPercent"));

const FName BUTTON_REM_DIM = FName(TEXT("RemDim"));
const FName BUTTON_SLASH = FName(TEXT("Slash"));

const FName BUTTON_DELAY = FName(TEXT("Delay"));
const FName BUTTON_TIME = FName(TEXT("Time"));

const FName BUTTON_PRESET = FName(TEXT("Preset"));
const FName BUTTON_SUB = FName(TEXT("Sub"));
const FName BUTTON_GROUP = FName(TEXT("Group"));
const FName BUTTON_CUE = FName(TEXT("Cue"));
const FName BUTTON_PART = FName(TEXT("Part"));
const FName BUTTON_EFFECT = FName(TEXT("Effect"));
const FName BUTTON_PATH = FName(TEXT("Path"));

const FName BUTTON_UPDATE = FName(TEXT("Update"));
const FName BUTTON_RECORD = FName(TEXT("Record"));
const FName BUTTON_RECORD_ONLY = FName(TEXT("RecordOnly"));

const FName BUTTON_INT_PALLET = FName(TEXT("IntPallet"));
const FName BUTTON_FOCUS_PALLET = FName(TEXT("FocusPallet"));
const FName BUTTON_COLOR_PALLET = FName(TEXT("ColorPallet"));
const FName BUTTON_BEAM_PALLET = FName(TEXT("BeamPallet"));

const FName BUTTON_GO_TO_CUE = FName(TEXT("GoToCue"));

const FName BUTTON_BLOCK = FName(TEXT("Block"));
const FName BUTTON_ASSERT = FName(TEXT("Assert"));
const FName BUTTON_QUERY = FName(TEXT("Query"));

const FName BUTTON_COPY_TO = FName(TEXT("CopyTo"));
const FName BUTTON_RECALL_FROM = FName(TEXT("RecallFrom"));

const FName BUTTON_LABEL = FName(TEXT("LabelName"));
const FName BUTTON_OFFSET = FName(TEXT("Offset"));
const FName BUTTON_HIGH = FName(TEXT("High"));
const FName BUTTON_FAN = FName(TEXT("Fan"));

const FName BUTTON_SNAPSHOT = FName(TEXT("Snapshot"));
const FName BUTTON_FADER = FName(TEXT("Fader"));
const FName BUTTON_ADDRESS = FName(TEXT("AddressPatch"));

const FName BUTTON_MACRO = FName(TEXT("Macro"));
const FName BUTTON_LEARN = FName(TEXT("Learn"));

const FName BUTTON_SK1 = FName(TEXT("SK1"));
const FName BUTTON_SK2 = FName(TEXT("SK2"));
const FName BUTTON_SK3 = FName(TEXT("SK3"));
const FName BUTTON_SK4 = FName(TEXT("SK4"));
const FName BUTTON_SK5 = FName(TEXT("SK5"));
const FName BUTTON_SK6 = FName(TEXT("SK6"));
const FName BUTTON_MORE_SK = FName(TEXT("MoreSK"));

const FName BUTTON_PARK = FName(TEXT("Park"));
const FName BUTTON_CAPTURE = FName(TEXT("Capture"));

const FName BUTTON_SELECT_LAST = FName(TEXT("SelectLast"));
const FName BUTTON_SELECT_MANUAL = FName(TEXT("SelectManual"));
const FName BUTTON_SELECT_ACTIVE = FName(TEXT("SelectActive"));
const FName BUTTON_LAST = FName(TEXT("Last"));
const FName BUTTON_NEXT = FName(TEXT("Next"));

const FName BUTTON_RELEASE = FName(TEXT("Release"));
const FName BUTTON_LOAD = FName(TEXT("Load"));
// const FName BUTTON_ASSERT_B = FName(TEXT("AssertB"));
const FName BUTTON_OFF = FName(TEXT("Off"));
const FName BUTTON_FREEZE = FName(TEXT("Freeze"));
const FName BUTTON_SPREAD = FName(TEXT("Spread"));
const FName BUTTON_FILTER = FName(TEXT("Filter"));
const FName BUTTON_TIMING_DISABLE = FName(TEXT("TimingDisable"));

const FName BUTTON_FADER_1 = FName(TEXT("Fader1"));
const FName BUTTON_FADER_1_GO = FName(TEXT("Fader1Go"));
const FName BUTTON_FADER_1_BACK = FName(TEXT("Fader1Back"));

const FName BUTTON_FADER_2 = FName(TEXT("Fader2"));
const FName BUTTON_FADER_2_GO = FName(TEXT("Fader2Go"));
const FName BUTTON_FADER_2_BACK = FName(TEXT("Fader2Back"));

const FName BUTTON_FADER_3 = FName(TEXT("Fader3"));
const FName BUTTON_FADER_3_GO = FName(TEXT("Fader3Go"));
const FName BUTTON_FADER_3_BACK = FName(TEXT("Fader3Back"));

const FName BUTTON_FADER_4 = FName(TEXT("Fader4"));
const FName BUTTON_FADER_4_GO = FName(TEXT("Fader4Go"));
const FName BUTTON_FADER_4_BACK = FName(TEXT("Fader4Back"));

const FName BUTTON_FADER_5 = FName(TEXT("Fader5"));
const FName BUTTON_FADER_5_GO = FName(TEXT("Fader5Go"));
const FName BUTTON_FADER_5_BACK = FName(TEXT("Fader5Back"));

const FName BUTTON_USER_1 = FName(TEXT("User1"));
const FName BUTTON_USER_2 = FName(TEXT("User2"));

const FName BUTTON_USER_L_1 = FName(TEXT("UserL1"));
const FName BUTTON_USER_L_2 = FName(TEXT("UserL2"));
const FName BUTTON_USER_L_3 = FName(TEXT("UserL3"));
const FName BUTTON_USER_L_4 = FName(TEXT("UserL4"));
const FName BUTTON_USER_L_5 = FName(TEXT("UserL5"));
const FName BUTTON_USER_L_6 = FName(TEXT("UserL6"));
const FName BUTTON_USER_L_7 = FName(TEXT("UserL7"));
const FName BUTTON_USER_L_8 = FName(TEXT("UserL8"));
const FName BUTTON_USER_L_9 = FName(TEXT("UserL9"));
const FName BUTTON_USER_L_10 = FName(TEXT("UserL10"));

const FName BUTTON_USER_R_1 = FName(TEXT("UserR1"));
const FName BUTTON_USER_R_2 = FName(TEXT("UserR2"));
const FName BUTTON_USER_R_3 = FName(TEXT("UserR3"));
const FName BUTTON_USER_R_4 = FName(TEXT("UserR4"));
const FName BUTTON_USER_R_5 = FName(TEXT("UserR5"));
const FName BUTTON_USER_R_6 = FName(TEXT("UserR6"));
const FName BUTTON_USER_R_7 = FName(TEXT("UserR7"));
const FName BUTTON_USER_R_8 = FName(TEXT("UserR8"));
const FName BUTTON_USER_R_9 = FName(TEXT("UserR9"));
const FName BUTTON_USER_R_10 = FName(TEXT("UserR10"));

UCLASS()
class CONCEPTTHEATERSIM_API UEOSButton : public UObject
{
	GENERATED_BODY()
public:
    UEOSButton()
    {
        ID = FName(TEXT("Unknown"));
        textRenderer = nullptr;
        buttonColor = FColor(255, 255, 255);
        activeColor = FColor(255, 255, 255);
    }

    void setup(FName ID_, UTextRenderComponent *textRenderer_, FColor buttonColor_)
    {
        ID = ID_;
        textRenderer = textRenderer_;
        buttonColor = buttonColor_;
        activeColor = buttonColor;
    }

    void setup(FName ID_, UTextRenderComponent *textRenderer_, FColor buttonColor_, FColor activeColor_)
    {
        ID = ID_;
        textRenderer = textRenderer_;
        buttonColor = buttonColor_;
        activeColor = activeColor_;
    }

    UEOSButton* setIntensity(float intensity_)
    {
        intensity = intensity_;
        updateRender();
        return this;
    }

    float getIntensity()
    {
        return intensity;
    }

    UEOSButton* setActive(bool active_)
    {
        active = active_;
        updateRender();
        return this;
    }

    bool isActive()
    {
        return active;
    }

    FName getID()
    {
        return ID;
    }

    UEOSButton* makeUnique(UMaterialInstanceDynamic* buttonMaterial_, UMaterialInstanceDynamic* indicatorMaterial_)
    {
        buttonMaterial = buttonMaterial_;
        indicatorMaterial = indicatorMaterial_;
        if(textRenderer != nullptr)
        {
            textRenderer->SetTextMaterial(buttonMaterial);
        }
        if(mesh != nullptr)
        {
            mesh->SetMaterial(buttonMaterialIndex, indicatorMaterial);
        }
        updateRender(true);
        return this;
    }

    UEOSButton* setIndicatorMesh(UStaticMeshComponent* mesh_)
    {
        return setIndicatorMesh(mesh_, 0);
    }
    UEOSButton* setIndicatorMesh(UStaticMeshComponent* mesh_, int index)
    {
        mesh = mesh_;
        buttonMaterialIndex = index;
        if(buttonMaterial != nullptr)
        {
            mesh->SetMaterial(buttonMaterialIndex, indicatorMaterial);
        }
        return this;
    }
    UEOSButton* setTextRenderer(UTextRenderComponent* textRenderer_)
    {
        textRenderer = textRenderer_;
        if(buttonMaterial != nullptr)
        {
            textRenderer->SetTextMaterial(buttonMaterial);
        }
        updateRender(true);
        return this;
    }

    UEOSButton* setButtonColor(FColor buttonColor_)
    {
        buttonColor = buttonColor_;
        return this;
    }

    UEOSButton* setButtonColor(FColor buttonColor_, FColor activeColor_)
    {
        buttonColor = buttonColor_;
        activeColor = activeColor_;
        return this;
    }

    UEOSButton* setActiveColor(FColor activeColor_)
    {
        activeColor = activeColor_;
        return this;
    }

    void updateRender()
    {
        updateRender(false);
    }
    void updateRender(bool force)
    {   
        if(lastActive == active && !force)
        {
            return;
        }
        lastActive = active;
        FColor color = active ? activeColor : buttonColor;
        float i2 = active ? 1.0f : intensity;

        // if(textRenderer != nullptr && IsValid(textRenderer))
        // {
        //     textRenderer->TextRenderColor = FColor((int)(intensity * color.R), (int)(intensity * color.G), (int)(intensity * color.B));
        // }
        FLinearColor c2 = FLinearColor(((float)color.R) / 255.0, ((float)color.G) / 255.0, ((float)color.B) / 255.0, 1.0);
        if(buttonMaterial != nullptr && IsValid(buttonMaterial))
        {
            buttonMaterial->SetScalarParameterValue("Intensity", i2);
            buttonMaterial->SetVectorParameterValue("Color", c2);
        }
        if(indicatorMaterial != nullptr &&  && IsValid(indicatorMaterial))
        {
            indicatorMaterial->SetScalarParameterValue("Intensity", i2);
            indicatorMaterial->SetVectorParameterValue("Color", c2);
        }
    }

    static TMap<const FName, FColor> getButtonColors()
    {
        TMap<const FName, FColor> map;

        map.Add(BUTTON_LIVE, FColor(255, 255, 0));
        map.Add(BUTTON_BLIND, FColor(32, 32, 255));
        map.Add(BUTTON_STAGE, FColor(32, 255, 32));
        
        map.Add(BUTTON_CLEAR, FColor(255, 32, 32));
        map.Add(BUTTON_SHIFT, FColor(32, 255, 32));
        
        map.Add(BUTTON_HIGH, FColor(255, 255, 32));
        map.Add(BUTTON_PARK, FColor(32, 255, 32));

        map.Add(BUTTON_SK1, FColor(32, 255, 32));
        map.Add(BUTTON_SK2, FColor(32, 255, 32));
        map.Add(BUTTON_SK3, FColor(32, 255, 32));
        map.Add(BUTTON_SK4, FColor(32, 255, 32));
        map.Add(BUTTON_SK5, FColor(32, 255, 32));
        map.Add(BUTTON_SK6, FColor(32, 255, 32));
        map.Add(BUTTON_MORE_SK, FColor(32, 255, 32));
        
        map.Add(BUTTON_GO, FColor(32, 255, 32));
        map.Add(BUTTON_BACK, FColor(255, 32, 32));

        return map;
    }

    static bool isNumeric(FName button)
    {
        return button == BUTTON_0 || button == BUTTON_1 || button == BUTTON_2 || button == BUTTON_3 || button == BUTTON_4 || button == BUTTON_5 || button == BUTTON_6 || button == BUTTON_7 || button == BUTTON_8 || button == BUTTON_9 || button == BUTTON_DOT || button == BUTTON_SLASH;
    }

protected:
    bool lastActive = false;

    FName ID;

    FColor buttonColor;
    FColor activeColor;
    float intensity = 0.5f;
    bool active = false;

    UTextRenderComponent *textRenderer = nullptr;
    UStaticMeshComponent *mesh = nullptr;
    int buttonMaterialIndex = 0;

    UMaterialInstanceDynamic* buttonMaterial = nullptr;
    UMaterialInstanceDynamic* indicatorMaterial = nullptr;
};