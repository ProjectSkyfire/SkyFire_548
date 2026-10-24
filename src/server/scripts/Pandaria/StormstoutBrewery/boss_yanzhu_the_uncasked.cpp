/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "stormstout_brewery.h"

static Position const gaoWaypoints[] =
{
    { 0.f, 0.f, 0.f                    },
    { -703.3f, 1187.5f, 166.7f         },
    { -707.4f, 1189.8f, 166.7f         },
    { -709.6f, 1185.0f, 166.7f, 4.905f }
};

static Position const chenWaypoints[] =
{
    { -673.7f, 1155.7f, 166.7f          },
    { 0.f,     0.f,     0.f             },
    { -708.1f, 1180.2f, 166.7f, 1.832f  }
};

static Position const largeBrewPos[] =
{
    { -702.09f, 1158.76f, 166.22f, 0.24f },
    { -704.20f, 1166.94f, 166.22f, 0.24f }
};

static Position const smallBrewPos[] =
{
    { -709.23f, 1152.57f, 166.22f, 0.17f },
    { -711.52f, 1160.55f, 166.22f, 0.17f },
    { -713.58f, 1168.67f, 166.22f, 0.17f },
    { -701.03f, 1154.85f, 166.22f, 0.17f },
    { -703.38f, 1162.76f, 166.22f, 0.17f },
    { -705.75f, 1170.97f, 166.22f, 0.17f },
    { -693.20f, 1157.04f, 166.22f, 0.17f },
    { -695.33f, 1165.07f, 166.22f, 0.17f },
    { -697.44f, 1173.05f, 166.22f, 0.17f },
};

static Position const middleBrewPos[] =
{
    { -700.52f, 1167.84f, 166.22f, 0.25f },
    { -698.25f, 1159.97f, 166.22f, 0.25f },
    { -708.61f, 1165.69f, 166.22f, 0.25f },
    { -706.43f, 1157.70f, 166.22f, 0.25f },
};

static Position const yanzhuPos = { -703.44f, 1162.43f, 166.22f, 0.24f };
static Position const gaoPotPos = { -676.96f, 1193.96f, 166.79f, 1.82f };

uint32 const brewSpells[3] =
{
    128253,
    128255,
    128257,
};

class npc_uncle_gao : public CreatureScript
{
    public:
        npc_uncle_gao() : CreatureScript("npc_uncle_gao") { }

        enum EncounterStage
        {
            STAGE_MIDDLE_ADDS,
            STAGE_SMALL_ADDS,
            STAGE_LARGE_ADDS,
            STAGE_BOSS
        };

        enum Talks
        {
            TALK_SUMMON1,
            TALK_SUMMON2,
            TALK_SUMMON3,
            TALK_SUMMON_BOSS,
            TALK_BOSS_DEATH,
            TALK_OUTRO_1,
            TALK_OUTRO_2,
            TALK_OUTRO_3,
            TALK_OUTRO_4,
            TALK_OUTRO_5,
            TALK_OUTRO_6
        };

        enum Events
        {
            EVENT_NONE,
            EVENT_HANDLE_SUMMONING,
            EVENT_HANDLE_SPELLCASTING,
            EVENT_HANDLE_CRAFT_ANIMATION,
            EVENT_NEXT_POT,
            EVENT_OUTRO_1,
            EVENT_OUTRO_2,
            EVENT_OUTRO_3,
            EVENT_OUTRO_4,
            EVENT_OUTRO_5,
            EVENT_OUTRO_6,
            EVENT_OUTRO_7,
            EVENT_MOVECHECK,
            EVENT_UPDATE_ENCOUNTER,
        };

        enum Actions
        {
            ACTION_START_OUTRO,
            ACTION_OUTRO_3,
            ACTION_OUTRO_4,
            ACTION_OUTRO_6,
            ACTION_START_BOSS,
            ACTION_SUMMON_BOSS
        };

        enum Spells
        {
            SPELL_SMALL_SPAWN          = 128242,
            SPELL_MEDIUM_SPAWN         = 128243,
            SPELL_LARGE_SPAWN          = 128244
        };

        struct npc_uncle_gaoAI : public ScriptedAI
        {
            npc_uncle_gaoAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                encounterStarted = false;
                encounterStage = 0;
                waypoint = 0;
                for (uint32 i = 0; i < 3; ++i)
                    addStore[i] = YanZhuBrews[i].entry;
            }

            bool encounterStarted;
            uint32 encounterStage;
            uint32 waypoint;
            uint32 addStore[3];
            std::list<uint64> currentStageGuidList;
            EventMap events;
            InstanceScript* instance;

            void Reset() OVERRIDE
            {
                if (instance && instance->GetBossState(DATA_YAN_ZHU) == DONE)
                {
                    encounterStarted = true;
                    me->NearTeleportTo(gaoWaypoints[3].GetPositionX(), gaoWaypoints[3].GetPositionY(), gaoWaypoints[3].GetPositionZ(), gaoWaypoints[3].GetOrientation());
                    GetChenAndDoAction(4);
                }
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (!encounterStarted)
                    if (who && who->GetTypeId() == TypeID::TYPEID_PLAYER && who->GetDistance(me) < 30.f && who->GetPositionZ() > 160.f)
                        StartEncounter();
            }

            void StartEncounter()
            {
                encounterStarted = true;
                encounterStage = 0;
                AdvanceEncounter();
            }

            void SummonAdd(uint32 entry, Position const& pos, uint32 spawnSpell)
            {
                if (Creature* creature = me->SummonCreature(entry, pos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, urand(14000, 24000)))
                {
                    currentStageGuidList.push_back(creature->GetGUID());
                    creature->CastSpell(creature, spawnSpell, false);
                }
            }

