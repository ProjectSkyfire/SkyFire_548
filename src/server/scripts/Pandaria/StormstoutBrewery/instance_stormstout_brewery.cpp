/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "stormstout_brewery.h"

class instance_stormstout_brewery : public InstanceMapScript
{
    public:
        instance_stormstout_brewery() : InstanceMapScript(StormstoutBreweryScriptName, 961) { }

        struct instance_stormstout_brewery_InstanceScript : public InstanceScript
        {
            instance_stormstout_brewery_InstanceScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                //LoadDoorData(doorData);

                OokOokGUID       = 0;
                HoptalusGUID     = 0;
                YanZhuGUID       = 0;
                BananaCount      = 0;
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_OOK_OOK:
                        OokOokGUID = creature->GetGUID();
                        break;
                    case NPC_HOPTALLUS:
                        HoptalusGUID = creature->GetGUID();
                        break;
                    case NPC_YAN_ZHU:
                        YanZhuGUID = creature->GetGUID();
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
                    case DATA_OOK_OOK:
                        return OokOokGUID;
                    case DATA_HOPTALLUS:
                        return HoptalusGUID;
                    case DATA_YAN_ZHU:
                        return YanZhuGUID;
                    case DATA_BANANA_EVENT:
                        return BananaCount;
                    default:
                        break;
                }
                return 0;
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_BANANA_EVENT:
                        if (data == SPECIAL)
                            ++BananaCount;
                        else if (data == IN_PROGRESS)
                            BananaCount = 0;
                        break;
                }
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S S B " << GetBossSaveData();

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

                if (dataHead1 == 'S' && dataHead2 == 'S' && dataHead3 == 'B')
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
            uint64 OokOokGUID;
            uint64 HoptalusGUID;
            uint64 YanZhuGUID;
            uint64 BananaCount;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_stormstout_brewery_InstanceScript(map);
        }
};

void AddSC_instance_stormstout_brewery()
{
   new instance_stormstout_brewery();
}
