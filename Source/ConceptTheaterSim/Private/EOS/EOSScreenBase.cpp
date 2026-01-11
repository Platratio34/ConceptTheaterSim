// Copyright Peter Crall 2025


#include "EOS/EOSScreenBase.h"

FString UEOSScreenBase::getCommandString()
{
    TMap<FName, FString> ntd = getNamesToDisplay();

    FString str = TEXT("");
    if(board == nullptr || board->showfile == nullptr)
    {
        return TEXT("DISCONNECTED");
    }
    if(board->mode == EEOSMode::LIVE)
    {
        str += TEXT("LIVE");
    }
    else if(board->mode == EEOSMode::BLIND)
    {
        str += TEXT("BLIND");
    }
    else if(board->mode == EEOSMode::STAGE)
    {
        str += TEXT("STAGING");
    }

    if (board->showfile != nullptr)
    {
        str += TEXT(" Cue ") + board->showfile->cues[board->showfile->currentCue]->cueNumber;
    }

    if (board->highlightMode)
    {
        str += TEXT(" Highlight");
    }

    str += ": ";
    TArray<FName> command = board->command;
    for (int i = 0; i < command.Num(); i++)
    {
        FName n = command[i];
        if(i > 0)
        {
            if(!(UEOSButton::isNumeric(n) && UEOSButton::isNumeric(command[i-1])))
                str += TEXT(" ");
        }
        if(FString* s2 = ntd.Find(n))
        {
            str += *s2;
        }
        else
        {
            str += n.ToString();
        }
    }
    if(board->commandError.Len() > 0)
    {
        str += TEXT(" ") + board->commandError;
    }
    if(board->confirmCmd)
    {
        str += TEXT(" Confirm");
    }
    if(board->cmdComplete)
    {
        str += TEXT(" *");
    }
    return str;
}

FLinearColor UEOSScreenBase::getCommandColor()
{
    if(board != nullptr && board->showfile != nullptr)
    {
        if(board->confirmCmd || board->commandError.Len() > 0)
        {
            return FLinearColor(1.0, 0.25, 0.25, 1.0);
        }
        else if(board->cmdComplete)
        {
            return FLinearColor(0.25, 1.0, 0.25, 1.0);
        }
        else if(board->mode == EEOSMode::BLIND)
        {
            return FLinearColor(0.25, 1.0, 1.0, 1.0);
        }
    }
    return FLinearColor(1.0, 1.0, 0.25, 1.0);
}

TArray<int> UEOSScreenBase::getPatchedChannels()
{
    if(board == nullptr || board->showfile == nullptr)
    {
        TArray<int> o;
        return o;
    }
    return board->showfile->getPatchedChannels();
}

double UEOSScreenBase::getChannelParameter(int ch, FName parameter)
{
    if(board == nullptr || board->showfile == nullptr)
    {
        return 0;
    }
    return board->showfile->getParameter(ch, parameter);
}

TArray<FName> UEOSScreenBase::getChannelParameters(int ch)
{
    TArray<FName> params;
    if(board == nullptr || board->showfile == nullptr)
        return params;
    UEOSChannelView *channel = board->showfile->getChannel(ch);
    if(channel == nullptr)
        return params;
    channel->getKeys(params);
    return params;
}

UEOSChannelView* UEOSScreenBase::getChannelView(int ch)
{
    if(board == nullptr || board->showfile == nullptr)
        return nullptr;
    return board->showfile->getChannel(ch);
}

FString UEOSScreenBase::getCurrentCue()
{
    if(board == nullptr || board->showfile == nullptr)
        return TEXT("");
    return board->showfile->cues[board->showfile->currentCue]->cueNumber;
}