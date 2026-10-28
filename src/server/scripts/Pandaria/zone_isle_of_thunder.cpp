/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "GameObjectAI.h"
#include "Transport.h"
#include "Group.h"
#include "Player.h"

enum Spells
{
    // Nalak
    SPELL_ARC_NOVA                  = 136338,
    SPELL_LIGHTNING_TETHER          = 136339,
    SPELL_STORMCLOUD                = 136340,
    SPELL_LIGHTNING_TETHER_TRIGGER  = 136353,
    SPELL_LIGHTNING_TETHER_30Y      = 136349, // if target dist > 30 from nalak
    SPELL_STATIC_SHIELD             = 136341,

    // Haywire Sunreaver Construct
    SPELL_HAYWIRE                   = 139324,
    
    // Mumta
    SPELL_SPIRIT_BOLTS              = 139004,
    SPELL_DARKNESS_DARKNESS         = 139010,
    SPELL_VENGEFUL_SPIRIT           = 139003,

    // Ku'lai the Skyclaw & Tamed Pterrorwing
    SPELL_BLADEWIND                 = 139125,
    SPELL_LEAPING_REND              = 139122,
    SPELL_SPIRIT_BOND               = 139124,
    SPELL_SUMMON_PTERRORWING        = 139123,

    // Progenitus
    SPELL_UNLEASH_BLOOD             = 139146,
    SPELL_MUTAGENIC_BURST           = 139169,
    SPELL_EVOLUTION                 = 139144,

    // Goda
    SPELL_SHELL_SPIN                = 139178,
    SPELL_CRYSTALLINE_SHIELD        = 139184,

    // God-Hulk Ramuk
    SPELL_CROUCH                    = 139014,
    SPELL_BLESSING_OF_THUNDER       = 139015,
    SPELL_POUND                     = 139016,

    // Al'tabim the All-Seeing
    SPELL_OMEN                      = 139250,
    SPELL_METEOR_SHOWER             = 139248,
    SPELL_SHADOW_BOLT_VOLLEY        = 139419,

    // Backbreaker Uru
    SPELL_BREAK_SPINE               = 139260,
    SPELL_MASSIVE_STOMP             = 139262,

    // Lu-Ban
    SPELL_SKYFORGED_HAMMERS         = 139269,
    SPELL_STORM_BOLT                = 139275,

    // Molthor
    //SPELL_SCARAB_SWARM              = 138036, //////////////////////////////////
    //SPELL_THUNDER_CRUSH             = 138044, // See Zandalaris shared spells //
    //SPELL_HORRIFIC_VISAGE           = 138040, //////////////////////////////////

    // Ra'sha
    SPELL_BLOODY_STRIKE             = 77558,
    SPELL_BLOODBOLT                 = 90387,
    SPELL_MARK_OF_BLOOD             = 61606,
    SPELL_MARK_OF_BLOOD_TRIGGER     = 61607,

    // Spirit of Warlord Teng
    SPELL_SPIRIT_SLASH              = 137140,
    SPELL_SPIRIT_S_WRATH            = 137402,
    SPELL_TENG_GHOST_VISUAL_1       = 134090,
    SPELL_TENG_GHOST_VISUAL_2       = 137135,

    // Draining Spirit
    SPELL_DRAIN_SPIRIT              = 137125,
    
    // Restless Spirit
    SPELL_BURIED_ALIVE              = 137280,

    // Kor'dok
    SPELL_FURIOUS_CHARGE            = 138063,
    SPELL_CALL_THUNDER              = 137997,
    SPELL_BATTLE_RAGE               = 137653,

    // Tinzo
    SPELL_DISRUPT                   = 137574,
    SPELL_FIRE_BURST                = 137703,
    SPELL_FLAME_BARRAGE             = 137705,

    // Echo of Kros
    SPELL_GHOST_DINOSAUR_STAMPEDE   = 136734,
    SPELL_GHASTLY_ROAR              = 136716,
    SPELL_DEVOUR                    = 136872,
    SPELL_RAGE_OF_KROS              = 137334,

    // Electromancer Ju'le
    SPELL_CHAIN_LIGHTNING           = 137884,
    SPELL_ELECTRIFIED               = 136691,

    // Lightning Elemental
    SPELL_STATIC_ELECTRICITY        = 137985,

    // Qi'nor
    SPELL_LIGHTNING_BOLT            = 138018,
    SPELL_LIGHTNING_PROD            = 138019,
    SPELL_LIGHTNING_STRIKE          = 138012,
    
    // Ancient Mogu Guardian
    SPELL_MANTLE_OF_THUNDER_KING    = 138073,

    // Floor tiles (trigger)
    SPELL_NULLIFY_MAGIC             = 138004,
    SPELL_ANCIENT_FLAMES            = 138015,
    SPELL_MOGU_POWER                = 138017,

    // Windweaver Akil'amon
    SPELL_IMPENDING_GUST            = 138976,
    SPELL_LIFT_OFF                  = 139021,
    
    // Soaring Eagle
    SPELL_PLUCKED                   = 97318,
    
    // Cera
    SPELL_DOUBLE_EDGE_TRASH         = 136866,
    SPELL_CHARGE                    = 138660,
    SPELL_CHARGE_IMPACT             = 138661,
    SPELL_REFLECTIVE_ARMOR_PLATING  = 136865,

    // Incomplete Drakkari Colossus
    SPELL_DEATH_STARE               = 136854,
    SPELL_GROUND_SLAM               = 137074,
    SPELL_SHOCKWAVE                 = 136847,
    SPELL_FLAMETHROWER              = 136898,

    // Zandalaris shared spells
    // Auras
    SPELL_SCARAB_BROOCH             = 138046,
    SPELL_TRIBAL_MASK               = 138047,
    SPELL_STAR_MAP                  = 138048,
    SPELL_SPIRIT_CHARM              = 138049,
    SPELL_THUNDERING_SIGIL          = 138050,
    // Spells
    SPELL_SCARAB_SWARM              = 138036, // Needs SPELL_SCARAB_BROOCH to be casted
    SPELL_HORRIFIC_VISAGE           = 138040, // Needs SPELL_TRIBAL_MASK to be casted
    SPELL_METEOR_SHOWER_2           = 138042, // Needs SPELL_STAR_MAP to be casted
    SPELL_VENGEFUL_SPIRIT_2         = 138043, // Needs SPELL_SPIRIT_CHARM to be casted
    SPELL_THUNDER_CRUSH             = 138044, // Needs SPELL_THUNDERING_SIGIL to be casted

    // Misc | Trash
    SPELL_SINISTER_STRIKE           = 129040,
    SPELL_SUNDER_ARMOR              = 76622,
    SPELL_DEVASTATE                 = 78660,
    SPELL_THERMAL_STRIKE            = 139894,
    SPELL_THERMAL_EXPANSION_AOE     = 139921,
    SPELL_THERMAL_EXPANSION_AT      = 139917,
    SPELL_THERMAL_EXPANSION_EFF     = 139971,
    SPELL_THERMODINAMYCS            = 139913,
    SPELL_THERMAL_COLLAPSE_AT       = 139911,
    SPELL_THERMAL_COLLAPSE_AOE      = 139645,
    SPELL_THERMAL_COLLAPSE_EFF      = 139970,
    SPELL_CURSE_OF_STONE            = 140207,
    SPELL_BODY_SLAM_25Y             = 140202,
    SPELL_BODY_SLAM_5Y              = 140199,
    SPELL_STATUE_FROZEN_SHEATH      = 123825,
    SPELL_LIGHTNING_FIELD           = 137990,
    SPELL_LIGHTNING_FIELD_DUMMY     = 138003,
    SPELL_LIGHTNING_FIELD_EFF       = 136717,
    SPELL_GASEOUS_ERRUPTION         = 140325,
    SPELL_VALOROUS_SPIN             = 140349,
    SPELL_MIGHTY_STOMP              = 136855,
    SPELL_MIGHTY_CRUSH              = 136844,
    SPELL_WAVE_OF_MIGHT             = 140254,
    SPELL_WAVE_OF_MIGHT_AT          = 140256, // should move
    SPELL_WAVE_OF_MIGHT_EFF         = 140257,
    SPELL_LIGHTNING_CRACK           = 140239,
    SPELL_LIGHTNING_CRACK_SUMM      = 140236,
    SPELL_LIGHTNING_CRACK_AURA      = 140241,
    SPELL_LIGHTNING_CRACK_VISUAL    = 140240,
    SPELL_REVERBERATING_SMASH       = 140220,
    SPELL_REVERBERATING_SMASH_RING  = 140221,
    SPELL_REVERBERATING_SMASH_INNER = 140224,
    SPELL_REVERBERATING_SMASH_OUT   = 140225,
    SPELL_THUNDERCLEAVE             = 139035,
    SPELL_SERRATED_SLASH            = 87395,
    SPELL_SHIELD_BASH               = 140659,
    SPELL_ALERT_DRUMBS              = 136838,
    SPELL_AXE_BLOW                  = 137732,
    SPELL_PIERCING_CRY              = 144039,
    SPELL_SHADOW_NOVA               = 75073,
    SPELL_SHADOW_BOLT               = 91997,
    SPELL_PIERCING_CRY_2            = 140408,
    SPELL_ARCANE_BOLT               = 13748,
    SPELL_ARCANE_EXPLOSION          = 51820,
    SPELL_ZANDALARI_POTION          = 136772,
    SPELL_JAGUAR_STALKER            = 136778,
    SPELL_JAGUAR_FEROCITY           = 136775,
    SPELL_JAGUAR_SPEED              = 136777,
    SPELL_KICK                      = 137708,
    SPELL_FOCUSED_STRIKE            = 82615,
    SPELL_SKULL_KICK                = 137707,
    SPELL_WILD_ENERGY               = 136738,
    SPELL_LIGHTNING_BOLT_2          = 9532,
    SPELL_CHAIN_LIGHTNING_2         = 12058,
    SPELL_RUNE_OF_THE_STORM         = 136737,
    SPELL_SLAM                      = 79881,
    SPELL_THUNDER_CLAP              = 81140,
    SPELL_CHAIN_LIGHTNING_3         = 140624,
    SPELL_STORM_SHIELD              = 136748,
    SPELL_PETRIFYING_GAZE           = 140248,
    SPELL_SHELLWIND                 = 140356,
    SPELL_BITE                      = 140374,
    SPELL_REND                      = 140396,
    SPELL_POUNCE_JUMP               = 133076,
    SPELL_POUNCE_EFF                = 133075,
    SPELL_THROW_AXE                 = 136585,
    SPELL_HUNTERS_MARK              = 80016,
    SPELL_BESTIAL_ROAR              = 139385,
    SPELL_CHARGE_EFF                = 22120,
    SPELL_BRUTE_SLAM                = 138433,
    SPELL_CONFIDENCE_OF_KROS        = 138422,
    SPELL_FLING_FILTH               = 83012,
    SPELL_REND_2                    = 140276,
    SPELL_SUMMON_ELSIA_TOWNLONG     = 139726,
    SPELL_SUMMON_ELSIA_ISLE         = 139727,
    SPELL_SUMMON_VEREESA_TOWNLONG   = 139722,
    SPELL_SUMMON_VEREESA_ISLE       = 139725,
    SPELL_TELEPORT_THUNDER_ISLE_H   = 140059,
    SPELL_TELEPORT_THUNDER_ISLE_A   = 140125,
    SPELL_SUMMON_PLAYER_MOUNT_H     = 140056,
    SPELL_SUMMON_PLAYER_MOUNT_A     = 140119,
    SPELL_SUMMON_LORTHEMAR          = 139721,
    SPELL_SAUROK_DEAD_AURA          = 138364, // extra button
    SPELL_PLANT_MOGU_WEAPON_EFF     = 138368,
    SPELL_VISUAL_RITUAL             = 133195,
    SPELL_MUMMY_BURN_CORPSE         = 136711,
    SPELL_CRYSTALL_LIGHTNING_STRIKE = 139511,
    SPELL_SIPHON_ESSENCE            = 137889,
    SPELL_EMPOWER_LIGHTNING_LANCE   = 139959,
    SPELL_SPEED_METAL_ACHIEV        = 139370,
    SPELL_YOU_MADE_ME_BLEED_ACHIEV  = 139306,
    SPELL_BLOOD_BOLT                = 136985,
    SPELL_EXSANGUINATION            = 137696,
    SPELL_EXSANGUINATION_SUMM       = 136981,
    SPELL_RIVERS_OF_BLOOD           = 140526,
    SPELL_TOSS_ENERGIZED_METAL_TAR  = 137547,
    SPELL_TOSS_ENERGIZED_METAL      = 137545,
    SPELL_SUMMON_ENERGIZED_METAL    = 137544,
    SPELL_POWER_SURGE_ACHIEV        = 136842,
    SPELL_CHARGED_BOLT_PERIODIC     = 137543,
    SPELL_MIGHTY_LOA                = 136749,
    SPELL_BOOP_ACHIEV               = 139323,
};

enum Events
{
    EVENT_SINISTER_STRIKE = 1,
    EVENT_SUNDER_ARMOR,
    EVENT_DEVASTATE,
    EVENT_THERMAL_STRIKE,
    EVENT_CURSE_OF_STONE,
    EVENT_BODY_SLAM,
    EVENT_CHAIN_LIGHTNING,
    EVENT_LIGHTNING_FIELD,
    EVENT_GASEOUS_ERRUPTION,
    EVENT_VALOROUS_SPIN,
    EVENT_MIGHTY_STOMP,
    EVENT_MIGHTY_CRUSH,
    EVENT_WAVE_OF_MIGHT,
    EVENT_LIGHTNING_CRACK,
    EVENT_REVEBRATING_SMASH,
    EVENT_THUNDER_CLEAVE,
    EVENT_SERRATED_SLASH,
    EVENT_SHIELD_BASH,
    EVENT_AXE_BLOW,
    EVENT_PIERCING_CRY,
    EVENT_SHADOW_NOVA,
    EVENT_SHADOW_BOLT,
    EVENT_ARCANE_BOLT,
    EVENT_ARCANE_EXPLOSION,
    EVENT_MAIN_ATTACK,
    EVENT_ZANDALARI_POTION,
    EVENT_LIGHTNING_BOLT,
    EVENT_SLAM,
    EVENT_THUNDER_CLAP,
    EVENT_RUNE_OF_THE_STORM,
    EVENT_STORM_SHIELD,
    EVENT_PETRIFYING_GAZE,
    EVENT_SHELLWIND,
    EVENT_BITE,
    EVENT_REND,
    EVENT_POUNCE,
    EVENT_THROW_AXE,
    EVENT_HUNTERS_MARK,
    EVENT_BESTIAL_ROAR,
    EVENT_CHARGE_EFF,
    EVENT_BRUTE_SLAM,
    EVENT_CONFIDENCE_OF_KROS,
    EVENT_FLING_FILTH,
    EVENT_BLADE_WING,
    EVENT_SPIRIT_BOND,
    EVENT_FLAMETHOWER,
    EVENT_GROUND_SLAM,
    EVENT_DEATH_STARE,
    EVENT_BLOOD_BOLT,
    EVENT_EXSANGUINATION,
    EVENT_RIVERS_OF_BLOOD,
    EVENT_TOSS_ENERGIZED_METAL,
};

