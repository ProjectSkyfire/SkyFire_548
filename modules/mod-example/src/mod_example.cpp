/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* Reference module demonstrating the SkyFire module system.
* It simply logs a line whenever a player logs in.
*/

#include "Config.h"
#include "Log.h"
#include "Player.h"
#include "ScriptMgr.h"

class mod_example_playerscript : public PlayerScript
{
public:
    mod_example_playerscript() : PlayerScript("mod_example_playerscript") { }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        if (!sConfigMgr->GetBoolDefault("Example.Enable", true))
            return;

        SF_LOG_INFO("modules", "[mod-example] Player '%s' logged in.",
            player ? player->GetName().c_str() : "<unknown>");
    }
};

void AddSC_mod_example()
{
    new mod_example_playerscript();
}
