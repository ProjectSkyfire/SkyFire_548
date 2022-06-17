/*
 * Copyright (C) 2011-2021 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2021 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2021 MaNGOS <https://www.getmangos.eu/>
 * Copyright (C) 2006-2014 ScriptDev2 <https://github.com/scriptdev2/scriptdev2/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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
#include "temple_of_jade_serpent.h"

class instance_jade_serpent : public InstanceMapScript
{
    public:
        instance_jade_serpent() : InstanceMapScript(JadeSerpentScriptName, 960) { }

        struct instance_jade_serpent_InstanceScript : public InstanceScript
        {
            instance_jade_serpent_InstanceScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                //LoadDoorData(doorData);

                WiseMariGUID        = 0;
                StonestepGUID       = 0;
                FlameheartGUID      = 0;
                ShaOfDoubtGUID      = 0;
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch (creature->GetEntry())
                {
                    case NPC_WISE_MARI:
                        WiseMariGUID = creature->GetGUID();
                        break;
                    case NPC_STONESTEP:
                        StonestepGUID = creature->GetGUID();
                        break;
                    case NPC_FLAMEHEART:
                        FlameheartGUID = creature->GetGUID();
                        break;
                    case NPC_SHA_OF_DOUBT:
                        ShaOfDoubtGUID = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            //void OnGameObjectCreate(GameObject* go) override
            //{}

            //void OnGameObjectRemove(GameObject* go) override
            //{}

            uint64 GetData64(uint32 type) const override
            {
                switch (type)
                {
                    case DATA_WISE_MARI:
                        return WiseMariGUID;
                    case DATA_STONESTEP:
                        return StonestepGUID;
                    case DATA_FLAMEHEART:
                        return FlameheartGUID;
                    case DATA_SHA_OF_DOUBT:
                        return ShaOfDoubtGUID;
                    default:
                        break;
                }
                return 0;
            }

            std::string GetSaveData() override
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "T J S " << GetBossSaveData();

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

                char dataHead1, dataHead2, dataHead3;

                std::istringstream loadStream(str);
                loadStream >> dataHead1 >> dataHead2 >> dataHead3;

                if (dataHead1 == 'T' && dataHead2 == 'J' && dataHead3 == 'S')
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

        protected:
            uint64 WiseMariGUID;
            uint64 StonestepGUID;
            uint64 FlameheartGUID;
            uint64 ShaOfDoubtGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_jade_serpent_InstanceScript(map);
        }
};

void AddSC_instance_jade_serpent()
{
   new instance_jade_serpent();
}
