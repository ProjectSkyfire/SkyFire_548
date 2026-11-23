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
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "karazhan.h"

enum ChessEventMisc
{
    // texts of Echo of Medivh
    EMOTE_LIFT_CURSE                = 0,
    EMOTE_CHEAT                     = 1,

    SOUND_ID_LOSE_PAWN_PLAYER_1     = 10339,
    SOUND_ID_LOSE_PAWN_PLAYER_2     = 10340,
    SOUND_ID_LOSE_PAWN_PLAYER_3     = 10341,
    SOUND_ID_LOSE_PAWN_MEDIVH_1     = 10342,
    SOUND_ID_LOSE_PAWN_MEDIVH_2     = 10343,
    SOUND_ID_LOSE_PAWN_MEDIVH_3     = 10344,
    SOUND_ID_LOSE_ROOK_PLAYER       = 10345,
    SOUND_ID_LOSE_ROOK_MEDIVH       = 10346,
    SOUND_ID_LOSE_BISHOP_PLAYER     = 10347,
    SOUND_ID_LOSE_BISHOP_MEDIVH     = 10348,
    SOUND_ID_LOSE_KNIGHT_PLAYER     = 10349,
    SOUND_ID_LOSE_KNIGHT_MEDIVH     = 10350,
    SOUND_ID_LOSE_QUEEN_PLAYER      = 10351,
    SOUND_ID_LOSE_QUEEN_MEDIVH      = 10352,
    SOUND_ID_CHECK_PLAYER           = 10353,
    SOUND_ID_CHECK_MEDIVH           = 10354,
    SOUND_ID_WIN_PLAYER             = 10355,
    SOUND_ID_WIN_MEDIVH             = 10356,
    SOUND_ID_CHEAT_1                = 10357,
    SOUND_ID_CHEAT_2                = 10358,
    SOUND_ID_CHEAT_3                = 10359,

    // movement
    SPELL_MOVE_GENERIC              = 30012, // sends the move signal to a square, see chess_event_spells.cpp
    SPELL_CHANGE_FACING             = 30284, // sends the facing request to a square
    SPELL_FACE_SQUARE               = 30270, // answer of the square to the facing request
    SPELL_MOVE_TO_SQUARE            = 30253, // answer of the square to the move signal
    SPELL_MOVE_COOLDOWN             = 30543,
    SPELL_MOVE_MARKER               = 32261, // white beam, marks the movement as complete
    SPELL_DISABLE_SQUARE            = 32745, // cast on a square when a piece moves onto it
    SPELL_IS_SQUARE_OCCUPIED        = 39400,

    // generic
    SPELL_IN_GAME                   = 30532, // teleports the player near the entrance
    SPELL_RECENTLY_IN_GAME          = 30529,
    SPELL_CHESS_AI_ATTACK_TIMER     = 32226, // triggers the melee action 32225
    SPELL_ACTION_MELEE              = 32225,

    // melee actions
    SPELL_MELEE_FOOTMAN             = 32227,
    SPELL_MELEE_WATER_ELEM          = 37142,
    SPELL_MELEE_CHARGER             = 37143,
    SPELL_MELEE_CLERIC              = 37147,
    SPELL_MELEE_CONJURER            = 37149,
    SPELL_MELEE_KING_LLANE          = 37150,
    SPELL_MELEE_GRUNT               = 32228,
    SPELL_MELEE_DAEMON              = 37220,
    SPELL_MELEE_NECROLYTE           = 37337,
    SPELL_MELEE_WOLF                = 37339,
    SPELL_MELEE_WARLOCK             = 37345,
    SPELL_MELEE_WARCHIEF_BLACKHAND  = 37348,

    // cheats of Medivh
    SPELL_HAND_OF_MEDIVH_HORDE      = 39338,
    SPELL_HAND_OF_MEDIVH_ALLIANCE   = 39342,
    SPELL_FURY_OF_MEDIVH_HORDE      = 39341,
    SPELL_FURY_OF_MEDIVH_ALLIANCE   = 39344,
    SPELL_FURY_OF_MEDIVH_AURA       = 39383,
    SPELL_FURY_OF_MEDIVH_INFERNO    = 39346,

    // spells of the pieces
    SPELL_HEROISM                   = 37471, // human king
    SPELL_SWEEP                     = 37474,
    SPELL_BLOODLUST                 = 37472, // orc king
    SPELL_CLEAVE                    = 37476,
    SPELL_HEROIC_BLOW               = 37406, // human pawn
    SPELL_SHIELD_BLOCK              = 37414,
    SPELL_VICIOUS_STRIKE            = 37413, // orc pawn
    SPELL_WEAPON_DEFLECTION         = 37416,
    SPELL_SMASH                     = 37453, // human knight
    SPELL_STOMP                     = 37498,
    SPELL_BITE                      = 37454, // orc knight
    SPELL_HOWL                      = 37502,
    SPELL_ELEMENTAL_BLAST           = 37462, // human queen
    SPELL_RAIN_OF_FIRE              = 37465,
    SPELL_FIREBALL                  = 37463, // orc queen
    SPELL_POISON_CLOUD_ACTION       = 37775, // target selector for Poison Cloud
    SPELL_HEALING                   = 37455, // human bishop
    SPELL_HOLY_LANCE                = 37459,
    SPELL_SHADOW_MEND_ACTION        = 37824, // target selector for Shadow Mend, orc bishop
    SPELL_SHADOW_SPEAR              = 37461,
    SPELL_GEYSER                    = 37427, // human rook
    SPELL_WATER_SHIELD              = 37432,
    SPELL_HELLFIRE                  = 37428, // orc rook
    SPELL_FIRE_SHIELD               = 37434,

