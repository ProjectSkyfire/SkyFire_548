/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "ObjectAccessor.h"
#include "MoveSplineInit.h"
#include "Vehicle.h"
#include "deadmines.h"

enum ScriptedTexts
{
    // Vanessa VanCleef 49541
    SAY_AGGRO                 = 0,
    SAY_KILL                  = 1,
    SAY_ADDS                  = 2,
    SAY_DETONATION_1          = 3,
    SAY_ANNOUNCE_DETONATION_1 = 4,
    SAY_DETONATION_2          = 5,
    SAY_ANNOUNCE_DETONATION_2 = 6,
    SAY_SUICIDE_1             = 7,
    SAY_SUICIDE_2             = 8,
    SAY_ANNOUNCE_POWDER       = 9,
    SAY_DEATH                 = 10,

    // Vanessa VanCleef 49429 (sitting; groups 8 to 12 repeat 1 to 5)
    SAY_NOISE_FROM_ABOVE      = 0,
    SAY_INTRO_1               = 8,
    SAY_INTRO_2               = 9,
    SAY_INTRO_3               = 10,
    SAY_INTRO_4               = 11,
    SAY_INTRO_5               = 12,
};

enum Spells
{
    SPELL_DEADLY_BLADES         = 92622,
    SPELL_BACKSLASH_TARGETING   = 92620,
    SPELL_BACKSLASH             = 92619,
    SPELL_DEFLECTION            = 92614,
    SPELL_VENGEANCE_OF_VANCLEEF = 95542,
    SPELL_POWDER_EXPLOSION      = 96283,
    SPELL_SITTING               = 89279,
    SPELL_NOXIOUS_CONCOCTION    = 92100,
    SPELL_VANESSA_ACHIEVEMENT   = 95654,
    SPELL_SUMMON_ENFORCER       = 92616,
    SPELL_SUMMON_SHADOWGUARD    = 92618,
    SPELL_SUMMON_BLOOD_WIZARD   = 92617,

    SPELL_CLEAR_ALL_DEBUFFS     = 34098,
    SPELL_COSMETIC_BOMB_STATE   = 96280,
    SPELL_FIERY_BLAZE           = 93484,
    SPELL_ROPE_BEAM             = 43785,
};

enum Adds
{
    NPC_DEFIAS_ENFORCER             = 49850,
    NPC_DEFIAS_SHADOWGUARD          = 49852,
    NPC_DEFIAS_BLOOD_WIZARD         = 49854,

    NPC_ROPE                        = 49550,
    NPC_ROPE_ANCHOR                 = 49552,
};

enum Events
{
    EVENT_INTRO_1       = 1,
    EVENT_INTRO_2       = 2,
    EVENT_INTRO_3       = 3,
    EVENT_INTRO_4       = 4,
    EVENT_INTRO_5       = 5,
    EVENT_INTRO_6       = 6,
    EVENT_INTRO_7       = 7,
    EVENT_ADDS_1        = 8,
    EVENT_ADDS_2        = 9,
    EVENT_ADDS_3        = 10,
    EVENT_BACKSLASH     = 11,
    EVENT_DEFLECTION    = 12,
    EVENT_CONTINUE      = 13,

    EVENT_ENTRANCE_FIRE,
    EVENT_FACE_TO_DECK,
    EVENT_TALK_DETONATION,
    EVENT_ANNOUNCE_DETONATION,
    EVENT_SUMMON_DETONATION_FIRE,
    EVENT_SUMMON_ROPES,
    EVENT_CAST_FIERY_BLAZE,
    EVENT_EXTINGUISH_FIRES,
    EVENT_JUMP_ON_DECK,
    EVENT_FACE_TO_ROPES,
    EVENT_VENGEANCE_OF_VANCLEEF,
    EVENT_TALK_SUICIDE_1,
    EVENT_COSMETIC_BOMB_STATE,
    EVENT_TALK_SUICIDE_2,
    EVENT_ANNOUNCE_POWDER,
    EVENT_TALK_LAST_WORDS,
    EVENT_POWDER_EXPLOSION,

