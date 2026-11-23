/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GameObject.h"
#include "Player.h"
#include "end_time.h"

#define MAX_ENCOUNTER 5

static const DoorData doordata[] =
{
    {GO_FIRE_WALL,  DATA_ECHO_OF_BAINE, DOOR_TYPE_ROOM, BOUNDARY_NONE},
    {0,             0,                  DOOR_TYPE_ROOM, BOUNDARY_NONE}
};

class instance_end_time : public InstanceMapScript
{
    public:
        instance_end_time() : InstanceMapScript("instance_end_time", 938) { }

        struct instance_end_time_InstanceMapScript : public InstanceScript
        {
            instance_end_time_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(MAX_ENCOUNTER);
                LoadDoorData(doordata);
                uiTeamInInstance = 0;
                uiFragmentsCollected = 0;
                jaina_event = NOT_STARTED;
                RandEchos();
                first_encounter = 0;
                second_encounter = 0;
                tyrande_event = 0;
                uiMurozondCacheGUID = 0;
                uiTyrandeGUID = 0;
                uiJainaGUID = 0;
                uiPlatform1GUID = 0;
                uiPlatform2GUID = 0;
                uiPlatform3GUID = 0;
                uiPlatform4GUID = 0;
                uiImageOfNozdormuGUID = 0;
                uiMurozondGUID = 0;
                uiHourglassGUID = 0;
                uiNozdormuGUID = 0;
                memset(nozdormu_dialog, 0, sizeof(nozdormu_dialog));
            }

            void RandEchos()
            {
                std::list<uint32> echo_list;
                for (uint8 i = 0; i < 4; ++i)
                    echo_list.push_back(i);
                first_echo = Skyfire::Containers::SelectRandomContainerElement(echo_list);
                echo_list.remove(first_echo);
                second_echo = Skyfire::Containers::SelectRandomContainerElement(echo_list);
            }

