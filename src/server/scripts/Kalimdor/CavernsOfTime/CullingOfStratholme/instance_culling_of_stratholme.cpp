/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "CreatureTextMgr.h"
#include "culling_of_stratholme.h"
#include "Player.h"
#include "TemporarySummon.h"
#include "SpellInfo.h"

#define MAX_ENCOUNTER 5

/* Culling of Stratholme encounters:
0 - Meathook
1 - Salramm the Fleshcrafter
2 - Chrono-Lord Epoch
3 - Mal'Ganis
4 - Infinite Corruptor (Heroic only)
*/

enum Texts
{
    SAY_CRATES_COMPLETED    = 0,
};

enum ChromieTexts
{
    SAY_GUARDIAN_CAPTURED   = 0,
    SAY_GUARDIAN_HURRY      = 1,
    SAY_GUARDIAN_FADING     = 2,
};

enum Corruptor
{
    NPC_TIME_RIFT           = 28409,
    MAX_GUARDIAN_WARNINGS   = 2,
};

uint32 const GuardianMinutes = 25;
uint32 const GuardianTimer = GuardianMinutes * MINUTE * IN_MILLISECONDS;
// Time left when Chromie warns about the Guardian of Time
uint32 const GuardianWarnings[MAX_GUARDIAN_WARNINGS] = { 270 * IN_MILLISECONDS, 60 * IN_MILLISECONDS };

Position const ChromieSummonPos = {1813.298f, 1283.578f, 142.3258f, 3.878161f};
Position const CorruptorPos     = {2331.642f, 1273.273f, 132.9524f, 3.717551f};
Position const GuardianPos      = {2321.489f, 1268.383f, 132.8507f, 0.418879f};
Position const CorruptorRiftPos = {2443.626f, 1280.450f, 133.0066f, 1.727876f};

