/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "violet_hold.h"
#include "Player.h"

enum Spells
{
    SPELL_ARCANE_BARRAGE_VOLLEY                 = 54202,
    SPELL_ARCANE_BARRAGE_VOLLEY_H               = 59483,
    SPELL_ARCANE_BUFFET                         = 54226,
    SPELL_ARCANE_BUFFET_H                       = 59485,
    SPELL_SUMMON_ETHEREAL_SPHERE_1              = 54102,
    SPELL_SUMMON_ETHEREAL_SPHERE_2              = 54137,
    SPELL_SUMMON_ETHEREAL_SPHERE_3              = 54138
};

enum NPCs
{
    NPC_ETHEREAL_SPHERE                         = 29271,
    //NPC_ETHEREAL_SPHERE2                      = 32582, // heroic only?
};

enum CreatureSpells
{
    SPELL_ARCANE_POWER                          = 54160,
    H_SPELL_ARCANE_POWER                        = 59474,
    SPELL_SUMMON_PLAYERS                        = 54164,
    SPELL_POWER_BALL_VISUAL                     = 54141
};

enum Yells
{
    SAY_AGGRO                                   = 0,
    SAY_SLAY                                    = 1,
    SAY_DEATH                                   = 2,
    SAY_SPAWN                                   = 3,
    SAY_CHARGED                                 = 4,
    SAY_REPEAT_SUMMON                           = 5,
    SAY_SUMMON_ENERGY                           = 6
};

class boss_xevozz : public CreatureScript
{
public:
    boss_xevozz() : CreatureScript("boss_xevozz") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_xevozzAI(creature);
    }

    struct boss_xevozzAI : public ScriptedAI
    {
        boss_xevozzAI(Creature* creature) : ScriptedAI(creature)
        {
            instance  = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 uiSummonEtherealSphere_Timer;
        uint32 uiArcaneBarrageVolley_Timer;
        uint32 uiArcaneBuffet_Timer;

        void Reset() OVERRIDE
        {
            if (instance)
            {
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                    instance->SetData(DATA_1ST_BOSS_EVENT, NOT_STARTED);
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                    instance->SetData(DATA_2ND_BOSS_EVENT, NOT_STARTED);
            }

            uiSummonEtherealSphere_Timer = urand(10000, 12000);
            uiArcaneBarrageVolley_Timer = urand(20000, 22000);
            uiArcaneBuffet_Timer = uiSummonEtherealSphere_Timer + urand(5000, 6000);
            DespawnSphere();
        }

        void DespawnSphere()
        {
            std::list<Creature*> assistList;
            GetCreatureListWithEntryInGrid(assistList, me, NPC_ETHEREAL_SPHERE, 150.0f);

            if (assistList.empty())
                return;

            for (std::list<Creature*>::const_iterator iter = assistList.begin(); iter != assistList.end(); ++iter)
            {
                if (Creature* pSphere = *iter)
                    pSphere->Kill(pSphere, false);
            }
        }

        void JustSummoned(Creature* summoned) OVERRIDE
        {
            summoned->SetSpeed(MOVE_RUN, 0.5f);
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
            {
                summoned->AddThreat(target, 0.00f);
                summoned->AI()->AttackStart(target);
            }
        }

        void AttackStart(Unit* who) OVERRIDE
        {
            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC) || me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if (me->Attack(who, true))
            {
                me->AddThreat(who, 0.0f);
                me->SetInCombatWith(who);
                who->SetInCombatWith(me);
                DoStartMovement(who);
            }
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
            if (instance)
            {
                if (GameObject* pDoor = instance->instance->GetGameObject(instance->GetData64(DATA_XEVOZZ_CELL)))
                    if (pDoor->GetGoState() == GOState::GO_STATE_READY)
                    {
                        EnterEvadeMode();
                        return;
                    }
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                    instance->SetData(DATA_1ST_BOSS_EVENT, IN_PROGRESS);
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                    instance->SetData(DATA_2ND_BOSS_EVENT, IN_PROGRESS);
            }
        }

        void MoveInLineOfSight(Unit* /*who*/) OVERRIDE { }


        void UpdateAI(uint32 uiDiff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiArcaneBarrageVolley_Timer < uiDiff)
            {
                DoCast(me, SPELL_ARCANE_BARRAGE_VOLLEY);
                uiArcaneBarrageVolley_Timer = urand(20000, 22000);
            }
            else uiArcaneBarrageVolley_Timer -= uiDiff;

            if (uiArcaneBuffet_Timer)
            {
                if (uiArcaneBuffet_Timer < uiDiff)
                {
                    DoCastVictim(SPELL_ARCANE_BUFFET);
                    uiArcaneBuffet_Timer = 0;
                }
                else uiArcaneBuffet_Timer -= uiDiff;
            }

            if (uiSummonEtherealSphere_Timer < uiDiff)
            {
                Talk(SAY_SPAWN);
                DoCast(me, SPELL_SUMMON_ETHEREAL_SPHERE_1);
                if (IsHeroic()) // extra one for heroic
                    me->SummonCreature(NPC_ETHEREAL_SPHERE, me->GetPositionX()-5+rand()%10, me->GetPositionY()-5+rand()%10, me->GetPositionZ(), 0, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 40000);

                uiSummonEtherealSphere_Timer = urand(45000, 47000);
                uiArcaneBuffet_Timer = urand(5000, 6000);
            }
            else uiSummonEtherealSphere_Timer -= uiDiff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);

            DespawnSphere();

            if (instance)
            {
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                {
                    instance->SetData(DATA_1ST_BOSS_EVENT, DONE);
                    instance->SetData(DATA_WAVE_COUNT, 7);
                }
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                {
                    instance->SetData(DATA_2ND_BOSS_EVENT, NOT_STARTED);
                    instance->SetData(DATA_WAVE_COUNT, 13);
                }
            }
        }
        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            Talk(SAY_SLAY);
        }
    };
};

