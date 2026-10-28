/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Felwood
SD%Complete: 95
SDComment: Quest support:
SDCategory: Felwood
EndScriptData */

/* ContentData
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"

enum q28288
{
    NPC_JADEFIRE_IMP = 47369,
};

 // Terrifying Rainbow - 89620
class spell_terrifyng_rainbow : public SpellScript
{
    PrepareSpellScript(spell_terrifyng_rainbow);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* obj) { return obj->GetEntry() != NPC_JADEFIRE_IMP; });
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_terrifyng_rainbow::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

// http://www.wowhead.com/quest=27989/ruumbo-demands-honey
enum QuestRuumboDemandsHoney
{
    SPELL_SWIPE_HONEY        = 88254,
    SPELL_BEES_BEES          = 88425,

    ITEM_DEADWOOD_HONEY_GLOB = 62820,

    SAY_BEES                 = 0
};

struct npc_felwood_honey_bunny : public ScriptedAI
{
    npc_felwood_honey_bunny(Creature* creature) : ScriptedAI(creature) { }

    void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
    {
        if (spell->Id == SPELL_SWIPE_HONEY)
        {
            if (Player* player = caster->ToPlayer())
            {
                player->AddItem(ITEM_DEADWOOD_HONEY_GLOB, 1);
                if ((rand() % 10) < 4) // Random chance 40%
                {
                    me->AI()->Talk(SAY_BEES, player);
                    me->CastSpell(caster, SPELL_BEES_BEES, true);
                }
                me->DespawnOrUnsummon();
            }
        }
    }
};

enum q27995
{
    QUEST_DANCE_FOR_RUUMBO = 27995,
                           
    NPC_FERLI              = 47558,
    NPC_DRIZLE             = 47556,
    NPC_RUMBO_DANCE_CREDIT = 47555,
};

const Position fulborogsLeavePos[4] =
{
    { 3834.64f, -1338.99f, 205.77f, 0.81f }, // ferli
    { 3841.56f, -1290.68f, 210.03f, 4.95f },
    { 3847.89f, -1322.69f, 210.73f, 2.06f }, // spawn
    { 3852.09f, -1320.37f, 211.61f, 2.68f },
};

// Ferli 47558, Drizzle 47556
struct npc_feelwood_ferli_drizzle : public ScriptedAI
{
    npc_feelwood_ferli_drizzle(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    bool hasDance;
    bool canAchieve;
    uint64 ownerGUID;
    uint64 ferliGUID;
    uint32 delay;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());
        ownerGUID = summoner->GetGUID();
        hasDance   = false;
        canAchieve = false;
        ferliGUID = 0;

        // Ferli is controller
        if (me->GetEntry() == NPC_FERLI)
            return;

        uint32 delay = 1000;
        scheduler
            .Schedule(Milliseconds(delay), [this](TaskContext context)
        {
            Talk(0);
        });

        scheduler
            .Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
        {
            if (Player* owner = ObjectAccessor::GetPlayer(*me, ownerGUID))
            {
                std::list<TempSummon*> fulborgs;
                owner->GetSummons(fulborgs, NPC_FERLI);

                if (!fulborgs.empty())
                {
                    fulborgs.front()->AI()->Talk(0);
                    ferliGUID = fulborgs.front()->GetGUID();
                }
            }
        });

        scheduler
            .Schedule(Milliseconds(delay += 2500), [this](TaskContext context)
        {
            Talk(1);
            canAchieve = true;

            if (Creature* ferli = ObjectAccessor::GetCreature(*me, ferliGUID))
                ferli->AI()->DoAction(0);
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        switch (actionId)
        {
            case 0:
                canAchieve = true;
                break;
            case 1:
                if (me->GetEntry() == NPC_DRIZLE)
                {
                    me->GetMotionMaster()->MovePoint(0, fulborogsLeavePos[1]);
                    me->DespawnOrUnsummon(me->GetSplineDuration());
                }
                else
                {
                    Talk(1);

                    scheduler
                        .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
                    {
                        me->GetMotionMaster()->MovePoint(0, fulborogsLeavePos[0]);
                        me->DespawnOrUnsummon(me->GetSplineDuration());
                    });
                }
                break;
        }
    }

    void ReceiveEmote(Player* player, uint32 uiTextEmote) OVERRIDE
    {
        if (uiTextEmote == TEXT_EMOTE_DANCE && !hasDance && canAchieve)
        {
            hasDance = true;
            player->KilledMonsterCredit(NPC_RUMBO_DANCE_CREDIT);

            std::list<TempSummon*> fulborgs;
            player->GetSummons(fulborgs, me->GetEntry() == NPC_FERLI ? NPC_DRIZLE : NPC_FERLI);

            if (!fulborgs.empty())
                fulborgs.front()->AI()->DoAction(1);

            DoAction(1);
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Totem of Ruumbo 206585
class go_totem_of_ruumbo : public GameObjectScript
{
    public:
        go_totem_of_ruumbo() : GameObjectScript("go_totem_of_ruumbo") { }

        bool OnQuestAccept(Player* player, GameObject* go, Quest const* quest) OVERRIDE
        {
            if (quest->GetQuestId() == QUEST_DANCE_FOR_RUUMBO)
            {
                player->SummonCreature(NPC_FERLI, fulborogsLeavePos[2], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN, Milliseconds(60 * IN_MILLISECONDS));
                player->SummonCreature(NPC_DRIZLE, fulborogsLeavePos[3], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN, Milliseconds(60 * IN_MILLISECONDS));
            }

            return true;
        }
};

void AddSC_felwood()
{
    new GenericSpellScriptLoader<spell_terrifyng_rainbow>("spell_terrifyng_rainbow");
    new creature_script<npc_felwood_honey_bunny>("npc_felwood_honey_bunny");
    new creature_script<npc_feelwood_ferli_drizzle>("npc_feelwood_ferli_drizzle");
    new go_totem_of_ruumbo();
}
