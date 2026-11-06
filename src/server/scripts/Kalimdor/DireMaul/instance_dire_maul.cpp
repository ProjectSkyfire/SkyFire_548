/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
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

/*
This placeholder for the instance is needed for dungeon finding to be able
to give credit after the boss defined in lastEncounterDungeon is killed.
Without it, the party doing random dungeon won't get satchel of spoils and
gets instead the deserter debuff.
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "GameObject.h"
#include "Map.h"
#include "ScriptedCreature.h"

// Bosses (East)
// 0 - Pusillin
// 1 - Lethtendris
// 2 - Hydrospawn
// 3 - Zevrim Thornhoof
// 4 - Alzzin the Wildshaper

// West
// 5 - Tendris Warpwood
// 6 - Magister Kalendris
// 7 - Tsu'zee
// 8 - Illyanna Ravenoak
// 9 - Immol'thar
// 10 - Prince Tortheldrin
// 17 - CRYSTAL_01
// 18 - CRYSTAL_02
// 19 - CRYSTAL_03
// 20 - CRYSTAL_04
// 21 - CRYSTAL_05
// 22 - FORCEFIELD

// North
// 11 - Guard Mol'dar
// 12 - Stomper Kreeg
// 13 - Guard Fengus
// 14 - Guard Slip'kik
// 15 - Captain Kromcrush
// 16 - King Gordok

uint8 const EncounterCount = 23;

enum DMDataTypes
{
    DATA_CRYSTAL_01                     = 17,
    DATA_CRYSTAL_02                     = 18,
    DATA_CRYSTAL_03                     = 19,
    DATA_CRYSTAL_04                     = 20,
    DATA_CRYSTAL_05                     = 21,
    DATA_FORCEFIELD                     = 22
};

enum DMCreatureIds
{
    NPC_IMMOLTHAR                       = 11496,
    NPC_TORTHELDRIN                     = 11486,
    NPC_ARCANE_ABERRATION               = 11480,
    NPC_MANA_REMNANT                    = 11483
};

enum DMGameobjectIds
{
    GO_FORCEFIELD                       = 179503,
    GO_CRYSTAL_01                       = 177259,
    GO_CRYSTAL_02                       = 177257,
    GO_CRYSTAL_03                       = 177258,
    GO_CRYSTAL_04                       = 179504,
    GO_CRYSTAL_05                       = 179505,
    GO_PRINCE_CHEST                     = 179545
};

enum DMEvents
{
    EVENT_CRYSTAL_CREATURE_CHECK        = 1
};

enum DMMisc
{
    CRYSTAL_COUNT                       = 5,
    // a crystal whose guards are not found yet is retried this many times (once a second) before it counts as unguarded
    CRYSTAL_STORE_TRIES                 = 30,
    FACTION_HOSTILE_TORTHELDRIN         = 14
};

uint32 const CrystalMobs[2] = { NPC_ARCANE_ABERRATION, NPC_MANA_REMNANT };
uint32 const CrystalEntries[CRYSTAL_COUNT] = { GO_CRYSTAL_01, GO_CRYSTAL_02, GO_CRYSTAL_03, GO_CRYSTAL_04, GO_CRYSTAL_05 };
uint32 const CrystalData[CRYSTAL_COUNT] = { DATA_CRYSTAL_01, DATA_CRYSTAL_02, DATA_CRYSTAL_03, DATA_CRYSTAL_04, DATA_CRYSTAL_05 };

class instance_dire_maul : public InstanceMapScript
{
public:
    instance_dire_maul() : InstanceMapScript("instance_dire_maul", 429) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
    {
        return new instance_dire_maul_InstanceMapScript(map);
    }

    struct instance_dire_maul_InstanceMapScript : public InstanceScript
    {
        instance_dire_maul_InstanceMapScript(Map* map) : InstanceScript(map), _forcefieldGUID(0), _princeChestGUID(0),
            _immoltharGUID(0), _tortheldrinGUID(0)
        {
            SetBossNumber(EncounterCount);
            for (uint8 i = 0; i < CRYSTAL_COUNT; ++i)
            {
                _crystalGUIDs[i] = 0;
                _guardsStored[i] = false;
                _storeTries[i] = 0;
            }
        }

        void OnCreatureCreate(Creature* creature) OVERRIDE
        {
            switch (creature->GetEntry())
            {
                case NPC_IMMOLTHAR:
                    _immoltharGUID = creature->GetGUID();
                    // Pets could pull him out through the field otherwise
                    if (GetBossState(DATA_FORCEFIELD) != DONE)
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    break;
                case NPC_TORTHELDRIN:
                    _tortheldrinGUID = creature->GetGUID();
                    break;
                default:
                    break;
            }
        }

        void OnUnitDeath(Unit* unit) OVERRIDE
        {
            if (unit->GetGUID() == _immoltharGUID)
            {
                if (Creature* tortheldrin = instance->GetCreature(_tortheldrinGUID))
                    tortheldrin->setFaction(FACTION_HOSTILE_TORTHELDRIN);
            }
            else if (unit->GetGUID() == _tortheldrinGUID)
            {
                if (GameObject* chest = instance->GetGameObject(_princeChestGUID))
                    chest->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
            }
        }

        void OnGameObjectCreate(GameObject* go) OVERRIDE
        {
            for (uint8 i = 0; i < CRYSTAL_COUNT; ++i)
                if (go->GetEntry() == CrystalEntries[i])
                    _crystalGUIDs[i] = go->GetGUID();

            switch (go->GetEntry())
            {
                case GO_FORCEFIELD:
                    _forcefieldGUID = go->GetGUID();
                    if (GetBossState(DATA_FORCEFIELD) != DONE)
                        _events.ScheduleEvent(EVENT_CRYSTAL_CREATURE_CHECK, 1000);
                    else
                        go->SetGoState(GOState::GO_STATE_ACTIVE);
                    break;
                case GO_PRINCE_CHEST:
                    _princeChestGUID = go->GetGUID();
                    break;
                default:
                    break;
            }
        }

        void Update(uint32 diff) OVERRIDE
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                if (eventId == EVENT_CRYSTAL_CREATURE_CHECK)
                {
                    CrystalCreatureCheck();
                    if (GetBossState(DATA_FORCEFIELD) != DONE)
                        _events.ScheduleEvent(EVENT_CRYSTAL_CREATURE_CHECK, 1000);
                }
            }
        }

        // Grids load on demand, so a crystal's guards are stored the first time that crystal is in the world, not once
        // at the start; until then an unloaded crystal cannot count as cleared
        void StoreCrystalGuards(uint8 index, GameObject* crystal)
        {
            if (_guardsStored[index])
                return;

            for (uint32 entry : CrystalMobs)
            {
                std::list<Creature*> guards;
                crystal->GetCreatureListWithEntryInGrid(guards, entry, 30.0f);
                for (Creature* guard : guards)
                    _crystalGuards[index].push_back(guard->GetGUID());
            }

            if (!_crystalGuards[index].empty() || ++_storeTries[index] >= CRYSTAL_STORE_TRIES)
                _guardsStored[index] = true;
        }

        void CrystalCreatureCheck()
        {
            for (uint8 i = 0; i < CRYSTAL_COUNT; ++i)
            {
                if (GetBossState(CrystalData[i]) == DONE)
                    continue;

                GameObject* crystal = instance->GetGameObject(_crystalGUIDs[i]);
                if (!crystal)
                    continue;

                StoreCrystalGuards(i, crystal);
                if (!_guardsStored[i])
                    continue;

                bool guardAlive = false;
                for (uint64 guardGUID : _crystalGuards[i])
                {
                    Creature* guard = instance->GetCreature(guardGUID);
                    if (guard && guard->IsAlive())
                    {
                        guardAlive = true;
                        break;
                    }
                }

                if (!guardAlive)
                {
                    SetBossState(CrystalData[i], DONE);
                    crystal->SetGoState(GOState::GO_STATE_ACTIVE);
                }
            }

            for (uint8 i = 0; i < CRYSTAL_COUNT; ++i)
                if (GetBossState(CrystalData[i]) != DONE)
                    return;

            SetBossState(DATA_FORCEFIELD, DONE);
            if (GameObject* forcefield = instance->GetGameObject(_forcefieldGUID))
                forcefield->SetGoState(GOState::GO_STATE_ACTIVE);
            if (Creature* immolthar = instance->GetCreature(_immoltharGUID))
                immolthar->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

    private:
        EventMap _events;
        uint64 _crystalGUIDs[CRYSTAL_COUNT];
        std::list<uint64> _crystalGuards[CRYSTAL_COUNT];
        bool _guardsStored[CRYSTAL_COUNT];
        uint8 _storeTries[CRYSTAL_COUNT];
        uint64 _forcefieldGUID;
        uint64 _princeChestGUID;
        uint64 _immoltharGUID;
        uint64 _tortheldrinGUID;
    };
};

void AddSC_instance_dire_maul()
{
    new instance_dire_maul();
}
