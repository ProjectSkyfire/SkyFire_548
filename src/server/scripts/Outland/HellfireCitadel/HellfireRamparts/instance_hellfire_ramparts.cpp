/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/
 
/* ScriptData
SDName: Instance_Hellfire_Ramparts
SD%Complete: 50
SDComment:
SDCategory: Hellfire Ramparts
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "hellfire_ramparts.h"

class instance_ramparts : public InstanceMapScript
{
    public:
        instance_ramparts() : InstanceMapScript("instance_ramparts", 543) { }

        struct instance_ramparts_InstanceMapScript : public InstanceScript
        {
            instance_ramparts_InstanceMapScript(Map* map) : InstanceScript(map) { }

            void Initialize() override
            {
                SetBossNumber(EncounterCount);
                felIronChestGUID = 0;
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                switch (go->GetEntry())
                {
                    case GO_FEL_IRON_CHEST_NORMAL:
                    case GO_FEL_IRON_CHECT_HEROIC:
                        felIronChestGUID = go->GetGUID();
                        break;
                }
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_VAZRUDEN:
                    case DATA_NAZAN:
                        if (GetBossState(DATA_VAZRUDEN) == DONE && GetBossState(DATA_NAZAN) == DONE && !spawned)
                        {
                            DoRespawnGameObject(felIronChestGUID, HOUR*IN_MILLISECONDS);
                            spawned = true;
                        }
                        break;
                }
                return true;
            }

            std::string GetSaveData() override
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "H R " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(const char* strIn) override
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

                if (dataHead1 == 'H' && dataHead2 == 'R')
                {
                    for (uint8 i = 0; i < EncounterCount; ++i)
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
                uint64 felIronChestGUID;
                bool spawned;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_ramparts_InstanceMapScript(map);
        }
};

void AddSC_instance_ramparts()
{
    new instance_ramparts;
}
