// Copyright Peter Crall 2025


#include "EOS/EOSScreenBase.h"

FString UEOSScreenBase::getCommandString()
{
    FString str = TEXT("");
    if(board != nullptr)
    {
        if(board->mode == LIVE)
        {
            str += TEXT("LIVE");
        }
        else if(board->mode == BLIND)
        {
            str += TEXT("BLIND");
        }
        else if(board->mode == STAGE)
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
                
            str += n.ToString();
        }
        if(board->commandError.Len() > 0)
        {
            str += TEXT(" ") + board->commandError;
        }
        if(board->confirmCmd)
        {
            str += TEXT(" Confirm");
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
        else if(board->mode == BLIND)
        {
            return FLinearColor(0.25, 1.0, 1.0, 1.0);
        }
    }
    return FLinearColor(1.0, 1.0, 0.25, 1.0);
}