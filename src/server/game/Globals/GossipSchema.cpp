/*
 * This file is part of Project SkyFire https://www.projectskyfire.org.
 * See LICENSE.md file for Copyright information
 */

#include "GossipSchema.h"

namespace SkyFire::Gossip
{
char const* GetGossipMenuOptionLoadQuery()
{
    return
        //      0       1         2           3           4                      5           6              7             8            9         10        11       12
        "SELECT MenuID, OptionID, OptionIcon, OptionText, OptionBroadcastTextID, OptionType, OptionNpcflag, ActionMenuID, ActionPoiID, BoxCoded, BoxMoney, BoxText, BoxBroadcastTextID, TrainerSkill "
        "FROM gossip_menu_option ORDER BY MenuID, OptionID";
}

char const* GetGossipMenuOptionLocaleLoadQuery()
{
    return
        //      0       1         2       3           4
        "SELECT MenuID, OptionID, Locale, OptionText, BoxText FROM gossip_menu_option_locale";
}
}
