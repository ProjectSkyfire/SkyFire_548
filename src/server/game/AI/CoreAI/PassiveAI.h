/*
 * Copyright (C) 2011-2022 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2022 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2022 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SKYFIRE_PASSIVEAI_H
#define SKYFIRE_PASSIVEAI_H

#include "CreatureAI.h"

class PassiveAI : public CreatureAI
{
    public:
        explicit PassiveAI(Creature* c);

        void MoveInLineOfSight(Unit*) OVERRIDE { }
        void AttackStart(Unit*) OVERRIDE { }
        void UpdateAI(uint32) OVERRIDE;

        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }
};

class PossessedAI : public CreatureAI
{
    public:
        explicit PossessedAI(Creature* c);

        void MoveInLineOfSight(Unit*) OVERRIDE { }
        void AttackStart(Unit* target) OVERRIDE;
        void UpdateAI(uint32) OVERRIDE;
        void EnterEvadeMode() OVERRIDE { }

        void JustDied(Unit*) OVERRIDE;
        void KilledUnit(Unit* victim) OVERRIDE;

        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }
};

class NullCreatureAI : public CreatureAI
{
    public:
        explicit NullCreatureAI(Creature* c);

        void MoveInLineOfSight(Unit*) OVERRIDE { }
        void AttackStart(Unit*) OVERRIDE { }
        void UpdateAI(uint32) OVERRIDE { }
        void EnterEvadeMode() OVERRIDE { }
        void OnCharmed(bool /*apply*/) OVERRIDE { }

        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }
};

class CritterAI : public PassiveAI
{
    public:
        explicit CritterAI(Creature* c) : PassiveAI(c) { }

        void DamageTaken(Unit* done_by, uint32& /*damage*/) OVERRIDE;
        void EnterEvadeMode() OVERRIDE;
};

class TriggerAI : public NullCreatureAI
{
    public:
        explicit TriggerAI(Creature* c) : NullCreatureAI(c) { }
        void IsSummonedBy(Unit* summoner) OVERRIDE;
};

#endif
