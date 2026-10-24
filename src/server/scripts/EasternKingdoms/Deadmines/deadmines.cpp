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

// Prototype Reaper 49208
struct npc_deadmines_prototype_reaper : public ScriptedAI
{
    npc_deadmines_prototype_reaper(Creature* creature) : ScriptedAI(creature) { }

    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
    {
        if (HealthBelowPct(90) && me->GetMap()->GetWorldState(WORLDSTATE_PROTOTYPE_PRODIGY))
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
                if (Creature* m_caster = GetOwner()->ToCreature())
                    m_caster->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
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
}
