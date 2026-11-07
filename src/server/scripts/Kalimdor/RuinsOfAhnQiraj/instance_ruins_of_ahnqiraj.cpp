/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "CreatureAI.h"
#include "Map.h"
#include "Player.h"
#include <set>
#include "ruins_of_ahnqiraj.h"

enum RajaxxWaves
{
    RAJAXX_WAVE_COUNT = 7,
    RAJAXX_WAVE_SIZE  = 7
};

// The seven waves of General Rajaxx's encounter, by spawn guid; each wave is a captain with his warriors and needlers
uint32 const RajaxxWaveSpawns[RAJAXX_WAVE_COUNT][RAJAXX_WAVE_SIZE] =
{
    { 9200200, 9200221, 9200199, 9200224, 9200223, 9200222, 9200071 },
    { 9200208, 9200206, 9200205, 9200228, 9200227, 9200226, 9200040 },
    { 9200203, 9200202, 9200201, 9200207, 9200225, 9200204, 9200041 },
    { 9200232, 9200230, 9200229, 9200215, 9200214, 9200212, 9200042 },
    { 9200211, 9200210, 9200209, 9200231, 9200213, 9200248, 9200073 },
    { 9200220, 9200237, 9200219, 9200249, 9200238, 9200236, 9200072 },
    { 9200218, 9200217, 9200216, 9200235, 9200234, 9200233, 9200112 }
};

class instance_ruins_of_ahnqiraj : public InstanceMapScript
{
    public:
        instance_ruins_of_ahnqiraj() : InstanceMapScript("instance_ruins_of_ahnqiraj", 509) { }

        struct instance_ruins_of_ahnqiraj_InstanceMapScript : public InstanceScript
        {
            instance_ruins_of_ahnqiraj_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(NUM_ENCOUNTER);

                _kurinaxxGUID   = 0;
                _rajaxxGUID     = 0;
                _moamGUID       = 0;
                _buruGUID       = 0;
                _ayamissGUID    = 0;
                _ossirianGUID   = 0;
                _paralyzedGUID  = 0;
                _rajaxxEvent    = NOT_STARTED;
                _checkTimer     = 1000;
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_KURINAXX:
                        _kurinaxxGUID = creature->GetGUID();
                        break;
                    case NPC_RAJAXX:
                        _rajaxxGUID = creature->GetGUID();
                        break;
                    case NPC_MOAM:
                        _moamGUID = creature->GetGUID();
                        break;
                    case NPC_BURU:
                        _buruGUID = creature->GetGUID();
                        break;
                    case NPC_AYAMISS:
                        _ayamissGUID = creature->GetGUID();
                        break;
                    case NPC_OSSIRIAN:
                        _ossirianGUID = creature->GetGUID();
                        break;
                }

