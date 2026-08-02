/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "darkmoon_faire.h"
#include "GameEventMgr.h"
#include "GameObject.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellScript.h"

class npc_darkmoon_deathmatch_announcer : public CreatureScript
{
public:
    npc_darkmoon_deathmatch_announcer() : CreatureScript("npc_darkmoon_deathmatch_announcer") { }

    struct npc_darkmoon_deathmatch_announcerAI : public ScriptedAI
    {
        npc_darkmoon_deathmatch_announcerAI(Creature* creature) : ScriptedAI(creature),
            _chestGuid(0), _chestNeedsRespawn(false)
        {
        }

        void UpdateAI(uint32 /*diff*/) override
        {
            if (!_chestNeedsRespawn || _chestGuid)
                return;

            if (GameObject* chest = GetClosestGameObjectWithEntry(me, GO_DARKMOON_DEATHMATCH_CHEST, 100.0f))
                PrepareChest(chest);
        }

        void sOnGameEvent(bool start, uint16 eventId) override
        {
            if (!start)
                return;

            switch (eventId)
            {
                case GAME_EVENT_DARKMOON_DEATHMATCH_ANNOUNCE:
                    Talk(SAY_ANNOUNCE);
                    break;
                case GAME_EVENT_DARKMOON_DEATHMATCH_RUN:
                    Talk(SAY_RUN);
                    RespawnChest();
                    break;
                default:
                    break;
            }
        }

        void SetWinner(uint64 winnerGuid)
        {
            if (Player* player = ObjectAccessor::GetPlayer(*me, winnerGuid))
                Talk(SAY_END, player);

            sGameEventMgr->StopEvent(GAME_EVENT_DARKMOON_DEATHMATCH_RUN);
        }

    private:
        void RespawnChest()
        {
            if (GameObject* chest = ObjectAccessor::GetGameObject(*me, _chestGuid))
            {
                PrepareChest(chest);
                return;
            }

            _chestGuid = 0;
            _chestNeedsRespawn = true;
        }

        void PrepareChest(GameObject* chest)
        {
            _chestGuid = chest->GetGUID();
            _chestNeedsRespawn = false;

            chest->loot.clear();
            chest->SetLootState(LootState::GO_READY);
            chest->Respawn();
        }

        uint64 _chestGuid;
        bool _chestNeedsRespawn;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_darkmoon_deathmatch_announcerAI(creature);
    }
};

class go_darkmoon_treasure_chest : public GameObjectScript
{
public:
    go_darkmoon_treasure_chest() : GameObjectScript("go_darkmoon_treasure_chest"), _lastLooter(0) { }

    bool OnGossipHello(Player* /*player*/, GameObject* /*go*/) override
    {
        return !sGameEventMgr->IsActiveEvent(GAME_EVENT_DARKMOON_DEATHMATCH_RUN);
    }

    void OnLootStateChanged(GameObject* go, uint32 state, Unit* unit) override
    {
        if (state == uint32(LootState::GO_ACTIVATED))
        {
            if (!sGameEventMgr->IsActiveEvent(GAME_EVENT_DARKMOON_DEATHMATCH_RUN))
            {
                _lastLooter = 0;
                go->loot.clear();
                go->SetLootState(LootState::GO_READY);
                return;
            }

            if (Player* player = unit ? unit->ToPlayer() : NULL)
                _lastLooter = player->GetGUID();
            else
                _lastLooter = 0;

            return;
        }

        if (state != uint32(LootState::GO_JUST_DEACTIVATED))
            return;

        if (!_lastLooter)
            return;

        if (Creature* announcer = GetClosestCreatureWithEntry(go, NPC_DARKMOON_DEATHMATCH_ANNOUNCER, 100.0f, true))
            if (npc_darkmoon_deathmatch_announcer::npc_darkmoon_deathmatch_announcerAI* ai = CAST_AI(npc_darkmoon_deathmatch_announcer::npc_darkmoon_deathmatch_announcerAI, announcer->GetAI()))
                ai->SetWinner(_lastLooter);

        _lastLooter = 0;
    }

private:
    uint64 _lastLooter;
};

class spell_darkmoon_deathmatch : public SpellScriptLoader
{
public:
    spell_darkmoon_deathmatch() : SpellScriptLoader("spell_darkmoon_deathmatch") { }

    class spell_darkmoon_deathmatch_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_darkmoon_deathmatch_SpellScript);

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->GetPositionY() < 6402.79f)
                caster->CastSpell(caster, SPELL_ENTER_DEATHMATCH, true);
            else
                caster->CastSpell(caster, SPELL_EXIT_DEATHMATCH, true);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_darkmoon_deathmatch_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_darkmoon_deathmatch_SpellScript();
    }
};

void AddSC_event_darkmoon_deathmatch()
{
    new npc_darkmoon_deathmatch_announcer();
    new go_darkmoon_treasure_chest();
    new spell_darkmoon_deathmatch();
}
