/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Magtheridon
SD%Complete: 60
SDComment: In Development
SDCategory: Hellfire Citadel, Magtheridon's lair
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "magtheridons_lair.h"
#include "Player.h"
#include "SpellInfo.h"

enum Yells
{
    SAY_TAUNT                  = 0,
    SAY_FREED                  = 1,
    SAY_AGGRO                  = 2,
    SAY_BANISH                 = 3,
    SAY_CHAMBER_DESTROY        = 4,
    SAY_PLAYER_KILLED          = 5,
    SAY_DEATH                  = 6
};

enum Emotes
{
    EMOTE_BERSERK              = 7,
    EMOTE_BLASTNOVA            = 8,
    EMOTE_BEGIN                = 9
};

enum Creatures
{
    NPC_MAGTHERIDON    = 17257,
    NPC_ROOM           = 17516,
    NPC_CHANNELLER     = 17256,
    NPC_ABYSSAL        = 17454,
};

enum Spells
{
    SPELL_BLASTNOVA            = 30616,
    SPELL_CLEAVE               = 30619,
    SPELL_QUAKE_TRIGGER        = 30657, //must be cast with 30561 as the proc spell
    SPELL_QUAKE_KNOCKBACK      = 30571,
    SPELL_BLAZE_TARGET         = 30541,
    SPELL_BLAZE_TRAP           = 30542,
    SPELL_DEBRIS_KNOCKDOWN     = 36449,
    SPELL_DEBRIS_VISUAL        = 30632,
    SPELL_DEBRIS_DAMAGE        = 30631, //core bug, does not support target 8
    SPELL_CAMERA_SHAKE         = 36455,
    SPELL_BERSERK              = 27680,
    SPELL_SHADOW_CAGE          = 30168,
    SPELL_SHADOW_GRASP         = 30410,
    SPELL_SHADOW_GRASP_VISUAL  = 30166,
    SPELL_MIND_EXHAUSTION      = 44032, //Casted by the cubes when channeling ends
    SPELL_SHADOW_CAGE_C        = 30205,
    SPELL_SHADOW_GRASP_C       = 30207,
    SPELL_SHADOW_BOLT_VOLLEY   = 30510,
    SPELL_DARK_MENDING         = 30528,
    SPELL_FEAR                 = 30530, //39176
    SPELL_BURNING_ABYSSAL      = 30511,
    SPELL_SOUL_TRANSFER        = 30531, //core bug, does not support target 7
    SPELL_FIRE_BLAST           = 37110,
};

//count of clickers needed to interrupt blast nova
#define CLICKERS_COUNT              5

typedef std::map<uint64, uint64> CubeMap;

class npc_abyssal : public CreatureScript
{
    public:
        npc_abyssal() : CreatureScript("npc_abyssal") { }

        struct npc_abyssalAI : public ScriptedAI
        {
            npc_abyssalAI(Creature* creature) : ScriptedAI(creature)
            {
                trigger = 0;
                Despawn_Timer = 60000;
            }

            uint32 FireBlast_Timer;
            uint32 Despawn_Timer;
            uint32 trigger;

            void Reset() OVERRIDE
            {
                FireBlast_Timer = 6000;
            }

            void SpellHit(Unit*, const SpellInfo* spell) OVERRIDE
            {
                if (trigger == 2 && spell->Id == SPELL_BLAZE_TARGET)
                {
                    DoCast(me, SPELL_BLAZE_TRAP, true);
                    me->SetVisible(false);
                    Despawn_Timer = 130000;
                }
            }

            void SetTrigger(uint32 _trigger)
            {
                trigger = _trigger;
                me->SetDisplayId(11686);
                if (trigger == 1) //debris
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    DoCast(me, SPELL_DEBRIS_VISUAL, true);
                    FireBlast_Timer = 5000;
                    Despawn_Timer = 10000;
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                DoZoneInCombat();
            }
            void AttackStart(Unit* who) OVERRIDE
            {
                if (!trigger)
                    ScriptedAI::AttackStart(who);
            }
            void MoveInLineOfSight(Unit* who) OVERRIDE

