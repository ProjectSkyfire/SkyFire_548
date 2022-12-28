/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "Field.h"

Field::Field()
{
    data.value = NULL;
    data.type = MYSQL_TYPE_NULL;
    data.length = 0;
    data.raw = false;
}

Field::~Field()
{
    CleanUp();
}

void Field::SetByteValue(const void* newValue, const size_t newSize, enum_field_types newType, uint32 length)
{
    if (data.value)
        CleanUp();

    // This value stores raw bytes that have to be explicitly casted later
    if (newValue)
    {
        data.value = new char[newSize];
        memcpy(data.value, newValue, newSize);
        data.length = length;
    }
    data.type = newType;
    data.raw = true;
}

void Field::SetStructuredValue(char* newValue, enum_field_types newType)
{
    if (data.value)
        CleanUp();

    // This value stores somewhat structured data that needs function style casting
    if (newValue)
    {
        size_t size = strlen(newValue);
        data.value = new char [size+1];
        strcpy((char*)data.value, newValue);
        data.length = size;
    }

    data.type = newType;
    data.raw = false;
}
