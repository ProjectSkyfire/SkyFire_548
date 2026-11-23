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

#include "end_time.h"
#include "MoveSplineInit.h"
#include "Util.h"

enum Yells
{
    SAY_AGGRO       = 0,
    SAY_80          = 1,
    SAY_80_EMOTE    = 2,
    SAY_55          = 3,
    SAY_30          = 4,
    SAY_30_EMOTE    = 5,
    SAY_DEATH       = 6,
    SAY_INTRO_1     = 7,
    SAY_INTRO_2     = 8,
    SAY_INTRO_3     = 9,
    SAY_INTRO_4     = 10,
    SAY_INTRO_5     = 11,
    SAY_KILL        = 12,
    SAY_EYES        = 13,
    SAY_MOONLANCE   = 14,
    SAY_LIGHT_1     = 15,
    SAY_LIGHT_2     = 16,
    SAY_LIGHT_3     = 17,
    SAY_LIGHT_4     = 18,
    SAY_LIGHT_5     = 19,
    SAY_LIGHT_6     = 20,
    SAY_LIGHT_LEFT  = 21,
};

enum Spells
{
    SPELL_MOONBOLT                      = 102193,
    SPELL_DARK_MOONLIGHT                = 102414,
    SPELL_MOONLIGHT_COSMETIC            = 108642,
    SPELL_STARDUST                      = 102173,
    SPELL_MOONLANCE_AURA                = 102150,
    SPELL_MOONLANCE_DMG                 = 102149,
    SPELL_MOONLANCE_SUMMON_1            = 102151,
    SPELL_MOONLANCE_SUMMON_2            = 102152,
    SPELL_LUNAR_GUIDANCE                = 102472,
    SPELL_TEARS_OF_ELUNE                = 102241,
    SPELL_TEARS_OF_ELUNE_SCRIPT         = 102242,
    SPELL_TEARS_OF_ELUNE_MISSILE        = 102243,
    SPELL_TEARS_OF_ELUNE_DMG            = 102244,
    SPELL_EYES_OF_GODDESS               = 102608,
    SPELL_EYES_OF_GODDESS_STUN          = 102248,

    SPELL_PIERCING_GAZE_OF_ELUNE_AURA   = 102182,
    SPELL_PIERCING_GAZE_OF_ELUNE_DMG    = 102183,

    // event
    SPELL_IN_SHADOW                     = 101841,
    SPELL_MOONLIGHT                     = 101946,
    SPELL_SHRINK                        = 102002,

    SPELL_ACHIEVEMENT_CHECK             = 102491,
    SPELL_ACHIEVEMENT_FAIL              = 102539,
    SPELL_ACHIEVEMENT                   = 102542,
};

enum Events
{
    EVENT_MOONBOLT          = 1,
    EVENT_MOONLANCE         = 2,
    EVENT_STARDUST          = 3,
    EVENT_EYES_OF_GODDESS   = 4,

    EVENT_START_EVENT       = 5,
    EVENT_CHECK_PLAYERS     = 6,
    EVENT_SUMMON_ADDS       = 7,
    EVENT_SUMMON_POOL       = 8,
    EVENT_STOP_EVENT        = 9,
    EVENT_STOP_EVENT_1      = 10,
};

enum Adds
{
    NPC_MOONLANCE_1                 = 54574,
    NPC_MOONLANCE_2_1               = 54580,
    NPC_MOONLANCE_2_2               = 54581,
    NPC_MOONLANCE_2_3               = 54582,
    NPC_EYE_OF_ELUNE_1_E            = 54594,
    NPC_EYE_OF_ELUNE_1_W            = 54597,
    NPC_EYE_OF_ELUNE_2_E            = 54939,
    NPC_EYE_OF_ELUNE_2_W            = 54940,
    NPC_EYE_OF_ELUNE_3_E            = 54941,
    NPC_EYE_OF_ELUNE_3_W            = 54942,

    // for aura
    NPC_STALKER                     = 45979,

    // event
    NPC_POOL_OF_MOONLIGHT           = 54508,
    NPC_TIME_TWISTED_SENTINEL       = 54512,
    NPC_TIME_TWISTED_HUNTRESS       = 54701,
    NPC_TIME_TWISTED_NIGHTSABER_1   = 54688,
    NPC_TIME_TWISTED_NIGHTSABER_2   = 54699,
    NPC_TIME_TWISTED_NIGHTSABER_3   = 54700,
};

