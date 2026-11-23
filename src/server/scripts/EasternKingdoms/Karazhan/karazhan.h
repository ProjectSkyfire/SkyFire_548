/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef DEF_KARAZHAN_H
#define DEF_KARAZHAN_H

enum DataTypes
{
    TYPE_ATTUMEN                    = 1,
    TYPE_MOROES                     = 2,
    TYPE_MAIDEN                     = 3,
    TYPE_OPTIONAL_BOSS              = 4,
    TYPE_OPERA                      = 5,
    TYPE_CURATOR                    = 6,
    TYPE_ARAN                       = 7,
    TYPE_TERESTIAN                  = 8,
    TYPE_NETHERSPITE                = 9,
    TYPE_CHESS                      = 10,
    TYPE_MALCHEZZAR                 = 11,
    TYPE_NIGHTBANE                  = 12,

    DATA_OPERA_PERFORMANCE          = 13,
    DATA_OPERA_OZ_DEATHCOUNT        = 14,

    DATA_KILREK                     = 15,
    DATA_TERESTIAN                  = 16,
    DATA_MOROES                     = 17,
    DATA_GO_CURTAINS                = 18,
    DATA_GO_STAGEDOORLEFT           = 19,
    DATA_GO_STAGEDOORRIGHT          = 20,
    DATA_GO_LIBRARY_DOOR            = 21,
    DATA_GO_MASSIVE_DOOR            = 22,
    DATA_GO_NETHER_DOOR             = 23,
    DATA_GO_GAME_DOOR               = 24,
    DATA_GO_GAME_EXIT_DOOR          = 25,

    DATA_IMAGE_OF_MEDIVH            = 26,
    DATA_MASTERS_TERRACE_DOOR_1     = 27,
    DATA_MASTERS_TERRACE_DOOR_2     = 28,
    DATA_GO_SIDE_ENTRANCE_DOOR      = 29
};

enum OperaEvents
{
    EVENT_OZ                        = 1,
    EVENT_HOOD                      = 2,
    EVENT_RAJ                       = 3
};

enum ChessEvent
{
    NPC_ECHO_MEDIVH                 = 16816,
    NPC_INVISIBLE_STALKER           = 22519, // placeholder for dead chess npcs
    NPC_CHESS_STATUS_BAR            = 22520, // transforms the placeholders, also the encounter credit entry
    NPC_CHESS_VICTORY_CONTROLLER    = 22524,
    NPC_SQUARE_WHITE                = 17208,
    NPC_SQUARE_BLACK                = 17305,
    NPC_SQUARE_OUTSIDE_BLACK        = 17316,
    NPC_SQUARE_OUTSIDE_WHITE        = 17317,

    NPC_ORC_GRUNT                   = 17469, // pawn
    NPC_ORC_WOLF                    = 21748, // knight
    NPC_ORC_WARLOCK                 = 21750, // queen
    NPC_ORC_NECROLYTE               = 21747, // bishop
    NPC_SUMMONED_DAEMON             = 21726, // rook
    NPC_WARCHIEF_BLACKHAND          = 21752, // king
    NPC_HUMAN_FOOTMAN               = 17211, // pawn
    NPC_HUMAN_CHARGER               = 21664, // knight
    NPC_HUMAN_CONJURER              = 21683, // queen
    NPC_HUMAN_CLERIC                = 21682, // bishop
    NPC_CONJURED_WATER_ELEMENTAL    = 21160, // rook
    NPC_KING_LLANE                  = 21684, // king

    SPELL_CONTROL_PIECE             = 30019,
    SPELL_CLEAR_BOARD               = 37366,
    SPELL_FORCE_KILL_BUNNY          = 45260, // triggers 45259
    SPELL_GAME_OVER                 = 39401,
    SPELL_VICTORY_VISUAL_CHESS      = 39394,

    FACTION_ID_CHESS_HORDE          = 1689,
    FACTION_ID_CHESS_ALLIANCE       = 1690
};

struct karazhan_accessor
{
    virtual void DoPrepareChessEvent() = 0;
    virtual uint32 GetPlayerTeam() = 0;
    virtual bool IsFriendlyGameReady() const = 0;
    virtual void DoMoveChessPieceToSides(uint32 spellId, uint32 faction) = 0;
    virtual void FinishChessEvent(uint32 faction) = 0;
    virtual std::list<uint64> const& GetChessPiecesByFaction(uint32 faction) const = 0;
};

#define ERROR_INST_DATA(a)          SF_LOG_ERROR("scripts", "Instance Data for Karazhan not set properly. Encounter for Creature Entry %u may not work properly.", a->GetEntry());
#endif
