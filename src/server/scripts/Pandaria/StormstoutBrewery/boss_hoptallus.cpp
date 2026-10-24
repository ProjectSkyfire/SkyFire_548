/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GameObject.h"
#include "MoveSplineInit.h"
#include "Player.h"
#include "stormstout_brewery.h"

enum Spells
{
    SPELL_HOPPER_ANIM_REPLACEMENT       = 114284,
    SPELL_HAMMER_COSMETIC_1             = 114530,
    SPELL_SUMMON_HAMMER                 = 114531,
    SPELL_HAMMER_VISUAL                 = 114533,
    SPELL_BLINKING_COSMETIC_AURA        = 114287,
    SPELL_LIT_FUSE                      = 114290,
    SPELL_EXPLOSIVE_BREW                = 114291,

    SPELL_SMASH_AURA                    = 111662,
    SPELL_SMASH                         = 111666,

    SPELL_COSMETIC_EARTHQUAKE           = 114928,
    SPELL_HOPTALLUS_INTRO_SOUND         = 106790,
    SPELL_HOPTALLUS_KEG_SCENE           = 114366,
    SPELL_HOPTALLUS_SCREECH             = 114367,

    SPELL_FURLWIND                      = 112992,
    SPELL_CARROT_BREATH                 = 112944,
    SPELL_SUMMON_BREATH_STALKER         = 112956,

    SPELL_CARROT_DOOR                   = 116005,
    SPELL_RIDE_VEHICLE                  = 93970,

    // Achievements
    SPELL_SMASH_ACHIEVEMENT             = 116286,
    SPELL_CLINGER_ACHIEVEMENT           = 116271,
};

enum Creatures
{
    NPC_CARROT_BREATH_STALKER           = 59018,
    NPC_PANDA_BREWMASTER_GHOST          = 59075,
};

enum HopperEvents
{
    EVENT_HOP_MOVE = 1,
    EVENT_HOP_JUMP_TO_WATER,
    EVENT_HOP_CHECK_POS,
    EVENT_HOP_EXPLODE,
};

Position const HoptallusHopPos = { -698.62f, 1259.4f, 162.79f, 0.24f };

Position const HoplingSpawns[5] =
{
    { -719.31f, 1248.27f, 166.8f, 0.24f },
    { -719.95f, 1250.89f, 166.8f, 0.24f },
    { -720.66f, 1253.79f, 166.8f, 0.24f },
    { -721.25f, 1256.18f, 166.8f, 0.24f },
    { -721.94f, 1259.02f, 166.8f, 0.24f }
};

Position const HopperSpawns[4] =
{
    { -726.7f,  1257.9f,  166.8f, 0.24f },
    { -720.86f, 1253.44f, 166.8f, 0.24f },
    { -726.86f, 1247.41f, 166.8f, 0.25f },
    { -719.38f, 1255.37f, 166.8f, 0.25f }
};

Position const NibblerWaypoints[12] =
{
    { -704.96f, 1292.42f, 162.7f,  4.95f },
    { -698.4f,  1267.73f, 162.7f,  4.97f },
    { -690.44f, 1254.47f, 162.8f,  5.32f },
    { -683.54f, 1252.15f, 162.82f, 6.0f  }, // picks up the carrot here
    { -663.1f,  1256.7f,  154.8f,  5.86f },
    { -653.1f,  1242.9f,  154.8f,  4.91f },
    { -639.69f, 1192.53f, 139.15f, 0.28f }, // jumps down
    { -601.1f,  1201.59f, 138.5f,  4.85f },
    { -598.2f,  1188.0f,  138.65f, 4.7f  },
    { -598.4f,  1151.8f,  138.3f,  4.86f },
    { -592.1f,  1127.8f,  138.5f,  4.98f },
    { -566.1f,  1102.2f,  142.1f,  5.44f },
};

uint32 const Hoplings[5] = { 59461, 56631, 59459, 59458, 59460 };
uint32 const Hoppers[2]  = { 56718, 59426 };

// Hoplings and hoppers that spawn outside the fight hop along the corridor and then leap into the water
static void LaunchHoplingPath(Creature* hopper)
{
    hopper->GetMotionMaster()->MovementExpired();
    hopper->GetMotionMaster()->Clear();

    Movement::MoveSplineInit init(hopper);
    for (Position const& point : HoplingPath)
    {
        float x, y, z;
        hopper->GetRandomPoint(point, 3.0f, x, y, z);
        init.Path().push_back(G3D::Vector3(x, y, z));
    }

    init.SetSmooth();
    init.SetUncompressed();
    init.Launch();
}

