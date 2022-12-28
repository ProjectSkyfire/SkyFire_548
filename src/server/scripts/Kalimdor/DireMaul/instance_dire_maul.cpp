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

class instance_dire_maul : public InstanceMapScript
{
public:
    instance_dire_maul() : InstanceMapScript("instance_dire_maul", 429) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_dire_maul_InstanceMapScript(map);
    }

    struct instance_dire_maul_InstanceMapScript : public InstanceScript
    {
        instance_dire_maul_InstanceMapScript(Map* map) : InstanceScript(map) { }
    };
};

void AddSC_instance_dire_maul()
{
    new instance_dire_maul();
}
