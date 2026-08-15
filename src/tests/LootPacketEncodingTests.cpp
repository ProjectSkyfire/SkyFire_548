/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LootPacketEncoding.h"
#include "Errors.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

ByteBufferPositionException::ByteBufferPositionException(bool add, size_t pos, size_t size, size_t valueSize)
{
    std::ostringstream message;
    message << "ByteBuffer position error: " << add << ' ' << pos << ' ' << size << ' ' << valueSize;
    this->message().assign(message.str());
}

ByteBufferSourceException::ByteBufferSourceException(size_t pos, size_t size, size_t valueSize)
{
    std::ostringstream message;
    message << "ByteBuffer source error: " << pos << ' ' << size << ' ' << valueSize;
    this->message().assign(message.str());
}

namespace Skyfire
{
    void Assert(char const* /*file*/, int /*line*/, char const* /*function*/, char const* message)
    {
        std::cerr << "Assertion failed: " << message << '\n';
        std::abort();
    }
}

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (condition)
            return true;

        std::cerr << message << '\n';
        return false;
    }
}

int main()
{
    ByteBuffer packet;
    Skyfire::Looting::WriteLootItemBitData(3, &packet);

    packet.rpos(0);

    bool passed = true;
    passed &= Expect(packet.ReadBits(3) == 3, "Corpse loot must encode the retail slot type");
    passed &= Expect(!packet.ReadBit(), "Loot item must not be marked tradeable to the tap list");
    passed &= Expect(packet.ReadBit(), "Loot item must omit the optional slot-type byte");
    passed &= Expect(!packet.ReadBit(), "Loot item must include its source slot");
    passed &= Expect(packet.ReadBits(2) == 3, "Loot item trailing bits must match retail");

    // A missing threshold reads as 0 on the client, which caps poor and common drops to a
    // single item and stops identical loot from merging.
    ByteBuffer header;
    Skyfire::Looting::WriteLootResponseMethodThresholdBits(&header);
    header.FlushBits();
    header.rpos(0);
    passed &= Expect(header.ReadBits(8) == 0, "Loot response must advertise FREE_FOR_ALL");
    passed &= Expect(header.ReadBits(2) == 2, "Loot response must advertise uncommon threshold");

    return passed ? 0 : 1;
}
