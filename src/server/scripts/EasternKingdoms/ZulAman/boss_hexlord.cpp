/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "zulaman.h"

enum Says
{
    SAY_AGGRO               = 0,
    SAY_PLAYER_KILL         = 1,
    SAY_SPIRIT_BOLTS        = 2,
    SAY_SIPHON_SOUL         = 3,
    SAY_PET_DEATH           = 4,
    SAY_DEATH               = 5
};

enum Spells
{
    SPELL_SPIRIT_BOLTS              = 43383,
    SPELL_DRAIN_POWER               = 44131,
    SPELL_SIPHON_SOUL               = 43501,

    SPELL_FLASH_HEAL                = 43575,
    SPELL_DISPEL_MAGIC              = 43577,

    SPELL_FIREBOLT                  = 43584,

    SPELL_PSYCHIC_WAIL              = 43590,

    SPELL_VENOM_SPIT                = 43579,

    // Druid
    SPELL_DR_THORNS                 = 43420,
    SPELL_DR_LIFEBLOOM              = 43421,
    SPELL_DR_MOONFIRE               = 43545,

    // Hunter
    SPELL_HU_EXPLOSIVE_TRAP         = 43444,
    SPELL_HU_FREEZING_TRAP          = 43447,
    SPELL_HU_SNAKE_TRAP             = 43449,

    // Mage
    SPELL_MG_FIREBALL               = 41383,
    SPELL_MG_FROST_NOVA             = 43426,
    SPELL_MG_ICE_LANCE              = 43427,
    SPELL_MG_FROSTBOLT              = 43428,

    // Paladin
    SPELL_PA_CONSECRATION           = 43429,
    SPELL_PA_AVENGING_WRATH         = 43430,
    SPELL_PA_HOLY_LIGHT             = 43451,

    // Priest
    SPELL_PR_HEAL                   = 41372,
    SPELL_PR_MIND_BLAST             = 41374,
    SPELL_PR_SW_DEATH               = 41375,
    SPELL_PR_PSYCHIC_SCREAM         = 43432,
    SPELL_PR_MIND_CONTROL           = 43550,
    SPELL_PR_PAIN_SUPP              = 44416,

    // Rogue
    SPELL_RO_BLIND                  = 43433,
    SPELL_RO_SLICE_DICE             = 43457,
    SPELL_RO_WOUND_POISON           = 43461,

    // Shaman
    SPELL_SH_CHAIN_LIGHT            = 43435,
    SPELL_SH_FIRE_NOVA              = 43436,
    SPELL_SH_HEALING_WAVE           = 43548,

    // Warlock
    SPELL_WL_CURSE_OF_DOOM          = 43439,
    SPELL_WL_RAIN_OF_FIRE           = 43440,
    SPELL_WL_UNSTABLE_AFFL          = 43522,
    SPELL_WL_UNSTABLE_AFFL_DISPEL   = 43523,

    // Warrior
    SPELL_WR_MORTAL_STRIKE          = 43441,
    SPELL_WR_WHIRLWIND              = 43442,
    SPELL_WR_SPELL_REFLECT          = 43443
};

enum Events
{
    EVENT_SPIRIT_BOLTS          = 1,
    EVENT_SIPHON_SOUL           = 2,
    EVENT_DRAIN_POWER           = 3,
    EVENT_PLAYER_ABILITY        = 4,
    EVENT_PLAYER_ABILITY_OFF    = 5,
};

enum Adds
{
    NPC_ALYSON_ANTILE   = 24240,
    NPC_SLITHER         = 24242,
    NPC_GAZAKROTH       = 24244,
    NPC_DARKHEART       = 24246,
};

Position const addPos[2] =
{
    {109.43f, 922.57f, 33.90f, 1.57f},
    {126.40f, 922.78f, 33.90f, 1.57f}
};

enum AbilityTarget
{
    ABILITY_TARGET_SELF    = 0,
    ABILITY_TARGET_VICTIM  = 1,
    ABILITY_TARGET_ENEMY   = 2,
    ABILITY_TARGET_HEAL    = 3,
    ABILITY_TARGET_BUFF    = 4,
    ABILITY_TARGET_SPECIAL = 5
};

struct PlayerAbilityStruct
{
    uint32 spell;
    AbilityTarget target;
    uint32 cooldown;
};

