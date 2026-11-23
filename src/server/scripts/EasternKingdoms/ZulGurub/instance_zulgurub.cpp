/*
* This file is part of the Legends of Azeroth Pandaria Project. See THANKS file for Copyright information
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
#include "ScriptedCreature.h"
#include "zulgurub.h"

DoorData const doorData[] =
{
    { GO_VENOXIS_EXIT,              DATA_VENOXIS,               DOOR_TYPE_ROOM, BOUNDARY_NONE },
    { GO_MANDOKIR_EXIT1,            DATA_MANDOKIR,              DOOR_TYPE_ROOM, BOUNDARY_NONE },
    { GO_MANDOKIR_EXIT2,            DATA_MANDOKIR,              DOOR_TYPE_ROOM, BOUNDARY_NONE },
    { GO_MANDOKIR_EXIT3,            DATA_MANDOKIR,              DOOR_TYPE_ROOM, BOUNDARY_NONE },
    { GO_MANDOKIR_EXIT4,            DATA_MANDOKIR,              DOOR_TYPE_ROOM, BOUNDARY_NONE },
    { GO_MANDOKIR_EXIT5,            DATA_MANDOKIR,              DOOR_TYPE_ROOM, BOUNDARY_NONE },
    { GO_ZANZIL_EXIT,               DATA_ZANZIL,                DOOR_TYPE_ROOM, BOUNDARY_NONE },
    { GO_KILNARA_EXIT,              DATA_KILNARA,               DOOR_TYPE_ROOM, BOUNDARY_NONE },
    { GO_THE_CACHE_OF_MADNESS_DOOR, DATA_CACHE_OF_MADNESS_BOSS, DOOR_TYPE_ROOM, BOUNDARY_NONE },
    { 0,                            0,                          DOOR_TYPE_ROOM, BOUNDARY_NONE }
};

class instance_zulgurub : public InstanceMapScript
{
    public:
        instance_zulgurub() : InstanceMapScript("instance_zulgurub", 859) { }

        struct instance_zulgurub_InstanceMapScript : public InstanceScript
        {
            instance_zulgurub_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);

                venoxisGUID = 0;
                mandokirGUID = 0;
                kilnaraGUID = 0;
                zanzilGUID = 0;
                jindoGUID = 0;
                cacheOfMadnessBossGUID = 0;
                cacheOfMadnessSummonerGUID = 0;
                artifactsActivated = 0;
                bossesDone = 0;

                for (uint8 i = 0; i < ARTIFACT_TYPE_COUNT; ++i)
                    chosenArtifactGUID[i] = 0;
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_VENOXIS:
                        venoxisGUID = creature->GetGUID();
                        break;
                    case NPC_MANDOKIR:
                        mandokirGUID = creature->GetGUID();
                        break;
                    case NPC_KILNARA:
                        kilnaraGUID = creature->GetGUID();
                        break;
                    case NPC_ZANZIL:
                        zanzilGUID = creature->GetGUID();
                        break;
                    case NPC_JINDO:
                        jindoGUID = creature->GetGUID();
                        break;
                    case NPC_HAZZARAH:
                    case NPC_RENATAKI:
                    case NPC_WUSHOOLAY:
                    case NPC_GRILEK:
                        cacheOfMadnessBossGUID = creature->GetGUID();
                        break;
                    case NPC_CACHE_OF_MADNESS_SUMMONER:
                        if (creature->GetDBTableGUIDLow() == CACHE_OF_MADNESS_SUMMONER_GUID)
                            cacheOfMadnessSummonerGUID = creature->GetGUID();
                        break;
                    case NPC_ARTIFACT_DWARVEN:
                    case NPC_ARTIFACT_ELVEN:
                    case NPC_ARTIFACT_TROLL:
                    case NPC_ARTIFACT_FOSSIL:
                        for (uint8 i = 0; i < ARTIFACT_TYPE_COUNT; ++i)
                            if (creature->GetEntry() == ARTIFACT_ENTRIES[i])
                                artifactGUIDs[i].push_back(creature->GetGUID());
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_VENOXIS_EXIT:
                    case GO_MANDOKIR_EXIT1:
                    case GO_MANDOKIR_EXIT2:
                    case GO_MANDOKIR_EXIT3:
                    case GO_MANDOKIR_EXIT4:
                    case GO_MANDOKIR_EXIT5:
                    case GO_ZANZIL_EXIT:
                    case GO_KILNARA_EXIT:
                    case GO_THE_CACHE_OF_MADNESS_DOOR:
                        AddDoor(go, true);
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_VENOXIS_EXIT:
                    case GO_MANDOKIR_EXIT1:
                    case GO_MANDOKIR_EXIT2:
                    case GO_MANDOKIR_EXIT3:
                    case GO_MANDOKIR_EXIT4:
                    case GO_MANDOKIR_EXIT5:
                    case GO_ZANZIL_EXIT:
                    case GO_KILNARA_EXIT:
                    case GO_THE_CACHE_OF_MADNESS_DOOR:
                        AddDoor(go, false);
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                if (state == DONE)
                {
                    ++bossesDone;
                    SaveToDB();
                }

                return true;
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_ARTIFACTS_ACTIVATED:
                        artifactsActivated = data;
                        if (artifactsActivated == ARTIFACT_TYPE_COUNT)
                        {
                            EncounterState state = GetBossState(DATA_CACHE_OF_MADNESS_BOSS);
                            if (state == NOT_STARTED || state == TO_BE_DECIDED)
                                events.ScheduleEvent(EVENT_CACHE_OF_MADNESS_START_SUMMON, 5000);
                        }
                        break;
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_BOSSES:
                        return bossesDone;
                    case DATA_ARTIFACTS_ACTIVATED:
                        return artifactsActivated;
                    default:
                        break;
                }

                return 0;
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_VENOXIS:
                        return venoxisGUID;
                    case DATA_MANDOKIR:
                        return mandokirGUID;
                    case DATA_KILNARA:
                        return kilnaraGUID;
                    case DATA_ZANZIL:
                        return zanzilGUID;
                    case DATA_JINDO:
                        return jindoGUID;
                    case DATA_CACHE_OF_MADNESS_BOSS:
                        return cacheOfMadnessBossGUID;
                    case DATA_ARTIFACT_DWARVEN:
                    case DATA_ARTIFACT_ELVEN:
                    case DATA_ARTIFACT_TROLL:
                    case DATA_ARTIFACT_FOSSIL:
                        return GetChosenArtifact(type - DATA_ARTIFACT_DWARVEN);
                    default:
                        break;
                }

                return 0;
            }

            void Update(uint32 diff) OVERRIDE
            {
                if (events.Empty())
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CACHE_OF_MADNESS_START_SUMMON:
                        if (Creature* summoner = instance->GetCreature(cacheOfMadnessSummonerGUID))
                        {
                            summoner->CastSpell(summoner, CACHE_OF_MADNESS_SUMMON_VISUAL, false);
                            events.ScheduleEvent(EVENT_CACHE_OF_MADNESS_SHATTER_ARTIFACTS, 10000);
                            events.ScheduleEvent(EVENT_CACHE_OF_MADNESS_SUMMON, 17000);
                        }
                        break;
                    case EVENT_CACHE_OF_MADNESS_SHATTER_ARTIFACTS:
                        if (Creature* summoner = instance->GetCreature(cacheOfMadnessSummonerGUID))
                        {
                            uint32 const active[ARTIFACT_TYPE_COUNT] =
                            {
                                NPC_ARTIFACT_ACTIVE_DWARVEN, NPC_ARTIFACT_ACTIVE_ELVEN, NPC_ARTIFACT_ACTIVE_TROLL, NPC_ARTIFACT_ACTIVE_FOSSIL
                            };

                            for (uint8 i = 0; i < ARTIFACT_TYPE_COUNT; ++i)
                            {
                                std::list<Creature*> artifacts;
                                summoner->GetCreatureListWithEntryInGrid(artifacts, active[i], 50.0f);
                                for (Creature* artifact : artifacts)
                                {
                                    artifact->CastSpell(artifact, CACHE_OF_MADNESS_SHATTER_ARTIFACT, false);
                                    artifact->DespawnOrUnsummon(1500);
                                }
                            }
                        }
                        break;
                    case EVENT_CACHE_OF_MADNESS_SUMMON:
                        if (Creature* summoner = instance->GetCreature(cacheOfMadnessSummonerGUID))
                        {
                            summoner->SummonCreature(CACHE_OF_MADNESS_BOSSES[urand(0, CACHE_OF_MADNESS_BOSSES_COUNT - 1)], summoner->GetPositionX(), summoner->GetPositionY(), summoner->GetPositionZ(), summoner->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            events.ScheduleEvent(EVENT_CACHE_OF_MADNESS_HIDE_PORTAL, 5000);
                        }
                        break;
                    case EVENT_CACHE_OF_MADNESS_HIDE_PORTAL:
                        if (Creature* summoner = instance->GetCreature(cacheOfMadnessSummonerGUID))
                            summoner->RemoveAurasDueToSpell(CACHE_OF_MADNESS_SUMMON_VISUAL);
                        break;
                    default:
                        break;
                }
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "Z G " << bossesDone << ' ' << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* str) OVERRIDE
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

                if (dataHead1 == 'Z' && dataHead2 == 'G')
                {
                    loadStream >> bossesDone;
                    if (bossesDone > EncounterCount)
                        bossesDone = 0;

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
            // One artifact of each kind is the one a player can activate; it is picked once, at random
            uint64 GetChosenArtifact(uint8 type) const
            {
                if (type >= ARTIFACT_TYPE_COUNT)
                    return 0;

                if (!chosenArtifactGUID[type] && !artifactGUIDs[type].empty())
                    chosenArtifactGUID[type] = artifactGUIDs[type][urand(0, artifactGUIDs[type].size() - 1)];

                return chosenArtifactGUID[type];
            }

            EventMap events;
            uint64 venoxisGUID;
            uint64 mandokirGUID;
            uint64 kilnaraGUID;
            uint64 zanzilGUID;
            uint64 jindoGUID;
            uint64 cacheOfMadnessBossGUID;
            uint64 cacheOfMadnessSummonerGUID;
            std::vector<uint64> artifactGUIDs[ARTIFACT_TYPE_COUNT];
            mutable uint64 chosenArtifactGUID[ARTIFACT_TYPE_COUNT];
            uint32 artifactsActivated;
            uint32 bossesDone;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_zulgurub_InstanceMapScript(map);
        }
};

void AddSC_instance_zulgurub()
{
    new instance_zulgurub();
}
