/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "MoveSplineInit.h"
#include "GameObjectAI.h"
#include "CombatAI.h"
#include "GameObject.h"
#include "Player.h"
#include "Vehicle.h"
#include "gate_of_the_setting_sun.h"

enum spells
{
    SPELL_EXPLOSE_GATE              = 115456,
    SPELL_BOMB_CAST_VISUAL          = 106729,
    SPELL_BOMB_AURA                 = 106875,
    SPELL_THROW_EXPLOSIVES          = 107130,
    SPELL_VOLATILE_MUNITIONS        = 109986,
    SPELL_RESIDUE_ACHIEVEMENT       = 118797,
    SPELL_AGGRO_NEARBY_TARGET       = 43263,
};

class AggroEvent : public BasicEvent
{
    public:
        AggroEvent(Creature* creature) : _creature(creature) { }

        bool Execute(uint64 /*time*/, uint32 /*diff*/) OVERRIDE
        {
            if (!_creature->IsInCombat())
                _creature->CastCustomSpell(SPELL_AGGRO_NEARBY_TARGET, SPELLVALUE_MAX_TARGETS, 1);

            return true;
        }

    private:
        Creature* _creature;
};

struct gss_trash_generic_baseAI : public ScriptedAI
{
    gss_trash_generic_baseAI(Creature* creature) : ScriptedAI(creature), randHealth(94.0f), summons(me) { }

    float randHealth;
    EventMap events;
    SummonList summons;

    void InitializeAI() OVERRIDE
    {
        randHealth = frand(92.f, 96.f);
        me->SetReactState(REACT_AGGRESSIVE);
        me->setActive(true);

        Reset();
    }

    void Reset() OVERRIDE
    {
        me->m_Events.AddEvent(new AggroEvent(me), me->m_Events.CalculateTime(500));
    }

    void MoveInLineOfSight(Unit* who) OVERRIDE
    {
        if (who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(who, 15.0f) && !who->IsFriendlyTo(me))
        {
            if ((me->GetVictim() && me->GetVictim()->GetTypeId() != TypeID::TYPEID_PLAYER) || !me->IsInCombat())
            {
                me->getHostileRefManager().deleteReferences();
                me->DeleteThreatList();
                me->AddThreat(who, 100.0f);
                AttackStart(who);
            }
        }
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        // The wall defenders only keep the mantids busy, players have to finish them
        if (attacker->GetTypeId() == TypeID::TYPEID_UNIT && !attacker->ToCreature()->IsPet())
        {
            if (me->GetHealthPct() <= randHealth || me->GetHealth() <= damage)
            {
                damage = 0;
                me->AddThreat(attacker, 0.f);
            }
        }
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->getThreatManager().resetAllAggro();
        me->getHostileRefManager().deleteReferences();
        me->DeleteThreatList();
    }
};

class npc_kirkthik_demolisher : public CreatureScript
{
    public:
        npc_kirkthik_demolisher() : CreatureScript("npc_kirkthik_demolisher") { }

        enum eEvents
        {
            EVENT_THROW_EXPLOSIVES = 1
        };

        enum eNpc
        {
            NPC_VOLATILE_MUNITIONS = 56896
        };

