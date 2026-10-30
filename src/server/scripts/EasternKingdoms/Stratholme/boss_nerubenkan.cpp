/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Nerubenkan
SD%Complete: 70
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stratholme.h"

enum Spells
{
    SPELL_ENCASINGWEBS          = 4962,
    SPELL_PIERCEARMOR           = 6016,
    SPELL_CRYPT_SCARABS         = 31602,
    SPELL_RAISEUNDEADSCARAB     = 17235
};

class boss_nerubenkan : public CreatureScript
{
public:
    boss_nerubenkan() : CreatureScript("boss_nerubenkan") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_nerubenkanAI(creature);
    }

    struct boss_nerubenkanAI : public ScriptedAI
    {
        boss_nerubenkanAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 EncasingWebs_Timer;
        uint32 PierceArmor_Timer;
        uint32 CryptScarabs_Timer;
        uint32 RaiseUndeadScarab_Timer;

        void Reset() OVERRIDE
        {
            CryptScarabs_Timer = 3000;
            EncasingWebs_Timer = 7000;
            PierceArmor_Timer = 19000;
            RaiseUndeadScarab_Timer = 3000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
                instance->SetData(TYPE_NERUB, IN_PROGRESS);
        }

        void RaiseUndeadScarab(Unit* victim)
        {
            if (Creature* pUndeadScarab = DoSpawnCreature(10876, float(irand(-9, 9)), float(irand(-9, 9)), 0, 0, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000))
                if (pUndeadScarab->AI())
                    pUndeadScarab->AI()->AttackStart(victim);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            //EncasingWebs
            if (EncasingWebs_Timer <= diff)
            {
                DoCastVictim(SPELL_ENCASINGWEBS);
                EncasingWebs_Timer = 30000;
            } else EncasingWebs_Timer -= diff;

            //PierceArmor
            if (PierceArmor_Timer <= diff)
            {
                if (urand(0, 3) < 2)
                    DoCastVictim(SPELL_PIERCEARMOR);
                PierceArmor_Timer = 35000;
            } else PierceArmor_Timer -= diff;

            //CryptScarabs_Timer
            if (CryptScarabs_Timer <= diff)
            {
                DoCastVictim(SPELL_CRYPT_SCARABS);
                CryptScarabs_Timer = 20000;
            } else CryptScarabs_Timer -= diff;

            //RaiseUndeadScarab
            if (RaiseUndeadScarab_Timer <= diff)
            {
                RaiseUndeadScarab(me->GetVictim());
                RaiseUndeadScarab_Timer = 16000;
            } else RaiseUndeadScarab_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_nerubenkan()
{
    new boss_nerubenkan();
}
