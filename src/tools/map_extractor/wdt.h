/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef WDT_H
#define WDT_H
#include "loadlib.h"

//**************************************************************************************
// WDT file class and structures
//**************************************************************************************
#define WDT_MAP_SIZE 64

#pragma pack(push, 1)

class wdt_MAIN
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
public:
    uint32 size;

    struct adtData{
        uint32 flag;
        uint32 data1;
    } adt_list[64][64];
};

#pragma pack(pop)

#endif