    EVENT_START_SWING,
};

#define ACHIEVEMENT_EVENT 27527

const Position addsPos[3] =
{
    {-67.115807f, -829.708740f, 40.959011f, 1.344201f},
    {-69.161903f, -814.125977f, 40.722912f, 1.611237f},
    {-57.057121f, -816.067200f, 41.852272f, 5.565719f}
};

const Position vanessaDetonationJumpPos      = { -53.09028f, -819.5538f, 51.42455f };
const Position vanessaAfterDetonationJumpPos = { -66.64063f, -815.1962f, 40.94127f };

uint32 const MODEL_ID_INVISIBLE = 11686;

Position const VanessaEntranceFirePos[] =
{
    { -38.11458f, -795.2952f, 39.45137f, 3.926991f },
    { -64.02257f, -797.4323f, 39.12309f, 3.926991f },
    { -46.48438f, -791.8073f, 39.12098f, 3.926991f },
    { -75.50694f, -787.1597f, 39.16325f, 3.926991f },
    { -66.18229f, -789.2778f, 39.78834f, 3.926991f },
    { -70.9809f, -795.5f, 39.14447f, 3.926991f }
};

Position const RopePos[] =
{
    { -62.17014f, -839.8438f, 41.48514f, 5.044002f },
    { -64.97049f, -840.0087f, 41.22668f, 4.834562f },
    { -67.79166f, -840.1736f, 40.96664f, 4.712389f },
    { -70.62153f, -840.0104f, 40.71846f, 4.537856f },
    { -73.42361f, -839.8646f, 40.47259f, 4.34587f  }
};

Position const RopePath1[] =
{
    { -59.18403f, -877.3125f, 35.78355f },
    { -58.08333f, -923.9757f, 50.83706f },
    { -52.68403f, -922.8768f, 50.83706f },
    { -58.59201f, -877.1684f, 38.01407f },
    { -62.86285f, -836.3472f, 46.62854f }
};
Position const RopePath2[] =
{
    { -62.23438f, -877.2969f, 35.78355f },
    { -66.63021f, -924.0417f, 50.83706f },
    { -60.85938f, -924.0399f, 50.83706f },
    { -62.03472f, -877.4514f, 38.01407f },
    { -64.96875f, -836.7847f, 46.62854f }
};
Position const RopePath3[] =
{
    { -67.98264f, -877.3785f, 35.78355f },
    { -75.66493f, -924.3958f, 50.83706f },
    { -69.01215f, -923.9114f, 50.83706f },
    { -67.80903f, -877.0972f, 38.01407f },
    { -68.02778f, -836.6268f, 46.62854f }
};
Position const RopePath4[] =
{
    { -74.47049f, -878.3611f, 35.78355f },
    { -82.86111f, -922.4688f, 50.83706f },
    { -77.72396f, -924.1285f, 50.83706f },
    { -71.76563f, -877.132f, 38.01407f },
    { -70.55209f, -836.6754f, 46.62854f }
};
Position const RopePath5[] =
{
    { -77.625f, -877.75f, 35.78355f },
    { -85.46528f, -922.4965f, 50.83706f },
    { -91.52431f, -921.3768f, 50.83706f },
    { -78.06944f, -875.9445f, 38.01407f },
    { -73.34549f, -836.3733f, 46.62854f }
};
Position const* const RopePaths[5] = { RopePath1, RopePath2, RopePath3, RopePath4, RopePath5 };
uint8 const ROPE_PATH_POINTS = 5;

