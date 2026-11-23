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

#include "end_time.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"

enum MurozondScriptTexts
{
    SAY_AGGRO   = 0,
    SAY_DEATH   = 1,
    SAY_GLASS_1 = 2,
    SAY_GLASS_2 = 3,
    SAY_GLASS_3 = 4,
    SAY_GLASS_4 = 5,
    SAY_GLASS_5 = 6,
    SAY_INTRO_1 = 7,
    SAY_INTRO_2 = 8,
    SAY_KILL    = 9,
};

enum NozdormuScriptTexts
{
    SAY_NOZDORMU_INTRO      = 0,
    SAY_NOZDORMU_OUTRO_1    = 1,
    SAY_NOZDORMU_OUTRO_2    = 2,
    SAY_NOZDORMU_OUTRO_3    = 3,
    SAY_NOZDORMU_OUTRO_4    = 4,
};

enum Spells
{
    SPELL_INFINITE_BREATH               = 102569,
    SPELL_TEMPORAL_BLAST                = 102381,
    SPELL_TAIL_SWEEP                    = 108589,
    SPELL_FADING                        = 107550,
    SPELL_DISTORTION_BOMB               = 102516,
    SPELL_DISTORTION_BOMB_DMG           = 101984,
    SPELL_SANDS_OF_THE_HOURGLASS        = 102668,
    SPELL_TEMPORAL_SNAPSHOT             = 101592,
    SPELL_REWIND_TIME                   = 101590,
    SPELL_BLESSING_OF_BRONZE_DRAGONS    = 102364,
    SPELL_KILL_MUROZOND                 = 110158,
};

enum Events
{
    EVENT_INFINITE_BREATH   = 1,
    EVENT_TEMPORAL_BLAST    = 2,
    EVENT_TAIL_SWEEP        = 3,
    EVENT_DESPAWN           = 4,
    EVENT_DISTORTION_BOMB   = 5,
    EVENT_CHECK_ADDS        = 6,
    EVENT_INTRO_1           = 7,
    EVENT_INTRO_2           = 8,
    EVENT_INTRO_3           = 9,
    EVENT_NOZDORMU_INTRO    = 10,
    EVENT_CONTINUE          = 11,
    EVENT_NOZDORMU_OUTRO_1  = 12,
    EVENT_NOZDORMU_OUTRO_2  = 13,
    EVENT_NOZDORMU_OUTRO_3  = 14,
    EVENT_NOZDORMU_OUTRO_4  = 15,
    EVENT_NOZDORMU_OUTRO_5  = 16,
};

enum Adds
{
    NPC_MIRROR  = 54435,
};

enum Other
{
    POINT_LAND          = 1,
    ACTION_HOURGLASS    = 2,
    TYPE_HOURGLASS      = 3,
    ACTION_NOZDORMU     = 4,
};

const Position landPos = { 4169.71f, -433.40f, 120.0f, 2.59f };

// Until Temporal Snapshot is properly implemented...
uint32 const removableAuras[5] = { 25771, 57723, 57724, 80354, 95809 };

// Clears every cooldown of up to ten minutes, except the ones that expire at midnight
static void ResetPlayerCooldowns(Player* player)
{
    SpellCooldowns const cooldowns = player->GetSpellCooldowns();
    for (SpellCooldowns::const_iterator itr = cooldowns.begin(); itr != cooldowns.end(); ++itr)
    {
        SpellInfo const* entry = sSpellMgr->GetSpellInfo(itr->first);
        if (!entry)
            continue;
        if (entry->RecoveryTime > 10 * MINUTE * IN_MILLISECONDS || entry->CategoryRecoveryTime > 10 * MINUTE * IN_MILLISECONDS)
            continue;
        if (entry->CategoryEntry && entry->CategoryEntry->Flags & SPELL_CATEGORY_FLAG_COOLDOWN_EXPIRES_AT_MIDNIGHT)
            continue;
        player->RemoveSpellCooldown(itr->first, true);
    }
}

class boss_murozond : public CreatureScript
{
    public:
        boss_murozond() : CreatureScript("boss_murozond") { }