            void GetChenAndDoAction(int32 action)
            {
                if (instance)
                    if (Creature* chen = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CHEN_YANZHU)))
                        chen->AI()->DoAction(action);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (pointId)
                {
                    case 0:
                        waypoint++;
                        me->SetStandState(UNIT_STAND_STATE_KNEEL);
                        events.ScheduleEvent(EVENT_OUTRO_1, 3300);
                        break;
                    case 1:
                    case 2:
                        waypoint++;
                        events.ScheduleEvent(EVENT_MOVECHECK, 100);
                        break;
                    case 3:
                        waypoint++;
                        events.ScheduleEvent(EVENT_OUTRO_7, 3400);
                        me->SetFacingTo(gaoWaypoints[pointId].GetOrientation());
                        break;
                    case 101:
                        me->SetFacingTo(gaoPotPos.GetOrientation());
                        me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING);
                        break;
                    default:
                        break;
                }
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_START_OUTRO:
                    {
                        me->HandleEmoteCommand(EMOTE_STATE_NONE);

                        if (instance)
                        {
                            if (Creature* yanZhu = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_YAN_ZHU)))
                            {
                                float x, y;
                                GetPositionWithDistInOrientation(me, me->GetDistance(yanZhu) - 1.5f, me->GetAngle(yanZhu), x, y);

                                waypoint = 0;
                                me->SetWalk(false);
                                me->GetMotionMaster()->MovePoint(waypoint, x, y, me->GetMap()->GetHeight(x, y, me->GetPositionZ(), true, 100.0f));
                            }
                        }

                        events.Reset();
                        Talk(TALK_BOSS_DEATH);
                        break;
                    }
                    case ACTION_OUTRO_3:
                        events.ScheduleEvent(EVENT_OUTRO_3, 6000);
                        break;
                    case ACTION_OUTRO_4:
                        events.ScheduleEvent(EVENT_OUTRO_4, 5300);
                        break;
                    case ACTION_OUTRO_6:
                        events.ScheduleEvent(EVENT_OUTRO_6, 3800);
                        break;
                    case ACTION_START_BOSS:
                        if (!encounterStarted)
                            StartEncounter();
                        break;
                    case ACTION_SUMMON_BOSS:
                        AdvanceEncounter();
                        break;
                    default:
                        break;
                }
            }

            void UpdateCurrentEncounterState(bool isBoss)
            {
                if (isBoss)
                    return;

                if (currentStageGuidList.empty())
                {
                    AdvanceEncounter();
                    return;
                }

                currentStageGuidList.remove_if([this](uint64 guid)
                {
                    Creature* creature = ObjectAccessor::GetCreature(*me, guid);
                    return !creature || !creature->IsAlive();
                });
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summon->AI()->DoZoneInCombat();
            }

            void AdvanceEncounter()
            {
                events.CancelEvent(EVENT_UPDATE_ENCOUNTER);

                switch (encounterStage)
                {
                    case 0:
                        events.ScheduleEvent(EVENT_HANDLE_SUMMONING, 9000);
                        events.ScheduleEvent(EVENT_HANDLE_SPELLCASTING, 5000);
                        events.ScheduleEvent(EVENT_NEXT_POT, 20000);
                        DoTalk(TALK_SUMMON1);
                        break;
                    case 1:
                        events.ScheduleEvent(EVENT_HANDLE_SUMMONING, 12000);
                        events.ScheduleEvent(EVENT_HANDLE_SPELLCASTING, 7000);
                        DoTalk(TALK_SUMMON2);
                        break;
                    case 2:
                        events.ScheduleEvent(EVENT_HANDLE_SUMMONING, 9000);
                        events.ScheduleEvent(EVENT_HANDLE_SPELLCASTING, 5000);
                        DoTalk(TALK_SUMMON3);
                        break;
                    case 3:
                        events.ScheduleEvent(EVENT_HANDLE_SPELLCASTING, 100);
                        events.ScheduleEvent(EVENT_HANDLE_SUMMONING, 5100);
                        break;
                    default:
                        break;
                }

                encounterStage++;
            }

            void DoHandleSpellcasting()
            {
                Creature* caster = GetClosestCreatureWithEntry(me, NPC_BARREL_TOSS_BUNNY, 10.f);
                if (!caster)
                    return;

                switch (encounterStage - 1)
                {
                    case STAGE_MIDDLE_ADDS:
                    case STAGE_SMALL_ADDS:
                    case STAGE_LARGE_ADDS:
                        caster->CastSpell(caster, brewSpells[encounterStage - 1], true);
                        break;
                    case STAGE_BOSS:
                        for (uint32 i = 0; i < 2; ++i)
                            caster->CastSpell(caster, brewSpells[i], true);
                        break;
                    default:
                        break;
                }
            }

            void DoTalk(uint8 id)
            {
                Talk(id);
                events.ScheduleEvent(EVENT_HANDLE_CRAFT_ANIMATION, 2100);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_OUTRO_1:
                            Talk(TALK_OUTRO_1);
                            events.ScheduleEvent(EVENT_OUTRO_2, 8900);
                            break;
                        case EVENT_OUTRO_2:
                            Talk(TALK_OUTRO_2);
                            me->SetStandState(UNIT_STAND_STATE_STAND);

                            if (instance)
                            {
                                if (Creature* chen = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CHEN_YANZHU)))
                                {
                                    chen->SetVisible(true);
                                    chen->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                                }
                            }
                            GetChenAndDoAction(0);
                            break;
                        case EVENT_OUTRO_3:
                            Talk(TALK_OUTRO_3);
                            GetChenAndDoAction(1);
                            break;
                        case EVENT_OUTRO_4:
                            Talk(TALK_OUTRO_4);
                            GetChenAndDoAction(2);
                            events.ScheduleEvent(EVENT_OUTRO_5, 6600);
                            break;
                        case EVENT_OUTRO_5:
                            Talk(TALK_OUTRO_5);
                            me->SetWalk(true);
                            events.ScheduleEvent(EVENT_MOVECHECK, 100);
                            break;
                        case EVENT_OUTRO_6:
                            Talk(TALK_OUTRO_6);
                            break;
                        case EVENT_OUTRO_7:
                            GetChenAndDoAction(3);
                            break;
                        case EVENT_MOVECHECK:
                            if (waypoint < 4)
                                me->GetMotionMaster()->MovePoint(waypoint, gaoWaypoints[waypoint]);
                            break;
                        case EVENT_HANDLE_SUMMONING:
                            events.ScheduleEvent(EVENT_UPDATE_ENCOUNTER, 3000);

                            switch (encounterStage - 1)
                            {
                                case STAGE_MIDDLE_ADDS:
                                    for (Position const& pos : middleBrewPos)
                                        SummonAdd(addStore[1], pos, SPELL_MEDIUM_SPAWN);
                                    break;
                                case STAGE_SMALL_ADDS:
                                    for (Position const& pos : smallBrewPos)
                                        SummonAdd(addStore[0], pos, SPELL_SMALL_SPAWN);
                                    break;
                                case STAGE_LARGE_ADDS:
                                    for (Position const& pos : largeBrewPos)
                                        SummonAdd(addStore[2], pos, SPELL_LARGE_SPAWN);
                                    break;
                                case STAGE_BOSS:
                                {
                                    events.CancelEvent(EVENT_UPDATE_ENCOUNTER);

                                    DoTalk(TALK_SUMMON_BOSS);
                                    if (Creature* creature = me->SummonCreature(NPC_YAN_ZHU, yanzhuPos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5 * MINUTE * IN_MILLISECONDS))
                                    {
                                        currentStageGuidList.push_back(creature->GetGUID());
                                        creature->CastSpell(creature, SPELL_LARGE_SPAWN, false);
                                        creature->CastSpell(creature, SPELL_MEDIUM_SPAWN, false);
                                        creature->CastSpell(creature, SPELL_SMALL_SPAWN, false);
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }
                            break;
                        case EVENT_HANDLE_SPELLCASTING:
                            DoHandleSpellcasting();
                            break;
                        case EVENT_UPDATE_ENCOUNTER:
                            events.ScheduleEvent(EVENT_UPDATE_ENCOUNTER, 1000);
                            UpdateCurrentEncounterState(encounterStage > STAGE_BOSS);
                            break;
                        case EVENT_NEXT_POT:
                            me->GetMotionMaster()->MovePoint(101, gaoPotPos);
                            break;
                        case EVENT_HANDLE_CRAFT_ANIMATION:
                            me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING);
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetStormstoutBreweryAI<npc_uncle_gaoAI>(creature);
        }
};

