/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "MoveSplineInit.h"
#include "temple_of_jade_serpent.h"

DoorData const doorData[] =
{
    { GO_DOOR_LOREWALKER_STONESTEP, DATA_LOREWALKER, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_DOOR_WISE_MARI,            DATA_WISE_MARI,  DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { 0,                            0,               DOOR_TYPE_ROOM,    BOUNDARY_NONE },
};

class instance_jade_serpent : public InstanceMapScript
{
    public:
        instance_jade_serpent() : InstanceMapScript(JadeSerpentScriptName, 960) { }

        struct instance_jade_serpent_InstanceScript : public InstanceScript
        {
            instance_jade_serpent_InstanceScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);

                memset(_data, 0, sizeof(_data));
                _twinDragonsDoorState = NOT_STARTED;
                _liuState             = NOT_STARTED;
                _sunsDefeated         = 0;
                _trialsDefeated       = 0;
                _minionsKilled        = 0;

                WiseMariGUID          = 0;
                LorewalkerGUID        = 0;
                LiuGUID               = 0;
                ShaOfDoubtGUID        = 0;
                ZaoGUID               = 0;
                OsongGUID             = 0;
                ScrollGUID            = 0;
                PerilGUID             = 0;
                StrifeGUID            = 0;
                YuLonGUID             = 0;
                TwinDragonsDoorGUID   = 0;
                ShaDoorGUID           = 0;
                WiseMariExitDoorGUID  = 0;
                LorewalkerDoorGUID    = 0;
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_DOOR_SHA_OF_DOUBT:
                        ShaDoorGUID = go->GetGUID();
                        HandleGameObject(0, GetBossState(DATA_LIU) == DONE, go);
                        break;
                    case GO_DOOR_LIU_FLAMEHEART:
                        TwinDragonsDoorGUID = go->GetGUID();
                        HandleGameObject(0, _twinDragonsDoorState == DONE, go);
                        if (_liuState == DONE && GetBossState(DATA_LIU) != DONE)
                            instance->SummonCreature(NPC_LIU_FLAMEHEART, LiuSpawnPosition);
                        break;
                    case GO_DOOR_WISE_MARI:
                        AddDoor(go, true);
                        break;
                    case GO_DOOR_LOREWALKER_STONESTEP:
                        LorewalkerDoorGUID = go->GetGUID();
                        AddDoor(go, true);
                        break;
                    case GO_DOOR_WISE_MARI_EXIT:
                        WiseMariExitDoorGUID = go->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_DOOR_WISE_MARI:
                    case GO_DOOR_LOREWALKER_STONESTEP:
                        AddDoor(go, false);
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_WISE_MARI:
                        WiseMariGUID = creature->GetGUID();
                        break;
                    case NPC_YU_LON:
                        YuLonGUID = creature->GetGUID();
                        break;
                    case NPC_STRIFE:
                        StrifeGUID = creature->GetGUID();
                        break;
                    case NPC_PERIL:
                        PerilGUID = creature->GetGUID();
                        break;
                    case NPC_HAUNTING_SHA:
                        _hauntingShas.push_back(creature->GetGUID());
                        break;
                    case NPC_ZAO_SUN_TRIGGER:
                        creature->setFaction(14);
                        creature->SetDisplayId(11686);
                        creature->SetReactState(REACT_PASSIVE);
                        _sunTriggers.push_back(creature->GetGUID());
                        break;
                    case NPC_ZAO_SUN:
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        _suns.push_back(creature->GetGUID());
                        break;
                    case NPC_ZAO_SUNSEEKER:
                        ZaoGUID = creature->GetGUID();
                        break;
                    case NPC_OSONG:
                        OsongGUID = creature->GetGUID();
                        creature->CastSpell(creature, SPELL_SHA_CORRUPTION, false);
                        break;
                    case NPC_LOREWALKER_STONESTEP:
                        LorewalkerGUID = creature->GetGUID();
                        creature->CastSpell(creature, SPELL_ROOT_SELF, true);
                        break;
                    case NPC_SCROLL:
                        ScrollGUID = creature->GetGUID();
                        break;
                    case NPC_WISE_DRAGON:
                    case NPC_WELL:
                    case NPC_STRIF:
                    case NPC_HEROINE:
                    case NPC_MOON:
                    case NPC_GOLDEN_BEETLE:
                    case NPC_CRYBABY_HOZEN:
                    case NPC_THE_NODDING_TIGER:
                        creature->CastSpell(creature, SPELL_SHA_CORRUPTION, false);
                        _corruptedCreatures.push_back(creature->GetGUID());
                        break;
                    case NPC_HAUNTING_SHA_3:
                        creature->CastSpell(creature, SPELL_SHA_CORRUPTION, false);
                        creature->CastSpell(creature, SPELL_HAUNTING_GAZE, false);
                        _corruptedCreatures.push_back(creature->GetGUID());
                        break;
                    case NPC_THE_SONGBIRD_QUEEN:
                        creature->CastSpell(creature, SPELL_SHA_CORRUPTION, false);
                        creature->CastSpell(creature, SPELL_SINGING_SONGBIRD, false);
                        _corruptedCreatures.push_back(creature->GetGUID());
                        break;
                    case NPC_TALKING_FISH:
                        creature->CastSpell(creature, SPELL_SHA_CORRUPTION, false);
                        creature->CastSpell(creature, SPELL_WATER_BUBBLE, false);
                        _corruptedCreatures.push_back(creature->GetGUID());
                        break;
                    case NPC_JIANG:
                        creature->CastSpell(creature, SPELL_SHA_CORRUPTION_2, false);
                        creature->CastSpell(creature, SPELL_JUGGLER_JIANG, false);
                        _corruptedCreatures.push_back(creature->GetGUID());
                        break;
                    case NPC_XIANG:
                        creature->CastSpell(creature, SPELL_SHA_CORRUPTION_2, false);
                        creature->CastSpell(creature, SPELL_JUGGLER_XIANG, false);
                        _corruptedCreatures.push_back(creature->GetGUID());
                        break;
                    case NPC_SWIRLING_SUNFIRE:
                        creature->SetDisplayId(11686);
                        creature->SetReactState(REACT_PASSIVE);
                        _sunfires.push_back(creature->GetGUID());
                        break;
                    case NPC_JADE_FIRE:
                        creature->setFaction(14);
                        creature->SetDisplayId(11686);
                        creature->SetReactState(REACT_PASSIVE);
                        creature->CastSpell(creature, SPELL_JADE_FIRE_PERIODIC, true);
                        creature->DespawnOrUnsummon(5 * IN_MILLISECONDS);
                        break;
                    case NPC_LIU_FLAMEHEART:
                        LiuGUID = creature->GetGUID();
                        break;
                    case NPC_SHA_OF_DOUBT:
                        ShaOfDoubtGUID = creature->GetGUID();
                        break;
                    case NPC_FIGMENT_OF_DOUBT:
                        MirrorSummoner(creature);
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
                    case NPC_ZAO_SUN:
                        unit->CastSpell(unit, SPELL_EXTRACT_SHA, false);
                        break;
                    case NPC_SCROLL:
                        StartLorewalkerEvent(unit, urand(0, 1) ? EVENT_LOREWALKER_STONESTEP_TRIAL : EVENT_LOREWALKER_STONESTEP_SUNS);
                        break;
                    case NPC_MINION_OF_DOUBT:
                    case NPC_LESSER_SHA:
                        if (_liuState != DONE && ++_minionsKilled == LiuSummonDeathCount)
                        {
                            instance->SummonCreature(NPC_LIU_FLAMEHEART, LiuSpawnPosition);
                            SetData(DATA_LIU_STATE, DONE);
                        }
                        break;
                    default:
                        break;
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_TWIN_DRAGONS_DOOR:
                        _twinDragonsDoorState = data;
                        if (data == DONE)
                            HandleGameObject(TwinDragonsDoorGUID, true);
                        else if (data == NOT_STARTED)
                            HandleGameObject(TwinDragonsDoorGUID, false);
                        break;
                    case DATA_LIU_STATE:
                        _liuState = data;
                        break;
                    case DATA_SUN_STATE:
                        _sunsDefeated += data;
                        if (_sunsDefeated == 5)
                            OnSunsDefeated();
                        break;
                    case DATA_SUN:
                        if (data == DONE)
                            OnSunsActivated();
                        break;
                    case DATA_LIU:
                        if (data == DONE)
                            OnLiuDefeated();
                        break;
                    case DATA_ZAO:
                        if (data == DONE)
                            CompleteLorewalker();
                        break;
                    case DATA_TRIAL:
                        if (data == DONE && ++_trialsDefeated == 2)
                            CompleteLorewalker();
                        break;
                    case DATA_LOREWALKER:
                        _data[type] = data;
                        if (data == FAIL)
                            ResetLorewalkerEvent();
                        else if (data == DONE)
                            RewardLorewalker();
                        break;
                    case DATA_SHA_OF_DOUBT:
                        if (data == IN_PROGRESS)
                            HandleGameObject(ShaDoorGUID, false);
                        else if (GetBossState(DATA_LIU) == DONE)
                            HandleGameObject(ShaDoorGUID, true);
                        break;
                    default:
                        if (type < MAX_DATA)
                            _data[type] = data;
                        break;
                }

                if (data == DONE)
                    SaveToDB();
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_TWIN_DRAGONS_DOOR:
                        return _twinDragonsDoorState;
                    case DATA_LIU_STATE:
                        return _liuState;
                    default:
                        if (type < MAX_DATA)
                            return _data[type];
                        break;
                }
                return 0;
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_WISE_MARI:
                        return WiseMariGUID;
                    case DATA_LOREWALKER:
                        return LorewalkerGUID;
                    case DATA_LIU:
                        return LiuGUID;
                    case DATA_SHA_OF_DOUBT:
                        return ShaOfDoubtGUID;
                    case DATA_ZAO:
                        return ZaoGUID;
                    default:
                        break;
                }
                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                if (state == DONE && (type == DATA_WISE_MARI || type == DATA_LOREWALKER) &&
                    GetBossState(DATA_WISE_MARI) == DONE && GetBossState(DATA_LOREWALKER) == DONE)
                {
                    HandleGameObject(WiseMariExitDoorGUID, true);
                    HandleGameObject(LorewalkerDoorGUID, true);
                    SetData(DATA_TWIN_DRAGONS_DOOR, DONE);
                }
                return true;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "T J S " << GetBossSaveData() << _twinDragonsDoorState << ' ' << _liuState;

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

                    uint32 temp = 0;
                    loadStream >> temp;
                    _twinDragonsDoorState = temp ? DONE : NOT_STARTED;
                    temp = 0;
                    loadStream >> temp;
                    _liuState = temp ? DONE : NOT_STARTED;
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            void MirrorSummoner(Creature* figment)
            {
                TempSummon* summon = figment->ToTempSummon();
                Unit* summoner = summon ? summon->GetSummoner() : NULL;
                if (!summoner)
                    return;

                figment->CastSpell(figment, SPELL_FIGMENT_OF_DOUBT_CLONE, false);
                figment->SetDisplayId(summoner->GetDisplayId());
                figment->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_MIRROR_IMAGE);
                summoner->CastSpell(figment, SPELL_COPY_WEAPON, false);
                summoner->CastSpell(figment, SPELL_COPY_WEAPON_2, false);

                uint8 const slots[2] = { EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_OFFHAND };
                for (uint8 i = 0; i < 2; ++i)
                {
                    uint32 itemId = summoner->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID + i);
                    if (Player* player = summoner->ToPlayer())
                    {
                        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slots[i]);
                        itemId = item ? item->GetEntry() : 0;
                    }
                    figment->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID + i, itemId);
                }
            }

            void StartLorewalkerEvent(Unit* scroll, uint32 eventType)
            {
                if (Creature* lorewalker = instance->GetCreature(LorewalkerGUID))
                    if (lorewalker->IsAIEnabled)
                        lorewalker->AI()->DoAction(eventType);

                for (uint64 guid : _corruptedCreatures)
                {
                    if (Creature* creature = instance->GetCreature(guid))
                    {
                        creature->AddAura(SPELL_DRAW_SHA, creature);
                        creature->CastSpell(scroll, SPELL_DRAW_SHA_2, false);
                        creature->DespawnOrUnsummon(2 * IN_MILLISECONDS);
                    }
                }

                if (eventType == EVENT_LOREWALKER_STONESTEP_SUNS)
                {
                    for (uint8 i = 0; i < 5; ++i)
                    {
                        instance->SummonCreature(NPC_ZAO_SUN, SunSummonPos[i]);
                        instance->SummonCreature(NPC_ZAO_SUN_TRIGGER, SunTriggerSummonPos[i]);
                    }

                    if (TempSummon* zao = scroll->SummonCreature(NPC_ZAO_SUNSEEKER, 846.877f, -2449.110f, 175.044f))
                        zao->SetFacingTo(4.450f);

                    for (uint64 guid : _sunfires)
                        if (Creature* creature = instance->GetCreature(guid))
                            creature->CastSpell(creature, SPELL_BLUSTERING_VORTEX, false);
                }
                else
                {
                    if (TempSummon* strife = scroll->SummonCreature(NPC_STRIFE, 847.530f, -2469.184f, 174.960f))
                    {
                        strife->SetFacingTo(1.525f);
                        strife->SetReactState(REACT_PASSIVE);
                    }

                    if (TempSummon* peril = scroll->SummonCreature(NPC_PERIL, 836.906f, -2465.859f, 174.960f))
                    {
                        peril->SetFacingTo(1.014f);
                        peril->SetReactState(REACT_PASSIVE);
                    }
                }
            }

            void OnSunsActivated()
            {
                _sunsDefeated = 0;

                for (uint64 guid : _suns)
                {
                    if (Creature* sun = instance->GetCreature(guid))
                    {
                        SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, sun);
                        sun->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        if (sun->IsAIEnabled)
                            sun->AI()->DoAction(0);
                    }
                }

                for (uint64 guid : _sunTriggers)
                    if (Creature* trigger = instance->GetCreature(guid))
                        trigger->CastSpell(trigger, SPELL_SUNFIRE_EXPLOSION, false);
            }

            void OnSunsDefeated()
            {
                if (Creature* zao = instance->GetCreature(ZaoGUID))
                    if (zao->IsAIEnabled)
                        zao->AI()->DoAction(2);

                for (uint64 guid : _suns)
                    if (Creature* sun = instance->GetCreature(guid))
                        SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, sun);

                for (uint64 guid : _hauntingShas)
                {
                    if (Creature* sha = instance->GetCreature(guid))
                    {
                        sha->Respawn(true);
                        sha->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        if (sha->IsAIEnabled)
                            sha->AI()->DoAction(0);
                    }
                }

                for (uint64 guid : _sunfires)
                    if (Creature* sunfire = instance->GetCreature(guid))
                        sunfire->RemoveAurasDueToSpell(SPELL_BLUSTERING_VORTEX);
            }

            void CompleteLorewalker()
            {
                SetBossState(DATA_LOREWALKER, DONE);
                SetData(DATA_LOREWALKER, DONE);
            }

            void ResetLorewalkerEvent()
            {
                if (Creature* lorewalker = instance->GetCreature(LorewalkerGUID))
                {
                    lorewalker->RemoveAurasDueToSpell(SPELL_MEDITATION);
                    lorewalker->Respawn();
                    if (lorewalker->IsAIEnabled)
                        lorewalker->AI()->Reset();
                }

                uint64 const encounterUnits[3] = { PerilGUID, StrifeGUID, ZaoGUID };
                for (uint8 i = 0; i < 3; ++i)
                {
                    if (Creature* creature = instance->GetCreature(encounterUnits[i]))
                    {
                        SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, creature);
                        creature->DespawnOrUnsummon();
                    }
                }

                if (Creature* osong = instance->GetCreature(OsongGUID))
                    osong->DespawnOrUnsummon(1500);

                if (Creature* scroll = instance->GetCreature(ScrollGUID))
                    scroll->Respawn();

                for (uint64 guid : _suns)
                {
                    if (Creature* sun = instance->GetCreature(guid))
                    {
                        SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, sun);
                        sun->DespawnOrUnsummon();
                    }
                }
                _suns.clear();

                for (uint64 guid : _hauntingShas)
                    if (Creature* sha = instance->GetCreature(guid))
                        sha->DespawnOrUnsummon();
                _hauntingShas.clear();

                for (uint64 guid : _sunfires)
                    if (Creature* sunfire = instance->GetCreature(guid))
                        sunfire->RemoveAurasDueToSpell(SPELL_BLUSTERING_VORTEX);

                _trialsDefeated = 0;
            }

            void RewardLorewalker()
            {
                if (Creature* osong = instance->GetCreature(OsongGUID))
                    osong->DespawnOrUnsummon(1500);

                if (Creature* lorewalker = instance->GetCreature(LorewalkerGUID))
                {
                    lorewalker->SetReactState(REACT_PASSIVE);
                    if (lorewalker->IsAIEnabled)
                        lorewalker->AI()->DoAction(2);
                }

                DoCastSpellOnPlayers(SPELL_LOREWALKER_ALACRITY);
            }

            void OnLiuDefeated()
            {
                if (Creature* liu = instance->GetCreature(LiuGUID))
                {
                    liu->RemoveAurasDueToSpell(SPELL_JADE_ESSENCE);
                    liu->CastSpell(liu, SPELL_TRANSFORM_VISUAL, false);
                    liu->RemoveAurasDueToSpell(SPELL_POSSESSED_BY_SHA);
                    liu->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }

                for (uint8 i = 0; i < 3; ++i)
                {
                    if (Creature* minion = instance->SummonCreature(NPC_MINION_OF_DOUBT, MinionOfDoubtSummonPos[i]))
                    {
                        float x, y;
                        GetPositionWithDistInOrientation(minion, 18.0f, minion->GetOrientation(), x, y);
                        Movement::MoveSplineInit init(minion);
                        init.MoveTo(x, y, minion->GetPositionZ());
                        init.SetVelocity(9.0f);
                        init.Launch();
                    }
                }

                HandleGameObject(ShaDoorGUID, true);

                if (Creature* sha = instance->GetCreature(ShaOfDoubtGUID))
                    sha->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            uint32 _data[MAX_DATA];
            uint32 _twinDragonsDoorState;
            uint32 _liuState;
            uint32 _sunsDefeated;
            uint32 _trialsDefeated;
            uint32 _minionsKilled;

            std::list<uint64> _corruptedCreatures;
            std::list<uint64> _sunfires;
            std::list<uint64> _suns;
            std::list<uint64> _sunTriggers;
            std::list<uint64> _hauntingShas;

            uint64 WiseMariGUID;
            uint64 LorewalkerGUID;
            uint64 LiuGUID;
            uint64 ShaOfDoubtGUID;
            uint64 ZaoGUID;
            uint64 OsongGUID;
            uint64 ScrollGUID;
            uint64 PerilGUID;
            uint64 StrifeGUID;
            uint64 YuLonGUID;
            uint64 TwinDragonsDoorGUID;
            uint64 ShaDoorGUID;
            uint64 WiseMariExitDoorGUID;
            uint64 LorewalkerDoorGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_jade_serpent_InstanceScript(map);
        }
};

void AddSC_instance_jade_serpent()
{
   new instance_jade_serpent();
}