        struct boss_murozondAI : public BossAI
        {
            boss_murozondAI(Creature* creature) : BossAI(creature, DATA_MUROZOND)
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
                bDead = false;
                phase = 0;
                hourglass = 5;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            void Reset() OVERRIDE
            {
                _Reset();
                me->SetReactState(REACT_AGGRESSIVE);
                bDead = false;
                hourglass = 5;
                me->RemoveAllDynObjects();
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SANDS_OF_THE_HOURGLASS);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                if (type == TYPE_HOURGLASS)
                    return hourglass;
                return 0;
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (phase)
                    return;

                if (me->GetDistance2d(who) >= 60.0f)
                    return;

                phase = 1;
                events.ScheduleEvent(EVENT_INTRO_1, 2000);
                events.ScheduleEvent(EVENT_INTRO_2, 28000);
                events.ScheduleEvent(EVENT_INTRO_3, 48000);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_HOURGLASS)
                {
                    if (hourglass == 0)
                        return;

                    ActivateMirrors();
                    me->InterruptNonMeleeSpells(true);
                    me->RemoveAllDynObjects();
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    hourglass--;
                    SetHourglassPower(hourglass);
                    events.ScheduleEvent(EVENT_CONTINUE, 5000);
                    events.RescheduleEvent(EVENT_INFINITE_BREATH, urand(14000, 24000));
                    events.RescheduleEvent(EVENT_TAIL_SWEEP, 14000);
                    events.RescheduleEvent(EVENT_TEMPORAL_BLAST, 9000);
                    events.RescheduleEvent(EVENT_DISTORTION_BOMB, urand(9000, 12000));
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_INFINITE_BREATH, urand(10000, 20000));
                events.ScheduleEvent(EVENT_TAIL_SWEEP, 10000);
                events.ScheduleEvent(EVENT_TEMPORAL_BLAST, 5000);
                events.ScheduleEvent(EVENT_NOZDORMU_INTRO, 5000);
                events.ScheduleEvent(EVENT_DISTORTION_BOMB, urand(5000, 8000));

                DoCastAOE(SPELL_TEMPORAL_SNAPSHOT);
                instance->DoCastSpellOnPlayers(SPELL_BLESSING_OF_BRONZE_DRAGONS);
                instance->DoCastSpellOnPlayers(SPELL_SANDS_OF_THE_HOURGLASS);
                instance->SetBossState(DATA_MUROZOND, IN_PROGRESS);
                me->RemoveAllDynObjects();
                DoZoneInCombat();
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim && victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void CompleteEncounter()
            {
                // Achievement
                instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_KILL_MUROZOND, 0, me);