class npc_chen_yanzhu : public CreatureScript
{
    public:
        npc_chen_yanzhu() : CreatureScript("npc_chen_yanzhu") { }

        enum Talks
        {
            TALK_CARE,
            TALK_COST,
            TALK_VIRMEN,
            TALK_FIRE
        };

        enum Actions
        {
            ACTION_INIT,
            ACTION_OUTRO_1,
            ACTION_OUTRO_2,
            ACTION_OUTRO_3,
            ACTION_DONE,
        };

        enum Events
        {
            EVENT_NONE,
            EVENT_MOVECHECK,
            EVENT_MOVEBOSS,
            EVENT_CARE,
            EVENT_OUTRO_1,
            EVENT_OUTRO_2
        };

        struct npc_chen_yanzhuAI : public ScriptedAI
        {
            npc_chen_yanzhuAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                waypoint = 0;
            }

            uint32 waypoint;
            EventMap events;
            InstanceScript* instance;

            void Reset() OVERRIDE
            {
                InitializeMyself();
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_INIT:
                        waypoint = 0;
                        me->SetWalk(false);
                        events.ScheduleEvent(EVENT_MOVECHECK, 100);
                        me->SetVisible(true);
                        break;
                    case ACTION_OUTRO_1:
                        events.ScheduleEvent(EVENT_OUTRO_1, 10000);
                        break;
                    case ACTION_OUTRO_2:
                        events.ScheduleEvent(EVENT_OUTRO_2, 2500);
                        events.ScheduleEvent(EVENT_MOVECHECK, 6500);
                        me->SetWalk(true);
                        break;
                    case ACTION_OUTRO_3:
                        Talk(TALK_FIRE);
                        GetGaoAndDoAction(3);
                        break;
                    case ACTION_DONE:
                        InitializeMyself();
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (pointId)
                {
                    case 0:
                        events.ScheduleEvent(EVENT_MOVEBOSS, 100);
                        break;
                    case 1:
                        events.ScheduleEvent(EVENT_CARE, 2400);
                        break;
                    case 2:
                        me->SetFacingTo(chenWaypoints[pointId].GetOrientation());
                        break;
                    default:
                        break;
                }

                waypoint++;
            }

            void GetGaoAndDoAction(int32 action)
            {
                if (instance)
                    if (Creature* gao = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_UNCLE_GAO)))
                        gao->AI()->DoAction(action);
            }

            void InitializeMyself()
            {
                if (instance && instance->GetBossState(DATA_YAN_ZHU) == DONE)
                {
                    me->NearTeleportTo(chenWaypoints[2].GetPositionX(), chenWaypoints[2].GetPositionY(), chenWaypoints[2].GetPositionZ(), chenWaypoints[2].GetOrientation());
                    me->SetVisible(true);
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CARE:
                            Talk(TALK_CARE);
                            GetGaoAndDoAction(1);
                            break;
                        case EVENT_OUTRO_1:
                            Talk(TALK_COST);
                            GetGaoAndDoAction(2);
                            break;
                        case EVENT_OUTRO_2:
                            Talk(TALK_VIRMEN);
                            break;
                        case EVENT_MOVECHECK:
                            if (waypoint < 3)
                                me->GetMotionMaster()->MovePoint(waypoint, chenWaypoints[waypoint]);
                            break;
                        case EVENT_MOVEBOSS:
                            if (instance)
                            {
                                if (Creature* yanzhu = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_YAN_ZHU)))
                                {
                                    float x, y;
                                    GetPositionWithDistInOrientation(me, me->GetDistance(yanzhu) - 1.5f, me->GetAngle(yanzhu), x, y);
                                    me->GetMotionMaster()->MovePoint(1, x, y, me->GetMap()->GetHeight(x, y, me->GetPositionZ(), true, 100.f));
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetStormstoutBreweryAI<npc_chen_yanzhuAI>(creature);
        }
};

