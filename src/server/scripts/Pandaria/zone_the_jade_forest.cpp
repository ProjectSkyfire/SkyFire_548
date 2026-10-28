/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"

const Position MySerpentPath[2]
{
    { 1709.67f, -2667.93f, 165.56f, 2.96f },
    { 1647.79f, -2647.67f, 153.89f, 2.72f },
};

const Position MySerpentRidingPath[2]
{
    { 1601.57f, -2609.71f, 150.51f, 5.54f },
    { 1632.73f, -2638.27f, 149.90f, 5.54f },
};

enum Spells
{
    SPELL_DEAFENED                    = 108918,
    SPELL_SHA_BOLT                    = 126139,
    SPELL_GROWING_DOUBT               = 126147,

    // Misc
    SPELL_SUMMON_INSTRUCTOR_WINDBLADE = 110976,
    SPELL_FLEET_WINDS                 = 110668,
    SPELL_CLOUD_RING_VISUAL           = 110671,
    SPELL_CLOUD_RING_AREATRIGGER      = 110669,

    SPELL_ROPE_BEAM                   = 104884,
    SPELL_SILKEN_ROPE                 = 110171,
    SPELL_KEG_OF_FELLOWSHIP           = 134263,
    SPELL_FEEL_THE_FLAVOR             = 134262,
    SPELL_FEEL_THE_FLAVOR_EFF         = 134261,
    SPELL_BEHOLD_METAL_BREW           = 134180,
    SPELL_DEFEAT_METAL_BREWMASTER     = 135786,
    SPELL_SUMMON_METAL_BREWMASTER     = 134254,
    SPELL_BOILING_CAULDRON            = 102879,
    SPELL_BOILING_CAULDRON_VISUAL     = 102878,
    SPELL_CAULDRON_ROPE               = 102881,
    SPELL_STAY_AND_WHILE              = 123312,
    SPELL_CHO_TEAPOT_MASTER           = 123246,
    SPELL_MONOLITH_01_MASTER          = 123589,
    SPELL_MONOLITH_03_MASTER          = 123590,
    SPELL_MONOLITH_02_MASTER          = 123544,
    SPELL_PATH_OF_BLOSSOM_EFF         = 122036,
    SPELL_PROWL                       = 8152,
    SPELL_FEROCIOUS_CLAW              = 115083,
    SPELL_POUNCE                      = 116273,
};

enum eEvents
{
    EVENT_DEAFENED = 1,
    EVENT_GROWING_DOUBT,
    EVENT_SHA_BOLT,
    EVENT_KEG_TOSS,
    EVENT_FEROCIOUS_CLAW,
    EVENT_POUNCE,
};

enum Creatures
{
    NPC_INSTRUCTOR_WINDBLADE  = 58572,
    NPC_GOLDEN_CLOUD_SERPENT  = 58528,
    NPC_CLOUD_RING            = 58423,
    NPC_WINDWARD_HATCHLING    = 58248,
    NPC_MOGU_RUINS_CREDIT     = 55471,
    NPC_CAULDRON_CREDIT       = 55105,
    NPC_MOTHER_WU             = 55092,
    NPC_GROUNDSKEEPER_WU      = 54915,
    NPC_STAY_AND_WHILE_CRED   = 63114,
    NPC_LOREWALKER_CHO_STONES = 63217,
    NPC_STONE_1_CREDIT        = 63235,
    NPC_STONE_2_CREDIT        = 63236,
    NPC_STONE_3_CREDIT        = 63237,
};

enum Quests
{
    QUEST_GROW_LIKE_WEEDS       = 30143,
    QUEST_RING_ROUND_UP         = 30144,
    QUEST_EMPTY_NESTS           = 30136,
    QUEST_SPIRITES_PLIGHT       = 29745,
    QUEST_A_STRONG_BACK         = 29628,
    QUEST_STAY_AND_WHILE        = 31121,
    QUEST_IF_STONES_COULD_SPEAK = 31134,
};

enum eTalks
{
    TALK_INTRO,
    TALK_SPECIAL_1,
    TALK_SPECIAL_2,
    TALK_SPECIAL_3,
    TALK_SPECIAL_4,
    TALK_SPECIAL_5,
};

class npc_shadow_of_doubt : public CreatureScript
{
    public:
        npc_shadow_of_doubt() : CreatureScript("npc_shadow_of_doubt") { }

        struct npc_shadow_of_doubtAI : public ScriptedAI
        {
            npc_shadow_of_doubtAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DEAFENED,      30000);
                events.ScheduleEvent(EVENT_GROWING_DOUBT,  9000);
                events.ScheduleEvent(EVENT_SHA_BOLT,      15000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);


                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DEAFENED:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DEAFENED, false);
                            events.ScheduleEvent(EVENT_DEAFENED,      30000);
                            break;
                        case EVENT_GROWING_DOUBT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GROWING_DOUBT, false);
                            events.ScheduleEvent(EVENT_GROWING_DOUBT, 9000);
                            break;
                        case EVENT_SHA_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHA_BOLT, false);
                            events.ScheduleEvent(EVENT_SHA_BOLT, 15000);
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
            return new npc_shadow_of_doubtAI(creature);
        }
};

enum eKorNasSpells
{
    SPELL_SHADOWBOLT       = 125212,
    SPELL_VOIDCLOUD        = 125241,
};

enum eKorNasEvents
{
    EVENT_SHADOWBOLT    = 1,
    EVENT_VOIDCLOUD     = 2,
};

class npc_kor_nas_nightsavage : public CreatureScript
{
    public:
        npc_kor_nas_nightsavage() : CreatureScript("npc_kor_nas_nightsavage") { }

        struct npc_kor_nas_nightsavageAI : public ScriptedAI
        {
            npc_kor_nas_nightsavageAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHADOWBOLT,      50000);
                events.ScheduleEvent(EVENT_VOIDCLOUD,  15000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOWBOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOWBOLT, false);
                            events.ScheduleEvent(EVENT_SHADOWBOLT,      70000);
                            break;
                        case EVENT_VOIDCLOUD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_VOIDCLOUD, false);
                            events.ScheduleEvent(EVENT_VOIDCLOUD, 35000);
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
            return new npc_kor_nas_nightsavageAI(creature);
        }
};

enum eMisterFerociousSpells
{
    SPELL_GOING_BANANAS     = 125363,
    SPELL_BANANARANG        = 125311,
    SPELL_TOSS_FILTH        = 125365
};

enum eMisterFerociousEvents
{
    EVENT_GOING_BANANAS         = 1,
    EVENT_BANANARANG            = 2,
    EVENT_TOSS_FILTH            = 3
};

class npc_mister_ferocious : public CreatureScript
{
    public:
        npc_mister_ferocious() : CreatureScript("npc_mister_ferocious") { }

        struct npc_mister_ferociousAI : public ScriptedAI
        {
            npc_mister_ferociousAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GOING_BANANAS,       12000);
                events.ScheduleEvent(EVENT_BANANARANG,           8000);
                events.ScheduleEvent(EVENT_TOSS_FILTH,          15000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summon->DespawnOrUnsummon(12000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GOING_BANANAS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GOING_BANANAS, false);
                            events.ScheduleEvent(EVENT_GOING_BANANAS,      10000);
                            break;
                        case EVENT_BANANARANG:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BANANARANG, false);
                            events.ScheduleEvent(EVENT_BANANARANG, 20000);
                            break;
                        case EVENT_TOSS_FILTH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TOSS_FILTH, false);
                            events.ScheduleEvent(EVENT_TOSS_FILTH, 15000);
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
            return new npc_mister_ferociousAI(creature);
        }
};

enum kraxikTornado
{
    SPELL_TORNADO_DMG = 131693,
};

class npc_kraxik_tornado : public CreatureScript
{
    public:
        npc_kraxik_tornado() : CreatureScript("npc_kraxik_tornado") { }

        struct npc_kraxik_tornadoAI : public ScriptedAI
        {
            npc_kraxik_tornadoAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->SetDisplayFromModel(0);
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_TORNADO_DMG);

                Unit* kraxik = me->GetCharmerOrOwner();
                if (!kraxik)
                    return;

                me->GetMotionMaster()->MoveCirclePath(kraxik->GetPositionX(), kraxik->GetPositionY(), kraxik->GetPositionZ(), 5.0f, false, 20.0f);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_kraxik_tornadoAI(creature);
        }
};

enum eAkkalarSpells
{
    SPELL_BOUCLIER_D_EAU        = 79892,
    SPELL_BOUCLIER_D_EAU_2      = 34828,
    SPELL_BOUCLIER_DE_TERRE     = 79927,
    SPELL_CARAPACE_BOUCLIER     = 83488,
    SPELL_MOT_DE_POUVOIR        = 11835
};

enum eAkkalarEvents
{
    EVENT_BOUCLIER_D_EAU            = 1,
    EVENT_BOUCLIER_D_EAU_2          = 2,
    EVENT_BOUCLIER_DE_TERRE         = 3,
    EVENT_CARAPACE_BOUCLIER         = 4,
    EVENT_MOT_DE_POUVOIR            = 5
};

class npc_akkalar : public CreatureScript
{
    public:
        npc_akkalar() : CreatureScript("npc_akkalar") { }

        struct npc_akkalarAI : public ScriptedAI
        {
            npc_akkalarAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BOUCLIER_D_EAU,      10000);
                events.ScheduleEvent(EVENT_BOUCLIER_D_EAU_2,    20000);
                events.ScheduleEvent(EVENT_BOUCLIER_DE_TERRE,   30000);
                events.ScheduleEvent(EVENT_CARAPACE_BOUCLIER,   40000);
                events.ScheduleEvent(EVENT_MOT_DE_POUVOIR,      50000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BOUCLIER_D_EAU:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BOUCLIER_D_EAU, false);
                            events.ScheduleEvent(EVENT_BOUCLIER_D_EAU,     620000);
                            break;
                        case EVENT_BOUCLIER_D_EAU_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BOUCLIER_D_EAU_2, false);
                            events.ScheduleEvent(EVENT_BOUCLIER_D_EAU_2, 70000);
                            break;
                        case EVENT_BOUCLIER_DE_TERRE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BOUCLIER_DE_TERRE, false);
                            events.ScheduleEvent(EVENT_BOUCLIER_DE_TERRE, 80000);
                            break;
                        case EVENT_CARAPACE_BOUCLIER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CARAPACE_BOUCLIER, false);
                            events.ScheduleEvent(EVENT_CARAPACE_BOUCLIER, 90000);
                            break;
                        case EVENT_MOT_DE_POUVOIR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_MOT_DE_POUVOIR, false);
                            events.ScheduleEvent(EVENT_MOT_DE_POUVOIR, 100000);
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
            return new npc_akkalarAI(creature);
        }
};

enum eGrookinOutrunnerSpells
{
    SPELL_SHOOT     = 23337,
    SPELL_CLEAVE    = 40505
};

enum eGrookinOutrunnerEvents
{
    EVENT_SHOOT             = 1,
    EVENT_CLEAVE            = 2
};

class npc_grookin_outrunner : public CreatureScript
{
    public:
        npc_grookin_outrunner() : CreatureScript("npc_grookin_outrunner") { }

        struct npc_grookin_outrunnerAI : public ScriptedAI
        {
            npc_grookin_outrunnerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHOOT,        7000);
                events.ScheduleEvent(EVENT_CLEAVE,        12000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHOOT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHOOT, false);
                            events.ScheduleEvent(EVENT_SHOOT,     10000);
                            break;
                        case EVENT_CLEAVE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLEAVE, false);
                            events.ScheduleEvent(EVENT_CLEAVE, 10000);
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
            return new npc_grookin_outrunnerAI(creature);
        }
};

enum eBambooPythonSpells
{
    SPELL_SWAMP_FEVER            = 116026
};

enum eBambooPythonEvents
{
    EVENT_SWAMP_FEVER             = 1
};

#define    SPELL_STEALTH    77806

class npc_bamboo_python : public CreatureScript
{
    public:
        npc_bamboo_python() : CreatureScript("npc_bamboo_python") { }

        struct npc_bamboo_pythonAI : public ScriptedAI
        {
            npc_bamboo_pythonAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SWAMP_FEVER,        7000);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->RemoveAurasDueToSpell(SPELL_STEALTH);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SWAMP_FEVER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SWAMP_FEVER, false);
                            events.ScheduleEvent(EVENT_SWAMP_FEVER,     15000);
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
            return new npc_bamboo_pythonAI(creature);
        }
};

class npc_lurking_tiger : public CreatureScript
{
    public:
        npc_lurking_tiger() : CreatureScript("npc_lurking_tiger") { }

        struct npc_lurking_tigerAI : public ScriptedAI
        {
            npc_lurking_tigerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                me->CastSpell(me, SPELL_STEALTH, true);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_lurking_tigerAI(creature);
        }
};

enum eRakiraSpells
{
    SPELL_BLOODY_PREY               = 131145,
    SPELL_CLAW_RIP                  = 131130,
    SPELL_GROWL                     = 2649,
    SPELL_HOWL_OF_THE_DENMOTHER     = 131199,
    SPELL_ROAR_OF_COURAGE           = 93435
};

enum eRakiraEvents
{
    EVENT_BLOODY_PREY               = 1,
    EVENT_CLAW_RIP                  = 2,
    EVENT_GROWL                     = 3,
    EVENT_HOWL_OF_THE_DENMOTHER     = 4,
    EVENT_ROAR_OF_COURAGE           = 5
};

class npc_rakira : public CreatureScript
{
    public:
        npc_rakira() : CreatureScript("npc_rakira") { }

        struct npc_rakiraAI : public ScriptedAI
        {
            npc_rakiraAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BLOODY_PREY,                  8000);
                events.ScheduleEvent(EVENT_CLAW_RIP,                    15000);
                events.ScheduleEvent(EVENT_GROWL,                       22000);
                events.ScheduleEvent(EVENT_HOWL_OF_THE_DENMOTHER,       30000);
                events.ScheduleEvent(EVENT_ROAR_OF_COURAGE,             40000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLOODY_PREY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOODY_PREY, false);
                            events.ScheduleEvent(EVENT_BLOODY_PREY,      10000);
                            break;
                        case EVENT_CLAW_RIP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLAW_RIP, false);
                            events.ScheduleEvent(EVENT_CLAW_RIP,      15000);
                            break;
                        case EVENT_GROWL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GROWL, false);
                            events.ScheduleEvent(EVENT_GROWL,      22000);
                            break;
                        case EVENT_HOWL_OF_THE_DENMOTHER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HOWL_OF_THE_DENMOTHER, false);
                            events.ScheduleEvent(EVENT_HOWL_OF_THE_DENMOTHER,      30000);
                            break;
                        case EVENT_ROAR_OF_COURAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ROAR_OF_COURAGE, false);
                            events.ScheduleEvent(EVENT_ROAR_OF_COURAGE,      47000);
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
            return new npc_rakiraAI(creature);
        }
};

