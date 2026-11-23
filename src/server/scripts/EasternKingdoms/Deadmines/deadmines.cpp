/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "Player.h"
#include "deadmines.h"

#define GOSSIP_SENDER_DEADMINES_PORT 36

Position const deadmines_locs[] =
{
    { -64.1528f, -385.99f, 53.192f, 1.85005f },
    { -305.321f, -491.292f, 49.232f, 0.488691f },
    { -201.096f, -606.05f, 19.3022f, 2.74016f },
    { -129.915f, -788.898f, 17.3409f, 0.366518f },
};

enum DefiasWatcher
{
    SPELL_ON_FIRE           = 91737,
    SPELL_EXPLOSIVE_SUICIDE = 91738,
    SPELL_ENERGIZE          = 89132,
    SPELL_WATCHER_CLEAVE    = 90980,
    FACTION_CONTROLLABLE    = 1816,
    EVENT_WATCHER_CLEAVE    = 1
};

// Defias Watcher 47404: below 30% health it stops fighting, burns and energizes the players around it,
// then blows up when the fire goes out
struct npc_deadmines_defias_watcher : public ScriptedAI
{
    npc_deadmines_defias_watcher(Creature* creature) : ScriptedAI(creature), isOnFire(false) { }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_WATCHER_CLEAVE, 3500);
    }

    void Reset() OVERRIDE
    {
        events.Reset();
        me->SetFullHealth();
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
    {
        if (isOnFire || !me->HealthBelowPctDamaged(30, damage))
            return;

        isOnFire = true;
        me->DeleteThreatList();
        me->CombatStop(true);
        me->setFaction(FACTION_CONTROLLABLE);
        DoCast(me, SPELL_ON_FIRE);
        DoCastAOE(SPELL_ENERGIZE);
        me->setRegeneratingHealth(false);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_WATCHER_CLEAVE)
            {
                DoCastVictim(SPELL_WATCHER_CLEAVE);
                events.ScheduleEvent(EVENT_WATCHER_CLEAVE, urand(4000, 5000));
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    bool isOnFire;
    EventMap events;
};

// 91737 - On Fire!
class spell_deadmines_on_fire : public AuraScript
{
    PrepareAuraScript(spell_deadmines_on_fire);

    bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
    {
        return sSpellMgr->GetSpellInfo(SPELL_EXPLOSIVE_SUICIDE) != NULL;
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->CastSpell(GetTarget(), SPELL_EXPLOSIVE_SUICIDE, true);
    }

    void Register() OVERRIDE
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_deadmines_on_fire::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

// Prototype Reaper 49208
struct npc_deadmines_prototype_reaper : public ScriptedAI
{
    npc_deadmines_prototype_reaper(Creature* creature) : ScriptedAI(creature) { }

    void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
    {
        if (me->HealthBelowPctDamaged(90, damage) && me->GetMap()->GetWorldState(WORLDSTATE_PROTOTYPE_PRODIGY))
            me->GetMap()->SetWorldState(WORLDSTATE_PROTOTYPE_PRODIGY, 0);
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

class go_defias_cannon : public GameObjectScript
{
    public:
        go_defias_cannon() : GameObjectScript("go_defias_cannon") { }

        bool OnGossipHello(Player* /*player*/, GameObject* go) OVERRIDE
        {
            InstanceScript* instance = go->GetInstanceScript();
            if (!instance)
                return false;

            instance->SetData(DATA_CANNON_EVENT, CANNON_BLAST_INITIATED);
            return false;
        }
};

class go_deadmines_teleport : public GameObjectScript
{
    public:
        go_deadmines_teleport() : GameObjectScript("go_deadmines_teleport") { }

        bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
        {
            if (InstanceScript* instance = go->GetInstanceScript())
            {
                if (instance->GetBossState(DATA_GLUBTOK) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to entrance.", GOSSIP_SENDER_DEADMINES_PORT, 0);

                if (instance->GetBossState(DATA_ADMIRAL) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Ironclad Cove.", GOSSIP_SENDER_DEADMINES_PORT, 3);
                else if (instance->GetBossState(DATA_FOEREAPER) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to The Foundry.", GOSSIP_SENDER_DEADMINES_PORT, 2);
                else if (instance->GetBossState(DATA_HELIX) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to The Mast Room.", GOSSIP_SENDER_DEADMINES_PORT, 1);
            }

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();
            player->CLOSE_GOSSIP_MENU();

            if (action >= 4)
                return false;

            Position const& loc = deadmines_locs[action];
            if (!player->IsInCombat())
                player->NearTeleportTo(loc.GetPositionX(), loc.GetPositionY(), loc.GetPositionZ(), loc.GetOrientation(), false);

            return true;
        }
};

// Off-line 88348
class spell_deadmines_off_line : public SpellScriptLoader
{
    public:
        spell_deadmines_off_line() : SpellScriptLoader("spell_deadmines_off_line") { }

        class spell_deadmines_off_line_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_deadmines_off_line_AuraScript);

            void OnAuraEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* m_caster = GetOwner()->ToCreature())
                    m_caster->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
            }

            void OnAuraEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                // The template keeps Foe Reaper 5000 immune to players until he comes on-line
                if (Creature* m_caster = GetOwner()->ToCreature())
                    m_caster->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_deadmines_off_line_AuraScript::OnAuraEffectApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_deadmines_off_line_AuraScript::OnAuraEffectRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_deadmines_off_line_AuraScript();
        }
};

void AddSC_deadmines()
{
    new creature_script<npc_deadmines_prototype_reaper>("npc_deadmines_prototype_reaper");
    new go_defias_cannon();
    new go_deadmines_teleport();
    new spell_deadmines_off_line();
    RegisterCreatureAI(npc_deadmines_defias_watcher);
    RegisterAuraScript(spell_deadmines_on_fire);
}
