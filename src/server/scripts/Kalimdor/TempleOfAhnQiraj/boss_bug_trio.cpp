/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: boss_kri, boss_yauj, boss_vem : The Bug Trio
SD%Complete: 100
SDComment:
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "temple_of_ahnqiraj.h"

enum Spells
{
    SPELL_CLEAVE       = 26350,
    SPELL_TOXIC_VOLLEY = 25812,
    SPELL_POISON_CLOUD = 38718, //Only Spell with right dmg.
    SPELL_ENRAGE       = 34624, //Changed cause 25790 is casted on gamers too. Same prob with old explosion of twin emperors.

    SPELL_CHARGE       = 26561,
    SPELL_KNOCKBACK    = 26027,

    SPELL_HEAL         = 25807,
    SPELL_FEAR         = 19408
};

class boss_kri : public CreatureScript
{
public:
    boss_kri() : CreatureScript("boss_kri") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_kriAI(creature);
    }

    struct boss_kriAI : public ScriptedAI
    {
        boss_kriAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 Cleave_Timer;
        uint32 ToxicVolley_Timer;
        uint32 Check_Timer;

        bool VemDead;
        bool Death;

        void Reset() OVERRIDE
        {
            Cleave_Timer = std::rand() % 8000 + 4000;
            ToxicVolley_Timer = std::rand() % 12000 + 6000;
            Check_Timer = 2000;

            VemDead = false;
            Death = false;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
            {
                if (instance->GetData(DATA_BUG_TRIO_DEATH) < 2)
                                                                // Unlootable if death
                    me->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);

                instance->SetData(DATA_BUG_TRIO_DEATH, 1);
            }
        }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Cleave_Timer
            if (Cleave_Timer <= diff)
            {
                DoCastVictim(SPELL_CLEAVE);
                Cleave_Timer = std::rand() % 12000 + 5000;
            } else Cleave_Timer -= diff;

            //ToxicVolley_Timer
            if (ToxicVolley_Timer <= diff)
            {
                DoCastVictim(SPELL_TOXIC_VOLLEY);
                ToxicVolley_Timer = std::rand() % 15000 + 10000;
            } else ToxicVolley_Timer -= diff;

            if (!HealthAbovePct(5) && !Death)
            {
                DoCastVictim(SPELL_POISON_CLOUD);
                Death = true;
            }

            if (!VemDead)
            {
                //Checking if Vem is dead. If yes we will enrage.
                if (Check_Timer <= diff)
                {
                    if (instance && instance->GetData(DATA_VEMISDEAD))
                    {
                        DoCast(me, SPELL_ENRAGE);
                        VemDead = true;
                    }
                    Check_Timer = 2000;
                } else Check_Timer -=diff;
            }

            DoMeleeAttackIfReady();
        }
    };
};

class boss_vem : public CreatureScript
{
public:
    boss_vem() : CreatureScript("boss_vem") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_vemAI(creature);
    }

    struct boss_vemAI : public ScriptedAI
    {
        boss_vemAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 Charge_Timer;
        uint32 KnockBack_Timer;
        uint32 Enrage_Timer;

        bool Enraged;

        void Reset() OVERRIDE
        {
            Charge_Timer = std::rand() % 27000 + 15000;
            KnockBack_Timer = std::rand() % 20000 + 8000;
            Enrage_Timer = 120000;

            Enraged = false;
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
            {
                instance->SetData(DATA_VEM_DEATH, 0);
                if (instance->GetData(DATA_BUG_TRIO_DEATH) < 2)
                                                                // Unlootable if death
                    me->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                instance->SetData(DATA_BUG_TRIO_DEATH, 1);
            }
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Charge_Timer
            if (Charge_Timer <= diff)
            {
                Unit* target = NULL;
                target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                if (target)
                {
                    DoCast(target, SPELL_CHARGE);
                    //me->SendMonsterMove(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, true, 1);
                    AttackStart(target);
                }

                Charge_Timer = std::rand() % 16000 + 8000;
            } else Charge_Timer -= diff;

            //KnockBack_Timer
            if (KnockBack_Timer <= diff)
            {
                DoCastVictim(SPELL_KNOCKBACK);
                if (DoGetThreat(me->GetVictim()))
                    DoModifyThreatPercent(me->GetVictim(), -80);
                KnockBack_Timer = std::rand() % 25000 + 15000;
            } else KnockBack_Timer -= diff;

            //Enrage_Timer
            if (!Enraged && Enrage_Timer <= diff)
            {
                DoCast(me, SPELL_ENRAGE);
                Enraged = true;
            } else Charge_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

class boss_yauj : public CreatureScript
{
public:
    boss_yauj() : CreatureScript("boss_yauj") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_yaujAI(creature);
    }

    struct boss_yaujAI : public ScriptedAI
    {
        boss_yaujAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 Heal_Timer;
        uint32 Fear_Timer;
        uint32 Check_Timer;

        bool VemDead;

        void Reset() OVERRIDE
        {
            Heal_Timer = std::rand() % 40000 + 25000;
            Fear_Timer = std::rand() % 24000 + 12000;
            Check_Timer = 2000;

            VemDead = false;
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
            {
                if (instance->GetData(DATA_BUG_TRIO_DEATH) < 2)
                                                                // Unlootable if death
                    me->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                instance->SetData(DATA_BUG_TRIO_DEATH, 1);
            }

            for (uint8 i = 0; i < 10; ++i)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    if (Creature* Summoned = me->SummonCreature(15621, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 90000))
                        Summoned->AI()->AttackStart(target);
                }
            }
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Fear_Timer
            if (Fear_Timer <= diff)
            {
                DoCastVictim(SPELL_FEAR);
                DoResetThreat();
                Fear_Timer = 20000;
            } else Fear_Timer -= diff;

            //Casting Heal to other twins or herself.
            if (Heal_Timer <= diff)
            {
                if (instance)
                {
                    Unit* pKri = Unit::GetUnit(*me, instance->GetData64(DATA_KRI));
                    Unit* pVem = Unit::GetUnit(*me, instance->GetData64(DATA_VEM));

                    switch (std::rand() % 2)
                    {
                        case 0:
                            if (pKri)
                                DoCast(pKri, SPELL_HEAL);
                            break;
                        case 1:
                            if (pVem)
                                DoCast(pVem, SPELL_HEAL);
                            break;
                        case 2:
                            DoCast(me, SPELL_HEAL);
                            break;
                    }
                }

                Heal_Timer = 15000+rand()%15000;
            } else Heal_Timer -= diff;

            //Checking if Vem is dead. If yes we will enrage.
            if (Check_Timer <= diff)
            {
                if (!VemDead)
                {
                    if (instance)
                    {
                        if (instance->GetData(DATA_VEMISDEAD))
                        {
                            DoCast(me, SPELL_ENRAGE);
                            VemDead = true;
                        }
                    }
                }
                Check_Timer = 2000;
            } else Check_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_bug_trio()
{
    new boss_kri();
    new boss_vem();
    new boss_yauj();
}
