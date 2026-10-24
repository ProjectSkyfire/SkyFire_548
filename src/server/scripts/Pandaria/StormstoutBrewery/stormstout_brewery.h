/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_STORMSTOUT_BREWERY_H_
#define SF_STORMSTOUT_BREWERY_H_

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "InstanceScript.h"

#define StormstoutBreweryScriptName "instance_stormstout_brewery"

uint32 const EncounterCount = 3;
uint32 const HozenToSlainForOokOok = 40;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_OOK_OOK        = 0,
    DATA_HOPTALLUS      = 1,
    DATA_YAN_ZHU        = 2,

    DATA_HOZEN_SLAIN    = 3,
    DATA_OOK_DOOR       = 4,
    DATA_UNCLE_GAO      = 5,
    DATA_CHEN_YANZHU    = 6,
};

enum CreatureIds
{
    NPC_OOK_OOK                  = 56637,
    NPC_HOPTALLUS                = 56717,
    NPC_YAN_ZHU                  = 59479,
    NPC_UNCLE_GAO                = 59074,
    NPC_CHEN_YANZHU              = 64361,
    NPC_CHEN_STORMSTOUT          = 59704,
    NPC_AUNTIE_STORMSTOUT        = 59822,
    NPC_NIBBLER                  = 65392,
    NPC_EXPLOSIVE_HOPPER         = 56718,
    NPC_HOZEN_CLINGER            = 60276,
    NPC_HOZEN_BOUNCER            = 56849,

    // Yan-Zhu adds
    NPC_BLOATED_ALEMENTAL        = 59518,
    NPC_STOUT_ALEMENTAL          = 59519,
    NPC_FIZZY_ALEMENTAL          = 59520,
    NPC_BUBBLING_ALEMENTAL       = 59521,
    NPC_YEASTY_ALEMENTAL         = 59494,
    NPC_SUDSY_ALEMENTAL          = 59522,

    NPC_WALL_OF_SUDS             = 59512,
    NPC_FIZZY_BUBBLE             = 59799,

    NPC_HOZEN_PARTY_ANIMAL       = 59684,
    NPC_HOZEN_PARTY_ANIMAL2      = 57097,
    NPC_HOZEN_PARTY_ANIMAL3      = 56927,
    NPC_DRUNKEN_HOZEN_BRAWLER    = 56862,
    NPC_SLEEPY_HOZEN_BRAWLER     = 56863,
    NPC_INFLAMED_HOZEN_BRAWLER   = 56924,
    NPC_SODDEN_HOZEN_BRAWLER     = 59605,

    NPC_PURPOSE_BUNNY_FLYING     = 54020,
    NPC_PURPOSE_BUNNY_GROUND     = 59394,
    NPC_BARREL_TOSS_BUNNY        = 45979,
};

enum GameObjectIds
{
    GO_INVIS_DOOR                = 211126,
    GO_OOK_DOOR                  = 211127,
    GO_SHAKING_KEG               = 211138,
    GO_OOK_EXIT_DOOR             = 211132,
    GO_HOPTALLUS_ENTRANCE        = 211133,
    GO_YANZHU_ENTRANCE           = 211136,
};

enum InstanceSpells
{
    SPELL_BLOATING_BREW          = 114929,
    SPELL_BLACKOUT_BREW          = 114930,
    SPELL_BUBBLING_BREW          = 114931,
    SPELL_YEASTY_BREW            = 114932,
    SPELL_SUDSY_BREW             = 114933,
    SPELL_FIZZY_BREW             = 114934,

    SPELL_GUSHING_BREW_BUNNY     = 114379,
    SPELL_BANANA_BAR             = 107297,
    SPELL_SPICY_EXPLOSION        = 107205,

    // Hozen party
    SPELL_HOZEN_PIRATE_HAT       = 115762,
    SPELL_HOZEN_HAT              = 118916,
    SPELL_HOZU_HAT_1             = 106655,
    SPELL_HOZU_HAT_2             = 127852,
    SPELL_HOZU_HAT_3             = 127853,
    SPELL_HOZU_HAT_4             = 127854,

    SPELL_FIREWORK               = 124556,
    SPELL_FIREWORK_2             = 126169,
    SPELL_FIREWORK_3             = 128313,
    SPELL_FIREWORK_4             = 128314,
    SPELL_FIREWORK_5             = 128316,
    SPELL_FIREWORK_6             = 128317,
    SPELL_FIREWORK_7             = 128318,
};

uint32 const HozenHats[6] =
{
    SPELL_HOZEN_PIRATE_HAT, SPELL_HOZEN_HAT, SPELL_HOZU_HAT_1, SPELL_HOZU_HAT_2, SPELL_HOZU_HAT_3, SPELL_HOZU_HAT_4
};

uint32 const HozenFireworks[7] =
{
    SPELL_FIREWORK, SPELL_FIREWORK_2, SPELL_FIREWORK_3, SPELL_FIREWORK_4, SPELL_FIREWORK_5, SPELL_FIREWORK_6, SPELL_FIREWORK_7
};

// Alementals Uncle Gao brews (small, medium, large waves) and the brew aura each one gives Yan-Zhu.
// The alternatives (Bubbling, Sudsy, Stout) need targeting the core doesn't support yet.
struct YanZhuBrew
{
    uint32 entry;
    uint32 spell;
};

YanZhuBrew const YanZhuBrews[3] =
{
    { NPC_YEASTY_ALEMENTAL,  SPELL_YEASTY_BREW   },
    { NPC_FIZZY_ALEMENTAL,   SPELL_FIZZY_BREW    },
    { NPC_BLOATED_ALEMENTAL, SPELL_BLOATING_BREW },
};

Position const BubblingAlementalPath[4] =
{
    { -701.38f, 1176.61f, 139.64f, 6.21f },
    { -689.12f, 1161.46f, 139.64f, 5.39f },
    { -706.69f, 1152.15f, 139.64f, 2.49f },
    { -712.64f, 1169.57f, 139.64f, 1.77f }
};

Position const HoplingPath[11] =
{
    { -704.30f, 1262.68f, 162.78f, 1.42f },
    { -702.49f, 1283.33f, 163.23f, 1.84f },
    { -712.32f, 1315.72f, 162.63f, 1.86f },
    { -771.25f, 1303.55f, 162.78f, 3.42f },
    { -766.27f, 1267.88f, 163.23f, 4.87f },
    { -762.14f, 1247.95f, 162.75f, 4.91f },
    { -773.15f, 1244.15f, 162.79f, 3.39f },
    { -787.02f, 1240.43f, 154.75f, 3.41f },
    { -800.84f, 1246.85f, 154.76f, 2.47f },
    { -804.38f, 1261.61f, 146.71f, 1.80f },
    { -810.98f, 1274.13f, 146.66f, 2.14f }
};

template<class AI>
AI* GetStormstoutBreweryAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, StormstoutBreweryScriptName);
}

#endif // SF_STORMSTOUT_BREWERY_H_