    // transform the stalker next to the board when a piece dies
    SPELL_TRANSFORM_FOOTMAN         = 39350,
    SPELL_TRANSFORM_CHARGER         = 39352,
    SPELL_TRANSFORM_CLERIC          = 39353,
    SPELL_TRANSFORM_WATER_ELEM      = 39354,
    SPELL_TRANSFORM_CONJURER        = 39355,
    SPELL_TRANSFORM_KING_LLANE      = 39356,
    SPELL_TRANSFORM_GRUNT           = 39357,
    SPELL_TRANSFORM_WOLF            = 39358,
    SPELL_TRANSFORM_NECROLYTE       = 39359,
    SPELL_TRANSFORM_DAEMON          = 39360,
    SPELL_TRANSFORM_WARLOCK         = 39361,
    SPELL_TRANSFORM_BLACKHAND       = 39362,

    NPC_FURY_MEDIVH_VISUAL          = 22521, // has aura 39383

    TARGET_TYPE_RANDOM              = 1,
    TARGET_TYPE_FRIENDLY            = 2,
    TARGET_TYPE_ADJACENT            = 3
};

struct ChessPieceInfo
{
    uint32 Entry;
    bool IsKing;
    uint32 PieceTeam;
    uint32 Faction;

    float MoveRadius;
    uint32 MoveChanceIncrement;
    uint32 MoveEmote;
    uint32 MoveSound;

    uint8 PrimaryTarget;
    float PrimaryRadius;
    float PrimaryArc;
    uint32 PrimarySpell;

    uint8 SecondaryTarget;
    float SecondaryRadius;
    float SecondaryArc;
    uint32 SecondarySpell;

    uint32 MeleeSpell;
    uint32 TransformSpell;

    uint32 LoseSoundPlayer[3];
    uint32 LoseSoundMedivh[3];
};