static PlayerAbilityStruct const PlayerAbility[][3] =
{
    // 1 Warrior
    {
        { SPELL_WR_SPELL_REFLECT,   ABILITY_TARGET_SELF,    10000 },
        { SPELL_WR_WHIRLWIND,       ABILITY_TARGET_SELF,    10000 },
        { SPELL_WR_MORTAL_STRIKE,   ABILITY_TARGET_VICTIM,  6000  }
    },
    // 2 Paladin
    {
        { SPELL_PA_CONSECRATION,    ABILITY_TARGET_SELF,    10000 },
        { SPELL_PA_HOLY_LIGHT,      ABILITY_TARGET_HEAL,    10000 },
        { SPELL_PA_AVENGING_WRATH,  ABILITY_TARGET_SELF,    10000 }
    },
    // 3 Hunter
    {
        { SPELL_HU_EXPLOSIVE_TRAP,  ABILITY_TARGET_SELF,    10000 },
        { SPELL_HU_FREEZING_TRAP,   ABILITY_TARGET_SELF,    10000 },
        { SPELL_HU_SNAKE_TRAP,      ABILITY_TARGET_SELF,    10000 }
    },
    // 4 Rogue
    {
        { SPELL_RO_WOUND_POISON,    ABILITY_TARGET_VICTIM,  3000  },
        { SPELL_RO_SLICE_DICE,      ABILITY_TARGET_SELF,    10000 },
        { SPELL_RO_BLIND,           ABILITY_TARGET_ENEMY,   10000 }
    },
    // 5 Priest
    {
        { SPELL_PR_PAIN_SUPP,       ABILITY_TARGET_HEAL,    10000 },
        { SPELL_PR_HEAL,            ABILITY_TARGET_HEAL,    10000 },
        { SPELL_PR_PSYCHIC_SCREAM,  ABILITY_TARGET_SELF,    10000 }
    },
    // 5* Shadow Priest
    {
        { SPELL_PR_MIND_CONTROL,    ABILITY_TARGET_ENEMY,   15000 },
        { SPELL_PR_MIND_BLAST,      ABILITY_TARGET_ENEMY,   5000  },
        { SPELL_PR_SW_DEATH,        ABILITY_TARGET_ENEMY,   10000 }
    },
    // 7 Shaman
    {
        { SPELL_SH_FIRE_NOVA,       ABILITY_TARGET_SELF,    10000 },
        { SPELL_SH_HEALING_WAVE,    ABILITY_TARGET_HEAL,    10000 },
        { SPELL_SH_CHAIN_LIGHT,     ABILITY_TARGET_ENEMY,   8000  }
    },
    // 8 Mage
    {
        { SPELL_MG_FIREBALL,        ABILITY_TARGET_ENEMY,   5000  },
        { SPELL_MG_FROSTBOLT,       ABILITY_TARGET_ENEMY,   5000  },
        { SPELL_MG_ICE_LANCE,       ABILITY_TARGET_SPECIAL, 2000  }
    },
    // 9 Warlock
    {
        { SPELL_WL_CURSE_OF_DOOM,   ABILITY_TARGET_ENEMY,   10000 },
        { SPELL_WL_RAIN_OF_FIRE,    ABILITY_TARGET_ENEMY,   10000 },
        { SPELL_WL_UNSTABLE_AFFL,   ABILITY_TARGET_ENEMY,   10000 }
    },
    // 11 Druid
    {
        { SPELL_DR_LIFEBLOOM,       ABILITY_TARGET_HEAL,    10000 },
        { SPELL_DR_THORNS,          ABILITY_TARGET_SELF,    10000 },
        { SPELL_DR_MOONFIRE,        ABILITY_TARGET_ENEMY,   8000  }
    }
};


// Index into PlayerAbility for the class of the player the Hex Lord siphons
enum AbilitySets
{
    SET_WARRIOR     = 0,
    SET_PALADIN     = 1,
    SET_HUNTER      = 2,
    SET_ROGUE       = 3,
    SET_PRIEST      = 4,
    SET_SHADOW      = 5,
    SET_SHAMAN      = 6,
    SET_MAGE        = 7,
    SET_WARLOCK     = 8,
    SET_DRUID       = 9
};

uint32 const SHADOWFORM_SPELL = 15473;

class boss_hexlord_malacrass : public CreatureScript
{
    public:
        boss_hexlord_malacrass() : CreatureScript("boss_hexlord_malacrass") { }

        struct boss_hex_lord_malacrassAI : public BossAI
        {
            boss_hex_lord_malacrassAI(Creature* creature) : BossAI(creature, DATA_HEXLORD), abilitySet(SET_WARRIOR) { }

            uint8 abilitySet;

            void InitializeAI() OVERRIDE
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
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();

