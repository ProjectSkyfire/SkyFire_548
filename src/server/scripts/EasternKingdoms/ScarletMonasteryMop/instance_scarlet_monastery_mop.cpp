/*
 * Ported from Legends of Azeroth 5.4.8.
 * Authors: Jason Dove, Kulyadev, lee, leelf00
 * See LICENSE.md for copyright information.
 */
/*
 * This file is part of the Pandaria 5.4.8 Project. See THANKS file for copyright information.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
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

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "GameObject.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "Player.h"
#include "scarlet_monastery_mop.h"

static DoorData const doorData[] =
{
    { GO_THALNOS_EXIT, BOSS_THALNOS_THE_SOULRENDER, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_KORLOFF_EXIT, BOSS_BROTHER_KORLOFF, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_WHITEMANE_ENTRANCE, BOSS_WHITEMANE, DOOR_TYPE_SPAWN_HOLE, BOUNDARY_NONE },
    { 0, 0, DOOR_TYPE_ROOM, BOUNDARY_NONE }
};

class instance_scarlet_monastery_mop : public InstanceMapScript
{
public:
    instance_scarlet_monastery_mop() : InstanceMapScript("instance_scarlet_monastery_mop", 1004) { }

    struct instance_scarlet_monastery_mop_InstanceMapScript : public InstanceScript
    {
        instance_scarlet_monastery_mop_InstanceMapScript(Map* map) : InstanceScript(map), hoodedGuid(0), whitemaneDoor(0)
        {
            for (uint8 i = 0; i < ENCOUNTER_COUNT; ++i)
                bossGuids[i] = 0;
        }

        uint64 bossGuids[ENCOUNTER_COUNT];
        uint64 hoodedGuid;
        uint64 whitemaneDoor;

        void Initialize() OVERRIDE
        {
            SetBossNumber(ENCOUNTER_COUNT);
            LoadDoorData(doorData);
        }

        void OnCreatureCreate(Creature* creature) OVERRIDE
        {
            switch (creature->GetEntry())
            {
                case NPC_THALNOS_THE_SOULRENDER: bossGuids[BOSS_THALNOS_THE_SOULRENDER] = creature->GetGUID(); break;
                case NPC_BROTHER_KORLOFF: bossGuids[BOSS_BROTHER_KORLOFF] = creature->GetGUID(); break;
                case NPC_HIGH_INQUISITOR_WHITEMANE: bossGuids[BOSS_WHITEMANE] = creature->GetGUID(); break;
                case NPC_COMMANDER_DURAND:
                    bossGuids[BOSS_DURAND] = creature->GetGUID();
                    if (GetBossState(BOSS_WHITEMANE) != DONE && GetBossState(BOSS_DURAND) == NOT_STARTED && !creature->IsAlive())
                        creature->Respawn();
                    break;
                case NPC_HOODED_CRUSADER_OUTRO:
                    hoodedGuid = creature->GetGUID();
                    creature->SetVisible(false);
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go) OVERRIDE
        {
            switch (go->GetEntry())
            {
                case GO_THALNOS_EXIT:
                case GO_KORLOFF_EXIT:
                    AddDoor(go, true);
                    break;
                case GO_WHITEMANE_ENTRANCE:
                    whitemaneDoor = go->GetGUID();
                    AddDoor(go, true);
                    if (GetBossState(BOSS_WHITEMANE) == DONE)
                        HandleGameObject(whitemaneDoor, true);
                    break;
            }
        }

        void OnGameObjectRemove(GameObject* go) OVERRIDE
        {
            switch (go->GetEntry())
            {
                case GO_THALNOS_EXIT:
                case GO_KORLOFF_EXIT:
                    AddDoor(go, false);
                    break;
                case GO_WHITEMANE_ENTRANCE:
                    AddDoor(go, false);
                    whitemaneDoor = 0;
                    break;
            }
        }

        uint64 GetData64(uint32 type) const OVERRIDE
        {
            if (type < ENCOUNTER_COUNT)
                return bossGuids[type];
            if (type == NPC_HOODED_CRUSADER_OUTRO)
                return hoodedGuid;
            return type == GO_WHITEMANE_ENTRANCE ? whitemaneDoor : 0;
        }

        bool SetBossState(uint32 type, EncounterState state) OVERRIDE
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            if (type == BOSS_WHITEMANE && state == DONE)
                HandleGameObject(whitemaneDoor, true);
            return true;
        }

        std::string GetSaveData() OVERRIDE
        {
            std::ostringstream out;
            out << "S C M " << GetBossSaveData();
            return out.str();
        }

        void Load(char const* data) OVERRIDE
        {
            if (!data)
                return;
            std::istringstream in(data);
            char a, b, c;
            if (!(in >> a >> b >> c) || a != 'S' || b != 'C' || c != 'M')
                return;
            uint32 states[ENCOUNTER_COUNT];
            for (uint8 i = 0; i < ENCOUNTER_COUNT; ++i)
                if (!(in >> states[i]))
                    return;
            // A wipe or restart before Whitemane dies resets the paired fight.
            if (states[BOSS_WHITEMANE] != DONE)
                states[BOSS_DURAND] = NOT_STARTED;
            else if (states[BOSS_DURAND] == SPECIAL)
                states[BOSS_DURAND] = NOT_STARTED;
            for (uint8 i = 0; i < ENCOUNTER_COUNT; ++i)
            {
                if (states[i] == IN_PROGRESS || states[i] > SPECIAL)
                    states[i] = NOT_STARTED;
                SetBossState(i, EncounterState(states[i]));
            }
        }
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
    {
        return new instance_scarlet_monastery_mop_InstanceMapScript(map);
    }
};

void AddSC_instance_scarlet_monastery_mop()
{
    new instance_scarlet_monastery_mop();
}