class boss_yanzhu : public CreatureScript
{
    public:
        boss_yanzhu() : CreatureScript("boss_yanzhu") { }

        enum Spells
        {
            SPELL_BLACKOUT             = 106851,
            SPELL_BREW_BOLT            = 114548,
            SPELL_BLOAT                = 106546,
            SPELL_CARBONATION          = 115003,
            SPELL_BUBBLE_SHIELD        = 106563,
        };

        enum Events
        {
            EVENT_NONE,
            EVENT_BLOAT,
            EVENT_BLACKOUT,
            EVENT_ADDS,
            EVENT_BUBBLE_SHIELD,
            EVENT_CARBONATION
        };

        enum Creatures
        {
            NPC_BUBBLE_SHIELD           = 65522
        };

        struct boss_yanzhuAI : public BossAI
        {
            boss_yanzhuAI(Creature* creature) : BossAI(creature, DATA_YAN_ZHU)
            {
                bubbleFacing = float(-M_PI / 4);
            }

            std::vector<uint64> bubbleGUIDs;
            float bubbleFacing;

            void InitializeAI() OVERRIDE
            {
                SetCombatMovement(false);
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                bubbleFacing = float(-M_PI / 4);

                for (YanZhuBrew const& brew : YanZhuBrews)
                    if (!me->HasAura(brew.spell))
                        me->AddAura(brew.spell, me);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (me->HasAura(SPELL_YEASTY_BREW))
                    events.ScheduleEvent(EVENT_ADDS, urand(9000, 17000));

                if (me->HasAura(SPELL_BUBBLING_BREW))
                {
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    events.ScheduleEvent(EVENT_BUBBLE_SHIELD, urand(6000, 14000));
                }

                events.ScheduleEvent(EVENT_BLACKOUT, 8000);
                events.ScheduleEvent(EVENT_BLOAT, 8000);
                events.ScheduleEvent(EVENT_CARBONATION, urand(9000, 17000));
            }

            void RemoveBubbles(bool exitVehicle)
            {
                for (uint64 guid : bubbleGUIDs)
                {
                    if (Creature* bubble = ObjectAccessor::GetCreature(*me, guid))
                    {
                        if (exitVehicle)
                            bubble->ExitVehicle();
                        bubble->AddObjectToRemoveList();
                    }
                }

                bubbleGUIDs.clear();
            }

            void EnterEvadeMode() OVERRIDE
            {
                if (!_EnterEvadeMode())
                    return;

                RemoveBubbles(true);
                me->GetMotionMaster()->MoveTargetedHome();
                Reset();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (Creature* gao = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_UNCLE_GAO)))
                    gao->AI()->DoAction(0);

                RemoveBubbles(false);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void DoSummonFizzyBubbles(uint32 count)
            {
                for (uint32 i = 0; i < count; ++i)
                {
                    Position pos;
                    me->GetRandomNearPosition(pos, 20.f);
                    pos.m_positionZ += frand(1.5f, 3.0f);

                    me->SummonCreature(NPC_FIZZY_BUBBLE, pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 12 * IN_MILLISECONDS);
                }
            }

            // Returns whether the boss should be allowed to chase its victim
            bool UpdateAttackState()
            {
                if (!me->isAttackReady() || me->HasUnitState(UNIT_STATE_CASTING))
                    return false;

                Unit* victim = me->GetVictim();
                if (!victim)
                    return true;

                if (me->IsWithinMeleeRange(victim))
                {
                    me->AttackerStateUpdate(victim);
                    me->resetAttackTimer();
                    return false;
                }

                if (!DoSpellAttackIfReady(SPELL_BREW_BOLT) && !IsCombatMovementAllowed())
                {
                    std::list<Unit*> targetList;
                    me->GetAttackableUnitListInRange(targetList, 35.f);

                    if (targetList.empty())
                        return true;

                    for (Unit* unit : targetList)
                    {
                        if (unit)
                        {
                            AttackStart(unit);
                            return false;
                        }
                    }

                    for (HostileReference* ref : me->getThreatManager().getThreatList())
                    {
                        if (Unit* unit = ref->getTarget())
                        {
                            if (me->GetDistance(unit) < 70.f)
                            {
                                AttackStart(unit);
                                return false;
                            }
                        }
                    }
                }

                return true;
            }

            Position GetBubbleShieldPos(float prevOri)
            {
                float x, y;
                GetPositionWithDistInOrientation(me, 3.0f, prevOri + float(M_PI / 4), x, y);

                Position pos;
                pos.Relocate(x, y, me->GetPositionZ() + 8.0f, prevOri + float(M_PI / 4));
                return pos;
            }

            void ApplyBubbleShield()
            {
                for (uint32 i = 0; i < 8; ++i)
                {
                    if (Creature* bubbleShield = me->SummonCreature(NPC_BUBBLE_SHIELD, GetBubbleShieldPos(bubbleFacing), TempSummonType::TEMPSUMMON_CORPSE_DESPAWN))
                    {
                        me->AddAura(SPELL_BUBBLE_SHIELD, me);
                        bubbleFacing += float(M_PI / 4);
                        bubbleGUIDs.push_back(bubbleShield->GetGUID());
                    }
                }
            }

