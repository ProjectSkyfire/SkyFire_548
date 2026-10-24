/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef RAGEFIRE_CHASM_N
#define RAGEFIRE_CHASM_N

const Position SlagMawPoints[3]
{
    { -256.643f, 172.957f, -16.253f, 5.383f },
    { -261.183f, 137.642f, -12.348f, 0.574f },
    { -225.866f, 164.639f, -15.437f, 3.885f }
};

enum eBosses
{
    BOSS_ADAROGG,
    BOSS_DARK_SHAMAN_CORANTHAL,
    BOSS_SLAGMAW,
    BOSS_GORDOTH,
    TOTAL_ENCOUNTERS,
};

enum eCreatures
{
    NPC_ADAROGG = 61408,
    NPC_DARK_SHAMAN_CORANTHAL = 61412,
    NPC_SLAGMAW = 61463,
    NPC_GORDOTH = 61528,

    // Horde camp at the entrance
    NPC_BOVAAL_WHITEHORN = 44216,
    NPC_STONE_GUARD_KURJACK = 44217,
    NPC_ORGRIMMAR_DEFENDER = 44223,
    NPC_COMMANDER_BAGRAN = 61724,
};

enum eType
{
    TYPE_TARGET_ID = 1,
};

#endif // RAGEFIRE_CHASM_N
