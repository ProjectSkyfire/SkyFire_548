/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
The instance also gives the dungeon finder its credit after the boss defined in
lastEncounterDungeon is killed. Without it, the party doing random dungeon won't
get satchel of spoils and gets instead the deserter debuff.
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "the_slave_pens.h"

class instance_the_slave_pens : public InstanceMapScript
{
public:
    instance_the_slave_pens() : InstanceMapScript("instance_the_slave_pens", 547) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
    {
        return new instance_the_slave_pens_InstanceMapScript(map);
    }

    struct instance_the_slave_pens_InstanceMapScript : public InstanceScript
    {
        instance_the_slave_pens_InstanceMapScript(Map* map) : InstanceScript(map)
        {
            SetBossNumber(EncounterCount);
        }

        std::string GetSaveData() OVERRIDE
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "S P " << GetBossSaveData();

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

            if (dataHead1 == 'S' && dataHead2 == 'P')
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
    };
};

void AddSC_instance_the_slave_pens()
{
    new instance_the_slave_pens();
}
