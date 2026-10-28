/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Vehicle.h"
#include "AchievementMgr.h"
#include "CombatAI.h"

enum Spells
{
    // Garr
    SPELL_ANTIMAGIC_PULSE   = 93506,
    SPELL_MAGMA_SHACKLES    = 93507,
    SPELL_MASSIVE_ERUPTION  = 93508,

    // Firesworn
    SPELL_ERUPTION          = 93512,
    SPELL_IMMOLATE          = 93663,

    // Misc
    SPELL_EMERALD_DREAM_EFF = 75243,
};

enum eQuests
{
    QUEST_THROUGH_THE_DREAM = 25325,
};

enum Events
{
    // Garr
    EVENT_MAGMA_SHACKLES    = 1,
    EVENT_MASSIVE_ERUPTION  = 2,
    EVENT_ANTIMAGIC_PULSE   = 3,

    // Firesworn
    EVENT_IMMOLATE          = 4,
};

class npc_garr : public CreatureScript
{
    public:
        npc_garr() : CreatureScript("npc_garr") { }

        struct npc_garrAI : public ScriptedAI
        {
            npc_garrAI(Creature* creature) : ScriptedAI(creature)
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
            }
            
            EventMap events;

            void Reset() OVERRIDE
            {
                me->setActive(true);
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_MAGMA_SHACKLES, 30000);
                events.ScheduleEvent(EVENT_MASSIVE_ERUPTION, 32000);
                events.ScheduleEvent(EVENT_ANTIMAGIC_PULSE, urand(12000, 16000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;
                
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
                
                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MAGMA_SHACKLES:
                            DoCastAOE(SPELL_MAGMA_SHACKLES);
                            events.ScheduleEvent(EVENT_MAGMA_SHACKLES, 30000);
                            break;
                        case EVENT_MASSIVE_ERUPTION:
                            DoCastAOE(SPELL_MASSIVE_ERUPTION);
                            events.ScheduleEvent(EVENT_MASSIVE_ERUPTION, 30000);
                            break;
                        case EVENT_ANTIMAGIC_PULSE:
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(pTarget, SPELL_ANTIMAGIC_PULSE);
                            events.ScheduleEvent(EVENT_ANTIMAGIC_PULSE, urand(12000, 16000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_garrAI(creature);
        }
};

class npc_garr_firesworn : public CreatureScript
{
    public:
        npc_garr_firesworn() : CreatureScript("npc_garr_firesworn") { }

        struct npc_garr_fireswornAI : public ScriptedAI
        {
            npc_garr_fireswornAI(Creature* creature) : ScriptedAI(creature)
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
            }
            
            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_IMMOLATE, urand(5000, 7000));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                DoCastAOE(SPELL_ERUPTION);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;
                
                events.Update(diff);
                
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
                
                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_IMMOLATE:
                            DoCast(me->GetVictim(), SPELL_IMMOLATE);
                            events.ScheduleEvent(EVENT_IMMOLATE, urand(10000, 14000));
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }   
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_garr_fireswornAI(creature);
        }
};

/* class npc_lycanthoth : public CreatureScript
{
    public:
        npc_lycanthoth() : CreatureScript("npc_lycanthoth") { }

        struct npc_lycanthothAI : public ScriptedAI
        {
            npc_lycanthothAI(Creature* creature) : ScriptedAI(creature) { }

            void JustDied(Unit* killer) OVERRIDE
            {
                Unit* originalKiller = killer->GetCharmerOrOwnerOrSelf();
                if (originalKiller && (originalKiller->GetTypeId() == TYPEID_PLAYER))
                    killer->CastSpell(killer, (originalKiller->ToPlayer()->GetTeamId() == TEAM_HORDE ? 74077 : 74078), true);
            } 
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_lycanthothAI(creature);
        }
}; */

class npc_marion_wormswing : public CreatureScript
{
    public:
        npc_marion_wormswing() : CreatureScript("npc_marion_wormswing") { }

