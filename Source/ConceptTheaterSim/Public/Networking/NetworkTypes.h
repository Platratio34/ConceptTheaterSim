// Copyright Peter Crall 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkTypes.generated.h"

UCLASS(BlueprintType)
class CONCEPTTHEATERSIM_API UNetworkPacket : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    int dest = -1;

    UPROPERTY(BlueprintReadOnly)
    int source = 0;

    UPROPERTY(BlueprintReadOnly)
    int destPort = 0;

    UPROPERTY(BlueprintReadOnly)
    int sourcePort = 0;

    UPROPERTY(BlueprintReadWrite)
    FName type;

    UPROPERTY(BlueprintReadWrite)
    UObject *data = nullptr;

    static UNetworkPacket* createPacket(int dest, FName type, int destPort = 0, int sourcePort = 0)
    {
        UNetworkPacket* packet = NewObject<UNetworkPacket>();
        packet->dest = dest;
        packet->type = type;
        packet->destPort = destPort;
        packet->sourcePort = sourcePort;
        return packet;
    }

    static UNetworkPacket* createDataPacket(int dest, FName type, UObject* data, int destPort = 0, int sourcePort = 0)
    {
        UNetworkPacket* packet = NewObject<UNetworkPacket>();
        packet->dest = dest;
        packet->type = type;
        packet->data = data;
        packet->destPort = destPort;
        packet->sourcePort = sourcePort;
        return packet;
    }
};

UDELEGATE(BlueprintCallable, BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNetworkPacket, UNetworkPacket*, packet);


class CONCEPTTHEATERSIM_API ACNetwork;

class CONCEPTTHEATERSIM_API UNetworkCard;

class CONCEPTTHEATERSIM_API UNetworkSocket;

USTRUCT(BlueprintType)
struct CONCEPTTHEATERSIM_API FIPAddress
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    int address;

    static FString toString(FIPAddress address)
    {
        return ipToString(address.address);
    };
    
    static FString ipToString(int address)
    {
        int o1 = (address >> 24) & 0xff;
        int o2 = (address >> 16) & 0xff;
        int o3 = (address >> 8) & 0xff;
        int o4 = address & 0xff;
        return FString::Printf(TEXT("%d.%d.%d.%d"), o1, o2, o3, o4);
    };

    static FIPAddress fromString(FString str)
    {
        FIPAddress address;
        address.address = ipFromString(str);
        return address;
    };

    static int ipFromString(FString str)
    {
        int address = 0;
        if(str.Find(TEXT("0x")) == 0) // hex
        {
            for (int i = 2; i < str.Len(); i++)
            {
                TCHAR c = str[i];
                switch (c)
                {
                case '0':
                    address = address << 4;
                    break;
                case '1':
                    address = (address << 4) + 0x1;
                    break;
                case '2':
                    address = (address << 4) + 0x2;
                    break;
                case '3':
                    address = (address << 4) + 0x3;
                    break;
                case '4':
                    address = (address << 4) + 0x4;
                    break;
                case '5':
                    address = (address << 4) + 0x5;
                    break;
                case '6':
                    address = (address << 4) + 0x6;
                    break;
                case '7':
                    address = (address << 4) + 0x7;
                    break;
                case '8':
                    address = (address << 4) + 0x8;
                    break;
                case '9':
                    address = (address << 4) + 0x9;
                    break;
                case 'a':
                    address = (address << 4) + 0xa;
                    break;
                case 'b':
                    address = (address << 4) + 0xb;
                    break;
                case 'c':
                    address = (address << 4) + 0xc;
                    break;
                case 'd':
                    address = (address << 4) + 0xd;
                    break;
                case 'e':
                    address = (address << 4) + 0xe;
                    break;
                case 'f':
                    address = (address << 4) + 0xf;
                    break;
                
                default:
                    break;
                }
            }
            return address;
        }
        TArray<FString> parts;
        str.ParseIntoArray(parts, TEXT("."), true);
        for (int i = 0; i < parts.Num(); i++)
        {
            address = (address << 8) + FCString::Atoi(*(parts[i]));
        }
        return address;
    };
};