ChessPieceInfo const ChessPieces[] =
{
    { NPC_KING_LLANE,               true,   ALLIANCE, FACTION_ID_CHESS_ALLIANCE,   8.0f, 1, EMOTE_ONESHOT_YES,    6164, TARGET_TYPE_RANDOM,   20.0f,   0, SPELL_HEROISM,             TARGET_TYPE_RANDOM,  10.0f, 180, SPELL_SWEEP,             SPELL_MELEE_KING_LLANE,         SPELL_TRANSFORM_KING_LLANE, { SOUND_ID_WIN_MEDIVH, SOUND_ID_WIN_MEDIVH, SOUND_ID_WIN_MEDIVH },                         { SOUND_ID_WIN_PLAYER, SOUND_ID_WIN_PLAYER, SOUND_ID_WIN_PLAYER } },
    { NPC_WARCHIEF_BLACKHAND,       true,   HORDE,    FACTION_ID_CHESS_HORDE,      8.0f, 1, EMOTE_ONESHOT_ROAR,   2694, TARGET_TYPE_RANDOM,   20.0f,   0, SPELL_BLOODLUST,           TARGET_TYPE_RANDOM,  10.0f, 180, SPELL_CLEAVE,            SPELL_MELEE_WARCHIEF_BLACKHAND, SPELL_TRANSFORM_BLACKHAND,  { SOUND_ID_WIN_MEDIVH, SOUND_ID_WIN_MEDIVH, SOUND_ID_WIN_MEDIVH },                         { SOUND_ID_WIN_PLAYER, SOUND_ID_WIN_PLAYER, SOUND_ID_WIN_PLAYER } },
    { NPC_HUMAN_CONJURER,           false,  ALLIANCE, FACTION_ID_CHESS_ALLIANCE,  20.0f, 3, EMOTE_ONESHOT_YES,    6164, TARGET_TYPE_RANDOM,   25.0f, 180, SPELL_RAIN_OF_FIRE,        TARGET_TYPE_RANDOM,  20.0f, 180, SPELL_ELEMENTAL_BLAST,   SPELL_MELEE_CONJURER,           SPELL_TRANSFORM_CONJURER,   { SOUND_ID_LOSE_QUEEN_PLAYER, SOUND_ID_LOSE_QUEEN_PLAYER, SOUND_ID_LOSE_QUEEN_PLAYER }, { SOUND_ID_LOSE_QUEEN_MEDIVH, SOUND_ID_LOSE_QUEEN_MEDIVH, SOUND_ID_LOSE_QUEEN_MEDIVH } },
    { NPC_ORC_WARLOCK,              false,  HORDE,    FACTION_ID_CHESS_HORDE,     20.0f, 3, EMOTE_ONESHOT_YES,    6024, TARGET_TYPE_RANDOM,   25.0f, 180, SPELL_POISON_CLOUD_ACTION, TARGET_TYPE_RANDOM,  20.0f, 180, SPELL_FIREBALL,          SPELL_MELEE_WARLOCK,            SPELL_TRANSFORM_WARLOCK,    { SOUND_ID_LOSE_QUEEN_PLAYER, SOUND_ID_LOSE_QUEEN_PLAYER, SOUND_ID_LOSE_QUEEN_PLAYER }, { SOUND_ID_LOSE_QUEEN_MEDIVH, SOUND_ID_LOSE_QUEEN_MEDIVH, SOUND_ID_LOSE_QUEEN_MEDIVH } },
    { NPC_HUMAN_CLERIC,             false,  ALLIANCE, FACTION_ID_CHESS_ALLIANCE,   8.0f, 2, EMOTE_ONESHOT_BOW,    0,    TARGET_TYPE_FRIENDLY, 25.0f, 360, SPELL_HEALING,             TARGET_TYPE_RANDOM,  18.0f,  20, SPELL_HOLY_LANCE,        SPELL_MELEE_CLERIC,             SPELL_TRANSFORM_CLERIC,     { SOUND_ID_LOSE_BISHOP_PLAYER, SOUND_ID_LOSE_BISHOP_PLAYER, SOUND_ID_LOSE_BISHOP_PLAYER }, { SOUND_ID_LOSE_BISHOP_MEDIVH, SOUND_ID_LOSE_BISHOP_MEDIVH, SOUND_ID_LOSE_BISHOP_MEDIVH } },
    { NPC_ORC_NECROLYTE,            false,  HORDE,    FACTION_ID_CHESS_HORDE,      8.0f, 2, EMOTE_ONESHOT_NONE,   6026, TARGET_TYPE_FRIENDLY, 25.0f, 360, SPELL_SHADOW_MEND_ACTION,  TARGET_TYPE_RANDOM,  18.0f,  20, SPELL_SHADOW_SPEAR,      SPELL_MELEE_NECROLYTE,          SPELL_TRANSFORM_NECROLYTE,  { SOUND_ID_LOSE_BISHOP_PLAYER, SOUND_ID_LOSE_BISHOP_PLAYER, SOUND_ID_LOSE_BISHOP_PLAYER }, { SOUND_ID_LOSE_BISHOP_MEDIVH, SOUND_ID_LOSE_BISHOP_MEDIVH, SOUND_ID_LOSE_BISHOP_MEDIVH } },
    { NPC_CONJURED_WATER_ELEMENTAL, false,  ALLIANCE, FACTION_ID_CHESS_ALLIANCE,   8.0f, 3, EMOTE_ONESHOT_NONE,   0,    TARGET_TYPE_RANDOM,    9.0f, 180, SPELL_GEYSER,              TARGET_TYPE_RANDOM,   9.0f, 180, SPELL_WATER_SHIELD,      SPELL_MELEE_WATER_ELEM,         SPELL_TRANSFORM_WATER_ELEM, { SOUND_ID_LOSE_ROOK_PLAYER, SOUND_ID_LOSE_ROOK_PLAYER, SOUND_ID_LOSE_ROOK_PLAYER },       { SOUND_ID_LOSE_ROOK_MEDIVH, SOUND_ID_LOSE_ROOK_MEDIVH, SOUND_ID_LOSE_ROOK_MEDIVH } },
    { NPC_SUMMONED_DAEMON,          false,  HORDE,    FACTION_ID_CHESS_HORDE,      8.0f, 3, EMOTE_ONESHOT_NONE,   9105, TARGET_TYPE_RANDOM,    9.0f, 180, SPELL_HELLFIRE,            TARGET_TYPE_RANDOM,   9.0f, 180, SPELL_FIRE_SHIELD,       SPELL_MELEE_DAEMON,             SPELL_TRANSFORM_DAEMON,     { SOUND_ID_LOSE_ROOK_PLAYER, SOUND_ID_LOSE_ROOK_PLAYER, SOUND_ID_LOSE_ROOK_PLAYER },       { SOUND_ID_LOSE_ROOK_MEDIVH, SOUND_ID_LOSE_ROOK_MEDIVH, SOUND_ID_LOSE_ROOK_MEDIVH } },
    { NPC_HUMAN_CHARGER,            false,  ALLIANCE, FACTION_ID_CHESS_ALLIANCE,  15.0f, 4, EMOTE_ONESHOT_NONE,   729,  TARGET_TYPE_RANDOM,    8.0f,  63, SPELL_SMASH,               TARGET_TYPE_RANDOM,  10.0f, 100, SPELL_STOMP,             SPELL_MELEE_CHARGER,            SPELL_TRANSFORM_CHARGER,    { SOUND_ID_LOSE_KNIGHT_PLAYER, SOUND_ID_LOSE_KNIGHT_PLAYER, SOUND_ID_LOSE_KNIGHT_PLAYER }, { SOUND_ID_LOSE_KNIGHT_MEDIVH, SOUND_ID_LOSE_KNIGHT_MEDIVH, SOUND_ID_LOSE_KNIGHT_MEDIVH } },
    { NPC_ORC_WOLF,                 false,  HORDE,    FACTION_ID_CHESS_HORDE,     15.0f, 4, EMOTE_ONESHOT_NONE,   390,  TARGET_TYPE_RANDOM,    8.0f,  63, SPELL_BITE,                TARGET_TYPE_RANDOM,  10.0f, 100, SPELL_HOWL,              SPELL_MELEE_WOLF,               SPELL_TRANSFORM_WOLF,       { SOUND_ID_LOSE_KNIGHT_PLAYER, SOUND_ID_LOSE_KNIGHT_PLAYER, SOUND_ID_LOSE_KNIGHT_PLAYER }, { SOUND_ID_LOSE_KNIGHT_MEDIVH, SOUND_ID_LOSE_KNIGHT_MEDIVH, SOUND_ID_LOSE_KNIGHT_MEDIVH } },
    { NPC_HUMAN_FOOTMAN,            false,  ALLIANCE, FACTION_ID_CHESS_ALLIANCE,   8.0f, 5, EMOTE_ONESHOT_SALUTE, 6164, TARGET_TYPE_RANDOM,    8.0f,  63, SPELL_HEROIC_BLOW,         TARGET_TYPE_RANDOM,   8.0f, 180, SPELL_SHIELD_BLOCK,      SPELL_MELEE_FOOTMAN,            SPELL_TRANSFORM_FOOTMAN,    { SOUND_ID_LOSE_PAWN_PLAYER_1, SOUND_ID_LOSE_PAWN_PLAYER_2, SOUND_ID_LOSE_PAWN_PLAYER_3 }, { SOUND_ID_LOSE_PAWN_MEDIVH_1, SOUND_ID_LOSE_PAWN_MEDIVH_2, SOUND_ID_LOSE_PAWN_MEDIVH_3 } },
    { NPC_ORC_GRUNT,                false,  HORDE,    FACTION_ID_CHESS_HORDE,      8.0f, 5, EMOTE_ONESHOT_SALUTE, 6025, TARGET_TYPE_RANDOM,    8.0f,  63, SPELL_VICIOUS_STRIKE,      TARGET_TYPE_RANDOM,   8.0f, 180, SPELL_WEAPON_DEFLECTION, SPELL_MELEE_GRUNT,              SPELL_TRANSFORM_GRUNT,      { SOUND_ID_LOSE_PAWN_PLAYER_1, SOUND_ID_LOSE_PAWN_PLAYER_2, SOUND_ID_LOSE_PAWN_PLAYER_3 }, { SOUND_ID_LOSE_PAWN_MEDIVH_1, SOUND_ID_LOSE_PAWN_MEDIVH_2, SOUND_ID_LOSE_PAWN_MEDIVH_3 } }
};