        struct npc_marion_wormswingAI : public ScriptedAI
        {
            npc_marion_wormswingAI(Creature* creature) : ScriptedAI(creature) { }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (damage > me->GetHealth() || me->HealthBelowPct(50) || me->HealthBelowPctDamaged(50, damage))
                {
                    if (Player* owner = attacker->GetCharmerOrOwnerPlayerOrPlayerItself())
                        if (owner->GetQuestStatus(25731) == QUEST_STATUS_INCOMPLETE)
                        {
                            owner->KilledMonsterCredit(41169, 0);
                            owner->KilledMonsterCredit(41170, 0);
                        }
                    me->setFaction(35);
                    me->DespawnOrUnsummon(5000);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_marion_wormswingAI(creature);
        }
};

/* class go_harpy_signal_fire : public GameObjectScript
{
    public:
        go_harpy_signal_fire() : GameObjectScript("go_harpy_signal_fire") { }

        bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
        {
            if (!go->FindNearestCreature(41112, 100.0f) && player->GetQuestStatus(25731) == QUEST_STATUS_INCOMPLETE)
                go->SummonCreature(41112, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 60000ms);
            return false;
        }
}; */

enum TrampolineSpells
{
    TRAMPOLINE_BOUNCE_1             = 79024,
    TRAMPOLINE_BOUNCE_2             = 79040,
    TRAMPOLINE_BOUNCE_3             = 79043,
    TRAMPOLINE_BOUNCE_4             = 79044,
    TRAMPOLINE_BOUNCE_5             = 79046,
    BOUNCE_ACHIEVEMENT_APPLY_AURA   = 95531,
};

class npc_soft_target : public CreatureScript
{
    public:
        npc_soft_target() : CreatureScript("npc_soft_target") { }

        struct npc_soft_targetAI : public ScriptedAI
        {
            npc_soft_targetAI(Creature* creature) : ScriptedAI(creature)
            {
                timer = 0;
                clearTimer = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (timer <= diff)
                {
                    std::list<Player*> players;
                    Skyfire::AnyPlayerInObjectRangeCheck check(me, 10.0f);
                    Skyfire::PlayerListSearcher<Skyfire::AnyPlayerInObjectRangeCheck> searcher(me, players, check);
                    me->VisitNearbyWorldObject(10.0f, searcher);

                    for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        if (Player* player = *itr)
                        {
                            if (player->IsFalling() || me->GetExactDist2d(player) > 2.5f || player->GetPositionZ() > 1366.0f)
                                continue;

                            std::map<uint64, uint32>::const_iterator itr = immunityTime.find(player->GetGUID());
                            if (itr != immunityTime.end() && getMSTime() < itr->second)
                                continue;

                            DoCast(player, BOUNCE_ACHIEVEMENT_APPLY_AURA, true);
                            DoCast(player, RAND(TRAMPOLINE_BOUNCE_1, TRAMPOLINE_BOUNCE_2, TRAMPOLINE_BOUNCE_3, TRAMPOLINE_BOUNCE_4, TRAMPOLINE_BOUNCE_5), true);
                            immunityTime[player->GetGUID()] = getMSTime() + 1000;
                        }
                    }

                    timer = 300;
                    clearTimer = 60000;
                }
                else
                    timer -= diff;

                if (clearTimer)
                {
                    if (clearTimer <= diff)
                    {
                        immunityTime.clear();
                        clearTimer = 0;
                    }
                    else
                        clearTimer -= diff;
                }
            }

        private:
            uint32 timer, clearTimer;
            std::map<uint64, uint32> immunityTime;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_soft_targetAI(creature);
        }
};

enum AngryLittleSquirrelEnums
{
    SPELL_ANGRY_LITTLE_ACORN = 96514,
};

class npc_angry_little_squirrel : public CreatureScript
{
    public:
        npc_angry_little_squirrel() : CreatureScript("npc_angry_little_squirrel") { }

