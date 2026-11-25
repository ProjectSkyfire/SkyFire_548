#include "UpdateFields.h"

#include <array>
#include <iostream>

int main()
{
    // Loading an empty skill list clears every packed skill slot. It must leave
    // the adjacent profession-slot counter and talent/tracking fields intact.
    std::array<unsigned, PLAYER_END> fields{};
    fields[PLAYER_FIELD_CHARACTER_POINTS] = 2;
    fields[PLAYER_FIELD_MAX_TALENT_TIERS] = 6;
    fields[PLAYER_FIELD_TRACK_RESOURCE_MASK] = 0x1234;
    unsigned const blocks[] = {
        PLAYER_FIELD_SKILL_LINEIDS, PLAYER_FIELD_SKILL_STEPS,
        PLAYER_FIELD_SKILL_RANKS, PLAYER_FIELD_SKILL_STARTING_RANKS,
        PLAYER_FIELD_SKILL_MAX_RANKS, PLAYER_FIELD_SKILL_MODIFIERS,
        PLAYER_FIELD_SKILL_TALENTS
    };

    for (unsigned block : blocks)
        for (unsigned field = 0; field < 64; ++field)
            fields[block + field] = 0;

    if (fields[PLAYER_FIELD_CHARACTER_POINTS] != 2 ||
        fields[PLAYER_FIELD_MAX_TALENT_TIERS] != 6 ||
        fields[PLAYER_FIELD_TRACK_RESOURCE_MASK] != 0x1234)
    {
        std::cerr << "Skill fields overlap profession slots, talent tiers or tracking fields\n";
        return 1;
    }

    for (unsigned i = 1; i < 7; ++i)
        if (blocks[i] != blocks[i - 1] + 64)
            return 1;

    return blocks[6] + 64 == PLAYER_FIELD_CHARACTER_POINTS ? 0 : 1;
}
