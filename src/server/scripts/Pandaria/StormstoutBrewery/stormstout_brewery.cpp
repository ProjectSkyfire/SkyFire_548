/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Player.h"
#include "stormstout_brewery.h"

enum TrashSpells
{
    SPELL_PROC_EXPLOSION    = 106787,
    SPELL_SPLASH            = 107030,
    SPELL_AQUATIC_ILLUSION  = 107044,
    SPELL_FIERY_ILLUSION    = 107175,
    SPELL_BLAZING_SPARK     = 107071,
    SPELL_BOUNCE            = 107019,
};

Position const PartyWaypoints[7] =
{
    { -766.2f,  1390.8f,  146.7f, 1.8f  },
    { -775.55f, 1422.47f, 139.5f, 3.4f  },
    { -801.81f, 1415.04f, 139.7f, 3.38f },
    { -814.9f,  1411.7f,  134.6f, 3.38f },
    { -811.6f,  1399.9f,  132.3f, 4.98f },
    { -804.4f,  1383.6f,  126.7f, 5.9f  },
    { -783.2f,  1383.4f,  126.7f, 4.9f  }
};

uint32 const PartyEmotes[4] =
{
    EMOTE_ONESHOT_TALK,
    EMOTE_ONESHOT_CHEER,
    EMOTE_ONESHOT_EAT,
    EMOTE_ONESHOT_LAUGH,
};

// 107205 - Spicy Explosion
class spell_spicy_explosion : public SpellScriptLoader
{
    public:
        spell_spicy_explosion() : SpellScriptLoader("spell_spicy_explosion") { }

        class spell_spicy_explosion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_spicy_explosion_SpellScript);

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if([](WorldObject* obj) { return obj->GetTypeId() == TypeID::TYPEID_PLAYER; });
            }

            void HandleAfterCast()
            {
                if (Creature* caster = GetCaster()->ToCreature())
                    caster->DespawnOrUnsummon(1000);
            }

            void Register() OVERRIDE
            {
                AfterCast += SpellCastFn(spell_spicy_explosion_SpellScript::HandleAfterCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_spicy_explosion_SpellScript::SelectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_spicy_explosion_SpellScript::SelectTargets, EFFECT_3, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_spicy_explosion_SpellScript();
        }
};

class npc_chen_stormstout : public CreatureScript
{
    public:
        npc_chen_stormstout() : CreatureScript("npc_chen_stormstout") { }

        enum Talks
        {
            TALK_NOT_ZAN,
            TALK_HAPPENED,
            TALK_WHERE,
            TALK_ABANDONED,
            TALK_GAO,
            TALK_COOKIES,
            TALK_GHOST
        };

        enum Events
        {
            EVENT_TALK_1 = 1,
            EVENT_TALK_2,
            EVENT_TALK_3,
            EVENT_TALK_4,
            EVENT_TALK_5,
            EVENT_TALK_6,
            EVENT_TALK_7,
        };

        struct npc_chen_stormstoutAI : public ScriptedAI
        {
            npc_chen_stormstoutAI(Creature* creature) : ScriptedAI(creature) { }

            void AuntieDoAction(int32 action)
            {
                if (Creature* auntie = GetClosestCreatureWithEntry(me, NPC_AUNTIE_STORMSTOUT, 12.0f))
                    auntie->AI()->DoAction(action);
            }

            void DoAction(int32 action) OVERRIDE
            {
                _events.Reset();

                switch (action)
                {
                    case 0: _events.ScheduleEvent(EVENT_TALK_1, 8000);  break;
                    case 1: _events.ScheduleEvent(EVENT_TALK_2, 6000);  break;
                    case 2: _events.ScheduleEvent(EVENT_TALK_3, 7400);  break;
                    case 3: _events.ScheduleEvent(EVENT_TALK_4, 7000);  break;
                    case 4: _events.ScheduleEvent(EVENT_TALK_5, 13000); break;
                    case 5: _events.ScheduleEvent(EVENT_TALK_6, 4000);  break;
                    default: break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TALK_1:
                            Talk(TALK_NOT_ZAN);
                            AuntieDoAction(1);
                            break;
                        case EVENT_TALK_2:
                            Talk(TALK_HAPPENED);
                            AuntieDoAction(2);
                            break;
                        case EVENT_TALK_3:
                            Talk(TALK_WHERE);
                            AuntieDoAction(3);
                            break;
                        case EVENT_TALK_4:
                            Talk(TALK_ABANDONED);
                            AuntieDoAction(4);
                            break;
                        case EVENT_TALK_5:
                            Talk(TALK_GAO);
                            AuntieDoAction(5);
                            break;
                        case EVENT_TALK_6:
                            Talk(TALK_COOKIES);
                            _events.ScheduleEvent(EVENT_TALK_7, 7000);
                            break;
                        case EVENT_TALK_7:
                            Talk(TALK_GHOST);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_chen_stormstoutAI(creature);
        }
};

class npc_auntie_stormstout : public CreatureScript
{
    public:
        npc_auntie_stormstout() : CreatureScript("npc_auntie_stormstout") { }

