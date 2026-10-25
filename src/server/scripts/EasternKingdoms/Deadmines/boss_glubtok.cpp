/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "Containers.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include "PassiveAI.h"
#include "deadmines.h"
#include <unordered_map>

enum ScriptTexts
{
    SAY_AGGRO        = 0,
    SAY_FIRE         = 1,
    SAY_FROST        = 2,
    SAY_HEAD1        = 3,
    SAY_HEAD2        = 4,
    SAY_ARCANE_POWER = 5,
    SAY_FIRE_WALL    = 6,
    SAY_DEATH        = 7,
    SAY_KILL         = 8,
};

enum Spells
{
    SPELL_ARCANE_POWER         = 88009,
    SPELL_FIST_OF_FLAME        = 87859,
    SPELL_FIST_OF_FLAME_0      = 87896,
    SPELL_FIST_OF_FROST        = 87861,
    SPELL_FIST_OF_FROST_0      = 87901,
    SPELL_FIRE_BLOSSOM         = 88129,
    SPELL_FROST_BLOSSOM        = 88169,
    SPELL_FROST_BLOSSOM_0      = 88177,
    SPELL_BLINK                = 87925,
    SPELL_DEEP_FREEZE          = 88000,
    SPELL_ARCANE_OVERLOAD      = 88183,

    // Misc
    SPELL_FIRE_BLOSSOM_VISUAL  = 88164,
    SPELL_FROST_BLOSSOM_VISUAL = 88165, // casted by bunny
    SPELL_ARCANE_FIRE_BEAM     = 88072, // channelled by a Fire Blossom Bunny at Glubtok
    SPELL_ARCANE_FROST_BEAM    = 88093, // channelled by a Frost Blossom Bunny at Glubtok
    SPELL_HOVERING_ANIM        = 90766,
    SPELL_FIRE_WALL            = 91398,
    SPELL_FIRE_WALL_DAMAGE     = 91397,
};

enum Actions
{
    ACTION_FIRE_WALL_START = 1,
    ACTION_FIRE_WALL_STOP  = 2,
};

enum Events
{
    EVENT_FIST_OF_FLAME = 1,
    EVENT_FIST_OF_FROST = 2,
    EVENT_BLINK         = 3,
    EVENT_BLOSSOM       = 4,
    EVENT_ARCANE_POWER1 = 5,
    EVENT_ARCANE_POWER2 = 6,
    EVENT_ARCANE_POWER3 = 7,
    EVENT_FIRE_WALL     = 8,
};

enum Stages
{
    STAGE_FISTS        = 0,
    STAGE_ARCANE_POWER = 1,
    STAGE_OVERLOAD     = 2,
};

// Over the firewall platter, which the fire wall turns around
Position const GlubtokCenterPos = { -193.29f, -441.95f, 53.76f, 4.88f };
float const GlubtokHoverHeight = 3.0f; // low enough for melee to keep hitting him
float const GlubtokHoverReachBonus = 2.0f; // melee range is 3D, so the hover eats most of his reach
float const GlubtokBlinkMaxDist = 30.0f;

enum Adds
{
    NPC_FIRE_BLOSSOM    = 48957,
    NPC_FROST_BLOSSOM   = 48958,
    NPC_FIREWALL_2A     = 48976,
    NPC_FIREWALL_1A     = 48975,
    NPC_FIREWALL_1B     = 49039,
    NPC_FIREWALL_2B     = 49041,
    NPC_FIREWALL_2C     = 49042,
    NPC_FIREWALL_PLATTER = 48974,
};

class boss_glubtok : public CreatureScript
{
    public:
        boss_glubtok() : CreatureScript("boss_glubtok") { }

        struct boss_glubtokAI : public BossAI
        {
            boss_glubtokAI(Creature* creature) : BossAI(creature, DATA_GLUBTOK)
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
                stage = STAGE_FISTS;
                groundCombatReach = 0.0f;
            }

            uint8 stage;
            float groundCombatReach;