enum eRoShenSpells
{
    SPELL_CLAW_SLASH                = 131133,
    SPELL_BLOOD_CRAZED              = 131150,
    SPELL_BLOODTHIRST               = 131141,
    SPELL_FRENZY                    =  19615
};

enum eRoShenEvents
{
    EVENT_BLOOD_CRAZED              = 6,
    EVENT_BLOODTHIRST               = 7,
    EVENT_CLAW_SLASH                = 8,
    EVENT_FRENZY                    = 9
};

class npc_ro_shen : public CreatureScript
{
    public:
        npc_ro_shen() : CreatureScript("npc_ro_shen") { }

        struct npc_ro_shenAI : public ScriptedAI
        {
            npc_ro_shenAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BLOODY_PREY,          8000);
                events.ScheduleEvent(EVENT_CLAW_RIP,            15000);
                events.ScheduleEvent(EVENT_GROWL,               22000);
                events.ScheduleEvent(EVENT_ROAR_OF_COURAGE,     30000);
                events.ScheduleEvent(EVENT_BLOOD_CRAZED,        40000);
                events.ScheduleEvent(EVENT_BLOODTHIRST,         50000);
                events.ScheduleEvent(EVENT_CLAW_SLASH,          60000);
                events.ScheduleEvent(EVENT_FRENZY,              70000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLOODY_PREY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOODY_PREY, false);
                            events.ScheduleEvent(EVENT_BLOODY_PREY,      10000);
                            break;
                        case EVENT_CLAW_RIP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLAW_RIP, false);
                            events.ScheduleEvent(EVENT_CLAW_RIP,      15000);
                            break;
                        case EVENT_GROWL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GROWL, false);
                            events.ScheduleEvent(EVENT_GROWL,      28000);
                            break;
                        case EVENT_ROAR_OF_COURAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ROAR_OF_COURAGE, false);
                            events.ScheduleEvent(EVENT_ROAR_OF_COURAGE,      70000);
                            break;
                        case EVENT_BLOOD_CRAZED:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOOD_CRAZED, false);
                            events.ScheduleEvent(EVENT_BLOOD_CRAZED,      31000);
                            break;
                        case EVENT_BLOODTHIRST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOODTHIRST, false);
                            events.ScheduleEvent(EVENT_BLOODTHIRST,      40000);
                            break;
                        case EVENT_CLAW_SLASH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLAW_SLASH, false);
                            events.ScheduleEvent(EVENT_CLAW_SLASH,      25000);
                            break;
                        case EVENT_FRENZY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FRENZY, false);
                            events.ScheduleEvent(EVENT_FRENZY,      50000);
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
            return new npc_ro_shenAI(creature);
        }
};

enum eShaReminantSpells
{
    SPELL_SHADOW_CLAW               = 116128,
    SPELL_OVERWHELMING_GUILT          = 131150,
};

enum eShaReminantEvents
{
    EVENT_SHADOW_CLAW               = 1,
    EVENT_OVERWHELMING_GUILT        = 2,
};

class npc_sha_reminant : public CreatureScript
{
    public:
        npc_sha_reminant() : CreatureScript("npc_sha_reminant") { }

        struct npc_sha_reminantAI : public ScriptedAI
        {
            npc_sha_reminantAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHADOW_CLAW,          10000);
                events.ScheduleEvent(EVENT_OVERWHELMING_GUILT,   15000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOW_CLAW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOW_CLAW, false);
                            events.ScheduleEvent(EVENT_SHADOW_CLAW,      10000);
                            break;
                        case EVENT_OVERWHELMING_GUILT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_OVERWHELMING_GUILT, false);
                            events.ScheduleEvent(EVENT_OVERWHELMING_GUILT,      15000);
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
            return new npc_sha_reminantAI(creature);
        }
};

#define GOSSIP_CHOICE_1 "Challenge the Patriarch."

enum ePandriarchWindfurSpells
{
    SPELL_THUNDERING_PALM   = 109497,
    SPELL_WIND_SWORD        = 113504,
    //SPELL_WINDFUR_PUNCH     = 105497, <-- Needs to be scritped
};

enum ePandriarchWindfurEvents
{
    EVENT_THUNDERING_PALM      = 1,
    EVENT_WIND_SWORD           = 2,
    //EVENT_WINDFUR_PUNCH        = 3,
};

class npc_pandriarch_windfur : public CreatureScript
{
    public:
        npc_pandriarch_windfur() : CreatureScript("npc_pandriarch_windfur") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(75009, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
                {
                    creature->setFaction(14);
                    creature->SetReactState(REACT_DEFENSIVE);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->Reset();
                    creature->CombatStart(player, true);
                }
                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        struct npc_pandriarch_windfurAI : public ScriptedAI
        {
            npc_pandriarch_windfurAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            uint64 playerGuid;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_THUNDERING_PALM, urand (3000, 7000));
                events.ScheduleEvent(EVENT_WIND_SWORD, urand (8000, 10000));
                //events.ScheduleEvent(EVENT_WINDFUR_PUNCH, urand (13000, 17000));
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (me->HealthBelowPctDamaged(10, damage))
                    {
                        damage = 0;
                        me->CombatStop();
                        me->GetMotionMaster()->MovePoint(0, 1996.76001f, -2216.780029f, 247.725006f);
                        me->setFaction(35);
                        me->SetFullHealth();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        player->KilledMonsterCredit(56206);
                    }

                    if (damage > me->GetHealth())
                        damage = 0;
                }
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (Player* player = who->ToPlayer())
                {
                    if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
                        return;
                    else
                    {
                        me->CombatStop();
                        me->setFaction(35);
                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_THUNDERING_PALM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_THUNDERING_PALM, false);
                            events.ScheduleEvent(EVENT_THUNDERING_PALM,      10000);
                            break;
                        case EVENT_WIND_SWORD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WIND_SWORD, false);
                            events.ScheduleEvent(EVENT_WIND_SWORD,      15000);
                            break;
                      /*case EVENT_WINDFUR_PUNCH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WINDFUR_PUNCH, false);
                            events.ScheduleEvent(EVENT_WINDFUR_PUNCH,      15000);
                            break;*/
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pandriarch_windfurAI(creature);
        }
};

enum ePandriarchBramblestaffSpells
{
    SPELL_ROLL                   = 113512,
    SPELL_STAFF_STRIKE           = 84647,
    SPELL_THROW_BRAMBLESTAFF     = 106763,
    //SPELL_WHIRLWIND              = 15576,  <-- Needs to be scripted
    //SPELL_WHIRLWIND_2            = 17207,  <-- Needs to be scripted
};

enum ePandriarchBramblestaffEvents
{
    EVENT_ROLL                 = 1,
    EVENT_STAFF_STRIKE         = 2,
    EVENT_THROW_BRAMBLESTAFF   = 3,
    //EVENT_WHIRLWIND            = 4,
    //EVENT_WHIRLWIND_2          = 5,
};

class npc_pandriarch_bramblestaff : public CreatureScript
{
    public:
        npc_pandriarch_bramblestaff() : CreatureScript("npc_pandriarch_bramblestaff") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(75010, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
                {
                    creature->setFaction(14);
                    creature->SetReactState(REACT_DEFENSIVE);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->Reset();
                    creature->CombatStart(player, true);
                }
                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        struct npc_pandriarch_bramblestaffAI : public ScriptedAI
        {
            npc_pandriarch_bramblestaffAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            uint64 playerGuid;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_ROLL, urand (3000, 7000));
                events.ScheduleEvent(EVENT_STAFF_STRIKE, urand (8000, 10000));
                events.ScheduleEvent(EVENT_THROW_BRAMBLESTAFF, urand (13000, 17000));
                //events.ScheduleEvent(EVENT_WHIRLWIND, urand (20000, 23000));
                //events.ScheduleEvent(EVENT_WHIRLWIND_2, urand (24000, 27000));
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (me->HealthBelowPctDamaged(10, damage) || damage > me->GetHealth())
                    {
                        damage = 0;
                        me->CombatStop();
                        me->GetMotionMaster()->MovePoint(0, 1862.300049f, -2325.060059f, 257.062012f);
                        me->setFaction(35);
                        me->SetFullHealth();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        player->KilledMonsterCredit(56209);
                    }
                }
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (Player* player = who->ToPlayer())
                {
                    if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
                        return;
                    else
                    {
                        me->CombatStop();
                        me->setFaction(35);
                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ROLL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ROLL, false);
                            events.ScheduleEvent(EVENT_ROLL,      10000);
                            break;
                        case EVENT_STAFF_STRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_STAFF_STRIKE, false);
                            events.ScheduleEvent(EVENT_STAFF_STRIKE,      10000);
                            break;
                        case EVENT_THROW_BRAMBLESTAFF:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_THROW_BRAMBLESTAFF, false);
                            events.ScheduleEvent(EVENT_THROW_BRAMBLESTAFF,      10000);
                            break;
                      /*case EVENT_WHIRLWIND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WHIRLWIND, false);
                            events.ScheduleEvent(EVENT_WHIRLWIND,      10000);
                            break;
                        case EVENT_WHIRLWIND_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WHIRLWIND_2, false);
                            events.ScheduleEvent(EVENT_WHIRLWIND_2,      10000);
                            break;*/
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pandriarch_bramblestaffAI(creature);
        }
};

enum ePandriarchGoldendraftSpells
{
    SPELL_EXPLOSIVE_LAGER    = 104909,
    SPELL_FIRE_ALE           = 105091,
};

enum ePandriarchGoldendraftEvents
{
    EVENT_EXPLOSIVE_LAGER    = 1,
    EVENT_FIRE_ALE           = 2,
};

class npc_pandriarch_goldendraft : public CreatureScript
{
    public:
        npc_pandriarch_goldendraft() : CreatureScript("npc_pandriarch_goldendraft") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(75010, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
                {
                    creature->setFaction(14);
                    creature->SetReactState(REACT_DEFENSIVE);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->Reset();
                    creature->CombatStart(player, true);
                }
                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        struct npc_pandriarch_goldendraftAI : public ScriptedAI
        {
            npc_pandriarch_goldendraftAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            uint64 playerGuid;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_EXPLOSIVE_LAGER, urand (3000, 7000));
                events.ScheduleEvent(EVENT_FIRE_ALE, urand (8000, 10000));
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (me->HealthBelowPctDamaged(10, damage) || damage > me->GetHealth())
                    {
                        damage = 0;
                        me->CombatStop();
                        me->GetMotionMaster()->MovePoint(0, 1942.630005f, -2290.530029f, 240.429001f);
                        me->setFaction(35);
                        me->SetFullHealth();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        player->KilledMonsterCredit(56210);
                    }
                }
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (Player* player = who->ToPlayer())
                {
                    if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
                        return;
                    else
                    {
                        me->CombatStop();
                        me->setFaction(35);
                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_EXPLOSIVE_LAGER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_EXPLOSIVE_LAGER, false);
                            events.ScheduleEvent(EVENT_EXPLOSIVE_LAGER,      10000);
                            break;
                        case EVENT_FIRE_ALE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FIRE_ALE, false);
                            events.ScheduleEvent(EVENT_FIRE_ALE,      10000);
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
            return new npc_pandriarch_goldendraftAI(creature);
        }
};

enum eBigBaoSpells
{
    SPELL_FORCE_BURST               = 129858,
    SPELL_KNOCK_AWAY                = 129837,
    SPELL_PULL_CLOSE                = 129839,
    SPELL_RACE_AGAINST_THE_CLOCK    = 129772,
    SPELL_WAR_STOMP                 = 129831
};

enum eBigBaoEvents
{
    EVENT_FORCE_BURST                   = 1,
    EVENT_KNOCK_AWAY                    = 2,
    EVENT_PULL_CLOSE                    = 3,
    EVENT_WAR_STOMP                     = 4
};

#define GOSSIP_CHOICE_2 "Let's see which one of us is the better student. I challenge you to a duel !"

class npc_big_bao : public CreatureScript
{
    public:
        npc_big_bao() : CreatureScript("npc_big_bao") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(75012, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (player->GetQuestStatus(31718) == QUEST_STATUS_INCOMPLETE)
                {
                    creature->setFaction(14);
                    creature->SetReactState(REACT_DEFENSIVE);
                    creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->Reset();
                    creature->CombatStart(player, true);
                }
                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        struct npc_big_baoAI : public ScriptedAI
        {
            npc_big_baoAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            uint64 playerGuid;

            void Reset() OVERRIDE
            {
                events.Reset();

                events.ScheduleEvent(EVENT_FORCE_BURST, 3000);
                events.ScheduleEvent(EVENT_KNOCK_AWAY, 8000);
                events.ScheduleEvent(EVENT_PULL_CLOSE, 13000);
                events.ScheduleEvent(EVENT_WAR_STOMP, 18000);
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (Player* player = who->ToPlayer())
                {
                    if (player->GetQuestStatus(31718) == QUEST_STATUS_INCOMPLETE)
                        player->CastSpell(player, SPELL_RACE_AGAINST_THE_CLOCK);
                    else
                    {
                        me->CombatStop();
                        me->setFaction(35);
                    }
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (me->HealthBelowPctDamaged(10, damage) || damage > me->GetHealth())
                    {
                        damage = 0;
                        me->CombatStop();
                        me->GetMotionMaster()->MovePoint(0, 1604.75f, -2562.139893f, 153.134003f);
                        me->setFaction(35);
                        me->SetFullHealth();
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        player->KilledMonsterCredit(58508);
                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FORCE_BURST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FORCE_BURST, false);
                            events.ScheduleEvent(EVENT_FORCE_BURST,      10000);
                            break;
                        case EVENT_KNOCK_AWAY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_KNOCK_AWAY, false);
                            events.ScheduleEvent(EVENT_KNOCK_AWAY,      10000);
                            break;
                        case EVENT_PULL_CLOSE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_PULL_CLOSE, false);
                            events.ScheduleEvent(EVENT_PULL_CLOSE,      10000);
                            break;
                        case EVENT_WAR_STOMP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FIRE_ALE, false);
                            events.ScheduleEvent(SPELL_WAR_STOMP,      10000);
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
            return new npc_big_baoAI(creature);
        }
};

