/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "GameObject.h"
#include "Player.h"
#include "SpellAuras.h"
#include "terrace_of_endless_spring.h"

#include <cstring>
#include <sstream>
#include <unordered_map>
#include <vector>

class instance_terrace_of_endless_spring : public InstanceMapScript
{
    public:
        instance_terrace_of_endless_spring()
            : InstanceMapScript("instance_terrace_of_endless_spring", 996) { }

        struct instance_terrace_of_endless_spring_InstanceScript : public InstanceScript
        {
            instance_terrace_of_endless_spring_InstanceScript(Map* map) : InstanceScript(map) { }

            EventMap events;
            uint32 encounters[MAX_TYPES];
            bool ritualOfPurification;
            bool introDone;
            bool loading;
            uint32 leiShiProtectorsCount;

            std::vector<uint64> protectorGuids;
            std::vector<uint64> animatedGuids;
            std::vector<uint64> returnTerraceGuids;
            std::unordered_map<uint32, uint64> gameObjectGuids;

            uint64 ancientRegailGuid;
            uint64 ancientAsaniGuid;
            uint64 protectorKaolanGuid;
            uint64 minionOfFearControllerGuid;
            uint64 tsulongGuid;
            uint64 enchantedPlantGuid;
            uint64 leiShiGuid;
            uint64 leiShiReflectionGuid;
            uint64 shaOfFearGuid;
            uint64 pureLightTerraceGuid;
            uint64 shaVortexGuid;
            uint64 shaVortexWallGuid;
            uint64 wallOfCouncilsVortexGuid;
            uint64 councilsVortexGuid;
            uint64 wallOfLeiShisVortexGuid;
            uint64 leiShisVortexGuid;

            void Initialize() OVERRIDE
            {
                SetBossNumber(DATA_MAX_BOSS_DATA);
                for (uint32 i = 0; i < DATA_MAX_BOSS_DATA; ++i)
                    InstanceScript::SetBossState(i, NOT_STARTED);

                events.Reset();
                std::memset(encounters, 0, sizeof(encounters));
                ritualOfPurification = false;
                introDone = false;
                loading = false;
                leiShiProtectorsCount = 0;

                protectorGuids.clear();
                animatedGuids.clear();
                returnTerraceGuids.clear();
                gameObjectGuids.clear();

                ancientRegailGuid = 0;
                ancientAsaniGuid = 0;
                protectorKaolanGuid = 0;
                minionOfFearControllerGuid = 0;
                tsulongGuid = 0;
                enchantedPlantGuid = 0;
                leiShiGuid = 0;
                leiShiReflectionGuid = 0;
                shaOfFearGuid = 0;
                pureLightTerraceGuid = 0;
                shaVortexGuid = 0;
                shaVortexWallGuid = 0;
                wallOfCouncilsVortexGuid = 0;
                councilsVortexGuid = 0;
                wallOfLeiShisVortexGuid = 0;
                leiShisVortexGuid = 0;
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_RETURN_TO_TERRACE:
                        returnTerraceGuids.push_back(creature->GetGUID());
                        break;
                    case NPC_SLG_GENERIC_MOP:
                    case NPC_DEFILED_GROUND:
                        creature->SetDisplayId(11686);
                        break;
                    case NPC_ANCIENT_ASANI:
                        ancientAsaniGuid = creature->GetGUID();
                        break;
                    case NPC_ANCIENT_REGAIL:
                        ancientRegailGuid = creature->GetGUID();
                        break;
                    case NPC_PROTECTOR_KAOLAN:
                        protectorKaolanGuid = creature->GetGUID();
                        break;
                    case NPC_MINION_OF_FEAR_CONTROLLER:
                        minionOfFearControllerGuid = creature->GetGUID();
                        break;
                    case NPC_TSULONG:
                        tsulongGuid = creature->GetGUID();
                        break;
                    case NPC_LEI_SHI:
                        leiShiGuid = creature->GetGUID();
                        instance->LoadGrid(creature->GetPositionX(), creature->GetPositionY());
                        creature->SetDisplayId(48588);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                        creature->SetReactState(REACT_PASSIVE);
                        break;
                    case NPC_SHA_OF_FEAR:
                        shaOfFearGuid = creature->GetGUID();
                        break;
                    case NPC_PURE_LIGHT_TERRACE:
                        pureLightTerraceGuid = creature->GetGUID();
                        creature->SetDisplayId(11686);
                        break;
                    case NPC_CORRUPTED_PROTECTOR:
                        creature->SetVisible(false);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                        protectorGuids.push_back(creature->GetGUID());
                        break;
                    case NPC_ANIMATED_PROTECTOR:
                        creature->SetVisible(false);
                        animatedGuids.push_back(creature->GetGUID());
                        break;
                    case NPC_REFLECTION_OF_LEI_SHI:
                        leiShiReflectionGuid = creature->GetGUID();
                        creature->SetDisplayId(42960);
                        break;
                    case NPC_LIGHT_OF_DAY:
                        creature->SetDisplayId(42960);
                        break;
                    case NPC_ENCHANTED_PLANT:
                        enchantedPlantGuid = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_COUNCILS_VORTEX:
                        councilsVortexGuid = go->GetGUID();
                        break;
                    case GO_WALL_OF_COUNCILS_VORTEX:
                        wallOfCouncilsVortexGuid = go->GetGUID();
                        break;
                    case GO_WALL_OF_LEI_SHI:
                        wallOfLeiShisVortexGuid = go->GetGUID();
                        break;
                    case GO_LEI_SHIS_VORTEX:
                        leiShisVortexGuid = go->GetGUID();
                        break;
                    case GO_SHA_VORTEX:
                        shaVortexGuid = go->GetGUID();
                        break;
                    case GO_SHA_VORTEX_WALL:
                        shaVortexWallGuid = go->GetGUID();
                        break;
                    case GO_TSULONG_CHEST_LFR:
                    case GO_TSULONG_CHEST_NORMAL:
                    case GO_TSULONG_CHEST_HEROIC:
                    case GO_LEI_SHI_CHEST_NORMAL:
                    case GO_LEI_SHI_CHEST_HEROIC:
                        gameObjectGuids.insert(std::make_pair(go->GetEntry(), go->GetGUID()));
                        break;
                    default:
                        break;
                }
            }