        struct npc_kirkthik_demolisherAI : public gss_trash_generic_baseAI
        {
            npc_kirkthik_demolisherAI(Creature* creature) : gss_trash_generic_baseAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
                events.ScheduleEvent(EVENT_THROW_EXPLOSIVES, urand(4 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                if (summon->GetEntry() == NPC_VOLATILE_MUNITIONS)
                    summon->CastSpell(summon, SPELL_VOLATILE_MUNITIONS, false);

                summons.Summon(summon);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events.ExecuteEvent() == EVENT_THROW_EXPLOSIVES)
                {
                    me->CastSpell((Unit*)NULL, SPELL_THROW_EXPLOSIVES, false);
                    events.ScheduleEvent(EVENT_THROW_EXPLOSIVES, urand(7 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_kirkthik_demolisherAI(creature);
        }
};

class npc_kirkthik_wind_shaper : public CreatureScript
{
    public:
        npc_kirkthik_wind_shaper() : CreatureScript("npc_kirkthik_wind_shaper") { }

        enum eEvents
        {
            EVENT_WIND_BLAST = 1,
            EVENT_GALE_FORCE = 2
        };

        enum eSpells
        {
            SPELL_WIND_BLAST = 115418,
            SPELL_GALE_FORCE = 115421
        };

        struct npc_kirkthik_wind_shaperAI : public gss_trash_generic_baseAI
        {
            npc_kirkthik_wind_shaperAI(Creature* creature) : gss_trash_generic_baseAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
                events.ScheduleEvent(EVENT_WIND_BLAST, 2 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_GALE_FORCE, urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case EVENT_WIND_BLAST:
                        me->CastSpell((Unit*)NULL, SPELL_WIND_BLAST, false);
                        events.ScheduleEvent(EVENT_WIND_BLAST, 2 * IN_MILLISECONDS);
                        break;
                    case EVENT_GALE_FORCE:
                        me->CastSpell(me, SPELL_GALE_FORCE, false);
                        events.ScheduleEvent(EVENT_GALE_FORCE, urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_kirkthik_wind_shaperAI(creature);
        }
};

class npc_kirkthik_conscript : public CreatureScript
{
    public:
        npc_kirkthik_conscript() : CreatureScript("npc_kirkthik_conscript") { }

        enum eEvents
        {
            EVENT_RESIN_BURST   = 1,
            EVENT_RESIN_RESIDUE = 2
        };

        enum eSpells
        {
            SPELL_RESIN_BURST   = 118787,
            SPELL_RESIN_RESIDUE = 118795,
            SPELL_FENZY         = 118777
        };

        struct npc_kirkthik_conscriptAI : public gss_trash_generic_baseAI
        {
            npc_kirkthik_conscriptAI(Creature* creature) : gss_trash_generic_baseAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
                me->SetHealth(me->CountPctFromMaxHealth(30));
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (attacker->GetTypeId() == TypeID::TYPEID_UNIT && !attacker->ToCreature()->IsPet())
                {
                    if (me->GetHealth() < me->GetMaxHealth() || me->GetHealth() <= damage)
                    {
                        damage = 0;
                        me->AddThreat(attacker, 0.f);
                    }
                }

                if (me->HealthBelowPctDamaged(15, damage) && !me->HasAura(SPELL_FENZY))
                    DoCast(me, SPELL_FENZY);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                DoCast(me, SPELL_RESIN_RESIDUE, true);
            }

            void EnterEvadeMode() OVERRIDE
            {
                ScriptedAI::EnterEvadeMode();
                me->RemoveAllAuras();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events.ExecuteEvent() == EVENT_RESIN_BURST)
                {
                    DoCast(me, SPELL_RESIN_BURST);
                    events.ScheduleEvent(EVENT_RESIN_BURST, urand(9 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_kirkthik_conscriptAI(creature);
        }
};

class npc_kirkthik_infiltrator : public CreatureScript
{
    public:
        npc_kirkthik_infiltrator() : CreatureScript("npc_kirkthik_infiltrator") { }

        enum eEvents
        {
            EVENT_SMOKE_BOMB = 1
        };

        enum eSpells
        {
            SPELL_SMOKE_BOMB = 128829
        };

        struct npc_kirkthik_infiltratortAI : public gss_trash_generic_baseAI
        {
            npc_kirkthik_infiltratortAI(Creature* creature) : gss_trash_generic_baseAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
                events.ScheduleEvent(EVENT_SMOKE_BOMB, urand(7 * IN_MILLISECONDS, 13 * IN_MILLISECONDS));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events.ExecuteEvent() == EVENT_SMOKE_BOMB)
                {
                    me->CastSpell(me, SPELL_SMOKE_BOMB, false);
                    events.ScheduleEvent(EVENT_SMOKE_BOMB, urand(7 * IN_MILLISECONDS, 13 * IN_MILLISECONDS));
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_kirkthik_infiltratortAI(creature);
        }
};

class npc_kirkthik_sapper : public CreatureScript
{
    public:
        npc_kirkthik_sapper() : CreatureScript("npc_kirkthik_sapper") { }

        enum eEvents
        {
            EVENT_PICK_A_PLAYER = 1
        };

        enum eSpells
        {
            SPELL_PICK_A_PLAYER = 107097
        };

        struct npc_kirkthik_sapperAI : public gss_trash_generic_baseAI
        {
            npc_kirkthik_sapperAI(Creature* creature) : gss_trash_generic_baseAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
                events.ScheduleEvent(EVENT_PICK_A_PLAYER, urand(7 * IN_MILLISECONDS, 13 * IN_MILLISECONDS));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events.ExecuteEvent() == EVENT_PICK_A_PLAYER)
                {
                    me->CastSpell(me, SPELL_PICK_A_PLAYER, false);
                    events.ScheduleEvent(EVENT_PICK_A_PLAYER, urand(11 * IN_MILLISECONDS, 17 * IN_MILLISECONDS));
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_kirkthik_sapperAI(creature);
        }
};

class npc_gss_trash_base : public CreatureScript
{
    public:
        npc_gss_trash_base() : CreatureScript("npc_gss_trash_base") { }

        struct npc_gss_trash_baseAI : public gss_trash_generic_baseAI
        {
            npc_gss_trash_baseAI(Creature* creature) : gss_trash_generic_baseAI(creature) { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_gss_trash_baseAI(creature);
        }
};

class npc_krikthik_bombarder : public CreatureScript
{
    public:
        npc_krikthik_bombarder() : CreatureScript("npc_krikthik_bombarder") { }

        struct npc_krikthik_bombarderAI : public ScriptedAI
        {
            npc_krikthik_bombarderAI(Creature* creature) : ScriptedAI(creature), bombTimer(5000)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                me->GetMotionMaster()->MoveRandom(5.0f);
                bombTimer = urand(1000, 7500);
            }

            void SpellHitTarget(Unit* target, SpellInfo const* /*spell*/) OVERRIDE
            {
                if (target->GetEntry() == NPC_BOMB_STALKER)
                    me->AddAura(SPELL_BOMB_AURA, target);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (bombTimer <= diff)
                {
                    if (pInstance)
                        if (Creature* stalker = pInstance->instance->GetCreature(pInstance->GetData64(DATA_RANDOM_BOMB_STALKER)))
                            if (!stalker->HasAura(SPELL_BOMB_AURA))
                                me->CastSpell(stalker, SPELL_BOMB_CAST_VISUAL, true);

                    bombTimer = urand(1000, 5000);
                }
                else
                    bombTimer -= diff;
            }

        private:
            InstanceScript* pInstance;
            uint32 bombTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_krikthik_bombarderAI(creature);
        }
};

class go_setting_sun_brasier : public GameObjectScript
{
    public:
        go_setting_sun_brasier() : GameObjectScript("go_setting_sun_brasier") { }

        enum eData
        {
            QUEST_LIGHTNING_THE_WAY = 31363,
            NPC_LTW_CREDIT          = 64553
        };

        static void SummonRimokTrash(InstanceScript* instance)
        {
            for (uint8 i = 0; i < 8; ++i)
            {
                uint32 entry = i < 4 ? NPC_KRITHIK_INFILTRATOR : (i < 6 ? NPC_KRITHIK_WND_SHAPER : NPC_KRITHIK_SHAPER);
                if (Creature* krithik = instance->instance->SummonCreature(entry, SummonPositionsRimok[i]))
                    krithik->HandleEmoteStateCommand(EMOTE_STATE_READY2H);
            }
        }

        bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
        {
            InstanceScript* instance = player->GetInstanceScript();
            if (!instance || instance->GetData(DATA_BRASIER_CLICKED) == DONE || instance->GetBossState(DATA_GADOK) != DONE)
                return true;

            instance->SetData(DATA_BRASIER_CLICKED, DONE);
            go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);

            Map::PlayerList const& playerList = go->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            {
                if (Player* member = i->GetSource())
                {
                    if (member->GetQuestStatus(QUEST_LIGHTNING_THE_WAY) == QUEST_STATUS_INCOMPLETE)
                        member->KilledMonsterCredit(NPC_LTW_CREDIT);

                    member->SendCinematicStart(CINEMATIC_SETTING_SUN);
                    member->NearTeleportTo(1370.0f, 2283.6f, 402.328f, 2.70f);
                }
            }

            if (Creature* rimok = ObjectAccessor::GetCreature(*go, instance->GetData64(DATA_RIMOK)))
            {
                rimok->SetVisible(true);
                if (rimok->IsAIEnabled)
                    rimok->AI()->Talk(2);
            }

            if (GameObject* signalFire = ObjectAccessor::GetGameObject(*go, instance->GetData64(DATA_SIGNAL_FIRE)))
                signalFire->SetGoState(GOState::GO_STATE_ACTIVE);

            SummonRimokTrash(instance);
            return false;
        }

        struct go_setting_sun_brasierAI : public GameObjectAI
        {
            go_setting_sun_brasierAI(GameObject* go) : GameObjectAI(go), restored(false) { }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (restored)
                    return;

                restored = true;
                go->setActive(true);

                InstanceScript* instance = go->GetInstanceScript();
                if (!instance || instance->GetBossState(DATA_GADOK) != DONE || instance->GetData(DATA_BRASIER_CLICKED) != DONE)
                    return;

                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);

                if (instance->GetBossState(DATA_RIMOK) != DONE)
                    SummonRimokTrash(instance);
            }

        private:
            bool restored;
        };

        GameObjectAI* GetAI(GameObject* go) const OVERRIDE
        {
            return new go_setting_sun_brasierAI(go);
        }
};

class vehicle_artillery_to_wall : public CreatureScript
{
    public:
        vehicle_artillery_to_wall() : CreatureScript("vehicle_artillery_to_wall") { }

        enum eSpells
        {
            SPELL_JUMP_TO_WALL = 131497
        };

        enum eEvents
        {
            EVENT_JUMP_TO_WALL = 1
        };

        struct vehicle_artillery_to_wallAI : public VehicleAI
        {
            vehicle_artillery_to_wallAI(Creature* creature) : VehicleAI(creature) { }

            void PassengerBoarded(Unit* /*who*/, int8 /*seatId*/, bool apply) OVERRIDE
            {
                if (apply)
                    events.ScheduleEvent(EVENT_JUMP_TO_WALL, 1 * IN_MILLISECONDS);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                VehicleAI::UpdateAI(diff);
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_JUMP_TO_WALL)
                {
                    if (Vehicle* vehicle = me->GetVehicleKit())
                    {
                        if (Unit* passenger = vehicle->GetPassenger(0))
                        {
                            if (Player* player = passenger->ToPlayer())
                            {
                                player->ExitVehicle();
                                player->CastSpell(player, SPELL_JUMP_TO_WALL, false);
                            }
                        }
                    }
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new vehicle_artillery_to_wallAI(creature);
        }
};

class npc_serpents_spine_marksman : public CreatureScript
{
    public:
        npc_serpents_spine_marksman() : CreatureScript("npc_serpents_spine_marksman") { }

        enum eEvents
        {
            EVENT_MISSILE     = 1,
            EVENT_KILL_MANTID = 2
        };

        enum eSpells
        {
            SPELL_MISSILE     = 106202,
            SPELL_KILL_MANTID = 115146
        };

        struct npc_serpents_spine_marksmanAI : public ScriptedAI
        {
            npc_serpents_spine_marksmanAI(Creature* creature) : ScriptedAI(creature) { }

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
                events.ScheduleEvent(EVENT_MISSILE, 1 * IN_MILLISECONDS);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
            {
                if (me->GetPositionX() > 860.0f) // the first group of archers shouldn't die at the wall corner
                    me->Kill(me);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case EVENT_MISSILE:
                        me->CastSpell(me, SPELL_MISSILE, false);
                        events.ScheduleEvent(urand(EVENT_MISSILE, EVENT_KILL_MANTID), 2 * IN_MILLISECONDS);
                        break;
                    case EVENT_KILL_MANTID:
                    {
                        std::list<Creature*> spitters;
                        me->GetCreatureListWithEntryInGrid(spitters, NPC_KRITHIK_GLIDER, 105.0f);
                        if (!spitters.empty())
                            me->CastSpell(Skyfire::Containers::SelectRandomContainerElement(spitters), SPELL_KILL_MANTID, false);

                        events.ScheduleEvent(urand(EVENT_MISSILE, EVENT_KILL_MANTID), 2 * IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_serpents_spine_marksmanAI(creature);
        }
};

class npc_kirthik_glider : public CreatureScript
{
    public:
        npc_kirthik_glider() : CreatureScript("npc_kirthik_glider") { }

        struct npc_kirthik_gliderAI : public ScriptedAI
        {
            npc_kirthik_gliderAI(Creature* creature) : ScriptedAI(creature) { }

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
                float x, y;
                GetPositionWithDistInOrientation(me, 130.0f, 4.743f, x, y);
                Movement::MoveSplineInit init(me);
                init.MoveTo(x, y, me->GetPositionZ());
                init.SetFly();
                init.SetVelocity(10.0f);
                init.Launch();

                me->DespawnOrUnsummon(me->GetSplineDuration());
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->StopMoving();
                me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_kirthik_gliderAI(creature);
        }
};

// Krithik Rager 59800
class npc_kirthik_rager : public CreatureScript
{
    public:
        npc_kirthik_rager() : CreatureScript("npc_kirthik_rager") { }

        enum eSpells
        {
            SPELL_SAVAGE_CLEAVE = 115436,
            SPELL_ENRAGE        = 115430,
        };

        enum eEvents
        {
            EVENT_SAVAGE = 1,
        };

        struct npc_kirthik_ragerAI : public ScriptedAI
        {
            npc_kirthik_ragerAI(Creature* creature) : ScriptedAI(creature), hasEnrage(false) { }

            void Reset() OVERRIDE
            {
                events.Reset();
                hasEnrage = false;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_SAVAGE, 6 * IN_MILLISECONDS);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
            {
                if (HealthBelowPct(30) && !hasEnrage)
                {
                    hasEnrage = true;
                    me->CastSpell(me, SPELL_ENRAGE, false);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_SAVAGE)
                {
                    if (Unit* victim = me->GetVictim())
                        me->CastSpell(victim, SPELL_SAVAGE_CLEAVE, false);

                    events.ScheduleEvent(EVENT_SAVAGE, urand(6 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
            bool hasEnrage;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_kirthik_ragerAI(creature);
        }
};

class npc_great_wall_explosion_target_stalker : public CreatureScript
{
    public:
        npc_great_wall_explosion_target_stalker() : CreatureScript("npc_great_wall_explosion_target_stalker") { }

        enum eSpells
        {
            SPELL_DESTROY_WALL_CORNER_A = 124993,
            SPELL_DESTROY_WALL_CORNER_B = 114906,
            SPELL_DESTROY_WALL_CORNER_C = 125000,
            SPELL_BOULDER               = 114971
        };

        enum eActions
        {
            ACTION_DESTROY_WALL_CORNER_A = 1,
            ACTION_DESTROY_WALL_CORNER_B = 2,
            ACTION_DESTROY_WALL_CORNER_C = 3
        };

        enum eEvents
        {
            EVENT_DESTROY = 1
        };

        struct npc_great_wall_explosion_target_stalkerAI : public ScriptedAI
        {
            npc_great_wall_explosion_target_stalkerAI(Creature* creature) : ScriptedAI(creature),
                cornerAdestroyed(false), cornerBdestroyed(false), cornerCdestroyed(false)
            {
                instance = creature->GetInstanceScript();
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_DESTROY_WALL_CORNER_A:
                        if (!cornerAdestroyed)
                        {
                            cornerAdestroyed = true;
                            if (Creature* caster = me->FindNearestCreature(NPC_EXPLOSION_CASTER, 100.0f))
                            {
                                caster->CastSpell(me, SPELL_DESTROY_WALL_CORNER_A, false);
                                caster->CastSpell(me, SPELL_BOULDER, false);
                            }
                        }
                        break;
                    case ACTION_DESTROY_WALL_CORNER_B:
                        if (!cornerBdestroyed)
                        {
                            cornerBdestroyed = true;
                            if (Creature* caster = me->FindNearestCreature(NPC_EXPLOSION_CASTER, 100.0f))
                                caster->CastSpell(me, SPELL_DESTROY_WALL_CORNER_B, false);

                            events.ScheduleEvent(EVENT_DESTROY, 2600);
                        }
                        break;
                    case ACTION_DESTROY_WALL_CORNER_C:
                        if (!cornerCdestroyed)
                        {
                            cornerCdestroyed = true;
                            if (Creature* caster = me->FindNearestCreature(NPC_EXPLOSION_CASTER, 200.0f))
                                caster->CastSpell(me, SPELL_DESTROY_WALL_CORNER_C, false);
                        }
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_DESTROY)
                    if (instance)
                        instance->SetData(DATA_CORNER_B, DONE);
            }

        private:
            InstanceScript* instance;
            EventMap events;
            bool cornerAdestroyed, cornerBdestroyed, cornerCdestroyed;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_great_wall_explosion_target_stalkerAI(creature);
        }
};

float const TraineeWPPath[7][3] =
{
    { 1233.126f, 2304.802f, 388.1548f },
    { 1240.626f, 2304.552f, 388.1548f },
    { 1248.126f, 2304.552f, 383.6548f },
    { 1253.126f, 2304.302f, 381.6548f },
    { 1259.126f, 2299.802f, 381.9048f },
    { 1264.626f, 2309.052f, 381.9048f },
    { 1271.626f, 2298.802f, 382.1548f }
};

class npc_great_wall_before_gadok_intro : public CreatureScript
{
    public:
        npc_great_wall_before_gadok_intro() : CreatureScript("npc_great_wall_before_gadok_intro") { }

        enum eActions
        {
            ACTION_TALK_INTRO = 1,
            ACTION_CANNONEER  = 2
        };

        enum eTalks
        {
            TALK_INTRO = 0
        };

        enum eEvents
        {
            EVENT_TALK_INTRO = 1,
            EVENT_MOVE       = 2,
            EVENT_CANN_1     = 3, // left
            EVENT_CANN_2     = 4, // left
            EVENT_CANN_3     = 5,
            EVENT_CANN_4     = 6
        };

        struct npc_great_wall_before_gadok_introAI : public ScriptedAI
        {
            npc_great_wall_before_gadok_introAI(Creature* creature) : ScriptedAI(creature), introTalked(false) { }

            void InitializeAI() OVERRIDE
            {
                introTalked = false;
                if (me->GetEntry() == NPC_SERPENTS_SPINE_DEF_A && me->GetInstanceScript() && me->GetInstanceScript()->GetBossState(DATA_RIMOK) == DONE)
                    ScheduleCannoneers();
            }

            void ScheduleCannoneers()
            {
                events.ScheduleEvent(EVENT_CANN_1, urand(2 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_CANN_3, urand(2 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_TALK_INTRO)
                {
                    if (introTalked)
                        return;

                    introTalked = true;
                    switch (me->GetEntry())
                    {
                        case NPC_SERPENTS_SPINE_DEF_A:
                            events.ScheduleEvent(EVENT_TALK_INTRO, 3 * IN_MILLISECONDS);
                            break;
                        case NPC_SERPENTS_SPINE_DEF_B:
                            events.ScheduleEvent(EVENT_TALK_INTRO, 8 * IN_MILLISECONDS);
                            break;
                        case NPC_SERPENTS_SPINE_TRAINEE:
                            events.ScheduleEvent(EVENT_TALK_INTRO, 12 * IN_MILLISECONDS);
                            events.ScheduleEvent(EVENT_MOVE, 14 * IN_MILLISECONDS);
                            break;
                        default:
                            break;
                    }
                }
                else if (action == ACTION_CANNONEER)
                    ScheduleCannoneers();
            }

            // Left cannoneers come from positions 0/1 and leave through the left path, right ones from 2/3
            void SummonCannoneer(uint8 posIndex)
            {
                if (Creature* cann = me->SummonCreature(NPC_CANNONEER, PandarenCannoneerPos[posIndex]))
                {
                    if (cann->IsAIEnabled)
                        cann->AI()->DoAction(posIndex < 2 ? 1 : 2);

                    uint32 nextEvent = posIndex < 2 ? EVENT_CANN_1 : EVENT_CANN_3;
                    events.ScheduleEvent(nextEvent + urand(0, 1), urand(3 * IN_MILLISECONDS, 6 * IN_MILLISECONDS));
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_TALK_INTRO:
                        Talk(TALK_INTRO);
                        break;
                    case EVENT_MOVE:
                    {
                        Movement::MoveSplineInit init(me);
                        for (uint8 i = 0; i < 7; ++i)
                            init.Path().push_back(G3D::Vector3(TraineeWPPath[i][0], TraineeWPPath[i][1], TraineeWPPath[i][2]));

                        init.SetSmooth();
                        init.SetVelocity(10.0f);
                        init.Launch();

                        me->DespawnOrUnsummon(me->GetSplineDuration());
                        break;
                    }
                    case EVENT_CANN_1:
                        SummonCannoneer(0);
                        break;
                    case EVENT_CANN_2:
                        SummonCannoneer(1);
                        break;
                    case EVENT_CANN_3:
                        SummonCannoneer(2);
                        break;
                    case EVENT_CANN_4:
                        SummonCannoneer(3);
                        break;
                    default:
                        break;
                }
            }

        private:
            bool introTalked;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_great_wall_before_gadok_introAI(creature);
        }
};

class npc_gss_pandaren_cannoneer : public CreatureScript
{
    public:
        npc_gss_pandaren_cannoneer() : CreatureScript("npc_gss_pandaren_cannoneer") { }

        enum eActions
        {
            ACTION_START_MOVEMENT_LEFT = 1
        };

        enum eEvents
        {
            EVENT_LEFT_MOVE_BEFORE_JUMP  = 1,
            EVENT_LEFT_MOVE_AFTER_JUMP   = 2,
            EVENT_LEFT_JUMP_1            = 3,
            EVENT_LEFT_JUMP_2            = 4,
            EVENT_RIGHT_MOVE_BEFORE_JUMP = 5,
            EVENT_RIGHT_MOVE_AFTER_JUMP  = 6,
            EVENT_RIGHT_JUMP_1           = 7,
            EVENT_RIGHT_JUMP_2           = 8,
            EVENT_RIGHT_BEFORE_JUMP      = 9
        };

        struct npc_gss_pandaren_cannoneerAI : public ScriptedAI
        {
            npc_gss_pandaren_cannoneerAI(Creature* creature) : ScriptedAI(creature) { }

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_START_MOVEMENT_LEFT)
                    events.ScheduleEvent(EVENT_LEFT_MOVE_BEFORE_JUMP, 2 * IN_MILLISECONDS);
                else
                    events.ScheduleEvent(EVENT_RIGHT_MOVE_BEFORE_JUMP, 2 * IN_MILLISECONDS);
            }

            void MovePath(float const (*path)[3], uint8 first, uint8 last)
            {
                Movement::MoveSplineInit init(me);
                for (uint8 i = first; i < last; ++i)
                    init.Path().push_back(G3D::Vector3(path[i][0], path[i][1], path[i][2]));

                init.SetSmooth();
                init.SetVelocity(10.0f);
                init.Launch();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_LEFT_MOVE_BEFORE_JUMP:
                        MovePath(LeftCannoneerWPPath, 0, 10);
                        events.ScheduleEvent(EVENT_LEFT_JUMP_1, me->GetSplineDuration());
                        break;
                    case EVENT_LEFT_JUMP_1:
                        me->GetMotionMaster()->MoveJump(1085.26f, 2304.05f, 361.189f, 15.0f, 15.f);
                        events.ScheduleEvent(EVENT_LEFT_JUMP_2, me->GetSplineDuration());
                        break;
                    case EVENT_LEFT_JUMP_2:
                        me->GetMotionMaster()->MoveJump(1090.68f, 2315.01f, 330.808f, 15.0f, 21.f);
                        events.ScheduleEvent(EVENT_LEFT_MOVE_AFTER_JUMP, me->GetSplineDuration());
                        break;
                    case EVENT_LEFT_MOVE_AFTER_JUMP:
                        MovePath(LeftCannoneerWPPath, 11, 24);
                        me->DespawnOrUnsummon(me->GetSplineDuration());
                        break;
                    case EVENT_RIGHT_MOVE_BEFORE_JUMP:
                        MovePath(RightCannoneerWPPath, 0, 9);
                        events.ScheduleEvent(EVENT_RIGHT_JUMP_1, me->GetSplineDuration());
                        break;
                    case EVENT_RIGHT_JUMP_1:
                        me->GetMotionMaster()->MoveJump(1079.03f, 2298.7f, 364.219f, 15.0f, 17.f);
                        events.ScheduleEvent(EVENT_RIGHT_BEFORE_JUMP, me->GetSplineDuration());
                        break;
                    case EVENT_RIGHT_BEFORE_JUMP:
                    {
                        Movement::MoveSplineInit init(me);
                        init.MoveTo(1084.88f, 2307.34f, 359.1696f);
                        init.SetVelocity(10.0f);
                        init.Launch();
                        events.ScheduleEvent(EVENT_RIGHT_JUMP_2, me->GetSplineDuration());
                        break;
                    }
                    case EVENT_RIGHT_JUMP_2:
                        me->GetMotionMaster()->MoveJump(1093.57f, 2310.65f, 330.807f, 15.0f, 21.f);
                        events.ScheduleEvent(EVENT_RIGHT_MOVE_AFTER_JUMP, me->GetSplineDuration());
                        break;
                    case EVENT_RIGHT_MOVE_AFTER_JUMP:
                        MovePath(RightCannoneerWPPath, 10, 27);
                        me->DespawnOrUnsummon(me->GetSplineDuration());
                        break;
                    default:
                        break;
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_gss_pandaren_cannoneerAI(creature);
        }
};

class npc_gss_rope_seat : public CreatureScript
{
    public:
        npc_gss_rope_seat() : CreatureScript("npc_gss_rope_seat") { }

        enum eEvents
        {
            EVENT_EJECT_PASSENGERS = 1
        };

        struct npc_gss_rope_seatAI : public ScriptedAI
        {
            npc_gss_rope_seatAI(Creature* creature) : ScriptedAI(creature) { }

            void PassengerBoarded(Unit* /*who*/, int8 /*seatId*/, bool apply) OVERRIDE
            {
                if (apply)
                {
                    float x, y, z;
                    me->GetPosition(x, y, z);
                    Movement::MoveSplineInit init(me);
                    init.MoveTo(x, y, z - 68.f);
                    init.SetVelocity(10.0f);
                    init.Launch();
                    events.ScheduleEvent(EVENT_EJECT_PASSENGERS, me->GetSplineDuration());
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_EJECT_PASSENGERS)
                {
                    if (Vehicle* vehicle = me->GetVehicleKit())
                    {
                        if (Unit* passenger = vehicle->GetPassenger(0))
                        {
                            if (Player* player = passenger->ToPlayer())
                            {
                                float x, y;
                                player->ExitVehicle();
                                GetPositionWithDistInOrientation(me, 10.f, 2 * M_PI, x, y);
                                player->GetMotionMaster()->MoveJump(x, y, 296.10f, 20.0f, 25.0f);
                            }
                        }
                    }
                    me->DespawnOrUnsummon();
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_gss_rope_seatAI(creature);
        }
};

class npc_gss_rope : public CreatureScript
{
    public:
        npc_gss_rope() : CreatureScript("npc_gss_rope") { }

        enum eSpells
        {
            SPELL_ROPE_VISUAL = 126493
        };

        enum eActions
        {
            ACTION_ACTIVATE_ROPE = 1
        };

        struct npc_gss_ropeAI : public ScriptedAI
        {
            npc_gss_ropeAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->SetCanFly(true);
                me->SetDisableGravity(true);

                if (me->GetInstanceScript() && me->GetInstanceScript()->GetBossState(DATA_RIMOK) == DONE)
                    ActivateRope();
                else
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }

            void ActivateRope()
            {
                me->CastSpell(me, SPELL_ROPE_VISUAL, false);
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
            {
                float x, y;
                GetPositionWithDistInOrientation(me, 1.85f, me->GetOrientation(), x, y);

                if (TempSummon* seat = me->SummonCreature(NPC_ROPE_SEAT, x, y, 382.26f, 3.10f, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    clicker->CastSpell(seat, VEHICLE_SPELL_RIDE_HARDCODED, true);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_ACTIVATE_ROPE)
                    ActivateRope();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_gss_ropeAI(creature);
        }
};

// 7332
class AreaTrigger_at_destroy_corner_a : public AreaTriggerScript
{
    public:
        AreaTrigger_at_destroy_corner_a() : AreaTriggerScript("at_destroy_corner_a") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
            {
                if (instance->GetData(DATA_CORNER_A) != DONE)
                {
                    if (Creature* cornerA = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_CORNER_A)))
                        if (cornerA->IsAIEnabled)
                            cornerA->AI()->DoAction(1);

                    instance->SetData(DATA_FALL_DEFENDERS, DONE);
                    instance->SetData(DATA_CORNER_A, DONE);
                }
            }

            return false;
        }
};

// 7665
class AreaTrigger_at_destroy_corner_b : public AreaTriggerScript
{
    public:
        AreaTrigger_at_destroy_corner_b() : AreaTriggerScript("at_destroy_corner_b") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetData(DATA_CORNER_B) != DONE)
                    if (Creature* cornerB = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_CORNER_B)))
                        if (cornerB->IsAIEnabled)
                            cornerB->AI()->DoAction(2);

            return false;
        }
};

// 7104
class AreaTrigger_at_before_gadok : public AreaTriggerScript
{
    public:
        AreaTrigger_at_before_gadok() : AreaTriggerScript("at_before_gadok") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            InstanceScript* instance = player->GetInstanceScript();
            if (!instance)
                return false;

            for (uint32 data : { DATA_DEFENDER_A, DATA_DEFENDER_B, DATA_TRAINEE })
                if (Creature* defender = ObjectAccessor::GetCreature(*player, instance->GetData64(data)))
                    if (defender->IsAIEnabled)
                        defender->AI()->DoAction(1);

            if (instance->GetBossState(DATA_RIMOK) == DONE && instance->GetData(DATA_BRASIER_CLICKED) == DONE && instance->GetData(DATA_CORNER_C) != DONE)
            {
                if (Creature* cornerC = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_CORNER_C)))
                {
                    if (cornerC->IsAIEnabled)
                    {
                        cornerC->AI()->DoAction(3);
                        instance->SetData(DATA_CORNER_C, DONE);
                    }
                }
            }

            return false;
        }
};

// 8395
class AreaTrigger_at_intro_gadok : public AreaTriggerScript
{
    public:
        AreaTrigger_at_intro_gadok() : AreaTriggerScript("at_intro_gadok") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* gadok = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_GADOK)))
                    if (gadok->IsAIEnabled)
                        gadok->AI()->DoAction(1);

            return false;
        }
};

// Door and wall states are restored by the instance script when the objects are created
class go_gss_generic : public GameObjectScript
{
    public:
        go_gss_generic() : GameObjectScript("go_gss_generic") { }

        struct go_gss_genericAI : public GameObjectAI
        {
            go_gss_genericAI(GameObject* go) : GameObjectAI(go) { }

            void InitializeAI() OVERRIDE
            {
                go->setActive(true);
            }
        };

        GameObjectAI* GetAI(GameObject* go) const OVERRIDE
        {
            return new go_gss_genericAI(go);
        }
};

// 109986
class spell_gss_volatile_explosion : public SpellScriptLoader
{
    public:
        spell_gss_volatile_explosion() : SpellScriptLoader("spell_gss_volatile_explosion") { }

        enum eSpells
        {
            SPELL_MANTID_MUNITION_EXPLOSION = 107153
        };

        class spell_gss_volatile_explosion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gss_volatile_explosion_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, SPELL_MANTID_MUNITION_EXPLOSION, false);
            }

