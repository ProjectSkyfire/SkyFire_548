/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "sethekk_halls.h"

DoorData const doorData[] =
{
    { GO_IKISS_DOOR,    DATA_TALON_KING_IKISS,  DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { 0,                0,                      DOOR_TYPE_ROOM,     BOUNDARY_NONE } // END
};

class instance_sethekk_halls : public InstanceMapScript
{
    public:
        instance_sethekk_halls() : InstanceMapScript(SHScriptName, 556) { }

        struct instance_sethekk_halls_InstanceMapScript : public InstanceScript
        {
            instance_sethekk_halls_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);
            }

            void OnCreatureCreate(Creature* creature) override
            {
                if (creature->GetEntry() == NPC_ANZU)
                {
                    if (GetBossState(DATA_ANZU) == DONE)
                        creature->DisappearAndDie();
                    else
                        SetBossState(DATA_ANZU, IN_PROGRESS);
                }
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                 if (go->GetEntry() == GO_IKISS_DOOR)
                     AddDoor(go, true);
            }

            void OnGameObjectRemove(GameObject* go) override
            {
                 if (go->GetEntry() == GO_IKISS_DOOR)
                     AddDoor(go, false);
            }

            std::string GetSaveData() override
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S H " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* str) override
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

                if (dataHead1 == 'S' && dataHead2 == 'H')
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
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_sethekk_halls_InstanceMapScript(map);
        }
};

void AddSC_instance_sethekk_halls()
{
    new instance_sethekk_halls();
}