            GameObject* GetGameObjectFromStorage(uint32 entry)
            {
                std::unordered_map<uint32, uint64>::const_iterator itr = gameObjectGuids.find(entry);
                if (itr != gameObjectGuids.end())
                    return instance->GetGameObject(itr->second);

                return NULL;
            }

            void OnUnitDeath(Unit* unit) OVERRIDE
            {
                if (!unit)
                    return;

                if (unit->ToPlayer() && unit->HasAura(120268))
                {
                    unit->RemoveAurasDueToSpell(120268);

                    if (Creature* sha = instance->GetCreature(shaOfFearGuid))
                        sha->AI()->DoAction(4);
                }

                if (unit->ToCreature() && unit->GetEntry() == NPC_CORRUPTED_PROTECTOR &&
                    GetData(TYPE_LEI_INTRO) != DONE)
                {
                    ++leiShiProtectorsCount;
                    if (leiShiProtectorsCount == 10)
                        SetData(TYPE_LEI_INTRO, DONE);
                }
            }

            void HandleActivateProtectors()
            {
                for (std::vector<uint64>::const_iterator itr = protectorGuids.begin(); itr != protectorGuids.end(); ++itr)
                {
                    if (Creature* creature = instance->GetCreature(*itr))
                    {
                        creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                        creature->SetVisible(true);
                    }
                }
            }

            void ActivateReturnToTerrace()
            {
                for (std::vector<uint64>::const_iterator itr = returnTerraceGuids.begin(); itr != returnTerraceGuids.end(); ++itr)
                {
                    if (Creature* creature = instance->GetCreature(*itr))
                    {
                        creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        creature->UpdateObjectVisibility();
                        creature->AddAura(120216, creature);
                    }
                }
            }

            void RemoveAnimatedProtectors()
            {
                for (std::vector<uint64>::const_iterator itr = animatedGuids.begin(); itr != animatedGuids.end(); ++itr)
                {
                    if (Creature* creature = instance->GetCreature(*itr))
                    {
                        if (creature->IsInWorld())
                            creature->AddObjectToRemoveList();
                    }
                }
            }

