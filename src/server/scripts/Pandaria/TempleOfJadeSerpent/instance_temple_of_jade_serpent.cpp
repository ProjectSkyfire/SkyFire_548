/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "temple_of_jade_serpent.h"

class instance_jade_serpent : public InstanceMapScript
{
    public:
        instance_jade_serpent() : InstanceMapScript(JadeSerpentScriptName, 960) { }

        struct instance_jade_serpent_InstanceScript : public InstanceScript
        {
            instance_jade_serpent_InstanceScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                //LoadDoorData(doorData);

                WiseMariGUID        = 0;
                StonestepGUID       = 0;
                FlameheartGUID      = 0;
                ShaOfDoubtGUID      = 0;
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_WISE_MARI:
                        WiseMariGUID = creature->GetGUID();
                        break;
                    case NPC_STONESTEP:
                        StonestepGUID = creature->GetGUID();
                        break;
                    case NPC_FLAMEHEART:
                        FlameheartGUID = creature->GetGUID();
                        break;
                    case NPC_SHA_OF_DOUBT:
                        ShaOfDoubtGUID = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            //void OnGameObjectCreate(GameObject* go) OVERRIDE
            //{}

            //void OnGameObjectRemove(GameObject* go) OVERRIDE
            //{}

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_WISE_MARI:
                        return WiseMariGUID;
                    case DATA_STONESTEP:
                        return StonestepGUID;
                    case DATA_FLAMEHEART:
                        return FlameheartGUID;
                    case DATA_SHA_OF_DOUBT:
                        return ShaOfDoubtGUID;
                    default:
                        break;
                }
                return 0;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "T J S " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* str) OVERRIDE
            {
                if (!str)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(str);

                char dataHead1, dataHead2, dataHead3;

                std::istringstream loadStream(str);
                loadStream >> dataHead1 >> dataHead2 >> dataHead3;

                if (dataHead1 == 'T' && dataHead2 == 'J' && dataHead3 == 'S')
                {
                    for (uint32 i = 0; i < EncounterCount; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        protected:
            uint64 WiseMariGUID;
            uint64 StonestepGUID;
            uint64 FlameheartGUID;
            uint64 ShaOfDoubtGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_jade_serpent_InstanceScript(map);
        }
};

void AddSC_instance_jade_serpent()
{
   new instance_jade_serpent();
}
