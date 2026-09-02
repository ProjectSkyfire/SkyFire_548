/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* mod-prabowow: shared helpers (config access, RBAC ids, log filter).
*
* The core does not load module .conf files on its own, so every key below is
* read from worldserver.conf. conf/prabowow.conf.dist documents them.
*/

#ifndef PRABOWOW_CONFIG_H
#define PRABOWOW_CONFIG_H

#include "Config.h"
#include "Define.h"

#include <string>

namespace PraboWoW
{
    // Log filter used by every script of the module (falls back to Logger.root).
    static char const* const LOG = "modules";

    // RBAC permission ids created by sql/updates/auth/*prabowow_player_commands*.sql
    // and linked to role 199 ("Role: Player Commands") so every account gets them.
    enum Permissions : uint32
    {
        RBAC_PERM_COMMAND_XP      = 1100,
        RBAC_PERM_COMMAND_XP_RATE = 1101,
        RBAC_PERM_COMMAND_CHAT    = 1102
    };

    inline bool GetBool(char const* key, bool def)
    {
        return sConfigMgr->GetBoolDefault(key, def);
    }

    inline int32 GetInt(char const* key, int32 def)
    {
        return sConfigMgr->GetIntDefault(key, def);
    }

    inline std::string GetString(char const* key, std::string const& def)
    {
        return sConfigMgr->GetStringDefault(key, def);
    }

    // "12g 34s 56c" style formatting for chat feedback.
    inline std::string FormatMoney(uint64 copper)
    {
        uint64 gold = copper / 10000;
        uint64 silver = (copper / 100) % 100;
        uint64 cop = copper % 100;

        std::string out;
        if (gold)
            out += std::to_string(gold) + "g ";
        if (gold || silver)
            out += std::to_string(silver) + "s ";
        out += std::to_string(cop) + "c";
        return out;
    }
}

#endif