            void FinishProtectorsIntro()
            {
                if (Creature* asani = instance->GetCreature(ancientAsaniGuid))
                    asani->AI()->DoAction(ACTION_INTRO_FINISHED);

                if (Creature* regail = instance->GetCreature(ancientRegailGuid))
                    regail->AI()->DoAction(ACTION_INTRO_FINISHED);

                if (Creature* kaolan = instance->GetCreature(protectorKaolanGuid))
                    kaolan->AI()->DoAction(ACTION_INTRO_FINISHED);
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                if (loading)
                    return true;

                if (type == DATA_PROTECTORS && state == DONE)
                {
                    if (Creature* creature = instance->GetCreature(tsulongGuid))
                    {
                        instance->LoadGrid(creature->GetPositionX(), creature->GetPositionY());
                        creature->AI()->DoAction(ACTION_START_TSULONG_WAYPOINT);
                    }
                }

                if (type == DATA_TSULONG && state == DONE)
                {
                    uint32 chest = instance->GetDifficulty() == DIFFICULTY_10MAN_HEROIC ||
                        instance->GetDifficulty() == DIFFICULTY_25MAN_HEROIC ?
                        GO_TSULONG_CHEST_HEROIC : GO_TSULONG_CHEST_NORMAL;

                    if (GameObject* go = GetGameObjectFromStorage(chest))
                        DoRespawnGameObject(go->GetGUID(), 7 * DAY);

                    HandleActivateProtectors();
                }

                if (type == DATA_LEI_SHI && state == DONE)
                {
                    uint32 chest = instance->GetDifficulty() == DIFFICULTY_10MAN_HEROIC ||
                        instance->GetDifficulty() == DIFFICULTY_25MAN_HEROIC ?
                        GO_LEI_SHI_CHEST_HEROIC : GO_LEI_SHI_CHEST_NORMAL;

                    if (GameObject* go = GetGameObjectFromStorage(chest))
                        DoRespawnGameObject(go->GetGUID(), 7 * DAY);
                }

                if (type < MAX_TYPES && state == DONE)
                    SetData(type, uint32(state));

                return true;
            }

