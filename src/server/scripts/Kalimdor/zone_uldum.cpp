/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Creature.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SmartAI.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "TemporarySummon.h"

enum GobblesRitual
{
    // Creatures
    NPC_COMMANDER_SCHNOTTZ      = 47159,
    NPC_SCHNOTTZ_RITUAL         = 47249,
    NPC_BELLOC_BRIGHTBLADE      = 47251,
    NPC_GOBBLES_RITUAL          = 47255,

    // Spells
    SPELL_SUMMON_SCHNOTTZ       = 88108,
    SPELL_SUMMON_BELLOC         = 88109,
    SPELL_PHASE_PLAYER          = 88111,

    // Phase IDs
    PHASE_GOBBLES_RITUAL        = 312,

    ACTOR_SEARCH_RANGE          = 200,
    ACTOR_DESPAWN_DELAY         = 1000
};

// 88107 - Gobbles Initialize
class spell_gobbles_initialize : public SpellScript
{
    PrepareSpellScript(spell_gobbles_initialize);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (Player* player = GetHitPlayer())
        {
            player->CastSpell(player, SPELL_SUMMON_SCHNOTTZ, true);
            player->CastSpell(player, SPELL_SUMMON_BELLOC, true);
            player->CastSpell(player, SPELL_PHASE_PLAYER, true);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_gobbles_initialize::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 88108 - Summon Schnottz
class spell_summon_schnottz : public SpellScript
{
    PrepareSpellScript(spell_summon_schnottz);

    void SetDest(SpellDestination& dest)
    {
        if (Creature* schnottz = GetCaster()->FindNearestCreature(NPC_COMMANDER_SCHNOTTZ, 10.0f, true))
            dest.Relocate(*schnottz);
    }

    void Register() override
    {
        OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_summon_schnottz::SetDest, EFFECT_0, TARGET_DEST_NEARBY_ENTRY);
    }
};

// 88111 - Phase Player
class spell_gobbles_phase_player : public AuraScript
{
    PrepareAuraScript(spell_gobbles_phase_player);

    // The Landing's first chapter excludes players with this aura, but area phases are only refreshed on area and quest changes
    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->UpdateAreaPhase();
    }

    // The ritual ends with the aura however it ends (cleanup, leaving the area, expiry), so the actors go with it
    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* player = GetTarget()->ToPlayer();
        if (!player || !player->IsInWorld())
            return;

        player->UpdateAreaPhase();

        for (uint32 entry : { NPC_SCHNOTTZ_RITUAL, NPC_BELLOC_BRIGHTBLADE, NPC_GOBBLES_RITUAL })
        {
            std::list<Creature*> actors;
            player->GetCreatureListWithEntryInGrid(actors, entry, ACTOR_SEARCH_RANGE);
            for (Creature* actor : actors)
                if (actor->ToTempSummon() && actor->ToTempSummon()->GetSummonerGUID() == player->GetGUID())
                    actor->DespawnOrUnsummon(ACTOR_DESPAWN_DELAY);
        }
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_gobbles_phase_player::HandleApply, EFFECT_0, SPELL_AURA_PHASE, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_gobbles_phase_player::HandleRemove, EFFECT_0, SPELL_AURA_PHASE, AURA_EFFECT_HANDLE_REAL);
    }
};

// Summons copy the phases of the player who cast the ritual, which would show the actors to everyone at the Landing
struct npc_gobbles_ritual_actor : public SmartAI
{
    npc_gobbles_ritual_actor(Creature* creature) : SmartAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        me->ClearPhases();
        me->SetPhased(PHASE_GOBBLES_RITUAL, true, true);
        SmartAI::IsSummonedBy(summoner);
    }
};

// A ritual aura saved at logout would come back without its actors
class player_gobbles_ritual : public PlayerScript
{
public:
    player_gobbles_ritual() : PlayerScript("player_gobbles_ritual") { }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        player->RemoveAurasDueToSpell(SPELL_PHASE_PLAYER);
    }
};

void AddSC_uldum()
{
    RegisterSpellScript(spell_gobbles_initialize);
    RegisterSpellScript(spell_summon_schnottz);
    RegisterAuraScript(spell_gobbles_phase_player);
    RegisterCreatureAI(npc_gobbles_ritual_actor);
    new player_gobbles_ritual();
}
