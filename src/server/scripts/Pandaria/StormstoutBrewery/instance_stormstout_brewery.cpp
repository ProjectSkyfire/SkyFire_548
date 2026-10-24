/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Player.h"
#include "GameObject.h"
#include "stormstout_brewery.h"

DoorData const doorData[] =
{
    { GO_OOK_EXIT_DOOR,      DATA_OOK_OOK,   DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_HOPTALLUS_ENTRANCE, DATA_HOPTALLUS, DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { GO_YANZHU_ENTRANCE,    DATA_YAN_ZHU,   DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { 0,                     0,              DOOR_TYPE_ROOM,    BOUNDARY_NONE }
};

enum InstanceEvents
{
    EVENT_CHECK_HOZEN_SLAIN = 1,
    EVENT_RELEASE_BOUNCERS  = 2,
    EVENT_HOPTALLUS_WAVES   = 3,
};

Position const OokOokDoorPos = { -766.863f, 1391.67f, 146.739f, 0.298219f };

class instance_stormstout_brewery : public InstanceMapScript
{
    public:
        instance_stormstout_brewery() : InstanceMapScript(StormstoutBreweryScriptName, 961) { }

        struct instance_stormstout_brewery_InstanceScript : public InstanceScript
        {
            instance_stormstout_brewery_InstanceScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);

                _ookOokGUID = 0;
                _hoptallusGUID = 0;
                _yanZhuGUID = 0;
                _ookDoorGUID = 0;
                _uncleGaoGUID = 0;
                _chenYanZhuGUID = 0;
                _hozenSlain = 0;

                _events.ScheduleEvent(EVENT_CHECK_HOZEN_SLAIN, 10000);
                _events.ScheduleEvent(EVENT_RELEASE_BOUNCERS, 10000);
                _events.ScheduleEvent(EVENT_HOPTALLUS_WAVES, 10000);
            }

            void OnPlayerEnter(Player* player) OVERRIDE
            {
                if (_hozenSlain >= HozenToSlainForOokOok)
                    return;

                // After a restart the bar is still applied but its counter is gone
                player->RemoveAurasDueToSpell(SPELL_BANANA_BAR);
                player->CastSpell(player, SPELL_BANANA_BAR, false);

                if (_hozenSlain > 0)
                    player->SetPower(POWER_ALTERNATE_POWER, _hozenSlain);
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_OOK_OOK:
                        _ookOokGUID = creature->GetGUID();
                        break;
                    case NPC_HOPTALLUS:
                        _hoptallusGUID = creature->GetGUID();
                        break;
                    case NPC_UNCLE_GAO:
                        _uncleGaoGUID = creature->GetGUID();
                        break;
                    case NPC_CHEN_YANZHU:
                        _chenYanZhuGUID = creature->GetGUID();
                        creature->SetVisible(false);
                        creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                        break;
                    case NPC_YAN_ZHU:
                        _yanZhuGUID = creature->GetGUID();
                        for (YanZhuBrew const& brew : YanZhuBrews)
                            creature->AddAura(brew.spell, creature);
                        break;
                    case NPC_HOZEN_CLINGER:
                        creature->SetCanFly(true);
                        break;
                    case NPC_HOZEN_PARTY_ANIMAL:
                    case NPC_HOZEN_PARTY_ANIMAL2:
                    case NPC_HOZEN_PARTY_ANIMAL3:
                        _hozenGUIDs.push_back(creature->GetGUID());
                        creature->CastSpell(creature, HozenHats[std::rand() % 6], false);
                        break;
                    case NPC_HOZEN_BOUNCER:
                        _bouncerGUIDs.push_back(creature->GetGUID());
                        break;
                    case NPC_PURPOSE_BUNNY_GROUND:
                    {
                        creature->RemoveAurasDueToSpell(128571);
                        uint64 bunnyGUID = creature->GetGUID();
                        creature->m_Events.Schedule(1500, [this, bunnyGUID]()
                        {
                            if (Creature* bunny = instance->GetCreature(bunnyGUID))
                                if (Creature* flyTrigger = bunny->FindNearestCreature(NPC_PURPOSE_BUNNY_FLYING, 30.0f, true))
                                    bunny->CastSpell(flyTrigger, SPELL_GUSHING_BREW_BUNNY, true);
                        });
                        break;
                    }
                    case NPC_DRUNKEN_HOZEN_BRAWLER:
                        creature->setRegeneratingHealth(false);
                        creature->SetHealth(creature->CountPctFromMaxHealth(10));
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_OOK_EXIT_DOOR:
                    case GO_HOPTALLUS_ENTRANCE:
                    case GO_YANZHU_ENTRANCE:
                        AddDoor(go, true);
                        break;
                    case GO_OOK_DOOR:
                        _ookDoorGUID = go->GetGUID();
                        if (GetBossState(DATA_OOK_OOK) == DONE)
                            go->AddObjectToRemoveList();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_OOK_EXIT_DOOR:
                    case GO_HOPTALLUS_ENTRANCE:
                    case GO_YANZHU_ENTRANCE:
                        AddDoor(go, false);
                        break;
                    case GO_OOK_DOOR:
                        _ookDoorGUID = 0;
                        break;
                    default:
                        break;
                }
            }

            void OnUnitDeath(Unit* unit) OVERRIDE
            {
                if (unit->GetTypeId() != TypeID::TYPEID_UNIT)
                    return;

                switch (unit->GetEntry())
                {
                    case NPC_DRUNKEN_HOZEN_BRAWLER:
                    case NPC_INFLAMED_HOZEN_BRAWLER:
                    case NPC_SLEEPY_HOZEN_BRAWLER:
                    case NPC_SODDEN_HOZEN_BRAWLER:
                    case NPC_HOZEN_PARTY_ANIMAL:
                    case NPC_HOZEN_PARTY_ANIMAL2:
                    case NPC_HOZEN_PARTY_ANIMAL3:
                    {
                        SetData(DATA_HOZEN_SLAIN, _hozenSlain + 1);

                        std::list<Player*> players;
                        GetPlayerListInGrid(players, unit, 200.0f);
                        for (Player* player : players)
                            if (player->HasAura(SPELL_BANANA_BAR) && player->GetPower(POWER_ALTERNATE_POWER) + 1 < int32(HozenToSlainForOokOok))
                                player->SetPower(POWER_ALTERNATE_POWER, player->GetPower(POWER_ALTERNATE_POWER) + 1);
                        break;
                    }
                    default:
                        break;
                }
            }

            void Update(uint32 diff) OVERRIDE
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_HOZEN_SLAIN:
                            if (_hozenSlain < HozenToSlainForOokOok || GetBossState(DATA_OOK_OOK) == DONE)
                            {
                                if (GetBossState(DATA_OOK_OOK) != DONE)
                                    _events.ScheduleEvent(EVENT_CHECK_HOZEN_SLAIN, 7000);
                                break;
                            }

                            if (Creature* ookOok = instance->GetCreature(_ookOokGUID))
                            {
                                SetBossState(DATA_OOK_OOK, SPECIAL);
                                ookOok->AI()->DoAction(0);

                                for (uint64 guid : _hozenGUIDs)
                                    if (Creature* hozen = instance->GetCreature(guid))
                                        if (hozen->IsAlive())
                                            hozen->AI()->DoAction(0);

                                Map::PlayerList const& players = instance->GetPlayers();
                                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                                    if (Player* player = itr->GetSource())
                                    {
                                        if (player->IsAlive() && !player->IsGameMaster())
                                            player->CombatStop(true);
                                        player->RemoveAurasDueToSpell(SPELL_BANANA_BAR);
                                    }
                            }
                            else
                                _events.ScheduleEvent(EVENT_CHECK_HOZEN_SLAIN, 7000);
                            break;
                        case EVENT_RELEASE_BOUNCERS:
                        {
                            if (GetBossState(DATA_OOK_OOK) != DONE)
                            {
                                _events.ScheduleEvent(EVENT_RELEASE_BOUNCERS, 3000);
                                break;
                            }

                            int32 number = 0;
                            for (uint64 guid : _bouncerGUIDs)
                                if (Creature* bouncer = instance->GetCreature(guid))
                                    bouncer->AI()->DoAction(number++);
                            break;
                        }
                        case EVENT_HOPTALLUS_WAVES:
                            if (GetBossState(DATA_HOPTALLUS) == SPECIAL || GetBossState(DATA_HOPTALLUS) == DONE)
                                break;

                            _events.ScheduleEvent(EVENT_HOPTALLUS_WAVES, urand(12 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));

                            if (Creature* hoptallus = instance->GetCreature(_hoptallusGUID))
                                if (hoptallus->IsAlive())
                                    hoptallus->AI()->DoAction(0);
                            break;
                        default:
                            break;
                    }
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                if (type == DATA_HOZEN_SLAIN)
                {
                    _hozenSlain = data;
                    SaveToDB();
                }
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                if (type == DATA_HOZEN_SLAIN)
                    return _hozenSlain;
                return 0;
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_OOK_OOK:
                        return _ookOokGUID;
                    case DATA_HOPTALLUS:
                        return _hoptallusGUID;
                    case DATA_YAN_ZHU:
                        return _yanZhuGUID;
                    case DATA_OOK_DOOR:
                        return _ookDoorGUID;
                    case DATA_UNCLE_GAO:
                        return _uncleGaoGUID;
                    case DATA_CHEN_YANZHU:
                        return _chenYanZhuGUID;
                    default:
                        break;
                }
                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                if (type == DATA_OOK_OOK)
                {
                    if (state == DONE)
                    {
                        if (GameObject* go = instance->GetGameObject(_ookDoorGUID))
                            go->AddObjectToRemoveList();
                    }
                    else if (!instance->GetGameObject(_ookDoorGUID))
                    {
                        if (Creature* ookOok = instance->GetCreature(_ookOokGUID))
                            if (GameObject* go = ookOok->SummonGameObject(GO_OOK_DOOR, OokOokDoorPos.GetPositionX(), OokOokDoorPos.GetPositionY(), OokOokDoorPos.GetPositionZ(), OokOokDoorPos.GetOrientation(), 0, 0, 0, 0, 14400))
                            {
                                go->SetGoState(GOState::GO_STATE_ACTIVE);
                                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
                            }
                    }
                }

                return true;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S S B " << GetBossSaveData() << _hozenSlain;

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

                char dataHead1, dataHead2, dataHead3;

                std::istringstream loadStream(str);
                loadStream >> dataHead1 >> dataHead2 >> dataHead3;

                if (dataHead1 == 'S' && dataHead2 == 'S' && dataHead3 == 'B')
                {
                    for (uint32 i = 0; i < EncounterCount; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }

                    loadStream >> _hozenSlain;
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        protected:
            EventMap _events;
            std::vector<uint64> _hozenGUIDs;
            std::vector<uint64> _bouncerGUIDs;
            uint64 _ookOokGUID;
            uint64 _hoptallusGUID;
            uint64 _yanZhuGUID;
            uint64 _ookDoorGUID;
            uint64 _uncleGaoGUID;
            uint64 _chenYanZhuGUID;
            uint32 _hozenSlain;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_stormstout_brewery_InstanceScript(map);
        }
};

// 7998
class AreaTrigger_at_stormstout_intro : public AreaTriggerScript
{
    public:
        AreaTrigger_at_stormstout_intro() : AreaTriggerScript("at_stormstout_intro") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            if (Creature* chen = GetClosestCreatureWithEntry(player, NPC_CHEN_STORMSTOUT, 20.0f))
            {
                chen->AI()->DoAction(0);
                return true;
            }
            return false;
        }
};

void AddSC_instance_stormstout_brewery()
{
    new instance_stormstout_brewery();
    new AreaTrigger_at_stormstout_intro();
}