            void Register() OVERRIDE
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_gss_volatile_explosion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_gss_volatile_explosion_AuraScript();
        }
};

class PickAPlayerTargetSelector
{
    public:
        PickAPlayerTargetSelector() { }

        bool operator()(WorldObject* object)
        {
            if (object->GetTypeId() == TypeID::TYPEID_PLAYER && !object->ToPlayer()->IsVehicle())
                return false;

            return true;
        }
};

class spell_gss_pick_a_player : public SpellScriptLoader
{
    public:
        spell_gss_pick_a_player() : SpellScriptLoader("spell_gss_pick_a_player") { }

        class spell_gss_pick_a_player_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gss_pick_a_player_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targetList)
            {
                targetList.remove_if(PickAPlayerTargetSelector());
                targetList.sort(Skyfire::ObjectDistanceOrderPred(GetCaster()));
                if (!targetList.empty())
                    targetList.resize(1);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gss_pick_a_player_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gss_pick_a_player_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        class spell_gss_pick_a_player_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gss_pick_a_player_AuraScript);

            void OnApplyVehicle(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Player* owner = GetOwner()->ToPlayer();
                Unit* caster = GetCaster();

                if (!owner || !caster)
                    return;

                caster->CastSpell(owner, uint32(aurEff->GetSpellInfo()->Effects[EFFECT_0].BasePoints), false);
                caster->CastSpell(owner, uint32(aurEff->GetSpellInfo()->Effects[EFFECT_1].BasePoints), false);
            }

            void Register() OVERRIDE
            {
                AfterEffectApply += AuraEffectApplyFn(spell_gss_pick_a_player_AuraScript::OnApplyVehicle, EFFECT_0, SPELL_AURA_SET_VEHICLE_ID, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_gss_pick_a_player_SpellScript();
        }

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_gss_pick_a_player_AuraScript();
        }
};