Position const VanessaNightmareFirePos[] =
{
    { -85.18403f, -822.6042f, 39.48589f, 3.926991f },
    { -62.18924f, -834.6563f, 41.49004f, 3.193953f },
    { -76.88889f, -806.1927f, 40.12347f, 3.926991f },
    { -60.28472f, -806.1979f, 41.61631f, 2.75762f },
    { -41.65799f, -834.1059f, 34.51352f, 3.926991f },
    { -79.61111f, -794.7726f, 38.50275f, 3.926991f },
    { -54.19097f, -789.9566f, 38.85344f, 3.926991f },
    { -59.27778f, -787.6597f, 39.37201f, 3.926991f },
    { -44.94792f, -801.1077f, 40.703f, 3.926991f },
    { -75.33681f, -833.434f, 40.29444f, 3.193953f },
    { -62.4375f, -825.9114f, 41.42738f, 3.193953f },
    { -70.38889f, -824.9722f, 40.70635f, 3.193953f },
    { -66.54514f, -815.118f, 41.0335f, 3.193953f },
    { -57.90972f, -816.8611f, 41.85588f, 2.75762f },
    { -54.5382f, -833.9739f, 42.20368f, 2.75762f },
    { -81.48264f, -832.3143f, 39.79344f, 2.75762f },
    { -76.38541f, -821.6371f, 40.15092f, 2.75762f },
    { -84.03125f, -817.4323f, 39.54358f, 2.75762f },
    { -34.31597f, -825.7864f, 35.14482f, 3.926991f },
    { -101.6024f, -822.2257f, 38.22025f, 3.926991f },
    { -110.0f, -821.908f, 30.27846f, 3.926991f },
    { -54.32118f, -840.3108f, 33.4867f, 3.926991f },
    { -92.3125f, -835.5712f, 31.01794f, 3.926991f },
    { -95.20313f, -826.7986f, 38.69661f, 3.926991f },
    { -81.6875f, -791.7795f, 25.86889f, 3.926991f },
    { -75.52952f, -788.3438f, 26.0653f, 3.926991f },
    { -104.3299f, -806.0121f, 30.58083f, 3.926991f },
    { -87.6875f, -830.8108f, 39.29154f, 2.75762f },
    { -46.73611f, -821.1771f, 52.5815f, 3.193953f },
    { -69.06944f, -834.368f, 40.86119f, 3.193953f },
    { -68.94444f, -805.1528f, 40.82362f, 3.926991f },
    { -38.67708f, -816.1389f, 43.68202f, 3.926991f },
    { -46.09722f, -830.2518f, 43.00029f, 2.75762f },
    { -46.44965f, -809.7448f, 42.93429f, 3.926991f },
    { -54.49479f, -826.5833f, 42.19642f, 3.926991f },
    { -37.77257f, -824.493f, 43.78171f, 3.926991f },
    { -106.7882f, -827.8351f, 30.3655f, 3.926991f },
    { -85.05209f, -808.5243f, 39.47224f, 3.926991f },
    { -108.5208f, -840.7136f, 17.11709f, 3.926991f },
    { -74.0f, -813.632f, 40.37197f, 3.926991f },
    { -101.724f, -815.1875f, 38.20302f, 3.926991f },
    { -109.6736f, -832.7049f, 20.99191f, 3.926991f },
    { -93.49653f, -809.6979f, 38.80095f, 3.926991f },
    { -92.09202f, -843.8507f, 21.32712f, 3.926991f },
    { -82.74479f, -853.8368f, 17.44633f, 3.926991f },
    { -83.51563f, -847.0851f, 18.62521f, 3.926991f },
    { -77.3316f, -848.7101f, 16.88625f, 3.926991f },
    { -47.97917f, -851.7882f, 18.46356f, 3.926991f },
    { -54.41667f, -846.2222f, 22.62641f, 3.926991f },
    { -46.15799f, -843.5104f, 23.03055f, 3.926991f },
    { -32.24479f, -844.8195f, 19.15213f, 3.926991f },
    { -28.87847f, -840.0226f, 19.30344f, 3.926991f },
    { -31.54861f, -830.5191f, 23.84564f, 3.926991f },
    { -53.39583f, -808.4184f, 42.27031f, 2.75762f },
    { -59.65799f, -786.8472f, 18.00813f, 3.926991f },
    { -51.8559f, -788.4774f, 18.27221f, 3.926991f },
    { -115.3142f, -827.2327f, 20.94427f, 3.926991f },
    { -116.1042f, -835.6597f, 17.07181f, 3.926991f },
    { -17.43924f, -821.8785f, 19.89213f, 3.926991f },
    { -22.3125f, -807.7292f, 19.61356f, 3.926991f },
    { -17.77431f, -814.7327f, 19.87694f, 3.926991f },
    { -116.9809f, -803.1337f, 17.05497f, 3.926991f },
    { -122.8872f, -816.658f, 17.01834f, 3.926991f },
    { -65.53993f, -768.6545f, 28.01228f, 3.926991f },
    { -123.3351f, -807.2153f, 16.98381f, 3.926991f }
};

