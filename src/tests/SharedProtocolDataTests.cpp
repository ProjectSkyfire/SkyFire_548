/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "Protocol/Opcodes.h"
#include "Packets/WorldPacket.h"
#include "DataStores/DBCStructure.h"
#include "DataStores/DB2Structure.h"
#include "DataStores/DBCfmt.h"
#include "DataStores/DB2fmt.h"
#include <cstddef>
#include <type_traits>
#include <string>

// This target deliberately has no game include paths or game library dependency.
static_assert(GetOpcodeMetadata(CMSG_MESSAGECHAT_WHISPER, false).OpcodeNumber == 0x123E);
static_assert(GetOpcodeMetadata(SMSG_MESSAGECHAT, true).OpcodeNumber == 0x1A9A);
static_assert(GetOpcodeMetadata(MSG_VERIFY_CONNECTIVITY, false).OpcodeNumber == 0x4F57);
static_assert(GetOpcodeMetadata(MSG_VERIFY_CONNECTIVITY, true).OpcodeNumber == 0x4F57);
static_assert(GetOpcodeMetadata(SMSG_MESSAGECHAT, false).Name == nullptr);
static_assert(GetOpcodeMetadata(UNKNOWN_OPCODE, true).Name == nullptr);
static_assert(std::is_base_of<ByteBuffer, WorldPacket>::value);
static_assert(sizeof(ItemEntry) == 20);
static_assert(offsetof(ItemEntry, InventoryType) == 16);
static_assert(sizeof(ChatChannelsEntry) == 8 + sizeof(char*));
static_assert(offsetof(ChatChannelsEntry, pattern) == 8);
static_assert(sizeof(((ItemSparseEntry*)nullptr)->ItemStatType) == 40);
static_assert(sizeof(((ItemSparseEntry*)nullptr)->SpellId) == 20);
static_assert(sizeof(((ItemSparseEntry*)nullptr)->Color) == 12);

int main()
{
    if (GetOpcodeNameForLogging(SMSG_MESSAGECHAT, true).find("SMSG_MESSAGECHAT 0x1A9A") == std::string::npos)
        return 1;
    if (GetOpcodeNameForLogging(UNKNOWN_OPCODE, false).find("INVALID OPCODE") == std::string::npos)
        return 2;
    if (GetOpcodeNameForLogging(SMSG_MESSAGECHAT, false).find("UNKNOWN OPCODE") == std::string::npos)
        return 3;
    return 0;
}
