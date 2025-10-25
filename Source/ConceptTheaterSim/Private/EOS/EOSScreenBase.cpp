// Copyright Peter Crall 2025


#include "EOS/EOSScreenBase.h"

FString UEOSScreenBase::getCommandString()
{
    TMap<FName, FString> ntd = getNamesToDisplay();

    FString str = TEXT("");
    if(board != nullptr)
    {
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

        if(board->highlightMode)
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
        if(board->clearCmd)
        {
            str += TEXT(" *");
        }
    } else 
    {
        str = TEXT("DISCONNECTED");
    }
    return str;
}

FLinearColor UEOSScreenBase::getCommandColor()
{
    if(board != nullptr)
    {
        if(board->confirmCmd || board->commandError.Len() > 0)
        {
            return FLinearColor(1.0, 0.25, 0.25, 1.0);
        }
        else if(board->clearCmd)
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
    if(board == nullptr)
    {
        TArray<int> o;
        return o;
    }
    return board->showfile->getPatchedChannels();
}

double UEOSScreenBase::getChannelParameter(int ch, FName parameter)
{
    if(board == nullptr)
    {
        return 0;
    }
    return board->showfile->getParameter(ch, parameter);
}

TArray<FName> UEOSScreenBase::getChannelParameters(int ch)
{
    TArray<FName> params;
    if(board == nullptr)
        return params;
    if(!board->showfile->channels.Contains(ch))
        return params;
    board->showfile->channels[ch].properties.GetKeys(params);
    return params;
}