class instance_culling_of_stratholme : public InstanceMapScript
{
    public:
        instance_culling_of_stratholme() : InstanceMapScript("instance_culling_of_stratholme", 595) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_culling_of_stratholme_InstanceMapScript(map);
        }

        struct instance_culling_of_stratholme_InstanceMapScript : public InstanceScript
        {
            instance_culling_of_stratholme_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                _arthasGUID = 0;
                _meathookGUID = 0;
                _salrammGUID = 0;
                _epochGUID = 0;
                _malGanisGUID = 0;
                _infiniteGUID = 0;
                _shkafGateGUID = 0;
                _malGanisGate1GUID = 0;
                _malGanisGate2GUID = 0;
                _exitGateGUID = 0;
                _malGanisChestGUID = 0;
                _genericBunnyGUID = 0;
                _chromieGUID = 0;
                _guardianGUID = 0;
                _riftGUID = 0;
                _guardianTimer = 0;
                _guardianMinutes = GuardianMinutes;
                _guardianWarning = 0;
                memset(&_encounterState[0], 0, sizeof(uint32) * MAX_ENCOUNTER);
                _crateCount = 0;
            }

            bool IsEncounterInProgress() const OVERRIDE
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (_encounterState[i] == IN_PROGRESS)
                        return true;

                return false;
            }

            void FillInitialWorldStates(WorldStateBuilder& builder) OVERRIDE
            {
                builder.AppendState(WORLDSTATE_SHOW_CRATES, 1);
                builder.AppendState(WORLDSTATE_CRATES_REVEALED, _crateCount);
                builder.AppendState(WORLDSTATE_WAVE_COUNT, 0);
                builder.AppendState(WORLDSTATE_TIME_GUARDIAN, _guardianTimer ? _guardianMinutes : GuardianMinutes);
                builder.AppendState(WORLDSTATE_TIME_GUARDIAN_SHOW, _guardianTimer ? 1 : 0);
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_ARTHAS:
                        _arthasGUID = creature->GetGUID();
                        break;
                    case NPC_MEATHOOK:
                        _meathookGUID = creature->GetGUID();
                        break;
                    case NPC_SALRAMM:
                        _salrammGUID = creature->GetGUID();
                        break;
                    case NPC_EPOCH:
                        _epochGUID = creature->GetGUID();
                        break;
                    case NPC_MAL_GANIS:
                        _malGanisGUID = creature->GetGUID();
                        break;
                    case NPC_INFINITE:
                        _infiniteGUID = creature->GetGUID();
                        break;
                    case NPC_GENERIC_BUNNY:
                        _genericBunnyGUID = creature->GetGUID();
                        break;
                    case NPC_CHROMIE_1:
                        _chromieGUID = creature->GetGUID();
                        creature->setActive(true); // her warnings must reach the players far from the entrance
                        break;
                    case NPC_GUARDIAN_OF_TIME:
                        _guardianGUID = creature->GetGUID();
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_SHKAF_GATE:
                        _shkafGateGUID = go->GetGUID();
                        break;
                    case GO_MALGANIS_GATE_1:
                        _malGanisGate1GUID = go->GetGUID();
                        break;
                    case GO_MALGANIS_GATE_2:
                        _malGanisGate2GUID = go->GetGUID();
                        break;
                    case GO_EXIT_GATE:
                        _exitGateGUID = go->GetGUID();
                        if (_encounterState[3] == DONE)
                            HandleGameObject(_exitGateGUID, true);
                        break;
                    case GO_MALGANIS_CHEST_N:
                    case GO_MALGANIS_CHEST_H:
                        _malGanisChestGUID = go->GetGUID();
                        if (_encounterState[3] == DONE)
                            go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        break;
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_MEATHOOK_EVENT:
                        _encounterState[0] = data;
                        break;
                    case DATA_SALRAMM_EVENT:
                        _encounterState[1] = data;
                        break;
                    case DATA_EPOCH_EVENT:
                        _encounterState[2] = data;
                        break;
                    case DATA_MAL_GANIS_EVENT:
                        _encounterState[3] = data;

                        switch (_encounterState[3])
                        {
                            case NOT_STARTED:
                                HandleGameObject(_malGanisGate2GUID, true);
                                break;
                            case IN_PROGRESS:
                                HandleGameObject(_malGanisGate2GUID, false);
                                break;
                            case DONE:
                                HandleGameObject(_exitGateGUID, true);
                                if (GameObject* go = instance->GetGameObject(_malGanisChestGUID))
                                    go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                                break;
                        }
                        break;
                    case DATA_ARTHAS_EVENT:
                        if (data == IN_PROGRESS)
                            StartGuardianEvent();
                        break;
                    case DATA_INFINITE_EVENT:
                        _encounterState[4] = data;
                        if (data == DONE || data == FAIL)
                        {
                            _guardianTimer = 0;
                            DoUpdateWorldState(WORLDSTATE_TIME_GUARDIAN_SHOW, 0);
                        }
                        if (data == FAIL)
                        {
                            if (Creature* corruptor = instance->GetCreature(_infiniteGUID))
                            {
                                corruptor->AI()->DoAction(ACTION_CORRUPTOR_LEAVE);
                                if (Creature* guardian = instance->GetCreature(_guardianGUID))
                                    corruptor->Kill(guardian);
                            }
                        }
                        break;
                    case DATA_CRATE_COUNT:
                        _crateCount = data;
                        if (_crateCount == 5)
                        {
                            if (Creature* bunny = instance->GetCreature(_genericBunnyGUID))
                                bunny->CastSpell(bunny, SPELL_CRATES_CREDIT, true);

                            // Summon Chromie and global whisper
                            if (Creature* chromie = instance->SummonCreature(NPC_CHROMIE_2, ChromieSummonPos))
                                if (!instance->GetPlayers().isEmpty())
                                    if (Player* player = instance->GetPlayers().getFirst()->GetSource())
                                        sCreatureTextMgr->SendChat(chromie, SAY_CRATES_COMPLETED, player, ChatMsg::CHAT_MSG_ADDON, Language::LANG_ADDON, TEXT_RANGE_MAP);
                        }
                        DoUpdateWorldState(WORLDSTATE_CRATES_REVEALED, _crateCount);
                        break;
                }

                if (data == DONE)
                    SaveToDB();
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_MEATHOOK_EVENT:
                        return _encounterState[0];
                    case DATA_SALRAMM_EVENT:
                        return _encounterState[1];
                    case DATA_EPOCH_EVENT:
                        return _encounterState[2];
                    case DATA_MAL_GANIS_EVENT:
                        return _encounterState[3];
                    case DATA_INFINITE_EVENT:
                        return _encounterState[4];
                    case DATA_CRATE_COUNT:
                        return _crateCount;
                }
                return 0;
            }

            uint64 GetData64(uint32 identifier) const OVERRIDE
            {
                switch (identifier)
                {
                    case DATA_ARTHAS:
                        return _arthasGUID;
                    case DATA_MEATHOOK:
                        return _meathookGUID;
                    case DATA_SALRAMM:
                        return _salrammGUID;
                    case DATA_EPOCH:
                        return _epochGUID;
                    case DATA_MAL_GANIS:
                        return _malGanisGUID;
                    case DATA_INFINITE:
                        return _infiniteGUID;
                    case DATA_SHKAF_GATE:
                        return _shkafGateGUID;
                    case DATA_MAL_GANIS_GATE_1:
                        return _malGanisGate1GUID;
                    case DATA_MAL_GANIS_GATE_2:
                        return _malGanisGate2GUID;
                    case DATA_EXIT_GATE:
                        return _exitGateGUID;
                    case DATA_MAL_GANIS_CHEST:
                        return _malGanisChestGUID;
                    case DATA_GUARDIAN_OF_TIME:
                        return _guardianGUID;
                    case DATA_TIME_RIFT:
                        return _riftGUID;
                }
                return 0;
            }

            void Update(uint32 diff) OVERRIDE
            {
                if (!_guardianTimer)
                    return;

                if (_guardianTimer <= diff)
                {
                    SetData(DATA_INFINITE_EVENT, FAIL);
                    return;
                }

                _guardianTimer -= diff;

                uint32 minutes = (_guardianTimer + MINUTE * IN_MILLISECONDS - 1) / (MINUTE * IN_MILLISECONDS);
                if (minutes != _guardianMinutes)
                {
                    _guardianMinutes = minutes;
                    DoUpdateWorldState(WORLDSTATE_TIME_GUARDIAN, minutes);
                }

                if (_guardianWarning < MAX_GUARDIAN_WARNINGS && _guardianTimer <= GuardianWarnings[_guardianWarning])
                    WhisperChromie(SAY_GUARDIAN_HURRY + _guardianWarning++);
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "C S " << _encounterState[0] << ' ' << _encounterState[1] << ' '
                    << _encounterState[2] << ' ' << _encounterState[3] << ' ' << _encounterState[4];

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(const char* in) OVERRIDE
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2;
                uint16 data0, data1, data2, data3, data4;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3 >> data4;

                if (dataHead1 == 'C' && dataHead2 == 'S')
                {
                    _encounterState[0] = data0;
                    _encounterState[1] = data1;
                    _encounterState[2] = data2;
                    _encounterState[3] = data3;
                    _encounterState[4] = data4;

                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                        if (_encounterState[i] == IN_PROGRESS)
                            _encounterState[i] = NOT_STARTED;
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            uint64 _arthasGUID;
            uint64 _meathookGUID;
            uint64 _salrammGUID;
            uint64 _epochGUID;
            uint64 _malGanisGUID;
            uint64 _infiniteGUID;
            uint64 _shkafGateGUID;
            uint64 _malGanisGate1GUID;
            uint64 _malGanisGate2GUID;
            uint64 _exitGateGUID;
            uint64 _malGanisChestGUID;
            uint64 _genericBunnyGUID;
            uint64 _chromieGUID;
            uint64 _guardianGUID;
            uint64 _riftGUID;
            uint32 _encounterState[MAX_ENCOUNTER];
            uint32 _crateCount;
            uint32 _guardianTimer;
            uint32 _guardianMinutes;
            uint8 _guardianWarning;

            // The Infinite Dragonflight starts corrupting a Guardian of Time when the Scourge waves begin, heroic only
            void StartGuardianEvent()
            {
                if (!instance->IsHeroic() || _guardianTimer || _encounterState[4] != NOT_STARTED)
                    return;

                _guardianTimer = GuardianTimer;
                _guardianMinutes = GuardianMinutes;
                _guardianWarning = 0;

                if (Creature* rift = instance->SummonCreature(NPC_TIME_RIFT, CorruptorRiftPos))
                    _riftGUID = rift->GetGUID();
                instance->SummonCreature(NPC_GUARDIAN_OF_TIME, GuardianPos);
                instance->SummonCreature(NPC_INFINITE, CorruptorPos);

                DoUpdateWorldState(WORLDSTATE_TIME_GUARDIAN, _guardianMinutes);
                DoUpdateWorldState(WORLDSTATE_TIME_GUARDIAN_SHOW, 1);
                WhisperChromie(SAY_GUARDIAN_CAPTURED);
            }

            void WhisperChromie(uint8 group)
            {
                if (Creature* chromie = instance->GetCreature(_chromieGUID))
                    sCreatureTextMgr->SendChat(chromie, group, NULL, ChatMsg::CHAT_MSG_ADDON, Language::LANG_ADDON, TEXT_RANGE_MAP);
            }
        };
};

void AddSC_instance_culling_of_stratholme()
{
    new instance_culling_of_stratholme();
}