static ChessPieceInfo const* GetChessPieceInfo(uint32 entry)
{
    for (ChessPieceInfo const& info : ChessPieces)
        if (info.Entry == entry)
            return &info;

    return nullptr;
}

class npc_echo_of_medivh : public CreatureScript
{
    public:
        npc_echo_of_medivh() : CreatureScript("npc_echo_of_medivh") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            InstanceScript* instance = creature->GetInstanceScript();
            if (!instance)
                return false;

            // offers to end the friendly game
            if (instance->GetData(TYPE_CHESS) == SPECIAL)
                player->ADD_GOSSIP_ITEM_DB(player->GetDefaultGossipMenuForSource(creature), 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (InstanceScript* instance = creature->GetInstanceScript())
                    if (instance->GetData(TYPE_CHESS) == SPECIAL)
                        instance->SetData(TYPE_CHESS, DONE);

                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        struct npc_echo_of_medivhAI : public ScriptedAI
        {
            npc_echo_of_medivhAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                Reset();
            }

            void Reset() OVERRIDE
            {
                cheatTimer = 90000;
                cheatEffectTimer = 0;
                movementTimer = 1000;
                me->SetWalk(true);
            }

            void MoveInLineOfSight(Unit* /*who*/) OVERRIDE { }
            void AttackStart(Unit* /*who*/) OVERRIDE { }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                if (summon->GetEntry() == NPC_FURY_MEDIVH_VISUAL)
                {
                    summon->CastSpell(summon, SPELL_FURY_OF_MEDIVH_AURA, true);
                    summon->CastSpell(summon, SPELL_FURY_OF_MEDIVH_INFERNO, true);
                }
            }

            void MovementInform(uint32 type, uint32 point) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE && point)
                    movementTimer = urand(1000, 5000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (movementTimer)
                {
                    if (movementTimer <= diff)
                    {
                        Position dest = { -11092.44f, -1849.69f, 221.07f, 3.79f };
                        dest.RelocateOffset(0.0f, frand(0.0f, 13.0f));
                        dest.RelocateOffset(frand(0.0f, 2 * M_PI), frand(0.0f, 3.0f));
                        me->GetMotionMaster()->MovePoint(1, dest);

                        movementTimer = 0;
                    }
                    else
                        movementTimer -= diff;
                }

                if (!instance || instance->GetData(TYPE_CHESS) != IN_PROGRESS)
                {
                    cheatTimer = 90000;
                    cheatEffectTimer = 0;
                    return;
                }

                if (cheatTimer <= diff)
                {
                    DoPlaySoundToSet(me, SOUND_ID_CHEAT_1 + urand(0, 2));
                    cheatTimer = 90000;
                    cheatEffectTimer = 2500;
                }
                else
                    cheatTimer -= diff;

                if (cheatEffectTimer)
                {
                    if (cheatEffectTimer <= diff)
                    {
                        // the cheats always hit the side of the players
                        bool alliance = dynamic_cast<karazhan_accessor*>(instance)->GetPlayerTeam() == ALLIANCE;
                        if (urand(0, 1))
                            DoCast(me, alliance ? SPELL_HAND_OF_MEDIVH_HORDE : SPELL_HAND_OF_MEDIVH_ALLIANCE);
                        else
                            DoCast(me, alliance ? SPELL_FURY_OF_MEDIVH_ALLIANCE : SPELL_FURY_OF_MEDIVH_HORDE);

                        Talk(EMOTE_CHEAT);
                        cheatEffectTimer = 0;
                    }
                    else
                        cheatEffectTimer -= diff;
                }
            }

        private:
            InstanceScript* instance;

            uint32 cheatTimer;
            uint32 cheatEffectTimer;
            uint32 movementTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_echo_of_medivhAI(creature);
        }
};

