/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "baradin_hold.h"
#include "InstanceScript.h"
#include "ScriptMgr.h"

DoorData const doorData[] =
{
    { GO_ARGALOTH_DOOR,  DATA_ARGALOTH, DOOR_TYPE_ROOM,  BOUNDARY_NONE },
    { GO_OCCUTHAR_DOOR,  DATA_OCCUTHAR, DOOR_TYPE_ROOM,  BOUNDARY_NONE },
    { GO_ALIZABAL_DOOR,       DATA_ALIZABAL, DOOR_TYPE_ROOM,  BOUNDARY_NONE },
};

class instance_baradin_hold: public InstanceMapScript
{
    public:
        instance_baradin_hold() : InstanceMapScript(BHScriptName, 757) { }

        struct instance_baradin_hold_InstanceMapScript: public InstanceScript
        {
            instance_baradin_hold_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);

                ArgalothGUID = 0;
                OccutharGUID = 0;
                AlizabalGUID = 0;
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch(creature->GetEntry())
                {
                    case BOSS_ARGALOTH:
                        ArgalothGUID = creature->GetGUID();
                        break;
                    case BOSS_OCCUTHAR:
                        OccutharGUID = creature->GetGUID();
                        break;
                    case BOSS_ALIZABAL:
                        AlizabalGUID = creature->GetGUID();
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                switch(go->GetEntry())
                {
                    case GO_ARGALOTH_DOOR:
                    case GO_OCCUTHAR_DOOR:
                    case GO_ALIZABAL_DOOR:
                        AddDoor(go, true);
                        break;
                }
            }

            uint64 GetData64(uint32 data) const override
            {
                switch (data)
                {
                    case DATA_ARGALOTH:
                        return ArgalothGUID;
                    case DATA_OCCUTHAR:
                        return OccutharGUID;
                    case DATA_ALIZABAL:
                        return AlizabalGUID;
                    default:
                        break;
                }

                return 0;
            }

            void OnGameObjectRemove(GameObject* go) override
            {
                switch(go->GetEntry())
                {
                    case GO_ARGALOTH_DOOR:
                    case GO_OCCUTHAR_DOOR:
                    case GO_ALIZABAL_DOOR:
                        AddDoor(go, false);
                        break;
                }
            }

            std::string GetSaveData() override
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "B H " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(const char* in) override
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'B' && dataHead2 == 'H')
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
            uint64 ArgalothGUID;
            uint64 OccutharGUID;
            uint64 AlizabalGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_baradin_hold_InstanceMapScript(map);
        }
};

void AddSC_instance_baradin_hold()
{
    new instance_baradin_hold();
}