static void JumpIntoWater(Creature* hopper)
{
    float x, y;
    GetPositionWithDistInOrientation(hopper, frand(80.0f, 95.0f), frand(1.94f, 4.38f), x, y);
    hopper->GetMotionMaster()->MoveJump(x, y, 134.28f, 15.0f, 9.0f, EVENT_JUMP + 1);
}

// Shared cosmetic behaviour of the summoned virmen: hop along the corridor, wander, jump into the water
struct HopperBaseAI : public ScriptedAI
{
    HopperBaseAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        cosmeticEvents.ScheduleEvent(EVENT_HOP_CHECK_POS, 5 * IN_MILLISECONDS);
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        me->GetMotionMaster()->Clear();
        cosmeticEvents.Reset();
    }

    void EnterEvadeMode() OVERRIDE
    {
        me->DespawnOrUnsummon();
    }

    void MovementInform(uint32 /*type*/, uint32 pointId) OVERRIDE
    {
        switch (pointId)
        {
            case EVENT_JUMP:
                if (instance && !me->IsInCombat())
                {
                    LaunchHoplingPath(me);
                    cosmeticEvents.ScheduleEvent(EVENT_HOP_MOVE, me->GetSplineDuration());
                }
                break;
            case EVENT_JUMP + 1:
                me->DespawnOrUnsummon(1000);
                break;
            default:
                break;
        }
    }

    void UpdateCosmetic(uint32 diff, float wanderDist)
    {
        cosmeticEvents.Update(diff);

        while (uint32 eventId = cosmeticEvents.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_HOP_MOVE:
                    me->GetMotionMaster()->MoveRandom(wanderDist);
                    cosmeticEvents.ScheduleEvent(EVENT_HOP_JUMP_TO_WATER, urand(3 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
                    break;
                case EVENT_HOP_JUMP_TO_WATER:
                    JumpIntoWater(me);
                    break;
                case EVENT_HOP_CHECK_POS:
                    if (me->GetPositionZ() > 164.0f)
                        me->DespawnOrUnsummon();
                    break;
                default:
                    break;
            }
        }
    }

    InstanceScript* instance;
    EventMap cosmeticEvents;
};

class npc_nibbler : public CreatureScript
{
    public:
        npc_nibbler() : CreatureScript("npc_nibbler") { }

        enum Events
        {
            EVENT_CHECK = 1,
            EVENT_MOVE,
            EVENT_JUMP_DOWN,
            EVENT_TALK_PANDA
        };

        struct npc_nibblerAI : public ScriptedAI
        {
            npc_nibblerAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()), _waypoint(0), _pandaTalk(0), _pandaIndex(0) { }