enum Other
{
    POINT_MOONLANCE     = 1,
    ACTION_START_EVENT  = 2,
    POINT_EYE           = 3,
};

const Position poolPos[5] =
{
    {2903.26f, 63.1788f, 3.2449f, 0.0f},
    {2862.83f, 131.462f, 3.18436f, 0.0f},
    {2756.57f, 129.971f, 5.58215f, 0.0f},
    {2695.44f, 28.7969f, 1.2324f, 0.0f},
    {2792.82f, 1.93924f, 2.46328f, 0.0f}
};

class boss_echo_of_tyrande : public CreatureScript
{
    public:
        boss_echo_of_tyrande() : CreatureScript("boss_echo_of_tyrande") { }

        struct boss_echo_of_tyrandeAI : public BossAI
        {
            boss_echo_of_tyrandeAI(Creature* creature) : BossAI(creature, DATA_ECHO_OF_TYRANDE)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->setActive(true);
                phase = 0;
                eventphase = 0;
                curPool = NULL;
                moonlight = false;
                me->SetStandState(UNIT_STAND_STATE_KNEEL);
                me->AddAura(SPELL_IN_SHADOW, me);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            void Reset() OVERRIDE
            {
                _Reset();

                moonlanceGUID = 0;
                phase = 0;
                eyesphase = -1;

                if (instance->GetData(DATA_TYRANDE_EVENT) == DONE)
                {
                    me->SetStandState(UNIT_STAND_STATE_STAND);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveAura(SPELL_IN_SHADOW);

                    if (!moonlight)
                        if (Creature* pStalker = me->SummonCreature(NPC_STALKER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f))
                            pStalker->CastSpell(pStalker, SPELL_MOONLIGHT_COSMETIC, true);

                    moonlight = true;

                    float x, y, z;
                    me->GetPosition(x, y, z);
                    me->CastSpell(x, y, z, SPELL_DARK_MOONLIGHT, true);
                }
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);