            void Update(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case 1:
                            if (Creature* creature = instance->GetCreature(tsulongGuid))
                            {
                                instance->LoadGrid(creature->GetPositionX(), creature->GetPositionY());
                                creature->AI()->DoAction(ACTION_START_TSULONG_WAYPOINT);
                            }
                            break;
                        case 2:
                            if (Creature* sha = instance->GetCreature(shaOfFearGuid))
                            {
                                if (!sha->AI()->GetData(TYPE_DREAD_TERRACE))
                                {
                                    Map::PlayerList const& players = instance->GetPlayers();
                                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                                    {
                                        if (Player* player = itr->GetSource())
                                        {
                                            if (!player->HasAura(SPELL_LEIS_HOPE))
                                            {
                                                if (Aura* aura = player->AddAura(SPELL_LEIS_HOPE, player))
                                                {
                                                    aura->SetMaxDuration(3 * HOUR * IN_MILLISECONDS);
                                                    aura->SetDuration(3 * HOUR * IN_MILLISECONDS);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            events.ScheduleEvent(2, 40000);
                            break;
                        case 3:
                            if (Creature* creature = instance->GetCreature(tsulongGuid))
                            {
                                instance->LoadGrid(creature->GetPositionX(), creature->GetPositionY());
                                creature->SetDisplayId(17612);
                                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                            }
                            break;
                        case 4:
                            if (Creature* creature = instance->GetCreature(leiShiGuid))
                            {
                                instance->LoadGrid(creature->GetPositionX(), creature->GetPositionY());
                                creature->AI()->DoAction(ACTION_LEISHI_INTRO);
                            }
                            break;
                        case 5:
                            if (GameObject* wall = instance->GetGameObject(wallOfCouncilsVortexGuid))
                                wall->SetGoState(GOState::GO_STATE_ACTIVE);

                            if (GameObject* vortex = instance->GetGameObject(councilsVortexGuid))
                                vortex->SetGoState(GOState::GO_STATE_ACTIVE);
                            break;
                        case 6:
                            FinishProtectorsIntro();
                            break;
                        case 7:
                            RemoveAnimatedProtectors();

                            if (Creature* leiShi = instance->GetCreature(leiShiGuid))
                            {
                                leiShi->setFaction(35);
                                leiShi->SetVisible(false);

                                if (Creature* reflection = instance->GetCreature(leiShiReflectionGuid))
                                    reflection->SetVisible(false);

                                if (GameObject* vortex = instance->GetGameObject(leiShisVortexGuid))
                                    vortex->SetGoState(GOState::GO_STATE_ACTIVE);

                                if (GameObject* wall = instance->GetGameObject(wallOfLeiShisVortexGuid))
                                    wall->SetGoState(GOState::GO_STATE_ACTIVE);
                            }

                            if (Creature* sha = instance->GetCreature(shaOfFearGuid))
                            {
                                instance->LoadGrid(sha->GetPositionX(), sha->GetPositionY());
                                sha->AI()->DoAction(ACTION_SHA_INTRO);
                            }
                            break;
                        case 9:
                            ActivateReturnToTerrace();
                            break;
                        default:
                            break;
                    }
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                if (type >= MAX_TYPES)
                {
                    if (type == SPELL_RITUAL_OF_PURIFICATION)
                        ritualOfPurification = data != 0;
                    return;
                }

                switch (type)
                {
                    case TYPE_PROTECTORS:
                    case TYPE_TSULONG:
                        encounters[type] = data;
                        break;
                    case TYPE_SHA:
                        ActivateReturnToTerrace();
                        encounters[type] = data;
                        break;
                    case TYPE_LEI_SHI:
                        if (encounters[type] == data)
                            return;

                        RemoveAnimatedProtectors();
                        encounters[type] = data;
                        break;
                    case TYPE_LEIS_HOPE:
                        if (data == DONE)
                            events.ScheduleEvent(2, 200);
                        encounters[type] = data;
                        break;
                    case TYPE_LEI_INTRO:
                        if (data == DONE)
                        {
                            for (std::vector<uint64>::const_iterator itr = animatedGuids.begin(); itr != animatedGuids.end(); ++itr)
                            {
                                if (Creature* creature = instance->GetCreature(*itr))
                                    creature->SetVisible(true);
                            }

                            if (Creature* reflection = instance->GetCreature(leiShiReflectionGuid))
                            {
                                if (reflection->AI())
                                    reflection->AI()->DoAction(ACTION_LEISHI_INTRO);
                            }
                        }
                        else if (data == SPECIAL)
                            HandleActivateProtectors();

                        encounters[type] = data;
                        break;
                    case INTRO_DONE:
                        if (data == DONE)
                            FinishProtectorsIntro();

                        introDone = data == DONE;
                        encounters[type] = data;
                        break;
                    default:
                        break;
                }

                if (data >= DONE && !loading)
                    SaveToDB();
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case SPELL_RITUAL_OF_PURIFICATION:
                        return ritualOfPurification;
                    case INTRO_DONE:
                        return introDone;
                    case TYPE_PROTECTORS:
                    case TYPE_TSULONG:
                    case TYPE_LEI_SHI:
                    case TYPE_SHA:
                    case TYPE_LEIS_HOPE:
                    case TYPE_LEI_INTRO:
                        return encounters[type];
                    default:
                        return 0;
                }
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case NPC_ANCIENT_ASANI:
                        return ancientAsaniGuid;
                    case NPC_ANCIENT_REGAIL:
                        return ancientRegailGuid;
                    case NPC_PROTECTOR_KAOLAN:
                        return protectorKaolanGuid;
                    case NPC_MINION_OF_FEAR_CONTROLLER:
                        return minionOfFearControllerGuid;
                    case NPC_TSULONG:
                        return tsulongGuid;
                    case NPC_LEI_SHI:
                        return leiShiGuid;
                    case NPC_REFLECTION_OF_LEI_SHI:
                        return leiShiReflectionGuid;
                    case NPC_SHA_OF_FEAR:
                        return shaOfFearGuid;
                    case NPC_PURE_LIGHT_TERRACE:
                        return pureLightTerraceGuid;
                    case NPC_ENCHANTED_PLANT:
                        return enchantedPlantGuid;
                    case GO_COUNCILS_VORTEX:
                        return councilsVortexGuid;
                    case GO_WALL_OF_COUNCILS_VORTEX:
                        return wallOfCouncilsVortexGuid;
                    case GO_WALL_OF_LEI_SHI:
                        return wallOfLeiShisVortexGuid;
                    case GO_LEI_SHIS_VORTEX:
                        return leiShisVortexGuid;
                    case GO_SHA_VORTEX:
                        return shaVortexGuid;
                    case GO_SHA_VORTEX_WALL:
                        return shaVortexWallGuid;
                    default:
                        return 0;
                }
            }

            std::string GetSaveData() OVERRIDE
            {
                if (loading)
                    return "";

                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "T E " << GetBossSaveData();
                for (uint32 i = 0; i < MAX_TYPES; ++i)
                    saveStream << encounters[i] << ' ';

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

                char dataHead1 = 0;
                char dataHead2 = 0;
                uint32 bossStates[DATA_MAX_BOSS_DATA];
                uint32 savedEncounters[MAX_TYPES];
                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 != 'T' || dataHead2 != 'E')
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                for (uint32 i = 0; i < DATA_MAX_BOSS_DATA; ++i)
                {
                    if (!(loadStream >> bossStates[i]))
                    {
                        OUT_LOAD_INST_DATA_FAIL;
                        return;
                    }

                    if (bossStates[i] == IN_PROGRESS || bossStates[i] > SPECIAL)
                        bossStates[i] = NOT_STARTED;
                }

                for (uint32 i = 0; i < MAX_TYPES; ++i)
                {
                    if (!(loadStream >> savedEncounters[i]))
                    {
                        OUT_LOAD_INST_DATA_FAIL;
                        return;
                    }

                    if (savedEncounters[i] == IN_PROGRESS)
                        savedEncounters[i] = NOT_STARTED;
                }

                loading = true;
                for (uint32 i = 0; i < MAX_TYPES; ++i)
                    encounters[i] = savedEncounters[i];

                for (uint32 i = 0; i < DATA_MAX_BOSS_DATA; ++i)
                    SetBossState(i, EncounterState(bossStates[i]));

                if (encounters[TYPE_PROTECTORS] == DONE)
                {
                    if (encounters[TYPE_TSULONG] == DONE)
                        events.ScheduleEvent(3, 200);
                    else
                        events.ScheduleEvent(1, 200);
                }

                if (encounters[TYPE_LEIS_HOPE] == DONE &&
                    (instance->GetDifficulty() == DIFFICULTY_10MAN_HEROIC ||
                     instance->GetDifficulty() == DIFFICULTY_25MAN_HEROIC))
                    events.ScheduleEvent(2, 200);

                if (encounters[TYPE_LEI_INTRO] == DONE && encounters[TYPE_LEI_SHI] != DONE)
                    SetData(TYPE_LEI_INTRO, DONE);

                if (encounters[INTRO_DONE] == DONE)
                {
                    events.ScheduleEvent(5, 200);
                    if (encounters[TYPE_PROTECTORS] != DONE)
                        events.ScheduleEvent(6, 200);
                }

                if (encounters[TYPE_LEI_SHI] == DONE)
                    events.ScheduleEvent(7, 200);

                if (encounters[TYPE_SHA] == DONE)
                    events.ScheduleEvent(9, 200);

                loading = false;
                OUT_LOAD_INST_DATA_COMPLETE;
            }

            bool IsWipe(float range, Unit* source) OVERRIDE
            {
                Map::PlayerList const& players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* player = itr->GetSource())
                    {
                        if (source && player->GetExactDist2d(source) > range)
                            continue;

                        if (player->IsAlive() && !player->IsGameMaster())
                            return false;
                    }
                }

                return true;
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_terrace_of_endless_spring_InstanceScript(map);
        }
};

void AddSC_instance_terrace_of_endless_spring()
{
    new instance_terrace_of_endless_spring();
}