            void InitializeAI() OVERRIDE
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetVisible(false);
            }

            void Reset() OVERRIDE { }

            void DoAction(int32 /*action*/) OVERRIDE
            {
                _events.ScheduleEvent(EVENT_CHECK, 500);
            }

            void PreparePandas()
            {
                _pandaTalk = 0;
                _pandaIndex = 0;
                _pandaGUIDs.clear();

                std::list<Creature*> pandas;
                GetCreatureListWithEntryInGrid(pandas, me, NPC_PANDA_BREWMASTER_GHOST, 20.0f);
                for (Creature* panda : pandas)
                    _pandaGUIDs.push_back(panda->GetGUID());

                _events.ScheduleEvent(EVENT_TALK_PANDA, 8000);
            }

            void MovementInform(uint32 /*type*/, uint32 pointId) OVERRIDE
            {
                switch (pointId)
                {
                    case 3:
                        if (Creature* bunny = GetClosestCreatureWithEntry(me, NPC_BARREL_TOSS_BUNNY, 10.0f))
                            bunny->CastSpell(me, SPELL_RIDE_VEHICLE, true);
                        if (GameObject* door = GetClosestGameObjectWithEntry(me, GO_INVIS_DOOR, 10.0f))
                            door->AddObjectToRemoveList();
                        _events.ScheduleEvent(EVENT_MOVE, 100);
                        break;
                    case 6:
                        _events.ScheduleEvent(EVENT_JUMP_DOWN, 100);
                        PreparePandas();
                        break;
                    case 11:
                        if (Creature* bunny = GetClosestCreatureWithEntry(me, NPC_BARREL_TOSS_BUNNY, 10.0f))
                            bunny->RemoveAurasDueToSpell(SPELL_CARROT_DOOR);
                        me->SetVisible(false);
                        break;
                    default:
                        if (pointId < 11)
                            _events.ScheduleEvent(EVENT_MOVE, 100);
                        break;
                }

                ++_waypoint;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK:
                            if (_instance && _instance->GetBossState(DATA_HOPTALLUS) == DONE)
                            {
                                me->SetVisible(true);
                                _waypoint = 0;
                                _events.ScheduleEvent(EVENT_MOVE, 100);
                            }
                            break;
                        case EVENT_MOVE:
                            if (_waypoint < 12)
                                me->GetMotionMaster()->MovePoint(_waypoint, NibblerWaypoints[_waypoint]);
                            break;
                        case EVENT_JUMP_DOWN:
                            if (_waypoint < 12)
                                me->GetMotionMaster()->MoveJump(NibblerWaypoints[_waypoint].GetPositionX(), NibblerWaypoints[_waypoint].GetPositionY(), NibblerWaypoints[_waypoint].GetPositionZ(), 15.0f, 15.0f, _waypoint);
                            break;
                        case EVENT_TALK_PANDA:
                            if (_pandaIndex >= _pandaGUIDs.size())
                                break;

                            if (Creature* panda = ObjectAccessor::GetCreature(*me, _pandaGUIDs[_pandaIndex]))
                                panda->AI()->Talk(_pandaTalk++);
                            ++_pandaIndex;
                            _events.ScheduleEvent(EVENT_TALK_PANDA, 9500);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            InstanceScript* _instance;
            uint32 _waypoint;
            uint8 _pandaTalk;
            std::size_t _pandaIndex;
            std::vector<uint64> _pandaGUIDs;
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_nibblerAI(creature);
        }
};

class boss_hoptallus : public CreatureScript
{
    public:
        boss_hoptallus() : CreatureScript("boss_hoptallus") { }

        enum Events
        {
            EVENT_FURLWIND = 1,
            EVENT_CARROT_BREATH,
            EVENT_SCREECH,
            EVENT_ENTER_COMBAT,
            EVENT_SUMMON_HOPLINGS,
            EVENT_SUMMON_HOPPERS
        };

        enum Talks
        {
            TALK_AGGRO          = 0,
            TALK_FURLWIND       = 1,
            TALK_CARROT_BREATH  = 2,
            TALK_DEATH          = 3,
            TALK_SCREECH        = 4,
            TALK_SLAY           = 5
        };

        struct boss_hoptallusAI : public BossAI
        {
            boss_hoptallusAI(Creature* creature) : BossAI(creature, DATA_HOPTALLUS), _introStarted(false), _hoplingWaves(0), _hopperWaves(0) { }

