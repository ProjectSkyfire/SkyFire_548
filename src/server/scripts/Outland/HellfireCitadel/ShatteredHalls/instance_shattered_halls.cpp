/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Instance_Shattered_Halls
SD%Complete: 50
SDComment: instance not complete
SDCategory: Hellfire Citadel, Shattered Halls
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "CreatureAI.h"
#include "Map.h"
#include "Player.h"
#include "SpellAuras.h"
#include "shattered_halls.h"

class instance_shattered_halls : public InstanceMapScript
{
    public:
        instance_shattered_halls() : InstanceMapScript("instance_shattered_halls", 540) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_shattered_halls_InstanceMapScript(map);
        }

        struct instance_shattered_halls_InstanceMapScript : public InstanceScript
        {
            instance_shattered_halls_InstanceMapScript(Map* map) : InstanceScript(map) { }

            void Initialize() OVERRIDE
            {
                SetBossNumber(EncounterCount);
                nethekurseGUID      = 0;
                nethekurseDoor1GUID = 0;
                nethekurseDoor2GUID = 0;
                kargathGUID         = 0;
                executionerGUID     = 0;
                for (uint8 i = 0; i < VictimCount; ++i)
                    victimsGUID[i] = 0;
                executed            = 0;
                executionTimer      = 0;
                executionerEvent    = EXECUTIONER_EVENT_NOT_STARTED;
                restoreExecutioner  = false;
                team                = 0;
            }

            void OnPlayerEnter(Player* player) OVERRIDE
            {
                if (!team)
                    team = player->GetTeam();

                player->CastSpell(player, SPELL_REMOVE_KARGATH_EXECUTIONER, true);

                if (!executionTimer || !executionerGUID)
                    return;

                uint32 spellId = 0;
                switch (executed)
                {
                    case 0:
                        spellId = SPELL_KARGATH_EXECUTIONER_1;
                        break;
                    case 1:
                        spellId = SPELL_KARGATH_EXECUTIONER_2;
                        break;
                    case 2:
                        spellId = SPELL_KARGATH_EXECUTIONER_3;
                        break;
                    default:
                        break;
                }

                if (spellId)
                    if (Aura* aura = player->AddAura(spellId, player))
                        aura->SetDuration(executionTimer);
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_GRAND_WARLOCK_CHAMBER_DOOR_1:
                        nethekurseDoor1GUID = go->GetGUID();
                        break;
                    case GO_GRAND_WARLOCK_CHAMBER_DOOR_2:
                        nethekurseDoor2GUID = go->GetGUID();
                        break;
                }
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                if (!team)
                {
                    Map::PlayerList const& players = instance->GetPlayers();
                    if (!players.isEmpty())
                        if (Player* player = players.begin()->GetSource())
                            team = player->GetTeam();
                }

                switch (creature->GetEntry())
                {
                    case NPC_GRAND_WARLOCK_NETHEKURSE:
                        nethekurseGUID = creature->GetGUID();
                        break;
                    case NPC_KARGATH_BLADEFIST:
                        kargathGUID = creature->GetGUID();
                        break;
                    case NPC_RANDY_WHIZZLESPROCKET:
                        // The Horde prisoner takes his place
                        if (team == HORDE)
                            creature->UpdateEntry(NPC_DRISELLA);
                        break;
                    case NPC_SHATTERED_EXECUTIONER:
                        executionerGUID = creature->GetGUID();
                        if (executionerEvent == EXECUTIONER_EVENT_NOT_STARTED)
                        {
                            executionerEvent = EXECUTIONER_EVENT_RUNNING;
                            executionTimer = 55 * MINUTE * IN_MILLISECONDS;
                            DoCastSpellOnPlayers(SPELL_KARGATH_EXECUTIONER_1);
                        }
                        SaveToDB();
                        break;
                    case NPC_CAPTAIN_ALINA:
                    case NPC_CAPTAIN_BONESHATTER:
                        victimsGUID[0] = creature->GetGUID();
                        break;
                    case NPC_ALLIANCE_VICTIM_1:
                    case NPC_HORDE_VICTIM_1:
                        victimsGUID[1] = creature->GetGUID();
                        break;
                    case NPC_ALLIANCE_VICTIM_2:
                    case NPC_HORDE_VICTIM_2:
                        victimsGUID[2] = creature->GetGUID();
                        break;
                }
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_NETHEKURSE:
                        if (state == IN_PROGRESS)
                        {
                            HandleGameObject(nethekurseDoor1GUID, false);
                            HandleGameObject(nethekurseDoor2GUID, false);
                        }
                        else
                        {
                            HandleGameObject(nethekurseDoor1GUID, true);
                            HandleGameObject(nethekurseDoor2GUID, true);
                        }
                        break;
                    case DATA_KARGATH:
                        // The executioner stops being immune once Kargath is dead
                        if (Creature* executioner = instance->GetCreature(executionerGUID))
                            executioner->AI()->Reset();
                        break;
                    case DATA_OMROGG:
                        break;
                }
                return true;
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                if (type == DATA_SHATTERED_EXECUTIONER && data == DONE)
                {
                    DoCastSpellOnPlayers(SPELL_REMOVE_KARGATH_EXECUTIONER);
                    executionTimer = 0;
                    executionerEvent = EXECUTIONER_EVENT_DONE;
                    SaveToDB();
                }
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_PRISONERS_EXECUTED:
                        return executed;
                    case DATA_TEAM_IN_INSTANCE:
                        return team;
                    case DATA_EXECUTIONER_EVENT:
                        return executionerEvent;
                    default:
                        return 0;
                }
            }

            void Update(uint32 diff) OVERRIDE
            {
                if (!executionTimer)
                    return;

                if (restoreExecutioner)
                    RestoreExecutioner();

                if (executionTimer > diff)
                {
                    executionTimer -= diff;
                    return;
                }

                DoCastSpellOnPlayers(SPELL_REMOVE_KARGATH_EXECUTIONER);
                switch (++executed)
                {
                    case 1:
                        DoCastSpellOnPlayers(SPELL_KARGATH_EXECUTIONER_2);
                        executionTimer = 10 * MINUTE * IN_MILLISECONDS;
                        break;
                    case 2:
                        DoCastSpellOnPlayers(SPELL_KARGATH_EXECUTIONER_3);
                        executionTimer = 15 * MINUTE * IN_MILLISECONDS;
                        break;
                    default:
                        executionTimer = 0;
                        executionerEvent = EXECUTIONER_EVENT_DONE;
                        break;
                }

                if (Creature* executioner = instance->GetCreature(executionerGUID))
                    executioner->AI()->SetData(DATA_PRISONERS_EXECUTED, executed);

                SaveToDB();
            }

            // The event was running when the instance was saved: put the executioner and the
            // prisoners that are still alive back as soon as Kargath exists
            void RestoreExecutioner()
            {
                instance->LoadGrid(ExecutionerPos.GetPositionX(), ExecutionerPos.GetPositionY());

                Creature* kargath = instance->GetCreature(kargathGUID);
                if (!kargath)
                    return;

                restoreExecutioner = false;

                if (executionerGUID)
                    return;

                if (Creature* executioner = kargath->SummonCreature(NPC_SHATTERED_EXECUTIONER, ExecutionerPos))
                    for (uint8 i = executed; i < VictimCount; ++i)
                        executioner->SummonCreature(team == ALLIANCE ? executionerVictims[i].allianceEntry : executionerVictims[i].hordeEntry, executionerVictims[i].pos);
            }

            uint64 GetData64(uint32 data) const OVERRIDE
            {
                switch (data)
                {
                    case NPC_GRAND_WARLOCK_NETHEKURSE:
                        return nethekurseGUID;
                        break;
                    case GO_GRAND_WARLOCK_CHAMBER_DOOR_1:
                        return nethekurseDoor1GUID;
                        break;
                    case GO_GRAND_WARLOCK_CHAMBER_DOOR_2:
                        return nethekurseDoor2GUID;
                        break;
                    case NPC_KARGATH_BLADEFIST:
                        return kargathGUID;
                    case NPC_SHATTERED_EXECUTIONER:
                        return executionerGUID;
                    case DATA_FIRST_PRISONER:
                    case DATA_SECOND_PRISONER:
                    case DATA_THIRD_PRISONER:
                        return victimsGUID[data - DATA_FIRST_PRISONER];
                }
                return 0;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S H " << GetBossSaveData();

                if (instance->IsHeroic())
                    saveStream << ' ' << uint32(executed) << ' ' << executionTimer << ' ' << uint32(executionerEvent);

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(const char* strIn) OVERRIDE
            {
                if (!strIn)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(strIn);

                char dataHead1, dataHead2;

                std::istringstream loadStream(strIn);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'S' && dataHead2 == 'H')
                {
                    for (uint8 i = 0; i < EncounterCount; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }

                    if (instance->IsHeroic())
                    {
                        uint32 savedExecuted = 0;
                        uint32 savedTimer = 0;
                        uint32 savedEvent = EXECUTIONER_EVENT_NOT_STARTED;
                        if (loadStream >> savedExecuted >> savedTimer)
                        {
                            executed = savedExecuted < VictimCount ? uint8(savedExecuted) : uint8(VictimCount);
                            executionTimer = executed < VictimCount ? savedTimer : 0;

                            // Saves from before the event state existed end after the timer
                            if (!(loadStream >> savedEvent))
                                savedEvent = executionTimer ? EXECUTIONER_EVENT_RUNNING : (executed ? EXECUTIONER_EVENT_DONE : EXECUTIONER_EVENT_NOT_STARTED);

                            if (savedEvent > EXECUTIONER_EVENT_DONE)
                                savedEvent = EXECUTIONER_EVENT_NOT_STARTED;
                            else if (savedEvent == EXECUTIONER_EVENT_RUNNING && !executionTimer)
                                savedEvent = EXECUTIONER_EVENT_DONE;

                            executionerEvent = uint8(savedEvent);
                            restoreExecutioner = executionTimer != 0;
                        }
                    }
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

            protected:
                uint64 nethekurseGUID;
                uint64 nethekurseDoor1GUID;
                uint64 nethekurseDoor2GUID;
                uint64 kargathGUID;
                uint64 executionerGUID;
                uint64 victimsGUID[VictimCount];

                uint8 executed;
                uint32 executionTimer;
                uint8 executionerEvent;
                bool restoreExecutioner;
                uint32 team;
        };
};

void AddSC_instance_shattered_halls()
{
    new instance_shattered_halls();
}
