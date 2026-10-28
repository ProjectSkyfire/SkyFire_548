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
#include "InstanceScript.h"
#include "GameObject.h"
#include "scarlet_halls.h"

static DoorData const doorData[] =
{
    { GO_HOUNDMASTER_BRAUN_EXIT, BOSS_HOUNDMASTER_BRAUN, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_ARMSMASTER_HARLAN_EXIT, BOSS_ARMSMASTER_HARLAN, DOOR_TYPE_ROOM, BOUNDARY_NONE },
    { 0, 0, DOOR_TYPE_ROOM, BOUNDARY_NONE }
};

class instance_scarlet_halls : public InstanceMapScript
{
public:
    instance_scarlet_halls() : InstanceMapScript("instance_scarlet_halls", 1001) { }

    struct instance_scarlet_halls_InstanceMapScript : public InstanceScript
    {
        instance_scarlet_halls_InstanceMapScript(Map* map) : InstanceScript(map), lindonState(NOT_STARTED), lindonDoor(0)
        {
            for (uint8 i = 0; i < ENCOUNTER_COUNT; ++i)
                bosses[i] = 0;
        }

        uint64 bosses[ENCOUNTER_COUNT];
        uint64 lindonGuid = 0;
        uint32 lindonState;
        uint64 lindonDoor;

        void Initialize() OVERRIDE
        {
            SetBossNumber(ENCOUNTER_COUNT);
            LoadDoorData(doorData);
            instance->SetWorldState(WORLDSTATE_HUMANE_SOCIETY, 1);
            // Challenge mode needs separate support in SkyFire.
        }

        void OnCreatureCreate(Creature* creature) OVERRIDE
        {
            switch (creature->GetEntry())
            {
                case NPC_HOUNDMASTER_BRAUN: bosses[0] = creature->GetGUID(); break;
                case NPC_ARMSMASTER_HARLAN: bosses[1] = creature->GetGUID(); break;
                case NPC_FLAMEWEAVER_KOEGLER: bosses[2] = creature->GetGUID(); break;
                case NPC_COMANDER_LINDON: lindonGuid = creature->GetGUID(); break;
                case NPC_SCARLET_GUARDIAN:
                case NPC_SERGEANT_VERDONE: creature->SetReactState(REACT_PASSIVE); break;
            }
        }

        void OnGameObjectCreate(GameObject* go) OVERRIDE
        {
            switch (go->GetEntry())
            {
                case GO_HOUNDMASTER_BRAUN_EXIT:
                case GO_ARMSMASTER_HARLAN_EXIT: AddDoor(go, true); break;
                case GO_COMANDER_LINDON_EXIT:
                    lindonDoor = go->GetGUID();
                    HandleGameObject(lindonDoor, lindonState == DONE, go);
                    break;
            }
        }

        void OnGameObjectRemove(GameObject* go) OVERRIDE
        {
            if (go->GetEntry() == GO_HOUNDMASTER_BRAUN_EXIT || go->GetEntry() == GO_ARMSMASTER_HARLAN_EXIT)
                AddDoor(go, false);
            else if (go->GetEntry() == GO_COMANDER_LINDON_EXIT)
                lindonDoor = 0;
        }

        void SetData(uint32 type, uint32 data) OVERRIDE
        {
            if (type == DATA_COMANDER_LINDON)
            {
                lindonState = data;
                HandleGameObject(lindonDoor, data == DONE);
                if (data == DONE)
                    SaveToDB();
            }
        }

        uint32 GetData(uint32 type) const OVERRIDE { return type == DATA_COMANDER_LINDON ? lindonState : 0; }
        uint64 GetData64(uint32 type) const OVERRIDE
        {
            if (type < ENCOUNTER_COUNT)
                return bosses[type];
            return type == DATA_COMANDER_LINDON ? lindonGuid : 0;
        }

        std::string GetSaveData() OVERRIDE
        {
            std::ostringstream out;
            out << "S H " << GetBossSaveData() << lindonState;
            return out.str();
        }

        void Load(char const* data) OVERRIDE
        {
            if (!data)
                return;
            std::istringstream in(data);
            char a, b;
            if (!(in >> a >> b) || a != 'S' || b != 'H')
                return;
            for (uint8 i = 0; i < ENCOUNTER_COUNT; ++i)
            {
                uint32 state;
                if (!(in >> state))
                    return;
                if (state == IN_PROGRESS || state > SPECIAL)
                    state = NOT_STARTED;
                SetBossState(i, EncounterState(state));
            }
            uint32 state;
            if (in >> state)
                lindonState = state == DONE ? DONE : NOT_STARTED;
        }
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
