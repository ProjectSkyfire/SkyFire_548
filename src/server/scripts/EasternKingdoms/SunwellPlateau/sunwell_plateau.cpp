/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Sunwell_Plateau
SD%Complete: 0
SDComment: Placeholder, Epilogue after Kil'jaeden, Captain Selana Gossips
EndScriptData */

/* ContentData
npc_prophet_velen
npc_captain_selana
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "sunwell_plateau.h"

/*######
## npc_prophet_velen
######*/

enum ProphetSpeeches
{
    PROPHET_SAY1 = -1580099,
    PROPHET_SAY2 = -1580100,
    PROPHET_SAY3 = -1580101,
    PROPHET_SAY4 = -1580102,
    PROPHET_SAY5 = -1580103,
    PROPHET_SAY6 = -1580104,
    PROPHET_SAY7 = -1580105,
    PROPHET_SAY8 = -1580106
};

enum LiadrinnSpeeches
{
    LIADRIN_SAY1 = -1580107,
    LIADRIN_SAY2 = -1580108,
    LIADRIN_SAY3 = -1580109
};

/*######
## npc_captain_selana
######*/

#define CS_GOSSIP1 "Give me a situation report, Captain."
#define CS_GOSSIP2 "What went wrong?"
#define CS_GOSSIP3 "Why did they stop?"
#define CS_GOSSIP4 "Your insight is appreciated."

void AddSC_sunwell_plateau() { }
