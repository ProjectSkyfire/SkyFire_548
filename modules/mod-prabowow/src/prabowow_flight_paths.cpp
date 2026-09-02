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
        if (!PraboWoW::GetBool("PraboWoW.AllFlightPaths.Enable", true))
            return;

        TaxiMask const& factionMask = player->GetTeam() == ALLIANCE ? sAllianceTaxiNodesMask : sHordeTaxiNodesMask;

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
            SF_LOG_DEBUG(PraboWoW::LOG, "[mod-prabowow] %s learned %u flight path(s) on login", player->GetName().c_str(), added);
    }
};

void AddSC_prabowow_flight_paths()
{
    new prabowow_flight_paths();
}
