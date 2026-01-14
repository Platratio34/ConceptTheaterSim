// Copyright Peter Crall 2025

#include "EOS/EOSPropertyType.h"
#include "Kismet/KismetMathLibrary.h"

UEOSPropertyIndex *UEOSPropertyIndex::Create(FName name, double min, double max)
{
    UEOSPropertyIndex* index = NewObject<UEOSPropertyIndex>();
    index->name = name;
    index->min = min;
    index->max = max;
    return index;
}

UEOSPropertyIndex *UEOSPropertyIndex::Create(FName name, double min, double max, bool variable)
{
    UEOSPropertyIndex* index = NewObject<UEOSPropertyIndex>();
    index->name = name;
    index->min = min;
    index->max = max;
    index->variable = variable;
    return index;
}

UEOSPropertyType *UEOSPropertyType::Create(FName name)
{
    UEOSPropertyType *type = NewObject<UEOSPropertyType>();
    type->property = name;

    return type;
}

UEOSPropertyType *UEOSPropertyType::Create(FName name, double def)
{
    UEOSPropertyType *type = NewObject<UEOSPropertyType>();
    type->property = name;
    type->def = def;
    return type;
}

UEOSPropertyType *UEOSPropertyType::Create(FName name, double def, double min, double max, double step)
{
    UEOSPropertyType *type = NewObject<UEOSPropertyType>();
    type->property = name;
    type->min = min;
    type->max = max;
    type->def = def;
    type->encoderStep = step;
    return type;
}

UEOSPropertyType *UEOSPropertyType::Create(FName name, TArray<UEOSPropertyIndex*> indexes)
{
    UEOSPropertyType *type = NewObject<UEOSPropertyType>();
    type->property = name;
    type->indexed = true;
    type->indexes = indexes;
    type->min = 9e9;
    for(int i = 0; i < indexes.Num(); i++)
    {
        UEOSPropertyIndex* index = indexes[i];
        if(index->min < type->min)
            type->min = index->min;
        if(index->max > type->max)
            type->max = index->max;
    }
    return type;
}

bool UEOSPropertyType::isValid(double value)
{
    if(indexed)
    {
        int index = FMath::RoundToInt(value);
        return 0 <= index && index < indexes.Num();
    }
    else
    {
        return min <= value && value <= max;
    }
}

FName UEOSPropertyType::getIndex(double value)
{
    if(!indexed)
        return FName("None");
    for(int i = 0; i < indexes.Num(); i++)
    {
        UEOSPropertyIndex *v = indexes[i];
        if(v->min <= value && value <= v->max)
            return v->name;
    }
    return FName("None");
}

double UEOSPropertyType::getValue(FName index)
{
    if(!indexed)
        return -1;
    for(int i = 0; i < indexes.Num(); i++)
    {
        UEOSPropertyIndex *v = indexes[i];
        if(v->name == index)
            return v->min;
    }
    return -1;
}

double UEOSPropertyType::nextIndex(double value)
{
    if(!indexed)
        return 0;
    for(int i = 0; i < indexes.Num(); i++)
    {
        UEOSPropertyIndex *v = indexes[i];
        if(v->min <= value && value <= v->max)
        {
            if(i == indexes.Num()-1)
                return indexes[0]->min;
            return indexes[i+1]->min;
        }
    }
    return indexes[0]->min;
}

double UEOSPropertyType::lastIndex(double value)
{
    if(!indexed)
        return 0;
    for(int i = 0; i < indexes.Num(); i++)
    {
        UEOSPropertyIndex *v = indexes[i];
        if(v->min <= value && value <= v->max)
        {
            if(i == 0)
                return indexes[indexes.Num()-1]->max;
            return indexes[i-1]->max;
        }
    }
    return indexes[0]->max;
}

double UEOSPropertyType::indexStep(double value)
{
    if(!indexed)
        return 0;
    for(int i = 0; i < indexes.Num(); i++)
    {
        UEOSPropertyIndex *v = indexes[i];
        if(v->min <= value && value <= v->max)
        {
            return v->variable ? 1 : 0;
        }
    }
    return 0;
}