                // Quest
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->GetSource())
                            if (me->GetDistance2d(player) <= 150.0f && player->GetQuestStatus(30097) == QUEST_STATUS_INCOMPLETE)
                                DoCast(player, SPELL_ARCHIVED_TYRANDE, true);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                switch (summon->GetEntry())
                {
                    case NPC_MOONLANCE_1:
                    {
                        summon->SetDisplayId(11686);
                        Player* target = ObjectAccessor::GetPlayer(*me, moonlanceGUID);
                        if (!target)
                        {
                            summon->DespawnOrUnsummon();
                            return;
                        }

                        summon->SetOrientation(me->GetAngle(target));
                        Position pos = summon->GetNearPosition(15.0f, 0.0f);
                        summon->GetMotionMaster()->MovePoint(POINT_MOONLANCE, pos);
                        break;
                    }
                    case NPC_STALKER:
                        BossAI::JustSummoned(summon);
                        summon->SetDisplayId(11686);
                        break;
                    case NPC_POOL_OF_MOONLIGHT:
                    case NPC_MOONLANCE_2_1:
                    case NPC_MOONLANCE_2_2:
                    case NPC_MOONLANCE_2_3:
                        summon->SetDisplayId(11686);
                        break;
                }
            }

            void SpellHit(Unit* /*caster*/, const SpellInfo* spell) OVERRIDE
            {
                if (spell->HasEffect(SPELL_EFFECT_INTERRUPT_CAST))
                    if (me->GetCurrentSpell(CURRENT_GENERIC_SPELL))
                        if (me->GetCurrentSpell(CURRENT_GENERIC_SPELL)->m_spellInfo->Id == SPELL_MOONBOLT)
                            me->InterruptSpell(CURRENT_GENERIC_SPELL);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);

                phase = 0;
                eyesphase = -1;

                events.ScheduleEvent(EVENT_MOONBOLT, 1000);
                events.ScheduleEvent(EVENT_MOONLANCE, urand(12000, 13000));
                events.ScheduleEvent(EVENT_STARDUST, urand(7000, 8000));
                events.ScheduleEvent(EVENT_EYES_OF_GODDESS, urand(30000, 33000));

                instance->SetBossState(DATA_ECHO_OF_TYRANDE, IN_PROGRESS);
                DoZoneInCombat();
            }

            void SummonedCreatureDespawn(Creature* summon) OVERRIDE
            {
                BossAI::SummonedCreatureDespawn(summon);
                if (summon->GetEntry() == NPC_POOL_OF_MOONLIGHT)
                    Talk(SAY_LIGHT_LEFT);
            }

            void AttackStart(Unit* target) OVERRIDE
            {
                if (target)
                    me->Attack(target, false);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_START_EVENT)
                {
                    eventphase = 1;
                    summons.DespawnAll();
                    events.ScheduleEvent(EVENT_START_EVENT, 10000);
                }
            }

            uint32 GetData(uint32 /*type*/) const OVERRIDE
            {
                return eyesphase % 3;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim() && !eventphase)
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (phase == 0 && me->HealthBelowPct(80))
                {
                    phase = 1;
                    Talk(SAY_80);
                    DoCast(me, SPELL_LUNAR_GUIDANCE);
                    return;
                }
                else if (phase == 1 && me->HealthBelowPct(55))
                {
                    phase = 2;
                    Talk(SAY_55);
                    DoCast(me, SPELL_LUNAR_GUIDANCE);
                    return;
                }
                else if (phase == 2 && me->HealthBelowPct(30))
                {
                    phase = 3;
                    Talk(SAY_30);
                    DoCast(me, SPELL_TEARS_OF_ELUNE);
                    return;
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_START_EVENT:
                            events.ScheduleEvent(EVENT_SUMMON_POOL, 2000);
                            events.ScheduleEvent(EVENT_SUMMON_ADDS, 5000);
                            events.ScheduleEvent(EVENT_CHECK_PLAYERS, 5000);
                            break;
                        case EVENT_SUMMON_POOL:
                            switch (eventphase)
                            {
                                case 1: Talk(SAY_LIGHT_1); break;
                                case 2: Talk(SAY_LIGHT_2); break;
                                case 3: Talk(SAY_LIGHT_3); break;
                                case 4: Talk(SAY_LIGHT_4); break;
                                case 5: Talk(SAY_LIGHT_5); break;
                                default: break;
                            }
                            curPool = NULL;
                            curPool = me->SummonCreature(NPC_POOL_OF_MOONLIGHT, poolPos[eventphase - 1], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 40000);
                            if (eventphase < 5)
                            {
                                switch (eventphase)
                                {
                                    case 2: Talk(SAY_INTRO_2); break;
                                    case 3: Talk(SAY_INTRO_3); break;
                                    case 4: Talk(SAY_INTRO_4); break;
                                    default: break;
                                }
                                eventphase++;
                                events.ScheduleEvent(EVENT_SUMMON_POOL, 45000);
                            }
                            else
                            {
                                Talk(SAY_INTRO_5);
                                events.ScheduleEvent(EVENT_STOP_EVENT, 30000);
                                events.ScheduleEvent(EVENT_STOP_EVENT_1, 40000);
                            }
                            break;
                        case EVENT_SUMMON_ADDS:
                        {
                            if (Player* player = me->FindNearestPlayer(500.0f))
                            {
                                Position pos = player->GetRandomNearPosition(frand(15.0f, 20.0f));
                                uint32 entry = NPC_TIME_TWISTED_NIGHTSABER_1;
                                switch (urand(1, eventphase))
                                {
                                    case 1: entry = NPC_TIME_TWISTED_NIGHTSABER_1; break;
                                    case 2: entry = NPC_TIME_TWISTED_NIGHTSABER_2; break;
                                    case 3: entry = NPC_TIME_TWISTED_NIGHTSABER_3; break;
                                    case 4: entry = NPC_TIME_TWISTED_SENTINEL; break;
                                    case 5: entry = NPC_TIME_TWISTED_HUNTRESS; break;
                                }
                                if (Creature* creature = me->SummonCreature(entry, pos))
                                    creature->AI()->AttackStart(player);
                            }

                            events.ScheduleEvent(EVENT_SUMMON_ADDS, urand(5000, 10000));
                            break;
                        }
                        case EVENT_CHECK_PLAYERS:
                        {
                            uint8 num = 0;
                            Map::PlayerList const &playerList = instance->instance->GetPlayers();
                            if (!playerList.isEmpty())
                                for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                                    if (Player* player = itr->GetSource())
                                        if (player->GetAreaId() == AREA_EMERALD && player->IsAlive())
                                            num++;

                            if (!num)
                            {
                                summons.DespawnAll();
                                events.CancelEvent(EVENT_SUMMON_ADDS);
                                events.CancelEvent(EVENT_SUMMON_POOL);
                                events.CancelEvent(EVENT_CHECK_PLAYERS);
                                events.CancelEvent(EVENT_STOP_EVENT);
                                events.CancelEvent(EVENT_STOP_EVENT_1);
                                eventphase = 0;
                                instance->SetData(DATA_TYRANDE_EVENT, NOT_STARTED);
                            }
                            else
                                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 5000);
                            break;
                        }
                        case EVENT_STOP_EVENT:
                            events.CancelEvent(EVENT_SUMMON_ADDS);
                            events.CancelEvent(EVENT_SUMMON_POOL);
                            break;
                        case EVENT_STOP_EVENT_1:
                            Talk(SAY_LIGHT_6);
                            DoCastAOE(SPELL_ACHIEVEMENT);
                            events.CancelEvent(EVENT_CHECK_PLAYERS);
                            me->SetStandState(UNIT_STAND_STATE_STAND);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                            me->RemoveAura(SPELL_IN_SHADOW);

                            if (Creature* pStalker = me->SummonCreature(NPC_STALKER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f))
                                pStalker->CastSpell(pStalker, SPELL_MOONLIGHT_COSMETIC, true);
                            float x, y, z;
                            me->GetPosition(x, y, z);
                            me->CastSpell(x, y, z, SPELL_DARK_MOONLIGHT, true);
                            moonlight = true;

                            instance->SetData(DATA_TYRANDE_EVENT, DONE);
                            break;
                        case EVENT_MOONBOLT:
                            DoCastVictim(SPELL_MOONBOLT);
                            events.ScheduleEvent(EVENT_MOONBOLT, 3000);
                            break;
                        case EVENT_MOONLANCE:
                        {
                            Unit* target;
                            target = SelectTarget(SELECT_TARGET_RANDOM, 0, -10.0f, true);
                            if (!target)
                                target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);
                            if (target)
                            {
                                if (roll_chance_i(20))
                                    Talk(SAY_MOONLANCE);
                                moonlanceGUID = target->GetGUID();
                                DoCast(target, SPELL_MOONLANCE_SUMMON_1);
                            }
                            events.ScheduleEvent(EVENT_MOONLANCE, urand(12000, 13000));
                            break;
                        }
                        case EVENT_STARDUST:
                            DoCastAOE(SPELL_STARDUST);
                            events.ScheduleEvent(EVENT_STARDUST, urand(7000, 8000));
                            break;
                        case EVENT_EYES_OF_GODDESS:
                            Talk(SAY_EYES);
                            ++eyesphase;
                            DoCast(me, SPELL_EYES_OF_GODDESS);
                            events.ScheduleEvent(EVENT_EYES_OF_GODDESS, 20000);
                            break;
                        default:
                            break;
                    }
                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        break;
                }
            }

        private:
            uint8 phase;
            uint64 moonlanceGUID;
            uint8 eventphase;
            int32 eyesphase;
            Unit* curPool;
            bool moonlight;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<boss_echo_of_tyrandeAI>(creature);
        }
};