        struct npc_angry_little_squirrelAI : public ScriptedAI
        {
            npc_angry_little_squirrelAI(Creature* creature) : ScriptedAI(creature), timer(0) { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (timer <= diff)
                {
                    std::list<Player*> players;
                    Skyfire::AnyPlayerInObjectRangeCheck check(me, 10.0f);
                    Skyfire::PlayerListSearcher<Skyfire::AnyPlayerInObjectRangeCheck> searcher(me, players, check);
                    me->VisitNearbyWorldObject(10.0f, searcher);

                    std::list<Unit*> enemies;

                    for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
                        if (Player* player = *itr)
                            if (player->IsInCombat())
                                for (auto const& threatEntry : player->getThreatManager().GetThreatenedByMeList())
                                    if (Unit* enemy = threatEntry.second->GetOwner())
                                        if (enemy->GetTypeId() == TypeID::TYPEID_UNIT && me->IsValidAttackTarget(enemy))
                                            enemies.push_back(enemy);

                    if (!enemies.empty())
                    {
                        if (Unit* target = Skyfire::Containers::SelectRandomContainerElement(enemies))
                        {
                            me->SetFacingToObject(target);
                            DoCast(target, SPELL_ANGRY_LITTLE_ACORN);
                        }
                    }

                    timer = 5000;
                }
                else
                    timer -= diff;
            }

        private:
            uint32 timer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_angry_little_squirrelAI(creature);
        }
};

class npc_wings_of_aviana : public CreatureScript
{
    public:
        npc_wings_of_aviana() : CreatureScript("npc_wings_of_aviana") { }

        struct npc_wings_of_avianaAI : public VehicleAI
        {
            npc_wings_of_avianaAI(Creature* creature) : VehicleAI(creature) { }

            void PassengerBoarded(Unit* who, int8 seatId, bool boarded) OVERRIDE
            {
                me->SetSpeed(MOVE_FLIGHT, me->GetSpeedRate(MOVE_RUN));
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_wings_of_avianaAI(creature);
        }
};

enum FlamewakerSentinelEnums
{
    SPELL_GRABBED           = 98169,
    NPC_WAVE_GENESIS_BUNNY  = 53099,
    POINT_LAVA              = 1,
};

class npc_flamewaker_sentinel : public CreatureScript
{
    public:
        npc_flamewaker_sentinel() : CreatureScript("npc_flamewaker_sentinel") { }

        struct npc_flamewaker_sentinelAI : public ScriptedAI
        {
            npc_flamewaker_sentinelAI(Creature* creature) : ScriptedAI(creature)
            {
                timer = 5000;
                me->SetReactState(REACT_DEFENSIVE);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE && pointId == POINT_LAVA && me->GetVehicleKit())
                {
                    timer = 30000;
                    if (Unit* passenger = me->GetVehicleKit()->GetPassenger(0))
                        passenger->ExitVehicle(GetClosestCreatureWithEntry(me, NPC_WAVE_GENESIS_BUNNY, 50.0f));

                    me->SetWalk(false);
                    if (me->GetVictim())
                        me->GetMotionMaster()->MoveChase(me->GetVictim());
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (timer <= diff)
                {
                    if (roll_chance_i(50))
                    {
                        timer = 60000;
                        std::list<Unit*> targets;
                        SelectTargetList(targets, 2, SELECT_TARGET_TOPAGGRO);
                        if (targets.size() >= 2)
                        {
                            if (Unit* target = targets.front())
                            {
                                target->CastSpell(me, SPELL_GRABBED, true);
                                if (Unit* bunny = GetClosestCreatureWithEntry(me, NPC_WAVE_GENESIS_BUNNY, 50.0f))
                                {
                                    float x, y, z;
                                    bunny->GetNearPoint(me, x, y, z, 2.5f, bunny->GetAngle(me));
                                    z += 5;
                                    me->UpdateAllowedPositionZ(x, y, z);
                                    me->SetWalk(true);
                                    me->GetMotionMaster()->MovePoint(POINT_LAVA, x, y, z, false);
                                    timer = 70000;
                                }
                            }
                        }
                    }
                    else
                        timer = 30000;
                }
                else
                    timer -= diff;

                if (me->GetVictim() && !me->GetVictim()->HasAura(SPELL_GRABBED))
                    DoMeleeAttackIfReady();
            }

