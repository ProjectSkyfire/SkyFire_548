/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef ADT_H
#define ADT_H

#include "loadlib.h"

#define TILESIZE (533.33333f)
#define CHUNKSIZE ((TILESIZE) / 16.0f)
#define UNITSIZE (CHUNKSIZE / 8.0f)

enum LiquidType
{
    LIQUID_TYPE_WATER = 0,
    LIQUID_TYPE_OCEAN = 1,
    LIQUID_TYPE_MAGMA = 2,
    LIQUID_TYPE_SLIME = 3
};

//**************************************************************************************
// ADT file class
//**************************************************************************************
#define ADT_CELLS_PER_GRID    16
#define ADT_CELL_SIZE         8
#define ADT_GRID_SIZE         (ADT_CELLS_PER_GRID*ADT_CELL_SIZE)

#pragma pack(push, 1)

//
// Adt file height map chunk
//
class adt_MCVT
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
public:
    float height_map[(ADT_CELL_SIZE+1)*(ADT_CELL_SIZE+1)+ADT_CELL_SIZE*ADT_CELL_SIZE];
};

//
// Adt file liquid map chunk (old)
//
class adt_MCLQ
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
public:
    float height1;
    float height2;
    struct liquid_data{
        uint32 light;
        float  height;
    } liquid[ADT_CELL_SIZE+1][ADT_CELL_SIZE+1];

    // 1<<0 - ochen
    // 1<<1 - lava/slime
    // 1<<2 - water
    // 1<<6 - all water
    // 1<<7 - dark water
    // == 0x0F - not show liquid
    uint8 flags[ADT_CELL_SIZE][ADT_CELL_SIZE];
    uint8 data[84];
};

//
// Adt file cell chunk
//
class adt_MCNK
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
public:
    uint32 flags;
    uint32 ix;
    uint32 iy;
    uint32 nLayers;
    uint32 nDoodadRefs;
    union
    {
        struct
        {
            uint32 offsMCVT;        // height map
            uint32 offsMCNR;        // Normal vectors for each vertex
        } offsets;
        uint64 HighResHoles;        // TODO: maybe use this?
    } union_5_3_0;
    uint32 offsMCLY;        // Texture layer definitions
    uint32 offsMCRF;        // A list of indices into the parent file's MDDF chunk
    uint32 offsMCAL;        // Alpha maps for additional texture layers
    uint32 sizeMCAL;
    uint32 offsMCSH;        // Shadow map for static shadows on the terrain
    uint32 sizeMCSH;
    uint32 areaid;
    uint32 nMapObjRefs;
    uint32 holes;
    uint16 s[2];
    uint32 data1;
    uint32 data2;
    uint32 data3;
    uint32 predTex;
    uint32 nEffectDoodad;
    uint32 offsMCSE;
    uint32 nSndEmitters;
    uint32 offsMCLQ;         // Liqid level (old)
    uint32 sizeMCLQ;         //
    float  zpos;
    float  xpos;
    float  ypos;
    uint32 offsMCCV;         // offsColorValues in WotLK
    uint32 props;
    uint32 effectId;
};

#define ADT_LIQUID_HEADER_FULL_LIGHT   0x01
#define ADT_LIQUID_HEADER_NO_HIGHT     0x02

struct adt_liquid_header{
    uint16 liquidType;             // Index from LiquidType.dbc
    uint16 formatFlags;
    float  heightLevel1;
    float  heightLevel2;
    uint8  xOffset;
    uint8  yOffset;
    uint8  width;
    uint8  height;
    uint32 offsData2a;
    uint32 offsData2b;
};

//
// Adt file liquid data chunk (new)
//
class adt_MH2O
{
public:
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;

    struct adt_LIQUID{
        uint32 offsData1;
        uint32 used;
        uint32 offsData2;
    } liquid[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];

    adt_liquid_header *getLiquidData(int x, int y)
    {
        if (liquid[x][y].used && liquid[x][y].offsData1)
            return (adt_liquid_header *)((uint8*)this + 8 + liquid[x][y].offsData1);
        return 0;
    }

    float *getLiquidHeightMap(adt_liquid_header *h)
    {
        if (h->formatFlags & ADT_LIQUID_HEADER_NO_HIGHT)
            return 0;
        if (h->offsData2b)
            return (float *)((uint8*)this + 8 + h->offsData2b);
        return 0;
    }

    uint8 *getLiquidLightMap(adt_liquid_header *h)
    {
        if (h->formatFlags&ADT_LIQUID_HEADER_FULL_LIGHT)
            return 0;
        if (h->offsData2b)
        {
            if (h->formatFlags & ADT_LIQUID_HEADER_NO_HIGHT)
                return (uint8 *)((uint8*)this + 8 + h->offsData2b);
            return (uint8 *)((uint8*)this + 8 + h->offsData2b + (h->width+1)*(h->height+1)*4);
        }
        return 0;
    }

    uint32 *getLiquidFullLightMap(adt_liquid_header *h)
    {
        if (!(h->formatFlags&ADT_LIQUID_HEADER_FULL_LIGHT))
            return 0;
        if (h->offsData2b)
        {
            if (h->formatFlags & ADT_LIQUID_HEADER_NO_HIGHT)
                return (uint32 *)((uint8*)this + 8 + h->offsData2b);
            return (uint32 *)((uint8*)this + 8 + h->offsData2b + (h->width+1)*(h->height+1)*4);
        }
        return 0;
    }

    uint64 getLiquidShowMap(adt_liquid_header *h)
    {
        if (h->offsData2a)
            return *((uint64 *)((uint8*)this + 8 + h->offsData2a));
        else
            return 0xFFFFFFFFFFFFFFFFuLL;
    }
};

#pragma pack(pop)

#endif