            {
                if (!trigger)
                    ScriptedAI::MoveInLineOfSight(who);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (trigger)
                {
                    if (trigger == 1)
                    {
                        if (FireBlast_Timer <= diff)
                        {
                            DoCast(me, SPELL_DEBRIS_DAMAGE, true);
                            trigger = 3;
                        }
                        else FireBlast_Timer -= diff;
                    }
                    return;
                }

                if (Despawn_Timer <= diff)
                {
                    me->DespawnOrUnsummon();
                }
                else Despawn_Timer -= diff;

                if (!UpdateVictim())
                    return;

                if (FireBlast_Timer <= diff)
                {
                    DoCastVictim(SPELL_FIRE_BLAST);
                    FireBlast_Timer = 5000+rand()%10000;
                }
                else FireBlast_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_abyssalAI(creature);
        }
};

class boss_magtheridon : public CreatureScript
{
    public:
        boss_magtheridon() : CreatureScript("boss_magtheridon") { }

        struct boss_magtheridonAI : public ScriptedAI
        {
            boss_magtheridonAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 10);
                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 10);
            }

            CubeMap Cube;

            InstanceScript* instance;

            uint32 Berserk_Timer;
            uint32 Quake_Timer;
            uint32 Cleave_Timer;
            uint32 BlastNova_Timer;
            uint32 Blaze_Timer;
            uint32 Debris_Timer;
            uint32 RandChat_Timer;

            bool Phase3;
            bool NeedCheckCube;

            void Reset() OVERRIDE
            {
                Berserk_Timer = 1320000;
                Quake_Timer = 40000;
                Debris_Timer = 10000;
                Blaze_Timer = 10000+rand()%20000;
                BlastNova_Timer = 60000;
                Cleave_Timer = 15000;
                RandChat_Timer = 90000;

                Phase3 = false;
                NeedCheckCube = false;

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->AddUnitState(UNIT_STATE_STUNNED);
                DoCast(me, SPELL_SHADOW_CAGE_C, true);
            }

            void JustReachedHome() OVERRIDE
            {
                if (instance)
                {
                    instance->SetData(DATA_MAGTHERIDON_EVENT, NOT_STARTED);
                    instance->SetData(DATA_COLLAPSE, false);
                }
            }

            void SetClicker(uint64 cubeGUID, uint64 clickerGUID)
            {
                // to avoid multiclicks from 1 cube
                if (uint64 guid = Cube[cubeGUID])
                    DebuffClicker(Unit::GetUnit(*me, guid));
                Cube[cubeGUID] = clickerGUID;
                NeedCheckCube = true;
            }

            //function to interrupt channeling and debuff clicker with mind exh(used if second person clicks with same cube or after dispeling/ending shadow grasp DoT)
            void DebuffClicker(Unit* clicker)
            {
                if (!clicker || !clicker->IsAlive())
                    return;

                clicker->RemoveAurasDueToSpell(SPELL_SHADOW_GRASP); // cannot interrupt triggered spells
                clicker->InterruptNonMeleeSpells(false);
                clicker->CastSpell(clicker, SPELL_MIND_EXHAUSTION, true);
            }

            void NeedCheckCubeStatus()
            {
                uint32 ClickerNum = 0;
                // now checking if every clicker has debuff from manticron(it is dispelable atm rev 6110 : S)
                // if not - apply mind exhaustion and delete from clicker's list
                for (CubeMap::iterator i = Cube.begin(); i != Cube.end(); ++i)
                {
                    Unit* clicker = Unit::GetUnit(*me, (*i).second);
                    if (!clicker || !clicker->HasAura(SPELL_SHADOW_GRASP))
                    {
                        DebuffClicker(clicker);
                        (*i).second = 0;
                    }
                    else
                        ++ClickerNum;
                }

                // if 5 clickers from other cubes apply shadow cage
                if (ClickerNum >= CLICKERS_COUNT && !me->HasAura(SPELL_SHADOW_CAGE))
                {
                    Talk(SAY_BANISH);
                    DoCast(me, SPELL_SHADOW_CAGE, true);
                }
                else
                    if (ClickerNum < CLICKERS_COUNT && me->HasAura(SPELL_SHADOW_CAGE))
                        me->RemoveAurasDueToSpell(SPELL_SHADOW_CAGE);

                if (!ClickerNum)
                    NeedCheckCube = false;
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_PLAYER_KILLED);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (instance)
                    instance->SetData(DATA_MAGTHERIDON_EVENT, DONE);

                Talk(SAY_DEATH);
            }

            void MoveInLineOfSight(Unit* /*who*/) OVERRIDE { }


            void AttackStart(Unit* who) OVERRIDE
            {
                if (!me->HasUnitState(UNIT_STATE_STUNNED))
                    ScriptedAI::AttackStart(who);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (instance)
                    instance->SetData(DATA_MAGTHERIDON_EVENT, IN_PROGRESS);
                DoZoneInCombat();

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->RemoveAurasDueToSpell(SPELL_SHADOW_CAGE_C);

                Talk(SAY_FREED);
           }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!me->IsInCombat())
                {
                    if (RandChat_Timer <= diff)
                    {
                        Talk(SAY_TAUNT);
                        RandChat_Timer = 90000;
                    }
                    else
                        RandChat_Timer -= diff;
                }

                if (!UpdateVictim())
                    return;

                if (NeedCheckCube) NeedCheckCubeStatus();

                if (Berserk_Timer <= diff)
                {
                    DoCast(me, SPELL_BERSERK, true);
                    Talk(EMOTE_BERSERK);
                    Berserk_Timer = 60000;
                }
                else
                    Berserk_Timer -= diff;

                if (Cleave_Timer <= diff)
                {
                    DoCastVictim(SPELL_CLEAVE);
                    Cleave_Timer = 10000;
                }
                else
                    Cleave_Timer -= diff;

                if (BlastNova_Timer <= diff)
                {
                    // to avoid earthquake interruption
                    if (!me->HasUnitState(UNIT_STATE_STUNNED))
                    {
                        Talk(EMOTE_BLASTNOVA);
                        DoCast(me, SPELL_BLASTNOVA);
                        BlastNova_Timer = 60000;
                    }
                }
                else
                    BlastNova_Timer -= diff;

                if (Quake_Timer <= diff)
                {
                    // to avoid blastnova interruption
                    if (!me->IsNonMeleeSpellCasted(false))
                    {
                        DoCast(me, SPELL_QUAKE_TRIGGER, true);
                        Quake_Timer = 50000;
                    }
                }
                else
                    Quake_Timer -= diff;

                if (Blaze_Timer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    {
                        float x, y, z;
                        target->GetPosition(x, y, z);
                        Creature* summon = me->SummonCreature(NPC_ABYSSAL, x, y, z, 0, TempSummonType::TEMPSUMMON_CORPSE_DESPAWN, 0);
                        if (summon)
                        {
                            CAST_AI(npc_abyssal::npc_abyssalAI, summon->AI())->SetTrigger(2);
                            DoCast(summon, SPELL_BLAZE_TARGET, true);
                            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        }
                    }
                    Blaze_Timer = std::rand() % 40000 + 20000;
                }
                else
                    Blaze_Timer -= diff;

                if (!Phase3 && HealthBelowPct(30)
                    && !me->IsNonMeleeSpellCasted(false) // blast nova
                    && !me->HasUnitState(UNIT_STATE_STUNNED)) // shadow cage and earthquake
                {
                    Phase3 = true;
                    Talk(SAY_CHAMBER_DESTROY);
                    DoCast(me, SPELL_CAMERA_SHAKE, true);
                    DoCast(me, SPELL_DEBRIS_KNOCKDOWN, true);

                    if (instance)
                        instance->SetData(DATA_COLLAPSE, true);
                }

                if (Phase3)
                {
                    if (Debris_Timer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        {
                            float x, y, z;
                            target->GetPosition(x, y, z);
                            Creature* summon = me->SummonCreature(NPC_ABYSSAL, x, y, z, 0, TempSummonType::TEMPSUMMON_CORPSE_DESPAWN, 0);
                            if (summon)
                                CAST_AI(npc_abyssal::npc_abyssalAI, summon->AI())->SetTrigger(1);
                        }
                        Debris_Timer = 10000;
                    }
                    else
                        Debris_Timer -= diff;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_magtheridonAI(creature);
        }
};

