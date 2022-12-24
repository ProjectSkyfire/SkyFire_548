/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_High_Inquisitor_Fairbanks
SD%Complete: 100
SDComment: @todo if this guy not involved in some special event, remove (and let ACID script)
SDCategory: Scarlet Monastery
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_CURSEOFBLOOD              = 8282,
    SPELL_DISPELMAGIC               = 15090,
    SPELL_FEAR                      = 12096,
    SPELL_HEAL                      = 12039,
    SPELL_POWERWORDSHIELD           = 11647,
    SPELL_SLEEP                     = 8399
};

class boss_high_inquisitor_fairbanks : public CreatureScript
{
public:
    boss_high_inquisitor_fairbanks() : CreatureScript("boss_high_inquisitor_fairbanks") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_high_inquisitor_fairbanksAI(creature);
    }

    struct boss_high_inquisitor_fairbanksAI : public ScriptedAI
    {
        boss_high_inquisitor_fairbanksAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 CurseOfBlood_Timer;
        uint32 DispelMagic_Timer;
        uint32 Fear_Timer;
        uint32 Heal_Timer;
        uint32 Sleep_Timer;
        uint32 Dispel_Timer;
        bool PowerWordShield;

        void Reset() OVERRIDE
        {
            CurseOfBlood_Timer = 10000;
            DispelMagic_Timer = 30000;
            Fear_Timer = 40000;
            Heal_Timer = 30000;
            Sleep_Timer = 30000;
            Dispel_Timer = 20000;
            PowerWordShield = false;
            me->SetStandState(UNIT_STAND_STATE_DEAD);
            me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 7);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            me->SetStandState(UNIT_STAND_STATE_STAND);
            me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            //If we are <25% hp cast Heal
            if (!HealthAbovePct(25) && !me->IsNonMeleeSpellCasted(false) && Heal_Timer <= diff)
            {
                DoCast(me, SPELL_HEAL);
                Heal_Timer = 30000;
            }
            else Heal_Timer -= diff;

            //Fear_Timer
            if (Fear_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                    DoCast(target, SPELL_FEAR);

                Fear_Timer = 40000;
            }
            else Fear_Timer -= diff;

            //Sleep_Timer
            if (Sleep_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0))
                    DoCast(target, SPELL_SLEEP);

                Sleep_Timer = 30000;
            }
            else Sleep_Timer -= diff;

            //PowerWordShield_Timer
            if (!PowerWordShield && !HealthAbovePct(25))
            {
                DoCast(me, SPELL_POWERWORDSHIELD);
                PowerWordShield = true;
            }

            //Dispel_Timer
            if (Dispel_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_DISPELMAGIC);

                DispelMagic_Timer = 30000;
            }
            else DispelMagic_Timer -= diff;

            //CurseOfBlood_Timer
            if (CurseOfBlood_Timer <= diff)
            {
                DoCastVictim(SPELL_CURSEOFBLOOD);
                CurseOfBlood_Timer = 25000;
            }
            else CurseOfBlood_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_high_inquisitor_fairbanks()
{
    new boss_high_inquisitor_fairbanks();
}
