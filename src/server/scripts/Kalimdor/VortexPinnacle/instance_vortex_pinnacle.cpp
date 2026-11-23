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

#include "ScriptPCH.h"
#include "vortex_pinnacle.h"

class instance_vortex_pinnacle : public InstanceMapScript
{
    public:
        instance_vortex_pinnacle() : InstanceMapScript("instance_vortex_pinnacle", 657) { }

        struct instance_vortex_pinnacle_InstanceMapScript : public InstanceScript
        {
            instance_vortex_pinnacle_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetBossNumber(MAX_ENCOUNTER);
                _ertanGUID = 0;
                _altairusGUID = 0;
                _asaadGUID = 0;
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_GRAND_VIZIER_ERTAN:
                        _ertanGUID = creature->GetGUID();
                        break;
                    case NPC_ALTAIRUS:
                        _altairusGUID = creature->GetGUID();
                        break;
                    case NPC_ASAAD:
                        _asaadGUID = creature->GetGUID();
                        break;
                }
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_ERTAN:
                        return _ertanGUID;
                    case DATA_ALTAIRUS:
                        return _altairusGUID;
                    case DATA_ASAAD:
                        return _asaadGUID;
                    default:
                        return 0;
                }
            }

            std::string GetSaveData() OVERRIDE
            {
                std::ostringstream saveStream;
                saveStream << "V P " << GetBossSaveData();
                return saveStream.str();
            }

            void Load(const char* data) OVERRIDE
            {
                if (!data)
                    return;

                std::istringstream loadStream(data);
                char head1 = 0;
                char head2 = 0;
                uint32 states[MAX_ENCOUNTER];
                if (!(loadStream >> head1 >> head2) || head1 != 'V' || head2 != 'P')
                    return;

                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (!(loadStream >> states[i]))
                        return;

                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    if (states[i] == IN_PROGRESS || states[i] > SPECIAL)
                        states[i] = NOT_STARTED;
                    SetBossState(i, EncounterState(states[i]));
                }
            }

        private:
            uint64 _ertanGUID;
            uint64 _altairusGUID;
            uint64 _asaadGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_vortex_pinnacle_InstanceMapScript(map);
        }
};

void AddSC_instance_vortex_pinnacle()
{
    new instance_vortex_pinnacle();
}
