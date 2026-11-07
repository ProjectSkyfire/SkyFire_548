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

#include "Creature.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "lost_city_of_the_tolvir.h"
#include <sstream>

class instance_lost_city_of_the_tolvir : public InstanceMapScript
{
    public:
        instance_lost_city_of_the_tolvir() : InstanceMapScript("instance_lost_city_of_the_tolvir", 755) { }

        struct instance_lost_city_of_the_tolvir_InstanceMapScript : public InstanceScript
        {
            instance_lost_city_of_the_tolvir_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetBossNumber(MAX_ENCOUNTER);
                _husamGUID = 0;
                _lockmawGUID = 0;
                _barimGUID = 0;
                _siamatGUID = 0;
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case BOSS_GENERAL_HUSAM:
                        _husamGUID = creature->GetGUID();
                        break;
                    case BOSS_LOCKMAW:
                        _lockmawGUID = creature->GetGUID();
                        break;
                    case BOSS_HIGH_PROPHET_BARIM:
                        _barimGUID = creature->GetGUID();
                        break;
                    case BOSS_SIAMAT:
                        _siamatGUID = creature->GetGUID();
                        break;
                }
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_GENERAL_HUSAM:
                        return _husamGUID;
                    case DATA_LOCKMAW:
                        return _lockmawGUID;
                    case DATA_HIGH_PROPHET_BARIM:
                        return _barimGUID;
                    case DATA_SIAMAT:
                        return _siamatGUID;
                    default:
                        return 0;
                }
            }

            std::string GetSaveData() OVERRIDE
            {
                std::ostringstream saveStream;
                saveStream << "L S " << GetBossSaveData();
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
                if (!(loadStream >> head1 >> head2) || head1 != 'L' || head2 != 'S')
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
            uint64 _husamGUID;
            uint64 _lockmawGUID;
            uint64 _barimGUID;
            uint64 _siamatGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_lost_city_of_the_tolvir_InstanceMapScript(map);
        }
};

void AddSC_instance_lost_city_of_the_tolvir()
{
    new instance_lost_city_of_the_tolvir();
}