class npc_echo_of_tyrande_moonlance : public CreatureScript
{
    public:
        npc_echo_of_tyrande_moonlance() : CreatureScript("npc_echo_of_tyrande_moonlance") { }

        struct npc_echo_of_tyrande_moonlanceAI : public ScriptedAI
        {
            npc_echo_of_tyrande_moonlanceAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetSpeed(MOVE_RUN, (me->GetEntry() == NPC_MOONLANCE_1 ? 0.6f : 0.9f), true);
                SetCombatMovement(false);
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                DoZoneInCombat();
            }

            void AttackStart(Unit* /*target*/) OVERRIDE
            {
                return;
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == POINT_MOONLANCE)
                {
                    if (me->GetEntry() == NPC_MOONLANCE_1)
                    {
                        Position pos1_1 = me->GetNearPosition(3.0f, -(M_PI / 4.0f));
                        Position pos1_2 = me->GetNearPosition(30.0f, -(M_PI / 4.0f));
                        Position pos2_1 = me->GetNearPosition(3.0f, 0.0f);
                        Position pos2_2 = me->GetNearPosition(30.0f, 0.0f);
                        Position pos3_1 = me->GetNearPosition(3.0f, (M_PI / 4.0f));
                        Position pos3_2 = me->GetNearPosition(30.0f, (M_PI / 4.0f));
                        if (Creature* pLance1 = me->SummonCreature(NPC_MOONLANCE_2_1, pos1_1, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 30000))
                            pLance1->GetMotionMaster()->MovePoint(POINT_MOONLANCE, pos1_2);
                        if (Creature* pLance2 = me->SummonCreature(NPC_MOONLANCE_2_2, pos2_1, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 30000))
                            pLance2->GetMotionMaster()->MovePoint(POINT_MOONLANCE, pos2_2);
                        if (Creature* pLance3 = me->SummonCreature(NPC_MOONLANCE_2_3, pos3_1, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 30000))
                            pLance3->GetMotionMaster()->MovePoint(POINT_MOONLANCE, pos3_2);
                    }

                    me->DespawnOrUnsummon(500);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_echo_of_tyrande_moonlanceAI>(creature);
        }
};

