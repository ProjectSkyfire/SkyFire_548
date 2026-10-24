/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "GameObject.h"
#include "Player.h"
#include "MoveSplineInit.h"
#include "Containers.h"
#include "mogu_shan_palace.h"

#include <numeric>
#include <random>

class instance_mogu_shan_palace : public InstanceMapScript
{
    public:
        instance_mogu_shan_palace() : InstanceMapScript("instance_mogu_shan_palace", 994) { }

        struct instance_mogu_shan_palace_InstanceMapScript : public InstanceScript
        {
            instance_mogu_shan_palace_InstanceMapScript(Map* map) : InstanceScript(map) { }

            uint32 m_auiEncounter[MAX_TYPES];
            EventMap m_mEvents;

            // Trial of the king.
            uint32 delay;
            uint64 m_uiXinGuid;
            uint64 m_uiKuaiGuid;
            uint64 m_uiMingGuid;
            uint64 m_uiHaiyanGuid;
            uint64 m_uiBeaconGuid;
            std::list<uint64> m_lScrapperList;
            std::list<uint64> m_lAdeptList;
            std::list<uint64> m_lGruntList;

            uint32 m_uiBossCount;
            uint32 m_auiBossNumber[3];

            // Xin the weaponmaster.
            std::list<uint64> m_lStaffList;
            std::list<uint64> m_lAxeList;

            uint64 m_uiScoutGuid;

            // Storage
            uint64 m_auiGuids64[MAX_GUIDS];
            std::unordered_map<uint32, uint64> m_mGoEntryGuidMap;

            void Initialize() OVERRIDE
            {
                memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
                memset(&m_auiGuids64, 0, sizeof(m_auiGuids64));

                SetBossNumber(MAX_ENCOUNTER);

                delay = 0;
                m_uiXinGuid = 0;
                m_uiKuaiGuid = 0;
                m_uiMingGuid = 0;
                m_uiHaiyanGuid = 0;
                m_uiBeaconGuid = 0;

                InitializeTrialOfKing();

                m_uiScoutGuid = 0;
            }

            void InitializeTrialOfKing()
            {
                m_uiBossCount = 0;

                std::iota(std::begin(m_auiBossNumber), std::end(m_auiBossNumber), 0);
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(std::begin(m_auiBossNumber), std::end(m_auiBossNumber), g);
            }

            GameObject* GetGameObjectFromStorage(uint32 uiEntry)
            {
                GameObject* go = NULL;

                std::unordered_map<uint32, uint64>::iterator find = m_mGoEntryGuidMap.find(uiEntry);

                if (find != m_mGoEntryGuidMap.cend())
                    go = instance->GetGameObject(find->second);

                return go;
            }

            void UnlockGekkanTreasure()
            {
                if (GameObject* go = GetGameObjectFromStorage(instance->IsHeroic() ? GO_ANCIENT_MOGU_TREASURE_HC : GO_ANCIENT_MOGU_TREASURE))
                    go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND | GO_FLAG_NOT_SELECTABLE);

                HandleGameObject(0, true, GetGameObjectFromStorage(GO_GEKKAN_TREASURE_DOOR));
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_GEKKAN:
                        HandleGameObject(0, state == DONE, GetGameObjectFromStorage(GO_DOOR_AFTER_TRIAL));
                        if (state == DONE)
                            UnlockGekkanTreasure();
                        break;
                }