                for (uint8 wave = 0; wave < RAJAXX_WAVE_COUNT; ++wave)
                    for (uint8 i = 0; i < RAJAXX_WAVE_SIZE; ++i)
                        if (RajaxxWaveSpawns[wave][i] == creature->GetDBTableGUIDLow())
                        {
                            _waveMembers[wave].insert(creature->GetGUID());
                            if (creature->IsAlive())
                                _waveAlive[wave].insert(creature->GetGUID());
                        }
            }

            void OnUnitDeath(Unit* unit) OVERRIDE
            {
                if (unit->GetTypeId() != TypeID::TYPEID_UNIT || _rajaxxEvent != IN_PROGRESS)
                    return;

                for (uint8 wave = 0; wave < RAJAXX_WAVE_COUNT; ++wave)
                    if (_waveAlive[wave].erase(unit->GetGUID()) && _waveAlive[wave].empty())
                    {
                        if (wave + 1 < RAJAXX_WAVE_COUNT)
                            SendWave(wave + 1);

                        // Rajaxx announces waves 3 to 7 as the previous one falls, then joins after the last
                        if (wave >= 1)
                            if (Creature* rajaxx = instance->GetCreature(_rajaxxGUID))
                                rajaxx->AI()->DoAction(ACTION_WAVE_STARTS_3 + wave - 1);
                        return;
                    }
            }

            void SendWave(uint8 wave)
            {
                for (std::set<uint64>::const_iterator itr = _waveAlive[wave].begin(); itr != _waveAlive[wave].end(); ++itr)
                    if (Creature* creature = instance->GetCreature(*itr))
                        if (creature->IsAlive() && !creature->IsInCombat())
                            creature->AI()->DoZoneInCombat(creature, 200.0f);
            }

            bool AnyPlayerFighting() const
            {
                Map::PlayerList const& players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* player = itr->GetSource())
                        if (player->IsAlive() && !player->IsGameMaster() && player->IsInCombat())
                            return true;
                return false;
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                if (type != DATA_RAJAXX_EVENT || data == _rajaxxEvent)
                    return;

                _rajaxxEvent = data;
                if (data == IN_PROGRESS)
                    SendWave(0);
                else if (data == NOT_STARTED)
                {
                    // a wipe: every wave comes back for the next attempt
                    for (uint8 wave = 0; wave < RAJAXX_WAVE_COUNT; ++wave)
                    {
                        _waveAlive[wave].clear();
                        for (std::set<uint64>::const_iterator itr = _waveMembers[wave].begin(); itr != _waveMembers[wave].end(); ++itr)
                            if (Creature* creature = instance->GetCreature(*itr))
                            {
                                if (!creature->IsAlive())
                                    creature->Respawn();
                                _waveAlive[wave].insert(*itr);
                            }
                    }
                }
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                return type == DATA_RAJAXX_EVENT ? _rajaxxEvent : 0;
            }

            void Update(uint32 diff) OVERRIDE
            {
                if (_checkTimer > diff)
                {
                    _checkTimer -= diff;
                    return;
                }
                _checkTimer = 1000;

                if (GetBossState(DATA_RAJAXX) == DONE)
                    return;

                if (_rajaxxEvent == NOT_STARTED)
                {
                    // until Lieutenant General Andorov leads the attack, engaging the first wave starts the event
                    for (std::set<uint64>::const_iterator itr = _waveAlive[0].begin(); itr != _waveAlive[0].end(); ++itr)
                        if (Creature* creature = instance->GetCreature(*itr))
                            if (creature->IsInCombat())
                            {
                                SetData(DATA_RAJAXX_EVENT, IN_PROGRESS);
                                break;
                            }
                }
                else if (_rajaxxEvent == IN_PROGRESS && !AnyPlayerFighting())
                    SetData(DATA_RAJAXX_EVENT, NOT_STARTED);
            }

            bool SetBossState(uint32 bossId, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(bossId, state))
                    return false;

                if (bossId == DATA_RAJAXX && state == DONE)
                    _rajaxxEvent = DONE;

                return true;
            }

            void SetData64(uint32 type, uint64 data) OVERRIDE
            {
                if (type == DATA_PARALYZED)
                    _paralyzedGUID = data;
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_KURINNAXX:
                        return _kurinaxxGUID;
                    case DATA_RAJAXX:
                        return _rajaxxGUID;
                    case DATA_MOAM:
                        return _moamGUID;
                    case DATA_BURU:
                        return _buruGUID;
                    case DATA_AYAMISS:
                        return _ayamissGUID;
                    case DATA_OSSIRIAN:
                        return _ossirianGUID;
                    case DATA_PARALYZED:
                        return _paralyzedGUID;
                }

                return 0;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "R A" << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* data) OVERRIDE
            {
                if (!data)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(data);

                char dataHead1, dataHead2;

                std::istringstream loadStream(data);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'R' && dataHead2 == 'A')
                {
                    for (uint8 i = 0; i < NUM_ENCOUNTER; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > TO_BE_DECIDED)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            uint64 _kurinaxxGUID;
            uint64 _rajaxxGUID;
            uint64 _moamGUID;
            uint64 _buruGUID;
            uint64 _ayamissGUID;
            uint64 _ossirianGUID;
            uint64 _paralyzedGUID;
            uint32 _rajaxxEvent;
            uint32 _checkTimer;
            std::set<uint64> _waveMembers[RAJAXX_WAVE_COUNT];
            std::set<uint64> _waveAlive[RAJAXX_WAVE_COUNT];
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_ruins_of_ahnqiraj_InstanceMapScript(map);
        }
};

void AddSC_instance_ruins_of_ahnqiraj()
{
    new instance_ruins_of_ahnqiraj();
}
