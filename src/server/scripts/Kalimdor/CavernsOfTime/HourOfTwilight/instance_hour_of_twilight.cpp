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

#include "hour_of_twilight.h"

#define MAX_ENCOUNTER 3

static const DoorData doordata[] =
{
    { GO_ICEWALL_1, DATA_ARCURION, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { 0,            0,             DOOR_TYPE_ROOM,    BOUNDARY_NONE }
};

class instance_hour_of_twilight : public InstanceMapScript
{
    public:
        instance_hour_of_twilight() : InstanceMapScript("instance_hour_of_twilight", 940) { }

        struct instance_hour_of_twilight_InstanceMapScript : public InstanceScript
        {
            instance_hour_of_twilight_InstanceMapScript(Map* map) : InstanceScript(map) { }

            uint32 m_auiEncounter[MAX_ENCOUNTER];

            void Initialize() OVERRIDE
            {
                SetBossNumber(MAX_ENCOUNTER);
                LoadDoorData(doordata);
                memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
                emptyGUID = 0;
                ArcurionGUID = 0;
                AsiraGUID = 0;
                BenedictusGUID = 0;
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_ICEWALL_1:
                    case GO_ICEWALL_2:
                    case GO_GATE:
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
                    case NPC_ARCURION:
                        ArcurionGUID = creature->GetGUID();
                        break;
                    case NPC_ASIRA:
                        AsiraGUID = creature->GetGUID();
                        break;
                    case NPC_BENEDICTUS:
                        BenedictusGUID = creature->GetGUID();
                        break;
                    case NPC_ICE_WALL_EXIT_STALKER:
                        creature->SetDisplayId(11686);
                        break;
                }
            }

            void SetData(uint32 /*type*/, uint32 /*data*/) OVERRIDE { }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_ARCURION:
                        return ArcurionGUID;
                    case DATA_ASIRA:
                        return AsiraGUID;
                    case DATA_BENEDICTUS:
                        return BenedictusGUID;
                }

                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                return true;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::string str_data;

                std::ostringstream saveStream;
                saveStream << "H o T " << GetBossSaveData();

                str_data = saveStream.str();

                OUT_SAVE_INST_DATA_COMPLETE;
                return str_data;
            }

            bool isWipe()
            {
                Map::PlayerList const& PlayerList = instance->GetPlayers();

                if (!PlayerList.isEmpty())
                {
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                        if (Player* plr = i->GetSource())
                            if (plr->IsAlive() && !plr->IsGameMaster())
                                return false;
                    }
                }
                return true;
            }

            void Load(const char* in) OVERRIDE
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2, dataHead3;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2 >> dataHead3;

                if (dataHead1 == 'H' && dataHead2 == 'o' && dataHead3 == 'T')
                {
                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }
                } else OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

            protected:
                uint64 emptyGUID;
                uint64 ArcurionGUID;
                uint64 AsiraGUID;
                uint64 BenedictusGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_hour_of_twilight_InstanceMapScript(map);
        }
};

void AddSC_instance_hour_of_twilight()
{
    new instance_hour_of_twilight();
}
