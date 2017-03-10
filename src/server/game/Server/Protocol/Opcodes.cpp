/*
 * Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2017 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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

#include "Opcodes.h"
#include "WorldSession.h"

OpcodeTable serverOpcodeTable;
OpcodeTable clientOpcodeTable;

template<bool isInValidRange, bool isNonZero>
void OpcodeTable::ValidateAndSetOpcode(uint16 /*opcode*/, uint16 /*opcodeNumber*/, char const* /*name*/, SessionStatus /*status*/, PacketProcessing /*processing*/, pOpcodeHandler /*handler*/)
{
    // if for some reason we are here, that means NUM_OPCODE_HANDLERS == 0 (or your compiler is broken)
}

template<>
void OpcodeTable::ValidateAndSetOpcode<true, true>(uint16 opcode, uint16 opcodeNumber, char const* name, SessionStatus status, PacketProcessing processing, pOpcodeHandler handler)
{
    if (_internalTable[opcode] != NULL)
    {
        SF_LOG_ERROR("network", "Tried to override handler of %s with %s (opcode %u)", _internalTable[opcode]->Name, name, opcodeNumber);
        return;
    }

    _internalTable[opcode] = new OpcodeHandler(opcodeNumber, name, status, processing, handler);
    _opcodeTable[opcodeNumber] = (Opcodes)opcode;
}

template<>
void OpcodeTable::ValidateAndSetOpcode<false, true>(uint16 /*opcode*/, uint16 opcodeNumber, char const* /*name*/, SessionStatus /*status*/, PacketProcessing /*processing*/, pOpcodeHandler /*handler*/)
{
    SF_LOG_ERROR("network", "Tried to set handler for an invalid opcode %d", opcodeNumber);
}

template<>
void OpcodeTable::ValidateAndSetOpcode<true, false>(uint16 /*opcode*/, uint16 /*opcodeNumber*/, char const* name, SessionStatus /*status*/, PacketProcessing /*processing*/, pOpcodeHandler /*handler*/)
{
    SF_LOG_ERROR("network", "Opcode %s got value 0", name);
}

