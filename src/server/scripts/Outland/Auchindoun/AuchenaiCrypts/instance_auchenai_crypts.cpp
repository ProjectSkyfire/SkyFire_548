/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "auchenai_crypts.h"

class instance_auchenai_crypts : public InstanceMapScript
{
    public:
        instance_auchenai_crypts() : InstanceMapScript(ACScriptName, 558) { }

        struct instance_auchenai_crypts_InstanceMapScript : public InstanceScript
        {
            instance_auchenai_crypts_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_auchenai_crypts_InstanceMapScript(map);
        }
};

void AddSC_instance_auchenai_crypts()
{
    new instance_auchenai_crypts();
}