            void DoSummonAdds(uint32 count)
            {
                for (uint32 i = 0; i < count; ++i)
                {
                    Position pos;
                    me->GetRandomNearPosition(pos, frand(5.f, 11.f));

                    if (Creature* add = me->SummonCreature(NPC_YEASTY_ALEMENTAL, pos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, urand(6000, 14000)))
                        add->AI()->DoZoneInCombat();
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLOAT:
                            if (Unit* unit = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.f, true))
                            {
                                DoCast(unit, SPELL_BLOAT);
                                events.ScheduleEvent(EVENT_BLOAT, urand(10000, 18000));
                            }
                            else
                                events.ScheduleEvent(EVENT_BLOAT, 1000);
                            break;
                        case EVENT_BLACKOUT:
                            DoCast(SPELL_BLACKOUT);
                            events.ScheduleEvent(EVENT_BLACKOUT, 8000);
                            break;
                        case EVENT_BUBBLE_SHIELD:
                            if (me->HasAura(SPELL_BUBBLE_SHIELD))
                            {
                                events.ScheduleEvent(EVENT_BUBBLE_SHIELD, 10 * IN_MILLISECONDS);
                                break;
                            }

                            bubbleFacing = float(-M_PI / 4);
                            ApplyBubbleShield();
                            events.ScheduleEvent(EVENT_BUBBLE_SHIELD, 30000);
                            break;
                        case EVENT_ADDS:
                            DoSummonAdds(8);
                            events.ScheduleEvent(EVENT_ADDS, 30000);
                            break;
                        case EVENT_CARBONATION:
                            events.ScheduleEvent(EVENT_CARBONATION, urand(35000, 48000));
                            DoCast(SPELL_CARBONATION);
                            DoSummonFizzyBubbles(6);
                            break;
                        default:
                            break;
                    }
                }

                SetCombatMovement(UpdateAttackState());
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetStormstoutBreweryAI<boss_yanzhuAI>(creature);
        }
};

// Shared behaviour of the alementals: pull the rest of the pack and go home without resetting
struct AlementalBaseAI : public ScriptedAI
{
    AlementalBaseAI(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE { }

    void PullPack()
    {
        std::list<Creature*> pack;
        GetCreatureListWithEntryInGrid(pack, me, me->GetEntry(), 10.f);

        for (Creature* creature : pack)
            creature->AI()->DoZoneInCombat();
    }

    void EnterEvadeMode() OVERRIDE
    {
        me->AttackStop();
        me->CombatStop(true);
        me->RemoveAllAurasExceptType(SPELL_AURA_DUMMY);
        me->SetLootRecipient(NULL);
        me->GetMotionMaster()->MoveTargetedHome();
    }

    // Casters stand still while the bolt is castable and chase into melee otherwise
    void DoBrewBoltOrMelee(uint32 brewBolt)
    {
        if (!DoSpellAttackIfReady(brewBolt))
        {
            SetCombatMovement(true);
            AttackStart(me->GetVictim());
            DoMeleeAttackIfReady();
            return;
        }

        SetCombatMovement(false);
    }
};

class npc_yeasty_alemental : public CreatureScript
{
    public:
        npc_yeasty_alemental() : CreatureScript("npc_yeasty_alemental") { }

        enum Spells
        {
            SPELL_BREW_BOLT     = 116155,
            SPELL_FERMENT       = 106859
        };

        enum Events
        {
            EVENT_NONE,
            EVENT_FERMENT
        };

        struct npc_yeasty_alementalAI : public AlementalBaseAI
        {
            npc_yeasty_alementalAI(Creature* creature) : AlementalBaseAI(creature)
            {
                instance = creature->GetInstanceScript();
                SetCombatMovement(false);
            }

            InstanceScript* instance;

            void EnterEvadeMode() OVERRIDE
            {
                ScriptedAI::EnterEvadeMode();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_FERMENT, 10000);
                PullPack();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_FERMENT && instance)
                    {
                        if (Creature* boss = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_YAN_ZHU)))
                            DoCast(boss, SPELL_FERMENT);

                        events.ScheduleEvent(EVENT_FERMENT, 15000);
                    }
                }

                DoBrewBoltOrMelee(SPELL_BREW_BOLT);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_yeasty_alementalAI(creature);
        }
};

class npc_sudsy_alemental : public CreatureScript
{
    public:
        npc_sudsy_alemental() : CreatureScript("npc_sudsy_alemental") { }

        enum Spells
        {
            SPELL_BREW_BOLT   = 115650,
            SPELL_SUDS        = 116178,
            SPELL_SUDS_AURA   = 116179
        };

        enum Events
        {
            EVENT_NONE,
            EVENT_SUDS,
            EVENT_BREW_BOLT
        };

        struct npc_sudsy_alementalAI : public AlementalBaseAI
        {
            npc_sudsy_alementalAI(Creature* creature) : AlementalBaseAI(creature) { }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_SUDS, urand(5000, 15000));
                events.ScheduleEvent(EVENT_BREW_BOLT, urand(4000, 11000));
                PullPack();
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summon->CastSpell(summon, SPELL_SUDS_AURA, true);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUDS:
                            DoCast(SPELL_SUDS);
                            events.ScheduleEvent(EVENT_SUDS, urand(10000, 16000));
                            break;
                        case EVENT_BREW_BOLT:
                            if (Unit* unit = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.f, true))
                                DoCast(unit, SPELL_BREW_BOLT);
                            events.ScheduleEvent(EVENT_BREW_BOLT, urand(6000, 14000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_sudsy_alementalAI(creature);
        }
};

class npc_fizzy_alemental : public CreatureScript
{
    public:
        npc_fizzy_alemental() : CreatureScript("npc_fizzy_alemental") { }

