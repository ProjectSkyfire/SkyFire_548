/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Stratholme
SD%Complete: 100
SDComment: Misc mobs for instance. go-script to apply aura and start event for quest 8945
SDCategory: Stratholme
EndScriptData */

/* ContentData
go_gauntlet_gate
npc_freed_soul
npc_restless_soul
npc_spectral_ghostly_citizen
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stratholme.h"
#include "Group.h"
#include "Player.h"
#include "SpellInfo.h"
#include "MoveSplineInit.h"

/*######
## go_gauntlet_gate (this is the _first_ of the gauntlet gates, two exist)
######*/

class go_gauntlet_gate : public GameObjectScript
{
public:
    go_gauntlet_gate() : GameObjectScript("go_gauntlet_gate") { }

    bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
    {
        InstanceScript* instance = go->GetInstanceScript();

        if (!instance)
            return false;

        if (instance->GetData(TYPE_BARON_RUN) != NOT_STARTED)
            return false;

        if (Group* group = player->GetGroup())
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* pGroupie = itr->GetSource();
                if (!pGroupie)
                    continue;

                if (pGroupie->GetQuestStatus(QUEST_DEAD_MAN_PLEA) == QUEST_STATUS_INCOMPLETE &&
                    !pGroupie->HasAura(SPELL_BARON_ULTIMATUM) &&
                    pGroupie->GetMap() == go->GetMap())
                    pGroupie->CastSpell(pGroupie, SPELL_BARON_ULTIMATUM, true);
            }
        } else if (player->GetQuestStatus(QUEST_DEAD_MAN_PLEA) == QUEST_STATUS_INCOMPLETE &&
                    !player->HasAura(SPELL_BARON_ULTIMATUM) &&
                    player->GetMap() == go->GetMap())
                    player->CastSpell(player, SPELL_BARON_ULTIMATUM, true);

        instance->SetData(TYPE_BARON_RUN, IN_PROGRESS);
        return false;
    }
};

/*######
## npc_freed_soul
######*/
enum FreedSoul
{
    SAY_ZAPPED = 0
};

class npc_freed_soul : public CreatureScript
{
public:
    npc_freed_soul() : CreatureScript("npc_freed_soul") { }

    struct npc_freed_soulAI : public ScriptedAI
    {
        npc_freed_soulAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            Talk(SAY_ZAPPED);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_freed_soulAI(creature);
    }
};

/*######
## npc_restless_soul
######*/

enum RestlessSoul
{
    // Spells
    SPELL_EGAN_BLASTER      = 17368,
    SPELL_SOUL_FREED        = 17370,

    // Quest
    QUEST_RESTLESS_SOUL     = 5282,

    // Creatures
    NPC_RESTLESS            = 11122,
    NPC_FREED               = 11136
};

class npc_restless_soul : public CreatureScript
{
public:
    npc_restless_soul() : CreatureScript("npc_restless_soul") { }

    struct npc_restless_soulAI : public ScriptedAI
    {
        npc_restless_soulAI(Creature* creature) : ScriptedAI(creature)
        {
            Tagger = 0;
            Die_Timer = 0;
            Tagged = false;
        }

        void Reset() OVERRIDE
        {
            Tagger = 0;
            Die_Timer = 5000;
            Tagged = false;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
        {
            if (Tagged || spell->Id != SPELL_EGAN_BLASTER)
                return;

            Player* player = caster->ToPlayer();
            if (!player || player->GetQuestStatus(QUEST_RESTLESS_SOUL) != QUEST_STATUS_INCOMPLETE)
                return;

            Tagged = true;
            Tagger = caster->GetGUID();
        }

        void JustSummoned(Creature* summoned) OVERRIDE
        {
            summoned->CastSpell(summoned, SPELL_SOUL_FREED, false);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (Tagged)
                me->SummonCreature(NPC_FREED, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 300000);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (Tagged)
            {
                if (Die_Timer <= diff)
                {
                    if (Unit* temp = Unit::GetUnit(*me, Tagger))
                    {
                        if (Player* player = temp->ToPlayer())
                            player->KilledMonsterCredit(NPC_RESTLESS, me->GetGUID());
                        me->Kill(me);
                    }
                }
                else
                    Die_Timer -= diff;
            }
        }
    private:
        uint64 Tagger;
        uint32 Die_Timer;
        bool Tagged;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_restless_soulAI(creature);
    }
};

/*######
## npc_spectral_ghostly_citizen
######*/

enum GhostlyCitizenSpells
{
    SPELL_HAUNTING_PHANTOM  = 16336,
    SPELL_SLAP              = 6754
};

class npc_spectral_ghostly_citizen : public CreatureScript
{
public:
    npc_spectral_ghostly_citizen() : CreatureScript("npc_spectral_ghostly_citizen") { }