// The fires with these two orientations go out after 11 seconds, the others burn on
static bool IsExtinguishableFire(Position const& pos)
{
    return std::fabs(pos.GetOrientation() - 2.75762f) < 0.001f || std::fabs(pos.GetOrientation() - 3.193953f) < 0.001f;
}

class boss_vanessa_vancleef : public CreatureScript
{
    public:
        boss_vanessa_vancleef() : CreatureScript("boss_vanessa_vancleef") { }

        struct boss_vanessa_vancleefAI : public BossAI
        {
            boss_vanessa_vancleefAI(Creature* creature) : BossAI(creature, DATA_VANESSA)
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
                me->SetReactState(REACT_DEFENSIVE);
                ResetFightState();
            }

            void ResetFightState()
            {
                detonationCounter = 0;
                prepareForReEngage = false;
                allowDeath = false;
                talkedAdds = false;
                nightmareFlames.clear();
                extinguishableFlames.clear();
            }

            void Reset() OVERRIDE
            {
                me->SetReactState(REACT_DEFENSIVE);
                _Reset();

                // The poison phase is not scripted, so the achievement stays available
                me->GetMap()->SetWorldState(WORLDSTATE_VANCLEEF_VINDICATION, 1);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    me->SetVisible(instance->GetData(DATA_VANESSA_EVENT) == DONE);
                }