        enum Spells
        {
            SPELL_CARBONATION    = 116162,
            SPELL_BREW_BOLT      = 115650
        };

        enum Events
        {
            EVENT_NONE,
            EVENT_BREW_BOLT,
            EVENT_CARBONATION
        };

        struct npc_fizzy_alementalAI : public AlementalBaseAI
        {
            npc_fizzy_alementalAI(Creature* creature) : AlementalBaseAI(creature) { }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_CARBONATION, urand(5000, 15000));
                events.ScheduleEvent(EVENT_BREW_BOLT, urand(4000, 11000));
                PullPack();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BREW_BOLT:
                            if (Unit* unit = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.f, true))
                                DoCast(unit, SPELL_BREW_BOLT);
                            events.ScheduleEvent(EVENT_BREW_BOLT, urand(6000, 14000));
                            break;
                        case EVENT_CARBONATION:
                            if (Unit* unit = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.f, true))
                                DoCast(unit, SPELL_CARBONATION);
                            events.ScheduleEvent(EVENT_CARBONATION, urand(1600, 10000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_fizzy_alementalAI(creature);
        }
};

class npc_bloated_alemental : public CreatureScript
{
    public:
        npc_bloated_alemental() : CreatureScript("npc_bloated_alemental") { }

        enum Spells
        {
            SPELL_BLOAT        = 106546,
            SPELL_BREW_BOLT    = 115652
        };

        enum Events
        {
            EVENT_NONE,
            EVENT_BLOAT,
            EVENT_BREW_BOLT
        };

        struct npc_bloated_alementalAI : public AlementalBaseAI
        {
            npc_bloated_alementalAI(Creature* creature) : AlementalBaseAI(creature) { }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_BLOAT, urand(5000, 15000));
                events.ScheduleEvent(EVENT_BREW_BOLT, urand(4000, 11000));
                PullPack();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BREW_BOLT:
                            if (Unit* unit = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.f, true))
                                DoCast(unit, SPELL_BREW_BOLT);
                            events.ScheduleEvent(EVENT_BREW_BOLT, urand(6000, 14000));
                            break;
                        case EVENT_BLOAT:
                            if (Unit* unit = SelectTarget(SELECT_TARGET_RANDOM, 0, 99.f, true))
                                DoCast(unit, SPELL_BLOAT);
                            events.ScheduleEvent(EVENT_BLOAT, urand(8000, 16000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_bloated_alementalAI(creature);
        }
};

class npc_stout_alemental : public CreatureScript
{
    public:
        npc_stout_alemental() : CreatureScript("npc_stout_alemental") { }

        enum Spells
        {
            SPELL_BLACKOUT     = 106851,
            SPELL_BREW_BOLT    = 115652
        };

        enum Events
        {
            EVENT_NONE,
            EVENT_BLACKOUT,
            EVENT_BREW_BOLT
        };

        struct npc_stout_alementalAI : public AlementalBaseAI
        {
            npc_stout_alementalAI(Creature* creature) : AlementalBaseAI(creature) { }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_BLACKOUT, 8000);
                events.ScheduleEvent(EVENT_BREW_BOLT, urand(4000, 11000));
                PullPack();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BREW_BOLT:
                            if (Unit* unit = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.f, true))
                                DoCast(unit, SPELL_BREW_BOLT);
                            events.ScheduleEvent(EVENT_BREW_BOLT, urand(6000, 14000));
                            break;
                        case EVENT_BLACKOUT:
                            DoCast(SPELL_BLACKOUT);
                            events.ScheduleEvent(EVENT_BLACKOUT, urand(6000, 10000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_stout_alementalAI(creature);
        }
};

class npc_bubbling_alemental : public CreatureScript
{
    public:
        npc_bubbling_alemental() : CreatureScript("npc_bubbling_alemental") { }

        enum Spells
        {
            SPELL_BUBBLE_SHIELD = 128708,
            SPELL_BREW_BOLT     = 116155
        };

        enum Creatures
        {
            NPC_BUBBLE_SHIELD   = 65522
        };

        enum Events
        {
            EVENT_NONE,
            EVENT_BUBBLE_SHIELD,
            EVENT_MOVE,
        };

        struct npc_bubbling_alementalAI : public AlementalBaseAI
        {
            npc_bubbling_alementalAI(Creature* creature) : AlementalBaseAI(creature), wp(0) { }

            std::vector<uint64> bubbleGUIDs;
            EventMap cosmeticEvents;
            uint32 wp;

            void InitializeAI() OVERRIDE
            {
                me->GetMotionMaster()->Clear();

                if (!me->GetDBTableGUIDLow())
                    SetCombatMovement(false);

                cosmeticEvents.ScheduleEvent(EVENT_MOVE, 1500);
                wp = 0;
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                cosmeticEvents.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_BUBBLE_SHIELD, 5000);
                me->GetMotionMaster()->Clear();
                PullPack();
            }

            void ApplyBubbleShield(uint32 first)
            {
                for (uint32 i = first; i < 2; ++i)
                {
                    if (Creature* bubbleShield = me->SummonCreature(NPC_BUBBLE_SHIELD, me->GetPositionX() + frand(-1.5f, 1.5f), me->GetPositionY() + frand(-0.8f, 0.8f), me->GetPositionZ() + 3.0f, me->GetOrientation(), TempSummonType::TEMPSUMMON_CORPSE_DESPAWN))
                    {
                        me->AddAura(SPELL_BUBBLE_SHIELD, me);
                        bubbleGUIDs.push_back(bubbleShield->GetGUID());
                    }
                }
            }