            void Reset() OVERRIDE
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                me->RemoveAurasDueToSpell(SPELL_FROST_BLOSSOM_VISUAL);
                Land();
                SetFireWall(ACTION_FIRE_WALL_STOP);

                stage = STAGE_FISTS;
                SetCombatMovement(true);
                me->SetReactState(REACT_AGGRESSIVE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                stage = STAGE_FISTS;
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_FIST_OF_FLAME, 1000);
                DoZoneInCombat();
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);

                summon->CastSpell(summon, summon->GetEntry() == NPC_FIRE_BLOSSOM ? SPELL_FIRE_BLOSSOM_VISUAL : SPELL_FROST_BLOSSOM_VISUAL, false);
                DoZoneInCombat(summon);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                me->RemoveAurasDueToSpell(SPELL_FROST_BLOSSOM_VISUAL);
                Land();
                SetFireWall(ACTION_FIRE_WALL_STOP);

                Talk(SAY_DEATH);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (stage == STAGE_FISTS && me->GetHealthPct() <= 50)
                {
                    stage = STAGE_ARCANE_POWER;
                    events.Reset();
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->InterruptNonMeleeSpells(false);
                    DoCast(me, SPELL_DEEP_FREEZE, true);
                    Talk(SAY_HEAD1);
                    events.ScheduleEvent(EVENT_ARCANE_POWER1, 1800);
                    return;
                }

                if (stage == STAGE_ARCANE_POWER && me->GetHealthPct() <= 10)
                {
                    stage = STAGE_OVERLOAD;
                    DoCast(me, SPELL_ARCANE_OVERLOAD, true);
                    StartArcaneBeams();
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ARCANE_POWER1:
                            Talk(SAY_HEAD2);
                            events.ScheduleEvent(EVENT_ARCANE_POWER2, 2200);
                            break;
                        case EVENT_ARCANE_POWER2:
                            me->AttackStop();
                            me->RemoveAllAuras();
                            SetCombatMovement(false);
                            me->GetMotionMaster()->Clear();
                            me->NearTeleportTo(GlubtokCenterPos.GetPositionX(), GlubtokCenterPos.GetPositionY(),
                                GlubtokCenterPos.GetPositionZ(), GlubtokCenterPos.GetOrientation(), true);
                            events.ScheduleEvent(EVENT_ARCANE_POWER3, 1000);
                            break;
                        case EVENT_ARCANE_POWER3:
                            Levitate();
                            DoCast(me, SPELL_ARCANE_POWER, true);
                            Talk(SAY_ARCANE_POWER);
                            StartArcaneBeams();
                            if (IsHeroic())
                                events.ScheduleEvent(EVENT_FIRE_WALL, 5000);
                            events.ScheduleEvent(EVENT_BLOSSOM, 6000);
                            break;
                        case EVENT_FIRE_WALL:
                            Talk(SAY_FIRE_WALL);
                            SetFireWall(ACTION_FIRE_WALL_START);
                            break;
                        case EVENT_BLOSSOM:
                        {
                            bool fire = urand(0, 1);
                            if (Creature* bunny = SelectBlossomBunny(fire ? NPC_FIRE_BLOSSOM_BUNNY : NPC_FROST_BLOSSOM_BUNNY))
                                me->CastSpell(bunny, fire ? SPELL_FIRE_BLOSSOM : SPELL_FROST_BLOSSOM, true);
                            else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                me->CastSpell(target, fire ? SPELL_FIRE_BLOSSOM : SPELL_FROST_BLOSSOM, true);
                            events.ScheduleEvent(EVENT_BLOSSOM, urand(3000, 5000));
                            break;
                        }
                        case EVENT_FIST_OF_FLAME:
                            DoCast(me, SPELL_FIST_OF_FLAME);
                            Talk(SAY_FIRE);
                            events.ScheduleEvent(EVENT_BLINK, 12000);
                            events.ScheduleEvent(EVENT_FIST_OF_FROST, 12500);
                            break;
                        case EVENT_FIST_OF_FROST:
                            DoCast(me, SPELL_FIST_OF_FROST);
                            Talk(SAY_FROST);
                            events.ScheduleEvent(EVENT_BLINK, 12000);
                            events.ScheduleEvent(EVENT_FIST_OF_FLAME, 12500);
                            break;
                        case EVENT_BLINK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                DoCast(target, SPELL_BLINK);
                                if (IsHeroic())
                                    DoResetThreat();
                            }
                            break;
                    }
                }

                if (stage == STAGE_FISTS)
                    DoMeleeAttackIfReady();
            }

            private:
                void SetFireWall(int32 action)
                {
                    if (Creature* platter = me->FindNearestCreature(NPC_FIREWALL_PLATTER, 100.0f))
                        platter->AI()->DoAction(action);
                }

                // Hover instead of disabling gravity: Creature::UpdateMovementFlags turns gravity back on for ground creatures
                void Levitate()
                {
                    DoCast(me, SPELL_HOVERING_ANIM, true);
                    me->SetFloatValue(UNIT_FIELD_HOVER_HEIGHT, GlubtokHoverHeight);
                    me->SetHover(true);
                    if (groundCombatReach <= 0.0f)
                    {
                        groundCombatReach = me->GetCombatReach();
                        me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, groundCombatReach + GlubtokHoverReachBonus);
                    }
                }

                void Land()
                {
                    me->RemoveAurasDueToSpell(SPELL_HOVERING_ANIM);
                    me->SetHover(false);
                    me->SetFloatValue(UNIT_FIELD_HOVER_HEIGHT, me->GetCreatureTemplate()->HoverHeight);
                    if (groundCombatReach > 0.0f)
                    {
                        me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, groundCombatReach);
                        groundCombatReach = 0.0f;
                    }
                }

                void StartArcaneBeams()
                {
                    std::list<Creature*> fireList;
                    std::list<Creature*> frostList;

                    GetCreatureListWithEntryInGrid(fireList, me, NPC_FIRE_BLOSSOM_BUNNY, 60.0f);
                    GetCreatureListWithEntryInGrid(frostList, me, NPC_FROST_BLOSSOM_BUNNY, 60.0f);

                    if (fireList.size() > 4)
                        Skyfire::Containers::RandomResizeList(fireList, 4);

                    if (frostList.size() > 3)
                        Skyfire::Containers::RandomResizeList(frostList, 3);

                    for (Creature* fire : fireList)
                        fire->CastSpell(fire, SPELL_ARCANE_FIRE_BEAM, true);

                    for (Creature* frost : frostList)
                        frost->CastSpell(frost, SPELL_ARCANE_FROST_BEAM, true);
                }

                Creature* SelectBlossomBunny(uint32 entry)
                {
                    std::list<Creature*> bunnies;
                    GetCreatureListWithEntryInGrid(bunnies, me, entry, 60.0f);
                    if (bunnies.empty())
                        return NULL;

                    return Skyfire::Containers::SelectRandomContainerElement(bunnies);
                }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_glubtokAI>(creature);
        }
};

