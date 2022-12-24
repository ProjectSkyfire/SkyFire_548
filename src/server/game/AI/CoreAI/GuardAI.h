/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_GUARDAI_H
#define SKYFIRE_GUARDAI_H

#include "ScriptedCreature.h"

class Creature;

class GuardAI : public ScriptedAI
{
    public:
        explicit GuardAI(Creature* creature);

        static int Permissible(Creature const* creature);
        bool CanSeeAlways(WorldObject const* obj) OVERRIDE;

        void EnterEvadeMode() OVERRIDE;
        void JustDied(Unit* killer) OVERRIDE;
};
#endif