enum Creatures
{
    BOSS_NALAK                  = 69099,
    NPC_LIGHTNING_CRACK         = 70519,
    NPC_WAVE_OF_MIGHT           = 70527,
    NPC_JUVENILE_SKYSCREAMER    = 69162,
    NPC_SHANZE_SOUL_REAPER      = 69236,
    NPC_SHANZE_GRAVEKEEPER      = 69235,
    NPC_SHANZE_THUNDERCALLER    = 69264,
    NPC_ANCIENT_STONE_CONQUEROR = 69238,
    NPC_KULAI                   = 69996,
    NPC_PTERRORWING             = 70141,
    NPC_ELSIA_BLOODHAWK         = 70368,
    NPC_PLAYER_BLOODHAWK        = 70367,
    NPC_PLAYER_HIPPOGRYPH       = 70369,
    NPC_VEREESA_HIPPOGRYPH      = 70366,
    NPC_ADDIT_SHANZE_SOULRIPPER = 69903,
    NPC_GEN_CONTROLLER_BUNNY    = 40789,
    NPC_PUZZLE_BUNNY            = 70311,

    /*Credits*/
    CREDIT_LORTHEMAR_THERON     = 70365,
    CREDIT_LEDY_JAINA_PROUD     = 70364,
    CREDIT_WEAPON_MOGU          = 69913,
    CREDIT_STONE_RITUAL_BUNNY   = 69900,
    CREDIT_MUMMY_CORPSE         = 69331,
};

enum Actions
{
    ACTION_INTRO,
};

enum Quests
{
    QUEST_THE_STORM_GATHERS_A   = 32681,
    QUEST_THE_STORM_GATHERS_H   = 32680,
    QUEST_THUNDER_CALLS_H       = 32678,

    // Misc
    ITEM_LIGHTNING_LANCE        = 95432,
};

enum Yells
{
    TALK_INTRO,
    TALK_SPECIAL_1,
    TALK_SPECIAL_2,
    TALK_SPECIAL_3,
    TALK_SPECIAL_4,
    TALK_SPECIAL_5,
    TALK_SPECIAL_6,
    TALK_SPECIAL_7,
    TALK_SPECIAL_8,
    TALK_SPECIAL_9,
};

const std::vector<uint32> mainAttackList=
{
    SPELL_KICK,
    SPELL_FOCUSED_STRIKE,
    SPELL_SKULL_KICK,
};

const Position isleOfThunderPath[6] =
{
    { 1837.36f, 4271.29f, 184.85f, 0.79f },
    { 1942.40f, 4352.25f, 169.26f, 0.65f },
    { 2177.30f, 4398.99f, 42.18f,  0.02f },
    { 2465.92f, 4392.40f, 12.96f,  6.24f },
    { 2722.56f, 4450.46f, 7.99f,   0.24f },
    { 2962.19f, 4548.56f, 23.07f,  0.20f },
};

const Position isleHordePath[3] =
{
    { 6736.18f, 6626.81f, 42.37f, 5.06f },
    { 6732.51f, 6511.47f, 17.20f, 4.39f },
    { 6707.38f, 6481.60f, 4.71f,  4.02f },
};

const Position isleAlliancePath[3] =
{
    { 5531.69f, 6056.49f, 34.12f, 1.05f },
    { 5692.54f, 6179.92f, 20.25f, 0.73f },
    { 5677.10f, 6215.60f, 13.01f, 1.93f },
};

const Position lorthemarPath[2] =
{
    { 6707.93f, 6478.74f, 4.69f, 4.97f },
    { 6708.29f, 6472.57f, 4.72f, 4.79f },
};

const Position puzzleStartPos = { 7142.58f, 5154.34f, 19.38f, 3.96f };

const std::map<uint32, uint32> invAltarType=
{
    { 217202, 69260 },
    { 217201, 69253 },
    { 217203, 69262 },
};

// Haywire Sunreaver Construct 50358
class npc_sunreaver_construct : public CreatureScript
{
    public:
        npc_sunreaver_construct() : CreatureScript("npc_sunreaver_construct") { }

        struct npc_sunreaver_constructAI : public ScriptedAI
        {
            npc_sunreaver_constructAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiHaywireTimer;

            void Reset() OVERRIDE
            {
                uiHaywireTimer = 3*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiHaywireTimer <= diff)
                {
                    me->CastSpell(me, SPELL_HAYWIRE);
                    uiHaywireTimer = 30*IN_MILLISECONDS;
                }
                else uiHaywireTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_sunreaver_constructAI(creature);
        }
};

// Mumta 69664
class npc_mumta : public CreatureScript
{
    public:
        npc_mumta() : CreatureScript("npc_mumta") { }

        struct npc_mumtaAI : public ScriptedAI
        {
            npc_mumtaAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiSpiritBoltsTimer;
            uint32 uiDarknessDarknessTimer;
            uint32 uiVengefulSpiritTimer;

            void Reset() OVERRIDE
            {
                uiSpiritBoltsTimer = 5*IN_MILLISECONDS;
                uiDarknessDarknessTimer = 10*IN_MILLISECONDS;
                uiVengefulSpiritTimer = 8*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiSpiritBoltsTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_SPIRIT_BOLTS);
                    uiSpiritBoltsTimer = 12*IN_MILLISECONDS;
                }
                else uiSpiritBoltsTimer -= diff;

                if (uiDarknessDarknessTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_DARKNESS_DARKNESS);
                    uiDarknessDarknessTimer = 15*IN_MILLISECONDS;
                }
                else uiDarknessDarknessTimer -= diff;

                if (uiVengefulSpiritTimer <= diff)
                {
                    me->CastSpell(me, SPELL_VENGEFUL_SPIRIT);
                    uiVengefulSpiritTimer = 30*IN_MILLISECONDS;
                }
                else uiVengefulSpiritTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_mumtaAI(creature);
        }
};

// Ku'lai the Skyclaw 69996 - Tamed Pterrorwing 70141
struct npc_thunder_pterodactyls : public ScriptedAI
{
    npc_thunder_pterodactyls(Creature* creature) : ScriptedAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();

        if (me->GetEntry() == NPC_KULAI)
        {
            if (!me->FindNearestCreature(NPC_PTERRORWING, 50.0f))
                me->CastSpell(me, SPELL_SUMMON_PTERRORWING);
            if (Creature* ptrerrowing = me->FindNearestCreature(NPC_PTERRORWING, 50.0f))
            {
                ptrerrowing->GetMotionMaster()->MoveFollow(me, 3.0f, me->GetFollowAngle());
                ptrerrowing->CastSpell(me, SPELL_SPIRIT_BOND);
                ptrerrowing->ClearUnitState(UNIT_STATE_CASTING);

                events.ScheduleEvent(EVENT_SPIRIT_BOND, 15 * IN_MILLISECONDS);
            }
        }
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        me->CastSpell(me->GetVictim(), SPELL_LEAPING_REND);
        events.ScheduleEvent(EVENT_BLADE_WING, 5 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        events.Update(diff);

        if (me->GetEntry() == NPC_PTERRORWING)
        {
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SPIRIT_BOND:
                    {
                        if (Creature* kulai = me->FindNearestCreature(NPC_KULAI, 50.0f))
                        {
                            me->CastSpell(kulai, SPELL_SPIRIT_BOND);
                            me->ClearUnitState(UNIT_STATE_CASTING);
                        }
                        events.ScheduleEvent(EVENT_SPIRIT_BOND, 15 * IN_MILLISECONDS);
                        break;
                    }
                }
            }
        }

        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_BLADE_WING:
                {
                    me->CastSpell(me, SPELL_BLADEWIND);
                    events.ScheduleEvent(EVENT_BLADE_WING, 18 * IN_MILLISECONDS);
                    break;
                }
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap events;
};

// Progenitus 69997
class npc_progenitus : public CreatureScript
{
    public:
        npc_progenitus() : CreatureScript("npc_progenitus") { }

        struct npc_progenitusAI : public ScriptedAI
        {
            npc_progenitusAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiUnleashBloodTimer;
            uint32 uiMutagenicBurstTimer;
            uint32 uiEvolutionTimer;

            void Reset() OVERRIDE
            {
                uiUnleashBloodTimer = 8*IN_MILLISECONDS;
                uiMutagenicBurstTimer = 4*IN_MILLISECONDS;
                uiEvolutionTimer = 16*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiUnleashBloodTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_UNLEASH_BLOOD);
                    uiUnleashBloodTimer = 18*IN_MILLISECONDS;
                }
                else uiUnleashBloodTimer -= diff;

                if (uiMutagenicBurstTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_MUTAGENIC_BURST);
                    uiMutagenicBurstTimer = 8*IN_MILLISECONDS;
                }
                else uiMutagenicBurstTimer -= diff;

                if (uiEvolutionTimer <= diff)
                {
                    me->CastSpell(me, SPELL_EVOLUTION);
                    uiEvolutionTimer = 8*IN_MILLISECONDS;
                }
                else uiEvolutionTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_progenitusAI(creature);
        }
};

// Goda 69998
class npc_goda : public CreatureScript
{
    public:
        npc_goda() : CreatureScript("npc_goda") { }

        struct npc_godaAI : public ScriptedAI
        {
            npc_godaAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiShellSpinTimer;
            uint32 uiCrystallineShieldTimer;

            void Reset() OVERRIDE
            {
                uiShellSpinTimer = 5*IN_MILLISECONDS;
                uiCrystallineShieldTimer = 8*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiShellSpinTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_SHELL_SPIN);
                    uiShellSpinTimer = 10*IN_MILLISECONDS;
                }
                else uiShellSpinTimer -= diff;

                if (uiCrystallineShieldTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_CRYSTALLINE_SHIELD);
                    uiCrystallineShieldTimer = 8*IN_MILLISECONDS;
                }
                else uiCrystallineShieldTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_godaAI(creature);
        }
};

// God-Hulk Ramuk 69999
class npc_godhulk_ramuk : public CreatureScript
{
    public:
        npc_godhulk_ramuk() : CreatureScript("npc_godhulk_ramuk") { }

        struct npc_godhulk_ramukAI : public ScriptedAI
        {
            npc_godhulk_ramukAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiCrouchTimer;
            uint32 uiBlessingOfThunderTimer;
            uint32 uiPoundTimer;

            void Reset() OVERRIDE
            {
                uiCrouchTimer = 20*IN_MILLISECONDS;
                uiBlessingOfThunderTimer = 14*IN_MILLISECONDS;
                uiPoundTimer = 8*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiCrouchTimer <= diff)
                {
                    me->CastSpell(me, SPELL_CROUCH);
                    uiCrouchTimer = 30*IN_MILLISECONDS;
                }
                else uiCrouchTimer -= diff;

                if (uiBlessingOfThunderTimer <= diff)
                {
                    me->CastSpell(me, SPELL_BLESSING_OF_THUNDER);
                    uiBlessingOfThunderTimer = 25*IN_MILLISECONDS;
                }
                else uiBlessingOfThunderTimer -= diff;

                if (uiPoundTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_POUND);
                    uiPoundTimer = 12*IN_MILLISECONDS;
                }
                else uiPoundTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_godhulk_ramukAI(creature);
        }
};

// Al'tabim the All-Seeing 70000
class npc_altabim_the_allseeing : public CreatureScript
{
    public:
        npc_altabim_the_allseeing() : CreatureScript("npc_altabim_the_allseeing") { }

        struct npc_altabim_the_allseeingAI : public ScriptedAI
        {
            npc_altabim_the_allseeingAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiOmenTimer;
            uint32 uiMeteorShowerTimer;
            uint32 uiShadowBoltVolleyTimer;

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->HandleEmoteStateCommand(EMOTE_STATE_NONE);
            }

            void Reset() OVERRIDE
            {
                uiOmenTimer = 10*IN_MILLISECONDS;
                uiMeteorShowerTimer = 14*IN_MILLISECONDS;
                uiShadowBoltVolleyTimer = 6*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiOmenTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_OMEN);
                    uiOmenTimer = 130*IN_MILLISECONDS;
                }
                else uiOmenTimer -= diff;

                if (uiMeteorShowerTimer <= diff)
                {
                    me->CastSpell(me, SPELL_METEOR_SHOWER);
                    uiMeteorShowerTimer = 80*IN_MILLISECONDS;
                }
                else uiMeteorShowerTimer -= diff;

                if (uiShadowBoltVolleyTimer <= diff)
                {
                    me->CastSpell(me, SPELL_SHADOW_BOLT_VOLLEY);
                    uiShadowBoltVolleyTimer = 12*IN_MILLISECONDS;
                }
                else uiShadowBoltVolleyTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_altabim_the_allseeingAI(creature);
        }
};

// Backbreaker Uru 70001
class npc_backbreaker_uru : public CreatureScript
{
    public:
        npc_backbreaker_uru() : CreatureScript("npc_backbreaker_uru") { }

        struct npc_backbreaker_uruAI : public ScriptedAI
        {
            npc_backbreaker_uruAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiBreakSpineTimer;
            uint32 uiMassiveStompTimer;

            void Reset() OVERRIDE
            {
                uiBreakSpineTimer = 6*IN_MILLISECONDS;
                uiMassiveStompTimer = 10*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiBreakSpineTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_BREAK_SPINE);
                    uiBreakSpineTimer = 20*IN_MILLISECONDS;
                }
                else uiBreakSpineTimer -= diff;

                if (uiMassiveStompTimer <= diff)
                {
                    me->CastSpell(me, SPELL_MASSIVE_STOMP);
                    uiMassiveStompTimer = 10*IN_MILLISECONDS;
                }
                else uiMassiveStompTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_backbreaker_uruAI(creature);
        }
};

// Lu-Ban 70002
class npc_luban : public CreatureScript
{
    public:
        npc_luban() : CreatureScript("npc_luban") { }

        struct npc_lubanAI : public ScriptedAI
        {
            npc_lubanAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiSkyforgedHammersTimer;

            void Reset() OVERRIDE
            {
                uiSkyforgedHammersTimer = 5*IN_MILLISECONDS;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->CastSpell(me->GetVictim(), SPELL_STORM_BOLT);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiSkyforgedHammersTimer <= diff)
                {
                    me->CastSpell(me, SPELL_SKYFORGED_HAMMERS);
                    uiSkyforgedHammersTimer = 20*IN_MILLISECONDS;
                }
                else uiSkyforgedHammersTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_lubanAI(creature);
        }
};

// Molthor 70003
class npc_molthor : public CreatureScript
{
    public:
        npc_molthor() : CreatureScript("npc_molthor") { }

        struct npc_molthorAI : public ScriptedAI
        {
            npc_molthorAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiScarabSwarmTimer;
            uint32 uiHorrificVisageTimer;
            uint32 uiThunderCrushTimer;