// Same layout as the platter's vehicle seats: 16 burning fires per arm, 1.5 to 46.5 yards out, 3 yards apart,
// with a harmless fire between each pair so the wall reads as solid.
// Nested vehicle passengers with invisible models are not drawn by the client, so each fire orbits on its own.
static uint8 const FireWallFiresPerArm = 31;
static float const FireWallSpacing = 1.5f;
static uint8 const FireWallPathPoints = 32;
static float const FireWallTurnTime = 45.0f;
// The fires burn this far above the floor; damage is still centred on the floor below them
static float const FireWallHeight = 2.0f;
// Fire Wall's 3 second cast shows the marker line; the wall starts turning once it finishes
static uint32 const FireWallMarkerTime = 3000;
// The damaging fires' 3 yard radii overlap, but the wall burns a player once per tick
static uint32 const FireWallHitInterval = 900;
// Damaging fires are 3 yards apart, so this still leaves no gap along the wall
static float const FireWallHitRadius = 2.5f;
static uint32 const FireWallResyncInterval = 1000;
static float const FireWallResyncDist = 2.0f;

class npc_glubtok_firewall_platter : public CreatureScript
{
    public:
        npc_glubtok_firewall_platter() : CreatureScript("npc_glubtok_firewall_platter") { }

        struct npc_glubtok_firewall_platterAI : public ScriptedAI
        {
            npc_glubtok_firewall_platterAI(Creature* creature) : ScriptedAI(creature), summons(creature), orbitTimer(0), orbitStart(0), resyncTimer(0)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            struct WallFire
            {
                float radius;
                float startAngle;
            };