        private:
            uint32 timer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_flamewaker_sentinelAI(creature);
        }
};

class npc_flame_lieutenant : public CreatureScript
{
    public:
        npc_flame_lieutenant() : CreatureScript("npc_flame_lieutenant") { }

        struct npc_flame_lieutenantAI : public ScriptedAI
        {
            npc_flame_lieutenantAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_DEFENSIVE);
            }

            // TODO someday: The rest of the AI
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_flame_lieutenantAI(creature);
        }
};

enum MorthisWhisperwingEnums
{
    QUEST_FIRE_IN_THE_SKIES     = 29290,
    NPC_FLAMEWAKER_CENTURION    = 53308,
    NPC_CINDERWEB_MATRIARCH     = 53479,
    NPC_MOLTEN_LORD             = 53310,
    SPELL_RAGEPYRE              = 101098,
    SPELL_FLASHFIRE             = 101099,
    SPELL_HEATFLAYER            = 101101,
    SPELL_BLAZEFURY             = 101097,
    SPELL_HATESPARK             = 101100,
    SPELL_SINGESLAYER           = 101103,
};

class npc_morthis_whisperwing : public CreatureScript
{
    public:
        npc_morthis_whisperwing() : CreatureScript("npc_morthis_whisperwing") { }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
        {
            if (!player || !creature || !quest || quest->GetQuestId() != QUEST_FIRE_IN_THE_SKIES)
                return false;

            for (uint32 i = 0; i < 100; ++i)
                player->KilledMonsterCredit(NPC_FLAMEWAKER_CENTURION, 0);
            for (uint32 i = 0; i < 40; ++i)
                player->KilledMonsterCredit(NPC_CINDERWEB_MATRIARCH, 0);
            for (uint32 i = 0; i < 3; ++i)
                player->KilledMonsterCredit(NPC_MOLTEN_LORD, 0);

            player->CastSpell(player, SPELL_RAGEPYRE, true);
            player->CastSpell(player, SPELL_FLASHFIRE, true);
            player->CastSpell(player, SPELL_HEATFLAYER, true);
            player->CastSpell(player, SPELL_BLAZEFURY, true);
            player->CastSpell(player, SPELL_HATESPARK, true);
            player->CastSpell(player, SPELL_SINGESLAYER, true);

            return true;
        }
};

enum q25464
{
    QUEST_THE_RETUN_OF_BARON_GEDDON = 25464,
    OBJECTIVE_BARON_GEDDON_WEAKENED = 266276,

    SPELL_INFERNO                   = 74813,
    SPELL_WEAKENING                 = 75192,
    SPELL_LIVING_BOMB               = 82924,

    EVENT_INFERNO                   = 1,

    SAY_GEDDON_LIVING_BOMB          = 0,

    NPC_BARON_GEDDON                = 40147,
    NPC_GALROND_OF_THE_CLAW         = 40148,

    SAY_GALROND_WARNING             = 0,
};

Position galrondRetreat = { 5468.942f, -2800.561f, 1518.113f };

class npc_baron_geddon : public CreatureScript
{
    public:
        npc_baron_geddon() : CreatureScript("npc_baron_geddon") { }