class spell_gss_sabotage_vehicle : public SpellScriptLoader
{
    public:
        spell_gss_sabotage_vehicle() : SpellScriptLoader("spell_gss_sabotage_vehicle") { }

        enum eSpells
        {
            SPELL_SABOTAGE = 107094
        };

        class spell_gss_sabotage_vehicle_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gss_sabotage_vehicle_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* owner = GetOwner()->ToPlayer();
                Unit* caster = GetCaster();
                if (!owner || !caster)
                    return;

                caster->CastSpell(owner, SPELL_SABOTAGE, false);
            }

            void Register() OVERRIDE
            {
                AfterEffectApply += AuraEffectApplyFn(spell_gss_sabotage_vehicle_AuraScript::OnApply, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_gss_sabotage_vehicle_AuraScript();
        }
};

class spell_gss_sabotage_explosion : public SpellScriptLoader
{
    public:
        spell_gss_sabotage_explosion() : SpellScriptLoader("spell_gss_sabotage_explosion") { }

        class spell_gss_sabotage_explosion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gss_sabotage_explosion_AuraScript);

            void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Player* owner = GetOwner()->ToPlayer();
                Unit* caster = GetCaster();

                if (!owner || !caster)
                    return;

                caster->CastSpell(owner, uint32(aurEff->GetSpellInfo()->Effects[EFFECT_0].BasePoints), false);

                if (caster->GetVehicle())
                    caster->ExitVehicle();
            }

            void Register() OVERRIDE
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_gss_sabotage_explosion_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_gss_sabotage_explosion_AuraScript();
        }
};