enum eCreatureSpells
{
    SPELL_GORE                   = 115821,
    SPELL_SILVERHORN_SWITFTNESS  = 115850,
    SPELL_PROTECT_YOUNG          = 115968,
    SPELL_RAZOR_TEETH            = 115656,
    SPELL_GREENSTONE_SHARDS      = 115663,
    SPELL_THROW_ORANGE           = 115678,
    SPELL_MISCHIEF               = 115358,
    SPELL_STARLING_WRATH         = 115853,
    SPELL_TRICKERY_1             = 115809,
    SPELL_TRICKERY_2             = 115814,
    SPELL_TRICKERY_3             = 115816,
    SPELL_TRICKERY_4             = 115825, // To target
    SPELL_BRITTLE                = 119858,
    SPELL_DEEP_CORRUPTION        = 116319,
    SPELL_DEEP_CORRUPTION_EFF    = 116318,
    SPELL_SHADOW_NOVA            = 116471,
    SPELL_STRIKING_STARE         = 103236,
    SPELL_SUBJUGATE              = 116838,
    SPELL_NOXIOUS_VENOM          = 106918,
    SPELL_RUSH                   = 115385,
    SPELL_VICIOUS_SNARL          = 115949,
    SPELL_HUNTING                = 115959,
    SPELL_SUMMON_QUILEN          = 124980,
    SPELL_DEVASTATING_ARC        = 124946,
    SPELL_TITANIC_STRENGTH       = 124976,
    SPELL_SHANZE_SPIRIT_TRAP     = 115510,
    SPELL_SHANZE_SPIRIT_TRAP_EFF = 117741,
    SPELL_SPIRIT_CLEAVE          = 115530,
    SPELL_SPINNING_CRANE_KICK    = 125799,
    SPELL_HEALING_MISTS          = 125802,
    SPELL_SPIT_SILK              = 129916,
    SPELL_POISON_STING           = 129922,
    SPELL_SMOKED_BLADE_REMOVE    = 125657,
    SPELL_SEED_OF_DOUBT          = 126149,
    SPELL_OVERPOWERING_DOUBT     = 126151,
    SPELL_TEAR_FLASH             = 131108,
    SPELL_BITE                   = 131103,
    SPELL_AVATAR                 = 130471,
    SPELL_REND                   = 13738,
    SPELL_SLAM                   = 130198,
    SPELL_MORTAL_STRIKE          = 32736,
    SPELL_SPRING                 = 130475,
    SPELL_SUNDER_ARMOR           = 64978,
    SPELL_FAE_TOUCH              = 130159,
    SPELL_THUNDERFIST            = 116007,
    SPELL_THUNDERFIST_RAGE       = 116013,
};

enum eCreatureEvents
{
    EVENT_GORE      = 1,
    EVENT_SWIFTNESS,
    EVENT_PROTECT_YOUNG,
    EVENT_CLAW,
    EVENT_TEETH,
    EVENT_SHARDS,
    EVENT_THROW_ORANGE,
    EVENT_MISCHIEF,
    EVENT_STARLING_WRATH,
    EVENT_TRICKERY,
    EVENT_DEEP_CORRUPTION,
    EVENT_SHADOW_NOVA,
    EVENT_STRIKING_STARE,
    EVENT_SUBJUGATE,
    EVENT_VENOM,
    EVENT_RUSH,
    EVENT_VIC_SNURL,
    EVENT_HUNTING,
    EVENT_DEV_ARC,
    EVENT_TITAN_STRENGTH,
    EVENT_SHANZE_TRAP,
    EVENT_SPIRIT_CLEAVE,
    EVENT_CRANE_KICK,
    EVENT_HEAL_MISTS,
    EVENT_SPIT_SILK,
    EVENT_POISON_STING,
    EVENT_SEED_OF_DOUBT,
    EVENT_TEAR_FLASH,
    EVENT_BITE,
    EVENT_SPECIAL_ATTACK,
    EVENT_SECOND_ATTACK,
    EVENT_FAE_TOUCH,
    EVENT_THUNDERFIST,
    EVENT_THUNDERFIST_RAGE,
};

const std::vector<uint32> TrickeryType=
{
    SPELL_TRICKERY_1,
    SPELL_TRICKERY_2,
    SPELL_TRICKERY_3,
    SPELL_TRICKERY_4,
};