            void Reset() OVERRIDE
            {
                me->CastSpell(me, SPELL_SCARAB_BROOCH);
                me->CastSpell(me, SPELL_TRIBAL_MASK);
                me->CastSpell(me, SPELL_THUNDERING_SIGIL);

                uiScarabSwarmTimer = 12*IN_MILLISECONDS;
                uiHorrificVisageTimer = 16*IN_MILLISECONDS;
                uiThunderCrushTimer = 8*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiScarabSwarmTimer <= diff)
                {
                    me->CastSpell(me, SPELL_SCARAB_SWARM);
                    uiScarabSwarmTimer = 45*IN_MILLISECONDS;
                }
                else uiScarabSwarmTimer -= diff;

                if (uiHorrificVisageTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_HORRIFIC_VISAGE);
                    uiHorrificVisageTimer = 30*IN_MILLISECONDS;
                }
                else uiHorrificVisageTimer -= diff;

                if (uiThunderCrushTimer <= diff)
                {
                    me->CastSpell(me, SPELL_THUNDER_CRUSH);
                    uiThunderCrushTimer = 15*IN_MILLISECONDS;
                }
                else uiThunderCrushTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_molthorAI(creature);
        }
};

// Ra'sha 70530
class npc_rasha : public CreatureScript
{
    public:
        npc_rasha() : CreatureScript("npc_rasha") { }

        struct npc_rashaAI : public ScriptedAI
        {
            npc_rashaAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiBloodyStrikeTimer;
            uint32 uiBloodboltTimer;
            uint32 uiMarkOfBloodTimer;

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->HandleEmoteStateCommand(EMOTE_STATE_NONE);
            }

            void Reset() OVERRIDE
            {
                uiBloodyStrikeTimer = 4*IN_MILLISECONDS;
                uiBloodboltTimer = 6*IN_MILLISECONDS;
                uiMarkOfBloodTimer = 12*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiBloodyStrikeTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_BLOODY_STRIKE);
                    uiBloodyStrikeTimer = 12*IN_MILLISECONDS;
                }
                else uiBloodyStrikeTimer -= diff;

                if (uiBloodboltTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_BLOODBOLT);
                    uiBloodboltTimer = 18*IN_MILLISECONDS;
                }
                else uiBloodboltTimer -= diff;

                if (uiMarkOfBloodTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_MARK_OF_BLOOD);
                    uiMarkOfBloodTimer = 30*IN_MILLISECONDS;
                }
                else uiMarkOfBloodTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_rashaAI(creature);
        }
};

enum zandalariEvents
{
    EVENT_HORRIFIC_VISAGE = 1,
    EVENT_METEOR_SHOWER,
    EVENT_SCARAB_SWARM,
    EVENT_VENGEFUL_SPIRIT,
    EVENT_THUNDER_CRUSH,
};

const std::map < uint32, std::array<uint32, 4>> invZandalariSpellsType =
{
    { 1, { SPELL_TRIBAL_MASK,      SPELL_HORRIFIC_VISAGE,   EVENT_HORRIFIC_VISAGE, 16000 } },
    { 2, { SPELL_STAR_MAP,         SPELL_METEOR_SHOWER_2,   EVENT_METEOR_SHOWER,   20000 } },
    { 3, { SPELL_SCARAB_BROOCH,    SPELL_SCARAB_SWARM,      EVENT_SCARAB_SWARM,    12000 } },
    { 4, { SPELL_SPIRIT_CHARM,     SPELL_VENGEFUL_SPIRIT_2, EVENT_VENGEFUL_SPIRIT, 25000 } },
    { 5, { SPELL_THUNDERING_SIGIL, SPELL_THUNDER_CRUSH,     EVENT_THUNDER_CRUSH,    8000 } },
};

// War-God Dokah 70096 - Zandalari Warbringer 69769 69841 69842 - Zandalari Warscout 69768
struct npc_thunder_zandalaris : public customCreatureAI
{
    npc_thunder_zandalaris(Creature* creature) : customCreatureAI(creature) 
    { 
        eventsList = { 1, 2, 3, 4, 5 };
        Skyfire::Containers::RandomResizeList(eventsList, 4);
    }

    std::list<uint32> eventsList;