        struct npc_baron_geddonAI : public ScriptedAI
        {
            npc_baron_geddonAI(Creature* creature) : ScriptedAI(creature)
            {
                SetCombatMovement(false);
            }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_INFERNO, 18000);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (me->GetHealth() <= damage)
                    damage = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INFERNO:
                            DoCast(SPELL_INFERNO);
                            if (Creature* galrond = me->FindNearestCreature(NPC_GALROND_OF_THE_CLAW, 50.0f, true))
                            {
                                if (Player* player = galrond->FindNearestPlayer(50.0f))
                                    if (player->GetQuestStatus(QUEST_THE_RETUN_OF_BARON_GEDDON) == QUEST_STATUS_INCOMPLETE)
                                        galrond->AI()->Talk(SAY_GALROND_WARNING);
                                galrond->GetMotionMaster()->MovePoint(1, galrondRetreat);

                                uint32 delay = 0;
                                galrond->m_Events.Schedule(delay += 3000, [galrond]() { galrond->GetMotionMaster()->MoveIdle(); });
                                galrond->m_Events.Schedule(delay += 5000, [galrond]() { galrond->GetMotionMaster()->MoveTargetedHome(); });
                            }
                            events.ScheduleEvent(EVENT_INFERNO, 18000);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_baron_geddonAI(creature);
        }
};

enum asHyjalBurnsTypes
{
    SPELL_HYJAL_INTRO_FLIGHT_SUMM = 73518,
    SPELL_HYJAL_INTRO_PORT        = 73519,
    SPELL_ARONUS_CUSTOM_RIDE      = 76649,

    QUEST_AS_HYJAL_BURNS = 25316,

    TALK_INTRO = 0,
    TALK_SPECIAL_1,
    TALK_SPECIAL_2,
};

const Position aronusFlyPos  = { 7733.60f, -2507.66f, 499.51f, 0.0f };
const Position aronusTelePos = { 4688.42f, -3206.25f, 1130.532f, 3.074446f };

const Position aronusFlyPath1[3] =
{
    { 4639.80f, -3203.77f, 1080.91f, 0.0f },
    { 4626.59f, -3267.07f, 1068.14f, 0.0f },
    { 4619.12f, -3283.12f, 1058.53f, 0.0f },
};

const Position aronusFlyPath2[4] =
{
    { 4671.70f, -3222.37f, 1098.02f, 0.0f },
    { 4670.71f, -3150.38f, 1106.82f, 0.0f },
    { 4455.97f, -3121.23f, 1086.35f, 0.0f },
    { 4419.07f, -3123.98f, 1094.57f, 0.0f },
};

const Position aronusFlyPath3[5] =
{
    { 4320.86f, -3151.21f, 1080.19f, 0.0f },
    { 4236.82f, -3117.47f, 1067.25f, 0.0f },
    { 4174.11f, -3031.53f, 1045.60f, 0.0f },
    { 4137.78f, -3020.57f, 1038.21f, 0.0f },
    { 4090.87f, -3039.72f, 1003.84f, 0.0f },
};

const Position aronusFlyPath4[10] =
{
    { 4201.70f, -3100.97f, 1053.17f, 0.0f },
    { 4461.28f, -3101.87f, 1113.38f, 0.0f },
    { 4679.03f, -3176.81f, 1238.66f, 0.0f },
    { 4860.93f, -3407.13f, 1451.36f, 0.0f },
    { 4986.73f, -3519.42f, 1587.79f, 0.0f },
    { 5174.43f, -3554.13f, 1686.15f, 0.0f },
    { 5367.66f, -3691.16f, 1661.98f, 0.0f },
    { 5491.47f, -3818.22f, 1663.15f, 0.0f },
    { 5551.23f, -3706.56f, 1623.92f, 0.0f },
    { 5533.11f, -3630.15f, 1578.64f, 0.0f },
};

