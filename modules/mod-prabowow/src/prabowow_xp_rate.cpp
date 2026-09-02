/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* mod-prabowow: per-character XP rate.
*
*   .xp            show the current multiplier
*   .xp rate <n>   pick a multiplier between 1 and PraboWoW.XpRate.Max
*
* The multiplier is applied in OnGiveXP, i.e. on top of Rate.XP.Kill /
* Rate.XP.Quest / Rate.XP.Explore, and persisted in characters.character_xp_rate.
*/

#include "PraboWoWConfig.h"

#include "Chat.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "WorldSession.h"

#include <algorithm>
#include <cstdlib>
#include <limits>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>

namespace
{
    // Map updates run on several threads, so the cache is guarded.
    std::shared_mutex g_lock;
    std::unordered_map<uint32, uint8> g_rates; // character low guid -> multiplier

    uint8 MaxRate()
    {
        return uint8(std::clamp<int32>(PraboWoW::GetInt("PraboWoW.XpRate.Max", 5), 1, 100));
    }

    uint8 GetRate(uint32 guidLow)
    {
        std::shared_lock<std::shared_mutex> lock(g_lock);
        auto itr = g_rates.find(guidLow);
        return itr != g_rates.end() ? itr->second : 1;
    }

    void SetRate(uint32 guidLow, uint8 rate)
    {
        std::unique_lock<std::shared_mutex> lock(g_lock);
        g_rates[guidLow] = rate;
    }

    void ForgetRate(uint32 guidLow)
    {
        std::unique_lock<std::shared_mutex> lock(g_lock);
        g_rates.erase(guidLow);
    }

    void SendStatus(ChatHandler* handler, uint32 guidLow)
    {
        uint32 max = MaxRate();
        handler->PSendSysMessage("|cff00ff00[PraboWoW]|r Your XP rate is |cffffff00%ux|r (max %ux). Usage: .xp rate <1-%u>",
            uint32(GetRate(guidLow)), max, max);
    }
}

class prabowow_xp_rate_player : public PlayerScript
{
public:
    prabowow_xp_rate_player() : PlayerScript("prabowow_xp_rate_player") { }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        uint32 guidLow = player->GetGUIDLow();
        uint8 rate = 1;

        if (QueryResult result = CharacterDatabase.PQuery("SELECT rate FROM character_xp_rate WHERE guid = %u", guidLow))
            rate = result->Fetch()[0].GetUInt8();

        rate = uint8(std::clamp<uint32>(rate, 1, MaxRate()));
        SetRate(guidLow, rate);

        if (rate > 1)
            ChatHandler(player->GetSession()).PSendSysMessage("|cff00ff00[PraboWoW]|r XP rate |cffffff00%ux|r active. Change it with .xp rate <1-%u>",
                uint32(rate), uint32(MaxRate()));
    }

    void OnLogout(Player* player) override
    {
        ForgetRate(player->GetGUIDLow());
    }

    void OnDelete(uint64 guid) override
    {
        uint32 guidLow = GUID_LOPART(guid);
        ForgetRate(guidLow);
        CharacterDatabase.PExecute("DELETE FROM character_xp_rate WHERE guid = %u", guidLow);
    }

    void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/) override
    {
        uint8 rate = GetRate(player->GetGUIDLow());
        if (rate <= 1)
            return;

        uint64 scaled = uint64(amount) * rate;
        amount = scaled > std::numeric_limits<uint32>::max() ? std::numeric_limits<uint32>::max() : uint32(scaled);
    }
};

class prabowow_xp_commandscript : public CommandScript
{
public:
    prabowow_xp_commandscript() : CommandScript("prabowow_xp_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> xpCommandTable =
        {
            { "rate", PraboWoW::RBAC_PERM_COMMAND_XP_RATE, false, &HandleXpRateCommand, "" },
            { "",     PraboWoW::RBAC_PERM_COMMAND_XP,      false, &HandleXpCommand,     "" },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "xp", PraboWoW::RBAC_PERM_COMMAND_XP, false, NULL, "", xpCommandTable },
        };
        return commandTable;
    }

    static bool HandleXpCommand(ChatHandler* handler, char const* /*args*/)
    {
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : NULL;
        if (!player)
            return false;

        SendStatus(handler, player->GetGUIDLow());
        return true;
    }

    static bool HandleXpRateCommand(ChatHandler* handler, char const* args)
    {
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : NULL;
        if (!player)
            return false;

        uint32 guidLow = player->GetGUIDLow();
        if (!*args)
        {
            SendStatus(handler, guidLow);
            return true;
        }

        int32 wanted = atoi(args);
        uint32 max = MaxRate();
        if (wanted < 1 || uint32(wanted) > max)
        {
            handler->PSendSysMessage("|cff00ff00[PraboWoW]|r XP rate must be between 1 and %u.", max);
            handler->SetSentErrorMessage(true);
            return false;
        }

        SetRate(guidLow, uint8(wanted));
        CharacterDatabase.PExecute("REPLACE INTO character_xp_rate (guid, rate) VALUES (%u, %u)", guidLow, uint32(wanted));

        handler->PSendSysMessage("|cff00ff00[PraboWoW]|r XP rate set to |cffffff00%ux|r.", uint32(wanted));
        SF_LOG_INFO(PraboWoW::LOG, "[mod-prabowow] %s (guid %u) set XP rate to %ux", player->GetName().c_str(), guidLow, uint32(wanted));
        return true;
    }
};

void AddSC_prabowow_xp_rate()
{
    new prabowow_xp_rate_player();
    new prabowow_xp_commandscript();
}
