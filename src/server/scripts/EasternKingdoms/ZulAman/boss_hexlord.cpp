/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
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
    SPELL_WL_UNSTABLE_AFFL          = 43522,
    SPELL_WL_UNSTABLE_AFFL_DISPEL   = 43523,
};

enum Events
{
};

class boss_hexlord_malacrass : public CreatureScript
{
    public:

        boss_hexlord_malacrass() : CreatureScript("boss_hexlord_malacrass") { }

        struct boss_hex_lord_malacrassAI : public BossAI
        {
            boss_hex_lord_malacrassAI(Creature* creature) : BossAI(creature, DATA_HEXLORD) { }

            void Reset() OVERRIDE
            {
                _Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);
                _EnterCombat();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_DEATH);
                _JustDied();
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_PLAYER_KILL);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
                /*
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        default:
                            break;
                    }
                }
                */

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulAmanAI<boss_hex_lord_malacrassAI>(creature);
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
    new spell_hexlord_unstable_affliction();
}