// Silverhorn Stag 59743
struct npc_silverhorn_stag : public ScriptedAI
{
    npc_silverhorn_stag(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_GORE, urand(3 * IN_MILLISECONDS, 4 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_SWIFTNESS, 10 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_GORE:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_GORE);

                    events.ScheduleEvent(EVENT_GORE, urand(13 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
                    break;
                case EVENT_SWIFTNESS:
                    DoCast(me, SPELL_SILVERHORN_SWITFTNESS);
                    events.ScheduleEvent(EVENT_SWIFTNESS, 15 * IN_MILLISECONDS);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Forest Huntress 59102
struct npc_forest_huntress : public ScriptedAI
{
    npc_forest_huntress(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_CLAW, urand(3 * IN_MILLISECONDS, 4 * IN_MILLISECONDS));
        //events.ScheduleEvent(EVENT_PROTECT_YOUNG, 8 * IN_MILLISECONDS); Missed cube spawn
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_CLAW)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_FEROCIOUS_CLAW);

                events.ScheduleEvent(EVENT_CLAW, urand(7500, 9 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Greenstone Nibbler 56401
struct npc_greenstone_nibbler : public ScriptedAI
{
    npc_greenstone_nibbler(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_TEETH, 5 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_TEETH)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_RAZOR_TEETH);

                events.ScheduleEvent(EVENT_TEETH, urand(11 * IN_MILLISECONDS, 13 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Greenstone Gorger 56404
struct npc_greenstone_gorger : public ScriptedAI
{
    npc_greenstone_gorger(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SHARDS, urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_SHARDS)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_GREENSTONE_SHARDS);

                events.ScheduleEvent(EVENT_SHARDS, urand(12500, 18 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Greenwood Thief 54930
struct npc_greenwood_thief : public ScriptedAI
{
    npc_greenwood_thief(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_THROW_ORANGE, urand(3500, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_MISCHIEF, 10 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_THROW_ORANGE:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_THROW_ORANGE);

                    events.ScheduleEvent(EVENT_THROW_ORANGE, urand(3500, 12 * IN_MILLISECONDS));
                    break;
                case EVENT_MISCHIEF:
                    DoCast(me, SPELL_MISCHIEF);
                    events.ScheduleEvent(EVENT_MISCHIEF, 25 * IN_MILLISECONDS);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Ivory Starling 59774
struct npc_ivory_starling : public ScriptedAI
{
    npc_ivory_starling(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_STARLING_WRATH, urand(3 * IN_MILLISECONDS, 7500));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_STARLING_WRATH)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_STARLING_WRATH);

                events.ScheduleEvent(EVENT_STARLING_WRATH, urand(4500, 10 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Greenwood Trickster 54987
struct npc_greenwood_trickster : public ScriptedAI
{
    npc_greenwood_trickster(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    uint32 selectedTrick;

    void Reset() OVERRIDE
    {
        events.Reset();
        selectedTrick = 0;
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_TRICKERY, 5 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_TRICKERY)
            {
                selectedTrick = Skyfire::Containers::SelectRandomContainerElement(TrickeryType);

                if (selectedTrick == SPELL_TRICKERY_4 && me->GetVictim())
                    DoCast(me->GetVictim(), selectedTrick);
                else
                    DoCast(me, selectedTrick);

                events.ScheduleEvent(EVENT_TRICKERY, 15 * IN_MILLISECONDS);
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Brittle Greenstone Gorger 56543
struct npc_brittle_greenstone_gorger : public ScriptedAI
{
    npc_brittle_greenstone_gorger(Creature* creature) : ScriptedAI(creature) { }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(51) && !me->HasAura(SPELL_BRITTLE))
            DoCast(me, SPELL_BRITTLE);
    }
};

// Shade of Ling Heartfist 56441
struct npc_shade_of_ling_heartfist : public ScriptedAI
{
    npc_shade_of_ling_heartfist(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_DEEP_CORRUPTION, urand(6 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_SHADOW_NOVA, 13500);
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
        summon->CastSpell(summon, SPELL_DEEP_CORRUPTION_EFF, true);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_DEEP_CORRUPTION:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_DEEP_CORRUPTION);

                    events.ScheduleEvent(EVENT_DEEP_CORRUPTION, urand(8 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
                    break;
                case EVENT_SHADOW_NOVA:
                    DoCast(me, SPELL_SHADOW_NOVA);
                    events.ScheduleEvent(EVENT_SHADOW_NOVA, 15500);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Jade Guardian 55236
struct npc_jade_guardian : public ScriptedAI
{
    npc_jade_guardian(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_STRIKING_STARE, 5 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_STRIKING_STARE)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_STRIKING_STARE);

                events.ScheduleEvent(EVENT_STRIKING_STARE, urand(8500, 18500));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Subjugator Gormal 55016
struct npc_subjugator_gormal : public ScriptedAI
{
    npc_subjugator_gormal(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SUBJUGATE, 8500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_SUBJUGATE)
            {
                DoCast(me, SPELL_SUBJUGATE);
                events.ScheduleEvent(EVENT_SUBJUGATE, urand(14500, 22 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Orchard Wasp 56201
struct npc_orchard_wasp : public ScriptedAI
{
    npc_orchard_wasp(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_VENOM, 3500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_VENOM)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_NOXIOUS_VENOM);

                events.ScheduleEvent(EVENT_VENOM, urand(8500, 13 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Ink-Tipped Crane 58694
struct npc_ink_tipped_crane : public ScriptedAI
{
    npc_ink_tipped_crane(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_RUSH, 8500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_RUSH)
            {
                DoCast(me, SPELL_RUSH);
                events.ScheduleEvent(EVENT_RUSH, urand(15500, 23 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Crane Hunter 55565
struct npc_crane_hunter : public ScriptedAI
{
    npc_crane_hunter(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_VIC_SNURL, urand(3500, 8 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_HUNTING, 10 * IN_MILLISECONDS);
        //events.ScheduleEvent(EVENT_PROTECT_YOUNG, 12 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_VIC_SNURL:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_VICIOUS_SNARL);

                    events.ScheduleEvent(EVENT_VIC_SNURL, urand(7 * IN_MILLISECONDS, 14500));
                    break;
                case EVENT_HUNTING:
                    DoCast(me, SPELL_HUNTING);
                    events.ScheduleEvent(EVENT_HUNTING, urand(20 * IN_MILLISECONDS, 25 * IN_MILLISECONDS));
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Morgrinn Crackfang 50350
struct npc_morgrinn_crackfang : public ScriptedAI
{
    npc_morgrinn_crackfang(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    uint32 delay;
    uint64 targetGUID;

    void Reset() OVERRIDE
    {
        events.Reset();
        delay      = 0;
        targetGUID = 0;
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        DoCast(me, SPELL_SUMMON_QUILEN);
        events.ScheduleEvent(EVENT_DEV_ARC, urand(8500, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_TITAN_STRENGTH, 10 * IN_MILLISECONDS);
        //events.ScheduleEvent(EVENT_PROTECT_YOUNG, 12 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_DEV_ARC:
                    if (Unit* target = me->GetVictim())
                    {
                        targetGUID = target->GetGUID();
                        me->PrepareChanneledCast(me->GetAngle(target), SPELL_DEVASTATING_ARC);

                        delay = 0;
                        me->m_Events.Schedule(delay += 3500, 1, [this]()
                        {
                            me->RemoveChanneledCast(targetGUID);
                        });
                    }

                    events.ScheduleEvent(EVENT_DEV_ARC, urand(12 * IN_MILLISECONDS, 18 * IN_MILLISECONDS));
                    break;
                case EVENT_TITAN_STRENGTH:
                    DoCast(me, SPELL_TITANIC_STRENGTH);
                    events.ScheduleEvent(EVENT_TITAN_STRENGTH, 10 * IN_MILLISECONDS);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Shanze Spiritclaw 55902
struct npc_shanze_spiritclaw : public ScriptedAI
{
    npc_shanze_spiritclaw(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SPIRIT_CLEAVE, urand(3500, 7500));
        events.ScheduleEvent(EVENT_SHANZE_TRAP, 10 * IN_MILLISECONDS);
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
        summon->CastSpell(summon, SPELL_SHANZE_SPIRIT_TRAP_EFF, true);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_SPIRIT_CLEAVE:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_SPIRIT_CLEAVE);

                    events.ScheduleEvent(EVENT_SPIRIT_CLEAVE, urand(5500, 10500));
                    break;
                case EVENT_SHANZE_TRAP:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_SHANZE_SPIRIT_TRAP);

                    events.ScheduleEvent(EVENT_SHANZE_TRAP, urand(12 * IN_MILLISECONDS, 19 * IN_MILLISECONDS));
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Urobi the Walker 50808
struct npc_urobi_the_walker : public ScriptedAI
{
    npc_urobi_the_walker(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_CRANE_KICK, urand(9 * IN_MILLISECONDS, 13 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_HEAL_MISTS, 10 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CRANE_KICK:
                    DoCast(me, SPELL_SPINNING_CRANE_KICK);
                    events.ScheduleEvent(EVENT_CRANE_KICK, urand(17 * IN_MILLISECONDS, 25 * IN_MILLISECONDS));
                    break;
                case EVENT_HEAL_MISTS:
                    if (HealthAbovePct(70))
                    {
                        events.ScheduleEvent(EVENT_HEAL_MISTS, 4 * IN_MILLISECONDS);
                        break;
                    }

                    DoCast(me, SPELL_HEALING_MISTS);
                    events.ScheduleEvent(EVENT_HEAL_MISTS, 16 * IN_MILLISECONDS);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Siftworm 65711
struct npc_siftworm : public ScriptedAI
{
    npc_siftworm(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SPIT_SILK, urand(3500, 12 * IN_MILLISECONDS));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_SPIT_SILK)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_SPIT_SILK);

                events.ScheduleEvent(EVENT_SPIT_SILK, urand(3500, 12 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Honeykeeper 58363
struct npc_honeykeeper : public ScriptedAI
{
    npc_honeykeeper(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_POISON_STING, urand(4500, 22 * IN_MILLISECONDS));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_POISON_STING)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_POISON_STING);

                events.ScheduleEvent(EVENT_POISON_STING, urand(4500, 22 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Final Doubt 57330
struct npc_final_doubt : public ScriptedAI
{
    npc_final_doubt(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SEED_OF_DOUBT, urand(4500, 9500));
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(51) && !me->HasAura(SPELL_OVERPOWERING_DOUBT))
            DoCast(me, SPELL_OVERPOWERING_DOUBT);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_SEED_OF_DOUBT)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_SEED_OF_DOUBT);

                events.ScheduleEvent(EVENT_SEED_OF_DOUBT, urand(10500, 18 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Nurse Shark 66750
struct npc_nurse_shark : public ScriptedAI
{
    npc_nurse_shark(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_BITE, urand(4500, 9500));
        events.ScheduleEvent(EVENT_TEAR_FLASH, 11 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_BITE:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_BITE);

                    events.ScheduleEvent(EVENT_BITE, urand(4500, 9 * IN_MILLISECONDS));
                    break;
                case EVENT_TEAR_FLASH:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_TEAR_FLASH);

                    events.ScheduleEvent(EVENT_TEAR_FLASH, urand(12 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Slitherscale Lizard-Lord 63536
struct npc_slitherscale_lizard_lord : public ScriptedAI
{
    npc_slitherscale_lizard_lord(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    uint32 prevSpellId;

    void Reset() OVERRIDE
    {
        events.Reset();
        prevSpellId = 0;
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SPECIAL_ATTACK, urand(4500, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_SECOND_ATTACK, urand(10 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(51) && !me->HasAura(SPELL_AVATAR))
            DoCast(me, SPELL_AVATAR);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_SPECIAL_ATTACK:
                {
                    std::vector<uint32> SpellStorage = { SPELL_MORTAL_STRIKE, SPELL_SLAM, SPELL_REND };

                    if (prevSpellId)
                        SpellStorage.erase(std::find(SpellStorage.begin(), SpellStorage.end(), prevSpellId));

                    // Select new spell from updated container
                    prevSpellId = Skyfire::Containers::SelectRandomContainerElement(SpellStorage);

                    if (Unit* target = me->GetVictim())
                        DoCast(target, prevSpellId);

                    events.ScheduleEvent(EVENT_SPECIAL_ATTACK, urand(4500, 12 * IN_MILLISECONDS));
                    break;
                }
                case EVENT_SECOND_ATTACK:
                    if (urand(0, 1))
                        DoCast(me, SPELL_SPRING);
                    else if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_SUNDER_ARMOR);

                    events.ScheduleEvent(EVENT_SECOND_ATTACK, urand(10 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Shadowfae Trickster 65635
struct npc_shadowfae_trickster : public ScriptedAI
{
    npc_shadowfae_trickster(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_FAE_TOUCH, urand(3500, 12500));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_FAE_TOUCH)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_FAE_TOUCH);

                events.ScheduleEvent(EVENT_FAE_TOUCH, urand(3500, 15 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Thunderfist Gorilla 59742, 59747
struct npc_thunderfist_gorilla : public ScriptedAI
{
    npc_thunderfist_gorilla(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_THUNDERFIST, urand(7500, 12500));

        if (me->GetEntry() == 59742)
            events.ScheduleEvent(EVENT_THUNDERFIST_RAGE, 10 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_THUNDERFIST:
                    DoCast(me, SPELL_THUNDERFIST);
                    events.ScheduleEvent(EVENT_THUNDERFIST, urand(12500, 20 * IN_MILLISECONDS));
                    break;
                case EVENT_THUNDERFIST_RAGE:
                    DoCast(me, SPELL_THUNDERFIST_RAGE);
                    events.ScheduleEvent(EVENT_THUNDERFIST_RAGE, 10 * IN_MILLISECONDS);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

enum eHutiaSpells
{
    SPELL_FRENZY_2              = 19615,
    SPELL_SPIRIT_BEAST_BLESSING = 127830,
    SPELL_SPIRIT_HEAL_2           = 138477,
    SPELL_SPIRIT_MEND           = 90361,
    SPELL_STRONG_WILL           = 138472
};

enum eHutiaEvents
{
    EVENT_FRENZY_2              = 1,
    EVENT_SPIRIT_BEAST_BLESSING = 2,
    EVENT_SPIRIT_HEAL           = 3
};

class npc_hutia : public CreatureScript
{
    public:
        npc_hutia() : CreatureScript("npc_hutia") { }

        struct npc_hutiatAI : public ScriptedAI
        {
            npc_hutiatAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();

                me->CastSpell(me, SPELL_SPIRIT_MEND, false);
                me->CastSpell(me, SPELL_STRONG_WILL, false);

                events.ScheduleEvent(EVENT_FRENZY_2, 7000);
                events.ScheduleEvent(EVENT_SPIRIT_BEAST_BLESSING, 12000);
                events.ScheduleEvent(EVENT_SPIRIT_HEAL, 19000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FRENZY_2:
                            me->CastSpell(me, SPELL_FRENZY_2, false);
                            events.ScheduleEvent(EVENT_FRENZY_2,      30000);
                            break;
                        case EVENT_SPIRIT_BEAST_BLESSING:
                            me->CastSpell(me, SPELL_SPIRIT_BEAST_BLESSING, false);
                            events.ScheduleEvent(EVENT_SPIRIT_BEAST_BLESSING,      40000);
                            break;
                        case EVENT_SPIRIT_HEAL:
                            me->CastSpell(me, SPELL_SPIRIT_HEAL_2, false);
                            events.ScheduleEvent(EVENT_SPIRIT_HEAL,      40000);
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
            return new npc_hutiatAI(creature);
        }
};

class npc_nectarbreeze_farmer : public CreatureScript
{
    public:
        npc_nectarbreeze_farmer() : CreatureScript("npc_nectarbreeze_farmer") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (player->GetQuestStatus(29579) == QUEST_STATUS_INCOMPLETE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Get to Hanae's house. It's safe there.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (player->GetQuestStatus(29579) == QUEST_STATUS_INCOMPLETE)
                {
                    player->CastSpell(player, 102469, true);
                    creature->AI()->Talk(0);
                    creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->DespawnOrUnsummon(2000);
                }
                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }
};

// 58220, 58243, 58244
struct npc_windward_hatchling : public ScriptedAI
{
    npc_windward_hatchling(Creature* creature) : ScriptedAI(creature) { }

    void Reset() OVERRIDE { }

    void SpellHit(Unit* caster, SpellInfo const* spell) OVERRIDE
    {
        auto player = caster->ToPlayer();
        if (spell->Id == SPELL_SILKEN_ROPE && player && player->GetQuestStatus(QUEST_EMPTY_NESTS) == QUEST_STATUS_INCOMPLETE)
        {
            if (auto summon = player->SummonCreature(NPC_WINDWARD_HATCHLING, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 300000ms))
            {
                summon->SetOwnerGUID(player->GetGUID());
                summon->SetDisplayId(me->GetDisplayId());
                summon->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, M_PI);
                summon->CastSpell(player, SPELL_ROPE_BEAM, true);
                summon->ClearUnitState(UNIT_STATE_CASTING); // Allows move
                me->DespawnOrUnsummon();
            }
        }
    }
};

// 58275
struct npc_windward_nest_trigger : public ScriptedAI
{
    npc_windward_nest_trigger(Creature* creature) : ScriptedAI(creature) { me->SetReactState(REACT_AGGRESSIVE); }

    void Reset() OVERRIDE { }

    void MoveInLineOfSight(Unit* who) OVERRIDE
    {
        if (who->GetTypeId() == TypeID::TYPEID_UNIT && who->GetEntry() == NPC_WINDWARD_HATCHLING)
        {
            if (who->GetDistance(me) > 5.0f)
                return;

            if (auto owner = who->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                owner->KilledMonsterCredit(58246);
                who->RemoveAurasDueToSpell(SPELL_ROPE_BEAM);
                who->GetMotionMaster()->MoveFollow(me, 0, 0);
                who->ToCreature()->SetOwnerGUID(0);
                who->ToCreature()->DespawnOrUnsummon(1000);
            }
        }
    }
};

class npc_instructor_skythorn : public CreatureScript
{
    public:
        npc_instructor_skythorn() : CreatureScript("npc_instructor_skythorn") { }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
        {
            if (quest->GetQuestId() == 30142)
                CAST_AI(npc_instructor_skythorn::npc_instructor_skythornAI, creature->AI())->BeginEvent(player);

            return true;
        }

        struct npc_instructor_skythornAI : public ScriptedAI
        {
            npc_instructor_skythornAI(Creature* creature) : ScriptedAI(creature) { }

            uint64 playerGUID;
            uint8 eventPhase;
            uint32 eventTimer;

            void Reset() OVERRIDE
            {
                eventTimer = 0;
                playerGUID = 0;
                eventPhase = 0;
            }

            void BeginEvent(Player* player)
            {
                eventTimer = 10000;
                playerGUID = player->GetGUID();
                Talk(0, Player::GetPlayer(*me, playerGUID));
                eventPhase = 1;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (eventPhase)
                {
                    if (eventTimer < diff)
                    {
                        Talk(eventPhase, Player::GetPlayer(*me, playerGUID));
                        eventTimer = 8000;
                        ++eventPhase;
                        if (eventPhase == 4)
                        {
                            if (auto player = Player::GetPlayer(*me, playerGUID))
                                player->KilledMonsterCredit(58315);
                            Reset();
                        }
                    }
                    else
                        eventTimer -= diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_instructor_skythornAI(creature);
        }
};

class npc_the_pearlfin_situation_q : public CreatureScript
{
    public:
        npc_the_pearlfin_situation_q() : CreatureScript("npc_the_pearlfin_situation_q") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (creature->IsVendor())
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

            if (player->GetQuestStatus(29883) == QUEST_STATUS_INCOMPLETE)
            {
                std::string gossip = "";
                switch (creature->GetEntry())
                {
                case 59058:
                    gossip = "Please allow us a chance to prove our friendship. We wish you no harm";
                    break;
                case 56693:
                    gossip = "What are you doing?";
                    break;
                case 56690:
                    gossip = "My friends and I come with peaceful intentions.";
                    break;
                case 54960:
                    gossip = "I come from the Alliance. We wish to be allies, not enemies.";
                    break;
                }
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossip, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            }

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                creature->AI()->Talk(0);
                player->KilledMonsterCredit(creature->GetEntry());
                player->CLOSE_GOSSIP_MENU();
            }
            else if (action == GOSSIP_ACTION_TRADE)
                player->GetSession()->SendListInventory(creature->GetGUID());

            return false;
        }
};

enum q29586
{
    QUEST_THE_SPLINTED_PATH = 29586,

    NPC_GORMALI_STALKER     = 55084,
    NPC_SHAO_THE_DEFIANT    = 55009,
    NPC_CREDIT_TRIGGER      = 54999,

    SAY_GORMALI_STALKER_ANN   = 0,
    SAY_GORMALI_STALKER_START = 1,
    SAY_SHAO_THE_DEFIANT_ANN  = 0,
};

Position pos[4] =
{
    { 169.715f, -1470.011f, 107.864f, 0.8308f }, // spawn pos 1
    { 217.769f, -1460.120f, 81.6439f, 0.4533f }, // jump pos 1
    { 175.699f, -1476.424f, 107.567f, 0.5414f }, // spawn pos 2
    { 217.361f, -1469.312f, 82.2334f, 0.1932f }, // jump pos 2
};

class AreaTrigger_q29586 : public AreaTriggerScript
{
    public:
        AreaTrigger_q29586() : AreaTriggerScript("AreaTrigger_q29586") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            if (player->GetQuestStatus(QUEST_THE_SPLINTED_PATH) != QUEST_STATUS_INCOMPLETE || player->FindNearestCreature(NPC_GORMALI_STALKER, 150.0f, true))
                return false;

            uint64 playerGuid = player->GetGUID();
            if (Creature* gormaliStalker1 = player->SummonCreature(NPC_GORMALI_STALKER, pos[0], TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000ms))
            {
                gormaliStalker1->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                gormaliStalker1->SetHomePosition(pos[1]);
                gormaliStalker1->AI()->Talk(SAY_GORMALI_STALKER_ANN, player);

                uint32 delay = 0;
                gormaliStalker1->m_Events.Schedule(delay += 1000, [gormaliStalker1, playerGuid]()
                {
                    gormaliStalker1->GetMotionMaster()->MoveJump(pos[1], 30.0f, 30.0f);
                    gormaliStalker1->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    if (Player* player = ObjectAccessor::GetPlayer(*gormaliStalker1, playerGuid))
                        gormaliStalker1->Attack(player, true);
                });
                gormaliStalker1->m_Events.Schedule(delay += 2000, [gormaliStalker1]()
                {
                    gormaliStalker1->AI()->Talk(SAY_GORMALI_STALKER_START);
                });
                gormaliStalker1->m_Events.Schedule(delay += 1000, [gormaliStalker1, playerGuid]()
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*gormaliStalker1, playerGuid))
                        if (Creature* shao = player->FindNearestCreature(NPC_SHAO_THE_DEFIANT, 150.0f, true))
                        {
                            shao->AI()->Talk(SAY_SHAO_THE_DEFIANT_ANN, player);
                            player->KilledMonsterCredit(NPC_CREDIT_TRIGGER);
                        }
                });
            }

            if (Creature* gormaliStalker2 = player->SummonCreature(NPC_GORMALI_STALKER, pos[2], TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000ms))
            {
                gormaliStalker2->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                gormaliStalker2->SetHomePosition(pos[3]);

                uint32 delay = 0;
                gormaliStalker2->m_Events.Schedule(delay += 1000, [gormaliStalker2, playerGuid]()
                {
                    gormaliStalker2->GetMotionMaster()->MoveJump(pos[3], 30.0f, 30.0f);
                    gormaliStalker2->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    if (Player* player = ObjectAccessor::GetPlayer(*gormaliStalker2, playerGuid))
                        gormaliStalker2->Attack(player, true);
                });
            }

            return true;
        }
};

enum q30063
{
    NPC_ELDER_SAGE_STORM_SING = 57319,
};

class spell_q30063 : public SpellScriptLoader
{
    public:
        spell_q30063() : SpellScriptLoader("spell_q30063") { }

        class spell_q30063_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_q30063_AuraScript)

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER)
                    if (Player* player = GetCaster()->ToPlayer())
                        player->Whisper(sCreatureTextMgr->GetLocalizedChatString(NPC_ELDER_SAGE_STORM_SING, 0, urand(0, 7), Gender(player->getGender()), player->GetSession()->GetSessionDbLocaleIndex()).c_str(), Language::LANG_UNIVERSAL, player);
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_q30063_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_q30063_AuraScript();
        }
};

enum q29637
{
    NPC_INSTRUCTOR_MYANG = 54918,
    NPC_RUMPUS_COMBATANT = 55139,
    NPC_RUMPUS_BRUTE     = 55151,

    SAY_MYANG_1          = 1,
    SAY_MYANG_2          = 2,
    SAY_MYANG_3          = 3,
    SAY_MYANG_4          = 4,
    SAY_MYANG_5          = 5,
};

Position rumpusEndPoint = { 2576.164f, -1239.395f, 377.749f, 1.611f };

class spell_q29637 : public SpellScriptLoader
{
    public:
        spell_q29637() : SpellScriptLoader("spell_q29637") { }

        class spell_q29637_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_q29637_SpellScript)

            void HandleApplyAura()
            {
                Player* caster = GetCaster()->ToPlayer();
                if (caster->GetQuestStatus(29637) != QUEST_STATUS_INCOMPLETE)
                    return;

                uint64 casterGuid = caster->GetGUID();
                Creature* myang = caster->FindNearestCreature(NPC_INSTRUCTOR_MYANG, 100.0f);
                if (!myang)
                    return;

                myang->AI()->Talk(SAY_MYANG_1, caster);
                SpawnWave(myang, casterGuid, 2, 0);

                uint32 delay = 0;
                myang->m_Events.Schedule(delay += 5000,  [myang]()             { myang->AI()->Talk(SAY_MYANG_2);     });
                delay = 0;
                myang->m_Events.Schedule(delay += 15000, [casterGuid, myang]() { SpawnWave(myang, casterGuid, 4, 0); });
                myang->m_Events.Schedule(delay += 15000, [casterGuid, myang]() { SpawnWave(myang, casterGuid, 1, 1); }); // 30 sec
                myang->m_Events.Schedule(delay += 15000, [casterGuid, myang]() { SpawnWave(myang, casterGuid, 8, 0); }); // 45 sec
                myang->m_Events.Schedule(delay += 15000, [casterGuid, myang]() { SpawnWave(myang, casterGuid, 8, 0); }); // 1 min
                myang->m_Events.Schedule(delay += 15000, [casterGuid, myang]()
                {
                    SpawnWave(myang, casterGuid, 8, 0);
                    if (Player* player = ObjectAccessor::GetPlayer(*myang, casterGuid))
                        myang->AI()->Talk(SAY_MYANG_3, player);                                                      }); // 1 min 15 sec
                myang->m_Events.Schedule(delay += 15000, [casterGuid, myang]() { SpawnWave(myang, casterGuid, 0, 1); }); // 1 min 30 sec
                myang->m_Events.Schedule(delay += 15000, [casterGuid, myang]()
                {
                    SpawnWave(myang, casterGuid, 10, 0);
                    if (Player* player = ObjectAccessor::GetPlayer(*myang, casterGuid))
                        myang->AI()->Talk(SAY_MYANG_4, player);                                                      }); // 1 min 45 sec
                myang->m_Events.Schedule(delay += 14500, [myang, casterGuid]()
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*myang, casterGuid))
                    {
                        myang->AI()->Talk(SAY_MYANG_5, player);
                        player->KilledMonsterCredit(NPC_INSTRUCTOR_MYANG);
                    }
                                                                                                                     }); // 2 min 0 sec (end)
            }

            static void SpawnWave(Creature* myang, uint64 casterguid, uint8 combatantCount, uint8 bruteCount)
            {
                Player* player = ObjectAccessor::GetPlayer(*myang, casterguid);
                if (player && player->IsAlive())
                {
                    for (uint8 j = 0; j < 2; j++)
                    {
                        for (uint8 i = 0; i < (j == 0 ? combatantCount : bruteCount); i++)
                        {
                            Position pos = player->GetRandomNearPosition(15.0f);
                            if (Creature* spawn = myang->SummonCreature(j == 0 ? NPC_RUMPUS_COMBATANT : NPC_RUMPUS_BRUTE, pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000ms))
                            {
                                spawn->setFaction(14); // hack, with default faction npcs cant be attackable
                                spawn->AI()->AttackStart(player);
                            }
                        }
                    }
                }
                else
                {
                    myang->m_Events.KillAllEvents(true);

                    std::list<Creature*> spawns;
                    GetCreatureListWithEntryInGrid(spawns, myang, NPC_RUMPUS_COMBATANT, 100.0f);
                    GetCreatureListWithEntryInGrid(spawns, myang, NPC_RUMPUS_BRUTE, 100.0f);
                    for (auto&& spawn : spawns)
                        spawn->DespawnOrUnsummon();
                }
            }

            void Register() OVERRIDE
            {
                AfterHit += SpellHitFn(spell_q29637_SpellScript::HandleApplyAura);
            }
        };

        class spell_q29637_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_q29637_AuraScript)

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();

                Player* caster = GetCaster()->ToPlayer();
                Creature* myang = caster->FindNearestCreature(NPC_INSTRUCTOR_MYANG, 100.0f);
                if (!myang)
                    return;

                myang->m_Events.KillAllEvents(true);

                std::list<Creature*> spawns;
                GetCreatureListWithEntryInGrid(spawns, myang, NPC_RUMPUS_COMBATANT, 100.0f);
                GetCreatureListWithEntryInGrid(spawns, myang, NPC_RUMPUS_BRUTE, 100.0f);
                for (auto&& spawn : spawns)
                {
                    if (removeMode == AURA_REMOVE_BY_EXPIRE)
                    {
                        spawn->setFaction(spawn->GetCreatureTemplate()->faction_A); // hack, see above
                        spawn->AttackStop();
                        spawn->SetReactState(REACT_PASSIVE);
                        spawn->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                        uint32 delay = 0;
                        spawn->m_Events.Schedule(delay += 3000, [spawn]() { spawn->GetMotionMaster()->MovePoint(0, rumpusEndPoint, true); });
                    }
                    else
                        spawn->DespawnOrUnsummon();
                }
            }

            void Register() OVERRIDE
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_q29637_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_q29637_SpellScript();
        }

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_q29637_AuraScript();
        }
};

class npc_rumpus_spawn : public CreatureScript
{
    public:
        npc_rumpus_spawn() : CreatureScript("npc_rumpus_spawn") { }

        struct npc_rumpus_spawnAI : public ScriptedAI
        {
            npc_rumpus_spawnAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE { }

            void DamageTaken(Unit* /*who*/, uint32& damage) OVERRIDE
            {
                if (damage >= me->GetHealth())
                {
                    damage = 0;

                    me->setFaction(me->GetCreatureTemplate()->faction_A); // hack, see above
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);

                    uint32 delay = 0;
                    me->m_Events.Schedule(delay += 3000, [this]() { me->GetMotionMaster()->MovePoint(0, rumpusEndPoint, true); });
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE && pointId == 0)
                    me->DespawnOrUnsummon(3000);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_rumpus_spawnAI(creature);
        }
};

enum q29552
{
    QUEST_CRITICAL_CONDITION      = 29552,

    NPC_WOUNDED_PILOT             = 61492,
    NPC_TERRIBLY_WOUNDED_PILOT    = 66564,
    NPC_CRITICALLY_WOUNDED_PILOT  = 66565,
    NPC_MORTALLY_WOUNDED_PILOT    = 66567,
    NPC_Q29552_CREDIT             = 54929,

    SPELL_WOUNDED_HEALING_TRACKER = 119550,
    SPELL_PERMANENT_FEIGN_DEATH   = 130865,
    SPELL_WOUNDED                 = 130851,
    SPELL_TERRIBLY_WOUNDED        = 130852,
    SPELL_CRITICALLY_WOUNDED      = 130854,
    SPELL_MORTALLY_WOUNDED        = 130855,
};

class npc_wounded_pilot : public CreatureScript
{
    public:
        npc_wounded_pilot() : CreatureScript("npc_wounded_pilot") { }

        struct npc_wounded_pilotAI : public ScriptedAI
        {
            npc_wounded_pilotAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->AddAura(SPELL_WOUNDED_HEALING_TRACKER, me);
                me->AddAura(SPELL_PERMANENT_FEIGN_DEATH, me);

                me->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_FEIGN_DEATH);
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->m_Events.KillAllEvents(true);

                uint32 delay = 0;
                switch (me->GetEntry())
                {
                    case NPC_WOUNDED_PILOT:
                        me->CastSpell(me, SPELL_WOUNDED);
                        me->SetHealth(me->CountPctFromMaxHealth(me->GetAura(SPELL_WOUNDED)->GetSpellInfo()->Effects[EFFECT_0].BasePoints));
                        for (uint8 i = 0; i < 3; i++)
                            me->m_Events.Schedule(delay += 20000, [this, i]() { me->DealDamage(me, me->CountPctFromMaxHealth(i == 2 ? 26 : 25)); });
                        break;
                    case NPC_TERRIBLY_WOUNDED_PILOT:
                        me->CastSpell(me, SPELL_TERRIBLY_WOUNDED);
                        me->SetHealth(me->CountPctFromMaxHealth(me->GetAura(SPELL_TERRIBLY_WOUNDED)->GetSpellInfo()->Effects[EFFECT_0].BasePoints));
                        for (uint8 i = 0; i < 5; i++)
                            me->m_Events.Schedule(delay += 12000, [this, i]() { me->DealDamage(me, me->CountPctFromMaxHealth(i == 4 ? 11 : 10)); });
                        break;
                    case NPC_CRITICALLY_WOUNDED_PILOT:
                        me->CastSpell(me, SPELL_CRITICALLY_WOUNDED);
                        me->SetHealth(me->CountPctFromMaxHealth(me->GetAura(SPELL_CRITICALLY_WOUNDED)->GetSpellInfo()->Effects[EFFECT_0].BasePoints));
                        for (uint8 i = 0; i < 5; i++)
                            me->m_Events.Schedule(delay += 12000, [this, i]() { me->DealDamage(me, me->CountPctFromMaxHealth(i == 4 ? 6 : 5)); });
                        break;
                    case NPC_MORTALLY_WOUNDED_PILOT:
                        me->CastSpell(me, SPELL_MORTALLY_WOUNDED);
                        me->SetHealth(me->CountPctFromMaxHealth(me->GetAura(SPELL_MORTALLY_WOUNDED)->GetSpellInfo()->Effects[EFFECT_0].BasePoints));
                        me->m_Events.Schedule(delay += 20000, [this]() { me->DealDamage(me, me->CountPctFromMaxHealth(1)); });
                        break;
                }
            }

            void HealReceived(Unit* healer, uint32& heal) OVERRIDE
            {
                if (healer->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    if (healer->ToPlayer()->GetQuestStatus(QUEST_CRITICAL_CONDITION) != QUEST_STATUS_INCOMPLETE)
                    {
                        heal = 0;
                        return;
                    }

                    if (me->HealthAbovePctHealed(100, heal) && me->GetHealth() < me->GetMaxHealth())
                    {
                        healer->ToPlayer()->KilledMonsterCredit(NPC_Q29552_CREDIT);

                        me->RemoveAllAuras();
                        me->RemoveFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_FEIGN_DEATH);
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                        me->AI()->Talk(0, healer);

                        uint32 delay = 0;
                        me->m_Events.KillAllEvents(true);
                        me->m_Events.Schedule(delay += 5000, [this]()
                        {
                            Position pos = me->GetRandomNearPosition(15.0f);
                            me->GetMotionMaster()->MovePoint(0, pos, true);
                        });
                    }
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE && pointId == 0)
                    me->DespawnOrUnsummon(3000);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_wounded_pilotAI(creature);
        }
};

// Instructor Bladewind 58420
class npc_instructor_bladewind : public CreatureScript
{
    public:
        npc_instructor_bladewind() : CreatureScript("npc_instructor_bladewind") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                player->GetSceneMgr().PlayScene(218);

            player->CLOSE_GOSSIP_MENU();

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(QUEST_GROW_LIKE_WEEDS) != QUEST_STATUS_INCOMPLETE)
                return false;

            player->ADD_GOSSIP_ITEM_DB(player->GetDefaultGossipMenuForSource(creature), 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
        {
            if (player->GetQuestStatus(QUEST_RING_ROUND_UP) != QUEST_STATUS_INCOMPLETE)
                return false;

            creature->AI()->Talk(TALK_INTRO);

            if (TempSummon* serpent = player->SummonCreature(NPC_GOLDEN_CLOUD_SERPENT, MySerpentRidingPath[0], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, Milliseconds(60 * IN_MILLISECONDS)))
            {
                serpent->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                Movement::MoveSplineInit init(serpent);
                for (auto itr : MySerpentRidingPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetSmooth();
                init.SetUncompressed();
                init.Launch();

                uint32 delay = 0;
                serpent->m_Events.Schedule(delay += serpent->GetSplineDuration(), 1, [serpent]()
                {
                    serpent->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    serpent->OverrideInhabitType(INHABIT_GROUND);
                    serpent->UpdateMovementFlags();
                });

            }

            return false;
        }
};

// Instructor Windblade 58572
class npc_instructor_windblade_cutscene : public CreatureScript
{
    public:
        npc_instructor_windblade_cutscene() : CreatureScript("npc_instructor_windblade_cutscene") { }

        struct npc_instructor_windblade_cutsceneAI : public ScriptedAI
        {
            npc_instructor_windblade_cutsceneAI(Creature* creature) : ScriptedAI(creature) { }

            uint64 SummonerGUID;

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                SummonerGUID = summoner->GetGUID();
                me->SetPhaseMask(2, true);
                me->DespawnOrUnsummon(23 * IN_MILLISECONDS);

                uint32 delay = 0;
                me->m_Events.Schedule(delay += 2000, 1, [this]()
                {
                    if (Unit* pSummoner = ObjectAccessor::GetUnit(*me, SummonerGUID))
                        Talk(TALK_INTRO, pSummoner);

                    if (TempSummon* serpent = me->SummonCreature(NPC_GOLDEN_CLOUD_SERPENT, MySerpentPath[0], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, Milliseconds(21 * IN_MILLISECONDS)))
                    {
                        serpent->SetPhaseMask(2, true);

                        Movement::MoveSplineInit init(serpent);
                        for (auto itr : MySerpentPath)
                            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                        init.SetSmooth();
                        init.SetUncompressed();
                        init.Launch();
                    }
                });
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_instructor_windblade_cutsceneAI(creature);
        }
};

// Cloud Ring 58423
class npc_cloud_ring_denrarium : public CreatureScript
{
    public:
        npc_cloud_ring_denrarium() : CreatureScript("npc_cloud_ring_denrarium") { }

        struct npc_cloud_ring_denrariumAI : public ScriptedAI
        {
            npc_cloud_ring_denrariumAI(Creature* creature) : ScriptedAI(creature) { }

            void InitializeAI() OVERRIDE
            {
                uint32 delay = 0;
                me->m_Events.Schedule(delay += 1000, 1, [this]()
                {
                    me->SetDisplayId(16946);
                    DoCast(me, SPELL_CLOUD_RING_VISUAL);
                    DoCast(me, SPELL_CLOUD_RING_AREATRIGGER);
                });
            }

        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_cloud_ring_denrariumAI(creature);
        }
};

// Sam the Wise 68128
struct npc_sam_the_wise : public customCreatureAI
{
    npc_sam_the_wise(Creature* creature) : customCreatureAI(creature) { }

    bool hasTriggered;
    bool hasDefeat;

    void Reset() OVERRIDE
    {
        events.Reset();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);
        hasTriggered = false;
        hasDefeat = false;
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (!hasTriggered)
        {
            hasTriggered = true;
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);
        }
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (hasDefeat)
            damage = 0;

        if ((HealthBelowPct(5) || damage >= me->GetHealth()) && !hasDefeat)
        {
            damage = 0;
            hasDefeat = true;
            events.Reset();
            me->PrepareChanneledCast(me->GetOrientation());
            
            // unk 122
            std::list<Player*> targets;
            GetPlayerListInGrid(targets, me, 50.0f);

            for (auto&& itr : targets)
                itr->CastSpell(itr, SPELL_DEFEAT_METAL_BREWMASTER, true);

            me->DespawnOrUnsummon(3 * IN_MILLISECONDS);

            // Summon Keg
            me->SummonGameObject(216362, 2060.77f, -902.95f, 516.55f, 3.14f, { }, 60);
        }
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_KEG_TOSS, 5 * IN_MILLISECONDS);
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
            ExecuteTargetEvent(SPELL_KEG_OF_FELLOWSHIP, 11500, EVENT_KEG_TOSS, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// General Bunny DLA 65743
struct npc_plight_general_bunny : public customCreatureAI
{
    npc_plight_general_bunny(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        // In case
        if (me->GetMapId() != 870)
            return;

        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            std::list<Player*> targets;
            GetPlayerListInGrid(targets, me, 40.0f);

            for (auto&& itr : targets)
                if (itr->GetQuestStatus(QUEST_SPIRITES_PLIGHT) == QUEST_STATUS_INCOMPLETE)
                    itr->KilledMonsterCredit(NPC_MOGU_RUINS_CREDIT);

            context.Repeat(Milliseconds(1500));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Wild Prowler 56653
struct npc_jf_wild_prowler : public customCreatureAI
{
    npc_jf_wild_prowler(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        DoCast(me, SPELL_PROWL);
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        me->RemoveAurasDueToSpell(SPELL_PROWL);
        events.ScheduleEvent(EVENT_FEROCIOUS_CLAW, 3 * IN_MILLISECONDS);
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
            ExecuteTargetEvent(SPELL_FEROCIOUS_CLAW, urand(7 * IN_MILLISECONDS, 9 * IN_MILLISECONDS), EVENT_FEROCIOUS_CLAW, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Wild Stalker 56683
struct npc_jf_wild_stalker : public customCreatureAI
{
    npc_jf_wild_stalker(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_POUNCE, 500);
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
            ExecuteTargetEvent(SPELL_POUNCE, urand(4 * IN_MILLISECONDS,  5 * IN_MILLISECONDS), EVENT_POUNCE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

enum choTypes
{
    QUEST_PEERING_INTO_THE_PAST = 31152,
    NPC_CHO_PEERING_INTO_PAST   = 63307,
    NPC_CHO_ESCORT_BUNNY        = 63305,
    SPELL_GREAT_BLACK_TURTLE    = 127295,
};

const Position peeringPoint       = { 2056.32f, -710.23f, 354.46f, 0.0f };
const Position peeringStonePoint  = { 2137.39f, -561.47f, 391.45f, 0.0f };
const Position peeringCenterPoint = { 2137.19f, -572.76f, 391.45f, 0.0f };
const Position peeringPath[12] =
{
    { 2045.17f, -720.93f, 353.33f, 0.0f },
    { 2030.78f, -742.55f, 346.36f, 0.0f },
    { 2049.20f, -750.36f, 350.19f, 0.0f },
    { 2074.65f, -753.58f, 357.92f, 0.0f },
    { 2102.10f, -741.96f, 364.83f, 0.0f },
    { 2120.04f, -728.94f, 364.63f, 0.0f },
    { 2135.04f, -695.76f, 363.00f, 0.0f },
    { 2151.62f, -675.04f, 364.75f, 0.0f },
    { 2177.04f, -660.55f, 368.26f, 0.0f },
    { 2182.10f, -642.13f, 371.53f, 0.0f },
    { 2162.28f, -608.32f, 385.98f, 0.0f },
    { 2146.39f, -582.56f, 391.44f, 0.0f },
};

// Lorewalker Cho 63217
class npc_lorewalker_cho_stones_could_speak : public CreatureScript
{
    public:
        npc_lorewalker_cho_stones_could_speak() : CreatureScript("npc_lorewalker_cho_stones_could_speak") { }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
        {
            if (quest->GetQuestId() == QUEST_PEERING_INTO_THE_PAST)
                player->SummonCreature(NPC_CHO_PEERING_INTO_PAST, *creature, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, Milliseconds(120 * IN_MILLISECONDS));
    
            return false;
        }

        struct npc_lorewalker_cho_stones_could_speakAI : public customCreatureAI
        {
            npc_lorewalker_cho_stones_could_speakAI(Creature* creature) : customCreatureAI(creature) { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_lorewalker_cho_stones_could_speakAI(creature);
        }
};

// Lorewalker Cho 63307
struct npc_lorewalker_cho_peering_into_past : public customCreatureAI
{
    npc_lorewalker_cho_peering_into_past(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    uint64 summonerGUID;
    uint32 delay;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());
        summonerGUID = summoner->GetGUID();
        delay = 1000;

        scheduler
            .Schedule(Milliseconds(delay), [this](TaskContext context)
        {
            Talk(TALK_INTRO);
            me->SetWalk(true);
            me->GetMotionMaster()->MovePoint(0, peeringPoint);

            delay = me->GetSplineDuration();
            scheduler
                .Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                DoCast(me, SPELL_GREAT_BLACK_TURTLE);
            });

            scheduler
                .Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
            {
                me->StopMoving();

                Movement::MoveSplineInit init(me);
                for (auto&& itr : peeringPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetUncompressed();
                init.SetWalk(true);
                init.SetVelocity(6.5f);
                init.Launch();

                scheduler
                    .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                {
                    // We are Reach this!
                    me->RemoveAurasDueToSpell(SPELL_GREAT_BLACK_TURTLE);
                    delay = 1550;

                    scheduler
                        .Schedule(Milliseconds(delay), [this](TaskContext context)
                    {
                        if (Player* owner = ObjectAccessor::GetPlayer(*me, summonerGUID))
                            me->SetFacingToObject(owner);

                        Talk(TALK_SPECIAL_3);
                    });

                    scheduler
                        .Schedule(Milliseconds(delay += 5500), [this](TaskContext context)
                    {
                        // Walk to stone
                        me->SetWalk(true);
                        me->GetMotionMaster()->MovePoint(0, peeringStonePoint);
                    });

                    scheduler
                        .Schedule(Milliseconds(delay += 13400), [this](TaskContext context)
                    {
                        if (Player* owner = ObjectAccessor::GetPlayer(*me, summonerGUID))
                            me->SetFacingToObject(owner);

                        Talk(TALK_SPECIAL_4);
                    });

                    scheduler
                        .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
                    {
                        // Walk to center
                        me->GetMotionMaster()->MovePoint(0, peeringCenterPoint);
                    });

                    scheduler
                        .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
                    {
                        if (Player* owner = ObjectAccessor::GetPlayer(*me, summonerGUID))
                            me->SetFacingToObject(owner);

                        Talk(TALK_SPECIAL_5);
                    });

                    scheduler
                        .Schedule(Milliseconds(delay += 10000), [this](TaskContext context)
                    {
                        if (Player* owner = ObjectAccessor::GetPlayer(*me, summonerGUID))
                            owner->KilledMonsterCredit(NPC_CHO_ESCORT_BUNNY);

                        me->DespawnOrUnsummon();
                    });
                });
            });

            scheduler
                .Schedule(Milliseconds(delay += 4400), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_1);
            });

            scheduler
                .Schedule(Milliseconds(delay += 8560), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_2);
            });
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

enum anduinTypes
{
    SPELL_SUMMON_ANDUIN        = 106176,
    SPELL_SUMMON_REN_WHITEPAW  = 106177, // triggered
    SPELL_SUMMON_LINA_WHITEPAW = 106178, // triggered
    SPELL_SUMMON_SULLY         = 106179,
    SPELL_SUMMON_KEARLEN       = 106180, // triggered
    SPELL_MIND_CONTROL         = 106185,

    NPC_PRINCE_ANDUIN_CREDIT   = 56485,
    NPC_SULLY                  = 56488,
    NPC_REN_WHITEPAW           = 56486,
    NPC_LINA_WHITEPAW          = 56487,
    NPC_KAERLEN                = 54620,

    ACTION_MEET_SALLY          = 0,

    QUEST_ANDUIN_DECISION      = 29901,
};

const Position anduinLeavePath[4] =
{
    { -36.11f, -3144.30f, 86.164f, 0.0f },
    { -33.77f, -3141.67f, 85.574f, 0.0f },
    { -33.10f, -3136.49f, 82.485f, 0.0f },
    { -32.61f, -3125.35f, 74.688f, 0.0f },
};

// Lorewalker Cho 63217
class npc_prince_anduin_decision_questgiver : public CreatureScript
{
    public:
        npc_prince_anduin_decision_questgiver() : CreatureScript("npc_prince_anduin_decision_questgiver") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                player->CastSpell(player, SPELL_SUMMON_ANDUIN, true);

            player->CLOSE_GOSSIP_MENU();

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(QUEST_ANDUIN_DECISION) != QUEST_STATUS_INCOMPLETE || creature->GetPositionZ() < 85.0f)
                return false;

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "It is time to go home, Prince Anduin.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1); // miss gossip_menu_id
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        struct npc_prince_anduin_decision_questgiverAI : public customCreatureAI
        {
            npc_prince_anduin_decision_questgiverAI(Creature* creature) : customCreatureAI(creature) { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_prince_anduin_decision_questgiverAI(creature);
        }
};

// Prince Anduin 56485
struct npc_prince_anduin_decision : public customCreatureAI
{
    npc_prince_anduin_decision(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    uint64 summonerGUID;
    uint32 delay;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());
        summonerGUID = summoner->GetGUID();
        delay = 1000;

        // Credit player here
        if (summoner->ToPlayer())
            summoner->ToPlayer()->KilledMonsterCredit(NPC_PRINCE_ANDUIN_CREDIT);

        scheduler
            .Schedule(Milliseconds(delay), [this](TaskContext context)
        {
            Talk(TALK_INTRO);
        });

        scheduler
            .Schedule(Milliseconds(delay += 3820), [this](TaskContext context)
        {
            Talk(TALK_SPECIAL_1);
        });

        scheduler
            .Schedule(Milliseconds(delay += 7000), [this](TaskContext context)
        {
            // Summon Sally & Kearlen
            if (Player* target = ObjectAccessor::GetPlayer(*me, summonerGUID))
                target->CastSpell(target, SPELL_SUMMON_SULLY, true);
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_MEET_SALLY)
        {
            delay = 2000;

            scheduler
                .Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_2);
            });

            scheduler
                .Schedule(Milliseconds(delay += 1500), [this](TaskContext context)
            {
                // Mind controll to Sally
                if (Creature* sally = ObjectAccessor::GetCreature(*me, getCreatureGuidByPlayer(NPC_SULLY)))
                {
                    me->SetFacingToObject(sally);
                    DoCast(me, SPELL_MIND_CONTROL);
                    me->AddAura(SPELL_MIND_CONTROL, sally);
                }
            });

            scheduler
                .Schedule(Milliseconds(delay += 3900), [this](TaskContext context)
            {
                if (Creature* sally = ObjectAccessor::GetCreature(*me, getCreatureGuidByPlayer(NPC_SULLY)))
                    sally->AI()->Talk(TALK_SPECIAL_2);
            });

            scheduler
                .Schedule(Milliseconds(delay += 7000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_3);
                me->InterruptNonMeleeSpells(false, SPELL_MIND_CONTROL);
                me->RemoveAurasDueToSpell(SPELL_MIND_CONTROL);
            });

            scheduler
                .Schedule(Milliseconds(delay += 1800), [this](TaskContext context)
            {
                // Move out with pandarens
                Movement::MoveSplineInit init(me);
                for (auto&& itr : anduinLeavePath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetUncompressed();
                init.Launch();
                me->DespawnOrUnsummon(me->GetSplineDuration());

                scheduler
                    .Schedule(Milliseconds(1000), [this](TaskContext context)
                {
                    if (Creature* ren = ObjectAccessor::GetCreature(*me, getCreatureGuidByPlayer(NPC_REN_WHITEPAW)))
                    {
                        Movement::MoveSplineInit init(ren);
                        for (auto&& itr : anduinLeavePath)
                            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                        init.SetUncompressed();
                        init.Launch();
                        ren->DespawnOrUnsummon(ren->GetSplineDuration());
                    }
                });

                scheduler
                    .Schedule(Milliseconds(2000), [this](TaskContext context)
                {
                    if (Creature* lina = ObjectAccessor::GetCreature(*me, getCreatureGuidByPlayer(NPC_LINA_WHITEPAW)))
                    {
                        Movement::MoveSplineInit init(lina);
                        for (auto&& itr : anduinLeavePath)
                            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                        init.SetUncompressed();
                        init.Launch();
                        lina->DespawnOrUnsummon(lina->GetSplineDuration());
                    }
                });


                // Send controll to Sally
                if (Creature* sally = ObjectAccessor::GetCreature(*me, getCreatureGuidByPlayer(NPC_SULLY)))
                    sally->AI()->DoAction(ACTION_MEET_SALLY);
            });
        }
    }

    private:
        uint64 getCreatureGuidByPlayer(uint32 creature_id)
        {
            if (Player* target = ObjectAccessor::GetPlayer(*me, summonerGUID))
            {
                std::list<TempSummon*> summonedCreatures;
                target->GetSummons(summonedCreatures, creature_id);

                if (!summonedCreatures.empty())
                    return summonedCreatures.front()->GetGUID();
            }

            return 0;
        }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Ren Whitepaw 56486, Lina Whitepaw 56487, Sally 56488, Kearlen 54620
struct npc_prince_anduin_decision_helpers : public customCreatureAI
{
    npc_prince_anduin_decision_helpers(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    uint64 summonerGUID;
    uint32 delay;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());
        summonerGUID = summoner->GetGUID();

        if (me->GetEntry() == NPC_SULLY || me->GetEntry() == NPC_KAERLEN)
        {
            float x = me->GetPositionX() + 11.5f * cos(me->GetOrientation());
            float y = me->GetPositionY() + 11.5f * sin(me->GetOrientation());
            float z = me->GetPositionZ();
            me->UpdateAllowedPositionZ(x, y, z);
            me->SetWalk(true);
            me->GetMotionMaster()->MovePoint(0, x, y, z);

            if (me->GetEntry() == NPC_SULLY)
            {
                delay = me->GetSplineDuration();
                scheduler
                    .Schedule(Milliseconds(delay), [this](TaskContext context)
                {
                    Talk(TALK_INTRO);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3300), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_1);

                    // Send controll to Prince
                    if (Creature* anduin = ObjectAccessor::GetCreature(*me, getCreatureGuidByPlayer(NPC_PRINCE_ANDUIN_CREDIT)))
                        anduin->AI()->DoAction(ACTION_MEET_SALLY);
                });
            }
        }
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_MEET_SALLY)
        {
            delay = 4300;
            scheduler
                .Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                if (Creature* kaerlean = ObjectAccessor::GetCreature(*me, getCreatureGuidByPlayer(NPC_KAERLEN)))
                {
                    kaerlean->SetFacingToObject(me);
                    kaerlean->AI()->Talk(TALK_INTRO);
                }
            });

            scheduler
                .Schedule(Milliseconds(delay += 2800), [this](TaskContext context)
            {
                me->RemoveAurasDueToSpell(SPELL_MIND_CONTROL);
                Talk(TALK_SPECIAL_3);
            });

            scheduler
                .Schedule(Milliseconds(delay += 3450), [this](TaskContext context)
            {
                if (Creature* kaerlean = ObjectAccessor::GetCreature(*me, getCreatureGuidByPlayer(NPC_KAERLEN)))
                {
                    me->SetFacingToObject(kaerlean);
                    kaerlean->AI()->Talk(TALK_SPECIAL_1);
                }
            });

            scheduler
                .Schedule(Milliseconds(delay += 4800), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_4);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
            {
                if (Creature* kaerlean = ObjectAccessor::GetCreature(*me, getCreatureGuidByPlayer(NPC_KAERLEN)))
                    kaerlean->AI()->Talk(TALK_SPECIAL_2);
            });

            scheduler
                .Schedule(Milliseconds(delay += 2600), [this](TaskContext context)
            {
                if (Creature* kaerlean = ObjectAccessor::GetCreature(*me, getCreatureGuidByPlayer(NPC_KAERLEN)))
                {
                    kaerlean->GetMotionMaster()->MovePoint(0, kaerlean->GetHomePosition().GetPositionX(), kaerlean->GetHomePosition().GetPositionY(), kaerlean->GetHomePosition().GetPositionZ());
                    kaerlean->DespawnOrUnsummon(kaerlean->GetSplineDuration());
                }
            });

            scheduler
                .Schedule(Milliseconds(delay += 2500), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_5);
                me->GetMotionMaster()->MovePoint(0, me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY(), me->GetHomePosition().GetPositionZ());
                me->DespawnOrUnsummon(me->GetSplineDuration());
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }

    private:
        uint64 getCreatureGuidByPlayer(uint32 creature_id)
        {
            if (Player* target = ObjectAccessor::GetPlayer(*me, summonerGUID))
            {
                std::list<TempSummon*> summonedCreatures;
                target->GetSummons(summonedCreatures, creature_id);

                if (!summonedCreatures.empty())
                    return summonedCreatures.front()->GetGUID();
            }

            return 0;
        }
};

// Boiling Cauldron 55086
struct npc_jade_forest_boiling_cauldron : public customCreatureAI
{
    npc_jade_forest_boiling_cauldron(Creature* creature) : customCreatureAI(creature) { }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        DoCast(me, SPELL_BOILING_CAULDRON_VISUAL, true);
        DoCast(me, SPELL_CAULDRON_ROPE, true);

        me->GetMotionMaster()->MoveFollow(summoner, 4.0f, me->GetAngle(summoner));
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

// Keg of Metal Brew 216362
class go_jade_forest_keg_of_metal_brew : public GameObjectScript
{
    public:
        go_jade_forest_keg_of_metal_brew() : GameObjectScript("go_jade_forest_keg_of_metal_brew") { }

        void OnLootStateChanged(GameObject* go, uint32 state, Unit* unit) OVERRIDE
        {
            if (unit && unit->ToPlayer())
                unit->ToPlayer()->AddItem(92557, 1);
        }
};

// Smoked Blade 125633
class spell_jade_forest_smoked_blade : public SpellScript
{
    PrepareSpellScript(spell_jade_forest_smoked_blade);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(caster, SPELL_SMOKED_BLADE_REMOVE, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_jade_forest_smoked_blade::HandleEffectHit, EFFECT_0, SPELL_EFFECT_DAMAGE_FROM_MAX_HEALTH_PCT);
    }
};

// Summon Metal Brewmaster 134254
class spell_jade_forest_summon_metal_brewmaster : public SpellScript
{
    PrepareSpellScript(spell_jade_forest_summon_metal_brewmaster);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        if (Creature* target = GetHitCreature())
            target->AI()->DoAction(0);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_jade_forest_summon_metal_brewmaster::HandleEffectHit, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Summon Boiling Cauldron 102879
class spell_summon_boiling_cauldron : public SpellScript
{
    PrepareSpellScript(spell_summon_boiling_cauldron);

    SpellCastResult CheckCast()
    {
        if (GetCaster() && GetCaster()->HasAura(SPELL_CAULDRON_ROPE))
            return SpellCastResult::SPELL_FAILED_UNKNOWN;

        return SpellCastResult::SPELL_CAST_OK;
    }

    void Register() OVERRIDE
    {
        OnCheckCast += SpellCheckCastFn(spell_summon_boiling_cauldron::CheckCast);
    }
};

// Cho Teapot Master 123246
class spell_cho_teapot_master : public AuraScript
{
    PrepareAuraScript(spell_cho_teapot_master);

    void HandleOnApply(AuraEffect const* aureff, AuraEffectHandleModes /*mode*/)
    {
        if (Player* owner = GetOwner()->ToPlayer())
            if (owner->GetQuestStatus(QUEST_STAY_AND_WHILE) == QUEST_STATUS_INCOMPLETE)
                owner->CastSpell(owner, SPELL_STAY_AND_WHILE, true);
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_cho_teapot_master::HandleOnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

const std::map<uint32, std::array<uint32, 2>> invStoneType =
{
    { SPELL_MONOLITH_01_MASTER, { NPC_STONE_1_CREDIT, 0} },
    { SPELL_MONOLITH_02_MASTER, { NPC_STONE_2_CREDIT, 2 } },
    { SPELL_MONOLITH_03_MASTER, { NPC_STONE_3_CREDIT, 3 } },
};

// Lorewalker Cho Monolith 123589, 123590, 123544
class spell_jade_forest_lorewalker_cho_monolith : public SpellScript
{
    PrepareSpellScript(spell_jade_forest_lorewalker_cho_monolith);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->GetQuestStatus(QUEST_IF_STONES_COULD_SPEAK) != QUEST_STATUS_INCOMPLETE)
                return;

            if (Creature* lorewalker = GetClosestCreatureWithEntry(caster, NPC_LOREWALKER_CHO_STONES, 100.0f, true))
            {
                auto key = invStoneType.find(GetSpellInfo()->Id);

                lorewalker->AI()->Talk(key->second[1]);
                caster->KilledMonsterCredit(key->second[0]);
            }
        }
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_jade_forest_lorewalker_cho_monolith::HandleEffectHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 585. Summoned by 134262 - Feel the Flavor
class sat_jade_forest_feel_the_flavor : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_FEEL_THE_FLAVOR_EFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_FEEL_THE_FLAVOR_EFF);
    }
};

// packageID 218 (spell not found)
class scene_grow_like_weeds : public SceneScript
{
    public:
        scene_grow_like_weeds() : SceneScript("scene_grow_like_weeds") { }

        void OnSceneStart(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            player->SetPhaseMask(2, true);
            player->SetFacingTo(5.4f);
            player->CastSpell(player, SPELL_SUMMON_INSTRUCTOR_WINDBLADE, true);
        }

        void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void OnSceneCancel(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void CreditScene(Player* player)
        {
            player->KilledMonsterCredit(NPC_INSTRUCTOR_WINDBLADE);
            player->SetPhaseMask(1, true);
        }
};

// packageID 147 (spell 123312)
class scene_stay_and_while : public SceneScript
{
    public:
        scene_stay_and_while() : SceneScript("scene_stay_and_while") { }

        void OnSceneStart(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            player->SetPhaseMask(2, true); // required here
        }

        void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            player->SetPhaseMask(1, true);
            player->KilledMonsterCredit(NPC_STAY_AND_WHILE_CRED);
        }

        void OnSceneCancel(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            player->SetPhaseMask(1, true);
            player->KilledMonsterCredit(NPC_STAY_AND_WHILE_CRED);
        }
};

class at_cave_of_words : public AreaTriggerScript
{
    public:
        at_cave_of_words() : AreaTriggerScript("at_cave_of_words") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            if (!player->isDead() && player->GetQuestStatus(29822) == QUEST_STATUS_COMPLETE)
            {
                if (Creature* lorewalker_cho = player->FindNearestCreature(55768, 20.0f))
                {
                    lorewalker_cho->AI()->Talk(0, player);
                    return true;
                }
            }
            return false;
        }
};

// Areatrigger 6982
class AreaTrigger_at_groundkeepers_wu : public AreaTriggerScript
{
    public:
        AreaTrigger_at_groundkeepers_wu() : AreaTriggerScript("AreaTrigger_at_groundkeepers_wu") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) OVERRIDE
        {
            if (Aura* cauldron = player->GetAura(SPELL_CAULDRON_ROPE))
            {
                if (cauldron->GetCaster() && cauldron->GetCaster()->ToCreature())
                    cauldron->GetCaster()->ToCreature()->DespawnOrUnsummon();

                player->RemoveAurasDueToSpell(SPELL_CAULDRON_ROPE);
                player->KilledMonsterCredit(NPC_CAULDRON_CREDIT);

                if (Creature* wu = player->FindNearestCreature(NPC_GROUNDSKEEPER_WU, 20.0f, true))
                    wu->AI()->Talk(TALK_INTRO, player);
            }

            return false;
        }
};

// Areatrigger 6983
class AreaTrigger_at_mother_wu : public AreaTriggerScript
{
    public:
        AreaTrigger_at_mother_wu() : AreaTriggerScript("AreaTrigger_at_mother_wu") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) OVERRIDE
        {
            if (player->GetQuestStatus(QUEST_A_STRONG_BACK) == QUEST_STATUS_INCOMPLETE && !player->HasAura(SPELL_CAULDRON_ROPE))
                if (Creature* motherWu = player->FindNearestCreature(NPC_MOTHER_WU, 50.0f, true))
                    motherWu->AI()->Talk(TALK_INTRO);

            return false;
        }
};

// 134. Summoned by 110669 - Cloud Ring
class sat_jade_forest_cloud_ring : public IAreaTriggerOnce
{
    bool CheckTriggering(WorldObject* object) OVERRIDE
    {
        return object && object->ToPlayer() && object->ToPlayer()->IsAlive() && object->ToPlayer()->GetQuestStatus(30144) == QUEST_STATUS_INCOMPLETE;
    }

    void OnTrigger(WorldObject* target) OVERRIDE
    {
        if (Player* itr = target->ToPlayer())
        {
            itr->QuestObjectiveSatisfy(58426, 1);

            if (GetCaster() && GetCaster()->ToCreature())
                GetCaster()->ToCreature()->DespawnOrUnsummon();
        }
    }
};

// 341. Summoned by 122035 - Path of Blossom (not sure that`s from this category - JF)
class sat_jade_forest_path_of_blossom : public IAreaTriggerOnce
{
    bool CheckTriggering(WorldObject* object) OVERRIDE
    {
        return object && object->ToPlayer() && object->ToPlayer()->IsAlive();
    }

    void OnTrigger(WorldObject* target) OVERRIDE
    {
        if (Player* itr = target->ToPlayer())
            if (Unit* caster = GetCaster())
                caster->CastSpell(itr, SPELL_PATH_OF_BLOSSOM_EFF, true);
    }
};

enum mouthwateringBrewData
{
    NPC_BORROWED_BREW_CREDIT        = 56260,

    SPELL_MOUTHWATERING_BREW_LAUNCH = 105583,
    SPELL_MOUTHWATERING_BREW        = 127894,
};

// SceneID 35
class scene_cho_mouthwatering_brew : public SceneScript
{
    public:
        scene_cho_mouthwatering_brew() : SceneScript("scene_cho_mouthwatering_brew") { }

        void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void OnSceneCancel(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void CreditScene(Player* player)
        {
            player->RemoveAurasDueToSpell(SPELL_MOUTHWATERING_BREW_LAUNCH);
            player->RemoveAurasDueToSpell(SPELL_MOUTHWATERING_BREW);
            player->KilledMonsterCredit(NPC_BORROWED_BREW_CREDIT);
        }
};

// Mouthwatering Brew Launcher 105583
class spell_jade_forest_mouthwatering_brew_launcher : public SpellScript
{
    PrepareSpellScript(spell_jade_forest_mouthwatering_brew_launcher);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->CastSpell(caster, SPELL_MOUTHWATERING_BREW, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_jade_forest_mouthwatering_brew_launcher::HandleEffectHit, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

enum finalBlowsTypes
{
    GO_BARRICADE           = 215646,
    GO_BARRICADE_LARGE     = 215647,

    SPELL_EXPLOSION_VISUAL = 71495,

    ACTION_UPD_COLLISION   = 0,
};

// Alliance Barricade 66554, 66555, 66556
struct npc_jade_forest_alliance_barricade : public customCreatureAI
{
    npc_jade_forest_alliance_barricade(Creature* creature) : customCreatureAI(creature) { }

    std::list<uint64> barricadeGUIDS;
    TaskScheduler scheduler;
    bool isRestoring;

    void Reset() OVERRIDE
    {
        isRestoring = false;
        barricadeGUIDS.clear();
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_UPD_COLLISION && !isRestoring) // Restore Collisions
        {
            isRestoring = true;
            barricadeGUIDS.clear();

            // Remove Collisions
            std::list<GameObject*> barricades;
            GetGameObjectListWithEntryInGrid(barricades, me, GO_BARRICADE, 10.0f);
            GetGameObjectListWithEntryInGrid(barricades, me, GO_BARRICADE_LARGE, 10.0f);

            for (auto&& itr : barricades)
            {
                barricadeGUIDS.push_back(itr->GetGUID());
                itr->SetPhaseMask(2, true);
            }

            scheduler
                .Schedule(Seconds(15), [this](TaskContext context)
            {
                for (auto&& itr : barricadeGUIDS)
                    if (GameObject* go = ObjectAccessor::GetGameObject(*me, itr))
                        go->SetPhaseMask(1, true);

                isRestoring = false;
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Nazgrim`s Flare Gun 130871
class spell_jade_forest_nazgrims_flare_gun : public SpellScript
{
    PrepareSpellScript(spell_jade_forest_nazgrims_flare_gun);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        if (Creature* target = GetHitCreature())
        {
            // Visual
            target->CastSpell(target, SPELL_EXPLOSION_VISUAL, true);

            if (Player* caster = GetCaster()->ToPlayer())
                caster->KilledMonsterCredit(target->GetEntry());

            target->AI()->DoAction(ACTION_UPD_COLLISION);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_jade_forest_nazgrims_flare_gun::HandleEffectHit, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

enum playfulSerpentTypes
{
    QUEST_RIDERS_BIND           = 29999,

    NPC_PLAYFUL_GOLD_SERPENT    = 56850,
    NPC_PLAYFUL_EMERALD_SERPENT = 56851,
    NPC_PLAYFUL_CRIMSON_SERPENT = 56852,
    NPC_PLAYFUL_AZURE_SERPENT   = 56853,
};

const std::map<uint32, uint32> coloredSerpentsType =
{
    { NPC_PLAYFUL_GOLD_SERPENT,    56860 },
    { NPC_PLAYFUL_EMERALD_SERPENT, 56859 },
    { NPC_PLAYFUL_CRIMSON_SERPENT, 56858 },
    { NPC_PLAYFUL_AZURE_SERPENT,   56856 },
};

// Playful Colored Serpent 56856, 56858, 56859, 56860
struct npc_jade_forest_playful_colored_serpent : public ScriptedAI
{
    npc_jade_forest_playful_colored_serpent(Creature* creature) : ScriptedAI(creature) { }

    void Reset() OVERRIDE
    {
        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    }

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        if (clicker && clicker->ToPlayer() && clicker->ToPlayer()->GetQuestStatus(QUEST_RIDERS_BIND) == QUEST_STATUS_INCOMPLETE)
            clicker->ToPlayer()->KilledMonsterCredit(coloredSerpentsType.find(me->GetEntry())->second);
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

enum jadeSerpentType
{
    NPC_JADER_SERPENT_CREDIT = 57243,
};

// SceneId 68
class scene_jade_forest_jade_serpent : public SceneScript
{
    public:
        scene_jade_forest_jade_serpent() : SceneScript("scene_jade_forest_jade_serpent") { }

        void OnSceneStart(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            player->SetPhaseMask(2, true); // @Todo should be another, but unk (spell effect has it)
        }

        void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void OnSceneCancel(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void CreditScene(Player* player)
        {
            // @Todo:: Scene not fully complete - on finish player should ride on sepent around statue (script-side part)
            player->KilledMonsterCredit(NPC_JADER_SERPENT_CREDIT);
            player->SetPhaseMask(1, true);
        }
};

enum instantMessagesType
{
    SPELL_SUMMON_CAMERA       = 105971,
    SPELL_REVERSE_SIDE_SEAT_1 = 88885,

    NPC_CAMER_BUNNY     = 56412,
    NPC_IM_CREDIT       = 56403,
};

const Position cameraMove = { 1460.24f, -489.851f, 351.0f, 0.0f };

const Position camerMovePath[4] =
{
    { 1464.49f,  -488.4896f, 351.8663f, 0.0f },
    { 1469.118f, -487.1858f, 351.5573f, 0.0f },
    { 1474.458f, -487.5382f, 350.2336f, 0.0f },
    { 1478.106f, -491.401f,  348.6242f, 0.0f },
};

// Camera Bunny 56412
struct npc_jade_forest_instant_message_camera_bunny : public ScriptedAI
{
    npc_jade_forest_instant_message_camera_bunny(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 summonerGUID;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        // @Todo:: Intro scene with hozens here
        summonerGUID = summoner->GetGUID();
        DoCast(me, SPELL_REVERSE_SIDE_SEAT_1);

        uint32 delay = 2;
        scheduler
            .Schedule(Seconds(delay), [this](TaskContext context)
        {
            me->GetMotionMaster()->MovePoint(0, cameraMove);
        });

        scheduler
            .Schedule(Seconds(delay += 8), [this](TaskContext context)
        {
            Movement::MoveSplineInit init(me);
            for (auto itr : camerMovePath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.SetSmooth();
            init.SetFly();
            init.SetUncompressed();
            init.Launch();
        });

        scheduler
            .Schedule(Seconds(delay += 10), [this](TaskContext context)
        {
            if (Unit* passenger = ObjectAccessor::GetUnit(*me, summonerGUID))
                passenger->ExitVehicle();

            me->DespawnOrUnsummon();
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE 
    {
        scheduler.Update(diff);
    }
};

// Signal Flare Initialize 105960
class spell_jade_forest_signal_flare_initialize : public AuraScript
{
    PrepareAuraScript(spell_jade_forest_signal_flare_initialize);

    void OnAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Player* target = GetOwner()->ToPlayer())
        {
            target->KilledMonsterCredit(NPC_IM_CREDIT);

            // Summon camera & make scene
            target->CastSpell(target, SPELL_SUMMON_CAMERA, true);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_jade_forest_signal_flare_initialize::OnAuraEffectRemove, EFFECT_0, SPELL_AURA_SCREEN_EFFECT, AURA_EFFECT_HANDLE_REAL);
    }
};

// Reverse Cast Ride Seat 1 88885
class spell_reverse_cast_ride_seat_1 : public SpellScript
{
    PrepareSpellScript(spell_reverse_cast_ride_seat_1);

    void HandleEffectHit(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
            target->CastSpell(GetCaster(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_reverse_cast_ride_seat_1::HandleEffectHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

void AddSC_jade_forest()
{
    // Rare mobs
    new npc_kor_nas_nightsavage();
    new npc_mister_ferocious();
    new npc_kraxik_tornado();
    // Elite mobs
    new npc_shadow_of_doubt();
    new npc_akkalar();
    new npc_grookin_outrunner();
    new npc_bamboo_python();
    new npc_lurking_tiger();
    new npc_rakira();
    new npc_ro_shen();
    new npc_sha_reminant();
    new npc_hutia();
    // Standard Mobs
    new npc_pandriarch_windfur();
    new npc_pandriarch_bramblestaff();
    new npc_pandriarch_goldendraft();
    new npc_big_bao();
    new creature_script<npc_silverhorn_stag>("npc_silverhorn_stag");
    new creature_script<npc_forest_huntress>("npc_forest_huntress");
    new creature_script<npc_greenstone_nibbler>("npc_greenstone_nibbler");
    new creature_script<npc_greenstone_gorger>("npc_greenstone_gorger");
    new creature_script<npc_greenwood_thief>("npc_greenwood_thief");
    new creature_script<npc_ivory_starling>("npc_ivory_starling");
    new creature_script<npc_greenwood_trickster>("npc_greenwood_trickster");
    new creature_script<npc_brittle_greenstone_gorger>("npc_brittle_greenstone_gorger");
    new creature_script<npc_shade_of_ling_heartfist>("npc_shade_of_ling_heartfist");
    new creature_script<npc_jade_guardian>("npc_jade_guardian");
    new creature_script<npc_subjugator_gormal>("npc_subjugator_gormal");
    new creature_script<npc_orchard_wasp>("npc_orchard_wasp");
    new creature_script<npc_ink_tipped_crane>("npc_ink_tipped_crane");
    new creature_script<npc_crane_hunter>("npc_crane_hunter");
    new creature_script<npc_morgrinn_crackfang>("npc_morgrinn_crackfang");
    new creature_script<npc_shanze_spiritclaw>("npc_shanze_spiritclaw");
    new creature_script<npc_urobi_the_walker>("npc_urobi_the_walker");
    new creature_script<npc_siftworm>("npc_siftworm");
    new creature_script<npc_honeykeeper>("npc_honeykeeper");
    new creature_script<npc_final_doubt>("npc_final_doubt");
    new creature_script<npc_nurse_shark>("npc_nurse_shark");
    new creature_script<npc_slitherscale_lizard_lord>("npc_slitherscale_lizard_lord");
    new creature_script<npc_shadowfae_trickster>("npc_shadowfae_trickster");
    new creature_script<npc_thunderfist_gorilla>("npc_thunderfist_gorilla");
    // Quest scripts
    new npc_nectarbreeze_farmer();
    new creature_script<npc_windward_hatchling>("npc_windward_hatchling");
    new creature_script<npc_windward_nest_trigger>("npc_windward_nest_trigger");
    new npc_instructor_skythorn();
    new npc_the_pearlfin_situation_q();
    new AreaTrigger_q29586();
    new spell_q30063();
    new spell_q29637();
    new npc_rumpus_spawn();
    new npc_wounded_pilot();
    new npc_instructor_bladewind();
    new npc_instructor_windblade_cutscene();
    new npc_cloud_ring_denrarium();
    new creature_script<npc_sam_the_wise>("npc_sam_the_wise");
    new creature_script<npc_plight_general_bunny>("npc_plight_general_bunny");
    new creature_script<npc_jade_forest_boiling_cauldron>("npc_jade_forest_boiling_cauldron");
    new creature_script<npc_jf_wild_prowler>("npc_jf_wild_prowler");
    new creature_script<npc_jf_wild_stalker>("npc_jf_wild_stalker");
    new npc_lorewalker_cho_stones_could_speak();
    new creature_script<npc_lorewalker_cho_peering_into_past>("npc_lorewalker_cho_peering_into_past");
    new npc_prince_anduin_decision_questgiver();
    new creature_script<npc_prince_anduin_decision>("npc_prince_anduin_decision");
    new creature_script<npc_prince_anduin_decision_helpers>("npc_prince_anduin_decision_helpers");
    new go_jade_forest_keg_of_metal_brew();
    new GenericSpellScriptLoader<spell_jade_forest_smoked_blade>("spell_jade_forest_smoked_blade");
    new GenericSpellScriptLoader<spell_jade_forest_summon_metal_brewmaster>("spell_jade_forest_summon_metal_brewmaster");
    new GenericSpellScriptLoader<spell_summon_boiling_cauldron>("spell_summon_boiling_cauldron");
    new GenericAuraScriptLoader<spell_cho_teapot_master>("spell_cho_teapot_master");
    new GenericSpellScriptLoader<spell_jade_forest_lorewalker_cho_monolith>("spell_jade_forest_lorewalker_cho_monolith");
    new atrigger_script<sat_jade_forest_feel_the_flavor>("sat_jade_forest_feel_the_flavor");
    new scene_grow_like_weeds();
    new scene_stay_and_while();
    new at_cave_of_words();
    new AreaTrigger_at_groundkeepers_wu();
    new AreaTrigger_at_mother_wu();
    new atrigger_script<sat_jade_forest_cloud_ring>("sat_jade_forest_cloud_ring");
    new atrigger_script<sat_jade_forest_path_of_blossom>("sat_jade_forest_path_of_blossom");
    new scene_cho_mouthwatering_brew();
    new creature_script<npc_jade_forest_alliance_barricade>("npc_jade_forest_alliance_barricade");
    new GenericSpellScriptLoader<spell_jade_forest_mouthwatering_brew_launcher>("spell_jade_forest_mouthwatering_brew_launcher");
    new GenericSpellScriptLoader<spell_jade_forest_nazgrims_flare_gun>("spell_jade_forest_nazgrims_flare_gun");
    new creature_script<npc_jade_forest_playful_colored_serpent>("npc_jade_forest_playful_colored_serpent");
    new scene_jade_forest_jade_serpent();
    new creature_script<npc_jade_forest_instant_message_camera_bunny>("npc_jade_forest_instant_message_camera_bunny");
    new GenericAuraScriptLoader<spell_jade_forest_signal_flare_initialize>("spell_jade_forest_signal_flare_initialize");
    new GenericSpellScriptLoader<spell_reverse_cast_ride_seat_1>("spell_reverse_cast_ride_seat_1");
}