            SummonList summons;
            uint32 orbitTimer;
            uint32 orbitStart;
            uint32 resyncTimer;
            std::unordered_map<uint64, WallFire> wallFires;
            std::unordered_map<uint64, uint32> lastWallHit;

            void Reset() OVERRIDE { }

            // A fire whose pulse hits someone enters combat; the default AI then evades it,
            // which strips Fire Wall and sends it home, so that stretch of the wall goes out.
            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
                summon->SetReactState(REACT_PASSIVE);
                summon->AIM_Initialize(new NullCreatureAI(summon));
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_FIRE_WALL_START:
                        if (!summons.empty())
                            return;
                        InstallWall();
                        orbitTimer = FireWallMarkerTime;
                        break;
                    case ACTION_FIRE_WALL_STOP:
                        summons.DespawnAll();
                        orbitTimer = 0;
                        orbitStart = 0;
                        wallFires.clear();
                        lastWallHit.clear();
                        break;
                }
            }

            // Where the client draws the fire right now; the server only refreshes spline positions every 400 ms,
            // which at the end of the wall lags several yards behind
            bool GetFirePosition(uint64 guid, Position& pos) const
            {
                auto itr = wallFires.find(guid);
                if (itr == wallFires.end())
                    return false;

                float const angle = GetFireAngle(itr->second);
                Position const& center = me->GetHomePosition();
                pos.Relocate(center.GetPositionX() + itr->second.radius * std::cos(angle),
                    center.GetPositionY() + itr->second.radius * std::sin(angle), center.GetPositionZ());
                return true;
            }

            bool ClaimWallHit(uint64 guid)
            {
                auto itr = lastWallHit.find(guid);
                if (itr != lastWallHit.end() && GetMSTimeDiffToNow(itr->second) < FireWallHitInterval)
                    return false;

                lastWallHit[guid] = getMSTime();
                return true;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (orbitStart)
                {
                    if (resyncTimer <= diff)
                    {
                        ResyncWall();
                        resyncTimer = FireWallResyncInterval;
                    }
                    else
                        resyncTimer -= diff;
                }

                if (!orbitTimer)
                    return;

                if (orbitTimer > diff)
                {
                    orbitTimer -= diff;
                    return;
                }

                orbitTimer = 0;
                orbitStart = getMSTime();
                resyncTimer = FireWallResyncInterval;
                Position const& center = me->GetHomePosition();
                for (uint64 guid : summons)
                {
                    Creature* fire = ObjectAccessor::GetCreature(*me, guid);
                    if (!fire)
                        continue;

                    auto itr = wallFires.find(guid);
                    if (itr != wallFires.end())
                        StartOrbit(fire, center, itr->second.radius, itr->second.startAngle);

                    if (fire->GetEntry() == NPC_FIREWALL_2C)
                        fire->CastSpell(fire, SPELL_FIRE_WALL, true);
                }
            }

        private:
            float GetFireAngle(WallFire const& wallFire) const
            {
                if (!orbitStart)
                    return wallFire.startAngle;

                uint32 const lapTime = uint32(FireWallTurnTime * float(IN_MILLISECONDS));
                return wallFire.startAngle - 2.0f * float(M_PI) * float(GetMSTimeDiffToNow(orbitStart) % lapTime) / float(lapTime);
            }

            // Fires get knocked off their orbit or lose Fire Wall mid-fight; put them back on the wall's line
            void ResyncWall()
            {
                Position const& center = me->GetHomePosition();
                for (uint64 guid : summons)
                {
                    Creature* fire = ObjectAccessor::GetCreature(*me, guid);
                    if (!fire || !fire->IsAlive())
                        continue;

                    auto itr = wallFires.find(guid);
                    if (itr == wallFires.end())
                        continue;

                    if (!fire->HasAura(SPELL_FIRE_WALL))
                        fire->CastSpell(fire, SPELL_FIRE_WALL, true);

                    float const angle = GetFireAngle(itr->second);
                    float const x = center.GetPositionX() + itr->second.radius * std::cos(angle);
                    float const y = center.GetPositionY() + itr->second.radius * std::sin(angle);
                    if (!fire->movespline->Finalized() && fire->movespline->isCyclic())
                    {
                        Movement::Location const at = fire->movespline->ComputePosition();
                        if (std::hypot(at.x - x, at.y - y) <= FireWallResyncDist)
                            continue;
                    }

                    if (fire->IsInCombat())
                        fire->CombatStop(true);
                    fire->GetMotionMaster()->Clear(false);
                    fire->UpdatePosition(x, y, center.GetPositionZ() + FireWallHeight, angle, true);
                    StartOrbit(fire, center, itr->second.radius, angle);
                }
            }

            void InstallWall()
            {
                Position const& center = me->GetHomePosition();

                for (uint8 arm = 0; arm < 2; ++arm)
                {
                    float angle = Position::NormalizeOrientation(center.GetOrientation() + (arm ? -1.0f : 1.0f) * float(M_PI) / 2.0f);

                    for (uint8 i = 0; i < FireWallFiresPerArm; ++i)
                    {
                        float radius = FireWallSpacing * (i + 1);
                        Position pos;
                        pos.Relocate(center.GetPositionX() + radius * std::cos(angle), center.GetPositionY() + radius * std::sin(angle),
                            center.GetPositionZ() + FireWallHeight, angle);

                        uint32 entry = (i % 2) ? NPC_FIREWALL_2C : (arm ? NPC_FIREWALL_2B : NPC_FIREWALL_2A);
                        if (Creature* fire = me->SummonCreature(entry, pos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            wallFires[fire->GetGUID()] = { radius, angle };
                            // Only the burning fires show the marker orbs; the fillers light up once it turns
                            if (entry != NPC_FIREWALL_2C)
                                fire->CastSpell(fire, SPELL_FIRE_WALL, false);
                        }
                    }
                }
            }

            // Every fire takes FireWallTurnTime per clockwise lap, so the wall stays a straight line while it sweeps the room
            static void StartOrbit(Creature* fire, Position const& center, float radius, float startAngle)
            {
                Movement::MoveSplineInit init(fire);
                for (uint8 i = 0; i < FireWallPathPoints; ++i)
                {
                    float angle = startAngle - 2.0f * float(M_PI) * i / FireWallPathPoints;
                    init.Path().push_back(G3D::Vector3(center.GetPositionX() + radius * std::cos(angle),
                        center.GetPositionY() + radius * std::sin(angle), center.GetPositionZ() + FireWallHeight));
                }

                float lapLength = FireWallPathPoints * 2.0f * radius * std::sin(float(M_PI) / FireWallPathPoints);
                init.SetFly();
                init.SetCyclic();
                init.SetVelocity(lapLength / FireWallTurnTime);
                init.Launch();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_glubtok_firewall_platterAI>(creature);
        }
};

