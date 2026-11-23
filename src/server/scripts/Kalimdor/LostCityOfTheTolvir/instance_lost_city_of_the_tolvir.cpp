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
                _aughGUID = 0;
                _harbingerGUID = 0;
                _blazeGUID = 0;
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
                    case BOSS_AUGH:
                        _aughGUID = creature->GetGUID();
                        break;
                }
            }

            // Siamat does not fight before the three other bosses are dead
            bool CheckRequiredBosses(uint32 bossId, Player const* /*player*/ = NULL) const OVERRIDE
            {
                if (bossId != DATA_SIAMAT)
                    return true;

                return GetBossState(DATA_GENERAL_HUSAM) == DONE && GetBossState(DATA_LOCKMAW) == DONE && GetBossState(DATA_HIGH_PROPHET_BARIM) == DONE;
            }

            void SetData64(uint32 type, uint64 data) OVERRIDE
            {
                // Barim's Harbinger of Darkness and Blaze of the Heavens register themselves while they exist
                if (type == DATA_HARBINGER)
                    _harbingerGUID = data;
                else if (type == DATA_BLAZE)
                    _blazeGUID = data;
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
                    case DATA_AUGH:
                        return _aughGUID;
                    case DATA_HARBINGER:
                        return _harbingerGUID;
                    case DATA_BLAZE:
                        return _blazeGUID;
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
                uint32 states[MAX_ENCOUNTER] = {};
                if (!(loadStream >> head1 >> head2) || head1 != 'L' || head2 != 'S')
                    return;

                for (uint8 i = 0; i < DATA_AUGH; ++i)
                    if (!(loadStream >> states[i]))
                        return;

                loadStream >> std::ws;
                if (loadStream.peek() != std::char_traits<char>::eof() && !(loadStream >> states[DATA_AUGH]))
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
            uint64 _aughGUID;
            uint64 _harbingerGUID;
            uint64 _blazeGUID;
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
