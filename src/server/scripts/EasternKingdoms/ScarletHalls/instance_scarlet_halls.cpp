/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "scarlet_halls.h"

DoorData const doorData[] =
{
    { GO_COMANDER_LINDON_EXIT,   DATA_COMANDER_LINDON,   DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_COMANDER_LINDON_EXIT,   BOSS_HOUNDMASTER_BRAUN, DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { GO_HOUNDMASTER_BRAUN_EXIT, BOSS_HOUNDMASTER_BRAUN, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_ARMSMASTER_HARLAN_EXIT, BOSS_ARMSMASTER_HARLAN, DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { 0,                         0,                      DOOR_TYPE_ROOM,    BOUNDARY_NONE },
};

class instance_scarlet_halls : public InstanceMapScript
{
    public:
        instance_scarlet_halls() : InstanceMapScript("instance_scarlet_halls", 1001) { }

        struct instance_scarlet_halls_InstanceMapScript : public InstanceScript
        {
            instance_scarlet_halls_InstanceMapScript(Map* map) : InstanceScript(map) { }

            void Initialize() OVERRIDE
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);

                houndmasterBraunGUID    = 0;
                armsmasterHarlanGUID    = 0;
                flameweaverKoeglerGUID  = 0;
                lindonGUID              = 0;
                hoodedCrusaderGUID      = 0;

                instance->SetWorldState(WORLDSTATE_HUMANE_SOCIETY, 1);
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_HOUNDMASTER_BRAUN:
                        houndmasterBraunGUID = creature->GetGUID();
                        break;
                    case NPC_ARMSMASTER_HARLAN:
                        armsmasterHarlanGUID = creature->GetGUID();
                        break;
                    case NPC_FLAMEWEAVER_KOEGLER:
                        flameweaverKoeglerGUID = creature->GetGUID();
                        break;
                    case NPC_COMANDER_LINDON:
                        lindonGUID = creature->GetGUID();
                        break;
                    case NPC_SCARLET_GUARDIAN:
                    case NPC_SERGEANT_VERDONE:
                        creature->SetReactState(REACT_PASSIVE);
                        break;
                    case NPC_EXPLODING_SHOT_STALKER:
                        creature->SetDisplayId(11686);
                        creature->AddAura(114861, creature);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    case NPC_DRAGON_BREATH_TARGET:
                        creature->SetDisplayId(11686);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    case NPC_HOODED_CRUSADER:
                        // The one in Koegler's library; the other waits at the entrance
                        if (creature->GetPositionX() > 1200.0f)
                            hoodedCrusaderGUID = creature->GetGUID();
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_COMANDER_LINDON_EXIT:
                    case GO_HOUNDMASTER_BRAUN_EXIT:
                    case GO_ARMSMASTER_HARLAN_EXIT:
                        AddDoor(go, true);
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_COMANDER_LINDON_EXIT:
                    case GO_HOUNDMASTER_BRAUN_EXIT:
                    case GO_ARMSMASTER_HARLAN_EXIT:
                        AddDoor(go, false);
                        break;
                }
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case BOSS_HOUNDMASTER_BRAUN:
                        return houndmasterBraunGUID;
                    case BOSS_ARMSMASTER_HARLAN:
                        return armsmasterHarlanGUID;
                    case BOSS_FLAMEWEAVER_KOEGLER:
                        return flameweaverKoeglerGUID;
                    case DATA_COMANDER_LINDON:
                        return lindonGUID;
                    case DATA_HOODED_CRUSADER:
                        return hoodedCrusaderGUID;
                }

                return 0;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S H " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* in) OVERRIDE
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

        private:
            uint64 houndmasterBraunGUID;
            uint64 armsmasterHarlanGUID;
            uint64 flameweaverKoeglerGUID;
            uint64 lindonGUID;
            uint64 hoodedCrusaderGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_scarlet_halls_InstanceMapScript(map);
        }
};

void AddSC_instance_scarlet_halls()
{
    new instance_scarlet_halls();
}