// Aronus 39140
struct npc_hyjal_aronus : public ScriptedAI
{
    npc_hyjal_aronus(Creature* creature) : ScriptedAI(creature)
    {
        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    }

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        if (clicker && clicker->ToPlayer() && clicker->ToPlayer()->GetQuestStatus(QUEST_AS_HYJAL_BURNS) == QUEST_STATUS_COMPLETE)
            clicker->CastSpell(clicker, SPELL_HYJAL_INTRO_FLIGHT_SUMM, true);
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

// Aronus Ride 39128
struct npc_hyjal_aronus_ride : public ScriptedAI
{
    npc_hyjal_aronus_ride(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 targetGUID;
    uint32 delay;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        targetGUID = summoner->GetGUID();
        me->OverrideInhabitType(INHABIT_AIR);
        me->SetAnimTier(ANIM_TIER_FLY);
        me->UpdateMovementFlags();
        me->SetSpeed(MOVE_FLIGHT, 2.5f);
        delay = 0;

        scheduler
            .Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            me->GetMotionMaster()->MovePoint(0, aronusFlyPos);

            delay = me->GetSplineDuration();
            scheduler
                .Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                DoCast(me, SPELL_HYJAL_INTRO_PORT);

                if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                {
                    target->UpdatePosition(aronusTelePos);
                    target->Relocate(aronusTelePos);
                }
            });

            scheduler
                .Schedule(Milliseconds(delay += 1500), [this](TaskContext context)
            {
                me->GetMotionMaster()->MovePoint(0, aronusTelePos.GetPositionX() + 5.0f, aronusTelePos.GetPositionY(), aronusTelePos.GetPositionZ(), aronusTelePos.GetOrientation()); // updating
            });

            scheduler
                .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
            {
                me->StopMoving();
                Movement::MoveSplineInit init(me);
                for (auto itr : aronusFlyPath1)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetFly();
                init.Launch();

                delay = me->GetSplineDuration() + 1500;
                scheduler
                    .Schedule(Milliseconds(delay), [this](TaskContext context)
                {
                    Talk(TALK_INTRO);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 1000), [this](TaskContext context)
                {
                    me->StopMoving();
                    Movement::MoveSplineInit init(me);
                    for (auto itr : aronusFlyPath2)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    init.SetFly();
                    init.Launch();

                    scheduler
                        .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                    {
                        Talk(TALK_SPECIAL_1);

                        me->SetSpeed(MOVE_FLIGHT, 4.0f);
                        me->StopMoving();
                        Movement::MoveSplineInit init(me);
                        for (auto itr : aronusFlyPath3)
                            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                        init.SetFly();
                        init.Launch();

                        scheduler
                            .Schedule(Milliseconds(me->GetSplineDuration() + 7800), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_2);

                            me->StopMoving();
                            Movement::MoveSplineInit init(me);
                            for (auto itr : aronusFlyPath4)
                                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                            init.SetFly();
                            init.SetSmooth();
                            init.Launch();

                            scheduler
                                .Schedule(Milliseconds(me->GetSplineDuration() + 1000), [this](TaskContext context)
                            {
                                if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                                {
                                    target->RemoveAurasDueToSpell(SPELL_ARONUS_CUSTOM_RIDE);
                                    target->_ExitVehicle();
                                    me->DespawnOrUnsummon(500);
                                }
                            });
                        });
                    });
                });
            });
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

class at_king_of_the_spider_hill : public AreaTriggerScript
{
    public:
        at_king_of_the_spider_hill() : AreaTriggerScript("at_king_of_the_spider_hill") { }
     
        bool OnTrigger(Player* player, const AreaTriggerEntry* /*trigger*/) OVERRIDE
        {
            if (player)
                player->CompletedAchievement(sAchievementMgr->GetAchievement(5872));

            return true;
        }
};

uint32 const sethriasRoostSummonSpells[] = { 96544, 96545, 96546, 96547, 96564, 96584, 96585, 96586, 96587, 96588, 96589, 96590, 96595, 96596, 96597, 96598, 96599, 96600, 96601, 96603, 96604, 96605, 96606, 96607, 96608, 96610, 96611, 96612, 96622, 96623, 96624, 96625, 96626, 96627, 96628, 96629, 99021, 99022, 99023, 99024, 99027, 99028, 99030, 99032 };