                return true;
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_TRIAL_CHEST:
                    case GO_TRIAL_CHEST_HC:
                        m_mGoEntryGuidMap.insert(std::make_pair(go->GetEntry(), go->GetGUID()));
                        break;
                    case GO_DOOR_BEFORE_TRIAL:
                    case GO_DOOR_AFTER_TRIAL:
                    case GO_DOOR_SECRET_STEPS:
                    case GO_DOOR_BEFORE_KING:
                    case GO_DOOR_BEFORE_KING2:
                    case GO_GEKKAN_TREASURE_DOOR:
                    case GO_ANCIENT_MOGU_TREASURE:
                    case GO_ANCIENT_MOGU_TREASURE_HC:
                        m_mGoEntryGuidMap.insert(std::make_pair(go->GetEntry(), go->GetGUID()));
                        go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
                        break;
                    case GO_USELESS_DOOR:
                        go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
                        break;
                }

                // Restore progress after the instance was reloaded
                switch (go->GetEntry())
                {
                    case GO_DOOR_SECRET_STEPS:
                        if (GetData(TYPE_TRIAL_CHEST) == DONE)
                            HandleGameObject(0, true, go);
                        break;
                    case GO_DOOR_AFTER_TRIAL:
                        if (GetBossState(DATA_GEKKAN) == DONE)
                            HandleGameObject(0, true, go);
                        break;
                    case GO_GEKKAN_TREASURE_DOOR:
                    case GO_ANCIENT_MOGU_TREASURE:
                    case GO_ANCIENT_MOGU_TREASURE_HC:
                        if (GetBossState(DATA_GEKKAN) == DONE)
                            UnlockGekkanTreasure();
                        break;
                }
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                OnCreatureCreate_trial_of_the_king(creature);
                OnCreatureCreate_xin_the_weaponmaster(creature);

                switch (creature->GetEntry())
                {
                    case CREATURE_GLINTROK_SCOUT:
                        m_uiScoutGuid = creature->GetGUID();
                        creature->setActive(true);
                        creature->SetVisible(false);
                        break;
                    case CREATURE_BEACON:
                        m_uiBeaconGuid = creature->GetGUID();
                        creature->SetVisible(GetData(TYPE_TRIAL_CHEST) == DONE);
                        break;
                }
            }

            void OnUnitDeath(Unit* unit) OVERRIDE
            {
                if (unit->ToCreature() && unit->ToCreature()->GetEntry() == CREATURE_GEKKAN)
                    SetBossState(DATA_GEKKAN, DONE);
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                if (type >= MAX_TYPES)
                    return;

                SetData_trial_of_the_king(type, data);
                SetData_xin_the_weaponmaster(type, data);

                if (type == TYPE_SHUFFLE_ORDER && m_uiBossCount >= 3)
                    m_auiEncounter[type] = DONE;
                else
                    m_auiEncounter[type] = data;

                if (data == DONE)
                    SaveToDB();
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                if (type >= MAX_TYPES)
                    return 0;

                return m_auiEncounter[type];
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                if (type < MAX_GUIDS)
                    return m_auiGuids64[type];

                auto const find = m_mGoEntryGuidMap.find(type);
                if (find != m_mGoEntryGuidMap.cend())
                    return find->second;

                return 0;
            }

            void SetData64(uint32 type, uint64 data) OVERRIDE
            {
                if (type >= MAX_GUIDS)
                    return;

                m_auiGuids64[type] = data;
            }

            template <class T>
            static uint64 SelectRandomGuid(std::list<T> const& guids)
            {
                if (guids.empty())
                    return 0;

                auto itr = guids.begin();
                std::advance(itr, urand(0, guids.size() - 1));
                return *itr;
            }

            void SetData_xin_the_weaponmaster(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case TYPE_ACTIVATE_ANIMATED_STAFF:
                    {
                        if (Creature* creature = instance->GetCreature(SelectRandomGuid(m_lStaffList)))
                            if (creature->AI())
                                creature->AI()->DoAction(0); //ACTION_ACTIVATE
                        break;
                    }
                    case TYPE_ACTIVATE_ANIMATED_AXE:
                    {
                        for (uint64 guid : m_lAxeList)
                        {
                            if (Creature* creature = instance->GetCreature(guid))
                                if (creature->AI())
                                    creature->AI()->DoAction(data ? 0 : 1);
                        }
                        break;
                    }
                    case TYPE_XIN:
                        HandleGameObject(0, data != IN_PROGRESS, GetGameObjectFromStorage(GO_DOOR_BEFORE_KING));
                        HandleGameObject(0, data != IN_PROGRESS, GetGameObjectFromStorage(GO_DOOR_BEFORE_KING2));
                        break;
                }
            }

            void OnCreatureCreate_xin_the_weaponmaster(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case 59481:
                        creature->SetReactState(REACT_PASSIVE);
                        break;
                    case CREATURE_ANIMATED_STAFF:
                        m_lStaffList.push_back(creature->GetGUID());
                        break;
                    case CREATURE_ANIMATED_AXE:
                        m_lAxeList.push_back(creature->GetGUID());
                        break;
                    case CREATURE_CROSSBOW:
                        creature->SetReactState(REACT_PASSIVE);
                        creature->CastSpell(creature, SPELL_PERMANENT_FEIGN_DEATH, true);
                        creature->SetDisplayId(42197);
                        break;
                }
            }

            void RetireList(std::list<uint64> const& guids, bool removeAuras)
            {
                for (uint64 guid : guids)
                {
                    Creature* creature = instance->GetCreature(guid);
                    if (!creature)
                        continue;

                    if (creature->AI())
                        creature->AI()->DoAction(1); //EVENT_RETIRE

                    if (removeAuras)
                        creature->RemoveAllAuras();
                }
            }

            void EncourageList(std::list<uint64> const& guids)
            {
                for (uint64 guid : guids)
                    if (Creature* creature = instance->GetCreature(guid))
                        if (creature->AI())
                            creature->AI()->DoAction(0); //EVENT_ENCOURAGE
            }

            void StartTrialBoss(uint64 bossGuid, uint32 type, int32 action)
            {
                Creature* boss = instance->GetCreature(bossGuid);
                if (!boss)
                    return;

                Movement::MoveSplineInit init(boss);
                init.MoveTo(pTrialIntroPositions[type].GetPositionX(), pTrialIntroPositions[type].GetPositionY(), pTrialIntroPositions[type].GetPositionZ());
                init.Launch();

                delay = 0;
                boss->m_Events.Schedule(delay += boss->GetSplineDuration(), 10, [this, bossGuid]()
                {
                    if (Creature* trialBoss = instance->GetCreature(bossGuid))
                    {
                        trialBoss->StopMoving();
                        trialBoss->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        trialBoss->SetReactState(REACT_AGGRESSIVE);
                        trialBoss->SetInCombatWithZone();
                    }
                });

                if (boss->AI())
                    boss->AI()->DoAction(action);
            }

            void AttackRandomOf(std::list<uint64> const& attackers, std::list<uint64> const& victims)
            {
                for (uint64 guid : attackers)
                {
                    Creature* creature = instance->GetCreature(guid);

                    if (creature && creature->AI())
                        creature->AI()->DoAction(2); //ACTION_ATTACK

                    Creature* victim = instance->GetCreature(SelectRandomGuid(victims));
                    if (creature && victim)
                    {
                        creature->Attack(victim, true);
                        if (creature->AI())
                            creature->AI()->AttackStart(victim);
                    }
                }
            }

            void SetData_trial_of_the_king(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case TYPE_WIPE_FIRST_BOSS:
                    {
                        HandleGameObject(0, true, GetGameObjectFromStorage(GO_DOOR_BEFORE_TRIAL));
                        switch (data)
                        {
                            case 0:
                                RetireList(m_lAdeptList, true);
                                break;
                            case 1:
                                RetireList(m_lScrapperList, true);
                                break;
                            case 2:
                                RetireList(m_lGruntList, true);
                                break;
                        }
                        break;
                    }
                    case TYPE_MING:
                        EncourageList(m_lAdeptList);
                        StartTrialBoss(m_uiMingGuid, type, 0);
                        break;
                    case TYPE_KUAI:
                        EncourageList(m_lScrapperList);
                        // special case action 2 for kuai
                        StartTrialBoss(m_uiKuaiGuid, type, 2);
                        break;
                    case TYPE_HAIYAN:
                        EncourageList(m_lGruntList);
                        StartTrialBoss(m_uiHaiyanGuid, type, 0);
                        break;
                    case TYPE_TRIAL_ENDED:
                    {
                        AttackRandomOf(m_lAdeptList, m_lGruntList);
                        AttackRandomOf(m_lGruntList, m_lScrapperList);
                        AttackRandomOf(m_lScrapperList, m_lAdeptList);

                        if (Creature* pKuai = instance->GetCreature(m_uiKuaiGuid))
                            pKuai->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

                        if (Creature* pHaiyan = instance->GetCreature(m_uiHaiyanGuid))
                            pHaiyan->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

                        if (Creature* pMing = instance->GetCreature(m_uiMingGuid))
                            pMing->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);
                        break;
                    }
                    case TYPE_MING_RETIRED:
                        // This boss is down, move on to next
                        ++m_uiBossCount;
                        RetireList(m_lAdeptList, false);
                        break;
                    case TYPE_KUAI_RETIRED:
                        ++m_uiBossCount;
                        RetireList(m_lScrapperList, false);
                        break;
                    case TYPE_HAIYAN_RETIRED:
                        ++m_uiBossCount;
                        RetireList(m_lGruntList, false);
                        break;
                    case TYPE_SHUFFLE_ORDER:
                        if (GetData(TYPE_SHUFFLE_ORDER) == DONE)
                            return;

                        if (m_uiBossCount < 3)
                        {
                            SetData(m_auiBossNumber[m_uiBossCount], 0);
                            HandleGameObject(0, false, GetGameObjectFromStorage(GO_DOOR_BEFORE_TRIAL));
                        }
                        else
                        {
                            SetBossState(DATA_TRIAL_OF_THE_KING, DONE);

                            if (GameObject* go = GetGameObjectFromStorage(GO_TRIAL_CHEST))
                                SetData64(TYPE_TRIAL_ENDED, go->GetGUID());

                            if (Creature* pScout = instance->GetCreature(m_uiScoutGuid))
                                if (pScout->AI())
                                    pScout->AI()->DoAction(0);

                            HandleGameObject(0, true, GetGameObjectFromStorage(GO_DOOR_BEFORE_TRIAL));
                        }
                        break;
                    case TYPE_TRIAL_CHEST:
                        if (GameObject* go = GetGameObjectFromStorage(instance->IsHeroic() ? GO_TRIAL_CHEST_HC : GO_TRIAL_CHEST))
                            DoRespawnGameObject(go->GetGUID(), 7 * DAY);

                        if (Creature* pBeacon = instance->GetCreature(m_uiBeaconGuid))
                            pBeacon->SetVisible(true);

                        for (Map::PlayerList::const_iterator itr = instance->GetPlayers().begin(); itr != instance->GetPlayers().end(); ++itr)
                            if (Player* player = itr->GetSource())
                                player->ModifyCurrency(CURRENCY_TYPE_JUSTICE_POINTS, (instance->IsHeroic() ? 125 : 70) * CURRENCY_PRECISION);

                        HandleGameObject(0, true, GetGameObjectFromStorage(GO_DOOR_SECRET_STEPS));
                        m_mEvents.ScheduleEvent(1, 5 * IN_MILLISECONDS);
                        break;
                }
            }

            void OnCreatureCreate_trial_of_the_king(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case CREATURE_GURTHAN_SCRAPPER:
                    case CREATURE_GURTHAN_SCRAPPER_2:
                        m_lScrapperList.push_back(creature->GetGUID());
                        break;
                    case CREATURE_HARTHAK_ADEPT:
                    case CREATURE_HARTHAK_ADEPT_2:
                        m_lAdeptList.push_back(creature->GetGUID());
                        break;
                    case CREATURE_KARGESH_GRUNT:
                    case CREATURE_KARGESH_GRUNT_2:
                        m_lGruntList.push_back(creature->GetGUID());
                        break;
                    case CREATURE_KUAI_THE_BRUTE:
                        m_uiKuaiGuid = creature->GetGUID();
                        SetData64(TYPE_KUAI, m_uiKuaiGuid);
                        creature->SetReactState(REACT_PASSIVE);
                        break;
                    case CREATURE_MING_THE_CUNNING:
                        m_uiMingGuid = creature->GetGUID();
                        SetData64(TYPE_MING, m_uiMingGuid);
                        creature->SetReactState(REACT_PASSIVE);
                        break;
                    case CREATURE_HAIYAN_THE_UNSTOPPABLE:
                        m_uiHaiyanGuid = creature->GetGUID();
                        SetData64(TYPE_HAIYAN, m_uiHaiyanGuid);
                        creature->SetReactState(REACT_PASSIVE);
                        break;
                    case CREATURE_XIN_THE_WEAPONMASTER_TRIGGER:
                        m_uiXinGuid = creature->GetGUID();
                        creature->SetReactState(REACT_PASSIVE);
                        break;
                }
            }

            void Update(uint32 diff) OVERRIDE
            {
                m_mEvents.Update(diff);

                while (uint32 eventId = m_mEvents.ExecuteEvent())
                {
                    if (eventId == 1)
                    {
                        if (Creature* Saurok = instance->SummonCreature(CREATURE_GLINTROK_PILLAGER, MiscCosmeticGlintrokPath[0]))
                        {
                            Movement::MoveSplineInit init(Saurok);
                            for (auto itr : MiscCosmeticGlintrokPath)
                                init.Path().push_back(G3D::Vector3(itr.GetPositionX() + frand(-2.0f, 2.0f), itr.GetPositionY() + frand(-2.0f, 2.0f), itr.GetPositionZ()));

                            init.SetSmooth();
                            init.SetUncompressed();
                            init.Launch();
                            Saurok->DespawnOrUnsummon(Saurok->GetSplineDuration());
                        }

                        m_mEvents.ScheduleEvent(1, 3 * IN_MILLISECONDS);
                    }
                    break;
                }
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "M P " << GetBossSaveData();
                for (uint8 i = 0; i < MAX_TYPES; ++i)
                    saveStream << m_auiEncounter[i] << ' ';

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

                char dataHead1, dataHead2;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'M' && dataHead2 == 'P')
                {
                    for (uint32 i = 0; i < MAX_ENCOUNTER; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;

                        SetBossState(i, EncounterState(tmpState));
                    }

                    for (uint8 i = 0; i < MAX_TYPES; ++i)
                    {
                        loadStream >> m_auiEncounter[i];

                        // Do not load an encounter as "In Progress" - reset it instead.
                        if (m_auiEncounter[i] == IN_PROGRESS)
                            m_auiEncounter[i] = NOT_STARTED;
                    }
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_mogu_shan_palace_InstanceMapScript(map);
        }
};

void AddSC_instance_mogu_shan_palace()
{
    new instance_mogu_shan_palace();
}
