/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_REALMLIST_H
#define SF_REALMLIST_H

#include "Common.h"
#include "NetworkAddress.h"
#include "Platform/Singleton.h"

enum RealmFlags
{
    REALM_FLAG_NONE = 0x00,
    REALM_FLAG_INVALID = 0x01,
    REALM_FLAG_OFFLINE = 0x02,
    REALM_FLAG_SPECIFYBUILD = 0x04,
    REALM_FLAG_UNK1 = 0x08,
    REALM_FLAG_UNK2 = 0x10,
    REALM_FLAG_RECOMMENDED = 0x20,
    REALM_FLAG_NEW = 0x40,
    REALM_FLAG_FULL = 0x80
};

// Storage object for a realm
struct Realm
{
    Realm() : icon(0), flag(REALM_FLAG_NONE), timezone(0), m_ID(0),
        allowedSecurityLevel(AccountTypes::SEC_PLAYER), populationLevel(0.0f), gamebuild(0) {}
    Skyfire::Net::Address ExternalAddress;
    Skyfire::Net::Address LocalAddress;
    Skyfire::Net::Address LocalSubnetMask;
    std::string name;
    uint8 icon;
    RealmFlags flag;
    uint8 timezone;
    uint32 m_ID;
    AccountTypes allowedSecurityLevel;
    float populationLevel;
    uint32 gamebuild;
};

/// Storage object for the list of realms on the server
class RealmList
{
public:
    typedef std::map<std::string, Realm> RealmMap;

    RealmList();
    ~RealmList() { }

    void Initialize(uint32 updateInterval);

    void UpdateIfNeed();

    // Probe-driven: set or clear REALM_FLAG_OFFLINE in memory and auth.realmlist.
    void SetRealmOffline(uint32 realmId, bool offline);

    void AddRealm(const Realm& NewRealm) { m_realms[NewRealm.name] = NewRealm; }

    RealmMap::const_iterator begin() const { return m_realms.begin(); }
    RealmMap::const_iterator end() const { return m_realms.end(); }
    uint32 size() const { return m_realms.size(); }

private:
    void UpdateRealms(bool init = false);
    void UpdateRealm(uint32 id, const std::string& name, Skyfire::Net::Address const& address,
        Skyfire::Net::Address const& localAddr, Skyfire::Net::Address const& localSubmask,
        uint8 icon, RealmFlags flag, uint8 timezone, AccountTypes allowedSecurityLevel,
        float popu, uint32 build);

    RealmMap m_realms;
    uint32   m_UpdateInterval;
    time_t   m_NextUpdateTime;
};

#define sRealmList Skyfire::Singleton<RealmList, Skyfire::NullMutex>::instance()
#endif