class spell_sethrias_roost_squad_aura : public SpellScriptLoader
{
    public:
        spell_sethrias_roost_squad_aura() : SpellScriptLoader("spell_sethrias_roost_squad_aura") { }

        class spell_sethrias_roost_squad_aura_AuraScript : public AuraScript
        {
            static std::set<uint32> sethriasRoostSummonCreatures;

            PrepareAuraScript(spell_sethrias_roost_squad_aura_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                sethriasRoostSummonCreatures.clear();
                for (uint32 i = 0; i < sizeof(sethriasRoostSummonSpells) / sizeof(sethriasRoostSummonSpells[0]); ++i)
                {
                    SpellInfo const* spell = sSpellMgr->GetSpellInfo(sethriasRoostSummonSpells[i]);
                    if (!spell || spell->Effects[EFFECT_0].Effect != SPELL_EFFECT_SUMMON || !spell->Effects[EFFECT_0].MiscValue)
                        return false;

                    sethriasRoostSummonCreatures.insert(spell->Effects[EFFECT_0].MiscValue);
                }
                return true;
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                uint32 spell = sethriasRoostSummonSpells[urand(0, sizeof(sethriasRoostSummonSpells) / sizeof(sethriasRoostSummonSpells[0]) - 1)];
                if (Unit* target = GetTarget())
                {
                    target->CastSpell(target, spell, true);

                    for (Unit::ControlList::iterator itr = target->m_Controlled.begin(); itr != target->m_Controlled.end(); ++itr)
                        if (Unit* controlled = *itr)
                            if (controlled->GetTypeId() == TypeID::TYPEID_UNIT && sethriasRoostSummonCreatures.find(controlled->GetEntry()) != sethriasRoostSummonCreatures.end())
                                if (!controlled->GetVictim())
                                    controlled->GetMotionMaster()->MoveFollow(target, PET_FOLLOW_ANGLE, PET_FOLLOW_DIST);
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    for (Unit::ControlList::iterator itr = target->m_Controlled.begin(); itr != target->m_Controlled.end(); ++itr)
                        if (Unit* controlled = *itr)
                            if (controlled->GetTypeId() == TypeID::TYPEID_UNIT && sethriasRoostSummonCreatures.find(controlled->GetEntry()) != sethriasRoostSummonCreatures.end())
                                controlled->ToCreature()->DespawnOrUnsummon(1);
            }

            void Register() OVERRIDE
            {
                AfterEffectApply += AuraEffectApplyFn(spell_sethrias_roost_squad_aura_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_sethrias_roost_squad_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_sethrias_roost_squad_aura_AuraScript();
        }
};

std::set<uint32> spell_sethrias_roost_squad_aura::spell_sethrias_roost_squad_aura_AuraScript::sethriasRoostSummonCreatures = std::set<uint32>();

class spell_weakening : public SpellScriptLoader
{
    public:
        spell_weakening() : SpellScriptLoader("spell_weakening") { }

        class spell_weakening_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_weakening_AuraScript);

            void OnUpdate(AuraEffect* /*aurEff*/)
            {
                WorldObject* target = GetOwner();
                Unit* caster = GetCaster();

                if (!caster || !target || target->GetEntry() != NPC_BARON_GEDDON)
                    return;

                if (Player* player = caster->ToPlayer())
                    if (player->GetQuestObjectiveCounter(OBJECTIVE_BARON_GEDDON_WEAKENED) == 19 && !player->HasAura(SPELL_LIVING_BOMB))
                    {
                        target->ToCreature()->AddAura(SPELL_LIVING_BOMB, player);
                        target->ToCreature()->AI()->Talk(SAY_GEDDON_LIVING_BOMB, player);
                    }

            }

            void Register() OVERRIDE
            {
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_weakening_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_weakening_AuraScript();
        }
};

class spell_geddon_inferno: public SpellScriptLoader
{
    public:
        spell_geddon_inferno() : SpellScriptLoader("spell_geddon_inferno") { }