/// Correspondence between opcodes and their names
void OpcodeTable::InitializeClientTable()
{
#define DEFINE_OPCODE_HANDLER(opcode, opcodeNumber, status, processing, handler)                                      \
    ValidateAndSetOpcode<(opcodeNumber < NUM_OPCODE_HANDLERS), (opcode != 0)>(opcode, opcodeNumber, #opcode, status, processing, handler);

    DEFINE_OPCODE_HANDLER(CMSG_ACCEPT_LEVEL_GRANT,                 0x02FB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAcceptGrantLevel             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ACCEPT_TRADE,                       0x144D, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAcceptTradeOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ACTIVATE_TAXI,                      0x03C9, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleActivateTaxiOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ACTIVATE_TAXI_EXPRESS,              0x06FB, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleActivateTaxiExpressOpcode    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ADDON_REGISTERED_PREFIXES,          0x040E, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAddonRegisteredPrefixesOpcode); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ADD_FRIEND,                         0x09A6, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAddFriendOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ADD_IGNORE,                         0x0D20, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAddIgnoreOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ALTER_APPEARANCE,                   0x07F0, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAlterAppearance              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AREATRIGGER,                        0x1C44, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAreaTriggerOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AREA_SPIRIT_HEALER_QUEUE,           0x12D8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAreaSpiritHealerQueueOpcode  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ATTACKSTOP,                         0x0345, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleAttackStopOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ATTACKSWING,                        0x02E7, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleAttackSwingOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUCTION_HELLO_REQUEST,              0x0379, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionHelloOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUCTION_LIST_BIDDER_ITEMS,          0x12D0, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionListBidderItems       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUCTION_LIST_ITEMS,                 0x02EA, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionListItems             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUCTION_LIST_OWNER_ITEMS,           0x0361, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionListOwnerItems        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUCTION_PLACE_BID,                  0x03C8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionPlaceBid              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUCTION_REMOVE_ITEM,                0x0259, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionRemoveItem            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUCTION_SELL_ITEM,                  0x02EB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionSellItem              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUTH_SESSION,                       0x00B2, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::Handle_EarlyProccess               ); // 5.4.8 18414
	DEFINE_OPCODE_HANDLER(CMSG_AUTH_CONTINUED_SESSION,             0x0F49, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::Handle_EarlyProccessContinued      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUTOBANK_ITEM,                      0x066D, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAutoBankItemOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUTOEQUIP_ITEM,                     0x025F, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAutoEquipItemOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUTOSTORE_BAG_ITEM,                 0x067C, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAutoStoreBagItemOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUTOSTORE_BANK_ITEM,                0x02CF, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAutoStoreBankItemOpcode      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUTOSTORE_LOOT_ITEM,                0x0354, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAutostoreLootItemOpcode      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_AUTO_DECLINE_GUILD_INVITES,         0x06CB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAutoDeclineGuildInvites      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BANKER_ACTIVATE,                    0x02E9, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBankerActivateOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEFIELD_LEAVE,                  0x0257, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleBattlefieldLeaveOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEFIELD_LIST,                   0x1C41, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlefieldListOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEFIELD_MGR_ENTRY_INVITE_RESPONSE, 0x1806, STATUS_LOGGEDIN,  PROCESS_INPLACE,   &WorldSession::HandleBfEntryInviteResponse        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEFIELD_MGR_EXIT_REQUEST,       0x08B3, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleBfExitRequest                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEFIELD_MGR_QUEUE_INVITE_RESPONSE, 0x0A97, STATUS_LOGGEDIN,  PROCESS_INPLACE,   &WorldSession::HandleBfQueueInviteResponse        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEFIELD_PORT,                   0x1379, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattleFieldPortOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEMASTER_JOIN,                  0x0769, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlemasterJoinOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEMASTER_JOIN_ARENA,            0x02D2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlemasterJoinArena        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLE_PET_DELETE_PET,              0x18B6, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlePetDelete              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLE_PET_MODIFY_NAME,             0x1887, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlePetModifyName          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLE_PET_QUERY_NAME,              0x1CE0, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlePetQueryName           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLE_PET_SET_BATTLE_SLOT,         0x0163, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlePetSetBattleSlot       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLE_PET_SET_FLAGS,               0x17AC, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlePetSetFlags            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BATTLE_PET_SUMMON_COMPANION,        0x1896, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlePetSummonCompanion     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BEGIN_TRADE,                        0x1CE3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBeginTradeOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BINDER_ACTIVATE,                    0x1248, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBinderActivateOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BLACKMARKET_BID,                    0x12C8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBlackMarketBidOnItem         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BLACKMARKET_HELLO,                  0x075A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBlackMarketHelloOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BLACKMARKET_REQUEST_ITEMS,          0x127A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBlackMarketRequestItemOpcode ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BUYBACK_ITEM,                       0x0661, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBuybackItem                  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BUY_BANK_SLOT,                      0x12F2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBuyBankSlotOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_BUY_ITEM,                           0x02E2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBuyItemOpcode                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_ADD_EVENT,                 0x0A37, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarAddEvent             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_COMPLAIN,                  0x1F8F, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarComplain             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_COPY_EVENT,                0x1A97, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarCopyEvent            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_EVENT_INVITE,              0x1D8E, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventInvite          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_EVENT_MODERATOR_STATUS,    0x0708, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventModeratorStatus ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_EVENT_REMOVE_INVITE,       0x0962, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventRemoveInvite    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_EVENT_RSVP,                0x1FB8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventRsvp            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_EVENT_SIGNUP,              0x01E3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventSignup          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_EVENT_STATUS,              0x1AB3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventStatus          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_GET_CALENDAR,              0x1F9F, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarGetCalendar          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_GET_EVENT,                 0x030C, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarGetEvent             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_REMOVE_EVENT,              0x0C61, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarRemoveEvent          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_UPDATE_EVENT,              0x1F8D, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarUpdateEvent          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CANCEL_AURA,                        0x1861, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelAuraOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CANCEL_AUTO_REPEAT_SPELL,           0x1272, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelAutoRepeatSpellOpcode  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CANCEL_CAST,                        0x18C0, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleCancelCastOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CANCEL_CHANNELLING,                 0x08C0, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelChanneling             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CANCEL_MOUNT_AURA,                  0x10E3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelMountAuraOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CANCEL_TEMP_ENCHANTMENT,            0x024B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelTempEnchantmentOpcode  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CANCEL_TRADE,                       0x1941, STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT, PROCESS_THREADUNSAFE, &WorldSession::HandleCancelTradeOpcode); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CAST_SPELL,                         0x0206, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleCastSpellOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE, 0x08F8, STATUS_LOGGEDIN, PROCESS_THREADUNSAFE, &WorldSession::HandleChangeSeatsOnControlledVehicle); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_ANNOUNCEMENTS,              0x06AF, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelAnnouncements         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_BAN,                        0x08BF, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelBan                   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_INVITE,                     0x10AB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelInvite                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_KICK,                       0x0E0B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelKick                  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_LIST,                       0x0C1B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelList                  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_MODERATOR,                  0x00AE, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelModerator             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_MUTE,                       0x000A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelMute                  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_OWNER,                      0x00AF, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelOwner                 ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_PASSWORD,                   0x0A1E, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelPassword              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_UNBAN,                      0x081F, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelUnban                 ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_UNMODERATOR,                0x041E, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelUnmoderator           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_UNMUTE,                     0x022A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelUnmute                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHAR_CREATE,                        0x0F1D, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleCharCreateOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHAR_CUSTOMIZE,                     0x0A13, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleCharCustomize                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHAR_DELETE,                        0x04E2, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleCharDeleteOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ENUM_CHARACTERS,                    0x00E0, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleCharEnumOpcode               ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHAR_FACTION_OR_RACE_CHANGE,        0x0329, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleCharFactionOrRaceChange      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHAR_RENAME,                        0x0963, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleCharRenameOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHAT_IGNORED,                       0x048A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChatIgnoredOpcode            ); // 5.4.8 18414
	DEFINE_OPCODE_HANDLER(CMSG_CHAT_JOIN_CHANNEL,                  0x148E, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleJoinChannel                  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CLEAR_TRADE_ITEM,                   0x00A7, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleClearTradeItemOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CONFIRM_RESPEC_WIPE,                0x0275, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRespecWipeConfirmOpcode      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CONTACT_LIST,                       0x0BB4, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleContactListOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CORPSE_MAP_POSITION_QUERY,          0x0000, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCorpseMapPositionQuery       );
    DEFINE_OPCODE_HANDLER(CMSG_CREATURE_QUERY,                     0x0842, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleCreatureQueryOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_DEL_FRIEND,                         0x1103, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleDelFriendOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_DEL_IGNORE,                         0x0737, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleDelIgnoreOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_DESTROY_ITEM,                       0x0026, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleDestroyItemOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_DISMISS_CRITTER,                    0x12DB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleDismissCritter               ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_DUEL_PROPOSED,                      0x1A26, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleDuelProposedOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_DUEL_RESPONSE,                      0x03E2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleDuelResponseOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_EMOTE,                              0x1924, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleEmoteOpcode                  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_ENABLE_TAXI,                        0x0741, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleTaxiQueryAvailableNodes      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_EQUIPMENT_SET_DELETE,               0x02E8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleEquipmentSetDelete           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_EQUIPMENT_SET_SAVE,                 0x0669, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleEquipmentSetSave             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_EQUIPMENT_SET_USE,                  0x036E, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleEquipmentSetUse              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_FORCE_MOVE_ROOT_ACK,                0x107A, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMoveRootAck                  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_FORCE_MOVE_UNROOT_ACK,              0x1051, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMoveUnRootAck                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GAMEOBJECT_QUERY,                   0x1461, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGameObjectQueryOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GAME_OBJ_USE,                       0x06D9, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGameobjectReportUse          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GAME_OBJ_REPORT_USE,                0x06D8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGameObjectUseOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GET_MAIL_LIST,                      0x077A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGetMailList                  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GET_MIRROR_IMAGE_DATA,              0x02A3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMirrorImageDataRequest       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GM_TICKET_CREATE,                   0x1A86, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketCreateOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GM_TICKET_DELETE_TICKET,            0x1A23, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketDeleteOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GM_TICKET_GET_TICKET,               0x1F89, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketGetTicketOpcode      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GM_TICKET_SYSTEM_STATUS,            0x0A82, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketSystemStatusOpcode   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GM_TICKET_CASE_STATUS,              0x15A8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketCaseStatusOpcode     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GM_TICKET_UPDATE_TEXT,              0x0A26, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketUpdateOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GOSSIP_HELLO,                       0x12F3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGossipHelloOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GOSSIP_SELECT_OPTION,               0x0748, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGossipSelectOptionOpcode     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GRANT_LEVEL,                        0x0662, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGrantLevel                   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_ASSISTANT_LEADER,             0x1897, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupAssistantLeaderOpcode   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_DISBAND,                      0x1798, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupDisbandOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_INITIATE_ROLE_POLL,           0x1882, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupInitiatePollRole        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_INVITE,                       0x072D, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupInviteOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_INVITE_RESPONSE,              0x0D61, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupInviteResponseOpcode    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_RAID_CONVERT,                 0x032C, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupRaidConvertOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_SET_LEADER,                   0x15BB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupSetLeaderOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_SET_ROLES,                    0x1A92, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGroupSetRolesOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_UNINVITE_GUID,                0x0CE1, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupUninviteGuidOpcode      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_ACCEPT,                       0x18A2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildAcceptOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_ACHIEVEMENT_PROGRESS_QUERY,   0x1552, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildAchievementProgressQuery); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_ADD_RANK,                     0x047A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildAddRankOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_ASSIGN_MEMBER_RANK,           0x05D0, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildAssignRankOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANKER_ACTIVATE,              0x0372, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankerActivate          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANK_BUY_TAB,                 0x0251, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankBuyTab              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANK_DEPOSIT_MONEY,           0x0770, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankDepositMoney        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANK_LOG_QUERY,               0x0CD3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankLogQuery            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANK_MONEY_WITHDRAWN_QUERY,   0x14DB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankMoneyWithdrawn      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANK_NOTE,                    0x04D9, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankTabNote             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANK_QUERY_TAB,               0x1372, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankQueryTab            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANK_QUERY_TEXT,              0x0550, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQueryGuildBankTabText        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANK_SWAP_ITEMS,              0x136A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankSwapItems           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANK_UPDATE_TAB,              0x07C2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankUpdateTab           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_BANK_WITHDRAW_MONEY,          0x07EA, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankWithdrawMoney       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_DECLINE,                      0x147B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildDeclineOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_DEL_RANK,                     0x0D79, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildDelRankOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_DEMOTE,                       0x1553, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildDemoteOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_DISBAND,                      0x0D73, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildDisbandOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_EVENT_LOG_QUERY,              0x15D9, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildEventLogQueryOpcode     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_INFO_TEXT,                    0x0C70, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildChangeInfoTextOpcode    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_INVITE,                       0x0869, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildInviteOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_LEAVE,                        0x04D8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildLeaveOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_MOTD,                         0x1473, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildMOTDOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_NEWS_UPDATE_STICKY,           0x04D1, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGuildNewsUpdateStickyOpcode  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_PERMISSIONS,                  0x145A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildPermissions             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_PROMOTE,                      0x0571, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildPromoteOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_QUERY,                        0x1AB6, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleGuildQueryOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_QUERY_NEWS,                   0x1C58, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGuildQueryNewsOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_QUERY_RANKS,                  0x0D50, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildQueryRanksOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_REMOVE,                       0x0CD8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildRemoveOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_REPLACE_GUILD_MASTER,         0x0CD0, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildReplaceGuildMaster      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_REQUEST_CHALLENGE_UPDATE,     0x147A, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleGuildRequestChallengeUpdate  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_ROSTER,                       0x1459, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildRosterOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_SET_GUILD_MASTER,             0x1A83, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildSetGuildMaster          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_SET_NOTE,                     0x05DA, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildSetNoteOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_SET_RANK_PERMISSIONS,         0x0C7A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildSetRankPermissionsOpcode); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_IGNORE_TRADE,                       0x0276, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleIgnoreTradeOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_INITIATE_TRADE,                     0x0267, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleInitiateTradeOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_INSPECT,                            0x1259, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleInspectOpcode                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_INSPECT_HONOR_STATS,                0x0882, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleInspectHonorStatsOpcode      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_GET_ITEM_PURCHASE_DATA,             0x1258, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleItemRefundInfoRequest        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LEARN_TALENT,                       0x02A7, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLearnTalentOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LEAVE_CHANNEL,                      0x042A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLeaveChannel                 ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LFG_GET_STATUS,                     0x0000, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleLfgGetStatus                 );
    DEFINE_OPCODE_HANDLER(CMSG_LFG_JOIN,                           0x046B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgJoinOpcode                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LFG_TELEPORT,                       0x1AA6, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgTeleportOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LF_GUILD_ADD_APPLICATION,           0x0C63, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderAddApplication    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LF_GUILD_BROWSE,                    0x159A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderBrowse            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LF_GUILD_GET_APPLICATIONS,          0x0558, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderGetApplications   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LF_GUILD_GET_RECRUITS,              0x057A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderGetRecruits       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LF_GUILD_REMOVE_APPLICATION,        0x1C53, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderRemoveApplication ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LF_GUILD_SET_GUILD_POST,            0x1D9F, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderSetGuildPost      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LIST_INVENTORY,                     0x02D8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleListInventoryOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LOADING_SCREEN_NOTIFY,              0x1DBD, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleLoadScreenOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LOGOUT_CANCEL,                      0x06C1, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLogoutCancelOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LOGOUT_REQUEST,                     0x1349, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLogoutRequestOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LOOT,                               0x1CE2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootOpcode                   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LOOT_METHOD,                        0x0DE1, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootMethodOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LOOT_MONEY,                         0x02F6, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootMoneyOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LOOT_RELEASE,                       0x0840, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootReleaseOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_LOOT_ROLL,                          0x15C2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootRoll                     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MAIL_CREATE_TEXT_ITEM,              0x1270, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMailCreateTextItem           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MAIL_DELETE,                        0x14E2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMailDelete                   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MAIL_MARK_AS_READ,                  0x0241, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMailMarkAsRead               ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MAIL_QUERY_NEXT_TIME,               0x077B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQueryNextMailTime            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MAIL_RETURN_TO_SENDER,              0x1FA8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMailReturnToSender           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MAIL_TAKE_ITEM,                     0x1371, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMailTakeItem                 ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MAIL_TAKE_MONEY,                    0x06FA, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMailTakeMoney                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_ADDON_WHISPER,          0x0EBB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAddonMessagechatOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CHAT_MESSAGE_AFK,                   0x0EAB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_CHANNEL,                0x00BB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_DND,                    0x002E, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_EMOTE,                  0x103E, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_GUILD,                  0x0CAE, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_OFFICER,                0x0ABF, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_PARTY,                  0x109A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_RAID,                   0x083E, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_RAID_WARNING,           0x16AB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_SAY,                    0x0A9A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_WHISPER,                0x123E, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_YELL,                   0x04AA, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MINIMAP_PING,                       0x0837, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMinimapPingOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MOUNTSPECIAL_ANIM,                  0x0082, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMountSpecialAnimOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_CHNG_TRANSPORT,                0x09DB, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_SPLINE_DONE,                   0x11D9, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMoveSplineDoneOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_TELEPORT_ACK,                  0x0078, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMoveTeleportAck              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_TIME_SKIPPED,                  0x0150, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleMoveTimeSkippedOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_NAME_QUERY,                         0x0328, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleNameQueryOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REALM_NAME_QUERY,                   0x1A16, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleRealmNameQueryOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_NPC_TEXT_QUERY,                     0x0287, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleNpcTextQueryOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_OBJECT_UPDATE_FAILED,               0x1061, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleObjectUpdateFailedOpcode     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_OFFER_PETITION,                     0x15BE, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleOfferPetitionOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_OPENING_CINEMATIC,                  0x0130, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleOpeningCinematic             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_OPEN_ITEM,                          0x1D10, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleOpenItemOpcode               ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PAGE_TEXT_QUERY,                    0x1022, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePageTextQueryOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PETITION_BUY,                       0x12D9, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionBuyOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PETITION_DECLINE,                   0x1279, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionDeclineOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PETITION_RENAME,                    0x1F9A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionRenameOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PETITION_QUERY,                     0x0255, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionQueryOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PETITION_SHOWLIST,                  0x037B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionShowListOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PETITION_SHOW_SIGNATURES,           0x136B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionShowSignOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PETITION_SIGN,                      0x06DA, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionSignOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PET_ABANDON,                        0x07D0, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetAbandon                   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PET_ACTION,                         0x025B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetAction                    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PET_NAME_QUERY,                     0x1C62, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetNameQuery                 ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PET_RENAME,                         0x0A32, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetRename                    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PET_SPELL_AUTOCAST,                 0x06F0, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetSpellAutocastOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PET_STOP_ATTACK,                    0x065B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetStopAttack                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_PLAYED_TIME,                0x03F6, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePlayedTime                   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PLAYER_LOGIN,                       0x158F, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandlePlayerLoginOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PLAYER_VEHICLE_ENTER,               0x0277, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleEnterPlayerVehicle           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PUSHQUESTTOPARTY,                   0x03D2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePushQuestToParty             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_PVP_LOG_DATA,                       0x14C2, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandlePVPLogDataOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUERY_GUILD_REWARDS,                0x06C4, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGuildRewardsQueryOpcode      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUERY_GUILD_XP,                     0x05F8, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildQueryXPOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUERY_INSPECT_ACHIEVEMENTS,         0x0373, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQueryInspectAchievements     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUERY_TIME,                         0x0640, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQueryTimeOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_GIVER_ACCEPT_QUEST,           0x06D1, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverAcceptQuestOpcode  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_GIVER_CHOOSE_REWARD,          0x07CB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverChooseRewardOpcode ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_GIVER_COMPLETE_QUEST,         0x0659, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverCompleteQuest      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_GIVER_HELLO,                  0x02DB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverHelloOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_GIVER_QUERY_QUEST,            0x12F0, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverQueryQuestOpcode   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_GIVER_REQUEST_REWARD,         0x0378, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverRequestRewardOpcode); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_GIVER_STATUS_MULTIPLE_QUERY,  0x02F1, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverStatusMultipleQuery); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_GIVER_STATUS_QUERY,           0x036A, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleQuestgiverStatusQueryOpcode  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUESTLOG_REMOVE_QUEST,              0x0779, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestLogRemoveQuest          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_CONFIRM_ACCEPT,               0x124B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestConfirmAccept           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_POI_QUERY,                    0x10C2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestPOIQuery                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_QUERY,                        0x02D5, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestQueryOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_RAID_READY_CHECK,                   0x0817, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRaidReadyCheckOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_RAID_READY_CHECK_CONFIRM,           0x158B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRaidReadyCheckConfirmOpcode  ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_RAID_TARGET_UPDATE,                 0x0886, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRaidTargetUpdateOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_RANDOM_ROLL,                        0x08A3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRandomRollOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_RANDOMIZE_CHAR_NAME,                0x0B1C, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleRandomizeCharNameOpcode      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_READY_FOR_ACCOUNT_DATA_TIMES,       0x031C, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleReadyForAccountDataTimes     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_READ_ITEM,                          0x0D00, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleReadItem                     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_RECLAIM_CORPSE,                     0x03D3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleReclaimCorpseOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REFORGE_ITEM,                       0x0C4F, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleReforgeItemOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REORDER_CHARACTERS,                 0x08A7, STATUS_AUTHED,    PROCESS_INPLACE,      &WorldSession::HandleReorderCharacters            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REPAIR_ITEM,                        0x02C1, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRepairItemOpcode             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REPOP_REQUEST,                      0x134A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRepopRequestOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REPORT_PVP_AFK,                     0x06F9, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleReportPvPAFK                 ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_ACCOUNT_DATA,               0x1D8A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRequestAccountData           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_CEMETERY_LIST,              0x06E4, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleRequestCemeteryList          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_HOTFIX,                     0x158D, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleRequestHotfix                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_PARTY_MEMBER_STATS,         0x0806, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRequestPartyMemberStatsOpcode); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_PVP_OPTIONS_ENABLED,        0x0A22, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleRequestPvpOptions            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_PVP_REWARDS,                0x0375, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleRequestPvpReward             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_RESET_INSTANCES,                    0x0C69, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleResetInstancesOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_RESURRECT_RESPONSE,                 0x0B0C, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleResurrectResponseOpcode      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_RETURN_TO_GRAVEYARD,                0x12EA, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleReturnToGraveyard            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SELL_ITEM,                          0x1358, STATUS_LOGGEDIN, PROCESS_THREADUNSAFE,  &WorldSession::HandleSellItemOpcode               ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SELECT_FACTION,                     0x0027, STATUS_LOGGEDIN, PROCESS_THREADUNSAFE,  &WorldSession::HandleSelectFactionOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SEND_MAIL,                          0x1DBA, STATUS_LOGGEDIN, PROCESS_THREADUNSAFE,  &WorldSession::HandleSendMail                     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SETSHEATHED,                        0x0249, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleSetSheathedOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_ACTIONBAR_TOGGLES,              0x0672, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleSetActionBarToggles          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_ACTION_BUTTON,                  0x1F8C, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetActionButtonOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_ACTIVE_MOVER,                   0x09F0, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetActiveMoverOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_CONTACT_NOTES,                  0x0937, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetContactNotesOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_DUNGEON_DIFFICULTY,             0x1A36, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetDungeonDifficultyOpcode   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_EVERYONE_IS_ASSISTANT,          0x01E1, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGroupEveryoneIsAssistantOpcode); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_FACTION_ATWAR,                  0x027B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetFactionAtWar              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_FACTION_INACTIVE,               0x0778, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetFactionInactiveOpcode     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_FACTION_NOTATWAR,               0x064B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetFactionNotAtWar           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_LFG_BONUS_FACTION_ID,           0x0247, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetLfgBonusFactionID         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_PLAYER_DECLINED_NAMES,          0x09E2, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleSetPlayerDeclinedNames       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_PRIMARY_TALENT_TREE,            0x06C6, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandeSetTalentSpecialization       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_PVP,                            0x02C5, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleSetPvP                       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_SELECTION,                      0x0740, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleSetSelectionOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_TAXI_BENCHMARK_MODE,            0x0762, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetTaxiBenchmarkOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_TITLE,                          0x03C7, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetTitleOpcode               ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_TRADE_GOLD,                     0x14E3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetTradeGoldOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_TRADE_ITEM,                     0x03D5, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetTradeItemOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SET_WATCHED_FACTION,                0x06C9, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetWatchedFactionOpcode      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SHOWING_CLOAK,                      0x02F2, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleShowingCloakOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SHOWING_HELM,                       0x126B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleShowingHelmOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SOCKET_GEMS,                        0x02CB, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSocketOpcode                 ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SPELLCLICK,                         0x067A, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSpellClick                   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SPIRIT_HEALER_ACTIVATE,             0x0340, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSpiritHealerActivateOpcode   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SPLIT_ITEM,                         0x02EC, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSplitItemOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_STAND_STATE_CHANGE,                 0x03E6, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleStandStateChangeOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SUBMIT_BUG,                         0x0861, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleSubmitBugOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SUGGESTION_SUBMIT,                  0x0A12, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleSubmitSuggestOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SUMMON_RESPONSE,                    0x0A33, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSummonResponseOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SWAP_INV_ITEM,                      0x03DF, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSwapInvItemOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SWAP_ITEM,                          0x035D, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSwapItem                     ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TAXI_NODE_STATUS_QUERY,             0x02E1, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleTaxiNodeStatusQueryOpcode    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TAXI_QUERY_AVAILABLE_NODES,         0x02E3, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleTaxiQueryAvailableNodes      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_SEND_TEXT_EMOTE,                    0x07E9, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTextEmoteOpcode              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TIME_SYNC_RESPONSE,                 0x01DB, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleTimeSyncResp                 ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TOGGLE_PVP,                         0x0644, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTogglePvP                    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TOTEM_DESTROYED,                    0x1263, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTotemDestroyed               ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TRAINER_BUY_SPELL,                  0x0352, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTrainerBuySpellOpcode        ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TRAINER_LIST,                       0x034B, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTrainerListOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TRANSMOGRIFY_ITEMS,                 0x06D7, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTransmogrifyItems            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TURN_IN_PETITION,                   0x0673, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTurnInPetitionOpcode         ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TUTORIAL_CLEAR,                     0x0F23, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTutorialClear                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_TUTORIAL_FLAG,                      0x1D36, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTutorialFlag                 ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_UNACCEPT_TRADE,                     0x0023, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleUnacceptTradeOpcode          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_UNLEARN_SKILL,                      0x0268, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleUnlearnSkillOpcode           ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_UNREGISTER_ALL_ADDON_PREFIXES,      0x029F, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleUnregisterAddonPrefixesOpcode); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_UPDATE_ACCOUNT_DATA,                0x0068, STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleUpdateAccountData            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_USE_ITEM,                           0x1CC1, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleUseItemOpcode                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_VIOLENCE_LEVEL,                     0x0040, STATUS_AUTHED,    PROCESS_INPLACE,      &WorldSession::HandleViolenceLevel                ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_VOID_STORAGE_QUERY,                 0x0140, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleVoidStorageQuery             ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_VOID_STORAGE_TRANSFER,              0x1440, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleVoidStorageTransfer          ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_VOID_STORAGE_UNLOCK,                0x0444, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleVoidStorageUnlock            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_WHO,                                0x18A3, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleWhoOpcode                    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_WORLD_STATE_UI_TIMER_UPDATE,        0x15AB, STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleWorldStateUITimerUpdate      ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(CMSG_CORPSE_QUERY,                       0x1FBE, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCorpseQueryOpcode            ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_FALL_LAND,                      0x08FA, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_HEARTBEAT,                      0x01F2, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_JUMP,                           0x1153, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_SET_FACING,                     0x1050, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_SET_PITCH,                      0x017A, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_ASCEND,                   0x11FA, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_BACKWARD,                 0x09D8, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_DESCEND,                  0x01D1, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_FORWARD,                  0x095A, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_PITCH_DOWN,               0x08D8, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_PITCH_UP,                 0x00D8, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_STRAFE_LEFT,              0x01F8, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_STRAFE_RIGHT,             0x1058, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_SWIM,                     0x1858, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_TURN_LEFT,                0x01D0, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_START_TURN_RIGHT,               0x107B, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_STOP,                           0x08F1, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_STOP_ASCEND,                    0x115A, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_STOP_PITCH,                     0x007A, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_STOP_STRAFE,                    0x0171, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_STOP_SWIM,                      0x0950, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_STOP_TURN,                      0x1170, STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMovementOpcodes              ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_MOVE_WORLDPORT_ACK,                  0x1FAD, STATUS_TRANSFER,  PROCESS_THREADUNSAFE, &WorldSession::HandleMoveWorldportAckOpcode       ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_SET_RAID_DIFFICULTY,                 0x0591, STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetRaidDifficultyOpcode      ); // 5.4.8 18414


    //UNHANDLED
    DEFINE_OPCODE_HANDLER(CMSG_ADD_VOICE_IGNORE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_AREA_SPIRIT_HEALER_QUERY, 0x03F1, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleAreaSpiritHealerQueryOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_ARENA_TEAM_ACCEPT, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL); // Removed in 5.2
    DEFINE_OPCODE_HANDLER(CMSG_ARENA_TEAM_CREATE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL); // Removed in 5.2
    DEFINE_OPCODE_HANDLER(CMSG_ARENA_TEAM_DECLINE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL); // Removed in 5.2
    DEFINE_OPCODE_HANDLER(CMSG_ARENA_TEAM_DISBAND, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL); // Removed in 5.2
    DEFINE_OPCODE_HANDLER(CMSG_ARENA_TEAM_INVITE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL); // Removed in 5.2
    DEFINE_OPCODE_HANDLER(CMSG_ARENA_TEAM_LEADER, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL); // Removed in 5.2
    DEFINE_OPCODE_HANDLER(CMSG_ARENA_TEAM_LEAVE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL); // Removed in 5.2
    DEFINE_OPCODE_HANDLER(CMSG_ARENA_TEAM_QUERY, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL); // Removed in 5.2
    DEFINE_OPCODE_HANDLER(CMSG_ARENA_TEAM_REMOVE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL); // Removed in 5.2
    DEFINE_OPCODE_HANDLER(CMSG_ARENA_TEAM_ROSTER, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL); // Removed in 5.2
    DEFINE_OPCODE_HANDLER(CMSG_AUCTION_LIST_PENDING_SALES, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionListPendingSales);
    DEFINE_OPCODE_HANDLER(CMSG_AUTOEQUIP_ITEM_SLOT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleAutoEquipItemSlotOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEFIELD_MGR_QUEUE_REQUEST, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEFIELD_STATUS, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleBattlefieldStatusOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEGROUND_PLAYER_POSITIONS, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::HandleBattlegroundPlayerPositionsOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_BATTLEMASTER_JOIN_RATED, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_BUG, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleBugOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_BUSY_TRADE, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleBusyTradeOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_ARENA_TEAM, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarArenaTeam);
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_GET_NUM_PENDING, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarGetNumPending);
    DEFINE_OPCODE_HANDLER(CMSG_CALENDAR_GUILD_FILTER, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarGuildFilter);
    DEFINE_OPCODE_HANDLER(CMSG_CANCEL_QUEUED_SPELL, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CHANGEPLAYER_DIFFICULTY, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_DISPLAY_LIST, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleChannelDisplayListQuery);
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_MODERATE, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_ROSTER_INFO, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_SET_OWNER, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleChannelSetOwner);
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_SILENCE_ALL, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_SILENCE_VOICE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_UNSILENCE_ALL, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_UNSILENCE_VOICE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_VOICE_OFF, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleChannelVoiceOnOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_CHANNEL_VOICE_ON, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleChannelVoiceOnOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_CHAT_FILTERED, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CLEAR_CHANNEL_WATCH, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CLEAR_RAID_MARKER, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_COMMENTATOR_ENABLE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_COMMENTATOR_ENTER_INSTANCE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_COMMENTATOR_EXIT_INSTANCE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_COMMENTATOR_GET_MAP_INFO, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_COMMENTATOR_GET_PARTY_INFO, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_COMMENTATOR_GET_PLAYER_INFO, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_COMMENTATOR_INSTANCE_COMMAND, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_COMMENTATOR_SKIRMISH_QUEUE_COMMAND, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_COMMENTATOR_START_WARGAME, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_COMPLAIN, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleComplainOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_COMPLETE_CINEMATIC, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleCompleteCinematic);
    DEFINE_OPCODE_HANDLER(CMSG_COMPLETE_MOVIE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_CONNECT_TO_FAILED, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_DANCE_QUERY, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_DB_QUERY_BULK, 0x158D, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_DEL_VOICE_IGNORE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_DISMISS_CONTROLLED_VEHICLE, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleDismissControlledVehicle);
    DEFINE_OPCODE_HANDLER(CMSG_EJECT_PASSENGER, 0x06E7, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_ENABLE_NAGLE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_EarlyProccess);
    DEFINE_OPCODE_HANDLER(CMSG_FAR_SIGHT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleFarSightOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_GMRESPONSE_RESOLVE, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleGMResponseResolve);
    DEFINE_OPCODE_HANDLER(CMSG_GMSURVEY_SUBMIT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleGMSurveySubmit);
    DEFINE_OPCODE_HANDLER(CMSG_GM_REPORT_LAG, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleReportLag);
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_CHANGE_SUB_GROUP, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleGroupChangeSubGroupOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_REQUEST_JOIN_UPDATES, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_GROUP_SWAP_SUB_GROUP, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleGroupSwapSubGroupOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_ACHIEVEMENT_MEMBERS, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_CHANGE_NAME_REQUEST, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_MEMBER_SEND_SOR_REQUEST, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_REQUEST_MAX_DAILY_XP, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleGuildRequestMaxDailyXP);
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_REQUEST_PARTY_STATE, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleGuildRequestPartyState);
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_SET_ACHIEVEMENT_TRACKING, 0x0CF0, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_GUILD_SWITCH_RANK, 0x0CD1, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_HEARTH_AND_RESURRECT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleHearthAndResurrect);
    DEFINE_OPCODE_HANDLER(CMSG_INSTANCE_LOCK_WARNING_RESPONSE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_ITEM_REFUND, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleItemRefund);
    DEFINE_OPCODE_HANDLER(CMSG_ITEM_TEXT_QUERY, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleItemTextQuery);
    DEFINE_OPCODE_HANDLER(CMSG_KEEP_ALIVE, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::Handle_EarlyProccess);
    DEFINE_OPCODE_HANDLER(CMSG_LEARN_PREVIEW_TALENTS, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleLearnPreviewTalents);
    DEFINE_OPCODE_HANDLER(CMSG_LEARN_PREVIEW_TALENTS_PET, 0x077B, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleLearnPreviewTalentsPet);
    DEFINE_OPCODE_HANDLER(CMSG_LFG_LEAVE, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleLfgLeaveOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_LFG_LFR_JOIN, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_LFG_LFR_LEAVE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_LFG_LOCK_INFO_REQUEST, 0x006B, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleLfgGetLockInfoOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_LFG_PROPOSAL_RESULT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleLfgProposalResultOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_LFG_SET_BOOT_VOTE, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleLfgSetBootVoteOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_LFG_SET_COMMENT, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_LFG_SET_ROLES, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleLfgSetRolesOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_LF_GUILD_DECLINE_RECRUIT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderDeclineRecruit);
    DEFINE_OPCODE_HANDLER(CMSG_LF_GUILD_POST_REQUEST, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderPostRequest);
    DEFINE_OPCODE_HANDLER(CMSG_LOG_DISCONNECT, 0x10B3, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_EarlyProccess);
    DEFINE_OPCODE_HANDLER(CMSG_LOOT_CURRENCY, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_LOOT_MASTER_GIVE, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleLootMasterGiveOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_ADDON_BATTLEGROUND, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleAddonMessagechatOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_ADDON_GUILD, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleAddonMessagechatOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_ADDON_OFFICER, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleAddonMessagechatOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_ADDON_PARTY, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleAddonMessagechatOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_ADDON_RAID, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleAddonMessagechatOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_MESSAGECHAT_BATTLEGROUND, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleMessagechatOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_MINIGAME_MOVE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FALL_RESET, 0x0000, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleMovementOpcodes);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FEATHER_FALL_ACK, 0x08D0, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleFeatherFallAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK, 0x0000, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleForceSpeedChangeAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FORCE_FLIGHT_SPEED_CHANGE_ACK, 0x09DA, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleForceSpeedChangeAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FORCE_PITCH_RATE_CHANGE_ACK, 0x0000, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleForceSpeedChangeAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FORCE_RUN_BACK_SPEED_CHANGE_ACK, 0x0158, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleForceSpeedChangeAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FORCE_RUN_SPEED_CHANGE_ACK, 0x10F3, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleForceSpeedChangeAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FORCE_SWIM_BACK_SPEED_CHANGE_ACK, 0x0000, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleForceSpeedChangeAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FORCE_SWIM_SPEED_CHANGE_ACK, 0x1853, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleForceSpeedChangeAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FORCE_TURN_RATE_CHANGE_ACK, 0x0000, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleForceSpeedChangeAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_FORCE_WALK_SPEED_CHANGE_ACK, 0x00DB, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleForceSpeedChangeAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_GRAVITY_DISABLE_ACK, 0x09D3, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_GRAVITY_ENABLE_ACK, 0x11D8, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_HOVER_ACK, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleMoveHoverAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_KNOCK_BACK_ACK, 0x0000, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleMoveKnockBackAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_NOT_ACTIVE_MOVER, 0x0000, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleMoveNotActiveMover);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_SET_CAN_FLY, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_SET_CAN_FLY_ACK, 0x1052, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleMoveSetCanFlyAckOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY_ACK, 0x11DB, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_SET_COLLISION_HEIGHT_ACK, 0x09FB, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleSetCollisionHeightAck);
    DEFINE_OPCODE_HANDLER(CMSG_MOVE_WATER_WALK_ACK, 0x10F2, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleMoveWaterWalkAck);
    DEFINE_OPCODE_HANDLER(CMSG_NEXT_CINEMATIC_CAMERA, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleNextCinematicCamera);
    DEFINE_OPCODE_HANDLER(CMSG_OBJECT_UPDATE_RESCUED, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_OPT_OUT_OF_LOOT, 0x06E0, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleOptOutOfLootOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_PARTY_SILENCE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_PARTY_UNSILENCE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_PET_CANCEL_AURA, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandlePetCancelAuraOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_PET_CAST_SPELL, 0x044D, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandlePetCastSpellOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_PET_LEARN_TALENT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandlePetLearnTalent);
    DEFINE_OPCODE_HANDLER(CMSG_PET_SET_ACTION, 0x12E9, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandlePetSetAction);
    DEFINE_OPCODE_HANDLER(CMSG_PING, 0x0012, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_EarlyProccess);
    DEFINE_OPCODE_HANDLER(CMSG_PLAY_DANCE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_QUERY_BATTLEFIELD_STATE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_QUERY_GUILD_MEMBERS_FOR_RECIPE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_QUERY_GUILD_MEMBER_RECIPES, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_QUERY_GUILD_RECIPES, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_QUERY_QUESTS_COMPLETED, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleQueryQuestsCompleted);
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_NPC_QUERY, 0x1DAE, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_QUEST_PUSH_RESULT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleQuestPushResult);
    DEFINE_OPCODE_HANDLER(CMSG_REALM_SPLIT, 0x18B2, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleRealmSplitOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_REDIRECTION_AUTH_PROOF, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_CATEGORY_COOLDOWNS, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::HandleRequestCategoryCooldowns);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_INSPECT_RATED_BG_STATS, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_PET_INFO, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleRequestPetInfoOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_RAID_INFO, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleRequestRaidInfoOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_RATED_BG_INFO, 0x0826, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::HandleRequestRatedBgInfo);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_RATED_BG_STATS, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::HandleRequestRatedBgStats);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_RESEARCH_HISTORY, 0x15E2, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_VEHICLE_EXIT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleRequestVehicleExit);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_VEHICLE_NEXT_SEAT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleChangeSeatsOnControlledVehicle);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_VEHICLE_PREV_SEAT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleChangeSeatsOnControlledVehicle);
    DEFINE_OPCODE_HANDLER(CMSG_REQUEST_VEHICLE_SWITCH_SEAT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleChangeSeatsOnControlledVehicle);
    DEFINE_OPCODE_HANDLER(CMSG_RESET_FACTION_CHEAT, 0x10B6, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_ROLE_POLL_BEGIN, 0x1882, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SAVE_CUF_PROFILES, 0x06E6, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::HandleSaveCUFProfiles);
    DEFINE_OPCODE_HANDLER(CMSG_SELF_RES, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleSelfResOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_SEND_SOR_REQUEST_VIA_ADDRESS, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SEND_SOR_REQUEST_VIA_BNET_ACCOUNT_ID, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SET_ACTIVE_VOICE_CHANNEL, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleSetActiveVoiceChannel);
    DEFINE_OPCODE_HANDLER(CMSG_SET_ALLOW_LOW_LEVEL_RAID1, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SET_ALLOW_LOW_LEVEL_RAID2, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SET_CHANNEL_WATCH, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleSetChannelWatch);
    DEFINE_OPCODE_HANDLER(CMSG_SET_CURRENCY_FLAGS, 0x03E4, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SET_GUILD_BANK_TEXT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleSetGuildBankTabText);
    DEFINE_OPCODE_HANDLER(CMSG_SET_PET_SLOT, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SET_PREFERED_CEMETERY, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SET_RAID_DIFFICULTY, 0x1093, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SET_RELATIVE_POSITION, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SET_SAVED_INSTANCE_EXTEND, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleSetSavedInstanceExtend);
    DEFINE_OPCODE_HANDLER(CMSG_SET_TRADE_CURRENCY, 0x0C44, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SET_VEHICLE_REC_ID_ACK, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_STOP_DANCE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SUBMIT_COMPLAIN, 0x030D, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SUSPEND_TOKEN, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_SYNC_DANCE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_TELEPORT_TO_UNIT, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_TIME_ADJUSTMENT_RESPONSE, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_TIME_SYNC_RESPONSE_FAILED, 0x0058, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_TUTORIAL_RESET, 0x0307, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleTutorialReset);
    DEFINE_OPCODE_HANDLER(CMSG_UNLEARN_SPECIALIZATION, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_UPDATE_MISSILE_TRAJECTORY, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleUpdateMissileTrajectory);
    DEFINE_OPCODE_HANDLER(CMSG_UPDATE_PROJECTILE_POSITION, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleUpdateProjectilePosition);
    DEFINE_OPCODE_HANDLER(CMSG_USED_FOLLOW, 0x0374, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_VOICE_SESSION_ENABLE, 0x15A9, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleVoiceSessionEnableOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_VOID_SWAP_ITEM, 0x0655, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::HandleVoidSwapItem);
    DEFINE_OPCODE_HANDLER(CMSG_WARDEN_DATA, 0x1816, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleWardenDataOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_WARGAME_ACCEPT, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_WARGAME_START, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(CMSG_WHOIS, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleWhoisOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_WORLD_TELEPORT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleWorldTeleportOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_WRAP_ITEM, 0x02DF, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleWrapItemOpcode);
    DEFINE_OPCODE_HANDLER(CMSG_ZONEUPDATE, 0x0000, STATUS_UNHANDLED, PROCESS_THREADSAFE, &WorldSession::HandleZoneUpdateOpcode);
    DEFINE_OPCODE_HANDLER(MSG_INSPECT_ARENA_TEAMS, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleInspectArenaTeamsOpcode);
    DEFINE_OPCODE_HANDLER(MSG_LIST_STABLED_PETS, 0x02CA, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleListStabledPetsOpcode);
    DEFINE_OPCODE_HANDLER(MSG_MOVE_CHARM_TELEPORT_CHEAT, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(MSG_MOVE_SET_RUN_MODE, 0x0979, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(MSG_MOVE_SET_WALK_MODE, 0x08D1, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(MSG_MOVE_TELEPORT_CHEAT, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(MSG_MOVE_TIME_SKIPPED, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(MSG_MOVE_TOGGLE_COLLISION_CHEAT, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(MSG_NOTIFY_PARTY_SQUELCH, 0x0000, STATUS_UNHANDLED, PROCESS_INPLACE, &WorldSession::Handle_NULL);
    DEFINE_OPCODE_HANDLER(MSG_PARTY_ASSIGNMENT, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandlePartyAssignmentOpcode);
    DEFINE_OPCODE_HANDLER(MSG_QUERY_NEXT_MAIL_TIME, 0x089B, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleQueryNextMailTime);
    DEFINE_OPCODE_HANDLER(MSG_RAID_READY_CHECK_FINISHED, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleRaidReadyCheckFinishedOpcode);
    DEFINE_OPCODE_HANDLER(MSG_SAVE_GUILD_EMBLEM, 0x0000, STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleSaveGuildEmblemOpcode);


    DEFINE_OPCODE_HANDLER(MSG_VERIFY_CONNECTIVITY,                 0x4F57, STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_EarlyProccess               ); // Not an opcode
        
#undef DEFINE_OPCODE_HANDLER
};

void OpcodeTable::InitializeServerTable()
{
#define DEFINE_OPCODE_HANDLER(opcode, opcodeNumber, status) ValidateAndSetOpcode<(opcodeNumber < NUM_OPCODE_HANDLERS), (opcode != 0)>(opcode, opcodeNumber, #opcode, status, PROCESS_INPLACE, 0);

	DEFINE_OPCODE_HANDLER(SMSG_ACCOUNT_DATA_TIMES,                 0x162B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ACHIEVEMENT_DELETED,                0x1A2F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ACHIEVEMENT_EARNED,                 0x080B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_ACTION_BUTTONS,              0x081A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ACTIVATE_TAXI_REPLY,                0x02A7, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ADDON_INFO,                         0x160A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ADD_RUNE_POWER,                     0x1860, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AI_REACTION,                        0x06AF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ALL_ACHIEVEMENT_DATA,               0x180A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ARCHAEOLOGY_SURVERY_CAST,           0x1160, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AREA_SPIRIT_HEALER_TIME,            0x188E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ATTACKER_STATE_UPDATE,              0x06AA, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ATTACK_START,                        0x1A9E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ATTACKSTOP,                         0x12AF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AUCTION_BIDDER_LIST_RESULT,         0x0B24, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AUCTION_BIDDER_NOTIFICATION,        0x11C1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AUCTION_COMMAND_RESULT,             0x1002, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AUCTION_HELLO,                      0x10A7, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AUCTION_LIST_RESULT,                0x0982, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AUCTION_OWNER_LIST_RESULT,          0x1785, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AUCTION_OWNER_NOTIFICATION,         0x1A8E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AURA_UPDATE,                        0x0072, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AUTH_CHALLENGE,                     0x0949, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_AUTH_RESPONSE,                      0x0ABA, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BARBER_SHOP_RESULT,                 0x0C3F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_LIST,                   0x160E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_MGR_EJECTED,            0x18C2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_MGR_ENTERED,            0x081B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS,                 0x0433, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_QUEUED,          0x122E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_ACTIVE,          0x1AAF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_NEEDCONFIRMATION, 0x1EAF, STATUS_NEVER   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_FAILED,          0x1140, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEGROUND_PLAYER_JOINED,         0x1E2F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEGROUND_PLAYER_LEFT,           0x0206, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLE_PET_DELETED,                 0x18AB, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLE_PET_JOURNAL,                 0x1542, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLE_PET_JOURNAL_LOCK_ACQUIRED,   0x1A0F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLE_PET_JOURNAL_LOCK_DENINED,    0x0203, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLE_PET_QUERY_NAME_RESPONSE,     0x1540, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLE_PET_SLOT_UPDATE,             0x16AF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BATTLE_PET_PET_UPDATES,             0x041A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BINDER_CONFIRM,                     0x1287, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BIND_POINT_UPDATE,                  0x0E3B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BLACKMARKET_HELLO,                  0x00AE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BLACKMARKET_REQUEST_ITEMS_RESULT,   0x128B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BLACKMARKET_BID_RESULT,             0x148A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BLACKMARKET_OUT_BID,                0x1040, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BLACKMARKET_BID_WON,                0x1060, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BREAK_TARGET,                       0x021A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BUY_FAILED,                         0x1563, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_BUY_ITEM,                           0x101A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_CLEAR_PENDING_ACTION,      0x1E3A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_COMMAND_RESULT,            0x142A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INITIAL_INVITE,      0x16AE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE,              0x15C3, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_ALERT,        0x0A9F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_REMOVED,      0x00A2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_REMOVED_ALERT, 0x122B, STATUS_NEVER   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_STATUS,       0x1C9B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_STATUS_ALERT, 0x0412, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_MODERATOR_STATUS,    0x048F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_REMOVED_ALERT,       0x049B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_UPDATED_ALERT,       0x0A0E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_RAID_LOCKOUT_ADDED,        0x0CAB, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_RAID_LOCKOUT_REMOVED,      0x11E0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_RAID_LOCKOUT_UPDATED,      0x0E1F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_SEND_CALENDAR,             0x1A0A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_SEND_EVENT,                0x12AE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_SEND_NUM_PENDING,          0x0A3F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CANCEL_AUTO_REPEAT,                 0x1E0F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CANCEL_COMBAT,                      0x0534, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CAST_FAILED,                        0x143A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CHANNEL_NOTIFY,                     0x0F06, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CHAR_CREATE,                        0x1CAA, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CHAR_CUSTOMIZE,                     0x1432, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CHAR_DELETE,                        0x0C9F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ENUM_CHARACTERS_RESULT,             0x11C3, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CHAR_RENAME,                        0x0CBF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CHAT_PLAYER_AMBIGUOUS,              0x061A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CHAT_PLAYER_NOT_FOUND,              0x1082, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CHAT_RESTRICTED,                    0x1A3B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CLEAR_COOLDOWNS,                    0x1458, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CLEAR_TARGET,                       0x1061, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CLIENTCACHE_VERSION,                0x002A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CLIENT_CONTROL_UPDATE,              0x1043, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CONTACT_LIST,                       0x1F22, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CONVERT_RUNE,                       0x1A1B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_COOLDOWN_EVENT,                     0x162A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CORPSE_MAP_POSITION_QUERY_RESPONSE, 0x1A3A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CORPSE_NOT_IN_INSTANCE,             0x089E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CORPSE_QUERY,                       0x0E0B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CORPSE_RECLAIM_DELAY,               0x022A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CREATURE_QUERY_RESPONSE,            0x048B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CRITERIA_DELETED,                   0x1C33, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CRITERIA_UPDATE,                    0x0E9B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DB_REPLY,                           0x103B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DEATH_RELEASE_LOC,                  0x1063, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DEFENSE_MESSAGE,                    0x0A1F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DESTROY_OBJECT,                     0x14C2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DISMOUNT,                           0x0E3A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DUEL_COMPLETE,                      0x1C0A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DUEL_COUNTDOWN,                     0x129F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DUEL_INBOUNDS,                      0x163A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DUEL_OUTOFBOUNDS,                   0x001A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DUEL_REQUESTED,                     0x0022, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DUEL_WINNER,                        0x10E1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_DURABILITY_DAMAGE_DEATH,            0x1E3E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_EMOTE,                              0x0987, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ENABLE_BARBER_SHOP,                 0x1222, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ENVIRONMENTALDAMAGELOG,             0x0DF1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_EQUIPMENT_SET_ID,                   0x0006, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_EXPLORATION_EXPERIENCE,             0x189A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_FEATURE_SYSTEM_STATUS,              0x16BB, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_FRIEND_STATUS,                      0x0532, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GAMEOBJECT_CUSTOM_ANIM,             0x001F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GAMEOBJECT_DESPAWN_ANIM,            0x108B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GAMEOBJECT_QUERY_RESPONSE,          0x06BF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GM_TICKET_RESPONSE,                 0x0207, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GM_TICKET_UPDATE,                   0x02A6, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GM_TICKET_GET_TICKET,               0x129B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GM_TICKET_SYSTEM_STATUS,            0x163B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GM_TICKET_CASE_STATUS,              0x148E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GOSSIP_COMPLETE,                    0x034E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GOSSIP_MESSAGE,                     0x0244, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GOSSIP_POI,                         0x0785, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GROUP_DECLINE,                      0x17A3, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GROUP_DESTROYED,                    0x1B27, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GROUP_INVITE,                       0x0A8F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GROUP_LIST,                         0x0CBB, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GROUP_ROLE_POLL_INFORM,             0x1007, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GROUP_SET_LEADER,                   0x18BF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GROUP_SET_ROLE,                     0x1E1F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_BANK_LIST,                    0x0B79, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_BANK_LOG_QUERY_RESULT,        0x0FF0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_BANK_MONEY_WITHDRAWN,         0x0B78, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_BANK_QUERY_TEXT_RESULT,       0x1AE0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_CHALLENGE_UPDATED,            0x0AE9, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_COMMAND_RESULT,               0x0EF1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_DECLINE,                      0x1AF9, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_EVENT_BANK_MONEY_CHANGED,     0x0F68, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_EVENT_BANK_TAB_ADDED,         0x1BE9, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_EVENT_BANK_TAB_TEXT_CHANGED,  0x0A70, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_EVENT_BANK_TAB_MODIFIED,      0x0BF1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_EVENT_LOG_QUERY_RESULT,       0x1AF1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_INVITE,                       0x0F71, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_INVITE_CANCEL,                0x0FE1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_MAX_DAILY_XP,                 0x1BE1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_MEMBER_DAILY_RESET,           0x1BE8, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_MEMBER_UPDATE_NOTE,           0x0BE1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_NEWS_UPDATE,                  0x0AE8, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_PARTY_STATE_RESPONSE,         0x0A78, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_PERMISSIONS_QUERY_RESULTS,    0x0FF9, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_QUERY_RESPONSE,               0x1B79, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_RANKS,                        0x0A79, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_RANKS_UPDATE,                 0x0A60, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_REPUTATION_WEEKLY_CAP,        0x1A71, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_REWARDS_LIST,                 0x1A69, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_ROSTER,                       0x0BE0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_SET_GUILD_MASTER,             0x0E69, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_XP,                           0x0AF0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_XP_GAIN,                      0x0FE0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_HIGHEST_THREAT_UPDATE,              0x14AE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_HOTFIX_NOTIFY_BLOB,                 0x1EBA, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_INITIALIZE_FACTIONS,                0x0AAA, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SEND_KNOWN_SPELLS,                  0x045A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SETUP_CURRENCY,                     0x1A8B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_INIT_WORLD_STATES,                  0x1560, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_INSPECT_HONOR_STATS,                0x1A1E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_INSPECT_RESULTS,                    0x1842, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_INSTANCE_RESET,                     0x160F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_INVENTORY_CHANGE_FAILURE,           0x0C1E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ITEM_ENCHANT_TIME_UPDATE,           0x10A2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ITEM_EXPIRE_PURCHASE_REFUND,        0x0E33, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ITEM_PURCHASE_REFUND_RESULT,        0x049E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ITEM_PUSH_RESULT,                   0x0E0A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ITEM_TIME_UPDATE,                   0x18C1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LEARNED_SPELL,                      0x129A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LEVELUP_INFO,                       0x1961, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LF_GUILD_APPLICANT_LIST_UPDATED,    0x0B71, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LF_GUILD_APPLICATIONS_LIST_CHANGED, 0x1A70, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LF_GUILD_BROWSE_UPDATED,            0x0F69, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LF_GUILD_MEMBERSHIP_LIST_UPDATED,   0x0AE0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LF_GUILD_POST_UPDATED,              0x1B71, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LIST_INVENTORY,                     0x1AAE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOAD_CUF_PROFILES,                  0x0E32, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOAD_EQUIPMENT_SET,                 0x18E2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOGIN_SET_TIME_SPEED,               0x082B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOGIN_VERIFY_WORLD,                 0x1C0F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOGOUT_CANCEL_ACK,                  0x0AAF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOGOUT_COMPLETE,                    0x142F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOGOUT_RESPONSE,                    0x008F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOG_XPGAIN,                         0x1E9A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_CLEAR_MONEY,                   0x1632, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_LIST,                          0x1C3F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_MASTER_LIST,                   0x02BF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_MONEY_NOTIFY,                  0x14C0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_RELEASE_RESPONSE,              0x123F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_REMOVED,                       0x0C3E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_RESPONSE,                      0x128A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MAIL_LIST_RESULT,                   0x1C0B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MESSAGECHAT,                        0x1A9A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MINIMAP_PING,                       0x168F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MIRROR_IMAGE_COMPONENTED_DATA,      0x04D9, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MIRROR_IMAGE_CREATURE_DATA,         0x04D0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MODIFY_COOLDOWN,                    0x1E2E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ON_MONSTER_MOVE,                    0x1A07, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOTD,                               0x183B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOUNTSPECIAL_ANIM,                  0x003A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_FEATHER_FALL,                  0x0C60, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_KNOCK_BACK,                    0x0562, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_LAND_WALK,                     0x086A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_NORMAL_FALL,                   0x08E0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_ROOT,                          0x15AE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_ACTIVE_MOVER,              0x0C6D, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_CAN_FLY,                   0x178D, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_COLLISION_HEIGHT,          0x0250, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_FLIGHT_BACK_SPEED,         0x0319, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_FLIGHT_SPEED,              0x006E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_HOVER,                     0x1802, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_PITCH_RATE,                0x17AB, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_RUN_BACK_SPEED,            0x0A83, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_RUN_SPEED,                 0x184C, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_SWIM_BACK_SPEED,           0x0962, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_SWIM_SPEED,                0x0817, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_TURN_RATE,                 0x0069, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_VEHICLE_REC_ID,            0x0861, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_WALK_SPEED,                0x0469, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_TELEPORT,                      0x0B39, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UNROOT,                        0x1FAE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UNSET_CAN_FLY,                 0x0162, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UNSET_HOVER,                   0x02D3, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_FLIGHT_SPEED,           0x00E1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_RUN_SPEED,              0x158E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_WATER_WALK,                    0x1F9A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUERY_PLAYER_NAME_RESPONSE,         0x169B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_REALM_NAME_QUERY_RESPONSE,          0x063E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_NEW_TAXI_PATH,                      0x141B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_NEW_WORLD,                          0x1C3B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_NOTIFICATION,                       0x0C2A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_NPC_TEXT_UPDATE,                    0x140A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ON_CANCEL_EXPECTED_RIDE_VEHICLE_AURA, 0x1A2A, STATUS_NEVER   ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PARTYKILLLOG,                       0x048A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PARTY_COMMAND_RESULT,               0x0F86, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PETITION_ALREADY_SIGNED,            0x0286, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PETITION_QUERY_RESPONSE,            0x1083, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PETITION_RENAME_RESULT,             0x082A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PETITION_SHOWLIST,                  0x10A3, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PETITION_SHOW_SIGNATURES,           0x00AA, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PETITION_SIGN_RESULTS,              0x06AE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PET_NAME_INVALID,                   0x028E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PET_NAME_QUERY_RESPONSE,            0x0ABE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PET_SPELLS_MESSAGE,                 0x095A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PLAYED_TIME,                        0x11E2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PLAYERBOUND,                        0x088E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PLAYER_MOVE,                        0x1A32, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PLAY_ONE_SHOT_ANIM_KIT,             0x043E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PLAY_SOUND,                         0x102A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PLAY_SPELL_VISUAL,                  0x061E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PLAY_SPELL_VISUAL_KIT,              0x11E3, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PONG,                               0x1969, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_POWER_UPDATE,                       0x109F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PRE_RESURRECT,                      0x19C0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PROPOSE_LEVEL_GRANT,                0x109A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PVP_LOG_DATA,                       0x1E8F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PVP_OPTIONS_ENABLED,                0x080A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PVP_SEASON,                         0x069B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUERY_TIME_RESPONSE,                0x100F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTGIVER_OFFER_REWARD,            0x074F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTGIVER_QUEST_COMPLETE,          0x0346, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTGIVER_QUEST_DETAILS,           0x134C, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTGIVER_QUEST_INVALID,           0x027D, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTGIVER_QUEST_LIST,              0x02D4, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTGIVER_REQUEST_ITEMS,           0x0277, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTGIVER_STATUS,                  0x1275, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUEST_GIVER_STATUS_MULTIPLE,        0x06CE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTLOG_FULL,                      0x07FD, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTUPDATE_ADD_CREDIT,             0x1645, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTUPDATE_ADD_PVP_KILL,           0x0256, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUESTUPDATE_COMPLETE,               0x0776, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUEST_CONFIRM_ACCEPT,               0x13C7, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUEST_POI_QUERY_RESPONSE,           0x067F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUEST_PUSH_RESULT,                  0x074D, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_QUEST_QUERY_RESPONSE,               0x0276, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RAID_READY_CHECK,                   0x1C8E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RAID_READY_CHECK_COMPLETED,         0x15C2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RAID_READY_CHECK_CONFIRM,           0x02AF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RAID_TARGET_UPDATE_ALL,             0x0283, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RAID_TARGET_UPDATE_SINGLE,          0x160B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RANDOM_ROLL,                        0x141A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RANDOMIZE_CHAR_NAME,                0x169F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_READ_ITEM_RESULT_FAILED,            0x0E8B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_READ_ITEM_RESULT_OK,                0x0305, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RECEIVED_MAIL,                      0x182B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_REFER_A_FRIEND_FAILURE,             0x021E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_REFORGE_RESULT,                     0x141E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_REMOVED_SPELL,                      0x14C3, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_REQUEST_CEMETERY_LIST_RESPONSE,     0x042A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_REQUEST_PVP_REWARDS_RESPONSE,       0x08AA, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RESPEC_WIPE_CONFIRM,                0x02AB, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RESPOND_INSPECT_ACHIEVEMENTS,       0x009E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_RESURRECT_REQUEST,                  0x1062, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SELL_ITEM,                          0x048E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SEND_MAIL_RESULT,                   0x1A9B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SEND_UNLEARN_SPELLS,                0x10F1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SERVER_MESSAGE,                     0x0302, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_DUNGEON_DIFFICULTY,             0x1283, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_FACTION_STANDING,               0x10AA, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_FACTION_VISIBLE,                0x1E8E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_FLAT_SPELL_MODIFIER,            0x10F2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_FORCED_REACTIONS,               0x068F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_ITEM_PURCHASE_DATA,             0x1C9A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_PCT_SPELL_MODIFIER,             0x09D3, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_PHASE_SHIFT_CHANGE,                 0x02A2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT,   0x180E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_PLAY_HOVER_ANIM,                0x069F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_PROFICIENCY,                    0x1440, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(MSG_SET_RAID_DIFFICULTY,                 0x0591, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_TIME_ZONE_INFORMATION,          0x19C1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SET_VEHICLE_REC_ID,                 0x149F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SHOW_TAXI_NODES,                    0x1E1A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SHOW_BANK,                          0x0007, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SHOW_NEURTRAL_PLAYER_FACTION_SELECT_UI, 0x15E0, STATUS_NEVER); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_ENERGIZE_LOG,                 0x0D79, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_HEAL_LOG,                     0x09FB, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELLINSTAKILLLOG,                  0x09F8, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELLINTERRUPTLOG,                  0x1851, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_EXECUTE_LOG,                  0x00D8, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_NON_MELEE_DAMAGE_LOG,         0x1450, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_CATEGORY_COOLDOWN,                  0x01DB, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_CHANNEL_START,                0x10F9, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_CHANNEL_UPDATE,               0x11D9, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_COOLDOWN,                     0x0452, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_DELAYED,                      0x087A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_FAILED_OTHER,                 0x040B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_FAILURE,                      0x04AF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_GO,                           0x09D8, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_PERIODIC_AURA_LOG,            0x0CF2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_START,                        0x107A, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPIRIT_HEALER_CONFIRM,              0x1EAA, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_GRAVITY_DISABLE,        0x0845, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_GRAVITY_ENABLE,         0x0865, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_ROOT,                   0x0728, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_FEATHER_FALL,       0x1893, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_FLIGHT_BACK_SPEED,  0x0B28, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_FLIGHT_SPEED,       0x1DAB, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_FLYING,             0x1046, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_HOVER,              0x0258, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_LAND_WALK,          0x18B6, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_NORMAL_FALL,        0x0B08, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_PITCH_RATE,         0x0AB3, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_RUN_BACK_SPEED,     0x1F9F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_RUN_MODE,           0x0B18, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_RUN_SPEED,          0x02F1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_SWIM_BACK_SPEED,    0x0046, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_SWIM_SPEED,         0x1D8E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_TURN_RATE,          0x0832, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_WALK_MODE,          0x1865, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_WALK_SPEED,         0x08B2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_WATER_WALK,         0x1823, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_START_SWIM,             0x0F29, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_STOP_SWIM,              0x1798, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_UNROOT,                 0x01E1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_UNSET_FLYING,           0x0DE2, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_UNSET_HOVER,            0x0CE1, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_STANDSTATE_UPDATE,                  0x1C12, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_START_MIRROR_TIMER,                 0x0E12, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_START_TIMER,                        0x0E3F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_STOP_MIRROR_TIMER,                  0x1026, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_SUMMON_REQUEST,                     0x081F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TABARD_VENDOR_ACTIVATE,             0x0A3E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_TALENT_DATA,                 0x0A9B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TAXI_NODE_STATUS,                   0x169E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TEXT_EMOTE,                         0x002E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_THREAT_CLEAR,                       0x180B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_THREAT_REMOVE,                      0x1960, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_THREAT_UPDATE,                      0x0632, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TIME_SYNC_REQUEST,                  0x1A8F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TITLE_EARNED,                       0x068E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TITLE_LOST,                         0x12BF, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TOTEM_CREATED,                      0x1C8F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TRADE_STATUS,                       0x1963, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TRADE_STATUS_EXTENDED,              0x181E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TRAINER_BUY_FAILED,                 0x042E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TRAINER_LIST,                       0x189F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TRANSFER_ABORTED,                   0x0C8F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TRANSFER_PENDING,                   0x061B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TRIGGER_CINEMATIC,                  0x0B01, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TRIGGER_MOVIE,                      0x1C2E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TURN_IN_PETITION_RESULTS,           0x0E13, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_TUTORIAL_FLAGS,                     0x1B90, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_ACCOUNT_DATA,                0x0AAE, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_COMBO_POINTS,                0x082F, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_INSTANCE_OWNERSHIP,          0x10E0, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_LAST_INSTANCE,               0x189B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_OBJECT,                      0x1792, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_WORLD_STATE,                 0x121B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_USE_EQUIPMENT_SET_RESULT,           0x0A2B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_VOID_STORAGE_CONTENTS,              0x008B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_VOID_TRANSFER_RESULT,               0x1C9E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_WEATHER,                            0x06AB, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_WEEKLY_RESET_CURRENCY,              0x023E, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_WHO,                                0x161B, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_WORLD_SERVER_INFO,                  0x0082, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_UI_TIME,                            0x0027, STATUS_NEVER    ); // 5.4.8 18414
    DEFINE_OPCODE_HANDLER(SMSG_ZONE_UNDER_ATTACK,                  0x10C2, STATUS_NEVER    ); // 5.4.8 18414


    //UNHANDLED

    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_SWIM_BACK_SPEED, 0x025A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_SWIM_SPEED, 0x01E2, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_TELEPORT, 0x15A9, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_TURN_RATE, 0x0D62, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_WALK_SPEED, 0x0047, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MULTIPLE_PACKETS, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_NEW_WORLD_ABORT, 0x0C1B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_NOTIFY_DANCE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_NOTIFY_DEST_LOC_SPELL_CAST, 0x1E0E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_OFFER_PETITION_ERROR, 0x161E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ACCOUNT_CRITERIA_UPDATE, 0x189E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ACCOUNT_INFO_RESPONSE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ACCOUNT_RESTRICTED_WARNING, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ALL_ACCOUNT_CRITERIA, 0x0A9E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_AREA_TRIGGER_MESSAGE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_AREA_TRIGGER_MOVEMENT_UPDATE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ARENA_ERROR, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ARENA_UNIT_DESTROYED, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ARENA_TEAM_CHANGE_FAILED_QUEUED, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ARENA_TEAM_COMMAND_RESULT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ARENA_TEAM_EVENT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ARENA_TEAM_INVITE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ARENA_TEAM_QUERY_RESPONSE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ARENA_TEAM_ROSTER, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ARENA_TEAM_STATS, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ATTACKSWING_BADFACING, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ATTACKSWING_CANT_ATTACK, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ATTACKSWING_DEADTARGET, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ATTACKSWING_NOTINRANGE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_AUCTION_LIST_PENDING_SALES, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_AUCTION_REMOVED_NOTIFICATION, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_AURA_POINTS_DEPLETED, 0x1553, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_AVAILABLE_VOICE_CHANNEL, 0x029A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_AVERAGE_ITEM_LEVEL_INFORM, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_MGR_EJECT_PENDING, 0x003E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_MGR_ENTRY_INVITE, 0x1226, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_MGR_EXIT_REQUEST, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_MGR_QUEUE_INVITE, 0x142E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE, 0x08BE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_MGR_STATE_CHANGE, 0x083A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_PLAYER_POSITIONS, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_PORT_DENIED, 0x149A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_RATED_INFO, 0x0EBA, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_WAITFORGROUPS, 0x10A6, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_BATTLEGROUND_INFO_THROTTLED, 0x1E1E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_ARENA_TEAM, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_NOTES, 0x11C0, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_NOTES_ALERT, 0x1286, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CAMERA_SHAKE, 0x0C3A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CHANNEL_LIST, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CHANNEL_MEMBER_COUNT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CHARACTER_LOGIN_FAILED, 0x1A0B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CHAR_FACTION_CHANGE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CHAT_IGNORED_ACCOUNT_MUTED, 0x0C3B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CHAT_NOT_IN_PARTY, 0x0A8A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CHAT_SERVER_DISCONNECTED, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CHAT_SERVER_RECONNECTED, 0x0A2E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CHAT_WRONG_FACTION, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CLEAR_BOSS_EMOTES, 0x062B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CLEAR_COOLDOWN, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CLEAR_FAR_SIGHT_IMMEDIATE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMBAT_EVENT_FAILED, 0x18C3, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMMENTATOR_MAP_INFO, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMMENTATOR_PARTY_INFO, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMMENTATOR_PLAYER_INFO, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMMENTATOR_SKIRMISH_QUEUE_RESULT1, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMMENTATOR_SKIRMISH_QUEUE_RESULT2, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMMENTATOR_STATE_CHANGED, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMPLAIN_RESULT, 0x128F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMPRESSED_MOVES, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMSAT_CONNECT_FAIL, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMSAT_DISCONNECT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COMSAT_RECONNECT_TRY, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_COOLDOWN_CHEAT, 0x0432, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CROSSED_INEBRIATION_THRESHOLD, 0x1E9E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CURRENCY_LOOT_REMOVED, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CURRENCY_LOOT_RESTORED, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CUSTOM_LOAD_SCREEN, 0x1CAF, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DAMAGE_CALC_LOG, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DANCE_QUERY_RESPONSE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DEBUG_RUNE_REGEN, 0x12A7, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DESTRUCTIBLE_BUILDING_DAMAGE, 0x14BF, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DIFFERENT_INSTANCE_FROM_PARTY, 0x120B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DISENCHANT_CREDIT, 0x10BB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DISMOUNTRESULT, 0x062F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DISPEL_FAILED, 0x085B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DISPLAY_GAME_ERROR, 0x181F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DONT_AUTO_PUSH_SPELLS_TO_ACTION_BAR, 0x1C8B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DROP_NEW_CONNECTION, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_DUMP_RIDE_TICKETS_RESPONSE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ECHO_PARTY_SQUELCH, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ENCHANTMENT_LOG, 0x12A3, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_EXPECTED_SPAM_RECORDS, 0x18C0, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_FAILED_PLAYER_CONDITION, 0x1223, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_FEIGN_DEATH_RESISTED, 0x029E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_FISH_ESCAPED, 0x0227, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_FISH_NOT_HOOKED, 0x10BE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_FLIGHT_SPLINE_SYNC, 0x0063, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_FLOOD_DETECTED, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_FORCED_DEATH_UPDATE, 0x0E8F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RESUME_COMMS, 0x0969, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_FORCE_SET_VEHICLE_REC_ID, 0x149F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_FORGE_MASTER_SET, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GAMEOBJECT_PAGETEXT, 0x14AF, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GAMEOBJECT_RESET_STATE, 0x100E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GAMESPEED_SET, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GAMETIME_SET, 0x0A0F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GAMETIME_UPDATE, 0x0E1B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GAME_EVENT_DEBUG_LOG, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GAME_OBJECT_ACTIVATE_ANIM_KIT, 0x0C8A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GAME_STORE_INGAME_BUY_FAILED, 0x023A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GMRESPONSE_DB_ERROR, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GM_RESPONSE_STATUS_UPDATE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GM_TICKET_UPDATE_TEXT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GM_TICKET_STATUS_UPDATE, 0x000B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GM_MESSAGE_CHAT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GM_PLAYER_INFO, 0x102B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GODMODE, 0x1862, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GROUPACTION_THROTTLED, 0x0287, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GROUP_CANCEL, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GROUP_UNINVITE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_ACHIEVEMENT_DATA, 0x0EF8, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_ACHIEVEMENT_DELETED, 0x1E61, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_ACHIEVEMENT_EARNED, 0x1BF1, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_ACHIEVEMENT_MEMBERS, 0x1B70, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_CHALLENGE_COMPLETED, 0x1AF8, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_CHANGE_NAME_RESULT, 0x0BE9, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_COMMAND_RESULT_2, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_CRITERIA_DATA, 0x1BF0, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_CRITERIA_DELETED, 0x1B60, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_EVENT, 0x0000, STATUS_UNHANDLED); // obsolete
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_FLAGGED_FOR_RENAME, 0x0FE9, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_MEMBERS_FOR_RECIPE, 0x0BF0, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_MEMBER_RECIPES, 0x0EE1, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_MOVE_COMPLETE, 0x0BE8, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_MOVE_STARTING, 0x0AE1, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_NEWS_DELETED, 0x0F70, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_RECIPES, 0x0FF1, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_RENAMED, 0x0E70, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_REPUTATION_REACTION_CHANGED, 0x0E71, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_RESET, 0x1AF0, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_GUILD_UPDATE_ROSTER, 0x0AF1, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_HEALTH_UPDATE, 0x148B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_HOTFIX_NOTIFY, 0x0EBF, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_INSPECT_PVP, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_INSTANCE_LOCK_WARNING_QUERY, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_INSTANCE_RESET_FAILED, 0x0026, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_INSTANCE_SAVE_CREATED, 0x1EAE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_INVALIDATE_DANCE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_INVALIDATE_PLAYER, 0x102E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_INVALID_PROMOTION_CODE, 0x1A0E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ITEM_ADD_PASSIVE, 0x161A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ITEM_COOLDOWN, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ITEM_REMOVE_PASSIVE, 0x0A2F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ITEM_SEND_PASSIVE, 0x122F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ITEM_TEXT_QUERY_RESPONSE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LEARNED_DANCE_MOVES, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_BOOT_PROPOSAL_UPDATE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_DISABLED, 0x008E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_JOIN_RESULT, 0x18E3, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_OFFER_CONTINUE, 0x1EAB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_PARTY_INFO, 0x168E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_PLAYER_INFO, 0x1861, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_PLAYER_REWARD, 0x121A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_PROPOSAL_UPDATE, 0x1E3B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_QUEUE_STATUS, 0x1006, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_ROLE_CHECK_UPDATE, 0x12BB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_ROLE_CHOSEN, 0x1A1F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_SLOT_INVALID, 0x0C12, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_TELEPORT_DENIED, 0x063B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_UPDATE_SEARCH, 0x1161, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_UPDATE_STATUS, 0x0C2E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LFG_UPDATE_STATUS_NONE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LF_GUILD_COMMAND_RESULT, 0x1A79, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LF_GUILD_RECRUIT_LIST_UPDATED, 0x0E68, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_ALL_PASSED, 0x0EBB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_CONTENTS, 0x121F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_ITEM_NOTIFY, 0x080F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_ROLL, 0x1840, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_ROLL_WON, 0x0A3A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_SLOT_CHANGED, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_LOOT_START_ROLL, 0x0EAA, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MAP_OBJ_EVENTS, 0x00BB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MEETINGSTONE_COMPLETE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MEETINGSTONE_IN_PROGRESS, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MESSAGE_BOX, 0x02AE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MINIGAME_SETUP, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MINIGAME_STATE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MISSILE_CANCEL, 0x1203, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MONEY_NOTIFY, 0x0C0F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ON_MONSTER_MOVE_TRANSPORT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOUNTRESULT, 0x0E0F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_COLLISION_DISABLE, 0x15B8, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_COLLISION_ENABLE, 0x1826, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_GRAVITY_DISABLE, 0x159F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_GRAVITY_ENABLE, 0x0A27, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY, 0x11DB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_COMPOUND_STATE, 0x0061, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UNSET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY, 0x0868, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_COLLISION_HEIGHT, 0x1812, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_FLIGHT_BACK_SPEED, 0x036A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_KNOCK_BACK, 0x0251, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_PITCH_RATE, 0x09E2, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_RUN_BACK_SPEED, 0x08A3, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_OPEN_CONTAINER, 0x14BB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_OPEN_LFG_DUNGEON_FINDER, 0x0E8A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_OVERRIDE_LIGHT, 0x068A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PAGE_TEXT_QUERY_RESPONSE, 0x081E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PARTY_MEMBER_STATS, 0x0A9A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PARTY_MEMBER_STATS_FULL, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PAUSE_MIRROR_TIMER, 0x162E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PETGODMODE, 0x1940, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_ACTION_FEEDBACK, 0x080E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_ACTION_SOUND, 0x15E2, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_ADDED, 0x123A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_BROKEN, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_CAST_FAILED, 0x149B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_DISMISS_SOUND, 0x1ABB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_GUIDS, 0x1227, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_LEARNED_SPELL, 0x0282, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_MODE, 0x163F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_REMOVED_SPELL, 0x1CAE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_RENAMEABLE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_SLOT_UPDATED, 0x069A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_TAME_FAILURE, 0x040E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PET_UPDATE_COMBO_POINTS, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PLAYERBINDERROR, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PLAYER_DIFFICULTY_CHANGE, 0x128E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PLAYER_SKINNED, 0x1463, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PLAY_DANCE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PLAY_MUSIC, 0x0023, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PLAY_OBJECT_SOUND, 0x1443, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PLAY_TIME_WARNING, 0x062A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PROCRESIST, 0x12BE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_PVP_CREDIT, 0x100A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_QUERY_QUESTS_COMPLETED_RESPONSE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_QUESTGIVER_QUEST_FAILED, 0x12DE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_QUESTUPDATE_FAILED, 0x07DD, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_QUESTUPDATE_FAILEDTIMER, 0x06FF, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_QUEST_FORCE_REMOVE, 0x07C5, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_QUEST_NPC_QUERY_RESPONSE, 0x036D, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RAID_GROUP_ONLY, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RAID_INSTANCE_INFO, 0x16BF, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RAID_INSTANCE_MESSAGE, 0x0CAF, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RAID_MARKERS_CHANGED, 0x008A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RAID_READY_CHECK_THROTTLED_ERROR, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RAID_SUMMON_FAILED, 0x108A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RATED_BG_RATING, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RATED_BG_STATS, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_REALM_SPLIT, 0x1A2E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_REAL_GROUP_UPDATE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_CONNECT_TO, 0x1149, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_REFER_A_FRIEND_EXPIRED, 0x1143, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_REPORT_PVP_AFK_RESULT, 0x18BE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RESEARCH_COMPLETE, 0x0C0E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RESEARCH_SETUP_HISTORY, 0x08AB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RESET_COMPRESSION_CONTEXT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RESET_FAILED_NOTIFY, 0x10AE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RESYNC_RUNES, 0x15E3, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_ROLE_POLL_BEGIN, 0x1007, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_RWHOIS, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SERVERTIME, 0x1C3E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SERVER_FIRST_ACHIEVEMENT, 0x028B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SERVER_INFO_RESPONSE, 0x022E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SERVER_PERF, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SET_DF_FAST_LAUNCH_RESULT, 0x1041, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SET_FACTION_ATWAR, 0x0C9B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SET_PROJECTILE_POSITION, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SHOW_RATINGS, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SOCKET_GEMS_RESULT, 0x12A6, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SOR_START_EXPERIENCE_INCOMPLETE, 0x0083, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SPELLBREAKLOG, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SPELLDAMAGESHIELD, 0x05F3, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SPELLDISPELLOG, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SPELLLOGMISS, 0x1570, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SPELLORDAMAGE_IMMUNE, 0x08FB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SPELLSTEALLOG, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SPELL_UPDATE_CHAIN_TARGETS, 0x0D52, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_COLLISION_DISABLE, 0x15B8, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_COLLISION_ENABLE, 0x1826, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_SET_ANIM, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_STABLE_RESULT, 0x14BE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_STOP_DANCE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_STREAMING_MOVIE, 0x1843, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SUMMON_CANCEL, 0x000A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SUPERCEDED_SPELL, 0x1943, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SUPPRESS_NPC_GREETINGS, 0x100B, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SUSPEND_COMMS, 0x1D48, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_SUSPEND_TOKEN_RESPONSE, 0x18BA, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_TALENTS_ERROR, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_TALENTS_INVOLUNTARILY_RESET, 0x088A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_TEST_DROP_RATE_RESULT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_TIME_ADJUSTMENT, 0x04AA, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_TOGGLE_XP_GAIN, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_UNIT_HEALTH_FREQUENT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_UNIT_SPELLCAST_START, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_ACCOUNT_DATA_COMPLETE, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_CURRENCY, 0x129E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_CURRENCY_WEEK_LIMIT, 0x0E2A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_DUNGEON_ENCOUNTER_FOR_LOOT, 0x1863, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_INSTANCE_ENCOUNTER_UNIT, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_UPDATE_SERVER_PLAYER_POSITION, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_USERLIST_ADD, 0x1462, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_USERLIST_REMOVE, 0x0AAB, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_USERLIST_UPDATE, 0x063A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_VOICESESSION_FULL, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_VOICE_CHAT_STATUS, 0x10E2, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_VOICE_PARENTAL_CONTROLS, 0x04BF, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_VOICE_SESSION_LEAVE, 0x15C0, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_VOICE_SESSION_ROSTER_UPDATE, 0x000E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_VOICE_SET_TALKER_MUTED, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_VOID_ITEM_SWAP_RESPONSE, 0x1EBF, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_VOID_STORAGE_FAILED, 0x19C2, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_VOID_STORAGE_TRANSFER_CHANGES, 0x14BA, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_WAIT_QUEUE_FINISH, 0x060E, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_WAIT_QUEUE_UPDATE, 0x0C2F, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_WARDEN_DATA, 0x0C0A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_WARGAME_CHECK_ENTRY, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_WARGAME_REQUEST_SENT, 0x0CAE, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_WEEKLY_LAST_RESET, 0x0000, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_WEEKLY_SPELL_USAGE, 0x00F9, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_WEEKLY_SPELL_USAGE_UPDATE, 0x117A, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_WHOIS, 0x12BA, STATUS_UNHANDLED);
    DEFINE_OPCODE_HANDLER(SMSG_XP_GAIN_ABORTED, 0x1A2B, STATUS_UNHANDLED);


    DEFINE_OPCODE_HANDLER(MSG_VERIFY_CONNECTIVITY,                 0x4F57, STATUS_NEVER    ); // Not an opcode

    #undef DEFINE_OPCODE_HANDLER
};