            void RemoveBubbles(bool exitVehicle)
            {
                for (uint64 guid : bubbleGUIDs)
                {
                    if (Creature* bubble = ObjectAccessor::GetCreature(*me, guid))
                    {
                        if (exitVehicle)
                            bubble->ExitVehicle();
                        bubble->AddObjectToRemoveList();
                    }
                }

                bubbleGUIDs.clear();
            }

            void MovementInform(uint32 type, uint32 /*pointId*/) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (++wp > 3)
                    wp = 0;

                cosmeticEvents.ScheduleEvent(EVENT_MOVE, urand(100, 200));
            }

            void EnterEvadeMode() OVERRIDE
            {
                RemoveBubbles(true);
                AlementalBaseAI::EnterEvadeMode();
            }

            void JustDied(Unit* killer) OVERRIDE
            {
                RemoveBubbles(false);
                ScriptedAI::JustDied(killer);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                cosmeticEvents.Update(diff);

                if (cosmeticEvents.ExecuteEvent() == EVENT_MOVE)
                {
                    me->GetMotionMaster()->MovementExpired();
                    me->GetMotionMaster()->Clear();

                    Position const& node = BubblingAlementalPath[wp];
                    Position pos;
                    pos.Relocate(node.GetPositionX() + frand(-2.0f, 2.0f), node.GetPositionY() + frand(-1.0f, 4.0f), node.GetPositionZ(), node.GetOrientation());
                    me->GetMotionMaster()->MovePoint(wp, pos);
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_BUBBLE_SHIELD)
                    {
                        if (Aura* shield = me->GetAura(SPELL_BUBBLE_SHIELD))
                            ApplyBubbleShield(shield->GetStackAmount());
                        else
                            ApplyBubbleShield(0);

                        events.ScheduleEvent(EVENT_BUBBLE_SHIELD, 12000);
                    }
                }

                DoBrewBoltOrMelee(SPELL_BREW_BOLT);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_bubbling_alementalAI(creature);
        }
};

// Ground triggers that hold an area aura for a few seconds
struct AreaAuraTriggerAI : public ScriptedAI
{
    enum Events
    {
        EVENT_DESPAWN = 1,
        EVENT_APPLY_AURA
    };

    AreaAuraTriggerAI(Creature* creature, uint32 auraSpell, bool cast) : ScriptedAI(creature), _auraSpell(auraSpell), _cast(cast)
    {
        me->SetReactState(REACT_PASSIVE);
    }

    void Reset() OVERRIDE
    {
        if (me->HasAura(_auraSpell))
            return;

        _events.ScheduleEvent(EVENT_DESPAWN, 8200);
        _events.ScheduleEvent(EVENT_APPLY_AURA, 100);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_DESPAWN:
                    me->AddObjectToRemoveList();
                    return;
                case EVENT_APPLY_AURA:
                    if (_cast)
                        DoCast(_auraSpell);
                    else
                        me->AddAura(_auraSpell, me);
                    break;
                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    uint32 _auraSpell;
    bool _cast;
};

class npc_carbonate_trigger : public CreatureScript
{
    public:
        npc_carbonate_trigger() : CreatureScript("npc_carbonate_trigger") { }

        enum Spells
        {
            SPELL_CARBONATE_AURA  = 116168
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new AreaAuraTriggerAI(creature, SPELL_CARBONATE_AURA, true);
        }
};

class npc_suds_trigger : public CreatureScript
{
    public:
        npc_suds_trigger() : CreatureScript("npc_suds_trigger") { }

        enum Spells
        {
            SPELL_SUDS_AURA = 116179
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new AreaAuraTriggerAI(creature, SPELL_SUDS_AURA, false);
        }
};

class npc_bubble_shield : public CreatureScript
{
    public:
        npc_bubble_shield() : CreatureScript("npc_bubble_shield") { }

        enum Spells
        {
            SPELL_REMOVE_SHIELD     = 106563,
            SPELL_REMOVE_SHIELD2    = 128708
        };

        struct npc_bubble_shieldAI : public ScriptedAI
        {
            npc_bubble_shieldAI(Creature* creature) : ScriptedAI(creature), summonerGUID(0)
            {
                SetCombatMovement(false);
            }

            uint64 summonerGUID;

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                summonerGUID = summoner->GetGUID();
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (damage < me->GetHealth())
                    return;

                damage = 0;
                if (Unit* alemental = ObjectAccessor::GetUnit(*me, summonerGUID))
                    alemental->RemoveAuraFromStack(alemental->GetEntry() == NPC_YAN_ZHU ? SPELL_REMOVE_SHIELD : SPELL_REMOVE_SHIELD2);

                me->AddObjectToRemoveList();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_bubble_shieldAI(creature);
        }
};

class npc_fizzy_bubble : public CreatureScript
{
    public:
        npc_fizzy_bubble() : CreatureScript("npc_fizzy_bubble") { }

        enum Events
        {
            EVENT_NONE,
            EVENT_DESPAWN
        };

        enum Spells
        {
            SPELL_FIZZY_BUBBLE_AURA        = 114458,
            SPELL_FIZZY_BUBBLE_PLAYER_AURA = 114459,
        };

        struct npc_fizzy_bubbleAI : public ScriptedAI
        {
            npc_fizzy_bubbleAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void InitializeAI() OVERRIDE
            {
                me->CastSpell(me, SPELL_FIZZY_BUBBLE_AURA, false);
                me->SetDisableGravity(true);
                me->SetCanFly(true);
                Reset();
            }

            void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
            {
                clicker->AddAura(SPELL_FIZZY_BUBBLE_PLAYER_AURA, clicker);
                me->AddObjectToRemoveList();
            }

            void Reset() OVERRIDE
            {
                events.ScheduleEvent(EVENT_DESPAWN, 20000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_DESPAWN)
                    me->AddObjectToRemoveList();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_fizzy_bubbleAI(creature);
        }
};

class npc_wall_of_suds : public CreatureScript
{
    public:
        npc_wall_of_suds() : CreatureScript("npc_wall_of_suds") { }

