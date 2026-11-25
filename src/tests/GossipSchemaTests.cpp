#include "GossipSchema.h"

#include <iostream>
#include <string>

namespace
{
bool Contains(std::string const& haystack, char const* needle)
{
    return haystack.find(needle) != std::string::npos;
}

bool ExpectContains(std::string const& query, char const* expected)
{
    if (Contains(query, expected))
        return true;

    std::cerr << "Expected gossip query to contain: " << expected << '\n';
    return false;
}

bool ExpectNotContains(std::string const& query, char const* unexpected)
{
    if (!Contains(query, unexpected))
        return true;

    std::cerr << "Expected gossip query to omit: " << unexpected << '\n';
    return false;
}
}

int main()
{
    std::string const query = SkyFire::Gossip::GetGossipMenuOptionLoadQuery();
    std::string const localeQuery = SkyFire::Gossip::GetGossipMenuOptionLocaleLoadQuery();

    bool passed = true;
    passed &= ExpectContains(query, "MenuID");
    passed &= ExpectContains(query, "OptionID");
    passed &= ExpectContains(query, "OptionBroadcastTextID");
    passed &= ExpectContains(query, "ActionMenuID");
    passed &= ExpectContains(query, "BoxBroadcastTextID");
    passed &= ExpectContains(query, "BoxBroadcastTextID, TrainerSkill ");
    passed &= ExpectContains(query, "FROM gossip_menu_option");
    passed &= ExpectNotContains(query, "gossip_menu_option_action");
    passed &= ExpectNotContains(query, "gossip_menu_option_box");
    passed &= ExpectContains(localeQuery, "MenuID");
    passed &= ExpectContains(localeQuery, "OptionID");
    passed &= ExpectContains(localeQuery, "Locale");
    passed &= ExpectContains(localeQuery, "OptionText");
    passed &= ExpectContains(localeQuery, "BoxText");
    passed &= ExpectContains(localeQuery, "FROM gossip_menu_option_locale");
    passed &= ExpectNotContains(localeQuery, "locales_gossip_menu_option");

    return passed ? 0 : 1;
}