            void InitializeAI() OVERRIDE
            {
                if (Creature* bunny = GetClosestCreatureWithEntry(me, NPC_BARREL_TOSS_BUNNY, 45.0f))
                    bunny->AddAura(SPELL_CARROT_DOOR, bunny);

                me->setActive(true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

                _introStarted = instance->GetBossState(DATA_HOPTALLUS) == SPECIAL;
                if (_introStarted && me->IsAlive())
                    StartIntro();
            }

            void Reset() OVERRIDE
            {
                _Reset();
            }

            void StartIntro()
            {
                me->SetReactState(REACT_PASSIVE);
                _introStarted = true;

                if (GameObject* keg = GetClosestGameObjectWithEntry(me, GO_SHAKING_KEG, 50.0f))
                    keg->UseDoorOrButton();

                instance->SetBossState(DATA_HOPTALLUS, SPECIAL);

                DoCast(me, SPELL_HOPTALLUS_KEG_SCENE);
                DoCast(me, SPELL_HOPTALLUS_INTRO_SOUND);
                DoCast(me, SPELL_COSMETIC_EARTHQUAKE);

                me->GetMotionMaster()->MoveJump(HoptallusHopPos.GetPositionX(), HoptallusHopPos.GetPositionY(), HoptallusHopPos.GetPositionZ(), 10.0f, 10.0f, 1);
                me->SetHomePosition(HoptallusHopPos);

                events.ScheduleEvent(EVENT_ENTER_COMBAT, 8000);
            }

            void MovementInform(uint32 /*type*/, uint32 pointId) OVERRIDE
            {
                if (pointId == 1)
                    me->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                // Only start from the corridor in front of the keg, not from behind it
                if (!_introStarted && who->GetTypeId() == TypeID::TYPEID_PLAYER && who->GetDistance(me) < 20.0f && who->GetPositionX() > -705.0f)
                    StartIntro();

                BossAI::MoveInLineOfSight(who);
            }

            void SummonHoplings(uint8 count)
            {
                for (uint8 i = 0; i < count && i < 5; ++i)
                {
                    if (Creature* hopling = me->SummonCreature(Hoplings[i], HoplingSpawns[i], TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, urand(2000, 5000)))
                    {
                        hopling->AddAura(SPELL_HOPPER_ANIM_REPLACEMENT, hopling);

                        Position pos;
                        me->GetRandomNearPosition(pos, 10.0f);
                        hopling->GetMotionMaster()->MoveJump(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 15.0f, 15.0f, EVENT_JUMP);

                        // The waves before the intro only hop around
                        if (instance->GetBossState(DATA_HOPTALLUS) == SPECIAL)
                            hopling->AI()->DoZoneInCombat();
                    }
                }

                if (instance->GetBossState(DATA_HOPTALLUS) == SPECIAL)
                {
                    if (++_hoplingWaves < 3)
                        events.ScheduleEvent(EVENT_SUMMON_HOPLINGS, 12 * IN_MILLISECONDS);
                    else
                        _hoplingWaves = 0;
                }
            }

            void SummonHoppers(uint8 count)
            {
                for (uint8 i = 0; i < count && i < 4; ++i)
                {
                    if (Creature* hopper = me->SummonCreature(Hoppers[urand(0, 1)], HopperSpawns[i], TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, urand(4000, 8000)))
                    {
                        Position pos;
                        me->GetRandomNearPosition(pos, 10.0f);
                        hopper->GetMotionMaster()->MoveJump(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 10.0f, 10.0f, EVENT_JUMP);

                        if (instance->GetBossState(DATA_HOPTALLUS) == SPECIAL)
                            hopper->AI()->DoZoneInCombat();
                    }
                }

                if (instance->GetBossState(DATA_HOPTALLUS) == SPECIAL)
                {
                    if (++_hopperWaves < 2)
                        events.ScheduleEvent(EVENT_SUMMON_HOPPERS, 16 * IN_MILLISECONDS);
                    else
                        _hopperWaves = 0;
                }
            }

            void EnterEvadeMode() OVERRIDE
            {
                me->ClearUnitState(UNIT_STATE_CANNOT_TURN);
                BossAI::EnterEvadeMode();
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(TALK_SLAY);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(TALK_DEATH);

                if (Creature* nibbler = GetClosestCreatureWithEntry(me, NPC_NIBBLER, 100.0f, true))
                    nibbler->AI()->DoAction(0);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(TALK_AGGRO);

                events.ScheduleEvent(EVENT_SCREECH, 30000);
                events.ScheduleEvent(EVENT_FURLWIND, urand(10000, 16000));
                events.ScheduleEvent(EVENT_CARROT_BREATH, urand(15000, 30000));

                _hoplingWaves = 0;
                _hopperWaves = 0;
            }

            // Called by the instance every 12-14 seconds before the intro
            void DoAction(int32 /*action*/) OVERRIDE
            {
                SummonHoplings(urand(4, 5));
                SummonHoppers(urand(2, 3));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (me->HasAura(SPELL_FURLWIND) || me->HasAura(SPELL_CARROT_BREATH) || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ENTER_COMBAT:
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);
                            me->SetReactState(REACT_AGGRESSIVE);
                            DoZoneInCombat();
                            break;
                        case EVENT_FURLWIND:
                            DoCast(me, SPELL_FURLWIND);
                            Talk(TALK_FURLWIND);
                            events.ScheduleEvent(EVENT_FURLWIND, urand(40000, 50000));
                            break;
                        case EVENT_CARROT_BREATH:
                            DoCast(me, SPELL_SUMMON_BREATH_STALKER);
                            DoCast(me, SPELL_CARROT_BREATH);
                            Talk(TALK_CARROT_BREATH);
                            events.ScheduleEvent(EVENT_CARROT_BREATH, urand(25000, 27000));
                            break;
                        case EVENT_SCREECH:
                            events.ScheduleEvent(EVENT_SCREECH, 30000);
                            events.ScheduleEvent(EVENT_SUMMON_HOPLINGS, 2000);
                            events.ScheduleEvent(EVENT_SUMMON_HOPPERS, 4000);
                            DoCast(me, SPELL_HOPTALLUS_SCREECH);
                            Talk(TALK_SCREECH);
                            break;
                        case EVENT_SUMMON_HOPLINGS:
                            SummonHoplings(5);
                            break;
                        case EVENT_SUMMON_HOPPERS:
                            SummonHoppers(4);
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
            bool _introStarted;
            uint32 _hoplingWaves;
            uint32 _hopperWaves;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetStormstoutBreweryAI<boss_hoptallusAI>(creature);
        }
};

class npc_hammer_bopper : public CreatureScript
{
    public:
        npc_hammer_bopper() : CreatureScript("npc_hammer_bopper") { }