                // Guild Achievement
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                {
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                        if (Player* player = i->GetSource())
                            if (Group* pGroup = player->GetGroup())
                                if (player->GetGuildId() && pGroup->IsGuildGroup(player->GetGuildId(), player))
                                {
                                    if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
                                        guild->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_KILL_MUROZOND, 0, 0, me, player);
                                    break;
                                }
                    }
                }
                instance->UpdateEncounterState(EncounterCreditType::ENCOUNTER_CREDIT_CAST_SPELL, SPELL_KILL_MUROZOND, me);

                if (GameObject* go = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_HOURGLASS)))
                    go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);

                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->GetSource())
                            if (player->GetQuestStatus(QUEST_MUROZOND) == QUEST_STATUS_INCOMPLETE)
                                player->KilledMonsterCredit(54432, 0);
            }

            void SetHourglassPower(uint32 value)
            {
                Map::PlayerList const& players = instance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                    if (Player* player = i->GetSource())
                        player->SetPower(POWER_ALTERNATE_POWER, value);
            }

            void JustReachedHome() OVERRIDE
            {
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SANDS_OF_THE_HOURGLASS);
                BossAI::JustReachedHome();
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (damage >= me->GetHealth())
                    damage = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim() && phase != 1)
                    return;

                if (!bDead && me->HealthBelowPct(8))
                {
                    bDead = true;
                    me->InterruptNonMeleeSpells(false);
                    me->RemoveAllAuras();
                    DoCast(me, SPELL_FADING, true);
                    CompleteEncounter();
                    events.ScheduleEvent(EVENT_DESPAWN, 3000);
                    Talk(SAY_DEATH);
                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_1:
                            Talk(SAY_INTRO_1);
                            break;
                        case EVENT_INTRO_2:
                            Talk(SAY_INTRO_2);
                            break;
                        case EVENT_INTRO_3:
                            phase = 2;
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                            break;
                        case EVENT_NOZDORMU_INTRO:
                            if (Creature* pNozdormu = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_NOZDORMU)))
                                pNozdormu->AI()->Talk(SAY_NOZDORMU_INTRO);
                            break;
                        case EVENT_DISTORTION_BOMB:
                        {
                            Unit* target;
                            target = SelectTarget(SELECT_TARGET_RANDOM, 1, -20.0f, true);
                            if (!target)
                                target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true);
                            if (!target)
                                target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);
                            if (target)
                                DoCast(target, SPELL_DISTORTION_BOMB);
                            events.ScheduleEvent(EVENT_DISTORTION_BOMB, urand(5000, 8000));
                            break;
                        }
                        case EVENT_INFINITE_BREATH:
                            DoCastVictim(SPELL_INFINITE_BREATH);
                            events.ScheduleEvent(EVENT_INFINITE_BREATH, 15000);
                            break;
                        case EVENT_TAIL_SWEEP:
                            DoCast(me, SPELL_TAIL_SWEEP);
                            events.ScheduleEvent(EVENT_TAIL_SWEEP, 10000);
                            break;
                        case EVENT_TEMPORAL_BLAST:
                            DoCastAOE(SPELL_TEMPORAL_BLAST);
                            events.ScheduleEvent(EVENT_TEMPORAL_BLAST, urand(14000, 18000));
                            break;
                        case EVENT_CONTINUE:
                            Talk(SAY_GLASS_5 - hourglass);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->GetVictim());
                            break;
                        case EVENT_DESPAWN:
                            me->InterruptNonMeleeSpells(true);
                            me->RemoveAllDynObjects();
                            if (Creature* pNozdormu = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_NOZDORMU)))
                                pNozdormu->AI()->DoAction(ACTION_NOZDORMU);
                            instance->SetBossState(DATA_MUROZOND, DONE);
                            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SANDS_OF_THE_HOURGLASS);
                            me->DespawnOrUnsummon(3000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            uint8 phase;
            bool bDead;
            uint32 hourglass;

            void ActivateMirrors()
            {
                //std::list<Creature*> mirrorList;
                //me->GetCreatureListWithEntryInGrid(mirrorList, NPC_MIRROR, 500.0f);

                //if (mirrorList.empty())
                //    return;

                //for (std::list<Creature*>::const_iterator itr = mirrorList.begin(); itr != mirrorList.end(); ++itr)
                //{
                //    if (Creature* pMirror = (*itr)->ToCreature())
                //        if (pMirror->IsAlive() && pMirror->IsInWorld())
                //            pMirror->AI()->DoAction(ACTION_HOURGLASS);
                //}
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->GetSource())
                        {
                            if (!player->IsInWorld())
                                continue;

                            if (!player->IsAlive())
                                player->ResurrectPlayer(1.0f, false);

                            ResetPlayerCooldowns(player);

                            player->RemoveAura(SPELL_TEMPORAL_BLAST);
                            for (uint32 i = 0; i < sizeof(removableAuras) / sizeof(removableAuras[0]); ++i)
                                player->RemoveAurasDueToSpell(removableAuras[i]);

                            player->SetHealth(player->GetMaxHealth());
                            player->SetPower(player->getPowerType(), player->GetMaxPower(player->getPowerType()));

                            player->CastSpell(player, SPELL_BLESSING_OF_BRONZE_DRAGONS, true);
                        }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<boss_murozondAI>(creature);
        }
};

class npc_murozond_mirror_image : public CreatureScript
{
    public:
        npc_murozond_mirror_image() : CreatureScript("npc_murozond_mirror_image") { }

        struct npc_murozond_mirror_imageAI : public ScriptedAI
        {
            npc_murozond_mirror_imageAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
                m_owner = NULL;
                instance = me->GetInstanceScript();
                SetCombatMovement(false);
            }

            Player* m_owner;
            InstanceScript* instance;

