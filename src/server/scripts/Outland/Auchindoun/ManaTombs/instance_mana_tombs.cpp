/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "mana_tombs.h"

class instance_mana_tombs : public InstanceMapScript
{
    public:
        instance_mana_tombs() : InstanceMapScript(MTScriptName, 557) { }

        struct instance_mana_tombs_InstanceMapScript : public InstanceScript
        {
            instance_mana_tombs_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                // Tavarok is a SmartAI creature, which can only set instance data
                if (type == DATA_TAVAROK)
                    SetBossState(DATA_TAVAROK, EncounterState(data));
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_mana_tombs_InstanceMapScript(map);
        }
};

void AddSC_instance_mana_tombs()
{
    new instance_mana_tombs();
}