class npc_hellfire_channeler : public CreatureScript
{
    public:
        npc_hellfire_channeler() : CreatureScript("npc_hellfire_channeler") { }

        struct npc_hellfire_channelerAI : public ScriptedAI
        {
            npc_hellfire_channelerAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;

            uint32 ShadowBoltVolley_Timer;
            uint32 DarkMending_Timer;
            uint32 Fear_Timer;
            uint32 Infernal_Timer;

            uint32 Check_Timer;

            void Reset() OVERRIDE
            {
                ShadowBoltVolley_Timer = std::rand() % 10000 + 8000;
                DarkMending_Timer = 10000;
                Fear_Timer = std::rand() % 20000 + 15000;
                Infernal_Timer = std::rand() % 50000 + 10000;

                Check_Timer = 5000;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (instance)
                    instance->SetData(DATA_CHANNELER_EVENT, IN_PROGRESS);

                me->InterruptNonMeleeSpells(false);
                DoZoneInCombat();
            }

            void JustReachedHome() OVERRIDE
            {
                if (instance)
                    instance->SetData(DATA_CHANNELER_EVENT, NOT_STARTED);

                DoCast(me, SPELL_SHADOW_GRASP_C, false);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summon->AI()->AttackStart(me->GetVictim());
            }

            void DamageTaken(Unit*, uint32 &damage) OVERRIDE
            {
                if (damage >= me->GetHealth())
                    DoCast(me, SPELL_SOUL_TRANSFER, true);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (instance)
                    instance->SetData(DATA_CHANNELER_EVENT, DONE);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (ShadowBoltVolley_Timer <= diff)
                {
                    DoCast(me, SPELL_SHADOW_BOLT_VOLLEY);
                    ShadowBoltVolley_Timer = std::rand() % 20000 + 10000;
                }
                else
                    ShadowBoltVolley_Timer -= diff;

                if (DarkMending_Timer <= diff)
                {
                    if (HealthBelowPct(50))
                        DoCast(me, SPELL_DARK_MENDING);
                    DarkMending_Timer = 10000 +(rand() % 10000);
                }
                else
                    DarkMending_Timer -= diff;

                if (Fear_Timer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                        DoCast(target, SPELL_FEAR);
                    Fear_Timer = std::rand() % 40000 + 25000;
                }
                else
                    Fear_Timer -= diff;

                if (Infernal_Timer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_BURNING_ABYSSAL, true);
                    Infernal_Timer = std::rand() % 40000 + 30000;
                }
                else
                    Infernal_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_hellfire_channelerAI(creature);
        }
};

