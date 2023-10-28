/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
This placeholder for the instance is needed for dungeon finding to be able
to give credit after the boss defined in lastEncounterDungeon is killed.
Without it, the party doing random dungeon won't get satchel of spoils and
gets instead the deserter debuff.
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "instance_the_stockade.h"

class instance_the_stockade : public InstanceMapScript
{
public:
    instance_the_stockade() : InstanceMapScript(TheStockadeScriptName, 34) { }

    struct instance_the_stockade_InstanceScript : public InstanceScript
    {
        instance_the_stockade_InstanceScript(Map* map) : InstanceScript(map)
        {
            SetBossNumber(EncounterCount);

            LordOverheatGUID = 0;
            HoggerGUID = 0;
        }

        void OnCreatureCreate(Creature* creature) OVERRIDE
        {
            switch (creature->GetEntry())
            {
            case NPC_LORD_OVERHEAT:
                LordOverheatGUID = creature->GetGUID();
            case NPC_HOGGER:
                HoggerGUID = creature->GetGUID();
                break;
            default:
                break;
            }
        }

        uint64 GetData64(uint32 type) const OVERRIDE
        {
            switch (type)
            {
            case DATA_LORD_OVERHEAT:
                return LordOverheatGUID;
            case DATA_HOGGER:
                return HoggerGUID;
            default:
                break;
            }
            return 0;
        }

        std::string GetSaveData() OVERRIDE
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "T S " << GetBossSaveData();

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

            char dataHead1, dataHead2;

            std::istringstream loadStream(str);
            loadStream >> dataHead1 >> dataHead2;

            if (dataHead1 == 'T' && dataHead2 == 'S')
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
        uint64 LordOverheatGUID;
        uint64 HoggerGUID;
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
    {
        return new instance_the_stockade_InstanceScript(map);
    }
};

void AddSC_instance_the_stockade()
{
    new instance_the_stockade();
}