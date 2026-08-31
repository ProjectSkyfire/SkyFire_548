/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Log.h"
#include "RealmList.h"

RealmList::RealmList() : m_UpdateInterval(0), m_NextUpdateTime(time(NULL)) { }

// Load the realm list from the database
void RealmList::Initialize(uint32 updateInterval)
{
    m_UpdateInterval = updateInterval;

    // Get the content of the realmlist table in the database
    UpdateRealms(true);
}

void RealmList::UpdateRealm(uint32 id, const std::string& name, Skyfire::Net::Address const& address,
    Skyfire::Net::Address const& localAddr, Skyfire::Net::Address const& localSubmask,
    uint8 icon, RealmFlags flag, uint8 timezone, AccountTypes allowedSecurityLevel,
    float popu, uint32 build)
{
    // Create new if not exist or update existed
    Realm& realm = m_realms[name];

    realm.m_ID = id;
    realm.name = name;
    realm.icon = icon;
    realm.flag = flag;
    realm.timezone = timezone;
    realm.allowedSecurityLevel = allowedSecurityLevel;
    realm.populationLevel = popu;

    // Append port to IP address.
    realm.ExternalAddress = address;
    realm.LocalAddress = localAddr;
    realm.LocalSubnetMask = localSubmask;
    realm.gamebuild = build;
}

void RealmList::UpdateIfNeed()
{
    // maybe disabled or updated recently
    if (!m_UpdateInterval || m_NextUpdateTime > time(NULL))
        return;

    m_NextUpdateTime = time(NULL) + m_UpdateInterval;

    // Clears Realm list
    m_realms.clear();

    // Get the content of the realmlist table in the database
    UpdateRealms();
}

void RealmList::UpdateRealms(bool init)
{
    SF_LOG_INFO("server.authserver", "Updating Realm List...");

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_AUTH_REALMLIST);
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    // Circle through results and add them to the realm map
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 realmId = fields[0].GetUInt32();
            std::string name = fields[1].GetString();
            std::string externalAddress = fields[2].GetString();
            std::string localAddress = fields[3].GetString();
            std::string localSubmask = fields[4].GetString();
            uint16 port = fields[5].GetUInt16();
            uint8 icon = fields[6].GetUInt8();
            RealmFlags flag = RealmFlags(fields[7].GetUInt8());
            uint8 timezone = fields[8].GetUInt8();
            AccountTypes allowedSecurityLevel = AccountTypes(fields[9].GetUInt8());
            float pop = fields[10].GetFloat();
            uint32 build = fields[11].GetUInt32();

            Skyfire::Net::Address externalAddr(externalAddress, port);
            Skyfire::Net::Address localAddr(localAddress, port);
            Skyfire::Net::Address submask(localSubmask);

            AccountTypes securityLevel = allowedSecurityLevel <= AccountTypes::SEC_ADMINISTRATOR ?
                AccountTypes(allowedSecurityLevel) : AccountTypes::SEC_ADMINISTRATOR;
            UpdateRealm(realmId, name, externalAddr, localAddr, submask, icon, flag, timezone,
                securityLevel, pop, build);

            if (init)
                SF_LOG_INFO("server.authserver", "Added realm \"%s\" at %s:%u.", name.c_str(),
                    m_realms[name].ExternalAddress.GetHost().c_str(), port);
        } while (result->NextRow());
    }
}

void RealmList::SetRealmOffline(uint32 realmId, bool offline)
{
    if (!realmId)
        return;

    uint32 const bit = REALM_FLAG_OFFLINE;
    for (RealmMap::iterator itr = m_realms.begin(); itr != m_realms.end(); ++itr)
    {
        if (itr->second.m_ID != realmId)
            continue;

        uint8 flags = uint8(itr->second.flag);
        uint8 const next = uint8(offline ? (flags | bit) : (flags & ~bit));
        if (next == flags)
            return;

        itr->second.flag = RealmFlags(next);
        LoginDatabase.DirectPExecute("UPDATE realmlist SET flag = (flag & ~%u) | %u WHERE id = %u",
            bit, offline ? bit : 0, realmId);
        if (!itr->first.empty())
            SF_LOG_INFO("server.authserver", "Realm \"%s\" (id=%u) %s (realmlist.flag=0x%02X).",
                itr->second.name.c_str(), realmId, offline ? "offline" : "online", next);
        return;
    }
}