//Manticron Cube
class go_manticron_cube : public GameObjectScript
{
public:
    go_manticron_cube() : GameObjectScript("go_manticron_cube")
    {
    }

    bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
    {
        InstanceScript* instance = go->GetInstanceScript();

        if (!instance)
            return true;

        if (instance->GetData(DATA_MAGTHERIDON_EVENT) != IN_PROGRESS)
            return true;
        Creature* Magtheridon =Unit::GetCreature(*go, instance->GetData64(DATA_MAGTHERIDON));
        if (!Magtheridon || !Magtheridon->IsAlive())
            return true;

        // if exhausted or already channeling return
        if (player->HasAura(SPELL_MIND_EXHAUSTION) || player->HasAura(SPELL_SHADOW_GRASP))
            return true;

        player->InterruptNonMeleeSpells(false);
        player->CastSpell(player, SPELL_SHADOW_GRASP, true);
        player->CastSpell(player, SPELL_SHADOW_GRASP_VISUAL, false);
        CAST_AI(boss_magtheridon::boss_magtheridonAI, Magtheridon->AI())->SetClicker(go->GetGUID(), player->GetGUID());
        return true;
    }
};

void AddSC_boss_magtheridon()
{
    new boss_magtheridon();
    new npc_hellfire_channeler();
    new npc_abyssal();
    new go_manticron_cube();
}