typedef npc_glubtok_firewall_platter::npc_glubtok_firewall_platterAI GlubtokPlatterAI;

static GlubtokPlatterAI* GetPlatterAI(Unit* fire)
{
    if (TempSummon* summon = fire->ToTempSummon())
        if (Unit* summoner = summon->GetSummoner())
            if (Creature* platter = summoner->ToCreature())
                return CAST_AI(GlubtokPlatterAI, platter->AI());
    return NULL;
}

// 91397 - Fire Wall (the per-second tick; its cast visual is what draws the wall)
class spell_glubtok_fire_wall : public SpellScriptLoader
{
    public:
        spell_glubtok_fire_wall() : SpellScriptLoader("spell_glubtok_fire_wall") { }

        class spell_glubtok_fire_wall_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_glubtok_fire_wall_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                Unit* caster = GetCaster();
                if (caster->GetEntry() == NPC_FIREWALL_2C)
                {
                    targets.clear();
                    return;
                }

                WorldLocation const* dest = GetExplTargetDest();
                GlubtokPlatterAI* platterAI = GetPlatterAI(caster);
                targets.remove_if([dest, platterAI](WorldObject* target)
                {
                    Unit* unit = target->ToUnit();
                    if (!unit || !unit->IsControlledByPlayer())
                        return true;
                    if (dest && unit->GetExactDist2d(dest->GetPositionX(), dest->GetPositionY()) > FireWallHitRadius)
                        return true;
                    return platterAI && !platterAI->ClaimWallHit(unit->GetGUID());
                });
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_glubtok_fire_wall_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_glubtok_fire_wall_SpellScript();
        }
};