                ResetFightState();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_VANCLEEF_VINDICATION, 1);

                _EnterCombat();

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_VANESSA_ACHIEVEMENT);
                }

                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_BACKSLASH, urand(10000, 15000));
                // Reinforcements come from the pull until 50%, Deflection 11 seconds in and again after each detonation
                events.ScheduleEvent(EVENT_ADDS_1, 1000);
                events.ScheduleEvent(EVENT_DEFLECTION, 11000);
                events.ScheduleEvent(EVENT_ENTRANCE_FIRE, 12500);

                DoZoneInCombat();
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim && victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                // The fires and the ropes are scenery, the adds fight
                if (summon->GetEntry() == NPC_GENERAL_PURPOSE_DUMMY_JMF || summon->GetEntry() == NPC_GLUBTOK_NIGHTMARE_FIRE_BUNNY ||
                    summon->GetEntry() == NPC_ROPE)
                {
                    summons.Summon(summon);
                    summon->SetReactState(REACT_PASSIVE);

                    // A rope has to stay clickable, so the immunity flags are for the fires only
                    if (summon->GetEntry() != NPC_ROPE)
                    {
                        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                        summon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
                    }

                    if (summon->GetEntry() == NPC_GENERAL_PURPOSE_DUMMY_JMF)
                        summon->CastSpell(summon, SPELL_FIERY_BLAZE, true);
                    return;
                }

                BossAI::JustSummoned(summon);
            }

            // No death before the Powder Explosion; the detonations at 50% and 25%, the countdown at 1% and the re-engage
            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (damage >= me->GetHealth() && !allowDeath)
                    damage = me->GetHealth() - 1;

                uint32 const after = me->GetHealth() > damage ? me->GetHealth() - damage : 0;
                float const pctAfter = 100.0f * float(after) / float(me->GetMaxHealth());

                bool const midDetonation = me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE) || detonationCounter > 2;
                if (!midDetonation && ((pctAfter < 50.0f && detonationCounter == 0) || (pctAfter < 25.0f && detonationCounter == 1)))
                {
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    me->RemoveAurasDueToSpell(SPELL_DEADLY_BLADES);
                    events.Reset();
                    DoCast(me, SPELL_CLEAR_ALL_DEBUFFS, true);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->GetMotionMaster()->MoveJump(vanessaDetonationJumpPos.GetPositionX(), vanessaDetonationJumpPos.GetPositionY(),
                        vanessaDetonationJumpPos.GetPositionZ(), std::max(1.0f, me->GetExactDist(&vanessaDetonationJumpPos)), 27.96389f);
                    events.ScheduleEvent(EVENT_FACE_TO_DECK, 1200);
                    ++detonationCounter;
                    prepareForReEngage = false;
                    return;
                }

                if (!midDetonation && pctAfter <= 1.0f && detonationCounter == 2)
                {
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    me->RemoveAurasDueToSpell(SPELL_DEADLY_BLADES);
                    events.Reset();
                    DoCast(me, SPELL_CLEAR_ALL_DEBUFFS, true);
                    events.ScheduleEvent(EVENT_TALK_SUICIDE_1, 200);
                    ++detonationCounter;
                    return;
                }

                if (prepareForReEngage)
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    prepareForReEngage = false;
                    if (detonationCounter == 1)
                        events.CancelEvent(EVENT_VENGEANCE_OF_VANCLEEF);

                    events.ScheduleEvent(EVENT_DEFLECTION, 400);
                    events.ScheduleEvent(EVENT_BACKSLASH, 5200);
                }
            }

            void SummonRopes()
            {
                // One set at a time: a rope that was not used goes before the next five come
                summons.DespawnEntry(NPC_ROPE);

                std::list<Creature*> anchors;
                GetCreatureListWithEntryInGrid(anchors, me, NPC_ROPE_ANCHOR, 200.0f);
                for (uint8 i = 0; i < 5; ++i)
                {
                    Creature* rope = me->SummonCreature(NPC_ROPE, RopePos[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    if (!rope)
                        continue;

                    // The anchors and the ropes both run west to east: the anchor nearest in x holds the beam
                    Creature* anchor = NULL;
                    float best = 1000.0f;
                    for (Creature* candidate : anchors)
                    {
                        float const dx = std::fabs(candidate->GetPositionX() - RopePos[i].GetPositionX());
                        if (dx < best)
                        {
                            best = dx;
                            anchor = candidate;
                        }
                    }

                    if (anchor)
                        anchor->CastSpell(rope, SPELL_ROPE_BEAM, true);
                }
            }

            void ExecuteEvent(uint32 eventId) OVERRIDE
            {
                switch (eventId)
                {
                    case EVENT_BACKSLASH:
                        DoCast(me, SPELL_DEADLY_BLADES);
                        events.ScheduleEvent(EVENT_CONTINUE, 6000);
                        break;
                    case EVENT_CONTINUE:
                        if (Unit* victim = me->GetVictim())
                            me->GetMotionMaster()->MoveChase(victim);
                        break;
                    case EVENT_DEFLECTION:
                        if (me->HealthAbovePct(25))
                            DoCast(me, SPELL_DEFLECTION);
                        break;
                    case EVENT_ADDS_1:
                        if (!talkedAdds)
                        {
                            Talk(SAY_ADDS);
                            talkedAdds = true;
                        }
                        me->SummonCreature(NPC_DEFIAS_ENFORCER, addsPos[0]);
                        events.ScheduleEvent(EVENT_ADDS_2, 5000);
                        break;
                    case EVENT_ADDS_2:
                        me->SummonCreature(NPC_DEFIAS_SHADOWGUARD, addsPos[1]);
                        events.ScheduleEvent(EVENT_ADDS_3, 5000);
                        break;
                    case EVENT_ADDS_3:
                        me->SummonCreature(NPC_DEFIAS_BLOOD_WIZARD, addsPos[2]);
                        events.ScheduleEvent(EVENT_ADDS_1, 60000);
                        break;
                    case EVENT_ENTRANCE_FIRE:
                        for (Position const& pos : VanessaEntranceFirePos)
                            me->SummonCreature(NPC_GENERAL_PURPOSE_DUMMY_JMF, pos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        break;
                    case EVENT_FACE_TO_DECK:
                        me->SetFacingTo(2.932153f);
                        events.ScheduleEvent(EVENT_TALK_DETONATION, 200);
                        break;
                    case EVENT_TALK_DETONATION:
                        Talk(detonationCounter == 1 ? SAY_DETONATION_1 : SAY_DETONATION_2);
                        events.ScheduleEvent(EVENT_ANNOUNCE_DETONATION, 3600);
                        break;
                    case EVENT_ANNOUNCE_DETONATION:
                        Talk(detonationCounter == 1 ? SAY_ANNOUNCE_DETONATION_1 : SAY_ANNOUNCE_DETONATION_2);
                        events.ScheduleEvent(EVENT_SUMMON_DETONATION_FIRE, 600);
                        events.ScheduleEvent(EVENT_SUMMON_ROPES, 700);
                        break;
                    case EVENT_SUMMON_DETONATION_FIRE:
                        for (Position const& pos : VanessaNightmareFirePos)
                        {
                            Creature* fire = me->SummonCreature(NPC_GLUBTOK_NIGHTMARE_FIRE_BUNNY, pos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            if (!fire)
                                continue;

                            nightmareFlames.insert(fire->GetGUID());
                            if (IsExtinguishableFire(pos))
                                extinguishableFlames.insert(fire->GetGUID());
                        }
                        events.ScheduleEvent(EVENT_CAST_FIERY_BLAZE, 4800);
                        events.ScheduleEvent(EVENT_EXTINGUISH_FIRES, 11000);
                        break;
                    case EVENT_SUMMON_ROPES:
                        SummonRopes();
                        break;
                    case EVENT_CAST_FIERY_BLAZE:
                    {
                        // The first set burns everywhere, the second only lights the deck again (the rest still burns)
                        GuidSet const& fires = detonationCounter == 1 ? nightmareFlames : extinguishableFlames;
                        for (uint64 guid : fires)
                            if (Creature* fire = ObjectAccessor::GetCreature(*me, guid))
                                fire->CastSpell(fire, SPELL_FIERY_BLAZE, true);
                        break;
                    }
                    case EVENT_EXTINGUISH_FIRES:
                        for (uint64 guid : extinguishableFlames)
                            if (Creature* fire = ObjectAccessor::GetCreature(*me, guid))
                                fire->RemoveAllAuras();
                        events.ScheduleEvent(EVENT_JUMP_ON_DECK, 3200);
                        break;
                    case EVENT_JUMP_ON_DECK:
                        me->GetMotionMaster()->MoveJump(vanessaAfterDetonationJumpPos.GetPositionX(), vanessaAfterDetonationJumpPos.GetPositionY(),
                            vanessaAfterDetonationJumpPos.GetPositionZ(), std::max(1.0f, me->GetExactDist(&vanessaAfterDetonationJumpPos)), 32.75936f);
                        events.ScheduleEvent(EVENT_FACE_TO_ROPES, 1200);
                        break;
                    case EVENT_FACE_TO_ROPES:
                        me->SetFacingTo(4.642576f);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        prepareForReEngage = true;
                        events.ScheduleEvent(EVENT_VENGEANCE_OF_VANCLEEF, 1200);
                        break;
                    case EVENT_VENGEANCE_OF_VANCLEEF:
                        DoCast(me, SPELL_VENGEANCE_OF_VANCLEEF, true);
                        events.ScheduleEvent(EVENT_VENGEANCE_OF_VANCLEEF, 2400);
                        break;
                    case EVENT_TALK_SUICIDE_1:
                        Talk(SAY_SUICIDE_1);
                        events.ScheduleEvent(EVENT_COSMETIC_BOMB_STATE, 4600);
                        break;
                    case EVENT_COSMETIC_BOMB_STATE:
                        DoCast(me, SPELL_COSMETIC_BOMB_STATE, true);
                        events.ScheduleEvent(EVENT_TALK_SUICIDE_2, 200);
                        break;
                    case EVENT_TALK_SUICIDE_2:
                        Talk(SAY_SUICIDE_2);
                        events.ScheduleEvent(EVENT_ANNOUNCE_POWDER, 2400);
                        break;
                    case EVENT_ANNOUNCE_POWDER:
                        Talk(SAY_ANNOUNCE_POWDER);
                        events.ScheduleEvent(EVENT_TALK_LAST_WORDS, 3500);
                        break;
                    case EVENT_TALK_LAST_WORDS:
                        Talk(SAY_DEATH);
                        events.ScheduleEvent(EVENT_POWDER_EXPLOSION, 1100);
                        break;
                    case EVENT_POWDER_EXPLOSION:
                        allowDeath = true;
                        DoCastAOE(SPELL_POWDER_EXPLOSION, true);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                    ExecuteEvent(eventId);

                if (!prepareForReEngage && !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
                    DoMeleeAttackIfReady();
            }

        private:
            uint8 detonationCounter;
            bool prepareForReEngage;
            bool allowDeath;
            bool talkedAdds;
            GuidSet nightmareFlames;
            GuidSet extinguishableFlames;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_vanessa_vancleefAI>(creature);
        }
};

class npc_vanessa_vancleef_a_note_from_vanessa : public CreatureScript
{
    public:
        npc_vanessa_vancleef_a_note_from_vanessa() : CreatureScript("npc_vanessa_vancleef_a_note_from_vanessa") { }

        bool OnGossipHello(Player* /*player*/, Creature* creature) OVERRIDE
        {
            InstanceScript* instance = creature->GetInstanceScript();
            if (!instance)
                return true;

            if (instance->GetBossState(DATA_CAPTAIN) != DONE)
                return true;

            if (instance->GetData(DATA_VANESSA_EVENT) != NOT_STARTED)
                return true;

            creature->SummonCreature(NPC_VANESSA_SITTING, vanessaPos);
            instance->SetData(DATA_VANESSA_EVENT, IN_PROGRESS);

            creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            creature->DespawnOrUnsummon(1000);
            return true;
        }
};

class npc_vanessa_vancleef_vanessa_sitting : public CreatureScript
{
    public:
        npc_vanessa_vancleef_vanessa_sitting() : CreatureScript("npc_vanessa_vancleef_vanessa_sitting") { }

        struct npc_vanessa_vancleef_vanessa_sittingAI : public ScriptedAI
        {
            npc_vanessa_vancleef_vanessa_sittingAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->setActive(true);
                instance = me->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                Talk(SAY_NOISE_FROM_ABOVE);
                events.ScheduleEvent(EVENT_INTRO_1, 1000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_1:
                            Talk(SAY_INTRO_1);
                            events.ScheduleEvent(EVENT_INTRO_2, 5000);
                            break;
                        case EVENT_INTRO_2:
                            Talk(SAY_INTRO_2);
                            events.ScheduleEvent(EVENT_INTRO_3, 8000);
                            break;
                        case EVENT_INTRO_3:
                            Talk(SAY_INTRO_3);
                            events.ScheduleEvent(EVENT_INTRO_4, 8000);
                            break;
                        case EVENT_INTRO_4:
                            Talk(SAY_INTRO_4);
                            events.ScheduleEvent(EVENT_INTRO_5, 5000);
                            break;
                        case EVENT_INTRO_5:
                            Talk(SAY_INTRO_5);
                            DoCastAOE(SPELL_NOXIOUS_CONCOCTION);
                            me->RemoveAura(SPELL_SITTING);
                            events.ScheduleEvent(EVENT_INTRO_6, 5000);
                            break;
                        case EVENT_INTRO_6:
                            me->GetMotionMaster()->MoveJump(centershipPos.GetPositionX(), centershipPos.GetPositionY(), centershipPos.GetPositionZ(), 5.0f, 10.0f);
                            events.ScheduleEvent(EVENT_INTRO_7, 5000);
                            break;
                        case EVENT_INTRO_7:
                            if (instance)
                            {
                                instance->SetData(DATA_VANESSA_EVENT, DONE);
                                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEVEMENT_EVENT);
                            }
                            if (Creature* pVanessa = me->FindNearestCreature(NPC_VANESSA_VANCLEEF, 40.0f))
                                pVanessa->SetVisible(true);
                            me->DespawnOrUnsummon(1000);
                            break;
                    }
                }
            }

        private:
            InstanceScript* instance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_vanessa_vancleef_vanessa_sittingAI>(creature);
        }
};

// The Rope 49550 (vehicle 1399, spellclick Ride Vehicle): 0.8 seconds after a player boards it swings him off the ship and
// back over a path (22 yards a second, invisible) and despawns 8.9 seconds later, the passenger lands on the deck
struct npc_deadmines_vanessa_rope : public ScriptedAI
{
    npc_deadmines_vanessa_rope(Creature* creature) : ScriptedAI(creature), swinging(false) { }

    void Reset() OVERRIDE { }

    void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) OVERRIDE
    {
        if (!passenger || !apply || passenger->GetTypeId() != TypeID::TYPEID_PLAYER || swinging)
            return;

        swinging = true;
        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        events.ScheduleEvent(EVENT_START_SWING, 800);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        events.Update(diff);

        if (events.ExecuteEvent() != EVENT_START_SWING)
            return;

        for (uint8 i = 0; i < 5; ++i)
        {
            if (RopePos[i].GetExactDist(me) > 1.0f)
                continue;

            me->DespawnOrUnsummon(8900);
            me->SetDisplayId(MODEL_ID_INVISIBLE);

            Movement::MoveSplineInit init(me);
            Movement::PointsArray path;
            for (uint8 point = 0; point < ROPE_PATH_POINTS; ++point)
                path.push_back(G3D::Vector3(RopePaths[i][point].GetPositionX(), RopePaths[i][point].GetPositionY(), RopePaths[i][point].GetPositionZ()));
            init.MovebyPath(path);
            init.SetSmooth();
            init.SetVelocity(22.0f);
            init.Launch();
            break;
        }
    }

private:
    EventMap events;
    bool swinging;
};

// Backslash Targeting 92620
class spell_vanessa_vancleef_backslash_targeting : public SpellScriptLoader
{
    public:
        spell_vanessa_vancleef_backslash_targeting() : SpellScriptLoader("spell_vanessa_vancleef_backslash_targeting") { }

        class spell_vanessa_vancleef_backslash_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_vanessa_vancleef_backslash_targeting_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_BACKSLASH, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_vanessa_vancleef_backslash_targeting_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_vanessa_vancleef_backslash_targeting_SpellScript();
        }
};

void AddSC_boss_vanessa_vancleef()
{
    new boss_vanessa_vancleef();
    new npc_vanessa_vancleef_a_note_from_vanessa();
    new npc_vanessa_vancleef_vanessa_sitting();
    new spell_vanessa_vancleef_backslash_targeting();
    RegisterCreatureAI(npc_deadmines_vanessa_rope);
}