        struct npc_hammer_bopperAI : public HopperBaseAI
        {
            npc_hammer_bopperAI(Creature* creature) : HopperBaseAI(creature) { }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->AddAura(SPELL_HAMMER_COSMETIC_1, me);
                me->AddAura(SPELL_HOPPER_ANIM_REPLACEMENT, me);
                HopperBaseAI::IsSummonedBy(summoner);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                DoCast(me, SPELL_SUMMON_HAMMER, true);
                me->RemoveAurasDueToSpell(SPELL_HAMMER_COSMETIC_1);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                UpdateCosmetic(diff, 5.0f);

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_hammer_bopperAI(creature);
        }
};

class npc_explosive_hopper : public CreatureScript
{
    public:
        npc_explosive_hopper() : CreatureScript("npc_explosive_hopper") { }

        struct npc_explosive_hopperAI : public HopperBaseAI
        {
            npc_explosive_hopperAI(Creature* creature) : HopperBaseAI(creature) { }

            void Reset() OVERRIDE { }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->AddAura(SPELL_HOPPER_ANIM_REPLACEMENT, me);
                HopperBaseAI::IsSummonedBy(summoner);
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                HopperBaseAI::EnterCombat(who);
                _events.ScheduleEvent(EVENT_HOP_EXPLODE, urand(3000, 5000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                UpdateCosmetic(diff, 5.0f);

                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    if (eventId == EVENT_HOP_EXPLODE)
                    {
                        DoCast(me, SPELL_LIT_FUSE, true);
                        DoCast(me, SPELL_BLINKING_COSMETIC_AURA, true);
                        DoCast(me, SPELL_EXPLOSIVE_BREW);
                        me->ClearUnitState(UNIT_STATE_CASTING);
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_explosive_hopperAI(creature);
        }
};

// 59461, 56631, 59458, 59459 - Hopling
class npc_sb_hopling : public CreatureScript
{
    public:
        npc_sb_hopling() : CreatureScript("npc_sb_hopling") { }

        struct npc_sb_hoplingAI : public HopperBaseAI
        {
            npc_sb_hoplingAI(Creature* creature) : HopperBaseAI(creature) { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                UpdateCosmetic(diff, 3.0f);

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_sb_hoplingAI(creature);
        }
};

class npc_hammer : public CreatureScript
{
    public:
        npc_hammer() : CreatureScript("npc_hammer") { }

        struct npc_hammerAI : public ScriptedAI
        {
            npc_hammerAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->AddAura(SPELL_HAMMER_VISUAL, me);
            }

            void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
            {
                clicker->AddAura(SPELL_SMASH_AURA, clicker);
                me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_hammerAI(creature);
        }
};

class npc_carrot_breath_stalker : public CreatureScript
{
    public:
        npc_carrot_breath_stalker() : CreatureScript("npc_carrot_breath_stalker") { }

        enum Events
        {
            EVENT_MOVE = 1
        };

        struct npc_carrot_breath_stalkerAI : public ScriptedAI
        {
            npc_carrot_breath_stalkerAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()),
                _counterClockwise(false), _x(0.0f), _y(0.0f), _arcPoint(0.0f) { }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->GetMotionMaster()->Clear();

                _events.ScheduleEvent(EVENT_MOVE, 1500);
                _x = me->GetPositionX();
                _y = me->GetPositionY();
                _arcPoint = 0.0f;
                _counterClockwise = urand(0, 1) != 0;

                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                FaceHoptallus();
            }

            // Hoptallus channels his breath at the stalker while it sweeps an arc
            void FaceHoptallus()
            {
                if (!_instance)
                    return;

                if (Creature* hoptallus = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HOPTALLUS)))
                {
                    hoptallus->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, me->GetGUID());
                    hoptallus->SetTarget(me->GetGUID());
                    hoptallus->SetFacingTo(hoptallus->GetAngle(me));
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE && pointId == 0)
                    _events.ScheduleEvent(EVENT_MOVE, 100);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    if (eventId == EVENT_MOVE)
                    {
                        float angle = _arcPoint * float(M_PI) / 16.0f;
                        float newX = _x + 10.0f * std::cos(angle);
                        float newY = _counterClockwise ? _y - 10.0f * std::sin(angle) : _y + 10.0f * std::sin(angle);
                        me->GetMotionMaster()->MovePoint(0, newX, newY, 162.784f);
                        _arcPoint += 1.0f;

                        FaceHoptallus();
                    }
                }
            }