class npc_echo_of_tyrande_pool_of_moonlight : public CreatureScript
{
    public:
        npc_echo_of_tyrande_pool_of_moonlight() : CreatureScript("npc_echo_of_tyrande_pool_of_moonlight") { }

        struct npc_echo_of_tyrande_pool_of_moonlightAI : public ScriptedAI
        {
            npc_echo_of_tyrande_pool_of_moonlightAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                SetCombatMovement(false);
            }

            uint32 uiShrinkTimer;
            uint32 uiDespawnTimer;

            void Reset() OVERRIDE
            {
                uiShrinkTimer = 5000;
                uiDespawnTimer = 60000;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (uiDespawnTimer <= diff)
                {
                    uiDespawnTimer = 60000;
                    me->DespawnOrUnsummon(500);
                }
                else
                    uiDespawnTimer -= diff;

                if (uiShrinkTimer <= diff)
                {
                    uiShrinkTimer = 2000;
                    DoCast(me, SPELL_SHRINK, true);
                }
                else
                    uiShrinkTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_echo_of_tyrande_pool_of_moonlightAI>(creature);
        }
};

class npc_echo_of_tyrande_eye_of_elune : public CreatureScript
{
    public:
        npc_echo_of_tyrande_eye_of_elune() : CreatureScript("npc_echo_of_tyrande_eye_of_elune") { }

        struct npc_echo_of_tyrande_eye_of_eluneAI : public ScriptedAI
        {
            npc_echo_of_tyrande_eye_of_eluneAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset() OVERRIDE
            {
                me->SetDisplayId(11686);
                DoCast(me, SPELL_PIERCING_GAZE_OF_ELUNE_AURA);

                float angle, dist;
                switch (me->GetEntry())
                {
                    case NPC_EYE_OF_ELUNE_1_E:
                    case NPC_EYE_OF_ELUNE_2_E:
                    case NPC_EYE_OF_ELUNE_3_E:
                        angle = M_PI / 2;
                        break;
                    case NPC_EYE_OF_ELUNE_1_W:
                    case NPC_EYE_OF_ELUNE_2_W:
                    case NPC_EYE_OF_ELUNE_3_W:
                        angle = -M_PI / 2;
                        break;
                    default:
                        return;
                }
                switch (me->GetEntry())
                {
                    case NPC_EYE_OF_ELUNE_1_E:
                    case NPC_EYE_OF_ELUNE_1_W:
                        dist = 16;
                        break;
                    case NPC_EYE_OF_ELUNE_2_E:
                    case NPC_EYE_OF_ELUNE_2_W:
                        dist = 23;
                        break;
                    case NPC_EYE_OF_ELUNE_3_E:
                    case NPC_EYE_OF_ELUNE_3_W:
                        dist = 30;
                        break;
                    default:
                        return;
                }

                me->SetCanFly(true);
                me->SetDisableGravity(true);

                float x, y, z;
                me->GetPosition(x, y, z);
                x += cos(angle) * dist;
                y += sin(angle) * dist;
                me->GetMotionMaster()->MovePoint(POINT_EYE, x, y, z);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE || pointId != POINT_EYE)
                    return;

