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
        bool CanSeeAlways(WorldObject const* obj) override;

        void EnterEvadeMode() override;
        void JustDied(Unit* killer) override;
};
#endif