// Resin Residue 118795
class spell_gss_resin_residue : public SpellScriptLoader
{
    public:
        spell_gss_resin_residue() : SpellScriptLoader("spell_gss_resin_residue") { }

        class spell_gss_resin_residue_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gss_resin_residue_AuraScript);

            void HandleAuraEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetStackAmount() > 2)
                    if (Unit* owner = GetOwner()->ToUnit())
                        owner->CastSpell(owner, SPELL_RESIDUE_ACHIEVEMENT, true);
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_gss_resin_residue_AuraScript::HandleAuraEffectApply, EFFECT_0, SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT, AURA_EFFECT_HANDLE_REAPPLY);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_gss_resin_residue_AuraScript();
        }
};

void AddSC_gate_of_the_setting_sun()
{
    new npc_kirkthik_demolisher();
    new npc_kirkthik_wind_shaper();
    new npc_kirkthik_conscript();
    new npc_kirkthik_infiltrator();
    new npc_kirkthik_sapper();
    new npc_gss_trash_base();
    new npc_krikthik_bombarder();
    new npc_serpents_spine_marksman();
    new npc_great_wall_explosion_target_stalker();
    new npc_kirthik_glider();
    new npc_kirthik_rager();
    new npc_great_wall_before_gadok_intro();
    new npc_gss_rope();
    new npc_gss_rope_seat();
    new npc_gss_pandaren_cannoneer();

    new go_setting_sun_brasier();
    new go_gss_generic();
    new vehicle_artillery_to_wall();
    new AreaTrigger_at_destroy_corner_a();
    new AreaTrigger_at_destroy_corner_b();
    new AreaTrigger_at_before_gadok();
    new AreaTrigger_at_intro_gadok();

    new spell_gss_volatile_explosion();
    new spell_gss_pick_a_player();
    new spell_gss_sabotage_vehicle();
    new spell_gss_sabotage_explosion();
    new spell_gss_resin_residue();
}
