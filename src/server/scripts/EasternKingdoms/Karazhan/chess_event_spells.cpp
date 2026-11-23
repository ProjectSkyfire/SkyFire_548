/*
* This file is part of the Legends of Azeroth Pandaria Project. See THANKS file for Copyright information
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

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Creature.h"
#include "InstanceScript.h"
#include "Player.h"
#include "karazhan.h"

enum ChessEventSpells
{
    SPELL_MOVE_GENERIC              = 30012,
    SPELL_MOVE_TO_SQUARE            = 30253,
    SPELL_FACE_SQUARE               = 30270,
    SPELL_IS_SQUARE_OCCUPIED        = 39400,
    SPELL_CHESS_AI_ATTACK_TIMER     = 32226,
    SPELL_MELEE_DAMAGE              = 32247,
    SPELL_POISON_CLOUD_ACTION       = 37775,
    SPELL_POISON_CLOUD              = 37469,
    SPELL_SHADOW_MEND               = 37456,
    SPELL_HAND_OF_MEDIVH            = 39339,
    SPELL_KILL_CHEST_BUNNY          = 45259,
    SPELL_BOARD_VISUAL              = 39390,

    CHANCE_VICTORY_VISUAL           = 10,
    DESPAWN_DELAY_CLEAR_BOARD       = 10000
};

// 37144 - Move (knight), 37146 - Move (pawn), 37148 - Move (queen), 37151 - Move (rook), 37152 - Move (bishop), 37153 - Move (king)
class spell_chess_player_move : public SpellScript
{
    PrepareSpellScript(spell_chess_player_move);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit();
        if (!target || target->GetTypeId() != TypeID::TYPEID_UNIT)
            return;

        GetCaster()->CastSpell(target, SPELL_MOVE_GENERIC, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_chess_player_move::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 30012 - Move
class spell_chess_move : public SpellScript
{
    PrepareSpellScript(spell_chess_move);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit();
        if (GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER || !target || target->HasAura(SPELL_IS_SQUARE_OCCUPIED))
            return;

        // the square answers the piece
        target->CastSpell(GetCaster(), SPELL_MOVE_TO_SQUARE, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_chess_move::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 30284 - Change Facing
class spell_chess_change_facing : public SpellScript
{
    PrepareSpellScript(spell_chess_change_facing);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            target->CastSpell(GetCaster(), SPELL_FACE_SQUARE, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_chess_change_facing::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 32227, 32228, 37142, 37143, 37147, 37149, 37150, 37220, 37337, 37339, 37345, 37348 - Melee Attack
class spell_chess_melee_action : public SpellScript
{
    PrepareSpellScript(spell_chess_melee_action);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            GetCaster()->CastSpell(target, SPELL_MELEE_DAMAGE, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_chess_melee_action::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 37775 - Poison Cloud (action), 37824 - Shadow Mend (action)
class spell_chess_action_target : public SpellScript
{
    PrepareSpellScript(spell_chess_action_target);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            GetCaster()->CastSpell(target, GetSpellInfo()->Id == SPELL_POISON_CLOUD_ACTION ? SPELL_POISON_CLOUD : SPELL_SHADOW_MEND, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_chess_action_target::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 39338, 39342 - Hand of Medivh
class spell_chess_hand_of_medivh : public SpellScript
{
    PrepareSpellScript(spell_chess_hand_of_medivh);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            GetCaster()->CastSpell(target, SPELL_HAND_OF_MEDIVH, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_chess_hand_of_medivh::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 39341, 39344 - Fury of Medivh
class spell_chess_fury_of_medivh : public SpellScript
{
    PrepareSpellScript(spell_chess_fury_of_medivh);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        // the base points hold the spell that summons the fire
        if (Unit* target = GetHitUnit())
            GetCaster()->CastSpell(target, GetEffectValue(), true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_chess_fury_of_medivh::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 45260 - Force Player to Kill Bunny
class spell_chess_force_kill_bunny : public SpellScript
{
    PrepareSpellScript(spell_chess_force_kill_bunny);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Player* target = GetHitPlayer())
            target->CastSpell(target, SPELL_KILL_CHEST_BUNNY, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_chess_force_kill_bunny::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 37366, 39398 - Clear Board
class spell_chess_clear_board : public SpellScript
{
    PrepareSpellScript(spell_chess_clear_board);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        Creature* target = GetHitCreature();
        if (!target)
            return;

        target->DespawnOrUnsummon(DESPAWN_DELAY_CLEAR_BOARD);
        target->RemoveAurasDueToSpell(SPELL_CHESS_AI_ATTACK_TIMER);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_chess_clear_board::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 39395 - Victory Visual Controller
class spell_chess_victory_visual : public SpellScript
{
    PrepareSpellScript(spell_chess_victory_visual);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit();
        if (!target || !roll_chance_i(CHANCE_VICTORY_VISUAL))
            return;

        // moves the dummy to a random square for the next flash
        std::list<Creature*> squares;
        target->GetCreatureListWithEntryInGrid(squares, NPC_SQUARE_WHITE, 150.0f);
        target->GetCreatureListWithEntryInGrid(squares, NPC_SQUARE_BLACK, 150.0f);
        if (squares.empty())
            return;

        Position pos = Skyfire::Containers::SelectRandomContainerElement(squares)->GetPosition();
        pos.RelocateOffset(frand(0.0f, 2 * M_PI), frand(0.0f, 2.0f));
        target->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), target->GetOrientation());
        target->CastSpell(target, SPELL_BOARD_VISUAL, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_chess_victory_visual::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 39331 - Game In Session
class spell_game_in_session : public AuraScript
{
    PrepareAuraScript(spell_game_in_session);

    bool CheckAreaTarget(Unit* target)
    {
        switch (target->GetEntry())
        {
            case NPC_ORC_GRUNT:
            case NPC_ORC_WOLF:
            case NPC_ORC_WARLOCK:
            case NPC_ORC_NECROLYTE:
            case NPC_SUMMONED_DAEMON:
            case NPC_WARCHIEF_BLACKHAND:
            case NPC_HUMAN_FOOTMAN:
            case NPC_HUMAN_CHARGER:
            case NPC_HUMAN_CONJURER:
            case NPC_HUMAN_CLERIC:
            case NPC_CONJURED_WATER_ELEMENTAL:
            case NPC_KING_LLANE:
                return false;
            default:
                // only the players that watch the game are affected, not the pieces they control
                if (target->IsControlledByPlayer())
                    if (InstanceScript* instance = target->GetInstanceScript())
                        return instance->GetData(TYPE_CHESS) == IN_PROGRESS || instance->GetData(TYPE_CHESS) == SPECIAL;
                return false;
        }
    }

    void Register() OVERRIDE
    {
        DoCheckAreaTarget += AuraCheckAreaTargetFn(spell_game_in_session::CheckAreaTarget);
    }
};

void AddSC_chess_event_spells()
{
    RegisterSpellScript(spell_chess_player_move);
    RegisterSpellScript(spell_chess_move);
    RegisterSpellScript(spell_chess_change_facing);
    RegisterSpellScript(spell_chess_melee_action);
    RegisterSpellScript(spell_chess_action_target);
    RegisterSpellScript(spell_chess_hand_of_medivh);
    RegisterSpellScript(spell_chess_fury_of_medivh);
    RegisterSpellScript(spell_chess_force_kill_bunny);
    RegisterSpellScript(spell_chess_clear_board);
    RegisterSpellScript(spell_chess_victory_visual);
    RegisterAuraScript(spell_game_in_session);
}
