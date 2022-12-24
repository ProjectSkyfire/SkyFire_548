/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef DEF_OCULUS_H
#define DEF_OCULUS_H

enum Data
{
    DATA_DRAKOS_EVENT,
    DATA_VAROS_EVENT,
    DATA_UROM_EVENT,
    DATA_EREGOS_EVENT,
    DATA_UROM_PLATAFORM
};

enum Data64
{
    DATA_DRAKOS,
    DATA_VAROS,
    DATA_UROM,
    DATA_EREGOS
};

enum Bosses_NPCs
{
    NPC_DRAKOS                  = 27654,
    NPC_VAROS                   = 27447,
    NPC_UROM                    = 27655,
    NPC_EREGOS                  = 27656,

    NPC_AZURE_RING_GUARDIAN         = 28236,
    NPC_CENTRIFUGE_CONSTRUCT        = 27641,
    NPC_RUBY_DRAKE_VEHICLE          = 27756,
    NPC_EMERALD_DRAKE_VEHICLE       = 27692,
    NPC_AMBER_DRAKE_VEHICLE         = 27755,
    NPC_VERDISA                     = 27657,
    NPC_BELGARISTRASZ               = 27658,
    NPC_ETERNOS                     = 27659,
    NPC_GREATER_WHELP               = 28276
};

enum GameObjects
{
    GO_DRAGON_CAGE_DOOR                           = 193995,
    GO_EREGOS_CACHE_N                             = 191349,
    GO_EREGOS_CACHE_H                             = 193603
};

enum SpellEvents
{
    EVENT_CALL_DRAGON = 12229
};

enum CreatureActions
{
    ACTION_CALL_DRAGON_EVENT = 1
};

enum OculusWorldStates
{
    WORLD_STATE_CENTRIFUGE_CONSTRUCT_SHOW   = 3524,
    WORLD_STATE_CENTRIFUGE_CONSTRUCT_AMOUNT = 3486
};

enum OculusSpells
{
    SPELL_CENTRIFUGE_SHIELD = 50053,
    SPELL_DEATH_SPELL       = 50415
};
#endif