        enum Spells
        {
            SPELL_WALL_OF_SUDS    = 114467,
        };

        struct npc_wall_of_sudsAI : public ScriptedAI
        {
            npc_wall_of_sudsAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_WALL_OF_SUDS, false);
                me->SetWalk(true);
                me->SetSpeed(MOVE_WALK, 0.9f);

                float x, y;
                GetPositionWithDistInOrientation(me, 90.0f, me->GetOrientation(), x, y);
                me->GetMotionMaster()->MovePoint(0, x, y, me->GetPositionZ());
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE && pointId == 0)
                    me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_wall_of_sudsAI(creature);
        }
};

// 106851 - Blackout Brew
class spell_blackout_brew : public SpellScriptLoader
{
    public:
        spell_blackout_brew() : SpellScriptLoader("spell_blackout_brew") { }

        enum Spells
        {
            SPELL_BLACKOUT_DRUNK = 106857
        };

        class spell_blackout_brew_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_blackout_brew_AuraScript);

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                Unit* owner = GetUnitOwner();
                if (!owner)
                    return;

                // Moving or jumping sobers the target up, standing still makes it worse
                if (owner->isMoving() || owner->HasUnitMovementFlag(MOVEMENTFLAG_FALLING))
                {
                    if (GetStackAmount() <= 1)
                        Remove(AURA_REMOVE_BY_EXPIRE);
                    else
                        SetStackAmount(GetStackAmount() - 1);
                }
                else if (GetStackAmount() + 1 >= 10)
                {
                    owner->AddAura(SPELL_BLACKOUT_DRUNK, owner);
                    Remove(AURA_REMOVE_BY_EXPIRE);
                }
                else
                    SetStackAmount(GetStackAmount() + 1);
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_blackout_brew_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_blackout_brew_AuraScript();
        }
};

// 106546 - Bloat
class spell_bloat_aura : public SpellScriptLoader
{
    public:
        spell_bloat_aura() : SpellScriptLoader("spell_bloat_aura") { }

        enum Misc
        {
            NPC_BLOATED_STALKER      = 59482,
            SPELL_FORCE_VEHICLE      = 46598,
            SPELL_GUSHING_BREW       = 106549
        };

        class spell_bloat_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_bloat_aura_AuraScript);

            void HandleOnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* owner = GetUnitOwner();
                if (!owner)
                    return;

                for (uint32 i = 0; i < 2; ++i)
                {
                    if (Creature* stalker = owner->SummonCreature(NPC_BLOATED_STALKER, owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), 0.0f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 30000))
                    {
                        stalker->CastSpell(owner, SPELL_FORCE_VEHICLE, true);
                        stalker->CastSpell(stalker, SPELL_GUSHING_BREW, true);
                    }
                }
            }

            void Register() OVERRIDE
            {
                AfterEffectApply += AuraEffectApplyFn(spell_bloat_aura_AuraScript::HandleOnApply, EFFECT_1, SPELL_AURA_SET_VEHICLE_ID, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_bloat_aura_AuraScript();
        }
};

// 114386 - Carbonation
class spell_carbonation : public SpellScriptLoader
{
    public:
        spell_carbonation() : SpellScriptLoader("spell_carbonation") { }

        class spell_carbonation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_carbonation_SpellScript);

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                float maxZ = GetCaster()->GetPositionZ() + 4.9f;
                targets.remove_if([maxZ](WorldObject* obj) { return obj->GetPositionZ() > maxZ; });
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_carbonation_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_carbonation_SpellScript::SelectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_carbonation_SpellScript();
        }
};

// 128254, 128256, 128258 - Brew Finale
class spell_brew_finale_wheat_effect : public SpellScriptLoader
{
    public:
        spell_brew_finale_wheat_effect() : SpellScriptLoader("spell_brew_finale_wheat_effect") { }

        class spell_brew_finale_wheat_effect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_brew_finale_wheat_effect_SpellScript);

            void SelectTargets(SpellDestination& dest)
            {
                Position pos;
                pos.Relocate(frand(-718.97f, -687.75f), frand(1147.53f, 1178.26f), 166.14f, 0.0f);
                dest.Relocate(pos);
            }

            void Register() OVERRIDE
            {
                OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_brew_finale_wheat_effect_SpellScript::SelectTargets, EFFECT_0, TARGET_DEST_DEST_RANDOM);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_brew_finale_wheat_effect_SpellScript();
        }
};

// 114459 - Fizzy Bubble
class spell_fizzi_buble : public SpellScriptLoader
{
    public:
        spell_fizzi_buble() : SpellScriptLoader("spell_fizzi_buble") { }

        class spell_fizzi_buble_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_fizzi_buble_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* owner = GetOwner()->ToPlayer())
                    owner->SetCanFly(true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* owner = GetOwner()->ToPlayer())
                    owner->SetCanFly(false);
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_fizzi_buble_AuraScript::OnApply, EFFECT_0, SPELL_AURA_FLY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_fizzi_buble_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_FLY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_fizzi_buble_AuraScript();
        }
};

void AddSC_boss_yanzhu()
{
    new npc_uncle_gao();
    new npc_chen_yanzhu();
    new boss_yanzhu();
    new npc_bubble_shield();
    new npc_yeasty_alemental();
    new npc_sudsy_alemental();
    new npc_fizzy_alemental();
    new npc_bloated_alemental();
    new npc_stout_alemental();
    new npc_bubbling_alemental();
    new npc_carbonate_trigger();
    new npc_suds_trigger();
    new npc_fizzy_bubble();
    new npc_wall_of_suds();
    new spell_blackout_brew();
    new spell_bloat_aura();
    new spell_carbonation();
    new spell_brew_finale_wheat_effect();
    new spell_fizzi_buble();
}
