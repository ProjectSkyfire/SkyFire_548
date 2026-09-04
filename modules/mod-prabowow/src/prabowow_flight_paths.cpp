/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* mod-prabowow: every flight path known on login.
*
* Marks all taxi nodes of the player's faction (sAllianceTaxiNodesMask /
* sHordeTaxiNodesMask, built from TaxiNodes.dbc) as discovered. Idempotent,
* so it runs on every login and covers characters created before the module.
*/

#include "PraboWoWConfig.h"

#include "DBCStores.h"
#include "DBCStructure.h"
#include "Log.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"

class prabowow_flight_paths : public PlayerScript
{
public:
    prabowow_flight_paths() : PlayerScript("prabowow_flight_paths") { }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        GrantFactionFlightPaths(player);
    }

    // A Pandaren picks their faction mid-session: WorldSession::HandleSelectFactionOpcode
    // calls setFactionForRace and teleports them to Stormwind or Orgrimmar without a
    // relog, so OnLogin never runs again with the new team. The teleport does change
    // their zone, which is the first chance to hand them the right paths.
    void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 /*newArea*/) override
    {
        // The faction choice is the only way a team changes without a relog, so keep
        // this to Pandaren rather than re-walking the mask for every zone change on
        // the realm.
        uint8 const race = player->getRace();
        if (race != RACE_PANDAREN_ALLIANCE && race != RACE_PANDAREN_HORDE)
            return;

        GrantFactionFlightPaths(player);
    }

private:
    static void GrantFactionFlightPaths(Player* player)
    {
        if (!PraboWoW::GetBool("PraboWoW.AllFlightPaths.Enable", true))
            return;

        uint32 const team = player->GetTeam();

        // Player::TeamForRace maps RACE_PANDAREN_NEUTRAL to PANDAREN_NEUTRAL (1249),
        // not to ALLIANCE or HORDE. A plain `team == ALLIANCE ? alliance : horde`
        // therefore handed neutral Pandaren the HORDE mask, and nothing corrected it
        // afterwards -- a Pandaren who then chose Alliance was left with Horde nodes
        // and none of their own. Wait until they have actually picked a side.
        if (team != ALLIANCE && team != HORDE)
            return;

        TaxiMask const& factionMask = team == ALLIANCE ? sAllianceTaxiNodesMask : sHordeTaxiNodesMask;

        uint32 added = 0;
        for (uint32 node = 1; node <= uint32(TaxiMaskSize) * 8; ++node)
        {
            uint8 field = uint8((node - 1) / 8);
            uint8 submask = uint8(1 << ((node - 1) % 8));
            if (!(factionMask[field] & submask))
                continue;

            if (player->m_taxi.SetTaximaskNode(node))
                ++added;
        }

        if (added)
            SF_LOG_DEBUG(PraboWoW::LOG, "[mod-prabowow] %s learned %u flight path(s)", player->GetName().c_str(), added);
    }
};

void AddSC_prabowow_flight_paths()
{
    new prabowow_flight_paths();
}