class npc_chess_piece : public CreatureScript
{
    public:
        npc_chess_piece() : CreatureScript("npc_chess_piece") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            InstanceScript* instance = creature->GetInstanceScript();
            karazhan_accessor* accessor = dynamic_cast<karazhan_accessor*>(instance);

            if (accessor && !player->HasAura(SPELL_RECENTLY_IN_GAME) && !player->HasAura(SPELL_CONTROL_PIECE) && !creature->HasAura(SPELL_CONTROL_PIECE))
            {
                // a player controls the pieces of his own side, after the win every piece is free
                uint32 state = instance->GetData(TYPE_CHESS);
                bool ownPiece = state != DONE && state != FAIL && player->GetTeam() == creature->AI()->GetData(1);
                if (ownPiece || accessor->IsFriendlyGameReady())
                    player->ADD_GOSSIP_ITEM_DB(player->GetDefaultGossipMenuForSource(creature), 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            }

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (InstanceScript* instance = creature->GetInstanceScript())
                {
                    if (instance->GetData(TYPE_CHESS) == NOT_STARTED)
                        instance->SetData(TYPE_CHESS, IN_PROGRESS);
                    else if (instance->GetData(TYPE_CHESS) == DONE)
                        instance->SetData(TYPE_CHESS, SPECIAL);

                    // teleport to the entrance and control the piece
                    player->CastSpell(player, SPELL_IN_GAME, true);
                    player->CastSpell(creature, SPELL_CONTROL_PIECE, true);
                }

                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        bool OnDummyEffect(Unit* caster, uint32 spellId, SpellEffIndex effIndex, Creature* target) OVERRIDE
        {
            if (effIndex != EFFECT_0)
                return false;

            switch (spellId)
            {
                // answer of a square to the move signal
                case SPELL_MOVE_TO_SQUARE:
                    if (caster->GetTypeId() == TypeID::TYPEID_UNIT && !caster->HasAura(SPELL_DISABLE_SQUARE) && !caster->HasAura(SPELL_IS_SQUARE_OCCUPIED))
                    {
                        caster->CastSpell(caster, SPELL_DISABLE_SQUARE, true);
                        caster->CastSpell(caster, SPELL_IS_SQUARE_OCCUPIED, true);

                        target->CastSpell(target, SPELL_MOVE_COOLDOWN);
                        target->AddCreatureSpellCooldown(SPELL_MOVE_COOLDOWN);
                        target->AI()->SetGUID(caster->GetGUID(), 1);
                    }
                    return true;
                // melee tick of the piece
                case SPELL_ACTION_MELEE:
                    if (!target->GetMotionMaster()->GetMotionSlot(MOTION_SLOT_ACTIVE))
                        target->SetFacingTo(target->GetOrientation());

                    target->CastSpell(target, target->AI()->GetData(0), true);
                    return true;
                // answer of a square to the facing request
                case SPELL_FACE_SQUARE:
                    if (caster->GetTypeId() != TypeID::TYPEID_UNIT)
                        return true;

                    if (Creature* square = ObjectAccessor::GetCreature(*target, target->AI()->GetGUID()))
                    {
                        if (caster->GetExactDist2d(square) > 8.0f)
                            return true;

                        // only the neighbouring squares of the opposite colour count
                        if (caster->GetEntry() != (square->GetEntry() == NPC_SQUARE_WHITE ? NPC_SQUARE_BLACK : NPC_SQUARE_WHITE) &&
                            caster->GetEntry() != (square->GetEntry() == NPC_SQUARE_WHITE ? NPC_SQUARE_OUTSIDE_BLACK : NPC_SQUARE_OUTSIDE_WHITE))
                            return true;

                        target->SetFacingTo(square->GetAngle(caster));
                        target->AI()->DoAction(0);
                    }
                    return true;
                default:
                    return false;
            }
        }

        struct npc_chess_pieceAI : public ScriptedAI
        {
            npc_chess_pieceAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                accessor = dynamic_cast<karazhan_accessor*>(instance);
                pieceInfo = GetChessPieceInfo(me->GetEntry());

                currentSquareGuid = 0;
                isAttacked = false;
                isUncharming = false;
                isAlternatingSpells = false;

                if (pieceInfo)
                {
                    SpellInfo const* spellPrimary = sSpellMgr->GetSpellInfo(pieceInfo->PrimarySpell);
                    SpellInfo const* spellSecondary = sSpellMgr->GetSpellInfo(pieceInfo->SecondarySpell);
                    isAlternatingSpells = spellPrimary && spellSecondary && !spellPrimary->RecoveryTime && !spellSecondary->RecoveryTime;
                }

                Reset();
            }

            void Reset() OVERRIDE
            {
                if (!me->IsVisible())
                {
                    Position curPos = me->GetPosition();

                    me->Relocate(me->GetHomePosition());
                    me->SetVisible(true);
                    me->Relocate(curPos);
                }

                if (!isUncharming)
                {
                    moveTimer = 0;
                    isPrimarySpell = true;
                    currentOrientation = me->GetOrientation();
                }
                moveCommandTimer = facingCommandTimer = 1000;
                spellPrimaryTimer = spellSecondaryTimer = me->HasAura(SPELL_CONTROL_PIECE) ? 0 : 1000;
                isAllowedToCast = spellPrimaryTimer != 0;
                ResetMoveChance();
                me->SetWalk(true);
                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 0.0f);

                // the pieces of the players do not move on their own, nor do they in a friendly game
                if (accessor)
                {
                    uint32 team = accessor->GetPlayerTeam();
                    uint32 state = instance->GetData(TYPE_CHESS);
                    if ((team == ALLIANCE && me->getFaction() == FACTION_ID_CHESS_ALLIANCE) ||
                        (team == HORDE && me->getFaction() == FACTION_ID_CHESS_HORDE) ||
                        state == DONE || state == SPECIAL)
                        moveCommandTimer = facingCommandTimer = 0;

                    if (state == SPECIAL)
                    {
                        spellPrimaryTimer = spellSecondaryTimer = 0;
                        isAllowedToCast = false;
                    }
                }
            }