// 91398 - Fire Wall (the aura on each fire that pulses 91397)
class spell_glubtok_fire_wall_periodic : public SpellScriptLoader
{
    public:
        spell_glubtok_fire_wall_periodic() : SpellScriptLoader("spell_glubtok_fire_wall_periodic") { }

        class spell_glubtok_fire_wall_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_glubtok_fire_wall_periodic_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                PreventDefaultAction();

                Unit* fire = GetTarget();
                Position pos;
                fire->GetPosition(&pos);
                if (GlubtokPlatterAI* platterAI = GetPlatterAI(fire))
                    platterAI->GetFirePosition(fire->GetGUID(), pos);

                fire->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_FIRE_WALL_DAMAGE, true, NULL, aurEff);
                // Fires pulled into combat by their own hits drift off the wall's line
                if (fire->IsInCombat())
                    fire->CombatStop(true);
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_glubtok_fire_wall_periodic_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_glubtok_fire_wall_periodic_AuraScript();
        }
};

// 87925 - Blink
// The random destination ignores walls, so keep it in line of sight of the target and inside the room
class spell_glubtok_blink : public SpellScriptLoader
{
    public:
        spell_glubtok_blink() : SpellScriptLoader("spell_glubtok_blink") { }

        class spell_glubtok_blink_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_glubtok_blink_SpellScript);

            void SetDest(SpellDestination& dest)
            {
                Unit* target = GetExplTargetUnit();
                if (!target)
                    return;

                Position const& pos = dest._position;
                if (pos.GetExactDist2d(GlubtokCenterPos.GetPositionX(), GlubtokCenterPos.GetPositionY()) <= GlubtokBlinkMaxDist &&
                    target->IsWithinLOS(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ()))
                    return;

                Position safePos;
                target->GetFirstCollisionPosition(safePos, 3.0f, target->GetRelativeAngle(&GlubtokCenterPos));
                dest.Relocate(safePos);
            }

            void Register() OVERRIDE
            {
                OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_glubtok_blink_SpellScript::SetDest, EFFECT_0, TARGET_DEST_TARGET_RANDOM);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_glubtok_blink_SpellScript();
        }
};

// 88129 - Fire Blossom, 88169 - Frost Blossom
// The add summon effect only has heroic SpellEffect rows, which SpellInfo also loads for normal
class spell_glubtok_blossom : public SpellScriptLoader
{
    public:
        spell_glubtok_blossom() : SpellScriptLoader("spell_glubtok_blossom") { }

        class spell_glubtok_blossom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_glubtok_blossom_SpellScript);

            void HandleSummonMissile(SpellEffIndex effIndex)
            {
                if (!GetCaster()->GetMap()->IsHeroic())
                    PreventHitDefaultEffect(effIndex);
            }

            void Register() OVERRIDE
            {
                OnEffectHit += SpellEffectFn(spell_glubtok_blossom_SpellScript::HandleSummonMissile, EFFECT_1, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_glubtok_blossom_SpellScript();
        }
};

void AddSC_boss_glubtok()
{
    new boss_glubtok();
    new npc_glubtok_firewall_platter();
    new spell_glubtok_fire_wall();
    new spell_glubtok_fire_wall_periodic();
    new spell_glubtok_blink();
    new spell_glubtok_blossom();
}
