/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "npc_pet_hun_".
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum HunterSpells
{
    SPELL_HUNTER_CRIPPLING_POISON       = 30981,   // Viper
    SPELL_HUNTER_DEADLY_POISON          = 34655,   // Venomous Snake
    SPELL_HUNTER_DIRE_BEAST_FOCUS       = 120694,
    SPELL_HUNTER_MIND_NUMBING_POISON    = 25810    // Viper
};

enum HunterCreatures
{
    NPC_HUNTER_DIRE_BEAST               = 62005,
    NPC_HUNTER_VIPER                    = 19921
};

class npc_dire_beast : public CreatureScript
{
public:
    npc_dire_beast() : CreatureScript("npc_dire_beast") { }

    struct npc_dire_beastAI : public ScriptedAI
    {
        npc_dire_beastAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_ASSIST);
        }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            if (!summoner || summoner->GetTypeId() != TypeID::TYPEID_PLAYER || summoner->getClass() != CLASS_HUNTER)
            {
                me->DespawnOrUnsummon();
                return;
            }

            me->SetReactState(REACT_ASSIST);
            me->setFaction(summoner->getFaction());

            if (Unit* victim = summoner->GetVictim())
                AttackStart(victim);
            else if (Unit* attacker = summoner->getAttackerForHelper())
                AttackStart(attacker);
        }

        void DamageDealt(Unit* /*victim*/, uint32& damage, DamageEffectType damageType) OVERRIDE
        {
            if (!damage || damageType == NODAMAGE)
                return;

            if (Unit* owner = GetHunterOwner())
                owner->EnergizeBySpell(owner, SPELL_HUNTER_DIRE_BEAST_FOCUS, 5, POWER_FOCUS);
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            Unit* owner = GetHunterOwner();
            if (!owner || !owner->IsAlive())
            {
                me->DespawnOrUnsummon();
                return;
            }

            if (!me->GetVictim())
            {
                if (Unit* victim = owner->GetVictim())
                    AttackStart(victim);
                else if (Unit* attacker = owner->getAttackerForHelper())
                    AttackStart(attacker);
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

    private:
        Unit* GetHunterOwner() const
        {
            Unit* owner = me->GetOwner();
            if (!owner && me->IsSummon())
                owner = me->ToTempSummon()->GetSummoner();

            if (!owner || owner->GetTypeId() != TypeID::TYPEID_PLAYER || owner->getClass() != CLASS_HUNTER)
                return NULL;

            return owner;
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_dire_beastAI(creature);
    }
};

class npc_pet_hunter_snake_trap : public CreatureScript
{
    public:
        npc_pet_hunter_snake_trap() : CreatureScript("npc_pet_hunter_snake_trap") { }

        struct npc_pet_hunter_snake_trapAI : public ScriptedAI
        {
            npc_pet_hunter_snake_trapAI(Creature* creature) : ScriptedAI(creature) { }

            void EnterCombat(Unit* /*who*/) OVERRIDE { }

            void Reset() OVERRIDE
            {
                _spellTimer = 0;

                CreatureTemplate const* Info = me->GetCreatureTemplate();

                _isViper = Info->Entry == NPC_HUNTER_VIPER ? true : false;

                me->SetMaxHealth(uint32(107 * (me->getLevel() - 40) * 0.025f));
                // Add delta to make them not all hit the same time
                uint32 delta = (rand() % 7) * 100;
                me->SetStatFloatValue(UNIT_FIELD_ATTACK_ROUND_BASE_TIME, float(Info->baseattacktime + delta));
                me->SetStatFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER, float(Info->attackpower));

                // Start attacking attacker of owner on first ai update after spawn - move in line of sight may choose better target
                if (!me->GetVictim() && me->IsSummon())
                    if (Unit* Owner = me->ToTempSummon()->GetSummoner())
                        if (Owner->getAttackerForHelper())
                            AttackStart(Owner->getAttackerForHelper());
            }

            // Redefined for random target selection:
            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (!me->GetVictim() && me->CanCreatureAttack(who))
                {
                    if (me->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                        return;

                    float attackRadius = me->GetAttackDistance(who);
                    if (me->IsWithinDistInMap(who, attackRadius) && me->IsWithinLOSInMap(who))
                    {
                        if (!(rand() % 5))
                        {
                            me->setAttackTimer(WeaponAttackType::BASE_ATTACK, (rand() % 10) * 100);
                            _spellTimer = (rand() % 10) * 100;
                            AttackStart(who);
                        }
                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->GetVictim()->HasBreakableByDamageCrowdControlAura(me))
                {
                    me->InterruptNonMeleeSpells(false);
                    return;
                }

                if (_spellTimer <= diff)
                {
                    if (_isViper) // Viper
                    {
                        if ((std::rand() % 2) == 0) //33% chance to cast
                        {
                            uint32 spell;
                            if ((std::rand() % 1) == 0)
                                spell = SPELL_HUNTER_MIND_NUMBING_POISON;
                            else
                                spell = SPELL_HUNTER_CRIPPLING_POISON;

                            DoCastVictim(spell);
                        }

                        _spellTimer = 3000;
                    }
                    else // Venomous Snake
                    {
                        if ((std::rand() % 2) == 0) // 33% chance to cast
                            DoCastVictim(SPELL_HUNTER_DEADLY_POISON);
                        _spellTimer = 1500 + (rand() % 5) * 100;
                    }
                }
                else
                    _spellTimer -= diff;

                DoMeleeAttackIfReady();
            }

        private:
            bool _isViper;
            uint32 _spellTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pet_hunter_snake_trapAI(creature);
        }
};

void AddSC_hunter_pet_scripts()
{
    new npc_dire_beast();
    new npc_pet_hunter_snake_trap();
}