        private:
            InstanceScript* _instance;
            bool _counterClockwise;
            float _x;
            float _y;
            float _arcPoint;
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_carrot_breath_stalkerAI(creature);
        }
};

// 111666 - Smash
class spell_hammer_smash : public SpellScriptLoader
{
    public:
        spell_hammer_smash() : SpellScriptLoader("spell_hammer_smash") { }

        class spell_hammer_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hammer_smash_SpellScript);

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if([](WorldObject* obj) { return !obj->ToUnit() || !obj->ToUnit()->HasAura(SPELL_HOPPER_ANIM_REPLACEMENT); });

                if (targets.size() >= 100)
                    GetCaster()->CastSpell(GetCaster(), SPELL_SMASH_ACHIEVEMENT, false);
            }

            void HandleKnockBack(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                    if (target->IsAlive() && target->GetEntry() != NPC_EXPLOSIVE_HOPPER)
                        target->Kill(target);
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                GetCaster()->RemoveAuraFromStack(GetSpellInfo()->Effects[effIndex].BasePoints);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_hammer_smash_SpellScript::HandleKnockBack, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
                OnEffectHitTarget += SpellEffectFn(spell_hammer_smash_SpellScript::HandleKnockBack, EFFECT_1, SPELL_EFFECT_KNOCK_BACK);
                OnEffectHit += SpellEffectFn(spell_hammer_smash_SpellScript::HandleScript, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hammer_smash_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hammer_smash_SpellScript::SelectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_hammer_smash_SpellScript();
        }
};

// 112944 - Carrot Breath
class spell_carrot_breath : public SpellScriptLoader
{
    public:
        spell_carrot_breath() : SpellScriptLoader("spell_carrot_breath") { }

        class spell_carrot_breath_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_carrot_breath_AuraScript);

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* creature = GetTarget()->ToCreature())
                {
                    creature->AddUnitState(UNIT_STATE_CANNOT_TURN);
                    creature->SetReactState(REACT_PASSIVE);
                }
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* creature = GetTarget()->ToCreature())
                {
                    creature->ClearUnitState(UNIT_STATE_CANNOT_TURN);
                    creature->SetReactState(REACT_AGGRESSIVE);

                    if (Creature* stalker = GetClosestCreatureWithEntry(creature, NPC_CARROT_BREATH_STALKER, 40.0f))
                        stalker->DespawnOrUnsummon();
                }
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_carrot_breath_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_carrot_breath_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_carrot_breath_AuraScript();
        }
};

// 112992 - Furlwind
class spell_furlwind : public SpellScriptLoader
{
    public:
        spell_furlwind() : SpellScriptLoader("spell_furlwind") { }

        class spell_furlwind_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_furlwind_AuraScript);

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* creature = GetTarget()->ToCreature())
                    creature->ClearUnitState(UNIT_STATE_CASTING);
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_furlwind_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_furlwind_AuraScript();
        }
};

// 114291 - Explosive Brew
class spell_sb_explosive_brew : public SpellScriptLoader
{
    public:
        spell_sb_explosive_brew() : SpellScriptLoader("spell_sb_explosive_brew") { }

        class spell_sb_explosive_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sb_explosive_brew_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                    if (target->GetEntry() == NPC_HOZEN_CLINGER)
                        target->CastSpell(target, SPELL_CLINGER_ACHIEVEMENT, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_sb_explosive_brew_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_sb_explosive_brew_SpellScript();
        }
};

void AddSC_boss_hoptallus()
{
    new boss_hoptallus();
    new npc_explosive_hopper();
    new npc_hammer_bopper();
    new npc_hammer();
    new npc_carrot_breath_stalker();
    new npc_nibbler();
    new npc_sb_hopling();
    new spell_hammer_smash();
    new spell_carrot_breath();
    new spell_furlwind();
    new spell_sb_explosive_brew();
}
