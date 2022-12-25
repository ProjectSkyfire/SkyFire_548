/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "npc_pet_sha_".
 */

#include "Pet.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum ShamanSpells
{
    SPELL_WARLOCK_FIREBOLT              = 104318,
    SPELL_WARLOCK_MOLTEN_CORE_AURA      = 122351,
    SPELL_WARLOCK_MOLTEN_CORE           = 122355,
};

class npc_wild_imp : public CreatureScript
{
public:
    npc_wild_imp() : CreatureScript("npc_wild_imp") { }

    struct npc_wild_impAI : public ScriptedAI
    {
        uint32 charges;

        npc_wild_impAI(Creature* creature) : ScriptedAI(creature)
        {
            charges = 10;
            me->SetReactState(REACT_ASSIST);
        }

        void Reset()
        {
            me->SetReactState(REACT_ASSIST);

            if (me->GetOwner())
                if (me->GetOwner()->GetVictim())
                    AttackStart(me->GetOwner()->GetVictim());
        }

        void UpdateAI(uint32 diff)
        {
            if (me->GetReactState() != REACT_ASSIST)
                me->SetReactState(REACT_ASSIST);

            if (!me->GetOwner())
                return;

            if (!me->GetOwner()->ToPlayer())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (!charges)
            {
                me->DespawnOrUnsummon();
                return;
            }

            if ((me->GetVictim() || me->GetOwner()->getAttackerForHelper()))
            {
                me->CastSpell(me->GetVictim() ? me->GetVictim() : me->GetOwner()->getAttackerForHelper(), SPELL_WARLOCK_FIREBOLT, false);
                me->GetOwner()->EnergizeBySpell(me->GetOwner(), SPELL_WARLOCK_FIREBOLT, 5, POWER_DEMONIC_FURY);
                charges--;

                if (me->GetOwner()->HasAura(SPELL_WARLOCK_MOLTEN_CORE_AURA))
                    if (roll_chance_i(8))
                        me->GetOwner()->CastSpell(me->GetOwner(), SPELL_WARLOCK_MOLTEN_CORE, true);
            }
            else if (Pet* pet = me->GetOwner()->ToPlayer()->GetPet())
            {
                if (pet->getAttackerForHelper())
                {
                    me->CastSpell(me->GetVictim() ? me->GetVictim() : pet->getAttackerForHelper(), SPELL_WARLOCK_FIREBOLT, false);
                    me->GetOwner()->EnergizeBySpell(me->GetOwner(), SPELL_WARLOCK_FIREBOLT, 5, POWER_DEMONIC_FURY);
                    charges--;

                    if (me->GetOwner()->HasAura(SPELL_WARLOCK_MOLTEN_CORE_AURA))
                        if (roll_chance_i(8))
                            me->GetOwner()->CastSpell(me->GetOwner(), SPELL_WARLOCK_MOLTEN_CORE, true);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_wild_impAI(creature);
    }
};

void AddSC_warlock_pet_scripts()
{
    new npc_wild_imp();
}
