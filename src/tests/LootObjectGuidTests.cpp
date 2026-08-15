/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ObjectDefines.h"

#include <iostream>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    // Values taken from retail 5.4.8 sniffs, where SMSG_LOOT_RESPONSE pairs a creature
    // owner with a loot object of its own and the client echoes back the loot object.
    bool TestLootGuidMatchesRetailLayout()
    {
        bool passed = true;

        passed &= Expect(MAKE_NEW_GUID(0x19, 0, HIGHGUID_LOOT) == UI64LIT(0xF190000000000019),
            "Loot object 0x19 should encode as F190000000000019");
        passed &= Expect(MAKE_NEW_GUID(0x11C65, 0, HIGHGUID_LOOT) == UI64LIT(0xF190000000011C65),
            "Loot object 0x11C65 should encode as F190000000011C65");

        return passed;
    }

    bool TestLootGuidIsDistinguishable()
    {
        bool passed = true;

        uint64 lootGuid = MAKE_NEW_GUID(0x19, 0, HIGHGUID_LOOT);
        uint64 creatureGuid = MAKE_NEW_GUID(0x0B, 0xECA, HIGHGUID_UNIT);

        passed &= Expect(IS_LOOT_GUID(lootGuid), "A loot object GUID should be recognised as loot");
        passed &= Expect(!IS_LOOT_GUID(creatureGuid), "A creature GUID should not be recognised as loot");
        passed &= Expect(!IS_CREATURE_GUID(lootGuid), "A loot object GUID should not be recognised as a creature");
        passed &= Expect(!IsGuidHaveEnPart(lootGuid), "Loot object GUIDs carry no entry part");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestLootGuidMatchesRetailLayout();
    passed &= TestLootGuidIsDistinguishable();

    return passed ? 0 : 1;
}