                Creature* tyrande = me->FindNearestCreature(NPC_ECHO_OF_TYRANDE, 100);
                if (!tyrande)
                {
                    me->DespawnOrUnsummon();
                    return;
                }

                float dist = tyrande->GetExactDist2d(me);
                float startAngle = tyrande->GetAngle(me);

                Movement::PointsArray points;

                int32 const steps = 36;
                for (int32 step = 0; step < steps; ++step)
                {
                    float x, y, z;
                    float angle = startAngle - 2 * M_PI * step / steps;
                    tyrande->GetPosition(x, y, z);
                    x += cos(angle) * dist;
                    y += sin(angle) * dist;
                    points.push_back(G3D::Vector3(x, y, z));
                }

                Movement::MoveSplineInit init(me);
                init.SetCyclic();
                init.MovebyPath(points);
                init.SetSmooth();
                init.SetVelocity(7.7f);
                init.Launch();
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_echo_of_tyrande_eye_of_eluneAI>(creature);
        }
};

class spell_echo_of_tyrande_tears_of_elune_script : public SpellScriptLoader
{
    public:
        spell_echo_of_tyrande_tears_of_elune_script() : SpellScriptLoader("spell_echo_of_tyrande_tears_of_elune_script") { }

        class spell_echo_of_tyrande_tears_of_elune_script_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_echo_of_tyrande_tears_of_elune_script_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (roll_chance_i(50))
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_TEARS_OF_ELUNE_MISSILE, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_echo_of_tyrande_tears_of_elune_script_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_echo_of_tyrande_tears_of_elune_script_SpellScript();
        }
};

class spell_echo_of_tyrande_eyes_of_the_goddess : public SpellScriptLoader
{
    public:
        spell_echo_of_tyrande_eyes_of_the_goddess() : SpellScriptLoader("spell_echo_of_tyrande_eyes_of_the_goddess") { }

        class spell_echo_of_tyrande_eyes_of_the_goddess_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_echo_of_tyrande_eyes_of_the_goddess_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Unit* tyrande = GetCaster())
                    tyrande->CastSpell(tyrande, GetSpellInfo()->Effects[EFFECT_2 - tyrande->GetAI()->GetData(0)].BasePoints);
            }

            void Register() OVERRIDE
            {
                OnEffectHit += SpellEffectFn(spell_echo_of_tyrande_eyes_of_the_goddess_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_echo_of_tyrande_eyes_of_the_goddess_SpellScript();
        }
};

class at_et_tyrande : public AreaTriggerScript
{
    public:
        at_et_tyrande() : AreaTriggerScript("at_et_tyrande") { }

        bool OnTrigger(Player* player, const AreaTriggerEntry* /*trigger*/) OVERRIDE
        {
            if (!player)
                return true;

            if (player->IsBeingTeleported() || player->isBeingLoaded())
                return true;

            if (InstanceScript* instance = player->GetInstanceScript())
            {
                if (instance->GetData(DATA_TYRANDE_EVENT) != IN_PROGRESS &&
                    instance->GetData(DATA_TYRANDE_EVENT) != DONE)
                {
                    if (Creature* pTyrande = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_ECHO_OF_TYRANDE)))
                    {
                        pTyrande->AI()->Talk(SAY_INTRO_1);
                        pTyrande->AI()->DoAction(ACTION_START_EVENT);
                        instance->SetData(DATA_TYRANDE_EVENT, IN_PROGRESS);
                    }
                }
            }

            return true;
        }
};

void AddSC_boss_echo_of_tyrande()
{
    new boss_echo_of_tyrande();
    new npc_echo_of_tyrande_moonlance();
    new npc_echo_of_tyrande_pool_of_moonlight();
    new npc_echo_of_tyrande_eye_of_elune();
    new spell_echo_of_tyrande_tears_of_elune_script();
    new spell_echo_of_tyrande_eyes_of_the_goddess();
    new at_et_tyrande();
}