class npc_ethereal_sphere : public CreatureScript
{
public:
    npc_ethereal_sphere() : CreatureScript("npc_ethereal_sphere") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_ethereal_sphereAI(creature);
    }

    struct npc_ethereal_sphereAI : public ScriptedAI
    {
        npc_ethereal_sphereAI(Creature* creature) : ScriptedAI(creature)
        {
            instance   = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 uiSummonPlayers_Timer;
        uint32 uiRangeCheck_Timer;

        void Reset() OVERRIDE
        {
            uiSummonPlayers_Timer = urand(33000, 35000);
            uiRangeCheck_Timer = 1000;
        }

        void UpdateAI(uint32 uiDiff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (!me->HasAura(SPELL_POWER_BALL_VISUAL))
                DoCast(me, SPELL_POWER_BALL_VISUAL);

            if (uiRangeCheck_Timer < uiDiff)
            {
                if (instance)
                {
                    if (Creature* pXevozz = Unit::GetCreature(*me, instance->GetData64(DATA_XEVOZZ)))
                    {
                        float fDistance = me->GetDistance2d(pXevozz);
                        if (fDistance <= 3)
                            DoCast(pXevozz, SPELL_ARCANE_POWER);
                        else
                            DoCast(me, 35845); //Is it blizzlike?
                    }
                }
                uiRangeCheck_Timer = 1000;
            }
            else uiRangeCheck_Timer -= uiDiff;

            if (uiSummonPlayers_Timer < uiDiff)
            {
                DoCast(me, SPELL_SUMMON_PLAYERS); // not working right

                Map* map = me->GetMap();
                if (map && map->IsDungeon())
                {
                    Map::PlayerList const &PlayerList = map->GetPlayers();

                    if (!PlayerList.isEmpty())
                        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                            if (i->GetSource()->IsAlive())
                                DoTeleportPlayer(i->GetSource(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), i->GetSource()->GetOrientation());
                }

                uiSummonPlayers_Timer = urand(33000, 35000);
            }
            else uiSummonPlayers_Timer -= uiDiff;
        }
    };
};

void AddSC_boss_xevozz()
{
    new boss_xevozz();
    new npc_ethereal_sphere();
}
