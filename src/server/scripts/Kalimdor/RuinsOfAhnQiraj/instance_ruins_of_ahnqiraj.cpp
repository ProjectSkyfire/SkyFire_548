/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ruins_of_ahnqiraj.h"

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
            }

            void OnCreatureCreate(Creature* creature) override
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
            }

            bool SetBossState(uint32 bossId, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(bossId, state))
                    return false;

                return true;
            }

            void SetData64(uint32 type, uint64 data) override
            {
                if (type == DATA_PARALYZED)
                    _paralyzedGUID = data;
            }

            uint64 GetData64(uint32 type) const override
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

            std::string GetSaveData() override
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "R A" << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* data) override
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
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_ruins_of_ahnqiraj_InstanceMapScript(map);
        }
};

void AddSC_instance_ruins_of_ahnqiraj()
{
    new instance_ruins_of_ahnqiraj();
}