        class spell_geddon_inferno_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_geddon_inferno_SpellScript);

            void HandleScript()
            {
                if (!GetCaster() || !GetHitPlayer())
                    return;

                uint32 baseDamage = 0;
                uint32 ticks = 1;

                if (AuraEffect const* aurEff = GetCaster()->GetAuraEffect(SPELL_INFERNO, EFFECT_0))
                    ticks = aurEff->GetTickNumber();

                switch (ticks)
                {
                    case 1:
                    case 2:
                        baseDamage = 1500;
                        break;
                    case 3:
                    case 4:
                        baseDamage = 3000;
                        break;
                    case 5:
                    case 6:
                        baseDamage = 6000;
                        break;
                    case 7:
                        baseDamage = 15000;
                        break;
                    case 8:
                        baseDamage = 30000; // possible changed after WoD (value from legion retail)
                        break;
                }

                SetHitDamage(baseDamage);
            }

            void Register() OVERRIDE
            {
                BeforeHit += SpellHitFn(spell_geddon_inferno_SpellScript::HandleScript);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_geddon_inferno_SpellScript();
        }
};

// 74180 - Twilight Firelance Equipped
class spell_twilight_firelance_equipped : public AuraScript
{
    PrepareAuraScript(spell_twilight_firelance_equipped);

    void UpdateVisibility(AuraEffect const*, AuraEffectHandleModes)
    {
        // Update Aviana's Guardians spellclick at client
        std::list<Creature*> list;
        GetUnitOwner()->GetCreatureListWithEntryInGridAppend(list, 40720, 50.0f);
        GetUnitOwner()->GetCreatureListWithEntryInGridAppend(list, 40723, 50.0f);
        for (auto& itr : list)
        {
            itr->ForceValuesUpdateAtIndex(UNIT_FIELD_NPC_FLAGS);
            itr->UpdateObjectVisibility();
        }
    }

    void Register() OVERRIDE
    {
        OnEffectApply  += AuraEffectApplyFn (spell_twilight_firelance_equipped::UpdateVisibility, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_twilight_firelance_equipped::UpdateVisibility, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// AreaTrigger 5876
class AreaTrigger_at_hyjal_alysra : public AreaTriggerScript
{
    public:
        AreaTrigger_at_hyjal_alysra() : AreaTriggerScript("AreaTrigger_at_hyjal_alysra") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) OVERRIDE
        {
            if (player->GetQuestStatus(QUEST_THROUGH_THE_DREAM) == QUEST_STATUS_INCOMPLETE)
                player->CompleteQuest(QUEST_THROUGH_THE_DREAM);

            return false;
        }
};

void AddSC_mount_hyjal()
{
    new npc_garr();
    new npc_garr_firesworn();
    // new npc_lycanthoth();
    new npc_marion_wormswing();
    // new go_harpy_signal_fire();
    new npc_soft_target();
    new npc_angry_little_squirrel();
    new npc_wings_of_aviana();
    new npc_flamewaker_sentinel();
    new npc_flame_lieutenant();
    new npc_morthis_whisperwing();
    new npc_baron_geddon();
    new creature_script<npc_hyjal_aronus>("npc_hyjal_aronus");
    new creature_script<npc_hyjal_aronus_ride>("npc_hyjal_aronus_ride");

    new at_king_of_the_spider_hill();
    new spell_sethrias_roost_squad_aura();
    new spell_weakening();
    new spell_geddon_inferno();
    new GenericAuraScriptLoader<spell_twilight_firelance_equipped>("spell_twilight_firelance_equipped");
    new AreaTrigger_at_hyjal_alysra();
}