            // no default attacking or evading
            void MoveInLineOfSight(Unit* /*who*/) OVERRIDE { }
            void AttackStart(Unit* /*who*/) OVERRIDE { }
            void EnterEvadeMode() OVERRIDE { }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (me->HasAura(SPELL_GAME_OVER))
                {
                    damage = 0;
                    return;
                }

                if (!pieceInfo || !pieceInfo->IsKing)
                    return;

                if (!damage || isAttacked || !instance || attacker->GetTypeId() != TypeID::TYPEID_UNIT || instance->GetData(TYPE_CHESS) == SPECIAL)
                    return;

                // announces the first hit on a king
                if (Creature* medivh = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_ECHO_MEDIVH)))
                    DoPlaySoundToSet(medivh, accessor->GetPlayerTeam() == pieceInfo->PieceTeam ? SOUND_ID_CHECK_PLAYER : SOUND_ID_CHECK_MEDIVH);

                isAttacked = true;
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (Creature* square = ObjectAccessor::GetCreature(*me, currentSquareGuid))
                    square->RemoveAllAuras();

                if (!instance || !pieceInfo)
                    return;

                Creature* medivh = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_ECHO_MEDIVH));
                if (!medivh)
                    return;

                if (pieceInfo->IsKing && instance->GetData(TYPE_CHESS) == SPECIAL)
                    instance->SetData(TYPE_CHESS, DONE);
                else if (accessor->GetPlayerTeam() == pieceInfo->PieceTeam)
                {
                    if (instance->GetData(TYPE_CHESS) != SPECIAL)
                        DoPlaySoundToSet(medivh, pieceInfo->LoseSoundPlayer[urand(0, 2)]);
                    if (pieceInfo->IsKing)
                        instance->SetData(TYPE_CHESS, FAIL);
                }
                else
                {
                    if (instance->GetData(TYPE_CHESS) != SPECIAL)
                        DoPlaySoundToSet(medivh, pieceInfo->LoseSoundMedivh[urand(0, 2)]);
                    if (pieceInfo->IsKing)
                    {
                        medivh->AI()->Talk(EMOTE_LIFT_CURSE);
                        instance->SetData(TYPE_CHESS, DONE);
                    }
                }

                // the dead piece leaves the board and its stalker next to it takes its form
                uint32 transformSpell = pieceInfo->TransformSpell;
                uint32 faction = pieceInfo->Faction;
                me->m_Events.Schedule(10000, [piece = me, transformSpell, faction]()
                {
                    if (piece->isDead())
                    {
                        if (karazhan_accessor* accessor = dynamic_cast<karazhan_accessor*>(piece->GetInstanceScript()))
                            accessor->DoMoveChessPieceToSides(transformSpell, faction);
                        piece->SetVisible(false);
                    }
                });

                if (pieceInfo->IsKing)
                    accessor->FinishChessEvent(pieceInfo->Faction);
            }

            void OnCharmed(bool apply) OVERRIDE
            {
                if (!apply)
                {
                    isUncharming = true;
                    Reset();
                    isUncharming = false;
                }
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                if (!pieceInfo)
                    return 0;

                return type ? pieceInfo->PieceTeam : pieceInfo->MeleeSpell;
            }

            uint64 GetGUID(int32 /*id*/ = 0) const OVERRIDE { return currentSquareGuid; }

            void SetGUID(uint64 guid, int32 type) OVERRIDE
            {
                // the square the piece moves to
                if (type)
                {
                    if (Creature* square = ObjectAccessor::GetCreature(*me, currentSquareGuid))
                        square->RemoveAllAuras();

                    if (pieceInfo && pieceInfo->MoveSound)
                        me->PlayDistanceSound(pieceInfo->MoveSound);

                    currentSquareGuid = guid;
                    moveTimer = 2000;
                }
                // the start square when the event begins
                else
                {
                    Reset();
                    currentSquareGuid = guid;
                    isAttacked = false;

                    DoCast(me, SPELL_CHESS_AI_ATTACK_TIMER, true);

                    me->m_Events.Schedule(500, [piece = me, squareGuid = guid]()
                    {
                        if (Creature* square = ObjectAccessor::GetCreature(*piece, squareGuid))
                        {
                            square->CastSpell(square, SPELL_MOVE_MARKER, true);
                            square->CastSpell(square, SPELL_IS_SQUARE_OCCUPIED, true);
                        }
                        piece->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK1H);
                    });
                }
            }

            void DoAction(int32 /*action*/) OVERRIDE
            {
                currentOrientation = me->GetOrientation();
            }

            void MovementInform(uint32 type, uint32 point) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE || !point)
                    return;

                // update the facing
                if (Unit* target = GetTargetByType(TARGET_TYPE_ADJACENT, 8.0f, 0))
                    DoCast(target, SPELL_CHANGE_FACING);
                else
                    me->SetFacingTo(currentOrientation);
            }

            void SpellHit(Unit* caster, SpellInfo const* spell) OVERRIDE
            {
                // the piece keeps its own faction when a player takes control
                if (caster->GetTypeId() == TypeID::TYPEID_PLAYER && spell->Id == SPELL_CONTROL_PIECE)
                {
                    me->RestoreFaction();
                    Reset();
                }
                else if (spell->Id == SPELL_MOVE_COOLDOWN && pieceInfo && pieceInfo->MoveEmote)
                    me->HandleEmoteCommand(pieceInfo->MoveEmote);
            }

            // random target of the given type in range and arc, adjacent targets stand on a neighbouring square
            Unit* GetTargetByType(uint8 type, float range, float arc = M_PI)
            {
                if (!accessor)
                    return nullptr;

                uint32 team = me->getFaction() == FACTION_ID_CHESS_ALLIANCE ? FACTION_ID_CHESS_HORDE : FACTION_ID_CHESS_ALLIANCE;
                if (type == TARGET_TYPE_FRIENDLY)
                    team = me->getFaction();

                std::vector<Creature*> targets;
                for (uint64 guid : accessor->GetChessPiecesByFaction(team))
                {
                    Creature* target = ObjectAccessor::GetCreature(*me, guid);
                    if (!target || !target->IsAlive())
                        continue;

                    if (!me->IsWithinCombatRange(target, range) || (arc > 0 && !me->HasInArc(arc, target)))
                        continue;

                    if (type == TARGET_TYPE_ADJACENT)
                        if (Creature* mySquare = ObjectAccessor::GetCreature(*me, currentSquareGuid))
                            if (Creature* theirSquare = ObjectAccessor::GetCreature(*target, target->AI()->GetGUID()))
                                if (theirSquare->GetEntry() != (mySquare->GetEntry() == NPC_SQUARE_WHITE ? NPC_SQUARE_BLACK : NPC_SQUARE_WHITE))
                                    continue;

                    // nothing to heal on a piece at full health
                    if (type == TARGET_TYPE_FRIENDLY && target->GetHealth() == target->GetMaxHealth())
                        continue;

                    targets.push_back(target);
                }

                if (targets.empty())
                    return nullptr;

                return Skyfire::Containers::SelectRandomContainerElement(targets);
            }

            // free square closest to the nearest enemy, the targeting of the move spells 32312 and 37388 cannot choose it
            Unit* GetMovementSquare()
            {
                if (!accessor || !pieceInfo)
                    return nullptr;

                std::list<Creature*> squares;
                GetCreatureListWithEntryInGrid(squares, me, NPC_SQUARE_BLACK, pieceInfo->MoveRadius);
                GetCreatureListWithEntryInGrid(squares, me, NPC_SQUARE_WHITE, pieceInfo->MoveRadius);

                squares.remove_if([this](Creature* square)
                {
                    return me->GetExactDist2d(square) > pieceInfo->MoveRadius || square->HasAura(SPELL_DISABLE_SQUARE) || square->HasAura(SPELL_IS_SQUARE_OCCUPIED);
                });

                if (squares.empty())
                    return nullptr;

                std::list<Creature*> enemies;
                uint32 enemyFaction = me->getFaction() == FACTION_ID_CHESS_ALLIANCE ? FACTION_ID_CHESS_HORDE : FACTION_ID_CHESS_ALLIANCE;
                for (uint64 guid : accessor->GetChessPiecesByFaction(enemyFaction))
                    if (Creature* target = ObjectAccessor::GetCreature(*me, guid))
                        if (target->IsAlive())
                            enemies.push_back(target);

                if (enemies.empty())
                    return nullptr;

                enemies.sort(Skyfire::ObjectDistanceOrderPred(me));
                squares.sort(Skyfire::ObjectDistanceOrderPred(enemies.front()));

                return squares.front();
            }

            void DoCastPrimarySpell()
            {
                if (!me->GetMotionMaster()->GetMotionSlot(MOTION_SLOT_ACTIVE))
                    me->SetFacingTo(me->GetOrientation());

                if (Unit* target = GetTargetByType(pieceInfo->PrimaryTarget, pieceInfo->PrimaryRadius, pieceInfo->PrimaryArc / 180.0f * M_PI))
                {
                    DoCast(target, pieceInfo->PrimarySpell);
                    SpellInfo const* spell = sSpellMgr->GetSpellInfo(pieceInfo->PrimarySpell);
                    me->AddCreatureSpellCooldown(pieceInfo->PrimarySpell);
                    me->_AddCreatureSpellCooldown(pieceInfo->PrimarySpell, time(nullptr) + spell->GetRecoveryTime() / IN_MILLISECONDS);
                    me->_AddCreatureSpellCooldown(pieceInfo->SecondarySpell, time(nullptr) + spell->CategoryRecoveryTime / IN_MILLISECONDS);
                    spellPrimaryTimer = spell->GetRecoveryTime();
                    if (spellSecondaryTimer < spell->CategoryRecoveryTime)
                        spellSecondaryTimer = spell->CategoryRecoveryTime;
                }
            }

            void DoCastSecondarySpell()
            {
                if (!me->GetMotionMaster()->GetMotionSlot(MOTION_SLOT_ACTIVE))
                    me->SetFacingTo(me->GetOrientation());

                if (Unit* target = GetTargetByType(pieceInfo->SecondaryTarget, pieceInfo->SecondaryRadius, pieceInfo->SecondaryArc / 180.0f * M_PI))
                {
                    DoCast(target, pieceInfo->SecondarySpell);
                    SpellInfo const* spell = sSpellMgr->GetSpellInfo(pieceInfo->SecondarySpell);
                    me->AddCreatureSpellCooldown(pieceInfo->SecondarySpell);
                    me->_AddCreatureSpellCooldown(pieceInfo->SecondarySpell, time(nullptr) + spell->GetRecoveryTime() / IN_MILLISECONDS);
                    me->_AddCreatureSpellCooldown(pieceInfo->PrimarySpell, time(nullptr) + spell->CategoryRecoveryTime / IN_MILLISECONDS);
                    spellSecondaryTimer = spell->GetRecoveryTime();
                    if (spellPrimaryTimer < spell->CategoryRecoveryTime)
                        spellPrimaryTimer = spell->CategoryRecoveryTime;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!instance || !pieceInfo)
                    return;

                uint32 state = instance->GetData(TYPE_CHESS);
                if (state != IN_PROGRESS && state != SPECIAL)
                    return;

                // issue the move command
                if (moveCommandTimer)
                {
                    if (moveCommandTimer <= diff)
                    {
                        // only face the enemy when one is next to the piece
                        if (!GetTargetByType(TARGET_TYPE_ADJACENT, 8.0f, 0))
                        {
                            // a piece does not move every time, the chance grows until it does
                            if (roll_chance_i(moveChance))
                            {
                                ResetMoveChance();

                                if (Unit* target = GetMovementSquare())
                                {
                                    DoCast(target, SPELL_MOVE_GENERIC, true);
                                    if (pieceInfo->MoveSound)
                                        me->PlayDistanceSound(pieceInfo->MoveSound);
                                }

                                currentOrientation = me->GetOrientation();
                                moveCommandTimer = facingCommandTimer = 12000;
                            }
                            else
                            {
                                moveCommandTimer = 5000;
                                moveChance += pieceInfo->MoveChanceIncrement;
                            }
                        }
                        else
                            ResetMoveChance();
                    }
                    else
                        moveCommandTimer -= diff;
                }

                if (facingCommandTimer)
                {
                    if (facingCommandTimer <= diff)
                    {
                        // an enemy directly in front only needs a short wait
                        if (GetTargetByType(TARGET_TYPE_ADJACENT, 8.0f, M_PI / 8))
                            facingCommandTimer = 1000;
                        else if (Unit* target = GetTargetByType(TARGET_TYPE_ADJACENT, 8.0f, 0))
                        {
                            DoCast(target, SPELL_CHANGE_FACING);
                            moveCommandTimer = facingCommandTimer = 1000;
                        }
                    }
                    else
                        facingCommandTimer -= diff;
                }

                // issue the spell command
                if (isAllowedToCast)
                {
                    spellPrimaryTimer -= std::min(spellPrimaryTimer, diff);
                    if (isAlternatingSpells)
                    {
                        if (!spellPrimaryTimer)
                        {
                            if (isPrimarySpell)
                                DoCastPrimarySpell();
                            else
                                DoCastSecondarySpell();

                            isPrimarySpell = !isPrimarySpell;
                        }
                    }
                    else
                    {
                        spellSecondaryTimer -= std::min(spellSecondaryTimer, diff);
                        if (!spellPrimaryTimer)
                            DoCastPrimarySpell();
                        if (!spellSecondaryTimer)
                            DoCastSecondarySpell();
                    }
                }

                // finish the move
                if (moveTimer)
                {
                    if (moveTimer <= diff)
                    {
                        if (Creature* square = ObjectAccessor::GetCreature(*me, currentSquareGuid))
                        {
                            DoCast(square, SPELL_MOVE_MARKER, true);
                            me->GetMotionMaster()->MovePoint(1, square->GetPositionX(), square->GetPositionY(), square->GetPositionZ());
                        }
                        moveTimer = 0;
                    }
                    else
                        moveTimer -= diff;
                }
            }

        private:
            InstanceScript* instance;
            karazhan_accessor* accessor;
            ChessPieceInfo const* pieceInfo;

            uint64 currentSquareGuid;

            uint32 moveTimer;
            uint32 moveCommandTimer;
            uint32 facingCommandTimer;
            uint32 spellPrimaryTimer;
            uint32 spellSecondaryTimer;
            uint32 moveChance; // Grows with every skipped move so the pieces do not stand still for too long

            bool isAttacked;
            bool isPrimarySpell;
            bool isAlternatingSpells;
            bool isUncharming;
            bool isAllowedToCast;
            float currentOrientation;

            void ResetMoveChance()
            {
                moveChance = 15;
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_chess_pieceAI(creature);
        }
};

void AddSC_chess_event()
{
    new npc_echo_of_medivh();
    new npc_chess_piece();
}
