/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef DEF_STRATHOLME_H
#define DEF_STRATHOLME_H

enum DataTypes
{
    TYPE_BARON_RUN                      = 1,
    TYPE_BARONESS                       = 2,
    TYPE_NERUB                          = 3,
    TYPE_PALLID                         = 4,
    TYPE_RAMSTEIN                       = 5,
    TYPE_BARON                          = 6,

    TYPE_MALOWN                         = 7,

    DATA_BARON                          = 10,
    DATA_YSIDA_TRIGGER                  = 11,

    TYPE_SH_QUEST                       = 20,
    TYPE_SH_CATHELA                     = 21,
    TYPE_SH_GREGOR                      = 22,
    TYPE_SH_NEMAS                       = 23,
    TYPE_SH_VICAR                       = 24,
    TYPE_SH_AELMAR                      = 25
};

enum CreatureIds
{
    NPC_CRYSTAL                         = 10415, // ziggurat crystal
    NPC_BARON                           = 10440, // ziggurat crystal
    NPC_YSIDA_TRIGGER                   = 16100, // ziggurat crystal

    NPC_RAMSTEIN                        = 10439,
    NPC_ABOM_BILE                       = 10416,
    NPC_ABOM_VENOM                      = 10417,
    NPC_BLACK_GUARD                     = 10394,
    NPC_YSIDA                           = 16031,
};

enum GameobjectIds
{
    GO_DOOR_HALAZZI                     = 186303,
    GO_SERVICE_ENTRANCE                 = 175368,
    GO_GAUNTLET_GATE1                   = 175357,
    GO_ZIGGURAT1                        = 175380,  // baroness
    GO_ZIGGURAT2                        = 175379,  // nerub'enkan
    GO_ZIGGURAT3                        = 175381,  // maleki
    GO_ZIGGURAT4                        = 175405,  // rammstein
    GO_ZIGGURAT5                        = 175796,  // baron
    GO_PORT_GAUNTLET                    = 175374,  // port from gauntlet to slaugther
    GO_PORT_SLAUGTHER                   = 175373,  // port at slaugther
    GO_PORT_ELDERS                      = 175377   // port at elders square
};

enum QuestIds
{
    QUEST_DEAD_MAN_PLEA                 = 8945
};

enum SpellIds
{
    SPELL_BARON_ULTIMATUM               = 27861
};

#endif