        enum Talks
        {
            TALK_HELLO,
            TALK_ZAN,
            TALK_DAY,
            TALK_SIZE,
            TALK_ABANDONED,
            TALK_COOKIES
        };

        enum Events
        {
            EVENT_TALK_1 = 1,
            EVENT_TALK_2,
            EVENT_TALK_3,
            EVENT_TALK_4,
            EVENT_TALK_5,
            EVENT_TALK_6
        };

        struct npc_auntie_stormstoutAI : public ScriptedAI
        {
            npc_auntie_stormstoutAI(Creature* creature) : ScriptedAI(creature), _eventStarted(false) { }

            void ChenDoAction(int32 action)
            {
                if (Creature* chen = GetClosestCreatureWithEntry(me, NPC_CHEN_STORMSTOUT, 12.0f))
                    chen->AI()->DoAction(action);
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (!_eventStarted && who->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    _eventStarted = true;
                    _events.ScheduleEvent(EVENT_TALK_1, 5000);
                }
            }

            void DoAction(int32 action) OVERRIDE
            {
                _events.Reset();

                switch (action)
                {
                    case 1: _events.ScheduleEvent(EVENT_TALK_2, 6000); break;
                    case 2: _events.ScheduleEvent(EVENT_TALK_3, 4500); break;
                    case 3: _events.ScheduleEvent(EVENT_TALK_4, 7000); break;
                    case 4: _events.ScheduleEvent(EVENT_TALK_5, 6500); break;
                    case 5: _events.ScheduleEvent(EVENT_TALK_6, 5000); break;
                    default: break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TALK_1:
                            Talk(TALK_HELLO);
                            ChenDoAction(0);
                            break;
                        case EVENT_TALK_2:
                            Talk(TALK_ZAN);
                            ChenDoAction(1);
                            break;
                        case EVENT_TALK_3:
                            Talk(TALK_DAY);
                            ChenDoAction(2);
                            break;
                        case EVENT_TALK_4:
                            Talk(TALK_SIZE);
                            ChenDoAction(3);
                            break;
                        case EVENT_TALK_5:
                            Talk(TALK_ABANDONED);
                            ChenDoAction(4);
                            break;
                        case EVENT_TALK_6:
                            Talk(TALK_COOKIES);
                            ChenDoAction(5);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            bool _eventStarted;
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_auntie_stormstoutAI(creature);
        }
};

class npc_aqua_dancer : public CreatureScript
{
    public:
        npc_aqua_dancer() : CreatureScript("npc_aqua_dancer") { }

        struct npc_aqua_dancerAI : public ScriptedAI
        {
            npc_aqua_dancerAI(Creature* creature) : ScriptedAI(creature), _hozenGUID(0) { }

            void InitializeAI() OVERRIDE
            {
                me->SetUnitMovementFlags(MOVEMENTFLAG_DISABLE_GRAVITY);

                if (Creature* hozen = GetClosestCreatureWithEntry(me, NPC_SODDEN_HOZEN_BRAWLER, 30.0f))
                {
                    DoCast(hozen, SPELL_AQUATIC_ILLUSION, true);
                    _hozenGUID = hozen->GetGUID();
                }

                me->SetReactState(REACT_PASSIVE);
            }

            void Reset() OVERRIDE { }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = me->GetHealth();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                DoCast(me, SPELL_SPLASH, true);

                if (Creature* hozen = ObjectAccessor::GetCreature(*me, _hozenGUID))
                {
                    Talk(0);
                    me->DealDamage(hozen, uint32(hozen->GetMaxHealth() / 2), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_FROST);
                    if (hozen->IsAlive())
                        hozen->AI()->DoZoneInCombat();
                }
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }

