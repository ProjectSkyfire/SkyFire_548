/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BattlefieldMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Zones/BattlefieldWG.h"

BattlefieldMgr::~BattlefieldMgr()
{
    //SF_LOG_DEBUG("bg.battlefield", "Deleting BattlefieldMgr");
    for (BattlefieldSet::iterator itr = m_BattlefieldSet.begin(); itr != m_BattlefieldSet.end(); ++itr)
        delete* itr;
}

void BattlefieldMgr::InitBattlefield()
{
    Battlefield* pBf = new BattlefieldWG;
    // respawn, init variables
    if (!pBf->SetupBattlefield())
    {
        SF_LOG_INFO("misc", "Battlefield : Wintergrasp init failed.");
        delete pBf;
    }
    else
    {
        m_BattlefieldSet.push_back(pBf);
        SF_LOG_INFO("misc", "Battlefield : Wintergrasp successfully initiated.");
    }

    /* For Cataclysm: Tol Barad
       pBf = new BattlefieldTB;
       // respawn, init variables
       if (!pBf->SetupBattlefield())
       {
       SF_LOG_DEBUG("bg.battlefield", "Battlefield : Tol Barad init failed.");
       delete pBf;
       }
       else
       {
       m_BattlefieldSet.push_back(pBf);
       SF_LOG_DEBUG("bg.battlefield", "Battlefield : Tol Barad successfully initiated.");
       } */
}

void BattlefieldMgr::AddZone(uint32 zoneid, Battlefield* handle)
{
    m_BattlefieldMap[zoneid] = handle;
}

void BattlefieldMgr::HandlePlayerEnterZone(Player* player, uint32 zoneid)
{
    BattlefieldMap::iterator itr = m_BattlefieldMap.find(zoneid);
    if (itr == m_BattlefieldMap.end())
        return;

    Battlefield* bf = itr->second;
    if (bf->HasPlayer(player) || !bf->IsEnabled())
        return;

    bf->HandlePlayerEnterZone(player, zoneid);
    SF_LOG_DEBUG("bg.battlefield", "Player %u entered outdoorpvp id %u", player->GetGUIDLow(), bf->GetTypeId());
}

void BattlefieldMgr::HandlePlayerLeaveZone(Player* player, uint32 zoneid)
{
    BattlefieldMap::iterator itr = m_BattlefieldMap.find(zoneid);
    if (itr == m_BattlefieldMap.end())
        return;

    // teleport: remove once in removefromworld, once in updatezone
    if (!itr->second->HasPlayer(player))
        return;
    itr->second->HandlePlayerLeaveZone(player, zoneid);
    SF_LOG_DEBUG("bg.battlefield", "Player %u left outdoorpvp id %u", player->GetGUIDLow(), itr->second->GetTypeId());
}

Battlefield* BattlefieldMgr::GetBattlefieldToZoneId(uint32 zoneid)
{
    BattlefieldMap::iterator itr = m_BattlefieldMap.find(zoneid);
    if (itr == m_BattlefieldMap.end())
    {
        // no handle for this zone, return
        return NULL;
    }
    if (!itr->second->IsEnabled())
        return NULL;
    return itr->second;
}

Battlefield* BattlefieldMgr::GetBattlefieldByBattleId(uint32 battleid)
{
    for (BattlefieldSet::iterator itr = m_BattlefieldSet.begin(); itr != m_BattlefieldSet.end(); ++itr)
    {
        if ((*itr)->GetBattleId() == battleid)
            return (*itr);
    }
    return NULL;
}

Battlefield* BattlefieldMgr::GetBattlefieldByGUID(uint64 guid)
{
    for (BattlefieldSet::iterator itr = m_BattlefieldSet.begin(); itr != m_BattlefieldSet.end(); ++itr)
        if ((*itr)->GetGUID() == guid)
            return (*itr);

    return NULL;
}

void BattlefieldMgr::Update(uint32 diff)
{
    m_UpdateTimer += diff;
    if (m_UpdateTimer > BATTLEFIELD_OBJECTIVE_UPDATE_INTERVAL)
    {
        for (BattlefieldSet::iterator itr = m_BattlefieldSet.begin(); itr != m_BattlefieldSet.end(); ++itr)
            if ((*itr)->IsEnabled())
                (*itr)->Update(m_UpdateTimer);
        m_UpdateTimer = 0;
    }
}

ZoneScript* BattlefieldMgr::GetZoneScript(uint32 zoneId)
{
    BattlefieldMap::iterator itr = m_BattlefieldMap.find(zoneId);
    if (itr != m_BattlefieldMap.end())
        return itr->second;
    else
        return NULL;
}