    struct npc_spectral_ghostly_citizenAI : public ScriptedAI
    {
        npc_spectral_ghostly_citizenAI(Creature* creature) : ScriptedAI(creature)
        {
            Die_Timer = 0;
            Tagged = false;
        }

        void Reset() OVERRIDE
        {
            Die_Timer = 5000;
            Tagged = false;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell) OVERRIDE
        {
            if (!Tagged && spell->Id == SPELL_EGAN_BLASTER)
                Tagged = true;
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (Tagged)
            {
                for (uint32 i = 1; i <= 4; ++i)
                {
                     //100%, 50%, 33%, 25% chance to spawn
                     if (urand(1, i) == 1)
                         DoSummon(NPC_RESTLESS, me, 20.0f, 600000);
                }
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (Tagged)
            {
                if (Die_Timer <= diff)
                    me->Kill(me);
                else Die_Timer -= diff;
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void ReceiveEmote(Player* player, uint32 emote) OVERRIDE
        {
            switch (emote)
            {
                case TEXT_EMOTE_DANCE:
                    EnterEvadeMode();
                    break;
                case TEXT_EMOTE_RUDE:
                    if (me->IsWithinDistInMap(player, 5))
                        DoCast(player, SPELL_SLAP, false);
                    else
                        me->HandleEmoteCommand(EMOTE_ONESHOT_RUDE);
                    break;
                case TEXT_EMOTE_WAVE:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                    break;
                case TEXT_EMOTE_BOW:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                    break;
                case TEXT_EMOTE_KISS:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_FLEX);
                    break;
            }
        }

    private:
        uint32 Die_Timer;
        bool Tagged;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_spectral_ghostly_citizenAI(creature);
    }
};

/*######
## npc_argent_crusader
######*/

class npc_argent_crusader : public CreatureScript
{
public:
    npc_argent_crusader() : CreatureScript("npc_argent_crusader") { }

    struct npc_argent_crusaderAI : public ScriptedAI
    {
        npc_argent_crusaderAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            // staged fights against the undead: the crusaders must not kill them
            me->SetBaseWeaponDamage(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MINDAMAGE, 0);
            me->SetBaseWeaponDamage(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MAXDAMAGE, 0);
            me->UpdateDamagePhysical(WeaponAttackType::BASE_ATTACK);

            for (uint32 entry : CitizenType)
            {
                if (Creature* citizen = me->FindNearestCreature(entry, 30.0f, true))
                {
                    me->Attack(citizen, true);
                    me->GetMotionMaster()->MoveChase(citizen);
                    break;
                }
            }
        }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            if (attacker->GetTypeId() != TypeID::TYPEID_PLAYER && HealthBelowPct(40))
                damage = 0;
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_argent_crusaderAI(creature);
    }
};

/*######
## npc_brotherhood_of_the_light_quester
######*/

enum BrotherhoodQuester
{
    EVENT_BALNAZZAR_QUEST = 1,
};

class npc_brotherhood_of_the_light_quester : public CreatureScript
{
public:
    npc_brotherhood_of_the_light_quester() : CreatureScript("npc_brotherhood_of_the_light_quester") { }

    struct npc_brotherhood_of_the_light_questerAI : public ScriptedAI
    {
        npc_brotherhood_of_the_light_questerAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap nonCombatEvents;

        void DoAction(int32 actionId) OVERRIDE
        {
            if (actionId != ACTION_BALNAZZAR)
                return;

            me->SetVisible(true);

            Movement::MoveSplineInit init(me);
            if (me->GetEntry() == NPC_COMMANDER_ELIGOR)
                init.MoveTo(EligorPath.GetPositionX(), EligorPath.GetPositionY(), EligorPath.GetPositionZ());
            else
            {
                for (Position const& pos : StoneBruiserPath)
                    init.Path().push_back(G3D::Vector3(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ()));
            }

            init.SetSmooth();
            init.SetUncompressed();
            init.Launch();
            nonCombatEvents.ScheduleEvent(EVENT_BALNAZZAR_QUEST, me->GetSplineDuration());
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            nonCombatEvents.Update(diff);

            while (uint32 eventId = nonCombatEvents.ExecuteEvent())
            {
                if (eventId == EVENT_BALNAZZAR_QUEST)
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_brotherhood_of_the_light_questerAI(creature);
    }
};

void AddSC_stratholme()
{
    new go_gauntlet_gate();
    new npc_freed_soul();
    new npc_restless_soul();
    new npc_spectral_ghostly_citizen();
    new npc_argent_crusader();
    new npc_brotherhood_of_the_light_quester();
}