    void Reset() OVERRIDE
    {
        for (auto&& itr : eventsList)
            DoCast(me, invZandalariSpellsType.find(itr)->second[0], true);

        events.Reset();
        summons.DespawnAll();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        for (auto&& itr : eventsList)
        {
            auto key = invZandalariSpellsType.find(itr);
            events.ScheduleEvent(key->second[2], key->second[3]);
        }
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summons.Summon(summon);

        if (Unit* target = me->GetVictim())
            summon->AI()->AttackStart(target);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_HORRIFIC_VISAGE, 30 * IN_MILLISECONDS, EVENT_HORRIFIC_VISAGE, eventId, PRIORITY_NOT_VICTIM);
            ExecuteTargetEvent(SPELL_METEOR_SHOWER_2, 30 * IN_MILLISECONDS, EVENT_METEOR_SHOWER, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_SCARAB_SWARM, 45 * IN_MILLISECONDS, EVENT_SCARAB_SWARM, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_VENGEFUL_SPIRIT_2, 45 * IN_MILLISECONDS, EVENT_VENGEFUL_SPIRIT, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_THUNDER_CRUSH, 15 * IN_MILLISECONDS, EVENT_THUNDER_CRUSH, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Spirit of Warlord Teng 69471
class npc_spirit_of_warlord_teng : public CreatureScript
{
    public:
        npc_spirit_of_warlord_teng() : CreatureScript("npc_spirit_of_warlord_teng") { }

        struct npc_spirit_of_warlord_tengAI : public ScriptedAI
        {
            npc_spirit_of_warlord_tengAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 uiSpiritSlashTimer;
            uint32 uiSpiritSWrathTimer;

            void Reset() OVERRIDE
            {
                if (me->HasAura(SPELL_TENG_GHOST_VISUAL_2))
                    me->RemoveAurasDueToSpell(SPELL_TENG_GHOST_VISUAL_2);

                if (!me->HasAura(SPELL_TENG_GHOST_VISUAL_1))
                me->CastSpell(me, SPELL_TENG_GHOST_VISUAL_1);

                uiSpiritSlashTimer = 6*IN_MILLISECONDS;
                uiSpiritSWrathTimer = 12*IN_MILLISECONDS;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (me->HasAura(SPELL_TENG_GHOST_VISUAL_1))
                    me->RemoveAurasDueToSpell(SPELL_TENG_GHOST_VISUAL_1);

                if (!me->HasAura(SPELL_TENG_GHOST_VISUAL_2))
                    me->CastSpell(me, SPELL_TENG_GHOST_VISUAL_2);
            }

            void JustDied(Unit* killer) OVERRIDE
            {
                killer->CastSpell(killer, 139575);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uiSpiritSlashTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_SPIRIT_SLASH);
                    uiSpiritSlashTimer = 10*IN_MILLISECONDS;
                }
                else uiSpiritSlashTimer -= diff;

                if (uiSpiritSWrathTimer <= diff)
                {
                    me->CastSpell(me->GetVictim(), SPELL_SPIRIT_S_WRATH);
                    uiSpiritSWrathTimer = 18*IN_MILLISECONDS;
                }
                else uiSpiritSWrathTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_spirit_of_warlord_tengAI(creature);
        }
};

// Shan`ze Bloodseeker 69431
struct npc_shanze_bloodseeker : public customCreatureAI
{
    npc_shanze_bloodseeker(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SINISTER_STRIKE, 4500);
        events.ScheduleEvent(EVENT_DEVASTATE, urand(8500, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_SUNDER_ARMOR, 6 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_SINISTER_STRIKE, urand(8500, 12 * IN_MILLISECONDS), EVENT_SINISTER_STRIKE, eventId);
            ExecuteTargetEvent(SPELL_SUNDER_ARMOR, 12 * IN_MILLISECONDS, EVENT_SUNDER_ARMOR, eventId);
            ExecuteTargetEvent(SPELL_DEVASTATE, 10 * IN_MILLISECONDS, EVENT_DEVASTATE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Horgak the Enslaver 70176
struct npc_horgak_the_enslaver : public customCreatureAI
{
    npc_horgak_the_enslaver(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
        me->m_Events.KillEventsByGroup(SPELL_THERMODINAMYCS);
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_THERMAL_STRIKE, 7500);

        me->m_Events.Schedule(5000, SPELL_THERMODINAMYCS, [this]()
        {
            DoCast(me, SPELL_THERMODINAMYCS);
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_THERMAL_STRIKE, urand(8500, 12 * IN_MILLISECONDS), EVENT_THERMAL_STRIKE, eventId, PRIORITY_NOT_VICTIM);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Animated Warrior 67473
struct npc_animated_warrior : public customCreatureAI
{
    npc_animated_warrior(Creature* creature) : customCreatureAI(creature)
    {
        ResetGuard();
    }

    void Reset() OVERRIDE
    {
        events.Reset();

        if (roll_chance_i(20)) // temp solution
            ActivateGuard(nullptr);
    }

    void JustAppeared() OVERRIDE
    {
        ResetGuard();
    }

    void ActivateGuard(Unit* invoker)
    {
        me->RemoveAurasDueToSpell(SPELL_STATUE_FROZEN_SHEATH);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NOT_SELECTABLE);
        me->GetMotionMaster()->MoveRandom(10.0f);

        if (invoker)
            me->Attack(invoker, true);
    }

    void ResetGuard()
    {
        DoCast(me, SPELL_STATUE_FROZEN_SHEATH);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NOT_SELECTABLE);
        me->GetMotionMaster()->MoveIdle();
    }

    void MoveInLineOfSight(Unit* who) OVERRIDE
    {
        if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC))
            return;
        if (who->GetTypeId() != TypeID::TYPEID_PLAYER)
            return;
        if (!me->IsWithinDistInMap(who, 5.0f, false))
            return;

        if (roll_chance_i(20)) // temp solution
            ActivateGuard(who);
    }

    void JustReachedHome() OVERRIDE
    {
        if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC))
            me->GetMotionMaster()->MoveRandom(10.0f);
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_CURSE_OF_STONE, 2500);
        events.ScheduleEvent(EVENT_BODY_SLAM, 8 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_CURSE_OF_STONE, urand(10500, 12 * IN_MILLISECONDS), EVENT_CURSE_OF_STONE, eventId);
            ExecuteTargetEvent(urand(0, 1) ? SPELL_BODY_SLAM_25Y : SPELL_BODY_SLAM_5Y, 6 * IN_MILLISECONDS, EVENT_BODY_SLAM, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Lightning Rod 69327
struct npc_lightning_rod : public ScriptedAI
{
    npc_lightning_rod(Creature* creature) : ScriptedAI(creature) 
    {
        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    }

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        clicker->RemoveAurasDueToSpell(SPELL_ELECTRIFIED);
        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

        me->m_Events.Schedule(10000, 1, [this]()
        {
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE { }
};

// Electromancer Ju`le 69339
struct npc_electromancer_jule : public customCreatureAI
{
    npc_electromancer_jule(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 3500);
        events.ScheduleEvent(EVENT_LIGHTNING_FIELD, 9 * IN_MILLISECONDS);
    }

    void JustDied(Unit* killer) OVERRIDE
    {
        killer->CastSpell(killer, 139576);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_CHAIN_LIGHTNING, urand(10 * IN_MILLISECONDS, 18 * IN_MILLISECONDS), EVENT_CHAIN_LIGHTNING, eventId);
            ExecuteTargetEvent(SPELL_LIGHTNING_FIELD, 15 * IN_MILLISECONDS, EVENT_LIGHTNING_FIELD, eventId, PRIORITY_NOT_VICTIM);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Creeping Moor Beast 67772
struct npc_creeping_moor_beast : public customCreatureAI
{
    npc_creeping_moor_beast(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_GASEOUS_ERRUPTION, 5 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_GASEOUS_ERRUPTION, urand(15 * IN_MILLISECONDS, 30 * IN_MILLISECONDS), EVENT_GASEOUS_ERRUPTION, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Slateshell Wanderer 67703
struct npc_slateshell_wanderer : public customCreatureAI
{
    npc_slateshell_wanderer(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_VALOROUS_SPIN, 3 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_VALOROUS_SPIN, urand(11 * IN_MILLISECONDS, 17500), EVENT_VALOROUS_SPIN, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Drakkari God-Hulk 69200
struct npc_drakkari_god_hulk : public customCreatureAI
{
    npc_drakkari_god_hulk(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_MIGHTY_STOMP, 5500);
        events.ScheduleEvent(EVENT_MIGHTY_CRUSH, 10500);
    }

    void JustDied(Unit* killer) OVERRIDE
    {
        killer->CastSpell(killer, SPELL_MIGHTY_LOA);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_MIGHTY_STOMP, urand(14 * IN_MILLISECONDS, 16 * IN_MILLISECONDS), EVENT_MIGHTY_STOMP, eventId);
            ExecuteTargetEvent(SPELL_MIGHTY_CRUSH, 12500, EVENT_MIGHTY_CRUSH, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Colossus 69405
struct npc_zandalari_colossus : public customCreatureAI
{
    npc_zandalari_colossus(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
        summons.DespawnAll();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_REVEBRATING_SMASH, 12 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_WAVE_OF_MIGHT, 4500);
        events.ScheduleEvent(EVENT_LIGHTNING_CRACK, 15 * IN_MILLISECONDS);
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summons.Summon(summon);

        switch (summon->GetEntry())
        {
            case NPC_LIGHTNING_CRACK:
                summon->CastSpell(summon, SPELL_LIGHTNING_CRACK_VISUAL, true);
                break;
            case NPC_WAVE_OF_MIGHT:
                summon->CastSpell(summon, SPELL_WAVE_OF_MIGHT_AT, true);
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
        {
            ExecuteTargetEvent(SPELL_REVERBERATING_SMASH, urand(15 * IN_MILLISECONDS, 26500), EVENT_REVEBRATING_SMASH, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_WAVE_OF_MIGHT, 14500, EVENT_WAVE_OF_MIGHT, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_LIGHTNING_CRACK, 9 * IN_MILLISECONDS, EVENT_LIGHTNING_CRACK, eventId, PRIORITY_NOT_VICTIM);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Wave of Might 70527
struct npc_wave_of_might : public ScriptedAI
{
    npc_wave_of_might(Creature* creature) : ScriptedAI(creature) { }

    float x, y;
    TaskScheduler scheduler;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->SetDisplayId(11686);

        x = 0.0f; y = 0.0f;
        GetPositionWithDistInOrientation(me, 20.0f, me->GetOrientation(), x, y);

        Movement::MoveSplineInit init(me);
        init.MoveTo(x, y, me->GetPositionZ());
        init.Launch();
        me->DespawnOrUnsummon(me->GetSplineDuration());

        scheduler
            .Schedule(Milliseconds(500), [this](TaskContext context)
        {
            // No any way - this should work like movement areaTrigger
            std::list<Player*> pList;
            GetPlayerListInGrid(pList, me, 1.5f);

            for (auto&& itr : pList)
                if (itr->IsAlive())
                    me->CastSpell(itr, SPELL_WAVE_OF_MIGHT_EFF, true);

            context.Repeat(Milliseconds(500));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Zandalari Stoneshield 69223
struct npc_zandalari_stoneshield : public customCreatureAI
{
    npc_zandalari_stoneshield(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_THUNDER_CLEAVE, 2500);
        events.ScheduleEvent(EVENT_SERRATED_SLASH, 4500);
        events.ScheduleEvent(EVENT_SHIELD_BASH, 7 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_THUNDERCLEAVE, urand(6 * IN_MILLISECONDS, 12 * IN_MILLISECONDS), EVENT_THUNDER_CLEAVE, eventId);
            ExecuteTargetEvent(SPELL_SERRATED_SLASH, 7500, EVENT_SERRATED_SLASH, eventId);
            ExecuteTargetEvent(SPELL_SHIELD_BASH, 10 * IN_MILLISECONDS, EVENT_SHIELD_BASH, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Beastcaller 69065
struct npc_zandalari_beastcaller : public customCreatureAI
{
    npc_zandalari_beastcaller(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        DoCast(me, SPELL_ALERT_DRUMBS);
        events.ScheduleEvent(EVENT_AXE_BLOW, 4500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_AXE_BLOW, 8 * IN_MILLISECONDS, EVENT_AXE_BLOW, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Juvenile Skyscreamer 69162, 69411, 69156
struct npc_juvenile_skyscreamer : public customCreatureAI
{
    npc_juvenile_skyscreamer(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_PIERCING_CRY, 3 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(me->GetEntry() == NPC_JUVENILE_SKYSCREAMER ? SPELL_PIERCING_CRY : SPELL_PIERCING_CRY_2, 9500, EVENT_PIERCING_CRY, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Spiritbinder 69225
struct npc_zandalari_spiritbinder : public customCreatureAI
{
    npc_zandalari_spiritbinder(Creature* creature) : customCreatureAI(creature) 
    {
        isCaster = true;
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SHADOW_BOLT, 1500);
        events.ScheduleEvent(EVENT_SHADOW_NOVA, 8 * IN_MILLISECONDS);
    }

    void AttackStart(Unit* target) OVERRIDE
    {
        if (!target)
            return;

        if (me->Attack(target, false))
            DoStartNoMovement(target);
    }

    void CastInterrupted(SpellInfo const* spell) OVERRIDE
    {
        events.RescheduleEvent(EVENT_SHADOW_BOLT, 6 * IN_MILLISECONDS);

        if (Unit* vict = me->GetVictim())
            me->GetMotionMaster()->MoveChase(vict);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_SHADOW_BOLT, 3500, EVENT_SHADOW_BOLT, eventId);
            ExecuteTargetEvent(SPELL_SHADOW_NOVA, 11500, EVENT_SHADOW_NOVA, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Arcweaver 69224
struct npc_zandalari_arcweaver : public customCreatureAI
{
    npc_zandalari_arcweaver(Creature* creature) : customCreatureAI(creature)
    {
        isCaster = true;
        questCreatures = me->GetDBTableGUIDLow() == 535599 || me->GetDBTableGUIDLow() == 535757;
    }

    bool questCreatures;

    void Reset() OVERRIDE
    {
        events.Reset();

        if (questCreatures)
        {
            Creature* screamer = me->FindNearestCreature(69263, 30.0f);
            if (!screamer)
            {
                if (screamer = me->SummonCreature(69263, 6218.99f, 5948.78f, 50.572f, 0.451248f))
                    DoCast(screamer, 136605);
            }
            else
                DoCast(screamer, 136605);
        }
    }

    void JustReachedHome() OVERRIDE
    {
        if (questCreatures)
        {
            Creature* screamer = me->FindNearestCreature(69263, 30.0f);
            if (!screamer)
            {
                if (screamer = me->SummonCreature(69263, 6218.99f, 5948.78f, 50.572f, 0.451248f))
                    DoCast(screamer, 136605);
            }
            else
                DoCast(screamer, 136605);
        }
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_ARCANE_BOLT, 1500);
        events.ScheduleEvent(EVENT_ARCANE_EXPLOSION, 8 * IN_MILLISECONDS);

        if (questCreatures)
            me->InterruptNonMeleeSpells(true);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (questCreatures)
        {
            if (Creature* screamer = me->FindNearestCreature(69263, 30.0f))
            {
                if (!screamer->HasAura(136605))
                {
                    std::list<Player*> playersForQuest;
                    GetPlayerListInGrid(playersForQuest, me, 30.0f);

                    for (auto&& player : playersForQuest)
                        player->KilledMonsterCredit(screamer->GetEntry());

                    screamer->GetMotionMaster()->MovePoint(0, 6189.103f, 5945.929f, 67.885f);
                    screamer->DespawnOrUnsummon(5000);
                }
            }
        }
    }

    void AttackStart(Unit* target) OVERRIDE
    {
        if (!target)
            return;

        if (me->Attack(target, false))
            DoStartNoMovement(target);
    }

    void CastInterrupted(SpellInfo const* spell) OVERRIDE
    {
        events.RescheduleEvent(EVENT_ARCANE_BOLT, 6 * IN_MILLISECONDS);

        if (Unit* vict = me->GetVictim())
            me->GetMotionMaster()->MoveChase(vict);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_ARCANE_BOLT, 1500, EVENT_ARCANE_BOLT, eventId);
            ExecuteTargetEvent(SPELL_ARCANE_EXPLOSION, 11500, EVENT_ARCANE_EXPLOSION, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Jaguar Warrior 69171
struct npc_zandalari_jaguar_warrior : public customCreatureAI
{
    npc_zandalari_jaguar_warrior(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_MAIN_ATTACK, 3 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_ZANDALARI_POTION, 6500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(Skyfire::Containers::SelectRandomContainerElement(mainAttackList), 4500, EVENT_MAIN_ATTACK, eventId);
            ExecuteTargetEvent(SPELL_ZANDALARI_POTION, 15 * IN_MILLISECONDS, EVENT_ZANDALARI_POTION, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Shan`ze Thundercaller 69264
struct npc_shanze_thundercaller : public customCreatureAI
{
    npc_shanze_thundercaller(Creature* creature) : customCreatureAI(creature)
    {
        isCaster    = true;
        questCreatures = me->GetDBTableGUIDLow() == 566259 || me->GetDBTableGUIDLow() == 566280 ||
            me->GetDBTableGUIDLow() == 565999 || me->GetDBTableGUIDLow() == 565963;
    }

    bool hasTriggered;
    bool questCreatures;

    void Reset() OVERRIDE
    {
        hasTriggered = false;
        events.Reset();

        if (questCreatures)
        {
            Creature* spear = me->FindNearestCreature(69367, 30.0f);
            if (!spear)
            {
                if (spear = me->SummonCreature(69367, 6604.359f, 6291.149f, 5.067f, 3.553f))
                    DoCast(spear, 136789);
            }
            else
                DoCast(spear, 136789);
        }
    }

    void JustReachedHome() OVERRIDE
    {
        if (questCreatures)
        {
            Creature* spear = me->FindNearestCreature(69367, 30.0f);
            if (!spear)
            {
                if (spear = me->SummonCreature(69367, 6604.359f, 6291.149f, 5.067f, 3.553f))
                    DoCast(spear, 136789);
            }
            else
                DoCast(spear, 136789);
        }
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 1500);
        events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 8 * IN_MILLISECONDS);

        if (questCreatures)
            me->InterruptNonMeleeSpells(true);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (questCreatures)
        {
            if (Creature* spear = me->FindNearestCreature(69367, 30.0f))
            {
                if (!spear->HasAura(136789))
                {
                    std::list<Player*> playersForQuest;
                    GetPlayerListInGrid(playersForQuest, me, 40.0f);

                    for (auto&& player : playersForQuest)
                        player->KilledMonsterCredit(69369);
                }
            }
        }
    }

    void AttackStart(Unit* target) OVERRIDE
    {
        if (!target)
            return;

        if (me->Attack(target, false))
            DoStartNoMovement(target);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(65) && !hasTriggered && !me->HasUnitState(UNIT_STATE_CASTING))
        {
            hasTriggered = true;
            DoCast(me, SPELL_RUNE_OF_THE_STORM);
        }
    }

    void CastInterrupted(SpellInfo const* spell) OVERRIDE
    {
        events.RescheduleEvent(EVENT_LIGHTNING_BOLT, 6 * IN_MILLISECONDS);

        if (Unit* vict = me->GetVictim())
            me->GetMotionMaster()->MoveChase(vict);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_LIGHTNING_BOLT_2, 3500, EVENT_LIGHTNING_BOLT, eventId);
            ExecuteTargetEvent(SPELL_CHAIN_LIGHTNING_2, 11500, EVENT_CHAIN_LIGHTNING, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Shan`ze Gravekeeper 69235
struct npc_shanze_gravekeeper : public customCreatureAI
{
    npc_shanze_gravekeeper(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SUNDER_ARMOR, 6 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_SLAM, 2500);
        events.ScheduleEvent(EVENT_THUNDER_CLAP, 8 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_RUNE_OF_THE_STORM, urand(12 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_SUNDER_ARMOR, urand(12 * IN_MILLISECONDS, 16 * IN_MILLISECONDS), EVENT_SUNDER_ARMOR, eventId);
            ExecuteTargetEvent(SPELL_SLAM, 9500, EVENT_SLAM, eventId);
            ExecuteTargetEvent(SPELL_THUNDER_CLAP, 10 * IN_MILLISECONDS, EVENT_THUNDER_CLAP, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_RUNE_OF_THE_STORM, 25 * IN_MILLISECONDS, EVENT_RUNE_OF_THE_STORM, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Sparkmancer Vu 69961
struct npc_sparkmancer_vu : public customCreatureAI
{
    npc_sparkmancer_vu(Creature* creature) : customCreatureAI(creature)
    {
        isCaster = true;
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 1500);
    }

    void AttackStart(Unit* target) OVERRIDE
    {
        if (!target)
            return;

        if (me->Attack(target, false))
            DoStartNoMovement(target);
    }

    void CastInterrupted(SpellInfo const* spell) OVERRIDE
    {
        events.RescheduleEvent(EVENT_CHAIN_LIGHTNING, 6 * IN_MILLISECONDS);

        if (Unit* vict = me->GetVictim())
            me->GetMotionMaster()->MoveChase(vict);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_CHAIN_LIGHTNING_3, 3500, EVENT_CHAIN_LIGHTNING, eventId, PRIORITY_NOT_VICTIM);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Shan`ze Soulripper 69236
struct npc_shanze_soulripper : public customCreatureAI
{
    npc_shanze_soulripper(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 6 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_STORM_SHIELD, 8500);
    }

    uint64 GetLowestFriendGUID() OVERRIDE
    {
        std::list<Creature*> tmpTargets;

        GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SHANZE_GRAVEKEEPER, 80.0f);
        GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SHANZE_SOUL_REAPER, 80.0f);
        GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SHANZE_THUNDERCALLER, 80.0f);
        GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_ANCIENT_STONE_CONQUEROR, 80.0f);

        if (tmpTargets.empty())
            return 0;

        tmpTargets.sort(Skyfire::HealthPctOrderPred());

        if (Creature* lowestTarget = tmpTargets.front())
            return lowestTarget->GetGUID();

        return 0;
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_LIGHTNING_BOLT_2, urand(8 * IN_MILLISECONDS, 16 * IN_MILLISECONDS), EVENT_LIGHTNING_BOLT, eventId);
            ExecuteTargetEvent(SPELL_STORM_SHIELD, 12500, EVENT_STORM_SHIELD, eventId, PRIORITY_ALLY);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Ancient Stone Conqueror 69238
struct npc_ancient_stone_conqueror : public customCreatureAI
{
    npc_ancient_stone_conqueror(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_PETRIFYING_GAZE, 7 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_PETRIFYING_GAZE, urand(10 * IN_MILLISECONDS, 17500), EVENT_PETRIFYING_GAZE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Slateshell Ancient 67763
struct npc_slateshell_ancient : public customCreatureAI
{
    npc_slateshell_ancient(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_BITE, 3 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_SHELLWIND, 8500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_BITE, urand(6500, 11 * IN_MILLISECONDS), EVENT_BITE, eventId);
            ExecuteTargetEvent(SPELL_SHELLWIND, urand(12500, 17500), EVENT_SHELLWIND, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Tamed Bladetalon 71297
struct npc_tamed_bladetalon : public customCreatureAI
{
    npc_tamed_bladetalon(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_REND, 2500);
        events.ScheduleEvent(EVENT_POUNCE, 8500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_REND, urand(10 * IN_MILLISECONDS, 13 * IN_MILLISECONDS), EVENT_REND, eventId);
            ExecuteTargetEvent(SPELL_POUNCE_JUMP, urand(14 * IN_MILLISECONDS, 22 * IN_MILLISECONDS), EVENT_POUNCE, eventId, PRIORITY_NOT_VICTIM);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Beastlord 71298
struct npc_zandalari_beastlord : public customCreatureAI
{
    npc_zandalari_beastlord(Creature* creature) : customCreatureAI(creature)
    {
        isCaster = true;
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_THROW_AXE, 1500);
        events.ScheduleEvent(EVENT_HUNTERS_MARK, 5500);
    }

    void AttackStart(Unit* target) OVERRIDE
    {
        if (!target)
            return;

        if (me->Attack(target, false))
            DoStartNoMovement(target);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_THROW_AXE, 3 * IN_MILLISECONDS, EVENT_THROW_AXE, eventId);
            ExecuteTargetEvent(SPELL_HUNTERS_MARK, urand(12 * IN_MILLISECONDS, 20 * IN_MILLISECONDS), EVENT_HUNTERS_MARK, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Skumblade Skirmisher 70348, Skumblade Scavenger 69227
struct npc_skumblade_skirmisher : public customCreatureAI
{
    npc_skumblade_skirmisher(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_CHARGE_EFF, 500);
        events.ScheduleEvent(EVENT_BESTIAL_ROAR, 6500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_CHARGE_EFF, urand(12 * IN_MILLISECONDS, 17 * IN_MILLISECONDS), EVENT_CHARGE_EFF, eventId, PRIORITY_NOT_VICTIM);
            ExecuteTargetEvent(SPELL_BESTIAL_ROAR, urand(12 * IN_MILLISECONDS, 22 * IN_MILLISECONDS), EVENT_BESTIAL_ROAR, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Skumblade Brute 69338
struct npc_skumblade_brute : public customCreatureAI
{
    npc_skumblade_brute(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_BRUTE_SLAM, 6 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_CONFIDENCE_OF_KROS, 12500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_BRUTE_SLAM, urand(9 * IN_MILLISECONDS, 15500), EVENT_BRUTE_SLAM, eventId, PRIORITY_CHANNELED);
            ExecuteTargetEvent(SPELL_CONFIDENCE_OF_KROS, urand(22 * IN_MILLISECONDS, 29 * IN_MILLISECONDS), EVENT_CONFIDENCE_OF_KROS, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Skumblade Flithmonger 69228
struct npc_skumblade_flithmonger : public customCreatureAI
{
    npc_skumblade_flithmonger(Creature* creature) : customCreatureAI(creature)
    {
        isCaster = true;
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_FLING_FILTH, 1500);
    }

    void AttackStart(Unit* target) OVERRIDE
    {
        if (!target)
            return;

        if (me->Attack(target, false))
            DoStartNoMovement(target);
    }

    void CastInterrupted(SpellInfo const* spell) OVERRIDE
    {
        events.RescheduleEvent(EVENT_FLING_FILTH, 6 * IN_MILLISECONDS);

        if (Unit* vict = me->GetVictim())
            me->GetMotionMaster()->MoveChase(vict);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_FLING_FILTH, 2500, EVENT_FLING_FILTH, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Skeletal Lasher 70534
struct npc_skeletal_lasher : public customCreatureAI
{
    npc_skeletal_lasher(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_REND, 2500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_REND_2, urand(10 * IN_MILLISECONDS, 13 * IN_MILLISECONDS), EVENT_REND, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Scout Captain Elsia 70358
class npc_scout_captain_elsia : public CreatureScript
{
    public:
        npc_scout_captain_elsia() : CreatureScript("npc_scout_captain_elsia") { }

        bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*item*/) OVERRIDE
        {
            if (quest->GetQuestId() == QUEST_THUNDER_CALLS_H)
                creature->AI()->Talk(TALK_INTRO, player);

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(QUEST_THE_STORM_GATHERS_H) != QUEST_STATUS_INCOMPLETE)
                return false;

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I`m ready to go", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                player->CastSpell(player, SPELL_SUMMON_PLAYER_MOUNT_H, true);

            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        struct npc_scout_captain_elsiaAI : public ScriptedAI
        {
            npc_scout_captain_elsiaAI(Creature* creature) : ScriptedAI(creature) 
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC);
            }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->m_Events.Schedule(2650, [this]()
                {
                    Talk(TALK_INTRO);
                });
                me->m_Events.Schedule(12100, [this]()
                {
                    Talk(TALK_SPECIAL_1);
                });
                me->m_Events.Schedule(22100, [this]()
                {
                    Talk(TALK_SPECIAL_2);
                });
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scout_captain_elsiaAI(creature);
        }
};

// Sunreaver Bloodhawk 70367, 70368
struct npc_sunreaver_bloodhawk : public ScriptedAI
{
    npc_sunreaver_bloodhawk(Creature* creature) : ScriptedAI(creature) 
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC);
    }

    TaskScheduler scheduler;
    uint64 summonerGUID;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        summonerGUID = summoner->GetGUID();
        me->SetSpeed(MOVE_RUN, 2.85f);

        if (me->GetMap()->GetZoneId(me->GetPhaseMask(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) == 6507)
            return;

        if (me->GetEntry() == NPC_ELSIA_BLOODHAWK)
            DoCast(me, SPELL_SUMMON_ELSIA_TOWNLONG, true);

        scheduler.Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            if (Unit* owner = ObjectAccessor::GetUnit(*me, summonerGUID))
                owner->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);

            Movement::MoveSplineInit init(me);
            for (auto itr : isleOfThunderPath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.SetFly();
            init.SetUncompressed();
            init.Launch();

            scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
            {
                if (Unit* owner = ObjectAccessor::GetUnit(*me, summonerGUID))
                {
                    if (owner->ToPlayer())
                    {
                        owner->ExitVehicle();
                        owner->CastSpell(owner, SPELL_TELEPORT_THUNDER_ISLE_H, true);
                        me->DespawnOrUnsummon();
                    }
                    else if (owner->ToCreature())
                    {
                        owner->ExitVehicle();
                        owner->ToCreature()->DespawnOrUnsummon();
                        me->DespawnOrUnsummon();
                    }
                }
            });
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_INTRO)
        {
            scheduler.Schedule(Milliseconds(1500), [this](TaskContext context) // delay for init or vehicle entrance will drop...
            {
                Movement::MoveSplineInit init(me);
                for (auto itr : isleHordePath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetFly();
                init.SetUncompressed();
                init.Launch();

                scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                {
                    if (Unit* owner = ObjectAccessor::GetUnit(*me, summonerGUID))
                    {
                        owner->ExitVehicle();
                        me->DespawnOrUnsummon();
                    }
                });
            });
        }
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summonerGUID = summon->GetGUID();
    }

    void UpdateAI(uint32 diff) OVERRIDE 
    {
        scheduler.Update(diff);
    }
};

// Vereesa Windrunner 70360, 70372
class npc_vereesa_windrunner : public CreatureScript
{
    public:
        npc_vereesa_windrunner() : CreatureScript("npc_vereesa_windrunner") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(QUEST_THE_STORM_GATHERS_A) != QUEST_STATUS_INCOMPLETE)
                return false;

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I`m ready to go", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                player->CastSpell(player, SPELL_SUMMON_PLAYER_MOUNT_A, true);

            player->CLOSE_GOSSIP_MENU();

            return true;
        }

        struct npc_vereesa_windrunnerAI : public ScriptedAI
        {
            npc_vereesa_windrunnerAI(Creature* creature) : ScriptedAI(creature) 
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC);
            }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->m_Events.Schedule(2650, [this]()
                {
                    Talk(TALK_INTRO);
                });

                me->m_Events.Schedule(12100, [this]()
                {
                    Talk(TALK_SPECIAL_1);
                });

                me->m_Events.Schedule(22100, [this]()
                {
                    Talk(TALK_SPECIAL_2);
                });
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_vereesa_windrunnerAI(creature);
        }
};

// Silver Covenant Hippogryph 70369, 70366
struct npc_silver_covenant_hippogryph : public ScriptedAI
{
    npc_silver_covenant_hippogryph(Creature* creature) : ScriptedAI(creature) 
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC);
    }

    TaskScheduler scheduler;
    uint64 summonerGUID;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        summonerGUID = summoner->GetGUID();
        me->SetSpeed(MOVE_RUN, 2.85f);

        if (me->GetMap()->GetZoneId(me->GetPhaseMask(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) == 6507)
            return;

        if (me->GetEntry() == NPC_VEREESA_HIPPOGRYPH)
            DoCast(me, SPELL_SUMMON_VEREESA_TOWNLONG, true);

        scheduler.Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            if (Unit* owner = ObjectAccessor::GetUnit(*me, summonerGUID))
                owner->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);

            Movement::MoveSplineInit init(me);
            for (auto itr : isleOfThunderPath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.SetFly();
            init.SetUncompressed();
            init.Launch();

            scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
            {
                if (Unit* owner = ObjectAccessor::GetUnit(*me, summonerGUID))
                {
                    if (owner->ToPlayer())
                    {
                        owner->ExitVehicle();
                        owner->CastSpell(owner, SPELL_TELEPORT_THUNDER_ISLE_A, true);
                        me->DespawnOrUnsummon();
                    }
                    else if (owner->ToCreature())
                    {
                        owner->ExitVehicle();
                        owner->ToCreature()->DespawnOrUnsummon();
                        me->DespawnOrUnsummon();
                    }
                }
            });
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_INTRO)
        {
            scheduler.Schedule(Milliseconds(1500), [this](TaskContext context) // delay for init or vehicle entrance will drop...
            {
                Movement::MoveSplineInit init(me);
                for (auto itr : isleAlliancePath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetFly();
                init.SetUncompressed();
                init.Launch();

                scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                {
                    if (Unit* owner = ObjectAccessor::GetUnit(*me, summonerGUID))
                    {
                        owner->ExitVehicle();
                        me->DespawnOrUnsummon();
                    }
                });
            });
        }
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summonerGUID = summon->GetGUID();
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Lor`themar Theron Isle Intro 70371
struct npc_lorthemar_theron_isle_intro : public ScriptedAI
{
    npc_lorthemar_theron_isle_intro(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 summonerGUID;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        summonerGUID = summoner->GetGUID();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        me->SetPhaseMask(4, true);

        scheduler.Schedule(Seconds(2), [this](TaskContext context)
        {
            Talk(TALK_INTRO);
            me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() + M_PI / 2));
        });

        scheduler.Schedule(Milliseconds(8250), [this](TaskContext context)
        {
            me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() - M_PI / 2));
            Talk(TALK_SPECIAL_1);
        });

        scheduler.Schedule(Milliseconds(15250), [this](TaskContext context)
        {
            Movement::MoveSplineInit init(me);
            for (auto itr : lorthemarPath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.SetUncompressed();
            init.Launch();

            uint32 delay = me->GetSplineDuration();
            scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_2);
            });
            scheduler.Schedule(Milliseconds(delay += 6000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_3);
            });
            scheduler.Schedule(Milliseconds(delay += 10000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_4);

                if (Unit* owner = ObjectAccessor::GetUnit(*me, summonerGUID))
                    me->SetFacingTo(me->GetAngle(owner));
            });
            scheduler.Schedule(Milliseconds(delay += 10000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_5);
            });
            scheduler.Schedule(Milliseconds(delay += 4500), [this](TaskContext context)
            {
                me->SetFacingTo(4.88f);
                Talk(TALK_SPECIAL_6);
            });
            scheduler.Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
            {
                if (Unit* owner = ObjectAccessor::GetUnit(*me, summonerGUID))
                    me->SetFacingTo(me->GetAngle(owner));

                Talk(TALK_SPECIAL_7);
            });
            scheduler.Schedule(Milliseconds(delay += 7800), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_8);
            });
            scheduler.Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_9);
                me->GetMotionMaster()->MoveTargetedHome();

                scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                {
                    if (Unit* owner = ObjectAccessor::GetUnit(*me, summonerGUID))
                        owner->RemoveAurasDueToSpell(SPELL_SUMMON_LORTHEMAR);

                    me->DespawnOrUnsummon();
                });
            });
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE 
    {
        scheduler.Update(diff);
    }
};

// Aethas isle intro 70389
struct npc_aethas_isle_intro : public ScriptedAI
{
    npc_aethas_isle_intro(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        me->SetPhaseMask(4, true);

        scheduler.Schedule(Seconds(2), [this](TaskContext context)
        {
            me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() - M_PI / 2));
            me->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
        });

        scheduler.Schedule(Seconds(2), [this](TaskContext context)
        {
            me->DespawnOrUnsummon();
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Stone Ritual Bunny Controller 69900
struct npc_stone_ritual_bunny_controller : public ScriptedAI
{
    npc_stone_ritual_bunny_controller(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 conquerorGUID;

    void Reset() OVERRIDE
    {
        scheduler.CancelAll();
        conquerorGUID = 0;

        if (Creature* stoneConqueror = me->SummonCreature(NPC_ANCIENT_STONE_CONQUEROR, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
            conquerorGUID = stoneConqueror->GetGUID();

        scheduler.Schedule(Seconds(2), [this](TaskContext context) // Init delay
        {
            std::list<Creature*> shanzeList;
            GetCreatureListWithEntryInGrid(shanzeList, me, NPC_ADDIT_SHANZE_SOULRIPPER, 55.0f);

            if (Creature* conqueror = ObjectAccessor::GetCreature(*me, conquerorGUID))
                for (auto&& itr : shanzeList)
                    itr->CastSpell(conqueror, SPELL_VISUAL_RITUAL, true);
        });

        scheduler.Schedule(Seconds(1), [this](TaskContext context) // Init delay
        {
            std::list<Creature*> shanzeList;
            GetCreatureListWithEntryInGrid(shanzeList, me, NPC_ADDIT_SHANZE_SOULRIPPER, 55.0f);
            shanzeList.remove_if([=](Creature* target) { return target && !target->IsAlive(); });

            if (shanzeList.empty()) // if ritual not done
            {
                me->DisappearAndDie();

                if (Creature* conqueror = ObjectAccessor::GetCreature(*me, conquerorGUID))
                    conqueror->DisappearAndDie();

                // Reward Nearby Players
                std::list<Player*> pList;
                GetPlayerListInGrid(pList, me, 50.0f);

                for (auto&& itr : pList)
                    itr->KilledMonsterCredit(CREDIT_STONE_RITUAL_BUNNY);

                return;
            }

            context.Repeat(Seconds(1));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

enum GuardianTaku
{
    EVENT_ACTIVATE         = 1,
    EVENT_CHECK_BUFF       = 2,
    EVENT_FIRE_AURA        = 3,
    EVENT_EARTH_AURA       = 4,
    EVENT_WATER_AURA       = 5,
    EVENT_AIR_AURA         = 6,
    EVENT_LIGHTNING        = 7,

    SPELL_FIRE_AURA        = 137304,
    SPELL_EARTH_AURA       = 137305,
    SPELL_WATER_AURA       = 137306,
    SPELL_AIR_AURA         = 137307,

    NPC_SERPENT            = 69567,
    NPC_HYDRA              = 69568,
    NPC_DIREHORN           = 69569,
    NPC_HAWK               = 69570,
    NPC_HAWK_TRIGGER       = 69636,

    SPELL_HYDRA_BLESS      = 137292,
    SPELL_SERPENT_BLESS    = 137290,
    SPELL_DIREHORN_BLESS   = 137291,
    SPELL_HAWK_BLESS       = 137293,

    SPELL_STONE_SKIN       = 120162,

    SPELL_HYDRA_ABILITY    = 137465,
    SPELL_SERPENT_ABILITY  = 137463,
    SPELL_DIREHORN_ABILITY = 137464,
    SPELL_HAWK_ABILITY     = 137537,
    SPELL_LIGHNING         = 137588,

    SPELL_DIREHORN_SAT     = 137518,

    SPELL_CONTROLL         = 137470,
};

// Guardian Tak'u 69336
struct npc_guardian_taku : public ScriptedAI
{
    npc_guardian_taku(Creature* creature) : ScriptedAI(creature) { }

    bool buff;

    void Reset() OVERRIDE
    {
        buff = false;

        me->CastSpell(me, SPELL_STONE_SKIN);
        me->RemoveAura(SPELL_CONTROLL);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);

        events.Reset();
        ooc_events.Reset();
        ooc_events.ScheduleEvent(EVENT_CHECK_BUFF, 2 * IN_MILLISECONDS);
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        me->RemoveAura(SPELL_STONE_SKIN);
        if (me->FindNearestCreature(NPC_HYDRA, 30.0f))
        {
            me->CastSpell(me, SPELL_HYDRA_BLESS);
            events.ScheduleEvent(EVENT_WATER_AURA, urand(12 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
        }
        if (me->FindNearestCreature(NPC_SERPENT, 30.0f))
        {
            me->CastSpell(me, SPELL_SERPENT_BLESS);
            events.ScheduleEvent(EVENT_FIRE_AURA, urand(5 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
        }
        if (me->FindNearestCreature(NPC_DIREHORN, 30.0f))
        {
            me->CastSpell(me, SPELL_DIREHORN_BLESS);
            events.ScheduleEvent(EVENT_EARTH_AURA, urand(7 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
        }
        if (me->FindNearestCreature(NPC_HAWK, 30.0f))
        {
            me->CastSpell(me, SPELL_HAWK_BLESS);
            events.ScheduleEvent(EVENT_AIR_AURA, urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
        }
        events.ScheduleEvent(EVENT_LIGHTNING, urand(5 * IN_MILLISECONDS, 7 * IN_MILLISECONDS));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        ooc_events.Update(diff);

        while (uint32 eventId = ooc_events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_ACTIVATE:
                    if (Player* player = me->FindNearestPlayer(50.0f))
                        me->Attack(player, true);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC); // no need break
                    break;
                case EVENT_CHECK_BUFF:
                    if (!buff && me->HasAura(SPELL_CONTROLL))
                    {
                        buff = true;
                        ooc_events.ScheduleEvent(EVENT_ACTIVATE, 5 * IN_MILLISECONDS);
                    }
                    else
                        ooc_events.ScheduleEvent(EVENT_CHECK_BUFF, 2 * IN_MILLISECONDS);
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_FIRE_AURA:
                {
                    me->CastSpell(me->GetVictim(), SPELL_SERPENT_ABILITY, true);
                    events.ScheduleEvent(EVENT_FIRE_AURA, urand(7 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
                    break;
                }
                case EVENT_EARTH_AURA:
                {
                    me->CastSpell(me, SPELL_DIREHORN_ABILITY, true);
                    events.ScheduleEvent(EVENT_EARTH_AURA, urand(10 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
                    break;
                }
                case EVENT_WATER_AURA:
                {
                    DoCast(SPELL_HYDRA_ABILITY);
                    events.ScheduleEvent(EVENT_WATER_AURA, urand(17 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                    break;
                }
                case EVENT_AIR_AURA:
                {
                    std::list<Creature*> hawkTriggers;
                    me->GetCreatureListWithEntryInGrid(hawkTriggers, NPC_HAWK_TRIGGER, 30.0f);
                    for (auto&& creature : hawkTriggers)
                        creature->CastSpell(creature, SPELL_HAWK_ABILITY);
                    break;
                }
                case EVENT_LIGHTNING:
                {
                    DoCastVictim(SPELL_LIGHNING);
                    events.ScheduleEvent(EVENT_LIGHTNING, urand(5 * IN_MILLISECONDS, 7 * IN_MILLISECONDS));
                    break;
                }
            }
        }

        DoMeleeAttackIfReady();
        EnterEvadeIfOutOfCombatArea(diff, 50.0f);
    }

private:
    EventMap events;
    EventMap ooc_events;
};

// Master Caller 69286
struct npc_master_caller : public customCreatureAI
{
    npc_master_caller(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();

        me->m_Events.Schedule(1000, [this]()
        {
            Creature* dino = me->FindNearestCreature(69277, 50.0f);
            if (!dino)
                dino = me->FindNearestCreature(67477, 50.0f);

            if (dino)
                DoCast(dino, 136613);
        });
    }

    void JustReachedHome() OVERRIDE
    {
        Creature* dino = me->FindNearestCreature(69277, 50.0f);
        if (!dino)
            dino = me->FindNearestCreature(67477, 50.0f);

        if (dino)
            DoCast(dino, 136613);
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        me->InterruptNonMeleeSpells(true);
        events.ScheduleEvent(1, 5  * IN_MILLISECONDS);
        events.ScheduleEvent(2, 15 * IN_MILLISECONDS);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        Creature* dino = me->FindNearestCreature(69277, 50.0f);
        if (!dino)
            dino = me->FindNearestCreature(67477, 50.0f);

        if (dino)
        {
            if (!dino->HasAura(136613))
            {
                std::list<Player*> playersForQuest;
                GetPlayerListInGrid(playersForQuest, me, 40.0f);

                for (auto&& player : playersForQuest)
                    player->KilledMonsterCredit(69276);
            }
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
        {
            ExecuteTargetEvent(140411, 5  * IN_MILLISECONDS, 1, eventId);
            ExecuteTargetEvent(140409, 15 * IN_MILLISECONDS, 2, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Puzzle Controller Bunny 40789
struct npc_puzzle_controller_bunny : public ScriptedAI
{
    npc_puzzle_controller_bunny(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    std::list<uint64> puzzleSequenceGUIDs;
    std::list<uint64> puzzleList;
    std::list<uint64> copyPuzzleList;

    void Reset() OVERRIDE
    {
        // Prevent if not this
        if (me->GetMapId() == 1064 && me->GetPositionX() > 7000.0f)
        {
            puzzleSequenceGUIDs.clear();

            scheduler
                .Schedule(Seconds(2), [this](TaskContext context) // Init delay
            {
                std::list<Creature*> cList;
                GetCreatureListWithEntryInGrid(cList, me, NPC_PUZZLE_BUNNY, 200.0f);

                // safety
                for (auto&& itr : cList)
                    puzzleList.push_back(itr->GetGUID());

                if (Creature* initPuzzle = me->SummonCreature(NPC_PUZZLE_BUNNY, puzzleStartPos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    puzzleList.push_back(initPuzzle->GetGUID());
                    puzzleSequenceGUIDs.push_back(initPuzzle->GetGUID());
                }

                copyPuzzleList = puzzleList;
            });

            scheduler
                .Schedule(Seconds(3), [this](TaskContext context)
            {
                puzzleList.clear();
                puzzleList = copyPuzzleList;

                // Exlude that we shouldn`t explode
                for (auto&& itr : puzzleSequenceGUIDs)
                    if (std::find(puzzleList.begin(), puzzleList.end(), itr) != puzzleList.end())
                        puzzleList.erase(std::find(puzzleList.begin(), puzzleList.end(), itr));

                for (auto&& itr : puzzleList)
                    if (Creature* puzzluBunny = ObjectAccessor::GetCreature(*me, itr))
                        puzzluBunny->CastSpell(puzzluBunny, SPELL_CRYSTALL_LIGHTNING_STRIKE, false);

                if (Creature* lastPuzzle = ObjectAccessor::GetCreature(*me, puzzleSequenceGUIDs.back()))
                {
                    // Path
                    std::list<Creature*> nearPuzzle;
                    GetCreatureListWithEntryInGrid(nearPuzzle, lastPuzzle, NPC_PUZZLE_BUNNY, 11.0f);
                    nearPuzzle.remove_if([=](Creature* target) { return lastPuzzle->GetGUID() == target->GetGUID(); });

                    if (!nearPuzzle.empty())
                        if (Creature* nearPuzzleSelected = Skyfire::Containers::SelectRandomContainerElement(nearPuzzle))
                            puzzleSequenceGUIDs.push_back(nearPuzzleSelected->GetGUID());

                    // Save only last 3
                    if (puzzleSequenceGUIDs.size() > 4)
                        puzzleSequenceGUIDs.pop_front();
                }

                context.Repeat(Milliseconds(1500));
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Incompleted Drakari Colossus 69347
struct npc_incompleted_drakari_colossus : public customCreatureAI
{
    npc_incompleted_drakari_colossus(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        SetCombatMovement(false);
        events.Reset();
        scheduler.CancelAll();
    }

    void JustDied(Unit* killer) OVERRIDE
    {
        killer->CastSpell(killer, 139577);
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_GROUND_SLAM, 4 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_FLAMETHOWER, 10 * IN_MILLISECONDS);

        scheduler
            .SetValidator([this] { return me->IsInCombat(); })
            .Schedule(Seconds(1), [this](TaskContext context)
        {
            if (me->GetVictim() && !me->IsWithinMeleeRange(me->GetVictim(), 6.5f))
                DoCast(me->GetVictim(), SPELL_DEATH_STARE);

            context.Repeat(Seconds(1));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);
        scheduler.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(urand(0, 1) ? SPELL_GROUND_SLAM : SPELL_SHOCKWAVE, urand(8 * IN_MILLISECONDS, 10 * IN_MILLISECONDS), EVENT_GROUND_SLAM, eventId, PRIORITY_CHANNELED);
            ExecuteTargetEvent(SPELL_FLAMETHROWER, 12500, EVENT_FLAMETHOWER, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady(true);
    }
};

class npc_iot_taran_zhu : public CreatureScript
{
    public:
        npc_iot_taran_zhu() : CreatureScript("npc_iot_taran_zhu") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(32640) == QUEST_STATUS_INCOMPLETE)
            {
                QuestObjective const* obj1 = sObjectMgr->GetQuestObjective(270185);
                QuestObjective const* obj2 = sObjectMgr->GetQuestObjective(270186);
                QuestObjective const* obj3 = sObjectMgr->GetQuestObjective(270187);
                Quest const* qInfo = sObjectMgr->GetQuestTemplate(32640);
                if (player->IsQuestObjectiveComplete(qInfo, *obj1) && player->IsQuestObjectiveComplete(qInfo, *obj2) && player->IsQuestObjectiveComplete(qInfo, *obj3))
                    player->CompleteQuest(32640);
            }

            if (player->GetQuestStatus(32641) == QUEST_STATUS_INCOMPLETE)
            {
                QuestObjective const* obj1 = sObjectMgr->GetQuestObjective(270188);
                QuestObjective const* obj2 = sObjectMgr->GetQuestObjective(270189);
                QuestObjective const* obj3 = sObjectMgr->GetQuestObjective(270190);
                Quest const* qInfo = sObjectMgr->GetQuestTemplate(32641);
                if (player->IsQuestObjectiveComplete(qInfo, *obj1) && player->IsQuestObjectiveComplete(qInfo, *obj2) && player->IsQuestObjectiveComplete(qInfo, *obj3))
                    player->CompleteQuest(32641);
            }

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }
};

// AStone Sentinel 70334
struct npc_iot_stone_sentinel : public customCreatureAI
{
    npc_iot_stone_sentinel(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        SetCombatMovement(false);
    }

    void MoveInLineOfSight(Unit* who) OVERRIDE
    {
        if (who->GetTypeId() != TypeID::TYPEID_PLAYER)
            return;
        if (!me->IsWithinDistInMap(who, 5.0f, false))
            return;
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        me->CastSpell(me, 126822);
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE { }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

class npc_assigment_map : public CreatureScript
{
    public:
        npc_assigment_map() : CreatureScript("npc_assigment_map") { }
    
        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->CLOSE_GOSSIP_MENU();
            creature->HandleSpellClick(player);
            return true;
        }
};

// Essence of Storms 69739
struct npc_nalak_essence_of_storms : public customCreatureAI
{
    npc_nalak_essence_of_storms(Creature* creature) : customCreatureAI(creature) { }

    uint64 ownerGUID;
    TaskScheduler scheduler;
    bool canAttack;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->SetSpeed(MOVE_RUN, 0.25f);
        me->AddUnitState(UNIT_STATE_ROOT);
        canAttack = false;
        DoCast(me, SPELL_SIPHON_ESSENCE, true);
        ownerGUID = summoner->GetGUID();
        DoAction(ACTION_INTRO);

        scheduler
            .Schedule(Seconds(3), [this](TaskContext context)
        {
            me->ClearUnitState(UNIT_STATE_ROOT);
            canAttack = true;
        });
    }


    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->DisappearAndDie();
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_INTRO)
            if (Unit* target = ObjectAccessor::GetUnit(*me, ownerGUID))
                SetGazeOn(target);
    }

    void JustDied(Unit* killer) OVERRIDE
    {
        if (Unit* target = ObjectAccessor::GetUnit(*me, ownerGUID))
        {
            target->CastSpell(target, SPELL_EMPOWER_LIGHTNING_LANCE, true);
            target->RemoveAurasDueToSpell(SPELL_SIPHON_ESSENCE);
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE 
    {
        scheduler.Update(diff);

        if (!UpdateVictim() || !canAttack)
            return;

        DoMeleeAttackIfReady();
    }
};

// Fleshcrafter Hoku 69435
struct npc_fleshcrafter_hoku : public customCreatureAI
{
    npc_fleshcrafter_hoku(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    bool achiev;

    void Reset() OVERRIDE
    {
        events.Reset();
        achiev = true;
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_BLOOD_BOLT, 1500);
        events.ScheduleEvent(EVENT_RIVERS_OF_BLOOD, 11 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_EXSANGUINATION, 15 * IN_MILLISECONDS);
    }

    void EnterEvadeMode() OVERRIDE
    {
        summons.DespawnAll();
        ScriptedAI::EnterEvadeMode();
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summons.Summon(summon);
    }

    void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) OVERRIDE
    {
        achiev = false;
    }

    void JustDied(Unit* killer) OVERRIDE
    {
        summons.DespawnAll();

        if (achiev)
        {
            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 50.0f);
            for (auto&& player : players)
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_YOU_MADE_ME_BLEED_ACHIEV);
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_BLOOD_BOLT, urand(3500, 4 * IN_MILLISECONDS), EVENT_BLOOD_BOLT, eventId);
            ExecuteTargetEvent(SPELL_EXSANGUINATION, 20 * IN_MILLISECONDS, EVENT_EXSANGUINATION, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_RIVERS_OF_BLOOD, 12 * IN_MILLISECONDS, EVENT_RIVERS_OF_BLOOD, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Metal Lord Meno-Han 69326
struct npc_metal_lord_meno_han : public customCreatureAI
{
    npc_metal_lord_meno_han(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_TOSS_ENERGIZED_METAL, 6500);
    }

    void EnterEvadeMode() OVERRIDE
    {
        summons.DespawnAll();
        ScriptedAI::EnterEvadeMode();
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summons.Summon(summon);
        summon->CastSpell(summon, SPELL_CHARGED_BOLT_PERIODIC, true);
    }

    void JustDied(Unit* killer) OVERRIDE
    {
        summons.DespawnAll();

        std::list<Player*> players;
        me->GetPlayerListInGrid(players, 50.0f);

        for (auto&& player : players)
            if (Aura* surge = player->GetAura(SPELL_POWER_SURGE_ACHIEV))
                if (surge->GetStackAmount() > 9)
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_SPEED_METAL_ACHIEV);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_TOSS_ENERGIZED_METAL_TAR, 12 * IN_MILLISECONDS, EVENT_TOSS_ENERGIZED_METAL, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Hatchling Skyscreamer - 69128
// Hatchling Skyscreamer - 70215
struct npc_hatchling_skyscreamer : public ScriptedAI
{
    npc_hatchling_skyscreamer(Creature* creature) : ScriptedAI(creature) { }

    bool runned;

    void Reset() OVERRIDE
    {
        runned = false;
        me->SetReactState(REACT_AGGRESSIVE);
    }

    void MoveInLineOfSight(Unit* who) OVERRIDE
    {
        if (who->GetTypeId() != TypeID::TYPEID_PLAYER)
            return;

        if (who->GetDistance(me->GetPosition()) < 5.0f)
            return;

        if (runned)
            return;

        runned = true;

        me->SetWalk(false);

        float o = me->GetPosition().GetOrientation();
        float x = me->GetPosition().GetPositionX() + (std::cos(o - (M_PI / 2))*20) + (std::cos(o)*30);
        float y = me->GetPosition().GetPositionY() + (std::sin(o - (M_PI / 2))*20) + (std::sin(o)*30);
        me->GetMotionMaster()->MovePoint(0, x, y, me->GetPositionZ());
        me->DespawnOrUnsummon(5000);

        if (Player* player = who->ToPlayer())
        {
            if (me->GetEntry() == 69128)
                player->KilledMonsterCredit(69128);
            else
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, 139283);
        }
    }
};

// Ingaluk Roach 69234
struct npc_ingaluk_roach : public ScriptedAI
{
    npc_ingaluk_roach(Creature* creature) : ScriptedAI(creature) { }

    void Reset() OVERRIDE
    {
        SetCombatMovement(false);
        me->setFaction(14);
        me->SetReactState(REACT_AGGRESSIVE);
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        if (who && who->ToPlayer())
        {
            me->setFaction(190); // won`t attack by default with 190, only assist
            who->ToPlayer()->KilledMonsterCredit(me->GetEntry());
            me->Kill(me);
        }
    }

    void JustDied(Unit* killer) OVERRIDE
    {
        if (killer && killer->ToPlayer())
            killer->ToPlayer()->StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_CREATURE, me->GetEntry());
    }
};

// Ingaluk Roach 69397
struct npc_zandalari_beastcaller_passenger : public ScriptedAI
{
    npc_zandalari_beastcaller_passenger(Creature* creature) : ScriptedAI(creature) { }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (me->IsOnVehicle())
        {
            me->ExitVehicle();

            if (attacker && attacker->ToPlayer())
                attacker->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_BOOP_ACHIEV);
        }
    }
};

class spell_incantation_of_gura : public SpellScriptLoader
{
    public:
        spell_incantation_of_gura() : SpellScriptLoader("spell_incantation_of_gura") { }

        class spell_incantation_of_gura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_incantation_of_gura_SpellScript);

            void HandleEffectHitTarget(SpellEffIndex /*eff_idx*/)
            {
                Creature* creature = GetHitCreature();
                Unit* caster = GetCaster();

                if (!creature || !caster)
                    return;

                creature->RemoveAllAuras();

                if (creature->AI())
                    creature->AI()->AttackStart(caster);

                creature->CastSpell(creature, 123625, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_incantation_of_gura_SpellScript::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_SEND_EVENT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_incantation_of_gura_SpellScript();
        }
};

// Reverberating Smash 140220
class spell_reverberating_smash : public SpellScript
{
    PrepareSpellScript(spell_reverberating_smash);

    void HandleAfterCast()
    {
        if (Unit* caster = GetCaster())
        {
            uint64 casterGUID = caster->GetGUID();
            float x = caster->GetPositionX();
            float y = caster->GetPositionY();
            float z = caster->GetPositionZ();

            caster->CastSpell(x, y, z, SPELL_REVERBERATING_SMASH_RING, true);

            caster->m_Events.Schedule(500, [caster, casterGUID, x, y, z]()
            {
                if (Unit* m_caster = ObjectAccessor::GetUnit(*caster, casterGUID))
                    m_caster->CastSpell(x, y, z, SPELL_REVERBERATING_SMASH_INNER, true);
            });
            caster->m_Events.Schedule(1000, [caster, casterGUID, x, y, z]()
            {
                if (Unit* m_caster = ObjectAccessor::GetUnit(*caster, casterGUID))
                    m_caster->CastSpell(x, y, z, SPELL_REVERBERATING_SMASH_OUT, true);
            });
        }
    }

    void Register() OVERRIDE
    {
        AfterCast += SpellCastFn(spell_reverberating_smash::HandleAfterCast);
    }
};

class ReverberatingSmashRangePredicate 
{
    public:
        ReverberatingSmashRangePredicate(uint32 const& spell_id, WorldLocation const* m_pos) : _spell_id(spell_id), _pos(m_pos) { }

        bool operator()(WorldObject* object)
        {
            switch (_spell_id)
            {
                case SPELL_REVERBERATING_SMASH_INNER:
                    range = 15.0f;
                    break;
                case SPELL_REVERBERATING_SMASH_OUT:
                    range = 20.0f;
                    break;
            }

            if (range)
                if (_pos->GetExactDist2d(object) < range)
                    return true;

            return false;
        }
    
    private:
        uint32 const& _spell_id;
        WorldLocation const* _pos;
        float range;
};

// Reverberating Smash Waves 140224, 140225
class spell_reverberating_smash_wave : public SpellScript
{
    PrepareSpellScript(spell_reverberating_smash_wave);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (SpellInfo const* m_spellInfo = GetSpellInfo())
            if (GetExplTargetDest())
                targets.remove_if(ReverberatingSmashRangePredicate(m_spellInfo->Id, GetExplTargetDest()));
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_reverberating_smash_wave::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// Lightning Crack 140239
class spell_lightning_crack : public SpellScript
{
    PrepareSpellScript(spell_lightning_crack);

    void HandleOnHit(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                caster->CastSpell(target, SPELL_LIGHTNING_CRACK_SUMM, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_lightning_crack::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Lightning Crack Visual 140240
class spell_lightning_crack_visual : public AuraScript
{
    PrepareAuraScript(spell_lightning_crack_visual);

    void OnAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* caster = GetOwner()->ToUnit())
            caster->CastSpell(caster, SPELL_LIGHTNING_CRACK_AURA, true);
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_lightning_crack_visual::OnAuraEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Alert Drumbs 136838
class spell_alert_drumbs : public AuraScript
{
    PrepareAuraScript(spell_alert_drumbs);

    void OnTrigger(AuraEffect const* /*aurEff*/)
    {
        if (Creature* owner = GetOwner()->ToCreature())
            owner->CallForHelp(50.0f);
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_alert_drumbs::OnTrigger, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// Zandalari Potion 136772
class spell_zandalari_potion : public SpellScript
{
    PrepareSpellScript(spell_zandalari_potion);

    void HandleOnHit(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
        {
            std::vector<uint32> PotionsSpellStorage = { SPELL_JAGUAR_STALKER, SPELL_JAGUAR_SPEED, SPELL_JAGUAR_FEROCITY };    
            caster->CastSpell(caster, Skyfire::Containers::SelectRandomContainerElement(PotionsSpellStorage), true);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_zandalari_potion::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Shellwind 140356
class spell_shellwind : public AuraScript
{
    PrepareAuraScript(spell_shellwind);

    void OnAuraEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Creature* caster = GetOwner()->ToCreature())
            caster->ClearUnitState(UNIT_STATE_CASTING);
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_shellwind::OnAuraEffectApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

// Pounce 133076
class spell_pounce_jump : public SpellScript
{
    PrepareSpellScript(spell_pounce_jump);

    void HandleOnHit(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                caster->CastSpell(target, SPELL_POUNCE_EFF, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_pounce_jump::HandleOnHit, EFFECT_0, SPELL_EFFECT_JUMP);
    }
};

std::unordered_map<uint32, std::pair<uint32, Position>> ritualSummons
{
    { 70203, { 69341, { 5727.82f, 5385.99f, 3.86797f, 0.71349f } } }, // Echo of Kros
    { 70201, { 69471, { 6383.20f, 6175.18f, -2.5055f, 1.53104f } } }, // Spirit of Warlord Teng (not blizzlike pos)
    { 70204, { 69633, { 6551.01f, 6374.80f, 8.20279f, 2.64397f } } }, // Kor'dok
    { 69770, { 69339, { 6483.71f, 5796.57f, 28.6016f, 2.25016f } } }, // Electromancer Ju'le
    { 70196, { 69749, { 7452.05f, 5677.36f, 49.6285f, 4.02183f } } }, // Qj'nor (not blizzlike pos)
    { 70197, { 69767, { 6828.98f, 5449.87f, 29.5743f, 0.81021f } } }, // Ancient Mogu Guardian (not blizzlike pos)
    { 70614, { 69396, { 6889.84f, 6244.11f, 1.41995f, 1.94310f } } }, // Cera
    { 70199, { 69396, { 5996.46f, 5284.26f, 7.45788f, 4.07158f } } }, // Cera (not blizzlike pos)
    { 70198, { 70080, { 7057.50f, 4793.73f, 9.38183f, 2.17161f } } }, // Windweaver Akil'amon (not blizzlike pos)
    { 70200, { 69347, { 7582.17f, 5600.78f, 25.1654f, 3.50619f } } }, // Incomplete Drakkari Colossu (not blizzlike pos)
};

// Perform Ritual 138054
class spell_preform_ritual : public SpellScript
{
    PrepareSpellScript(spell_preform_ritual);

    void HandleOnHit(SpellEffIndex /*effIndex*/)
    {
        auto itr = ritualSummons.find(GetHitUnit() ? GetHitUnit()->GetEntry() : 0);
        if (itr != ritualSummons.end())
            GetCaster()->SummonCreature(itr->second.first, itr->second.second);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_preform_ritual::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Teleport to Thunder Isle Horde 140059
class spell_teleport_to_thunder_isle_horde : public AuraScript
{
    PrepareAuraScript(spell_teleport_to_thunder_isle_horde);

    void OnUpdate(uint32 diff)
    {
        if (GetUnitOwner() && GetUnitOwner()->GetMap()->GetZoneId(GetUnitOwner()->GetPhaseMask(), GetUnitOwner()->GetPositionX(), GetUnitOwner()->GetPositionY(), GetUnitOwner()->GetPositionZ()) == 6507)
            GetUnitOwner()->RemoveAurasDueToSpell(SPELL_TELEPORT_THUNDER_ISLE_H);
    }

    void OnAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetUnitOwner())
        {
            if (Creature* bloodHawk = owner->SummonCreature(NPC_PLAYER_BLOODHAWK, *owner, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
            {
                owner->CastSpell(bloodHawk, VEHICLE_SPELL_RIDE_HARDCODED, true);
                bloodHawk->AI()->DoAction(ACTION_INTRO);
            }
        }
    }

    void Register() OVERRIDE
    {
        OnAuraUpdate += AuraUpdateFn(spell_teleport_to_thunder_isle_horde::OnUpdate);
        OnEffectRemove += AuraEffectRemoveFn(spell_teleport_to_thunder_isle_horde::OnAuraEffectRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Teleport to Thunder Isle Alliance 140125
class spell_teleport_to_thunder_isle_alliance : public AuraScript
{
    PrepareAuraScript(spell_teleport_to_thunder_isle_alliance);

    void OnUpdate(uint32 diff)
    {
        if (GetUnitOwner() && GetUnitOwner()->GetMap()->GetZoneId(GetUnitOwner()->GetPhaseMask(), GetUnitOwner()->GetPositionX(), GetUnitOwner()->GetPositionY(), GetUnitOwner()->GetPositionZ()) == 6507)
            GetUnitOwner()->RemoveAurasDueToSpell(SPELL_TELEPORT_THUNDER_ISLE_A);
    }

    void OnAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetUnitOwner())
        {
            if (Creature* hippogryph = owner->SummonCreature(NPC_PLAYER_HIPPOGRYPH, *owner, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
            {
                owner->CastSpell(hippogryph, VEHICLE_SPELL_RIDE_HARDCODED, true);
                hippogryph->AI()->DoAction(ACTION_INTRO);
            }
        }
    }

    void Register() OVERRIDE
    {
        OnAuraUpdate += AuraUpdateFn(spell_teleport_to_thunder_isle_alliance::OnUpdate);
        OnEffectRemove += AuraEffectRemoveFn(spell_teleport_to_thunder_isle_alliance::OnAuraEffectRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Summon Lorthemar Isle Intro 139721
class spell_summon_lorthemar_isle_intro : public AuraScript
{
    PrepareAuraScript(spell_summon_lorthemar_isle_intro);

    void OnAuraEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetUnitOwner())
            owner->SetPhaseMask(4, true);
    }

    void OnAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetUnitOwner())
            owner->SetPhaseMask(2, true);
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_summon_lorthemar_isle_intro::OnAuraEffectApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_summon_lorthemar_isle_intro::OnAuraEffectRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Plant Mogu Weapon 138368
class spell_plant_mogu_weapon : public SpellScript
{
    PrepareSpellScript(spell_plant_mogu_weapon);

    void FilterTargets(std::list<WorldObject*> &targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != 69227 && target->GetEntry() != 70348 && target->GetEntry() != 69210 && target->GetEntry() != 69338 && target->GetEntry() != 69229; });
        targets.remove_if([=](WorldObject* target) { return target && target->ToUnit() && (target->ToUnit()->HasAura(SPELL_PLANT_MOGU_WEAPON_EFF) || target->ToUnit()->IsAlive()); });
    }

    void HandleDamage(SpellEffIndex effIndex)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->KilledMonsterCredit(CREDIT_WEAPON_MOGU);
    }

    void Register()
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_plant_mogu_weapon::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
        OnEffectHitTarget += SpellEffectFn(spell_plant_mogu_weapon::HandleDamage, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    }
};

// Mummy Burn Corpse 136711
class spell_mummy_burn_corpse : public SpellScript
{
    PrepareSpellScript(spell_mummy_burn_corpse);

    SpellCastResult CheckCast()
    {
        if (Unit* caster = GetCaster())
            if (Creature* target = ObjectAccessor::GetCreature(*caster, caster->GetTarget()))
                if ((target->GetEntry() == 69331 || target->GetEntry() == 69337) && target->IsAlive())
                    return SpellCastResult::SPELL_CAST_OK;

        return SpellCastResult::SPELL_FAILED_UNKNOWN;
    }

    void HandleOnHit()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (GetHitUnit() && GetHitUnit()->IsAlive())
                caster->DealDamage(GetHitUnit(), GetHitUnit()->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
    }

    void Register() OVERRIDE
    {
        OnCheckCast += SpellCheckCastFn(spell_mummy_burn_corpse::CheckCast);
        OnHit += SpellHitFn(spell_mummy_burn_corpse::HandleOnHit);
    }
};

// Resonance Siphon 136970
class spell_resonanse_siphon : public AuraScript
{
    PrepareAuraScript(spell_resonanse_siphon);

    void OnAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetUnitOwner())
        {
            if (owner->HasAura(136972))
            {
                owner->RemoveAura(136972);
                owner->SummonCreature(69434, *owner, TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                if (Player* player = GetCaster()->ToPlayer())
                    player->KilledMonsterCredit(69434);
            }
        }
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_resonanse_siphon::OnAuraEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Mogu Manipulation - 136401
class spell_mogu_manipulation : public SpellScript
{
    PrepareSpellScript(spell_mogu_manipulation);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != 69140; });
        if (!targets.empty())
        {
            targets.sort(Skyfire::ObjectDistanceOrderPred{ GetCaster() });
            targets.resize(1);
        }
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mogu_manipulation::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
    }
};

// Cancel Mogu Manipulation on Death - 136703
class spell_mogu_manipulation_cancel : public SpellScript
{
    PrepareSpellScript(spell_mogu_manipulation_cancel);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return GetCaster()->GetGUID() != target->GetGUID(); });
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mogu_manipulation_cancel::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
    }
};

// Lightning Charge 136495
class spell_iot_lightning_charge : public SpellScript
{
    PrepareSpellScript(spell_iot_lightning_charge);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != 69217; });
        Skyfire::Containers::RandomResizeList(targets, 1);
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_iot_lightning_charge::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
    }
};

// Embedded Spear 137658
class spell_embedded_spear : public AuraScript
{
    PrepareAuraScript(spell_embedded_spear);

    void OnAuraEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Creature* owner = GetOwner()->ToCreature())
            owner->AI()->Talk(TALK_SPECIAL_6);
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_embedded_spear::OnAuraEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Throw Spear 137660
class spell_nalak_throw_spear : public SpellScript
{
    PrepareSpellScript(spell_nalak_throw_spear);
    SpellCastResult CheckCast()
    {
        if (Unit* caster = GetCaster())
            if (Creature* nalak = caster->FindNearestCreature(BOSS_NALAK, 50.0f, true))
                if (nalak->IsAlive())
                    return SpellCastResult::SPELL_CAST_OK;

        return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
    }

    void Register() OVERRIDE
    {
        OnCheckCast += SpellCheckCastFn(spell_nalak_throw_spear::CheckCast);
    }
};

// Summon Essence of Storm 137883
class spell_summon_essence_of_storm : public SpellScript
{
    PrepareSpellScript(spell_summon_essence_of_storm);


    void SelectTargets(SpellDestination& dest)
    {
        if (Unit* caster = GetCaster())
            if (Creature* nalak = caster->FindNearestCreature(BOSS_NALAK, 100.0f, true))
                dest.Relocate(*nalak);
    }

    void Register() OVERRIDE
    {
        OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_summon_essence_of_storm::SelectTargets, EFFECT_0, TARGET_DEST_DEST);
    }
};

// Empower Lightning Lance 139959
class spell_nalak_empower_lightning_lance : public SpellScript
{
    PrepareSpellScript(spell_nalak_empower_lightning_lance);

    void HandleCreateItem(SpellEffIndex effIndex)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (caster->HasItemCount(ITEM_LIGHTNING_LANCE))
                caster->DestroyItemCount(ITEM_LIGHTNING_LANCE, 1, true);
    }

    void Register()
    {
        OnEffectHitTarget += SpellEffectFn(spell_nalak_empower_lightning_lance::HandleCreateItem, EFFECT_0, SPELL_EFFECT_CREATE_ITEM);
    }
};

// Exsanguination 137696
class spell_hoku_exsanguination : public SpellScript
{
    PrepareSpellScript(spell_hoku_exsanguination);

    void HandleSummonBlood(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
            target->CastSpell(target, SPELL_EXSANGUINATION_SUMM, true);
    }

    void Register()
    {
        OnEffectHitTarget += SpellEffectFn(spell_hoku_exsanguination::HandleSummonBlood, EFFECT_0, SPELL_EFFECT_DAMAGE_FROM_MAX_HEALTH_PCT);
    }
};

// Toss Energized Metal Targeting 137547
class spell_mono_han_toss_energized_metal_targeting : public SpellScript
{
    PrepareSpellScript(spell_mono_han_toss_energized_metal_targeting);

    void HandleToss(SpellEffIndex effIndex)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                caster->CastSpell(target, GetSpellInfo()->Effects[effIndex].BasePoints, true);
    }

    void Register()
    {
        OnEffectHitTarget += SpellEffectFn(spell_mono_han_toss_energized_metal_targeting::HandleToss, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Power Surge Achiev 136842
class spell_power_surge_achiev : public AuraScript
{
    PrepareAuraScript(spell_power_surge_achiev);

    bool Load() OVERRIDE
    {
        oldAmount = 1;
        return true;
    }

    void HandleTick(AuraEffect const* eff)
    {
        if (GetStackAmount() > 1 && GetStackAmount() != oldAmount)
        {
            oldAmount = GetStackAmount();
            int32 newDuration = (int32)((GetStackAmount() * GetSpellInfo()->GetMaxDuration()));
            SetDuration(newDuration);
        }
    }

    private:
        uint8 oldAmount;

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_power_surge_achiev::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 855, 862. Summoned by 139917 - Thermal Expansion
class sat_thermal_expansion : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer() && triggering->ToPlayer()->IsAlive();
    }

    uint32 timer;
    float radius;

    void OnInit() OVERRIDE
    {
        timer = 1000;
        radius = 1.875f;
        HandleGrow();
    }

    void HandleGrow()
    {
        if (AreaTrigger* areaTrigger = m_target->ToAreaTrigger())
        {
            areaTrigger->SetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE, radius / 15.0f);
            areaTrigger->SetVisualRadius(radius);
            areaTrigger->UpdateObjectVisibility();
        }
    }

    void OnUpdate(uint32 diff) OVERRIDE
    {
        if (timer <= diff)
        {
            radius += 1.875f;
            HandleGrow();

            timer = 1000;
        }
        else timer -= diff;
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_THERMAL_EXPANSION_EFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_THERMAL_EXPANSION_EFF);
    }

    void OnDestroy() OVERRIDE
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(m_location->GetPositionX(), m_location->GetPositionY(), m_location->GetPositionZ(), SPELL_THERMAL_EXPANSION_AOE, true);
    }
};

// 854, 863. Summoned by 139911 - Thermal Collapse
class sat_thermal_collapse : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer() && triggering->ToPlayer()->IsAlive();
    }

    uint32 timer;
    float radius;

    void OnInit() OVERRIDE
    {
        timer = 1000;
        radius = 15.0f;
        HandleGrow();
    }

    void HandleGrow()
    {
        if (AreaTrigger* areaTrigger = m_target->ToAreaTrigger())
        {
            areaTrigger->SetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE, radius / 15.0f);
            areaTrigger->SetVisualRadius(radius);
            areaTrigger->UpdateObjectVisibility();
        }
    }

    void OnUpdate(uint32 diff) OVERRIDE
    {
        if (timer <= diff)
        {
            radius -= 1.875f;
            HandleGrow();

            timer = 1000;
        }
        else timer -= diff;
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_THERMAL_COLLAPSE_EFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_THERMAL_COLLAPSE_EFF);
    }

    void OnDestroy() OVERRIDE
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(m_location->GetPositionX(), m_location->GetPositionY(), m_location->GetPositionZ(), SPELL_THERMAL_COLLAPSE_AOE, true);
    }
};

// 776. Summoned by 137990 - lightning field
class sat_lightning_field : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer() && triggering->ToPlayer()->IsAlive();
    }

    void OnInit() OVERRIDE
    {
        if (AreaTrigger* areaTrigger = m_target->ToAreaTrigger())
            areaTrigger->SetVisualRadius(10.0f);
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_LIGHTNING_FIELD_EFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_LIGHTNING_FIELD_EFF);
    }
};

// 699. Summoned by 136737 - Rune of the Storm.
class sat_rune_of_the_storm : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToUnit();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Unit* itr = triggering->ToUnit())
            itr->CastSpell(itr, SPELL_WILD_ENERGY, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Unit* itr = triggering->ToUnit())
            itr->RemoveAurasDueToSpell(SPELL_WILD_ENERGY);
    }
};

// 746 summoned by 137464 - Wrath of the Direhorn
class sat_wrath_of_the_direhorn : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer() && triggering->ToPlayer()->IsAlive();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Unit* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_DIREHORN_SAT, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Unit* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_DIREHORN_SAT);
    }
};

// 695 summoned by 136671 - Spiritual Guide
class sat_spirital_guide : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToCreature();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Unit* itr = triggering->ToCreature())
            if (itr->HasAura(136760))
                itr->CastSpell(itr, 136676, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Unit* itr = triggering->ToCreature())
            itr->RemoveAurasDueToSpell(136676);
    }
};

class go_thunder_isle_altar : public GameObjectScript
{
    public:
        go_thunder_isle_altar() : GameObjectScript("go_thunder_isle_altar") { }

        bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
        {
            player->KilledMonsterCredit(invAltarType.find(go->GetEntry())->second);
            return true;
        }
};

class transport_zandalari_ship : public TransportScript
{
    public:
        transport_zandalari_ship() : TransportScript("transport_zandalari_ship") { }

        void OnCreate(Transport* transport) OVERRIDE
        {
            transport->EnableMovement(false);
        }
};

void AddSC_isle_of_thunder()
{
    new npc_sunreaver_construct();
    new npc_mumta();
    new creature_script<npc_thunder_pterodactyls>("npc_thunder_pterodactyls");
    new npc_progenitus();
    new npc_goda();
    new npc_godhulk_ramuk();
    new npc_altabim_the_allseeing();
    new npc_backbreaker_uru();
    new npc_luban();
    new npc_molthor();
    new npc_rasha();
    new creature_script<npc_thunder_zandalaris>("npc_thunder_zandalaris");
    new npc_spirit_of_warlord_teng();
    new creature_script<npc_shanze_bloodseeker>("npc_shanze_bloodseeker");
    new creature_script<npc_horgak_the_enslaver>("npc_horgak_the_enslaver");
    new creature_script<npc_animated_warrior>("npc_animated_warrior");
    new creature_script<npc_lightning_rod>("npc_lightning_rod");
    new creature_script<npc_electromancer_jule>("npc_electromancer_jule");
    new creature_script<npc_creeping_moor_beast>("npc_creeping_moor_beast");
    new creature_script<npc_slateshell_wanderer>("npc_slateshell_wanderer");
    new creature_script<npc_drakkari_god_hulk>("npc_drakkari_god_hulk");
    new creature_script<npc_zandalari_colossus>("npc_zandalari_colossus");
    new creature_script<npc_wave_of_might>("npc_wave_of_might");
    new creature_script<npc_zandalari_stoneshield>("npc_zandalari_stoneshield");
    new creature_script<npc_zandalari_beastcaller>("npc_zandalari_beastcaller");
    new creature_script<npc_juvenile_skyscreamer>("npc_juvenile_skyscreamer");
    new creature_script<npc_zandalari_spiritbinder>("npc_zandalari_spiritbinder");
    new creature_script<npc_zandalari_arcweaver>("npc_zandalari_arcweaver");
    new creature_script<npc_zandalari_jaguar_warrior>("npc_zandalari_jaguar_warrior");
    new creature_script<npc_shanze_thundercaller>("npc_shanze_thundercaller");
    new creature_script<npc_shanze_gravekeeper>("npc_shanze_gravekeeper");
    new creature_script<npc_sparkmancer_vu>("npc_sparkmancer_vu");
    new creature_script<npc_shanze_soulripper>("npc_shanze_soulripper");
    new creature_script<npc_ancient_stone_conqueror>("npc_ancient_stone_conqueror");
    new creature_script<npc_slateshell_ancient>("npc_slateshell_ancient");
    new creature_script<npc_tamed_bladetalon>("npc_tamed_bladetalon");
    new creature_script<npc_zandalari_beastlord>("npc_zandalari_beastlord");
    new creature_script<npc_skumblade_skirmisher>("npc_skumblade_skirmisher");
    new creature_script<npc_skumblade_brute>("npc_skumblade_brute");
    new creature_script<npc_skumblade_flithmonger>("npc_skumblade_flithmonger");
    new creature_script<npc_skeletal_lasher>("npc_skeletal_lasher");
    new npc_scout_captain_elsia();
    new creature_script<npc_sunreaver_bloodhawk>("npc_sunreaver_bloodhawk");
    new npc_vereesa_windrunner();
    new creature_script<npc_silver_covenant_hippogryph>("npc_silver_covenant_hippogryph");
    new creature_script<npc_lorthemar_theron_isle_intro>("npc_lorthemar_theron_isle_intro");
    new creature_script<npc_aethas_isle_intro>("npc_aethas_isle_intro");
    new creature_script<npc_stone_ritual_bunny_controller>("npc_stone_ritual_bunny_controller");
    new creature_script<npc_guardian_taku>("npc_guardian_taku");
    new creature_script<npc_master_caller>("npc_master_caller");
    new creature_script<npc_puzzle_controller_bunny>("npc_puzzle_controller_bunny");
    new creature_script<npc_incompleted_drakari_colossus>("npc_incompleted_drakari_colossus");
    new npc_iot_taran_zhu();
    new creature_script<npc_iot_stone_sentinel>("npc_iot_stone_sentinel");
    new npc_assigment_map();
    new creature_script<npc_nalak_essence_of_storms>("npc_nalak_essence_of_storms");
    new creature_script<npc_fleshcrafter_hoku>("npc_fleshcrafter_hoku");
    new creature_script<npc_metal_lord_meno_han>("npc_metal_lord_meno_han");
    new creature_script<npc_hatchling_skyscreamer>("npc_hatchling_skyscreamer");
    new creature_script<npc_ingaluk_roach>("npc_ingaluk_roach");
    new creature_script<npc_zandalari_beastcaller_passenger>("npc_zandalari_beastcaller_passenger");

    new spell_incantation_of_gura();
    new GenericSpellScriptLoader<spell_reverberating_smash>("spell_reverberating_smash");
    new GenericSpellScriptLoader<spell_reverberating_smash_wave>("spell_reverberating_smash_wave");
    new GenericSpellScriptLoader<spell_lightning_crack>("spell_lightning_crack");
    new GenericAuraScriptLoader<spell_lightning_crack_visual>("spell_lightning_crack_visual");
    new GenericAuraScriptLoader<spell_alert_drumbs>("spell_alert_drumbs");
    new GenericSpellScriptLoader<spell_zandalari_potion>("spell_zandalari_potion");
    new GenericAuraScriptLoader<spell_shellwind>("spell_shellwind");
    new GenericSpellScriptLoader<spell_pounce_jump>("spell_pounce_jump");
    new GenericSpellScriptLoader<spell_preform_ritual>("spell_preform_ritual");
    new GenericAuraScriptLoader<spell_teleport_to_thunder_isle_horde>("spell_teleport_to_thunder_isle_horde");
    new GenericAuraScriptLoader<spell_teleport_to_thunder_isle_alliance>("spell_teleport_to_thunder_isle_alliance");
    new GenericAuraScriptLoader<spell_summon_lorthemar_isle_intro>("spell_summon_lorthemar_isle_intro");
    new GenericSpellScriptLoader<spell_plant_mogu_weapon>("spell_plant_mogu_weapon");
    new GenericSpellScriptLoader<spell_mummy_burn_corpse>("spell_mummy_burn_corpse");
    new GenericAuraScriptLoader<spell_resonanse_siphon>("spell_resonanse_siphon");
    new GenericSpellScriptLoader<spell_mogu_manipulation>("spell_mogu_manipulation");
    new GenericSpellScriptLoader<spell_mogu_manipulation_cancel>("spell_mogu_manipulation_cancel");
    new GenericSpellScriptLoader<spell_iot_lightning_charge>("spell_iot_lightning_charge");
    new GenericAuraScriptLoader<spell_embedded_spear>("spell_embedded_spear");
    new GenericSpellScriptLoader<spell_nalak_throw_spear>("spell_nalak_throw_spear");
    new GenericSpellScriptLoader<spell_summon_essence_of_storm>("spell_summon_essence_of_storm");
    new GenericSpellScriptLoader<spell_nalak_empower_lightning_lance>("spell_nalak_empower_lightning_lance");
    new GenericSpellScriptLoader<spell_hoku_exsanguination>("spell_hoku_exsanguination");
    new GenericSpellScriptLoader<spell_mono_han_toss_energized_metal_targeting>("spell_mono_han_toss_energized_metal_targeting");
    new GenericAuraScriptLoader<spell_power_surge_achiev>("spell_power_surge_achiev");
    new atrigger_script<sat_thermal_expansion>("sat_thermal_expansion");
    new atrigger_script<sat_thermal_collapse>("sat_thermal_collapse");
    new atrigger_script<sat_lightning_field>("sat_lightning_field");
    new atrigger_script<sat_rune_of_the_storm>("sat_rune_of_the_storm");
    new atrigger_script<sat_wrath_of_the_direhorn>("sat_wrath_of_the_direhorn");
    new atrigger_script<sat_spirital_guide>("sat_spirital_guide");
    new go_thunder_isle_altar();
    new transport_zandalari_ship();
}