                switch (urand(0, 5))
                {
                    case 0:
                        me->SummonCreature(NPC_ALYSON_ANTILE, addPos[0], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        me->SummonCreature(NPC_SLITHER, addPos[1], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        break;
                    case 1:
                        me->SummonCreature(NPC_ALYSON_ANTILE, addPos[0], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        me->SummonCreature(NPC_GAZAKROTH, addPos[1], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        break;
                    case 2:
                        me->SummonCreature(NPC_ALYSON_ANTILE, addPos[0], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        me->SummonCreature(NPC_DARKHEART, addPos[1], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        break;
                    case 3:
                        me->SummonCreature(NPC_GAZAKROTH, addPos[0], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        me->SummonCreature(NPC_SLITHER, addPos[1], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        break;
                    case 4:
                        me->SummonCreature(NPC_GAZAKROTH, addPos[0], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        me->SummonCreature(NPC_DARKHEART, addPos[1], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        break;
                    default:
                        me->SummonCreature(NPC_SLITHER, addPos[0], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        me->SummonCreature(NPC_DARKHEART, addPos[1], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                        break;
                }

                SetEquipmentSlots(false, 46916, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                me->SetSheath(SHEATH_STATE_MELEE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_SPIRIT_BOLTS, 10000);
                events.ScheduleEvent(EVENT_DRAIN_POWER, 6000);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_PLAYER_KILL);
            }

            void SummonedCreatureDies(Creature* /*summon*/, Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_PET_DEATH);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            static uint8 GetAbilitySet(Unit* target)
            {
                switch (target->getClass())
                {
                    case CLASS_PALADIN:
                        return SET_PALADIN;
                    case CLASS_HUNTER:
                        return SET_HUNTER;
                    case CLASS_ROGUE:
                    case CLASS_MONK:
                        return SET_ROGUE;
                    case CLASS_PRIEST:
                        return target->HasAura(SHADOWFORM_SPELL) ? SET_SHADOW : SET_PRIEST;
                    case CLASS_SHAMAN:
                        return SET_SHAMAN;
                    case CLASS_MAGE:
                        return SET_MAGE;
                    case CLASS_WARLOCK:
                        return SET_WARLOCK;
                    case CLASS_DRUID:
                        return SET_DRUID;
                    default:
                        return SET_WARRIOR;
                }
            }

            void UseAbility()
            {
                PlayerAbilityStruct const& ability = PlayerAbility[abilitySet][urand(0, 2)];
                Unit* target = NULL;

                switch (ability.target)
                {
                    case ABILITY_TARGET_SELF:
                        target = me;
                        break;
                    case ABILITY_TARGET_VICTIM:
                        target = me->GetVictim();
                        break;
                    case ABILITY_TARGET_HEAL:
                        target = DoSelectLowestHpFriendly(50.0f, 0);
                        break;
                    case ABILITY_TARGET_BUFF:
                    {
                        std::list<Creature*> missing = DoFindFriendlyMissingBuff(50.0f, ability.spell);
                        if (!missing.empty())
                            target = *missing.begin();
                        break;
                    }
                    case ABILITY_TARGET_ENEMY:
                    default:
                        target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true);
                        break;
                }

                if (target)
                    DoCast(target, ability.spell);
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
                    switch (eventId)
                    {
                        case EVENT_DRAIN_POWER:
                            DoCast(me, SPELL_DRAIN_POWER, true);
                            events.ScheduleEvent(EVENT_DRAIN_POWER, 40000);
                            break;
                        case EVENT_SPIRIT_BOLTS:
                            Talk(SAY_SPIRIT_BOLTS);
                            DoCast(me, SPELL_SPIRIT_BOLTS);
                            events.ScheduleEvent(EVENT_SPIRIT_BOLTS, 45000);
                            events.ScheduleEvent(EVENT_SIPHON_SOUL, 10000);
                            break;
                        case EVENT_SIPHON_SOUL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                Talk(SAY_SIPHON_SOUL);
                                abilitySet = GetAbilitySet(target);
                                DoCast(target, SPELL_SIPHON_SOUL);
                                me->ClearUnitState(UNIT_STATE_CASTING);
                                events.ScheduleEvent(EVENT_PLAYER_ABILITY, urand(5000, 8000));
                                events.ScheduleEvent(EVENT_PLAYER_ABILITY_OFF, 30000);
                            }
                            break;
                        case EVENT_PLAYER_ABILITY:
                            UseAbility();
                            events.ScheduleEvent(EVENT_PLAYER_ABILITY, urand(8000, 10000));
                            break;
                        case EVENT_PLAYER_ABILITY_OFF:
                            events.CancelEvent(EVENT_PLAYER_ABILITY);
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
            return GetZulAmanAI<boss_hex_lord_malacrassAI>(creature);
        }
};

class boss_alyson_antille : public CreatureScript
{
    public:
        boss_alyson_antille() : CreatureScript("boss_alyson_antille") { }

        struct boss_alyson_antilleAI : public ScriptedAI
        {
            boss_alyson_antilleAI(Creature* creature) : ScriptedAI(creature), flashHealTimer(2500) { }

            void Reset() OVERRIDE
            {
                flashHealTimer = 2500;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (flashHealTimer <= diff)
                {
                    if (Unit* target = DoSelectLowestHpFriendly(99.0f, 30000))
                    {
                        if (target->IsWithinDistInMap(me, 50.0f))
                            DoCast(target, SPELL_FLASH_HEAL);
                    }
                    else
                    {
                        if (urand(0, 1))
                            target = DoSelectLowestHpFriendly(50.0f, 0);
                        else
                            target = SelectTarget(SELECT_TARGET_RANDOM, 0);

                        if (target)
                            DoCast(target, SPELL_DISPEL_MAGIC);
                    }
                    flashHealTimer = 2500;
                }
                else
                    flashHealTimer -= diff;

                DoMeleeAttackIfReady();
            }

        private:
            uint32 flashHealTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_alyson_antilleAI(creature);
        }
};

class boss_gazakroth : public CreatureScript
{
    public:
        boss_gazakroth() : CreatureScript("boss_gazakroth") { }

        struct boss_gazakrothAI : public ScriptedAI
        {
            boss_gazakrothAI(Creature* creature) : ScriptedAI(creature), fireboltTimer(2000) { }

            void Reset() OVERRIDE
            {
                fireboltTimer = 2000;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (fireboltTimer <= diff)
                {
                    DoCastVictim(SPELL_FIREBOLT);
                    fireboltTimer = 700;
                }
                else
                    fireboltTimer -= diff;

                DoMeleeAttackIfReady();
            }

        private:
            uint32 fireboltTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_gazakrothAI(creature);
        }
};

class boss_darkheart : public CreatureScript
{
    public:
        boss_darkheart() : CreatureScript("boss_darkheart") { }

        struct boss_darkheartAI : public ScriptedAI
        {
            boss_darkheartAI(Creature* creature) : ScriptedAI(creature), psychicWailTimer(8000) { }

            void Reset() OVERRIDE
            {
                psychicWailTimer = 8000;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (psychicWailTimer <= diff)
                {
                    DoCastVictim(SPELL_PSYCHIC_WAIL);
                    psychicWailTimer = 12000;
                }
                else
                    psychicWailTimer -= diff;

                DoMeleeAttackIfReady();
            }

        private:
            uint32 psychicWailTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_darkheartAI(creature);
        }
};

class boss_slither : public CreatureScript
{
    public:
        boss_slither() : CreatureScript("boss_slither") { }

        struct boss_slitherAI : public ScriptedAI
        {
            boss_slitherAI(Creature* creature) : ScriptedAI(creature), venomSpitTimer(5000) { }

            void Reset() OVERRIDE
            {
                venomSpitTimer = 5000;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (venomSpitTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        DoCast(target, SPELL_VENOM_SPIT);
                    venomSpitTimer = 2500;
                }
                else
                    venomSpitTimer -= diff;

                DoMeleeAttackIfReady();
            }

        private:
            uint32 venomSpitTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_slitherAI(creature);
        }
};

class spell_hexlord_unstable_affliction : public SpellScriptLoader
{
    public:
        spell_hexlord_unstable_affliction() : SpellScriptLoader("spell_hexlord_unstable_affliction") { }

        class spell_hexlord_unstable_affliction_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hexlord_unstable_affliction_AuraScript);

            bool Validate(SpellInfo const* /*spell*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WL_UNSTABLE_AFFL_DISPEL))
                    return false;
                return true;
            }

            void HandleDispel(DispelInfo* dispelInfo)
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(dispelInfo->GetDispeller(), SPELL_WL_UNSTABLE_AFFL_DISPEL, true, NULL, GetEffect(EFFECT_0));
            }

            void Register() OVERRIDE
            {
                AfterDispel += AuraDispelFn(spell_hexlord_unstable_affliction_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_hexlord_unstable_affliction_AuraScript();
        }
};

void AddSC_boss_hex_lord_malacrass()
{
    new boss_hexlord_malacrass();
    new boss_alyson_antille();
    new boss_gazakroth();
    new boss_darkheart();
    new boss_slither();
    new spell_hexlord_unstable_affliction();
}