            void Reset() OVERRIDE
            {
                if (!me->HasAura(SPELL_FADING))
                    me->AddAura(SPELL_FADING, me);
            }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                if (summoner && summoner->IsAlive() && summoner->IsInWorld())
                    if (summoner->GetTypeId() == TypeID::TYPEID_PLAYER)
                        m_owner = summoner->ToPlayer();
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_HOURGLASS)
                {
                    if (m_owner && m_owner->IsInWorld())
                    {
                        if (!m_owner->IsAlive())
                            m_owner->ResurrectPlayer(1.0f, false);

                        ResetPlayerCooldowns(m_owner);

                        m_owner->RemoveAura(SPELL_TEMPORAL_BLAST);
                        for (uint32 i = 0; i < sizeof(removableAuras) / sizeof(removableAuras[0]); ++i)
                            m_owner->RemoveAurasDueToSpell(removableAuras[i]);

                        m_owner->SetHealth(m_owner->GetMaxHealth());
                        m_owner->SetPower(m_owner->getPowerType(), m_owner->GetMaxPower(m_owner->getPowerType()));

                        m_owner->CastSpell(m_owner, SPELL_BLESSING_OF_BRONZE_DRAGONS, true);
                        m_owner->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), true);
                    }
                }
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (instance->GetBossState(DATA_MUROZOND) != IN_PROGRESS)
                {
                    me->DespawnOrUnsummon();
                    return;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_murozond_mirror_imageAI>(creature);
        }
};

class npc_end_time_nozdormu : public CreatureScript
{
    public:
        npc_end_time_nozdormu() : CreatureScript("npc_end_time_nozdormu") { }

        struct npc_end_time_nozdormuAI : public ScriptedAI
        {
            npc_end_time_nozdormuAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetVisible(false);
                me->setActive(true);
                bTalk = false;
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (!bTalk && action == ACTION_NOZDORMU)
                {
                    bTalk = true;
                    me->SetVisible(true);
                    events.ScheduleEvent(EVENT_NOZDORMU_OUTRO_1, 5000);
                    events.ScheduleEvent(EVENT_NOZDORMU_OUTRO_2, 20000);
                    events.ScheduleEvent(EVENT_NOZDORMU_OUTRO_3, 33000);
                    events.ScheduleEvent(EVENT_NOZDORMU_OUTRO_4, 41000);
                    events.ScheduleEvent(EVENT_NOZDORMU_OUTRO_5, 44000);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!bTalk)
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_NOZDORMU_OUTRO_1:
                            Talk(SAY_NOZDORMU_OUTRO_1);
                            break;
                        case EVENT_NOZDORMU_OUTRO_2:
                            Talk(SAY_NOZDORMU_OUTRO_2);
                            break;
                        case EVENT_NOZDORMU_OUTRO_3:
                            Talk(SAY_NOZDORMU_OUTRO_3);
                            break;
                        case EVENT_NOZDORMU_OUTRO_4:
                            if (GameObject* go = me->FindNearestGameObject(HOURGLASS_OF_TIME, 300.0f))
                                me->SetFacingToObject(go);
                            break;
                        case EVENT_NOZDORMU_OUTRO_5:
                            Talk(SAY_NOZDORMU_OUTRO_4);
                            bTalk = false;
                            break;
                    }
                }
            }

        private:
            bool bTalk;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_end_time_nozdormuAI>(creature);
        }
};

class go_murozond_hourglass_of_time : public GameObjectScript
{
    public:
        go_murozond_hourglass_of_time() : GameObjectScript("go_murozond_hourglass_of_time") { }

        bool OnReportUse(Player* /*player*/, GameObject* go) OVERRIDE
        {
            InstanceScript* instance = go->GetInstanceScript();
            if (!instance || instance->GetBossState(DATA_MUROZOND) != IN_PROGRESS)
                return false;

            if (Creature* pMurozond = ObjectAccessor::GetCreature(*go, instance->GetData64(DATA_MUROZOND)))
            {
                if (pMurozond->AI()->GetData(TYPE_HOURGLASS) == 0)
                    return true;

                pMurozond->AI()->DoAction(ACTION_HOURGLASS);
            }

            return false;
        }
};

void AddSC_boss_murozond()
{
    new boss_murozond();
    new npc_end_time_nozdormu();
    new npc_murozond_mirror_image();
    new go_murozond_hourglass_of_time();
}