        private:
            uint64 _hozenGUID;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_aqua_dancerAI(creature);
        }
};

class npc_fiery_trickster : public CreatureScript
{
    public:
        npc_fiery_trickster() : CreatureScript("npc_fiery_trickster") { }

        enum Events
        {
            EVENT_INIT = 1,
            EVENT_FIRE_SPARK
        };

        struct npc_fiery_tricksterAI : public ScriptedAI
        {
            npc_fiery_tricksterAI(Creature* creature) : ScriptedAI(creature), _hozenGUID(0)
            {
                me->SetUnitMovementFlags(MOVEMENTFLAG_DISABLE_GRAVITY);
                _events.ScheduleEvent(EVENT_INIT, urand(400, 800));
                _events.ScheduleEvent(EVENT_FIRE_SPARK, urand(4000, 9000));
            }

            void Reset() OVERRIDE { }

            void BindHozen()
            {
                Creature* hozen = GetClosestCreatureWithEntry(me, NPC_INFLAMED_HOZEN_BRAWLER, 39.0f);
                if (hozen && !hozen->HasAura(SPELL_FIERY_ILLUSION))
                {
                    DoCast(hozen, SPELL_FIERY_ILLUSION, true);
                    _hozenGUID = hozen->GetGUID();
                }

                me->SetReactState(REACT_PASSIVE);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = me->GetHealth();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (Creature* hozen = ObjectAccessor::GetCreature(*me, _hozenGUID))
                {
                    Talk(0);
                    me->DealDamage(hozen, uint32(hozen->GetMaxHealth() / 2), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_FIRE);
                    if (hozen->IsAlive())
                        hozen->AI()->DoZoneInCombat();
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INIT:
                            BindHozen();
                            break;
                        case EVENT_FIRE_SPARK:
                            DoCast(me, SPELL_BLAZING_SPARK, true);
                            _events.ScheduleEvent(EVENT_FIRE_SPARK, urand(6000, 11000));
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            uint64 _hozenGUID;
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_fiery_tricksterAI(creature);
        }
};

class npc_hozen_party_animal : public CreatureScript
{
    public:
        npc_hozen_party_animal() : CreatureScript("npc_hozen_party_animal") { }

        enum Events
        {
            EVENT_MOVE = 1,
            EVENT_FIREWORK,
        };

        struct npc_hozen_party_animalAI : public ScriptedAI
        {
            npc_hozen_party_animalAI(Creature* creature) : ScriptedAI(creature), _canAggroMore(true), _waypoint(0) { }

            void Reset() OVERRIDE
            {
                _events.ScheduleEvent(EVENT_FIREWORK, urand(2, 28) * IN_MILLISECONDS);
                me->CastSpell(me, HozenHats[std::rand() % 6], false);
            }

            // Ook-Ook has been called: stop partying and run off
            void Flee()
            {
                me->CombatStop(true);
                me->AttackStop();
                me->SetReactState(REACT_PASSIVE);
                _canAggroMore = true;
                me->HandleEmoteCommand(PartyEmotes[urand(0, 3)]);
                me->GetMotionMaster()->Clear(false);

                _waypoint = me->GetDistance(PartyWaypoints[0]) > me->GetDistance(PartyWaypoints[1]) ? 1 : 0;

                _events.Reset();
                _events.ScheduleEvent(EVENT_MOVE, 100);
                _events.ScheduleEvent(EVENT_FIREWORK, urand(2, 28) * IN_MILLISECONDS);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == 0)
                    Flee();
                else if (action == 1)
                    _canAggroMore = false;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (_canAggroMore)
                {
                    std::list<Creature*> partyAnimals;
                    GetCreatureListWithEntryInGrid(partyAnimals, me, me->GetEntry(), 15.0f);
                    for (Creature* hozen : partyAnimals)
                    {
                        if (hozen->IsAlive() && !hozen->IsInCombat())
                        {
                            hozen->AI()->DoAction(1);
                            hozen->AI()->DoZoneInCombat();
                        }
                    }
                }

                _events.CancelEvent(EVENT_FIREWORK);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId < 6)
                    _events.ScheduleEvent(EVENT_MOVE, 100);
                else
                    me->DespawnOrUnsummon();

