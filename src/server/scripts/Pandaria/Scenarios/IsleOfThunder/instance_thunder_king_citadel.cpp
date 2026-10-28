/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "thunder_king_citadel.h"
#include "ScenarioMgr.h"

class instance_thunder_king_citadel : public InstanceMapScript
{
    public:
        instance_thunder_king_citadel() : InstanceMapScript("instance_thunder_king_citadel", 1135) { }
    
        struct instance_thunder_king_citadel_InstanceScript : public InstanceScript
        {
            uint32 step1;
            uint32 step2;
            uint32 chestCounter;
            uint64 playerGUID;
            uint64 doorGUID;
            uint32 timeData;
            uint32 goldenChestCount;
            std::list<uint64> firstDoorPackGUIDs;
            std::list<uint64> secondDoorPackGUIDs;
            std::list<uint64> burialThroveGUIDs;
            bool hasGoldenChestAchieved;
    
            instance_thunder_king_citadel_InstanceScript(Map* map) : InstanceScript(map) { }
    
            void Initialize() OVERRIDE
            {
                step1            = 0;
                step2            = 0;
                chestCounter     = 0;
                playerGUID = 0;
                doorGUID = 0;
                timeData         = 0;
                goldenChestCount = 0;
                hasGoldenChestAchieved = false;
                firstDoorPackGUIDs.clear();
                secondDoorPackGUIDs.clear();
                burialThroveGUIDs.clear();
            }

            void OnPlayerEnter(Player* player) OVERRIDE
            {
                if (!playerGUID)
                    playerGUID = player->GetGUID();

                sScenarioMgr->SendScenarioState(player, 1135, GetData(DATA_SPEAK_WITH_TAOSHI) == DONE ? DATA_REACH_THE_EXIT : DATA_SPEAK_WITH_TAOSHI, 0);

                // Apply time on logout
                if (GetData(DATA_SPEAK_WITH_TAOSHI) == DONE)
                    player->CastSpell(player, 140000, true);
            }
    
            void OnCreatureCreate(Creature* creature) OVERRIDE 
            {
                if (creature->GetEntry() == NPC_CLOUD_TILE_TRAP_BUNNY)
                {
                    creature->SetVisible(false);
                    creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                }
            }
    
            void OnGameObjectCreate(GameObject* go) OVERRIDE 
            { 
                switch (go->GetEntry())
                {
                    case GO_ANCIENT_GATE:
                    case GO_ANCIENT_GATE_2:
                        firstDoorPackGUIDs.push_back(go->GetGUID());
                        break;
                    case GO_ANCIENT_GATE_3:
                    case GO_ANCIENT_GATE_4:
                        secondDoorPackGUIDs.push_back(go->GetGUID());
                        break;
                    case GO_ANCIENT_GATE_6:
                        doorGUID = go->GetGUID();
                        break;
                    case GO_LEI_SHEN_BURIAL_THROVE:
                        burialThroveGUIDs.push_back(go->GetGUID());
                        break;
                }
            }
    
            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;
    
                return true;
            }
    
            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_SPEAK_WITH_TAOSHI:
                        step1 = data;
                        HandleGameObject(GetData64(GO_ANCIENT_GATE_6), true, NULL);

                        for (auto&& itr : instance->GetPlayers())
                            if (Player* player = itr.GetSource())
                                sScenarioMgr->SendScenarioState(player, 1135, DATA_REACH_THE_EXIT, 0);
                        break;
                    case DATA_REACH_THE_EXIT:
                        step2 = data;
                        DoFinishLFGDungeon(620);
                        break;
                    case DATA_OPEN_FIRST_DOOR_PACK:
                        for (auto&& itr : firstDoorPackGUIDs)
                            HandleGameObject(itr, true, NULL);
                        break;
                    case DATA_OPEN_SECOND_DOOR_PACK:
                        for (auto&& itr : secondDoorPackGUIDs)
                            HandleGameObject(itr, true, NULL);
                        break;
                    case CHEST_DATA:
                        chestCounter += data;

                        if (Unit* target = ObjectAccessor::FindPlayer(GetData64(PLAYER_DATA)))
                            target->ModifyPower(POWER_ALTERNATE_POWER, data);
                        break;
                    case TIME_DATA:
                        timeData = data;
                        SaveToDB();
                        break;
                    case GOLDEN_CHEST_DATA:
                        if (++goldenChestCount > 9 && !hasGoldenChestAchieved)
                        {
                            hasGoldenChestAchieved = true;

                            if (Player* scenarioOwner = ObjectAccessor::FindPlayer(GetData64(PLAYER_DATA)))
                            {
                                for (uint8 i = 0; i < 10; i++)
                                    scenarioOwner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_LOOTED_GOLDEN_CHEST_ACHIEV); // actually doesn`t work for unk reason
                            }
                        }
                        break;
                }

                if (data == DONE || data == SPECIAL)
                    SaveToDB();
            }
    
            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_SPEAK_WITH_TAOSHI:
                        return step1;
                    case DATA_REACH_THE_EXIT:
                        return step2;
                    case CHEST_DATA:
                        return chestCounter;
                    case TIME_DATA:
                        return timeData;
                }

                return 0;
            }
    
            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case PLAYER_DATA:
                        return playerGUID;
                    case GO_ANCIENT_GATE_6:
                        return doorGUID;
                }

                return 0;
            }
    
            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;
    
                std::ostringstream saveStream;
                saveStream << "T K C " << step1 << ' ' << step2 << ' ' << timeData;
    
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
    
                char dataHead1, dataHead2, dataHead3;
    
                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2 >> dataHead3;
    
                if (dataHead1 == 'T' && dataHead2 == 'K' && dataHead3 == 'C')
                {
                    uint32 tmpState;
                    loadStream >> tmpState;
                    step1 = tmpState;
                    SetData(DATA_SPEAK_WITH_TAOSHI, step1);
                    loadStream >> tmpState;
                    step2 = tmpState;
                    SetData(DATA_REACH_THE_EXIT, step2);
                    loadStream >> tmpState;
                    timeData = tmpState;
                    SetData(TIME_DATA, timeData);
                }
                else OUT_LOAD_INST_DATA_FAIL;
    
                OUT_LOAD_INST_DATA_COMPLETE;
            }
        };
    
        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_thunder_king_citadel_InstanceScript(map);
        }
};

void AddSC_instance_thunder_king_citadel()
{
    new instance_thunder_king_citadel();
}