            void OnPlayerEnter(Player* player) OVERRIDE
            {
                if (!uiTeamInInstance)
                    uiTeamInInstance = player->GetTeam();
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case MUROZOND_CACHE:
                        uiMurozondCacheGUID = go->GetGUID();
                        break;
                    case GO_PLATFORM_1:
                        uiPlatform1GUID = go->GetGUID();
                        break;
                    case GO_PLATFORM_2:
                        uiPlatform2GUID = go->GetGUID();
                        break;
                    case GO_PLATFORM_3:
                        uiPlatform3GUID = go->GetGUID();
                        break;
                    case GO_PLATFORM_4:
                        uiPlatform4GUID = go->GetGUID();
                        break;
                    case HOURGLASS_OF_TIME:
                        uiHourglassGUID = go->GetGUID();
                        break;
                    case GO_FIRE_WALL:
                        AddDoor(go, true);
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_ECHO_OF_TYRANDE:
                        uiTyrandeGUID = creature->GetGUID();
                        break;
                    case NPC_ECHO_OF_JAINA:
                        uiJainaGUID = creature->GetGUID();
                        break;
                    case NPC_IMAGE_OF_NOZDORMU:
                        uiImageOfNozdormuGUID = creature->GetGUID();
                        break;
                    case NPC_MUROZOND:
                        uiMurozondGUID = creature->GetGUID();
                        break;
                    case NPC_NOZDORMU:
                        uiNozdormuGUID = creature->GetGUID();
                        break;
                }
            }

            void SetData(uint32 type,uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_JAINA_EVENT:
                        switch (data)
                        {
                            case IN_PROGRESS:
                                DoUpdateWorldState(WORLDSTATE_SHOW_FRAGMENTS, 1);
                                break;
                            case DONE:
                                DoUpdateWorldState(WORLDSTATE_SHOW_FRAGMENTS, 0);
                                break;
                        }
                        jaina_event = data;
                        break;
                    case DATA_FRAGMENTS:
                        uiFragmentsCollected = data;
                        return; // Don't save instance if data equals 3
                        break;
                    case DATA_TYRANDE_EVENT:
                        tyrande_event = data;
                        break;
                    case DATA_NOZDORMU_1:
                        nozdormu_dialog[0] = data;
                        if (data == IN_PROGRESS)
                            if (Creature* pImage = instance->GetCreature(uiImageOfNozdormuGUID))
                                pImage->AI()->DoAction(ACTION_TALK_TYRANDE);
                        break;
                    case DATA_NOZDORMU_2:
                        nozdormu_dialog[1] = data;
                        if (data == IN_PROGRESS)
                            if (Creature* pImage = instance->GetCreature(uiImageOfNozdormuGUID))
                                pImage->AI()->DoAction(ACTION_TALK_BAINE);
                        break;
                    case DATA_NOZDORMU_3:
                        nozdormu_dialog[2] = data;
                        if (data == IN_PROGRESS)
                            if (Creature* pImage = instance->GetCreature(uiImageOfNozdormuGUID))
                                pImage->AI()->DoAction(ACTION_TALK_JAINA);
                        break;
                    case DATA_NOZDORMU_4:
                        nozdormu_dialog[3] = data;
                        if (data == IN_PROGRESS)
                            if (Creature* pImage = instance->GetCreature(uiImageOfNozdormuGUID))
                                pImage->AI()->DoAction(ACTION_TALK_SYLVANAS);
                        break;
                    case DATA_PLATFORMS:
                        if (data == NOT_STARTED)
                        {
                            if (GameObject* go = instance->GetGameObject(uiPlatform1GUID))
                                go->SetDestructibleState(GO_DESTRUCTIBLE_INTACT, NULL, true);
                            if (GameObject* go = instance->GetGameObject(uiPlatform2GUID))
                                go->SetDestructibleState(GO_DESTRUCTIBLE_INTACT, NULL, true);
                            if (GameObject* go = instance->GetGameObject(uiPlatform3GUID))
                                go->SetDestructibleState(GO_DESTRUCTIBLE_INTACT, NULL, true);
                            if (GameObject* go = instance->GetGameObject(uiPlatform4GUID))
                                go->SetDestructibleState(GO_DESTRUCTIBLE_INTACT, NULL, true);
                        }
                        break;
                }

                if (data == DONE)
                    SaveToDB();
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_ECHO_1:
                        return first_echo;
                    case DATA_ECHO_2:
                        return second_echo;
                    case DATA_FIRST_ENCOUNTER:
                        return first_encounter;
                    case DATA_SECOND_ENCOUNTER:
                        return second_encounter;
                    case DATA_TYRANDE_EVENT:
                        return tyrande_event;
                    case DATA_JAINA_EVENT:
                        return jaina_event;
                    case DATA_FRAGMENTS:
                        return uiFragmentsCollected;
                    case DATA_NOZDORMU_1:
                        return nozdormu_dialog[0];
                    case DATA_NOZDORMU_2:
                        return nozdormu_dialog[1];
                    case DATA_NOZDORMU_3:
                        return nozdormu_dialog[2];
                    case DATA_NOZDORMU_4:
                        return nozdormu_dialog[3];
                }
                return 0;
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_ECHO_OF_TYRANDE:
                        return uiTyrandeGUID;
                    case DATA_ECHO_OF_JAINA:
                        return uiJainaGUID;
                    case DATA_IMAGE_OF_NOZDORMU:
                        return uiImageOfNozdormuGUID;
                    case DATA_NOZDORMU:
                        return uiNozdormuGUID;
                    case DATA_MUROZOND:
                        return uiMurozondGUID;
                    case DATA_HOURGLASS:
                        return uiHourglassGUID;
                    default:
                        return 0;
                }
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_MUROZOND:
                        if (state == DONE)
                            DoRespawnGameObject(uiMurozondCacheGUID, DAY);
                        break;
                    case DATA_ECHO_OF_JAINA:
                    case DATA_ECHO_OF_BAINE:
                    case DATA_ECHO_OF_TYRANDE:
                    case DATA_ECHO_OF_SYLVANAS:
                        if (state == DONE)
                        {
                            if (first_encounter != DONE)
                                first_encounter = DONE;
                            else
                                second_encounter = DONE;

                            SaveToDB();
                        }
                        break;
                }

                return true;
            }

            void FillInitialWorldStates(WorldStateBuilder& data) OVERRIDE
            {
                data.AppendState(WORLDSTATE_SHOW_FRAGMENTS, jaina_event == IN_PROGRESS);
                data.AppendState(WORLDSTATE_FRAGMENTS_COLLECTED, uiFragmentsCollected);
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::string str_data;

                std::ostringstream saveStream;
                saveStream << "E T " << GetBossSaveData()
                    << first_echo  << ' ' << second_echo << ' '
                    << first_encounter << ' ' << second_encounter << ' '
                    << jaina_event << ' ' << tyrande_event << ' '
                    << nozdormu_dialog[0] << ' ' << nozdormu_dialog[1] << ' '
                    << nozdormu_dialog[2] << ' ' << nozdormu_dialog[3] << ' ';

                str_data = saveStream.str();

                OUT_SAVE_INST_DATA_COMPLETE;
                return str_data;
            }

            void Load(const char* in) OVERRIDE
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

                if (dataHead1 == 'E' && dataHead2 == 'T')
                {
                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }

                    uint32 temp_echo1 = 0;
                    loadStream >> temp_echo1;
                    first_echo = temp_echo1;

                    uint32 temp_echo2 = 0;
                    loadStream >> temp_echo2;
                    second_echo = temp_echo2;

                    uint32 temp_enc1 = 0;
                    loadStream >> temp_enc1;
                    first_encounter = temp_enc1;

                    uint32 temp_enc2 = 0;
                    loadStream >> temp_enc2;
                    second_encounter = temp_enc2;

                    uint32 temp = 0;
                    loadStream >> temp;
                    jaina_event = temp ? DONE : NOT_STARTED;

                    uint32 temp_event = 0;
                    loadStream >> temp_event;
                    if (temp_event == IN_PROGRESS || temp_event > SPECIAL)
                        temp_event = NOT_STARTED;
                    tyrande_event = temp_event;

                    for (uint8 i = 0; i < 4; ++i)
                    {
                        uint32 tmpDialog;
                        loadStream >> tmpDialog;
                        if (tmpDialog == IN_PROGRESS || tmpDialog > SPECIAL)
                            tmpDialog = NOT_STARTED;
                        nozdormu_dialog[i] = tmpDialog;
                    }

                } else OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

            private:
                uint32 uiTeamInInstance;
                uint32 uiFragmentsCollected;
                uint32 jaina_event;
                uint32 first_echo;
                uint32 second_echo;
                uint32 first_encounter;
                uint32 second_encounter;
                uint32 tyrande_event;
                uint32 nozdormu_dialog[4];
                uint64 uiMurozondCacheGUID;
                uint64 uiTyrandeGUID;
                uint64 uiJainaGUID;
                uint64 uiPlatform1GUID;
                uint64 uiPlatform2GUID;
                uint64 uiPlatform3GUID;
                uint64 uiPlatform4GUID;
                uint64 uiImageOfNozdormuGUID;
                uint64 uiMurozondGUID;
                uint64 uiHourglassGUID;
                uint64 uiNozdormuGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_end_time_InstanceMapScript(map);
        }
};

void AddSC_instance_end_time()
{
    new instance_end_time();
}