                ++_waypoint;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE:
                            if (_waypoint < 7)
                                me->GetMotionMaster()->MovePoint(_waypoint, PartyWaypoints[_waypoint]);
                            break;
                        case EVENT_FIREWORK:
                            me->CastSpell(me, HozenFireworks[std::rand() % 7], false);
                            _events.ScheduleEvent(EVENT_FIREWORK, urand(2, 28) * IN_MILLISECONDS);
                            break;
                        default:
                            break;
                    }
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

        private:
            bool _canAggroMore;
            uint32 _waypoint;
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_hozen_party_animalAI(creature);
        }
};

class npc_hozen_bouncer : public CreatureScript
{
    public:
        npc_hozen_bouncer() : CreatureScript("npc_hozen_bouncer") { }

        enum Events
        {
            EVENT_TALK = 1
        };

        struct npc_hozen_bouncerAI : public ScriptedAI
        {
            npc_hozen_bouncerAI(Creature* creature) : ScriptedAI(creature), _talk(0)
            {
                InstanceScript* instance = me->GetInstanceScript();
                if (instance && instance->GetBossState(DATA_OOK_OOK) == DONE)
                    me->AddAura(SPELL_BOUNCE, me);

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            }

            void Reset() OVERRIDE { }

            void DoAction(int32 action) OVERRIDE
            {
                _talk = uint32(action);
                me->RemoveAllAuras();

                float x, y;
                GetPositionWithDistInOrientation(me, 18.0f, me->GetOrientation(), x, y);
                me->SetWalk(true);
                me->GetMotionMaster()->MovePoint(0, x, y, me->GetMap()->GetHeight(x, y, me->GetPositionZ()));
            }

            void MovementInform(uint32 type, uint32 /*pointId*/) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                me->SetFacingTo(1.83f);
                _events.ScheduleEvent(EVENT_TALK, 2000 * (_talk + 1) * 2);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    if (eventId == EVENT_TALK)
                    {
                        Talk(_talk);
                        if (_talk < 2)
                            _events.ScheduleEvent(EVENT_TALK, 5000);
                        _talk += 2;
                    }
                }
            }

        private:
            uint32 _talk;
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_hozen_bouncerAI(creature);
        }
};

class npc_controlled_hozen : public CreatureScript
{
    public:
        npc_controlled_hozen() : CreatureScript("npc_controlled_hozen") { }

        struct npc_controlled_hozenAI : public ScriptedAI
        {
            npc_controlled_hozenAI(Creature* creature) : ScriptedAI(creature) { }

            void EnterEvadeMode() OVERRIDE
            {
                me->CombatStop(true);
                me->AttackStop();
                me->RemoveAllAurasExceptType(SPELL_AURA_PROC_TRIGGER_SPELL);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                std::list<Creature*> brawlers;
                GetCreatureListWithEntryInGrid(brawlers, me, me->GetEntry(), 60.0f);
                for (Creature* brawler : brawlers)
                    if (brawler->IsAlive() && !brawler->IsInCombat())
                        brawler->AI()->DoZoneInCombat();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_controlled_hozenAI(creature);
        }
};

// 56731 - Habanero Brew
class npc_habanero_brew : public CreatureScript
{
    public:
        npc_habanero_brew() : CreatureScript("npc_habanero_brew") { }

        struct npc_habanero_brewAI : public ScriptedAI
        {
            npc_habanero_brewAI(Creature* creature) : ScriptedAI(creature) { }

            void InitializeAI() OVERRIDE
            {
                if (!me->HasAura(SPELL_PROC_EXPLOSION))
                    me->AddAura(SPELL_PROC_EXPLOSION, me);

                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void Reset() OVERRIDE { }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->DespawnOrUnsummon(1000);
            }

            void OnSpellClick(Unit* /*clicker*/, bool& /*result*/) OVERRIDE
            {
                me->CastSpell(me, SPELL_SPICY_EXPLOSION, false);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_habanero_brewAI(creature);
        }
};

void AddSC_stormstout_brewery()
{
    new spell_spicy_explosion();
    new npc_chen_stormstout();
    new npc_auntie_stormstout();
    new npc_aqua_dancer();
    new npc_fiery_trickster();
    new npc_hozen_party_animal();
    new npc_hozen_bouncer();
    new npc_controlled_hozen();
    new npc_habanero_brew();
}
