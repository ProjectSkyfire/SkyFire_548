/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/** \file
    \ingroup world
*/

#include "Common.h"
#include "Memory.h"
#include "DatabaseEnv.h"
#include "Config.h"
#include "SystemConfig.h"
#include "Log.h"
#include "Opcodes.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "Player.h"
#include "Vehicle.h"
#include "SkillExtraItems.h"
#include "SkillDiscovery.h"
#include "World.h"
#include "AccountMgr.h"
#include "AchievementMgr.h"
#include "AuctionHouseMgr.h"
#include "BlackMarketMgr.h"
#include "ObjectMgr.h"
#include "ArenaTeamMgr.h"
#include "GuildMgr.h"
#include "GuildFinderMgr.h"
#include "TicketMgr.h"
#include "SpellMgr.h"
#include "GroupMgr.h"
#include "Chat.h"
#include "DBCStores.h"
#include "DB2Stores.h"
#include "LootMgr.h"
#include "ItemEnchantmentMgr.h"
#include "MapManager.h"
#include "CreatureAIRegistry.h"
#include "BattlegroundMgr.h"
#include "OutdoorPvPMgr.h"
#include "TemporarySummon.h"
#include "WaypointMovementGenerator.h"
#include "VMapFactory.h"
#include "MMapFactory.h"
#include "GameEventMgr.h"
#include "PoolMgr.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "InstanceSaveMgr.h"
#include "Util.h"
#include "Language.h"
#include "CreatureGroups.h"
#include "Transport.h"
#include "ScriptMgr.h"
#include "AddonMgr.h"
#include "LFGMgr.h"
#include "ConditionMgr.h"
#include "DisableMgr.h"
#include "CharacterDatabaseCleaner.h"
#include "ScriptMgr.h"
#include "WeatherMgr.h"
#include "CreatureTextMgr.h"
#include "SmartAI.h"
#include "Channel.h"
#include "WardenCheckMgr.h"
#include "Warden.h"
#include "CalendarMgr.h"
#include "BattlefieldMgr.h"
#include "TransportMgr.h"
#include "BattlePetMgr.h"

ACE_Atomic_Op<ACE_Thread_Mutex, bool> World::m_stopEvent = false;
uint8 World::m_ExitCode = SHUTDOWN_EXIT_CODE;
ACE_Atomic_Op<ACE_Thread_Mutex, uint32> World::m_worldLoopCounter = 0;

float World::m_MaxVisibleDistanceOnContinents = DEFAULT_VISIBILITY_DISTANCE;
float World::m_MaxVisibleDistanceInInstances  = DEFAULT_VISIBILITY_INSTANCE;
float World::m_MaxVisibleDistanceInBGArenas   = DEFAULT_VISIBILITY_BGARENAS;

int32 World::m_visibility_notify_periodOnContinents = DEFAULT_VISIBILITY_NOTIFY_PERIOD;
int32 World::m_visibility_notify_periodInInstances  = DEFAULT_VISIBILITY_NOTIFY_PERIOD;
int32 World::m_visibility_notify_periodInBGArenas   = DEFAULT_VISIBILITY_NOTIFY_PERIOD;

/// World constructor
World::World()
{
    m_playerLimit = 0;
    m_allowedSecurityLevel = AccountTypes::SEC_PLAYER;
    m_allowMovement = true;
    m_ShutdownMask = 0;
    m_ShutdownTimer = 0;
    m_gameTime = time(NULL);
    m_startTime = m_gameTime;
    m_maxActiveSessionCount = 0;
    m_maxQueuedSessionCount = 0;
    m_PlayerCount = 0;
    m_MaxPlayerCount = 0;
    m_NextDailyQuestReset = 0;
    m_NextWeeklyQuestReset = 0;
    m_NextCurrencyReset = 0;

    m_defaultDbcLocale = LOCALE_enUS;
    m_availableDbcLocaleMask = 0;

    m_updateTimeSum = 0;
    m_updateTimeCount = 0;

    m_isClosed = false;

    m_CleaningFlags = 0;
}

/// World destructor
World::~World()
{
    ///- Empty the kicked session set
    while (!m_sessions.empty())
    {
        // not remove from queue, prevent loading new sessions
        delete m_sessions.begin()->second;
        m_sessions.erase(m_sessions.begin());
    }

    CliCommandHolder* command = NULL;
    while (cliCmdQueue.next(command))
        delete command;

    VMAP::VMapFactory::clear();
    MMAP::MMapFactory::clear();

    /// @todo free addSessQueue
}

/// Find a player in a specified zone
Player* World::FindPlayerInZone(uint32 zone)
{
    ///- circle through active sessions and return the first player found in the zone
    SessionMap::const_iterator itr;
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (!itr->second)
            continue;

        Player* player = itr->second->GetPlayer();
        if (!player)
            continue;

        if (player->IsInWorld() && player->GetZoneId() == zone)
        {
            // Used by the weather system. We return the player to broadcast the change weather message to him and all players in the zone.
            return player;
        }
    }
    return NULL;
}

bool World::IsClosed() const
{
    return m_isClosed;
}

void World::SetClosed(bool val)
{
    m_isClosed = val;

    // Invert the value, for simplicity for scripters.
    sScriptMgr->OnOpenStateChange(!val);
}

void World::SetMotd(const std::string& motd)
{
    m_motd = motd;

    sScriptMgr->OnMotdChange(m_motd);
}

const char* World::GetMotd() const
{
    return m_motd.c_str();
}

/// Find a session by its id
WorldSession* World::FindSession(uint32 id) const
{
    SessionMap::const_iterator itr = m_sessions.find(id);

    if (itr != m_sessions.end())
        return itr->second;                                 // also can return NULL for kicked session
    else
        return NULL;
}

/// Remove a given session
bool World::RemoveSession(uint32 id)
{
    ///- Find the session, kick the user, but we can't delete session at this moment to prevent iterator invalidation
    SessionMap::const_iterator itr = m_sessions.find(id);

    if (itr != m_sessions.end() && itr->second)
    {
        if (itr->second->PlayerLoading())
            return false;

        itr->second->KickPlayer();
    }

    return true;
}

void World::AddSession(WorldSession* s)
{
    addSessQueue.add(s);
}

void World::AddSession_(WorldSession* s)
{
    ASSERT(s);

    //NOTE - Still there is race condition in WorldSession* being used in the Sockets

    ///- kick already loaded player with same account (if any) and remove session
    ///- if player is in loading and want to load again, return
    if (!RemoveSession (s->GetAccountId()))
    {
        s->KickPlayer();
        delete s;                                           // session not added yet in session list, so not listed in queue
        return;
    }

    // decrease session counts only at not reconnection case
    bool decrease_session = true;

    // if session already exist, prepare to it deleting at next world update
    // NOTE - KickPlayer() should be called on "old" in RemoveSession()
    {
        SessionMap::const_iterator old = m_sessions.find(s->GetAccountId());

        if (old != m_sessions.end())
        {
            // prevent decrease sessions count if session queued
            if (RemoveQueuedPlayer(old->second))
                decrease_session = false;
            // not remove replaced session form queue if listed
            delete old->second;
        }
    }

    m_sessions[s->GetAccountId()] = s;

    uint32 Sessions = GetActiveAndQueuedSessionCount();
    uint32 pLimit = GetPlayerAmountLimit();
    uint32 QueueSize = GetQueuedSessionCount(); //number of players in the queue

    //so we don't count the user trying to
    //login as a session and queue the socket that we are using
    if (decrease_session)
        --Sessions;

    if (pLimit > 0 && Sessions >= pLimit && !s->HasPermission(rbac::RBAC_PERM_SKIP_QUEUE) && !HasRecentlyDisconnected(s))
    {
        AddQueuedPlayer(s);
        UpdateMaxSessionCounters();
        SF_LOG_INFO("misc", "PlayerQueue: Account id %u is in Queue Position (%u).", s->GetAccountId(), ++QueueSize);
        return;
    }

    s->SendAuthResponse(ResponseCodes::AUTH_OK, false);
    s->SendAddonsInfo();
    s->SendClientCacheVersion(sWorld->getIntConfig(WorldIntConfigs::CONFIG_CLIENTCACHE_VERSION));
    if (s->HasBoost())
        s->SendBattlePayDistributionUpdate(0, CHARACTER_BOOST, CHARACTER_BOOST_ALLOW, CHARACTER_BOOST_TEXT_ID, CHARACTER_BOOST_BONUS_TEXT, CHARACTER_BOOST_BONUS_TEXT2);
    s->SendTutorialsData();
    s->SendTimezoneInformation();

    UpdateMaxSessionCounters();

    // Updates the population
    if (pLimit > 0)
    {
        float popu = (float)GetActiveSessionCount();              // updated number of users on the server
        popu /= pLimit;
        popu *= 2;
        SF_LOG_INFO("misc", "Server Population (%f).", popu);
    }
}

bool World::HasRecentlyDisconnected(WorldSession* session)
{
    if (!session)
        return false;

    if (uint32 tolerance = getIntConfig(WorldIntConfigs::CONFIG_INTERVAL_DISCONNECT_TOLERANCE))
    {
        for (DisconnectMap::iterator i = m_disconnects.begin(); i != m_disconnects.end();)
        {
            if (difftime(i->second, time(NULL)) < tolerance)
            {
                if (i->first == session->GetAccountId())
                    return true;
                ++i;
            }
            else
                m_disconnects.erase(i++);
        }
    }
    return false;
}

int32 World::GetQueuePos(WorldSession* sess)
{
    uint32 position = 1;

    for (Queue::const_iterator iter = m_QueuedPlayer.begin(); iter != m_QueuedPlayer.end(); ++iter, ++position)
        if ((*iter) == sess)
            return position;

    return 0;
}

void World::AddQueuedPlayer(WorldSession* sess)
{
    sess->SetInQueue(true);
    m_QueuedPlayer.push_back(sess);

    // The 1st SMSG_AUTH_RESPONSE needs to contain other info too.
    sess->SendAuthResponse(ResponseCodes::AUTH_WAIT_QUEUE, true, GetQueuePos(sess));
}

bool World::RemoveQueuedPlayer(WorldSession* sess)
{
    // sessions count including queued to remove (if removed_session set)
    uint32 sessions = GetActiveSessionCount();

    uint32 position = 1;
    Queue::iterator iter = m_QueuedPlayer.begin();

    // search to remove and count skipped positions
    bool found = false;

    for (; iter != m_QueuedPlayer.end(); ++iter, ++position)
    {
        if (*iter == sess)
        {
            sess->SetInQueue(false);
            sess->ResetTimeOutTime();
            iter = m_QueuedPlayer.erase(iter);
            found = true;                                   // removing queued session
            break;
        }
    }

    // iter point to next socked after removed or end()
    // position store position of removed socket and then new position next socket after removed

    // if session not queued then we need decrease sessions count
    if (!found && sessions)
        --sessions;

    // accept first in queue
    if ((!m_playerLimit || sessions < m_playerLimit) && !m_QueuedPlayer.empty())
    {
        WorldSession* pop_sess = m_QueuedPlayer.front();
        pop_sess->SetInQueue(false);
        pop_sess->ResetTimeOutTime();
        pop_sess->SendAuthWaitQue(0);
        pop_sess->SendAddonsInfo();

        pop_sess->SendClientCacheVersion(sWorld->getIntConfig(WorldIntConfigs::CONFIG_CLIENTCACHE_VERSION));
        pop_sess->SendAccountDataTimes(GLOBAL_CACHE_MASK);
        pop_sess->SendTutorialsData();
        pop_sess->SendTimezoneInformation();

        m_QueuedPlayer.pop_front();

        // update iter to point first queued socket or end() if queue is empty now
        iter = m_QueuedPlayer.begin();
        position = 1;
    }

    // update position from iter to end()
    // iter point to first not updated socket, position store new position
    for (; iter != m_QueuedPlayer.end(); ++iter, ++position)
        (*iter)->SendAuthWaitQue(position);

    return found;
}

/// Initialize config values
void World::LoadConfigSettings(bool reload)
{
    if (reload)
    {
        if (!sConfigMgr->Reload())
        {
            SF_LOG_ERROR("misc", "World settings reload fail: can't read settings from %s.", sConfigMgr->GetFilename().c_str());
            return;
        }
        sLog->LoadFromConfig();
    }

    m_defaultDbcLocale = LocaleConstant(sConfigMgr->GetIntDefault("DBC.Locale", 0));

    if (m_defaultDbcLocale >= TOTAL_LOCALES)
    {
        SF_LOG_ERROR("server.loading", "Incorrect DBC.Locale! Must be >= 0 and < %d (set to 0)", TOTAL_LOCALES);
        m_defaultDbcLocale = LOCALE_enUS;
    }

    SF_LOG_INFO("server.loading", "Using %s DBC Locale", localeNames[m_defaultDbcLocale]);

    ///- Read the player limit and the Message of the day from the config file
    SetPlayerAmountLimit(sConfigMgr->GetIntDefault("PlayerLimit", 100));
    SetMotd(sConfigMgr->GetStringDefault("Motd", "Welcome to a Skyfire Core Server."));

    ///- Read ticket system setting from the config file
    SetBoolConfig(WorldBoolConfigs::CONFIG_TICKETS_FEEDBACK_SYSTEM_ENABLED, sConfigMgr->GetBoolDefault("TicketSystem.FeedBackTickets", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_TICKETS_GM_ENABLED,              sConfigMgr->GetBoolDefault("TicketSystem.GMTickets", true));
    if (reload)
    {
        sTicketMgr->SetFeedBackSystemStatus(GetBoolConfig(WorldBoolConfigs::CONFIG_TICKETS_FEEDBACK_SYSTEM_ENABLED));
        sTicketMgr->SetGmTicketSystemStatus(GetBoolConfig(WorldBoolConfigs::CONFIG_TICKETS_GM_ENABLED));
    }

    ///- Get string for new logins (newly created characters)
    SetNewCharString(sConfigMgr->GetStringDefault("PlayerStart.String", ""));

    ///- Send server info on login?
    setIntConfig(WorldIntConfigs::CONFIG_ENABLE_SINFO_LOGIN, sConfigMgr->GetIntDefault("Server.LoginInfo", 0));

    ///- Read all rates from the config file
    setRate(Rates::RATE_HEALTH, sConfigMgr->GetFloatDefault("Rate.Health", 1));
    if (getRate(Rates::RATE_HEALTH) < 0)
    {
        SF_LOG_ERROR("server.loading", "Rate.Health (%f) must be > 0. Using 1 instead.", getRate(Rates::RATE_HEALTH));
        setRate(Rates::RATE_HEALTH, 1);
    }
    setRate(Rates::RATE_POWER_MANA, sConfigMgr->GetFloatDefault("Rate.Mana", 1));
    if (getRate(Rates::RATE_POWER_MANA) < 0)
    {
        SF_LOG_ERROR("server.loading", "Rate.Mana (%f) must be > 0. Using 1 instead.", getRate(Rates::RATE_POWER_MANA));
        setRate(Rates::RATE_POWER_MANA, 1);
    }
    setRate(Rates::RATE_POWER_RAGE_INCOME, sConfigMgr->GetFloatDefault("Rate.Rage.Income", 1));
    setRate(Rates::RATE_POWER_RAGE_LOSS, sConfigMgr->GetFloatDefault("Rate.Rage.Loss", 1));
    if (getRate(Rates::RATE_POWER_RAGE_LOSS) < 0)
    {
        SF_LOG_ERROR("server.loading", "Rate.Rage.Loss (%f) must be > 0. Using 1 instead.", getRate(Rates::RATE_POWER_RAGE_LOSS));
        setRate(Rates::RATE_POWER_RAGE_LOSS, 1);
    }
    setRate(Rates::RATE_POWER_RUNICPOWER_INCOME, sConfigMgr->GetFloatDefault("Rate.RunicPower.Income", 1));
    setRate(Rates::RATE_POWER_RUNICPOWER_LOSS, sConfigMgr->GetFloatDefault("Rate.RunicPower.Loss", 1));
    if (getRate(Rates::RATE_POWER_RUNICPOWER_LOSS) < 0)
    {
        SF_LOG_ERROR("server.loading", "Rate.RunicPower.Loss (%f) must be > 0. Using 1 instead.", getRate(Rates::RATE_POWER_RUNICPOWER_LOSS));
        setRate(Rates::RATE_POWER_RUNICPOWER_LOSS, 1);
    }
    setRate(Rates::RATE_POWER_DEMONICFURY_LOSS, sConfigMgr->GetFloatDefault("Rate.DemonicFury.Loss", 1));
    if (getRate(Rates::RATE_POWER_DEMONICFURY_LOSS) < 0)
    {
        SF_LOG_ERROR("server.loading", "Rate.DemonicFury.Loss (%f) must be > 0. Using 1 instead.", getRate(Rates::RATE_POWER_DEMONICFURY_LOSS));
        setRate(Rates::RATE_POWER_DEMONICFURY_LOSS, 1);
    }
    setRate(Rates::RATE_POWER_FOCUS, sConfigMgr->GetFloatDefault("Rate.Focus", 1.0f));
    setRate(Rates::RATE_POWER_ENERGY, sConfigMgr->GetFloatDefault("Rate.Energy", 1.0f));
    setRate(Rates::RATE_POWER_CHI, sConfigMgr->GetFloatDefault("Rate.Chi", 1.0f));

    setRate(Rates::RATE_SKILL_DISCOVERY, sConfigMgr->GetFloatDefault("Rate.Skill.Discovery", 1.0f));

    setRate(Rates::RATE_DROP_ITEM_POOR, sConfigMgr->GetFloatDefault("Rate.Drop.Item.Poor", 1.0f));
    setRate(Rates::RATE_DROP_ITEM_NORMAL, sConfigMgr->GetFloatDefault("Rate.Drop.Item.Normal", 1.0f));
    setRate(Rates::RATE_DROP_ITEM_UNCOMMON, sConfigMgr->GetFloatDefault("Rate.Drop.Item.Uncommon", 1.0f));
    setRate(Rates::RATE_DROP_ITEM_RARE, sConfigMgr->GetFloatDefault("Rate.Drop.Item.Rare", 1.0f));
    setRate(Rates::RATE_DROP_ITEM_EPIC, sConfigMgr->GetFloatDefault("Rate.Drop.Item.Epic", 1.0f));
    setRate(Rates::RATE_DROP_ITEM_LEGENDARY, sConfigMgr->GetFloatDefault("Rate.Drop.Item.Legendary", 1.0f));
    setRate(Rates::RATE_DROP_ITEM_ARTIFACT, sConfigMgr->GetFloatDefault("Rate.Drop.Item.Artifact", 1.0f));
    setRate(Rates::RATE_DROP_ITEM_REFERENCED, sConfigMgr->GetFloatDefault("Rate.Drop.Item.Referenced", 1.0f));
    setRate(Rates::RATE_DROP_ITEM_REFERENCED_AMOUNT, sConfigMgr->GetFloatDefault("Rate.Drop.Item.ReferencedAmount", 1.0f));
    setRate(Rates::RATE_DROP_MONEY, sConfigMgr->GetFloatDefault("Rate.Drop.Money", 1.0f));
    setRate(Rates::RATE_XP_KILL, sConfigMgr->GetFloatDefault("Rate.XP.Kill", 1.0f));
    setRate(Rates::RATE_XP_QUEST, sConfigMgr->GetFloatDefault("Rate.XP.Quest", 1.0f));
    setRate(Rates::RATE_XP_EXPLORE, sConfigMgr->GetFloatDefault("Rate.XP.Explore", 1.0f));
    setRate(Rates::RATE_REPAIRCOST, sConfigMgr->GetFloatDefault("Rate.RepairCost", 1.0f));
    if (getRate(Rates::RATE_REPAIRCOST) < 0.0f)
    {
        SF_LOG_ERROR("server.loading", "Rate.RepairCost (%f) must be >=0. Using 0.0 instead.", getRate(Rates::RATE_REPAIRCOST));
        setRate(Rates::RATE_REPAIRCOST, 0.0f);
    }
    setRate(Rates::RATE_REPUTATION_GAIN, sConfigMgr->GetFloatDefault("Rate.Reputation.Gain", 1.0f));
    setRate(Rates::RATE_REPUTATION_LFG_BONUS, sConfigMgr->GetFloatDefault("Rate.Reputation.LFGBonus", 1.0f));
    setRate(Rates::RATE_REPUTATION_LOWLEVEL_KILL, sConfigMgr->GetFloatDefault("Rate.Reputation.LowLevel.Kill", 1.0f));
    setRate(Rates::RATE_REPUTATION_LOWLEVEL_QUEST, sConfigMgr->GetFloatDefault("Rate.Reputation.LowLevel.Quest", 1.0f));
    setRate(Rates::RATE_REPUTATION_RECRUIT_A_FRIEND_BONUS, sConfigMgr->GetFloatDefault("Rate.Reputation.RecruitAFriendBonus", 0.1f));
    setRate(Rates::RATE_CREATURE_NORMAL_DAMAGE, sConfigMgr->GetFloatDefault("Rate.Creature.Normal.Damage", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_ELITE_DAMAGE, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.Elite.Damage", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_RAREELITE_DAMAGE, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.RAREELITE.Damage", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_WORLDBOSS_DAMAGE, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.WORLDBOSS.Damage", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_RARE_DAMAGE, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.RARE.Damage", 1.0f));
    setRate(Rates::RATE_CREATURE_NORMAL_HP, sConfigMgr->GetFloatDefault("Rate.Creature.Normal.HP", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_ELITE_HP, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.Elite.HP", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_RAREELITE_HP, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.RAREELITE.HP", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_WORLDBOSS_HP, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.WORLDBOSS.HP", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_RARE_HP, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.RARE.HP", 1.0f));
    setRate(Rates::RATE_CREATURE_NORMAL_SPELLDAMAGE, sConfigMgr->GetFloatDefault("Rate.Creature.Normal.SpellDamage", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_ELITE_SPELLDAMAGE, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.Elite.SpellDamage", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_RAREELITE_SPELLDAMAGE, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.RAREELITE.SpellDamage", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_WORLDBOSS_SPELLDAMAGE, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.WORLDBOSS.SpellDamage", 1.0f));
    setRate(Rates::RATE_CREATURE_ELITE_RARE_SPELLDAMAGE, sConfigMgr->GetFloatDefault("Rate.Creature.Elite.RARE.SpellDamage", 1.0f));
    setRate(Rates::RATE_CREATURE_AGGRO, sConfigMgr->GetFloatDefault("Rate.Creature.Aggro", 1.0f));
    setRate(Rates::RATE_REST_INGAME, sConfigMgr->GetFloatDefault("Rate.Rest.InGame", 1.0f));
    setRate(Rates::RATE_REST_OFFLINE_IN_TAVERN_OR_CITY, sConfigMgr->GetFloatDefault("Rate.Rest.Offline.InTavernOrCity", 1.0f));
    setRate(Rates::RATE_REST_OFFLINE_IN_WILDERNESS, sConfigMgr->GetFloatDefault("Rate.Rest.Offline.InWilderness", 1.0f));
    setRate(Rates::RATE_DAMAGE_FALL, sConfigMgr->GetFloatDefault("Rate.Damage.Fall", 1.0f));
    setRate(Rates::RATE_AUCTION_TIME, sConfigMgr->GetFloatDefault("Rate.Auction.Time", 1.0f));
    setRate(Rates::RATE_AUCTION_DEPOSIT, sConfigMgr->GetFloatDefault("Rate.Auction.Deposit", 1.0f));
    setRate(Rates::RATE_AUCTION_CUT, sConfigMgr->GetFloatDefault("Rate.Auction.Cut", 1.0f));
    setRate(Rates::RATE_HONOR, sConfigMgr->GetFloatDefault("Rate.Honor", 1.0f));
    setRate(Rates::RATE_INSTANCE_RESET_TIME, sConfigMgr->GetFloatDefault("Rate.InstanceResetTime", 1.0f));
    setRate(Rates::RATE_MOVESPEED, sConfigMgr->GetFloatDefault("Rate.MoveSpeed", 1.0f));
    if (getRate(Rates::RATE_MOVESPEED) < 0)
    {
        SF_LOG_ERROR("server.loading", "Rate.MoveSpeed (%f) must be > 0. Using 1 instead.", getRate(Rates::RATE_MOVESPEED));
        setRate(Rates::RATE_MOVESPEED, 1.0f);
    }
    for (uint8 i = 0; i < MAX_MOVE_TYPE; ++i) playerBaseMoveSpeed[i] = baseMoveSpeed[i] * getRate(Rates::RATE_MOVESPEED);

    setRate(Rates::RATE_CORPSE_DECAY_LOOTED, sConfigMgr->GetFloatDefault("Rate.Corpse.Decay.Looted", 0.5f));

    setRate(Rates::RATE_TARGET_POS_RECALCULATION_RANGE, sConfigMgr->GetFloatDefault("TargetPosRecalculateRange", 1.5f));
    if (getRate(Rates::RATE_TARGET_POS_RECALCULATION_RANGE) < CONTACT_DISTANCE)
    {
        SF_LOG_ERROR("server.loading", "TargetPosRecalculateRange (%f) must be >= %f. Using %f instead.", getRate(Rates::RATE_TARGET_POS_RECALCULATION_RANGE), CONTACT_DISTANCE, CONTACT_DISTANCE);
        setRate(Rates::RATE_TARGET_POS_RECALCULATION_RANGE, CONTACT_DISTANCE);
    }
    else if (getRate(Rates::RATE_TARGET_POS_RECALCULATION_RANGE) > NOMINAL_MELEE_RANGE)
    {
        SF_LOG_ERROR("server.loading", "TargetPosRecalculateRange (%f) must be <= %f. Using %f instead.",
            getRate(Rates::RATE_TARGET_POS_RECALCULATION_RANGE), NOMINAL_MELEE_RANGE, NOMINAL_MELEE_RANGE);
        setRate(Rates::RATE_TARGET_POS_RECALCULATION_RANGE, NOMINAL_MELEE_RANGE);
    }

    setRate(Rates::RATE_DURABILITY_LOSS_ON_DEATH, sConfigMgr->GetFloatDefault("DurabilityLoss.OnDeath", 10.0f));
    if (getRate(Rates::RATE_DURABILITY_LOSS_ON_DEATH) < 0.0f)
    {
        SF_LOG_ERROR("server.loading", "DurabilityLoss.OnDeath (%f) must be >=0. Using 0.0 instead.", getRate(Rates::RATE_DURABILITY_LOSS_ON_DEATH));
        setRate(Rates::RATE_DURABILITY_LOSS_ON_DEATH, 0.0f);
    }
    if (getRate(Rates::RATE_DURABILITY_LOSS_ON_DEATH) > 100.0f)
    {
        SF_LOG_ERROR("server.loading", "DurabilityLoss.OnDeath (%f) must be <= 100. Using 100.0 instead.", getRate(Rates::RATE_DURABILITY_LOSS_ON_DEATH));
        setRate(Rates::RATE_DURABILITY_LOSS_ON_DEATH, 0.0f);
    }
    setRate(Rates::RATE_DURABILITY_LOSS_ON_DEATH, getRate(Rates::RATE_DURABILITY_LOSS_ON_DEATH) / 100.0f);

    setRate(Rates::RATE_DURABILITY_LOSS_DAMAGE, sConfigMgr->GetFloatDefault("DurabilityLossChance.Damage", 0.5f));
    if (getRate(Rates::RATE_DURABILITY_LOSS_DAMAGE) < 0.0f)
    {
        SF_LOG_ERROR("server.loading", "DurabilityLossChance.Damage (%f) must be >=0. Using 0.0 instead.", getRate(Rates::RATE_DURABILITY_LOSS_DAMAGE));
        setRate(Rates::RATE_DURABILITY_LOSS_DAMAGE, 0.0f);
    }
    setRate(Rates::RATE_DURABILITY_LOSS_ABSORB, sConfigMgr->GetFloatDefault("DurabilityLossChance.Absorb", 0.5f));
    if (getRate(Rates::RATE_DURABILITY_LOSS_ABSORB) < 0.0f)
    {
        SF_LOG_ERROR("server.loading", "DurabilityLossChance.Absorb (%f) must be >=0. Using 0.0 instead.", getRate(Rates::RATE_DURABILITY_LOSS_ABSORB));
        setRate(Rates::RATE_DURABILITY_LOSS_ABSORB, 0.0f);
    }
    setRate(Rates::RATE_DURABILITY_LOSS_PARRY, sConfigMgr->GetFloatDefault("DurabilityLossChance.Parry", 0.05f));
    if (getRate(Rates::RATE_DURABILITY_LOSS_PARRY) < 0.0f)
    {
        SF_LOG_ERROR("server.loading", "DurabilityLossChance.Parry (%f) must be >=0. Using 0.0 instead.", getRate(Rates::RATE_DURABILITY_LOSS_PARRY));
        setRate(Rates::RATE_DURABILITY_LOSS_PARRY, 0.0f);
    }
    setRate(Rates::RATE_DURABILITY_LOSS_BLOCK, sConfigMgr->GetFloatDefault("DurabilityLossChance.Block", 0.05f));
    if (getRate(Rates::RATE_DURABILITY_LOSS_BLOCK) < 0.0f)
    {
        SF_LOG_ERROR("server.loading", "DurabilityLossChance.Block (%f) must be >=0. Using 0.0 instead.", getRate(Rates::RATE_DURABILITY_LOSS_BLOCK));
        setRate(Rates::RATE_DURABILITY_LOSS_BLOCK, 0.0f);
    }
    ///- Read other configuration items from the config file

    SetBoolConfig(WorldBoolConfigs::CONFIG_DURABILITY_LOSS_IN_PVP, sConfigMgr->GetBoolDefault("DurabilityLoss.InPvP", false));

    setIntConfig(WorldIntConfigs::CONFIG_COMPRESSION, sConfigMgr->GetIntDefault("Compression", 1));
    if (getIntConfig(WorldIntConfigs::CONFIG_COMPRESSION) < 1 || getIntConfig(WorldIntConfigs::CONFIG_COMPRESSION) > 9)
    {
        SF_LOG_ERROR("server.loading", "Compression level (%i) must be in range 1..9. Using default compression level (1).", getIntConfig(WorldIntConfigs::CONFIG_COMPRESSION));
        setIntConfig(WorldIntConfigs::CONFIG_COMPRESSION, 1);
    }
    SetBoolConfig(WorldBoolConfigs::CONFIG_ADDON_CHANNEL, sConfigMgr->GetBoolDefault("AddonChannel", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_CLEAN_CHARACTER_DB, sConfigMgr->GetBoolDefault("CleanCharacterDB", false));
    setIntConfig(WorldIntConfigs::CONFIG_PERSISTENT_CHARACTER_CLEAN_FLAGS, sConfigMgr->GetIntDefault("PersistentCharacterCleanFlags", 0));
    setIntConfig(WorldIntConfigs::CONFIG_CHAT_CHANNEL_LEVEL_REQ, sConfigMgr->GetIntDefault("ChatLevelReq.Channel", 1));
    setIntConfig(WorldIntConfigs::CONFIG_CHAT_WHISPER_LEVEL_REQ, sConfigMgr->GetIntDefault("ChatLevelReq.Whisper", 1));
    setIntConfig(WorldIntConfigs::CONFIG_CHAT_SAY_LEVEL_REQ, sConfigMgr->GetIntDefault("ChatLevelReq.Say", 1));
    setIntConfig(WorldIntConfigs::CONFIG_TRADE_LEVEL_REQ, sConfigMgr->GetIntDefault("LevelReq.Trade", 1));
    setIntConfig(WorldIntConfigs::CONFIG_TICKET_LEVEL_REQ, sConfigMgr->GetIntDefault("LevelReq.Ticket", 1));
    setIntConfig(WorldIntConfigs::CONFIG_AUCTION_LEVEL_REQ, sConfigMgr->GetIntDefault("LevelReq.Auction", 1));
    setIntConfig(WorldIntConfigs::CONFIG_MAIL_LEVEL_REQ, sConfigMgr->GetIntDefault("LevelReq.Mail", 1));
    SetBoolConfig(WorldBoolConfigs::CONFIG_PRESERVE_CUSTOM_CHANNELS, sConfigMgr->GetBoolDefault("PreserveCustomChannels", false));
    setIntConfig(WorldIntConfigs::CONFIG_PRESERVE_CUSTOM_CHANNEL_DURATION, sConfigMgr->GetIntDefault("PreserveCustomChannelDuration", 14));
    SetBoolConfig(WorldBoolConfigs::CONFIG_GRID_UNLOAD, sConfigMgr->GetBoolDefault("GridUnload", true));
    setIntConfig(WorldIntConfigs::CONFIG_INTERVAL_SAVE, sConfigMgr->GetIntDefault("PlayerSaveInterval", 15 * MINUTE * IN_MILLISECONDS));
    setIntConfig(WorldIntConfigs::CONFIG_INTERVAL_DISCONNECT_TOLERANCE, sConfigMgr->GetIntDefault("DisconnectToleranceInterval", 0));
    SetBoolConfig(WorldBoolConfigs::CONFIG_STATS_SAVE_ONLY_ON_LOGOUT, sConfigMgr->GetBoolDefault("PlayerSave.Stats.SaveOnlyOnLogout", true));

    setIntConfig(WorldIntConfigs::CONFIG_MIN_LEVEL_STAT_SAVE, sConfigMgr->GetIntDefault("PlayerSave.Stats.MinLevel", 0));
    if (getIntConfig(WorldIntConfigs::CONFIG_MIN_LEVEL_STAT_SAVE) > MAX_LEVEL)
    {
        SF_LOG_ERROR("server.loading", "PlayerSave.Stats.MinLevel (%i) must be in range 0..80. Using default, do not save character stats (0).", getIntConfig(WorldIntConfigs::CONFIG_MIN_LEVEL_STAT_SAVE));
        setIntConfig(WorldIntConfigs::CONFIG_MIN_LEVEL_STAT_SAVE, 0);
    }

    setIntConfig(WorldIntConfigs::CONFIG_INTERVAL_GRIDCLEAN, sConfigMgr->GetIntDefault("GridCleanUpDelay", 5 * MINUTE * IN_MILLISECONDS));
    if (getIntConfig(WorldIntConfigs::CONFIG_INTERVAL_GRIDCLEAN) < MIN_GRID_DELAY)
    {
        SF_LOG_ERROR("server.loading", "GridCleanUpDelay (%i) must be greater %u. Use this minimal value.", getIntConfig(WorldIntConfigs::CONFIG_INTERVAL_GRIDCLEAN), MIN_GRID_DELAY);
        setIntConfig(WorldIntConfigs::CONFIG_INTERVAL_GRIDCLEAN, MIN_GRID_DELAY);
    }
    if (reload)
        sMapMgr->SetGridCleanUpDelay(getIntConfig(WorldIntConfigs::CONFIG_INTERVAL_GRIDCLEAN));

    setIntConfig(WorldIntConfigs::CONFIG_INTERVAL_MAPUPDATE, sConfigMgr->GetIntDefault("MapUpdateInterval", 100));
    if (getIntConfig(WorldIntConfigs::CONFIG_INTERVAL_MAPUPDATE) < MIN_MAP_UPDATE_DELAY)
    {
        SF_LOG_ERROR("server.loading", "MapUpdateInterval (%i) must be greater %u. Use this minimal value.", getIntConfig(WorldIntConfigs::CONFIG_INTERVAL_MAPUPDATE), MIN_MAP_UPDATE_DELAY);
        setIntConfig(WorldIntConfigs::CONFIG_INTERVAL_MAPUPDATE, MIN_MAP_UPDATE_DELAY);
    }
    if (reload)
        sMapMgr->SetMapUpdateInterval(getIntConfig(WorldIntConfigs::CONFIG_INTERVAL_MAPUPDATE));

    setIntConfig(WorldIntConfigs::CONFIG_INTERVAL_CHANGEWEATHER, sConfigMgr->GetIntDefault("ChangeWeatherInterval", 10 * MINUTE * IN_MILLISECONDS));

    if (reload)
    {
        uint32 val = sConfigMgr->GetIntDefault("WorldServerPort", 8085);
        if (val != getIntConfig(WorldIntConfigs::CONFIG_PORT_WORLD))
            SF_LOG_ERROR("server.loading", "WorldServerPort option can't be changed at worldserver.conf reload, using current value (%u).", getIntConfig(WorldIntConfigs::CONFIG_PORT_WORLD));
    }
    else
        setIntConfig(WorldIntConfigs::CONFIG_PORT_WORLD, sConfigMgr->GetIntDefault("WorldServerPort", 8085));

    setIntConfig(WorldIntConfigs::CONFIG_SOCKET_TIMEOUTTIME, sConfigMgr->GetIntDefault("SocketTimeOutTime", 900000));
    setIntConfig(WorldIntConfigs::CONFIG_SESSION_ADD_DELAY, sConfigMgr->GetIntDefault("SessionAddDelay", 10000));

    SetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE, sConfigMgr->GetFloatDefault("MaxGroupXPDistance", 74.0f));
    SetFloatConfig(WorldFloatConfigs::CONFIG_MAX_RECRUIT_A_FRIEND_DISTANCE, sConfigMgr->GetFloatDefault("MaxRecruitAFriendBonusDistance", 100.0f));

    /// @todo Add MonsterSight and GuarderSight (with meaning) in worldserver.conf or put them as define
    SetFloatConfig(WorldFloatConfigs::CONFIG_SIGHT_MONSTER, sConfigMgr->GetFloatDefault("MonsterSight", 50));
    SetFloatConfig(WorldFloatConfigs::CONFIG_SIGHT_GUARDER, sConfigMgr->GetFloatDefault("GuarderSight", 50));

    if (reload)
    {
        uint32 val = sConfigMgr->GetIntDefault("GameType", 0);
        if (val != getIntConfig(WorldIntConfigs::CONFIG_GAME_TYPE))
            SF_LOG_ERROR("server.loading", "GameType option can't be changed at worldserver.conf reload, using current value (%u).", getIntConfig(WorldIntConfigs::CONFIG_GAME_TYPE));
    }
    else
        setIntConfig(WorldIntConfigs::CONFIG_GAME_TYPE, sConfigMgr->GetIntDefault("GameType", 0));

    if (reload)
    {
        uint32 val = sConfigMgr->GetIntDefault("RealmZone", REALM_ZONE_DEVELOPMENT);
        if (val != getIntConfig(WorldIntConfigs::CONFIG_REALM_ZONE))
            SF_LOG_ERROR("server.loading", "RealmZone option can't be changed at worldserver.conf reload, using current value (%u).", getIntConfig(WorldIntConfigs::CONFIG_REALM_ZONE));
    }
    else
        setIntConfig(WorldIntConfigs::CONFIG_REALM_ZONE, sConfigMgr->GetIntDefault("RealmZone", REALM_ZONE_DEVELOPMENT));

    SetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_CALENDAR, sConfigMgr->GetBoolDefault("AllowTwoSide.Interaction.Calendar", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHANNEL, sConfigMgr->GetBoolDefault("AllowTwoSide.Interaction.Channel", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP, sConfigMgr->GetBoolDefault("AllowTwoSide.Interaction.Group", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD, sConfigMgr->GetBoolDefault("AllowTwoSide.Interaction.Guild", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_AUCTION, sConfigMgr->GetBoolDefault("AllowTwoSide.Interaction.Auction", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_TRADE, sConfigMgr->GetBoolDefault("AllowTwoSide.trade", false));
    setIntConfig(WorldIntConfigs::CONFIG_STRICT_PLAYER_NAMES, sConfigMgr->GetIntDefault ("StrictPlayerNames",  0));
    setIntConfig(WorldIntConfigs::CONFIG_STRICT_CHARTER_NAMES, sConfigMgr->GetIntDefault ("StrictCharterNames", 0));
    setIntConfig(WorldIntConfigs::CONFIG_STRICT_PET_NAMES, sConfigMgr->GetIntDefault ("StrictPetNames",     0));

    setIntConfig(WorldIntConfigs::CONFIG_MIN_PLAYER_NAME, sConfigMgr->GetIntDefault ("MinPlayerName",  2));
    if (getIntConfig(WorldIntConfigs::CONFIG_MIN_PLAYER_NAME) < 1 || getIntConfig(WorldIntConfigs::CONFIG_MIN_PLAYER_NAME) > MAX_PLAYER_NAME)
    {
        SF_LOG_ERROR("server.loading", "MinPlayerName (%i) must be in range 1..%u. Set to 2.", getIntConfig(WorldIntConfigs::CONFIG_MIN_PLAYER_NAME), MAX_PLAYER_NAME);
        setIntConfig(WorldIntConfigs::CONFIG_MIN_PLAYER_NAME, 2);
    }

    setIntConfig(WorldIntConfigs::CONFIG_MIN_CHARTER_NAME, sConfigMgr->GetIntDefault ("MinCharterName", 2));
    if (getIntConfig(WorldIntConfigs::CONFIG_MIN_CHARTER_NAME) < 1 || getIntConfig(WorldIntConfigs::CONFIG_MIN_CHARTER_NAME) > MAX_CHARTER_NAME)
    {
        SF_LOG_ERROR("server.loading", "MinCharterName (%i) must be in range 1..%u. Set to 2.", getIntConfig(WorldIntConfigs::CONFIG_MIN_CHARTER_NAME), MAX_CHARTER_NAME);
        setIntConfig(WorldIntConfigs::CONFIG_MIN_CHARTER_NAME, 2);
    }

    setIntConfig(WorldIntConfigs::CONFIG_MIN_PET_NAME, sConfigMgr->GetIntDefault("MinPetName", 2));
    if (getIntConfig(WorldIntConfigs::CONFIG_MIN_PET_NAME) < 1 || getIntConfig(WorldIntConfigs::CONFIG_MIN_PET_NAME) > MAX_PET_NAME)
    {
        SF_LOG_ERROR("server.loading", "MinPetName (%i) must be in range 1..%u. Set to 2.", getIntConfig(WorldIntConfigs::CONFIG_MIN_PET_NAME), MAX_PET_NAME);
        setIntConfig(WorldIntConfigs::CONFIG_MIN_PET_NAME, 2);
    }

    setIntConfig(WorldIntConfigs::CONFIG_CHARACTER_CREATING_DISABLED, sConfigMgr->GetIntDefault("CharacterCreating.Disabled", 0));
    setIntConfig(WorldIntConfigs::CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK, sConfigMgr->GetIntDefault("CharacterCreating.Disabled.RaceMask", 0));
    setIntConfig(WorldIntConfigs::CONFIG_CHARACTER_CREATING_DISABLED_CLASSMASK, sConfigMgr->GetIntDefault("CharacterCreating.Disabled.ClassMask", 0));

    setIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_REALM, sConfigMgr->GetIntDefault("CharactersPerRealm", 11));
    if (getIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_REALM) < 1 || getIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_REALM) > 11)
    {
        SF_LOG_ERROR("server.loading", "CharactersPerRealm (%i) must be in range 1..11. Set to 11.", getIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_REALM));
        setIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_REALM, 11);
    }

    // must be after CONFIG_CHARACTERS_PER_REALM
    setIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_ACCOUNT, sConfigMgr->GetIntDefault("CharactersPerAccount", 50));
    if (getIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_ACCOUNT) < getIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_REALM))
    {
        SF_LOG_ERROR("server.loading", "CharactersPerAccount (%i) can't be less than CharactersPerRealm (%i).", getIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_ACCOUNT), getIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_REALM));
        setIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_ACCOUNT, getIntConfig(WorldIntConfigs::CONFIG_CHARACTERS_PER_REALM));
    }

    setIntConfig(WorldIntConfigs::CONFIG_HEROIC_CHARACTERS_PER_REALM, sConfigMgr->GetIntDefault("HeroicCharactersPerRealm", 1));
    if (int32(getIntConfig(WorldIntConfigs::CONFIG_HEROIC_CHARACTERS_PER_REALM)) < 0 || getIntConfig(WorldIntConfigs::CONFIG_HEROIC_CHARACTERS_PER_REALM) > 10)
    {
        SF_LOG_ERROR("server.loading", "HeroicCharactersPerRealm (%i) must be in range 0..10. Set to 1.", getIntConfig(WorldIntConfigs::CONFIG_HEROIC_CHARACTERS_PER_REALM));
        setIntConfig(WorldIntConfigs::CONFIG_HEROIC_CHARACTERS_PER_REALM, 1);
    }

    setIntConfig(WorldIntConfigs::CONFIG_CHARACTER_CREATING_MIN_LEVEL_FOR_HEROIC_CHARACTER, sConfigMgr->GetIntDefault("CharacterCreating.MinLevelForHeroicCharacter", 55));

    setIntConfig(WorldIntConfigs::CONFIG_SKIP_CINEMATICS, sConfigMgr->GetIntDefault("SkipCinematics", 0));
    if (int32(getIntConfig(WorldIntConfigs::CONFIG_SKIP_CINEMATICS)) < 0 || getIntConfig(WorldIntConfigs::CONFIG_SKIP_CINEMATICS) > 2)
    {
        SF_LOG_ERROR("server.loading", "SkipCinematics (%i) must be in range 0..2. Set to 0.", getIntConfig(WorldIntConfigs::CONFIG_SKIP_CINEMATICS));
        setIntConfig(WorldIntConfigs::CONFIG_SKIP_CINEMATICS, 0);
    }

    if (reload)
    {
        uint32 val = sConfigMgr->GetIntDefault("MaxPlayerLevel", DEFAULT_MAX_LEVEL);
        if (val != getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL))
            SF_LOG_ERROR("server.loading", "MaxPlayerLevel option can't be changed at config reload, using current value (%u).", getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL));
    }
    else
        setIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL, sConfigMgr->GetIntDefault("MaxPlayerLevel", DEFAULT_MAX_LEVEL));

    if (getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL) > MAX_LEVEL)
    {
        SF_LOG_ERROR("server.loading", "MaxPlayerLevel (%i) must be in range 1..%u. Set to %u.", getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL), MAX_LEVEL, MAX_LEVEL);
        setIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL, MAX_LEVEL);
    }

    setIntConfig(WorldIntConfigs::CONFIG_MIN_DUALSPEC_LEVEL, sConfigMgr->GetIntDefault("MinDualSpecLevel", 40));

    setIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL, sConfigMgr->GetIntDefault("StartPlayerLevel", 1));
    if (getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL) < 1)
    {
        SF_LOG_ERROR("server.loading", "StartPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to 1.", getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL), getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL));
        setIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL, 1);
    }
    else if (getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL) > getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL))
    {
        SF_LOG_ERROR("server.loading", "StartPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to %u.", getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL), getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL), getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL));
        setIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL, getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL));
    }

    setIntConfig(WorldIntConfigs::CONFIG_START_HEROIC_PLAYER_LEVEL, sConfigMgr->GetIntDefault("StartHeroicPlayerLevel", 55));
    if (getIntConfig(WorldIntConfigs::CONFIG_START_HEROIC_PLAYER_LEVEL) < 1)
    {
        SF_LOG_ERROR("server.loading", "StartHeroicPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to 55.",
            getIntConfig(WorldIntConfigs::CONFIG_START_HEROIC_PLAYER_LEVEL), getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL));
        setIntConfig(WorldIntConfigs::CONFIG_START_HEROIC_PLAYER_LEVEL, 55);
    }
    else if (getIntConfig(WorldIntConfigs::CONFIG_START_HEROIC_PLAYER_LEVEL) > getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL))
    {
        SF_LOG_ERROR("server.loading", "StartHeroicPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to %u.",
            getIntConfig(WorldIntConfigs::CONFIG_START_HEROIC_PLAYER_LEVEL), getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL), getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL));
        setIntConfig(WorldIntConfigs::CONFIG_START_HEROIC_PLAYER_LEVEL, getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL));
    }

    setIntConfig(WorldIntConfigs::CONFIG_START_PETBAR_LEVEL, sConfigMgr->GetIntDefault("StartPetbarLevel", 10));

    setIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_MONEY, sConfigMgr->GetIntDefault("StartPlayerMoney", 0));
    if (int32(getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_MONEY)) < 0)
    {
        SF_LOG_ERROR("server.loading", "StartPlayerMoney (%i) must be in range 0.." UI64FMTD ". Set to %u.", getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_MONEY), uint64(MAX_MONEY_AMOUNT), 0);
        setIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_MONEY, 0);
    }
    else if (getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_MONEY) > 0x7FFFFFFF-1) // TODO: (See MAX_MONEY_AMOUNT)
    {
        SF_LOG_ERROR("server.loading", "StartPlayerMoney (%i) must be in range 0..%u. Set to %u.",
            getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_MONEY), 0x7FFFFFFF-1, 0x7FFFFFFF-1);
        setIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_MONEY, 0x7FFFFFFF-1);
    }

    setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_HOUR, sConfigMgr->GetIntDefault("Currency.ResetHour", 3));
    if (getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_HOUR) > 23)
    {
        SF_LOG_ERROR("server.loading", "Currency.ResetHour (%i) can't be load. Set to 6.", getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_HOUR));
        setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_HOUR, 3);
    }
    setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_DAY, sConfigMgr->GetIntDefault("Currency.ResetDay", 3));
    if (getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_DAY) > 6)
    {
        SF_LOG_ERROR("server.loading", "Currency.ResetDay (%i) can't be load. Set to 3.", getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_DAY));
        setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_DAY, 3);
    }
    setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_INTERVAL, sConfigMgr->GetIntDefault("Currency.ResetInterval", 7));
    if (getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_INTERVAL) <= 0)
    {
        SF_LOG_ERROR("server.loading", "Currency.ResetInterval (%i) must be > 0, set to default 7.", getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_INTERVAL));
        setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_INTERVAL, 7);
    }

    setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_HONOR_POINTS, sConfigMgr->GetIntDefault("Currency.StartHonorPoints", 0));
    if (int32(getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_HONOR_POINTS)) < 0)
    {
        SF_LOG_ERROR("server.loading", "Currency.StartHonorPoints (%i) must be >= 0, set to default 0.", getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_HONOR_POINTS));
        setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_HONOR_POINTS, 0);
    }
    setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_MAX_HONOR_POINTS, sConfigMgr->GetIntDefault("Currency.MaxHonorPoints", 4000));
    if (int32(getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_MAX_HONOR_POINTS)) < 0)
    {
        SF_LOG_ERROR("server.loading", "Currency.MaxHonorPoints (%i) can't be negative. Set to default 4000.", getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_MAX_HONOR_POINTS));
        setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_MAX_HONOR_POINTS, 4000);
    }
    //...
    //setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_MAX_HONOR_POINTS] *= 100;     //precision mod

    setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_JUSTICE_POINTS, sConfigMgr->GetIntDefault("Currency.StartJusticePoints", 0));
    if (int32(getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_JUSTICE_POINTS)) < 0)
    {
        SF_LOG_ERROR("server.loading", "Currency.StartJusticePoints (%i) must be >= 0, set to default 0.", getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_JUSTICE_POINTS));
        setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_JUSTICE_POINTS, 0);
    }
    setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_MAX_JUSTICE_POINTS, sConfigMgr->GetIntDefault("Currency.MaxJusticePoints", 4000));
    if (int32(getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_MAX_JUSTICE_POINTS)) < 0)
    {
        SF_LOG_ERROR("server.loading", "Currency.MaxJusticePoints (%i) can't be negative. Set to default 4000.", getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_MAX_JUSTICE_POINTS));
        setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_MAX_JUSTICE_POINTS, 4000);
    }
    //...
    //setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_MAX_JUSTICE_POINTS] *= 100;     //precision mod

    setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_CONQUEST_POINTS, sConfigMgr->GetIntDefault("Currency.StartConquestPoints", 0));
    if (int32(getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_CONQUEST_POINTS)) < 0)
    {
        SF_LOG_ERROR("server.loading", "Currency.StartConquestPoints (%i) must be >= 0, set to default 0.", getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_CONQUEST_POINTS));
        setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_START_CONQUEST_POINTS, 0);
    }
    setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP, sConfigMgr->GetIntDefault("Currency.ConquestPointsWeekCap", 1650));
    if (getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP) <= 0)
    {
        SF_LOG_ERROR("server.loading", "Currency.ConquestPointsWeekCap (%i) must be > 0, set to default 1650.", getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP));
        setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP, 1650);
    }
    //...
    //setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP] *= 100;     //precision mod

    setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD, sConfigMgr->GetIntDefault("Currency.ConquestPointsArenaReward", 180));
    if (getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD) <= 0)
    {
        SF_LOG_ERROR("server.loading", "Currency.ConquestPointsArenaReward (%i) must be > 0, set to default 180.", getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD));
        setIntConfig(WorldIntConfigs::CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD, 180);
    }
    //[WorldIntConfigs::CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD] *= 100;     //precision mod

    setIntConfig(WorldIntConfigs::CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL, sConfigMgr->GetIntDefault("RecruitAFriend.MaxLevel", 60));
    if (getIntConfig(WorldIntConfigs::CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL) > getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL))
    {
        SF_LOG_ERROR("server.loading", "RecruitAFriend.MaxLevel (%i) must be in the range 0..MaxLevel(%u). Set to %u.",
            getIntConfig(WorldIntConfigs::CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL), getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL), 60);
        setIntConfig(WorldIntConfigs::CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL, 60);
    }

    setIntConfig(WorldIntConfigs::CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL_DIFFERENCE, sConfigMgr->GetIntDefault("RecruitAFriend.MaxDifference", 4));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ALL_TAXI_PATHS, sConfigMgr->GetBoolDefault("AllFlightPaths", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_INSTANT_TAXI, sConfigMgr->GetBoolDefault("InstantFlightPaths", false));

    SetBoolConfig(WorldBoolConfigs::CONFIG_INSTANCE_IGNORE_LEVEL, sConfigMgr->GetBoolDefault("Instance.IgnoreLevel", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_INSTANCE_IGNORE_RAID, sConfigMgr->GetBoolDefault("Instance.IgnoreRaid", false));

    SetBoolConfig(WorldBoolConfigs::CONFIG_CAST_UNSTUCK, sConfigMgr->GetBoolDefault("CastUnstuck", true));
    setIntConfig(WorldIntConfigs::CONFIG_INSTANCE_RESET_TIME_HOUR, sConfigMgr->GetIntDefault("Instance.ResetTimeHour", 4));
    setIntConfig(WorldIntConfigs::CONFIG_INSTANCE_UNLOAD_DELAY, sConfigMgr->GetIntDefault("Instance.UnloadDelay", 30 * MINUTE * IN_MILLISECONDS));

    setIntConfig(WorldIntConfigs::CONFIG_MAX_PRIMARY_TRADE_SKILL, sConfigMgr->GetIntDefault("MaxPrimaryTradeSkill", 2));
    setIntConfig(WorldIntConfigs::CONFIG_MIN_PETITION_SIGNS, sConfigMgr->GetIntDefault("MinPetitionSigns", 9));
    if (getIntConfig(WorldIntConfigs::CONFIG_MIN_PETITION_SIGNS) > 9)
    {
        SF_LOG_ERROR("server.loading", "MinPetitionSigns (%i) must be in range 0..9. Set to 9.", getIntConfig(WorldIntConfigs::CONFIG_MIN_PETITION_SIGNS));
        setIntConfig(WorldIntConfigs::CONFIG_MIN_PETITION_SIGNS, 9);
    }

    setIntConfig(WorldIntConfigs::CONFIG_GM_LOGIN_STATE, sConfigMgr->GetIntDefault("GM.LoginState", 2));
    setIntConfig(WorldIntConfigs::CONFIG_GM_VISIBLE_STATE, sConfigMgr->GetIntDefault("GM.Visible", 2));
    setIntConfig(WorldIntConfigs::CONFIG_GM_CHAT, sConfigMgr->GetIntDefault("GM.Chat", 2));
    setIntConfig(WorldIntConfigs::CONFIG_GM_WHISPERING_TO, sConfigMgr->GetIntDefault("GM.WhisperingTo", 2));

    setIntConfig(WorldIntConfigs::CONFIG_GM_LEVEL_IN_GM_LIST, sConfigMgr->GetIntDefault("GM.InGMList.Level", uint8(AccountTypes::SEC_ADMINISTRATOR)));
    setIntConfig(WorldIntConfigs::CONFIG_GM_LEVEL_IN_WHO_LIST, sConfigMgr->GetIntDefault("GM.InWhoList.Level", uint8(AccountTypes::SEC_ADMINISTRATOR)));
    setIntConfig(WorldIntConfigs::CONFIG_START_GM_LEVEL, sConfigMgr->GetIntDefault("GM.StartLevel", 1));
    if (getIntConfig(WorldIntConfigs::CONFIG_START_GM_LEVEL) < getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL))
    {
        SF_LOG_ERROR("server.loading", "GM.StartLevel (%i) must be in range StartPlayerLevel(%u)..%u. Set to %u.",
            getIntConfig(WorldIntConfigs::CONFIG_START_GM_LEVEL), getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL), MAX_LEVEL, getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL));
        setIntConfig(WorldIntConfigs::CONFIG_START_GM_LEVEL, getIntConfig(WorldIntConfigs::CONFIG_START_PLAYER_LEVEL));
    }
    else if (getIntConfig(WorldIntConfigs::CONFIG_START_GM_LEVEL) > MAX_LEVEL)
    {
        SF_LOG_ERROR("server.loading", "GM.StartLevel (%i) must be in range 1..%u. Set to %u.", getIntConfig(WorldIntConfigs::CONFIG_START_GM_LEVEL), MAX_LEVEL, MAX_LEVEL);
        setIntConfig(WorldIntConfigs::CONFIG_START_GM_LEVEL, MAX_LEVEL);
    }
    SetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_GM_GROUP, sConfigMgr->GetBoolDefault("GM.AllowInvite", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_GM_LOWER_SECURITY, sConfigMgr->GetBoolDefault("GM.LowerSecurity", false));
    SetFloatConfig(WorldFloatConfigs::CONFIG_CHANCE_OF_GM_SURVEY, sConfigMgr->GetFloatDefault("GM.TicketSystem.ChanceOfGMSurvey", 50.0f));

    setIntConfig(WorldIntConfigs::CONFIG_GROUP_VISIBILITY, sConfigMgr->GetIntDefault("Visibility.GroupMode", 1));

    setIntConfig(WorldIntConfigs::CONFIG_MAIL_DELIVERY_DELAY, sConfigMgr->GetIntDefault("MailDeliveryDelay", HOUR));

    setIntConfig(WorldIntConfigs::CONFIG_UPTIME_UPDATE, sConfigMgr->GetIntDefault("UpdateUptimeInterval", 10));
    if (getIntConfig(WorldIntConfigs::CONFIG_UPTIME_UPDATE) <= 0)
    {
        SF_LOG_ERROR("server.loading", "UpdateUptimeInterval (%i) must be > 0, set to default 10.", getIntConfig(WorldIntConfigs::CONFIG_UPTIME_UPDATE));
        setIntConfig(WorldIntConfigs::CONFIG_UPTIME_UPDATE, 10);
    }
    if (reload)
    {
        m_timers[WUPDATE_UPTIME].SetInterval(getIntConfig(WorldIntConfigs::CONFIG_UPTIME_UPDATE)*MINUTE*IN_MILLISECONDS);
        m_timers[WUPDATE_UPTIME].Reset();
    }

    // log db cleanup interval
    setIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARINTERVAL, sConfigMgr->GetIntDefault("LogDB.Opt.ClearInterval", 10));
    if (getIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARINTERVAL) <= 0)
    {
        SF_LOG_ERROR("server.loading", "LogDB.Opt.ClearInterval (%i) must be > 0, set to default 10.", getIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARINTERVAL));
        setIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARINTERVAL, 10);
    }
    if (reload)
    {
        m_timers[WUPDATE_CLEANDB].SetInterval(getIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARINTERVAL) * MINUTE * IN_MILLISECONDS);
        m_timers[WUPDATE_CLEANDB].Reset();
    }
    setIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARTIME, sConfigMgr->GetIntDefault("LogDB.Opt.ClearTime", 1209600)); // 14 days default
    SF_LOG_INFO("server.loading", "Will clear `logs` table of entries older than %i seconds every %u minutes.",
        getIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARTIME), getIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARINTERVAL));

    setIntConfig(WorldIntConfigs::CONFIG_SKILL_CHANCE_ORANGE, sConfigMgr->GetIntDefault("SkillChance.Orange", 100));
    setIntConfig(WorldIntConfigs::CONFIG_SKILL_CHANCE_YELLOW, sConfigMgr->GetIntDefault("SkillChance.Yellow", 75));
    setIntConfig(WorldIntConfigs::CONFIG_SKILL_CHANCE_GREEN, sConfigMgr->GetIntDefault("SkillChance.Green", 25));
    setIntConfig(WorldIntConfigs::CONFIG_SKILL_CHANCE_GREY, sConfigMgr->GetIntDefault("SkillChance.Grey", 0));

    setIntConfig(WorldIntConfigs::CONFIG_SKILL_CHANCE_MINING_STEPS, sConfigMgr->GetIntDefault("SkillChance.MiningSteps", 75));
    setIntConfig(WorldIntConfigs::CONFIG_SKILL_CHANCE_SKINNING_STEPS, sConfigMgr->GetIntDefault("SkillChance.SkinningSteps", 75));

    SetBoolConfig(WorldBoolConfigs::CONFIG_SKILL_PROSPECTING, sConfigMgr->GetBoolDefault("SkillChance.Prospecting", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_SKILL_MILLING, sConfigMgr->GetBoolDefault("SkillChance.Milling", false));

    setIntConfig(WorldIntConfigs::CONFIG_SKILL_GAIN_CRAFTING, sConfigMgr->GetIntDefault("SkillGain.Crafting", 1));

    setIntConfig(WorldIntConfigs::CONFIG_SKILL_GAIN_GATHERING, sConfigMgr->GetIntDefault("SkillGain.Gathering", 1));

    setIntConfig(WorldIntConfigs::CONFIG_MAX_OVERSPEED_PINGS, sConfigMgr->GetIntDefault("MaxOverspeedPings", 2));

    if (getIntConfig(WorldIntConfigs::CONFIG_MAX_OVERSPEED_PINGS) != 0 && getIntConfig(WorldIntConfigs::CONFIG_MAX_OVERSPEED_PINGS) < 2)
    {
        SF_LOG_ERROR("server.loading", "MaxOverspeedPings (%i) must be in range 2..infinity (or 0 to disable check). Set to 2.", getIntConfig(WorldIntConfigs::CONFIG_MAX_OVERSPEED_PINGS));
        setIntConfig(WorldIntConfigs::CONFIG_MAX_OVERSPEED_PINGS, 2);
    }

    SetBoolConfig(WorldBoolConfigs::CONFIG_SAVE_RESPAWN_TIME_IMMEDIATELY, sConfigMgr->GetBoolDefault("SaveRespawnTimeImmediately", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_WEATHER, sConfigMgr->GetBoolDefault("ActivateWeather", true));

    setIntConfig(WorldIntConfigs::CONFIG_DISABLE_BREATHING, sConfigMgr->GetIntDefault("DisableWaterBreath", uint8(AccountTypes::SEC_CONSOLE)));

    if (reload)
    {
        uint32 val = sConfigMgr->GetIntDefault("Expansion", 1);
        if (val != getIntConfig(WorldIntConfigs::CONFIG_EXPANSION))
            SF_LOG_ERROR("server.loading", "Expansion option can't be changed at worldserver.conf reload, using current value (%u).", getIntConfig(WorldIntConfigs::CONFIG_EXPANSION));
    }
    else
        setIntConfig(WorldIntConfigs::CONFIG_EXPANSION, sConfigMgr->GetIntDefault("Expansion", 1));

    setIntConfig(WorldIntConfigs::CONFIG_CHATFLOOD_MESSAGE_COUNT, sConfigMgr->GetIntDefault("ChatFlood.MessageCount", 10));
    setIntConfig(WorldIntConfigs::CONFIG_CHATFLOOD_MESSAGE_DELAY, sConfigMgr->GetIntDefault("ChatFlood.MessageDelay", 1));
    setIntConfig(WorldIntConfigs::CONFIG_CHATFLOOD_MUTE_TIME, sConfigMgr->GetIntDefault("ChatFlood.MuteTime", 10));

    SetBoolConfig(WorldBoolConfigs::CONFIG_EVENT_ANNOUNCE, sConfigMgr->GetIntDefault("Event.Announce", false));

    SetFloatConfig(WorldFloatConfigs::CONFIG_CREATURE_FAMILY_FLEE_ASSISTANCE_RADIUS, sConfigMgr->GetFloatDefault("CreatureFamilyFleeAssistanceRadius", 30.0f));
    SetFloatConfig(WorldFloatConfigs::CONFIG_CREATURE_FAMILY_ASSISTANCE_RADIUS, sConfigMgr->GetFloatDefault("CreatureFamilyAssistanceRadius", 10.0f));
    setIntConfig(WorldIntConfigs::CONFIG_CREATURE_FAMILY_ASSISTANCE_DELAY, sConfigMgr->GetIntDefault("CreatureFamilyAssistanceDelay", 1500));
    setIntConfig(WorldIntConfigs::CONFIG_CREATURE_FAMILY_FLEE_DELAY, sConfigMgr->GetIntDefault("CreatureFamilyFleeDelay", 7000));

    setIntConfig(WorldIntConfigs::CONFIG_WORLD_BOSS_LEVEL_DIFF, sConfigMgr->GetIntDefault("WorldBossLevelDiff", 3));

    // note: disable value (-1) will assigned as 0xFFFFFFF, to prevent overflow at calculations limit it to max possible player level MAX_LEVEL(100)
    setIntConfig(WorldIntConfigs::CONFIG_QUEST_LOW_LEVEL_HIDE_DIFF, sConfigMgr->GetIntDefault("Quests.LowLevelHideDiff", 4));
    if (getIntConfig(WorldIntConfigs::CONFIG_QUEST_LOW_LEVEL_HIDE_DIFF) > MAX_LEVEL)
        setIntConfig(WorldIntConfigs::CONFIG_QUEST_LOW_LEVEL_HIDE_DIFF, MAX_LEVEL);
    setIntConfig(WorldIntConfigs::CONFIG_QUEST_HIGH_LEVEL_HIDE_DIFF, sConfigMgr->GetIntDefault("Quests.HighLevelHideDiff", 7));
    if (getIntConfig(WorldIntConfigs::CONFIG_QUEST_HIGH_LEVEL_HIDE_DIFF) > MAX_LEVEL)
        setIntConfig(WorldIntConfigs::CONFIG_QUEST_HIGH_LEVEL_HIDE_DIFF, MAX_LEVEL);
    SetBoolConfig(WorldBoolConfigs::CONFIG_QUEST_IGNORE_RAID, sConfigMgr->GetBoolDefault("Quests.IgnoreRaid", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_QUEST_IGNORE_AUTO_ACCEPT, sConfigMgr->GetBoolDefault("Quests.IgnoreAutoAccept", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_QUEST_IGNORE_AUTO_COMPLETE, sConfigMgr->GetBoolDefault("Quests.IgnoreAutoComplete", false));

    setIntConfig(WorldIntConfigs::CONFIG_RANDOM_BG_RESET_HOUR, sConfigMgr->GetIntDefault("Battleground.Random.ResetHour", 6));
    if (getIntConfig(WorldIntConfigs::CONFIG_RANDOM_BG_RESET_HOUR) > 23)
    {
        SF_LOG_ERROR("server.loading", "Battleground.Random.ResetHour (%i) can't be load. Set to 6.", getIntConfig(WorldIntConfigs::CONFIG_RANDOM_BG_RESET_HOUR));
        setIntConfig(WorldIntConfigs::CONFIG_RANDOM_BG_RESET_HOUR, 6);
    }

    setIntConfig(WorldIntConfigs::CONFIG_GUILD_RESET_HOUR, sConfigMgr->GetIntDefault("Guild.ResetHour", 6));
    if (getIntConfig(WorldIntConfigs::CONFIG_GUILD_RESET_HOUR) > 23)
    {
        SF_LOG_ERROR("misc", "Guild.ResetHour (%i) can't be load. Set to 6.", getIntConfig(WorldIntConfigs::CONFIG_GUILD_RESET_HOUR));
        setIntConfig(WorldIntConfigs::CONFIG_GUILD_RESET_HOUR, 6);
    }

    SetBoolConfig(WorldBoolConfigs::CONFIG_DETECT_POS_COLLISION, sConfigMgr->GetBoolDefault("DetectPosCollision", true));

    SetBoolConfig(WorldBoolConfigs::CONFIG_RESTRICTED_LFG_CHANNEL, sConfigMgr->GetBoolDefault("Channel.RestrictedLfg", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_TALENTS_INSPECTING, sConfigMgr->GetBoolDefault("TalentsInspecting", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_CHAT_FAKE_MESSAGE_PREVENTING, sConfigMgr->GetBoolDefault("ChatFakeMessagePreventing", false));
    setIntConfig(WorldIntConfigs::CONFIG_CHAT_STRICT_LINK_CHECKING_SEVERITY, sConfigMgr->GetIntDefault("ChatStrictLinkChecking.Severity", 0));
    setIntConfig(WorldIntConfigs::CONFIG_CHAT_STRICT_LINK_CHECKING_KICK, sConfigMgr->GetIntDefault("ChatStrictLinkChecking.Kick", 0));

    setIntConfig(WorldIntConfigs::CONFIG_CORPSE_DECAY_NORMAL, sConfigMgr->GetIntDefault("Corpse.Decay.NORMAL", 60));
    setIntConfig(WorldIntConfigs::CONFIG_CORPSE_DECAY_RARE, sConfigMgr->GetIntDefault("Corpse.Decay.RARE", 300));
    setIntConfig(WorldIntConfigs::CONFIG_CORPSE_DECAY_ELITE, sConfigMgr->GetIntDefault("Corpse.Decay.ELITE", 300));
    setIntConfig(WorldIntConfigs::CONFIG_CORPSE_DECAY_RAREELITE, sConfigMgr->GetIntDefault("Corpse.Decay.RAREELITE", 300));
    setIntConfig(WorldIntConfigs::CONFIG_CORPSE_DECAY_WORLDBOSS, sConfigMgr->GetIntDefault("Corpse.Decay.WORLDBOSS", 3600));

    setIntConfig(WorldIntConfigs::CONFIG_DEATH_SICKNESS_LEVEL, sConfigMgr->GetIntDefault ("Death.SicknessLevel", 11));
    SetBoolConfig(WorldBoolConfigs::CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVP, sConfigMgr->GetBoolDefault("Death.CorpseReclaimDelay.PvP", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVE, sConfigMgr->GetBoolDefault("Death.CorpseReclaimDelay.PvE", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_DEATH_BONES_WORLD, sConfigMgr->GetBoolDefault("Death.Bones.World", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_DEATH_BONES_BG_OR_ARENA, sConfigMgr->GetBoolDefault("Death.Bones.BattlegroundOrArena", true));

    SetBoolConfig(WorldBoolConfigs::CONFIG_DIE_COMMAND_MODE, sConfigMgr->GetBoolDefault("Die.Command.Mode", true));

    SetFloatConfig(WorldFloatConfigs::CONFIG_THREAT_RADIUS, sConfigMgr->GetFloatDefault("ThreatRadius", 60.0f));

    // always use declined names in the russian client
    SetBoolConfig(WorldBoolConfigs::CONFIG_DECLINED_NAMES_USED,

        (getIntConfig(WorldIntConfigs::CONFIG_REALM_ZONE) == REALM_ZONE_RUSSIAN) ? true : sConfigMgr->GetBoolDefault("DeclinedNames", false));

    SetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_SAY, sConfigMgr->GetFloatDefault("ListenRange.Say", 25.0f));
    SetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_TEXTEMOTE, sConfigMgr->GetFloatDefault("ListenRange.TextEmote", 25.0f));
    SetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_YELL, sConfigMgr->GetFloatDefault("ListenRange.Yell", 300.0f));

    SetBoolConfig(WorldBoolConfigs::CONFIG_BATTLEGROUND_CAST_DESERTER, sConfigMgr->GetBoolDefault("Battleground.CastDeserter", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_ENABLE, sConfigMgr->GetBoolDefault("Battleground.QueueAnnouncer.Enable", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_PLAYERONLY, sConfigMgr->GetBoolDefault("Battleground.QueueAnnouncer.PlayerOnly", false));
    setIntConfig(WorldIntConfigs::CONFIG_BATTLEGROUND_INVITATION_TYPE, sConfigMgr->GetIntDefault ("Battleground.InvitationType", 0));
    setIntConfig(WorldIntConfigs::CONFIG_BATTLEGROUND_PREMATURE_FINISH_TIMER, sConfigMgr->GetIntDefault ("Battleground.PrematureFinishTimer", 5 * MINUTE * IN_MILLISECONDS));
    setIntConfig(WorldIntConfigs::CONFIG_BATTLEGROUND_PREMADE_GROUP_WAIT_FOR_MATCH, sConfigMgr->GetIntDefault ("Battleground.PremadeGroupWaitForMatch", 30 * MINUTE * IN_MILLISECONDS));
    SetBoolConfig(WorldBoolConfigs::CONFIG_BG_XP_FOR_KILL, sConfigMgr->GetBoolDefault("Battleground.GiveXPForKills", false));
    setIntConfig(WorldIntConfigs::CONFIG_ARENA_MAX_RATING_DIFFERENCE, sConfigMgr->GetIntDefault ("Arena.MaxRatingDifference", 150));
    setIntConfig(WorldIntConfigs::CONFIG_ARENA_RATING_DISCARD_TIMER, sConfigMgr->GetIntDefault ("Arena.RatingDiscardTimer", 10 * MINUTE * IN_MILLISECONDS));
    setIntConfig(WorldIntConfigs::CONFIG_ARENA_RATED_UPDATE_TIMER, sConfigMgr->GetIntDefault ("Arena.RatedUpdateTimer", 5 * IN_MILLISECONDS));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ARENA_QUEUE_ANNOUNCER_ENABLE, sConfigMgr->GetBoolDefault("Arena.QueueAnnouncer.Enable", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ARENA_QUEUE_ANNOUNCER_PLAYERONLY, sConfigMgr->GetBoolDefault("Arena.QueueAnnouncer.PlayerOnly", false));
    setIntConfig(WorldIntConfigs::CONFIG_ARENA_SEASON_ID, sConfigMgr->GetIntDefault ("Arena.ArenaSeason.ID", 1));
    setIntConfig(WorldIntConfigs::CONFIG_ARENA_START_RATING, sConfigMgr->GetIntDefault ("Arena.ArenaStartRating", 0));
    setIntConfig(WorldIntConfigs::CONFIG_ARENA_START_PERSONAL_RATING, sConfigMgr->GetIntDefault ("Arena.ArenaStartPersonalRating", 1000));
    setIntConfig(WorldIntConfigs::CONFIG_ARENA_START_MATCHMAKER_RATING, sConfigMgr->GetIntDefault ("Arena.ArenaStartMatchmakerRating", 1500));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ARENA_SEASON_IN_PROGRESS, sConfigMgr->GetBoolDefault("Arena.ArenaSeason.InProgress", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ARENA_LOG_EXTENDED_INFO, sConfigMgr->GetBoolDefault("ArenaLog.ExtendedInfo", false));

    SetBoolConfig(WorldBoolConfigs::CONFIG_OFFHAND_CHECK_AT_SPELL_UNLEARN, sConfigMgr->GetBoolDefault("OffhandCheckAtSpellUnlearn", true));

    if (int32 clientCacheId = sConfigMgr->GetIntDefault("ClientCacheVersion", 0))
    {
        // overwrite DB/old value
        if (clientCacheId > 0)
        {
            setIntConfig(WorldIntConfigs::CONFIG_CLIENTCACHE_VERSION, clientCacheId);
            SF_LOG_INFO("server.loading", "Client cache version set to: %u", clientCacheId);
        }
        else
            SF_LOG_ERROR("server.loading", "ClientCacheVersion can't be negative %d, ignored.", clientCacheId);
    }

    setIntConfig(WorldIntConfigs::CONFIG_GUILD_NEWS_LOG_COUNT, sConfigMgr->GetIntDefault("Guild.NewsLogRecordsCount", GUILD_NEWSLOG_MAX_RECORDS));
    if (getIntConfig(WorldIntConfigs::CONFIG_GUILD_NEWS_LOG_COUNT) > GUILD_NEWSLOG_MAX_RECORDS)
        setIntConfig(WorldIntConfigs::CONFIG_GUILD_NEWS_LOG_COUNT, GUILD_NEWSLOG_MAX_RECORDS);
    setIntConfig(WorldIntConfigs::CONFIG_GUILD_EVENT_LOG_COUNT, sConfigMgr->GetIntDefault("Guild.EventLogRecordsCount", GUILD_EVENTLOG_MAX_RECORDS));
    if (getIntConfig(WorldIntConfigs::CONFIG_GUILD_EVENT_LOG_COUNT) > GUILD_EVENTLOG_MAX_RECORDS)
        setIntConfig(WorldIntConfigs::CONFIG_GUILD_EVENT_LOG_COUNT, GUILD_EVENTLOG_MAX_RECORDS);
    setIntConfig(WorldIntConfigs::CONFIG_GUILD_BANK_EVENT_LOG_COUNT, sConfigMgr->GetIntDefault("Guild.BankEventLogRecordsCount", GUILD_BANKLOG_MAX_RECORDS));
    if (getIntConfig(WorldIntConfigs::CONFIG_GUILD_BANK_EVENT_LOG_COUNT) > GUILD_BANKLOG_MAX_RECORDS)
        setIntConfig(WorldIntConfigs::CONFIG_GUILD_BANK_EVENT_LOG_COUNT, GUILD_BANKLOG_MAX_RECORDS);

    // battle pet
    setIntConfig(WorldIntConfigs::CONFIG_BATTLE_PET_LOADOUT_UNLOCK_COUNT, sConfigMgr->GetIntDefault("BattlePet.LoadoutUnlockCount", 1));
    if (getIntConfig(WorldIntConfigs::CONFIG_BATTLE_PET_LOADOUT_UNLOCK_COUNT) > BATTLE_PET_MAX_LOADOUT_SLOTS)
    {
        SF_LOG_ERROR("server.loading", "BattlePet.LoadoutUnlockCount (%i) can't be loaded. Set to 1.", getIntConfig(WorldIntConfigs::CONFIG_BATTLE_PET_LOADOUT_UNLOCK_COUNT));
        setIntConfig(WorldIntConfigs::CONFIG_BATTLE_PET_LOADOUT_UNLOCK_COUNT, 1);
    }

    setIntConfig(WorldIntConfigs::CONFIG_BATTLE_PET_INITIAL_LEVEL, sConfigMgr->GetIntDefault("BattlePet.InitialLevel", 1));
    if (getIntConfig(WorldIntConfigs::CONFIG_BATTLE_PET_INITIAL_LEVEL) > BATTLE_PET_MAX_LEVEL)
    {
        SF_LOG_ERROR("server.loading", "BattlePet.InitialLevel (%i) can't be loaded. Set to 1.", getIntConfig(WorldIntConfigs::CONFIG_BATTLE_PET_INITIAL_LEVEL));
        setIntConfig(WorldIntConfigs::CONFIG_BATTLE_PET_INITIAL_LEVEL, 1);
    }

    // blackmarket
    SetBoolConfig(WorldBoolConfigs::CONFIG_BLACK_MARKET_OPEN, sConfigMgr->GetBoolDefault("BlackMarket.Open", false));
    setIntConfig(WorldIntConfigs::CONFIG_BLACK_MARKET_MAX_AUCTIONS, sConfigMgr->GetIntDefault("BlackMarket.MaxAuctions", 12));
    // LOGIC ?
    //if (setIntConfig(WorldIntConfigs::CONFIG_BLACK_MARKET_MAX_AUCTIONS] > WorldIntConfigs::CONFIG_BLACK_MARKET_MAX_AUCTIONS)
    //   setIntConfig(WorldIntConfigs::CONFIG_BLACK_MARKET_MAX_AUCTIONS] = WorldIntConfigs::CONFIG_BLACK_MARKET_MAX_AUCTIONS;

    setIntConfig(WorldIntConfigs::CONFIG_BLACK_MARKET_AUCTION_DELAY, sConfigMgr->GetIntDefault("BlackMarket.AuctionDelay", 12));
    // LOGIC ?
    //if (m_int_configs(WorldIntConfigs::CONFIG_BLACK_MARKET_AUCTION_DELAY) > (WorldIntConfigs::CONFIG_BLACK_MARKET_AUCTION_DELAY)
    //    setIntConfig(WorldIntConfigs::CONFIG_BLACK_MARKET_AUCTION_DELAY] = WorldIntConfigs::CONFIG_BLACK_MARKET_AUCTION_DELAY;

    setIntConfig(WorldIntConfigs::CONFIG_BLACK_MARKET_AUCTION_DELAY_MOD, sConfigMgr->GetIntDefault("BlackMarket.AuctionDelayMod", 12));
    
    // LOGIC ?
    //if (m_int_configs(WorldIntConfigs::CONFIG_BLACK_MARKET_AUCTION_DELAY_MOD) > WorldIntConfigs::CONFIG_BLACK_MARKET_AUCTION_DELAY_MOD)
    //   setIntConfig(WorldIntConfigs::CONFIG_BLACK_MARKET_AUCTION_DELAY_MOD] = WorldIntConfigs::CONFIG_BLACK_MARKET_AUCTION_DELAY_MOD;
        
    // character boost
    SetBoolConfig(WorldBoolConfigs::CONFIG_BOOST_NEW_ACCOUNT, sConfigMgr->GetBoolDefault("Boost.NewAccounts", false));
    setIntConfig(WorldIntConfigs::CONFIG_BOOST_START_MONEY, sConfigMgr->GetIntDefault("Boost.StartMoney", 1500000));
    setIntConfig(WorldIntConfigs::CONFIG_BOOST_START_LEVEL, sConfigMgr->GetIntDefault("Boost.StartLevel", 90));

    //visibility on continents
    m_MaxVisibleDistanceOnContinents = sConfigMgr->GetFloatDefault("Visibility.Distance.Continents", DEFAULT_VISIBILITY_DISTANCE);
    if (m_MaxVisibleDistanceOnContinents < 45*sWorld->getRate(Rates::RATE_CREATURE_AGGRO))
    {
        SF_LOG_ERROR("server.loading", "Visibility.Distance.Continents can't be less max aggro radius %f", 45*sWorld->getRate(Rates::RATE_CREATURE_AGGRO));
        m_MaxVisibleDistanceOnContinents = 45*sWorld->getRate(Rates::RATE_CREATURE_AGGRO);
    }
    else if (m_MaxVisibleDistanceOnContinents > MAX_VISIBILITY_DISTANCE)
    {
        SF_LOG_ERROR("server.loading", "Visibility.Distance.Continents can't be greater %f", MAX_VISIBILITY_DISTANCE);
        m_MaxVisibleDistanceOnContinents = MAX_VISIBILITY_DISTANCE;
    }

    //visibility in instances
    m_MaxVisibleDistanceInInstances = sConfigMgr->GetFloatDefault("Visibility.Distance.Instances", DEFAULT_VISIBILITY_INSTANCE);
    if (m_MaxVisibleDistanceInInstances < 45*sWorld->getRate(Rates::RATE_CREATURE_AGGRO))
    {
        SF_LOG_ERROR("server.loading", "Visibility.Distance.Instances can't be less max aggro radius %f", 45*sWorld->getRate(Rates::RATE_CREATURE_AGGRO));
        m_MaxVisibleDistanceInInstances = 45*sWorld->getRate(Rates::RATE_CREATURE_AGGRO);
    }
    else if (m_MaxVisibleDistanceInInstances > MAX_VISIBILITY_DISTANCE)
    {
        SF_LOG_ERROR("server.loading", "Visibility.Distance.Instances can't be greater %f", MAX_VISIBILITY_DISTANCE);
        m_MaxVisibleDistanceInInstances = MAX_VISIBILITY_DISTANCE;
    }

    //visibility in BG/Arenas
    m_MaxVisibleDistanceInBGArenas = sConfigMgr->GetFloatDefault("Visibility.Distance.BGArenas", DEFAULT_VISIBILITY_BGARENAS);
    if (m_MaxVisibleDistanceInBGArenas < 45*sWorld->getRate(Rates::RATE_CREATURE_AGGRO))
    {
        SF_LOG_ERROR("server.loading", "Visibility.Distance.BGArenas can't be less max aggro radius %f", 45*sWorld->getRate(Rates::RATE_CREATURE_AGGRO));
        m_MaxVisibleDistanceInBGArenas = 45*sWorld->getRate(Rates::RATE_CREATURE_AGGRO);
    }
    else if (m_MaxVisibleDistanceInBGArenas > MAX_VISIBILITY_DISTANCE)
    {
        SF_LOG_ERROR("server.loading", "Visibility.Distance.BGArenas can't be greater %f", MAX_VISIBILITY_DISTANCE);
        m_MaxVisibleDistanceInBGArenas = MAX_VISIBILITY_DISTANCE;
    }

    m_visibility_notify_periodOnContinents = sConfigMgr->GetIntDefault("Visibility.Notify.Period.OnContinents", DEFAULT_VISIBILITY_NOTIFY_PERIOD);
    m_visibility_notify_periodInInstances = sConfigMgr->GetIntDefault("Visibility.Notify.Period.InInstances",   DEFAULT_VISIBILITY_NOTIFY_PERIOD);
    m_visibility_notify_periodInBGArenas = sConfigMgr->GetIntDefault("Visibility.Notify.Period.InBGArenas",    DEFAULT_VISIBILITY_NOTIFY_PERIOD);

    ///- Load the CharDelete related config options
    setIntConfig(WorldIntConfigs::CONFIG_CHARDELETE_METHOD, sConfigMgr->GetIntDefault("CharDelete.Method", 0));
    setIntConfig(WorldIntConfigs::CONFIG_CHARDELETE_MIN_LEVEL, sConfigMgr->GetIntDefault("CharDelete.MinLevel", 0));
    setIntConfig(WorldIntConfigs::CONFIG_CHARDELETE_HEROIC_MIN_LEVEL, sConfigMgr->GetIntDefault("CharDelete.Heroic.MinLevel", 0));
    setIntConfig(WorldIntConfigs::CONFIG_CHARDELETE_KEEP_DAYS, sConfigMgr->GetIntDefault("CharDelete.KeepDays", 30));

    ///- Read the "Data" directory from the config file
    std::string dataPath = sConfigMgr->GetStringDefault("DataDir", "./");
    if (dataPath.empty() || (dataPath.at(dataPath.length()-1) != '/' && dataPath.at(dataPath.length()-1) != '\\'))
        dataPath.push_back('/');

#if PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_APPLE
    if (dataPath[0] == '~')
    {
        const char* home = getenv("HOME");
        if (home)
            dataPath.replace(0, 1, home);
    }
#endif

    if (reload)
    {
        if (dataPath != m_dataPath)
            SF_LOG_ERROR("server.loading", "DataDir option can't be changed at worldserver.conf reload, using current value (%s).", m_dataPath.c_str());
    }
    else
    {
        m_dataPath = dataPath;
        SF_LOG_INFO("server.loading", "Using DataDir %s", m_dataPath.c_str());
    }

    SetBoolConfig(WorldBoolConfigs::CONFIG_ENABLE_MMAPS, sConfigMgr->GetBoolDefault("mmap.enablePathFinding", false));
    SF_LOG_INFO("server.loading", "WORLD: MMap data directory is: %smmaps", m_dataPath.c_str());

    SetBoolConfig(WorldBoolConfigs::CONFIG_VMAP_INDOOR_CHECK, sConfigMgr->GetBoolDefault("vmap.enableIndoorCheck", 0));
    bool enableIndoor = sConfigMgr->GetBoolDefault("vmap.enableIndoorCheck", true);
    bool enableLOS = sConfigMgr->GetBoolDefault("vmap.enableLOS", true);
    bool enableHeight = sConfigMgr->GetBoolDefault("vmap.enableHeight", true);

    if (!enableHeight)
        SF_LOG_ERROR("server.loading", "VMap height checking disabled! Creatures movements and other various things WILL be broken! Expect no support.");

    VMAP::VMapFactory::createOrGetVMapManager()->setEnableLineOfSightCalc(enableLOS);
    VMAP::VMapFactory::createOrGetVMapManager()->setEnableHeightCalc(enableHeight);
    SF_LOG_INFO("server.loading", "VMap support included. LineOfSight: %i, getHeight: %i, indoorCheck: %i", enableLOS, enableHeight, enableIndoor);
    SF_LOG_INFO("server.loading", "VMap data directory is: %svmaps", m_dataPath.c_str());

    setIntConfig(WorldIntConfigs::CONFIG_MAX_WHO, sConfigMgr->GetIntDefault("MaxWhoListReturns", 49));
    SetBoolConfig(WorldBoolConfigs::CONFIG_START_ALL_SPELLS, sConfigMgr->GetBoolDefault("PlayerStart.AllSpells", false));
    if (GetBoolConfig(WorldBoolConfigs::CONFIG_START_ALL_SPELLS))
        SF_LOG_WARN("server.loading", "PlayerStart.AllSpells enabled - may not function as intended!");
    setIntConfig(WorldIntConfigs::CONFIG_HONOR_AFTER_DUEL, sConfigMgr->GetIntDefault("HonorPointsAfterDuel", 0));
    SetBoolConfig(WorldBoolConfigs::CONFIG_START_ALL_EXPLORED, sConfigMgr->GetBoolDefault("PlayerStart.MapsExplored", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_START_ALL_REP, sConfigMgr->GetBoolDefault("PlayerStart.AllReputation", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_ALWAYS_MAXSKILL, sConfigMgr->GetBoolDefault("AlwaysMaxWeaponSkill", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_PVP_TOKEN_ENABLE, sConfigMgr->GetBoolDefault("PvPToken.Enable", false));
    setIntConfig(WorldIntConfigs::CONFIG_PVP_TOKEN_MAP_TYPE, sConfigMgr->GetIntDefault("PvPToken.MapAllowType", 4));
    setIntConfig(WorldIntConfigs::CONFIG_PVP_TOKEN_ID, sConfigMgr->GetIntDefault("PvPToken.ItemID", 29434));
    setIntConfig(WorldIntConfigs::CONFIG_PVP_TOKEN_COUNT, sConfigMgr->GetIntDefault("PvPToken.ItemCount", 1));
    if (getIntConfig(WorldIntConfigs::CONFIG_PVP_TOKEN_COUNT) < 1)
        setIntConfig(WorldIntConfigs::CONFIG_PVP_TOKEN_COUNT, 1);

    SetBoolConfig(WorldBoolConfigs::CONFIG_NO_RESET_TALENT_COST, sConfigMgr->GetBoolDefault("NoResetTalentsCost", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_SHOW_KICK_IN_WORLD, sConfigMgr->GetBoolDefault("ShowKickInWorld", false));
    setIntConfig(WorldIntConfigs::CONFIG_INTERVAL_LOG_UPDATE, sConfigMgr->GetIntDefault("RecordUpdateTimeDiffInterval", 60000));
    setIntConfig(WorldIntConfigs::CONFIG_MIN_LOG_UPDATE, sConfigMgr->GetIntDefault("MinRecordUpdateTimeDiff", 100));
    setIntConfig(WorldIntConfigs::CONFIG_NUMTHREADS, sConfigMgr->GetIntDefault("MapUpdate.Threads", 1));
    setIntConfig(WorldIntConfigs::CONFIG_MAX_RESULTS_LOOKUP_COMMANDS, sConfigMgr->GetIntDefault("Command.LookupMaxResults", 0));

    // chat logging
    SetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_CHANNEL, sConfigMgr->GetBoolDefault("ChatLogs.Channel", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_WHISPER, sConfigMgr->GetBoolDefault("ChatLogs.Whisper", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_SYSCHAN, sConfigMgr->GetBoolDefault("ChatLogs.SysChan", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_PARTY, sConfigMgr->GetBoolDefault("ChatLogs.Party", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_RAID, sConfigMgr->GetBoolDefault("ChatLogs.Raid", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_GUILD, sConfigMgr->GetBoolDefault("ChatLogs.Guild", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_PUBLIC, sConfigMgr->GetBoolDefault("ChatLogs.Public", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_ADDON, sConfigMgr->GetBoolDefault("ChatLogs.Addon", false));
    SetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_BGROUND, sConfigMgr->GetBoolDefault("ChatLogs.Battleground", false));

    // Warden
    SetBoolConfig(WorldBoolConfigs::CONFIG_WARDEN_ENABLED, sConfigMgr->GetBoolDefault("Warden.Enabled", false));
    setIntConfig(WorldIntConfigs::CONFIG_WARDEN_NUM_MEM_CHECKS, sConfigMgr->GetIntDefault("Warden.NumMemChecks", 3));
    setIntConfig(WorldIntConfigs::CONFIG_WARDEN_NUM_OTHER_CHECKS, sConfigMgr->GetIntDefault("Warden.NumOtherChecks", 7));
    setIntConfig(WorldIntConfigs::CONFIG_WARDEN_CLIENT_BAN_DURATION, sConfigMgr->GetIntDefault("Warden.BanDuration", 86400));
    setIntConfig(WorldIntConfigs::CONFIG_WARDEN_CLIENT_CHECK_HOLDOFF, sConfigMgr->GetIntDefault("Warden.ClientCheckHoldOff", 30));
    setIntConfig(WorldIntConfigs::CONFIG_WARDEN_CLIENT_FAIL_ACTION, sConfigMgr->GetIntDefault("Warden.ClientCheckFailAction", 0));
    setIntConfig(WorldIntConfigs::CONFIG_WARDEN_CLIENT_RESPONSE_DELAY, sConfigMgr->GetIntDefault("Warden.ClientResponseDelay", 600));

    // Dungeon finder
    setIntConfig(WorldIntConfigs::CONFIG_LFG_OPTIONSMASK, sConfigMgr->GetIntDefault("DungeonFinder.OptionsMask", 1));

    // DBC_ItemAttributes
    SetBoolConfig(WorldBoolConfigs::CONFIG_DBC_ENFORCE_ITEM_ATTRIBUTES, sConfigMgr->GetBoolDefault("DBC.EnforceItemAttributes", true));

    // Accountpassword Secruity
    setIntConfig(WorldIntConfigs::CONFIG_ACC_PASSCHANGESEC, sConfigMgr->GetIntDefault("Account.PasswordChangeSecurity", 0));

    // Rbac Free Permission mode
    setIntConfig(WorldIntConfigs::CONFIG_RBAC_FREE_PERMISSION_MODE, sConfigMgr->GetIntDefault("RBAC.FreePermissionMode", 0));

    // Random Battleground Rewards
    setIntConfig(WorldIntConfigs::CONFIG_BG_REWARD_WINNER_HONOR_FIRST, sConfigMgr->GetIntDefault("Battleground.RewardWinnerHonorFirst", 27000));
    setIntConfig(WorldIntConfigs::CONFIG_BG_REWARD_WINNER_CONQUEST_FIRST, sConfigMgr->GetIntDefault("Battleground.RewardWinnerConquestFirst", 10000));
    setIntConfig(WorldIntConfigs::CONFIG_BG_REWARD_WINNER_HONOR_LAST, sConfigMgr->GetIntDefault("Battleground.RewardWinnerHonorLast", 13500));
    setIntConfig(WorldIntConfigs::CONFIG_BG_REWARD_WINNER_CONQUEST_LAST, sConfigMgr->GetIntDefault("Battleground.RewardWinnerConquestLast", 5000));
    setIntConfig(WorldIntConfigs::CONFIG_BG_REWARD_LOSER_HONOR_FIRST, sConfigMgr->GetIntDefault("Battleground.RewardLoserHonorFirst", 4500));
    setIntConfig(WorldIntConfigs::CONFIG_BG_REWARD_LOSER_HONOR_LAST, sConfigMgr->GetIntDefault("Battleground.RewardLoserHonorLast", 3500));

    // Max instances per hour
    setIntConfig(WorldIntConfigs::CONFIG_MAX_INSTANCES_PER_HOUR, sConfigMgr->GetIntDefault("AccountInstancesPerHour", 5));

    // Anounce reset of instance to whole party
    SetBoolConfig(WorldBoolConfigs::CONFIG_INSTANCES_RESET_ANNOUNCE, sConfigMgr->GetBoolDefault("InstancesResetAnnounce", false));

    // AutoBroadcast
    SetBoolConfig(WorldBoolConfigs::CONFIG_AUTOBROADCAST, sConfigMgr->GetBoolDefault("AutoBroadcast.On", false));
    setIntConfig(WorldIntConfigs::CONFIG_AUTOBROADCAST_CENTER, sConfigMgr->GetIntDefault("AutoBroadcast.Center", 0));
    setIntConfig(WorldIntConfigs::CONFIG_AUTOBROADCAST_INTERVAL, sConfigMgr->GetIntDefault("AutoBroadcast.Timer", 60000));
    if (reload)
    {
        m_timers[WUPDATE_AUTOBROADCAST].SetInterval(getIntConfig(WorldIntConfigs::CONFIG_AUTOBROADCAST_INTERVAL));
        m_timers[WUPDATE_AUTOBROADCAST].Reset();
    }

    // MySQL ping time interval
    setIntConfig(WorldIntConfigs::CONFIG_DB_PING_INTERVAL, sConfigMgr->GetIntDefault("MaxPingTime", 30));

    // Guild save interval
    SetBoolConfig(WorldBoolConfigs::CONFIG_GUILD_LEVELING_ENABLED, sConfigMgr->GetBoolDefault("Guild.LevelingEnabled", true));
    setIntConfig(WorldIntConfigs::CONFIG_GUILD_SAVE_INTERVAL, sConfigMgr->GetIntDefault("Guild.SaveInterval", 15));
    setIntConfig(WorldIntConfigs::CONFIG_GUILD_MAX_LEVEL, sConfigMgr->GetIntDefault("Guild.MaxLevel", 25));
    setIntConfig(WorldIntConfigs::CONFIG_GUILD_UNDELETABLE_LEVEL, sConfigMgr->GetIntDefault("Guild.UndeletableLevel", 4));
    setRate(Rates::RATE_XP_GUILD_MODIFIER, sConfigMgr->GetFloatDefault("Guild.XPModifier", 0.25f));
    setIntConfig(WorldIntConfigs::CONFIG_GUILD_DAILY_XP_CAP, sConfigMgr->GetIntDefault("Guild.DailyXPCap", 7807500));
    setIntConfig(WorldIntConfigs::CONFIG_GUILD_WEEKLY_REP_CAP, sConfigMgr->GetIntDefault("Guild.WeeklyReputationCap", 4375));

    // misc
    SetBoolConfig(WorldBoolConfigs::CONFIG_PDUMP_NO_PATHS, sConfigMgr->GetBoolDefault("PlayerDump.DisallowPaths", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_PDUMP_NO_OVERWRITE, sConfigMgr->GetBoolDefault("PlayerDump.DisallowOverwrite", true));
    SetBoolConfig(WorldBoolConfigs::CONFIG_UI_QUESTLEVELS_IN_DIALOGS, sConfigMgr->GetBoolDefault("UI.ShowQuestLevelsInDialogs", false));

    // Wintergrasp battlefield
    SetBoolConfig(WorldBoolConfigs::CONFIG_WINTERGRASP_ENABLE, sConfigMgr->GetBoolDefault("Wintergrasp.Enable", false));
    setIntConfig(WorldIntConfigs::CONFIG_WINTERGRASP_PLR_MAX, sConfigMgr->GetIntDefault("Wintergrasp.PlayerMax", 100));
    setIntConfig(WorldIntConfigs::CONFIG_WINTERGRASP_PLR_MIN, sConfigMgr->GetIntDefault("Wintergrasp.PlayerMin", 0));
    setIntConfig(WorldIntConfigs::CONFIG_WINTERGRASP_PLR_MIN_LVL, sConfigMgr->GetIntDefault("Wintergrasp.PlayerMinLvl", 77));
    setIntConfig(WorldIntConfigs::CONFIG_WINTERGRASP_BATTLETIME, sConfigMgr->GetIntDefault("Wintergrasp.BattleTimer", 30));
    setIntConfig(WorldIntConfigs::CONFIG_WINTERGRASP_NOBATTLETIME, sConfigMgr->GetIntDefault("Wintergrasp.NoBattleTimer", 150));
    setIntConfig(WorldIntConfigs::CONFIG_WINTERGRASP_RESTART_AFTER_CRASH, sConfigMgr->GetIntDefault("Wintergrasp.CrashRestartTimer", 10));

    // Stats limits
    SetBoolConfig(WorldBoolConfigs::CONFIG_STATS_LIMITS_ENABLE, sConfigMgr->GetBoolDefault("Stats.Limits.Enable", false));
    SetFloatConfig(WorldFloatConfigs::CONFIG_STATS_LIMITS_DODGE, sConfigMgr->GetFloatDefault("Stats.Limits.Dodge", 95.0f));
    SetFloatConfig(WorldFloatConfigs::CONFIG_STATS_LIMITS_PARRY, sConfigMgr->GetFloatDefault("Stats.Limits.Parry", 95.0f));
    SetFloatConfig(WorldFloatConfigs::CONFIG_STATS_LIMITS_BLOCK, sConfigMgr->GetFloatDefault("Stats.Limits.Block", 95.0f));
    SetFloatConfig(WorldFloatConfigs::CONFIG_STATS_LIMITS_CRIT, sConfigMgr->GetFloatDefault("Stats.Limits.Crit", 95.0f));

    //packet spoof punishment
    setIntConfig(WorldIntConfigs::CONFIG_PACKET_SPOOF_POLICY, sConfigMgr->GetIntDefault("PacketSpoof.Policy", (uint32)WorldSession::DosProtection::POLICY_KICK));
    setIntConfig(WorldIntConfigs::CONFIG_PACKET_SPOOF_BANMODE, sConfigMgr->GetIntDefault("PacketSpoof.BanMode", (uint32)BAN_ACCOUNT));
    if (getIntConfig(WorldIntConfigs::CONFIG_PACKET_SPOOF_BANMODE) == BAN_CHARACTER || getIntConfig(WorldIntConfigs::CONFIG_PACKET_SPOOF_BANMODE) > BAN_IP)
        setIntConfig(WorldIntConfigs::CONFIG_PACKET_SPOOF_BANMODE, BAN_ACCOUNT);

    setIntConfig(WorldIntConfigs::CONFIG_PACKET_SPOOF_BANDURATION, sConfigMgr->GetIntDefault("PacketSpoof.BanDuration", 86400));

    // call ScriptMgr if we're reloading the configuration
    if (reload)
        sScriptMgr->OnConfigLoad(reload);
}

extern void LoadGameObjectModelList(std::string const& dataPath);

/// Initialize the World
void World::SetInitialWorldSettings()
{
    ///- Server startup begin
    uint32 startupBegin = getMSTime();

    ///- Initialize the random number generator
    srand((unsigned int)time(NULL));

    ///- Initialize detour memory management
    dtAllocSetCustom(dtCustomAlloc, dtCustomFree);

    ///- Initialize config settings
    LoadConfigSettings();

    ///- Initialize Allowed Security Level
    LoadDBAllowedSecurityLevel();

    ///- Init highest guids before any table loading to prevent using not initialized guids in some code.
    sObjectMgr->SetHighestGuids();

    ///- Check the existence of the map files for all races' startup areas.
    if (!MapManager::ExistMapAndVMap(0, -6240.32f, 331.033f)
        || !MapManager::ExistMapAndVMap(0, -8949.95f, -132.493f)
        || !MapManager::ExistMapAndVMap(1, -618.518f, -4251.67f)
        || !MapManager::ExistMapAndVMap(0, 1676.35f, 1677.45f)
        || !MapManager::ExistMapAndVMap(1, 10311.3f, 832.463f)
        || !MapManager::ExistMapAndVMap(1, -2917.58f, -257.98f)
        || (getIntConfig(WorldIntConfigs::CONFIG_EXPANSION) > 0 && (
           !MapManager::ExistMapAndVMap(530, 10349.6f, -6357.29f)
        || !MapManager::ExistMapAndVMap(530, -3961.64f, -13931.2f)
        || !MapManager::ExistMapAndVMap(648, -8423.81f, 1361.3f)
        || !MapManager::ExistMapAndVMap(654, -1451.53f, 1403.35f)
        || !MapManager::ExistMapAndVMap(609, 2356.21f, -5662.21f)
        || !MapManager::ExistMapAndVMap(860, 1471.67f, 3466.25f))))
    {
        SF_LOG_ERROR("server.loading", "Correct *.map files not found in path '%smaps' or *.vmtree/*.vmtile files in '%svmaps'. Please place *.map/*.vmtree/*.vmtile files in appropriate directories or correct the DataDir value in the worldserver.conf file.", m_dataPath.c_str(), m_dataPath.c_str());
        exit(1);
    }

    ///- Initialize pool manager
    sPoolMgr->Initialize();

    ///- Initialize game event manager
    sGameEventMgr->Initialize();

    ///- Loading strings. Getting no records means core load has to be canceled because no error message can be output.

    SF_LOG_INFO("server.loading", "Loading Skyfire strings...");
    if (!sObjectMgr->LoadSkyFireStrings())
        exit(1);                                            // Error message displayed in function already

    ///- Update the realm entry in the database with the realm type from the config file
    //No SQL injection as values are treated as integers

    // not send custom type REALM_FFA_PVP to realm list
    uint32 server_type = IsFFAPvPRealm() ? uint32(REALM_TYPE_PVP) : getIntConfig(WorldIntConfigs::CONFIG_GAME_TYPE);
    uint32 realm_zone = getIntConfig(WorldIntConfigs::CONFIG_REALM_ZONE);

    LoginDatabase.PExecute("UPDATE realmlist SET icon = %u, timezone = %u WHERE id = '%d'", server_type, realm_zone, realmID);      // One-time query

    ///- Remove the bones (they should not exist in DB though) and old corpses after a restart
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_OLD_CORPSES);
    stmt->setUInt32(0, 3 * DAY);
    CharacterDatabase.Execute(stmt);

    ///- Load the DBC files
    SF_LOG_INFO("server.loading", "Initialize data stores...");
    LoadDBCStores(m_dataPath);
    LoadDB2Stores(m_dataPath);

    SF_LOG_INFO("server.loading", "Loading SpellInfo store...");
    sSpellMgr->LoadSpellInfoStore();

    SF_LOG_INFO("server.loading", "Loading SpellInfo corrections...");
    sSpellMgr->LoadSpellInfoCorrections();

    SF_LOG_INFO("server.loading", "Loading SkillLineAbilityMultiMap Data...");
    sSpellMgr->LoadSkillLineAbilityMap();

    SF_LOG_INFO("server.loading", "Loading SpellInfo custom attributes...");
    sSpellMgr->LoadSpellInfoCustomAttributes();

    SF_LOG_INFO("server.loading", "Loading GameObject models...");
    LoadGameObjectModelList(m_dataPath);

    SF_LOG_INFO("server.loading", "Loading Script Names...");
    sObjectMgr->LoadScriptNames();

    SF_LOG_INFO("server.loading", "Loading Instance Template...");
    sObjectMgr->LoadInstanceTemplate();

    // Must be called before `creature_respawn`/`gameobject_respawn` tables
    SF_LOG_INFO("server.loading", "Loading instances...");
    sInstanceSaveMgr->LoadInstances();

    SF_LOG_INFO("server.loading", "Loading Localization strings...");
    uint32 oldMSTime = getMSTime();
    sObjectMgr->LoadCreatureLocales();
    sObjectMgr->LoadGameObjectLocales();
    sObjectMgr->LoadItemLocales();
    sObjectMgr->LoadQuestLocales();
    sObjectMgr->LoadNpcTextLocales();
    sObjectMgr->LoadPageTextLocales();
    sObjectMgr->LoadGossipMenuItemsLocales();
    sObjectMgr->LoadPointOfInterestLocales();

    sObjectMgr->SetDBCLocaleIndex(GetDefaultDbcLocale());        // Get once for all the locale index of DBC language (console/broadcasts)
    SF_LOG_INFO("server.loading", ">> Localization strings loaded in %u ms", GetMSTimeDiffToNow(oldMSTime));

    SF_LOG_INFO("server.loading", "Loading Account Roles and Permissions...");
    sAccountMgr->LoadRBAC();

    SF_LOG_INFO("server.loading", "Loading Page Texts...");
    sObjectMgr->LoadPageTexts();

    SF_LOG_INFO("server.loading", "Loading Game Object Templates...");         // must be after LoadPageTexts
    sObjectMgr->LoadGameObjectTemplate();

    SF_LOG_INFO("server.loading", "Loading Transport templates...");
    sTransportMgr->LoadTransportTemplates();

    SF_LOG_INFO("server.loading", "Loading Spell Rank Data...");
    sSpellMgr->LoadSpellRanks();

    SF_LOG_INFO("server.loading", "Loading Spell Required Data...");
    sSpellMgr->LoadSpellRequired();

    SF_LOG_INFO("server.loading", "Loading Spell Group types...");
    sSpellMgr->LoadSpellGroups();

    SF_LOG_INFO("server.loading", "Loading Spell Learn Skills...");
    sSpellMgr->LoadSpellLearnSkills();                           // must be after LoadSpellRanks

    SF_LOG_INFO("server.loading", "Loading Spell Learn Spells...");
    sSpellMgr->LoadSpellLearnSpells();

    SF_LOG_INFO("server.loading", "Loading Spell Proc Event conditions...");
    sSpellMgr->LoadSpellProcEvents();

    SF_LOG_INFO("server.loading", "Loading Spell Proc conditions and data...");
    sSpellMgr->LoadSpellProcs();

    SF_LOG_INFO("server.loading", "Loading Spell Bonus Data...");
    sSpellMgr->LoadSpellBonusess();

    SF_LOG_INFO("server.loading", "Loading Aggro Spells Definitions...");
    sSpellMgr->LoadSpellThreats();

    SF_LOG_INFO("server.loading", "Loading Spell Group Stack Rules...");
    sSpellMgr->LoadSpellGroupStackRules();

    SF_LOG_INFO("server.loading", "Loading NPC Texts...");
    sObjectMgr->LoadGossipText();

    SF_LOG_INFO("server.loading", "Loading Enchant Spells Proc datas...");
    sSpellMgr->LoadSpellEnchantProcData();

    SF_LOG_INFO("server.loading", "Loading Item Random Enchantments Table...");
    LoadRandomEnchantmentsTable();

    SF_LOG_INFO("server.loading", "Loading Disables");                         // must be before loading quests and items
    DisableMgr::LoadDisables();

    SF_LOG_INFO("server.loading", "Loading Items...");                         // must be after LoadRandomEnchantmentsTable and LoadPageTexts
    sObjectMgr->LoadItemTemplates();

    SF_LOG_INFO("server.loading", "Loading Item set names...");                // must be after LoadItemPrototypes
    sObjectMgr->LoadItemTemplateAddon();

    SF_LOG_INFO("misc", "Loading Item Scripts...");                 // must be after LoadItemPrototypes
    sObjectMgr->LoadItemScriptNames();

    SF_LOG_INFO("server.loading", "Loading Creature Model Based Info Data...");
    sObjectMgr->LoadCreatureModelInfo();

    SF_LOG_INFO("server.loading", "Loading Creature templates...");
    sObjectMgr->LoadCreatureTemplates();

    SF_LOG_INFO("server.loading", "Loading Equipment templates...");           // must be after LoadCreatureTemplates
    sObjectMgr->LoadEquipmentTemplates();

    SF_LOG_INFO("server.loading", "Loading Creature template addons...");
    sObjectMgr->LoadCreatureTemplateAddons();

    SF_LOG_INFO("server.loading", "Loading Reputation Reward Rates...");
    sObjectMgr->LoadReputationRewardRate();

    SF_LOG_INFO("server.loading", "Loading Creature Reputation OnKill Data...");
    sObjectMgr->LoadReputationOnKill();

    SF_LOG_INFO("server.loading", "Loading Reputation Spillover Data...");
    sObjectMgr->LoadReputationSpilloverTemplate();

    SF_LOG_INFO("server.loading", "Loading Points Of Interest Data...");
    sObjectMgr->LoadPointsOfInterest();

    SF_LOG_INFO("server.loading", "Loading Creature Base Stats...");
    sObjectMgr->LoadCreatureClassLevelStats();

    SF_LOG_INFO("server.loading", "Loading Creature Data...");
    sObjectMgr->LoadCreatures();

    SF_LOG_INFO("server.loading", "Loading Temporary Summon Data...");
    sObjectMgr->LoadTempSummons();                               // must be after LoadCreatureTemplates() and LoadGameObjectTemplates()

    SF_LOG_INFO("server.loading", "Loading pet levelup spells...");
    sSpellMgr->LoadPetLevelupSpellMap();

    SF_LOG_INFO("server.loading", "Loading pet default spells additional to levelup spells...");
    sSpellMgr->LoadPetDefaultSpells();

    SF_LOG_INFO("server.loading", "Loading Creature Addon Data...");
    sObjectMgr->LoadCreatureAddons();                            // must be after LoadCreatureTemplates() and LoadCreatures()

    SF_LOG_INFO("server.loading", "Loading Gameobject Data...");
    sObjectMgr->LoadGameobjects();

    SF_LOG_INFO("server.loading", "Loading Creature Linked Respawn...");
    sObjectMgr->LoadLinkedRespawn();                             // must be after LoadCreatures(), LoadGameObjects()

    SF_LOG_INFO("server.loading", "Loading Weather Data...");
    WeatherMgr::LoadWeatherData();

    SF_LOG_INFO("server.loading", "Loading SceneTemplate Data..");
    sObjectMgr->LoadSceneTemplates();

    SF_LOG_INFO("server.loading", "Loading Quests...");
    sObjectMgr->LoadQuests();                                    // must be loaded after DBCs, creature_template, item_template, gameobject tables

    SF_LOG_INFO("server.loading", "Checking Quest Disables");
    DisableMgr::CheckQuestDisables();                           // must be after loading quests

    SF_LOG_INFO("server.loading", "Loading Quest Objectives...");
    sObjectMgr->LoadQuestObjectives();

    SF_LOG_INFO("server.loading", "Loading Quest Objective Locales...");
    sObjectMgr->LoadQuestObjectiveLocales();

    SF_LOG_INFO("server.loading", "Loading Quest Objective Visual Effects...");
    sObjectMgr->LoadQuestObjectiveVisualEffects();

    SF_LOG_INFO("server.loading", "Loading Quest POI");
    sObjectMgr->LoadQuestPOI();

    SF_LOG_INFO("server.loading", "Loading Quests Starters and Enders...");
    sObjectMgr->LoadQuestStartersAndEnders();                    // must be after quest load

    SF_LOG_INFO("server.loading", "Loading Objects Pooling Data...");
    sPoolMgr->LoadFromDB();

    SF_LOG_INFO("server.loading", "Loading Game Event Data...");               // must be after loading pools fully
    sGameEventMgr->LoadFromDB();

    SF_LOG_INFO("server.loading", "Loading UNIT_NPC_FLAG_SPELLCLICK Data..."); // must be after LoadQuests
    sObjectMgr->LoadNPCSpellClickSpells();

    SF_LOG_INFO("server.loading", "Loading Vehicle Template Accessories...");
    sObjectMgr->LoadVehicleTemplateAccessories();                // must be after LoadCreatureTemplates() and LoadNPCSpellClickSpells()

    SF_LOG_INFO("server.loading", "Loading Vehicle Accessories...");
    sObjectMgr->LoadVehicleAccessories();                       // must be after LoadCreatureTemplates() and LoadNPCSpellClickSpells()

    SF_LOG_INFO("server.loading", "Loading SpellArea Data...");                // must be after quest load
    sSpellMgr->LoadSpellAreas();

    SF_LOG_INFO("server.loading", "Loading AreaTrigger definitions...");
    sObjectMgr->LoadAreaTriggerTeleports();

    SF_LOG_INFO("server.loading", "Loading Access Requirements...");
    sObjectMgr->LoadAccessRequirements();                        // must be after item template load

    SF_LOG_INFO("server.loading", "Loading Quest Area Triggers...");
    sObjectMgr->LoadQuestAreaTriggers();                         // must be after LoadQuests

    SF_LOG_INFO("server.loading", "Loading Tavern Area Triggers...");
    sObjectMgr->LoadTavernAreaTriggers();

    SF_LOG_INFO("server.loading", "Loading AreaTrigger script names...");
    sObjectMgr->LoadAreaTriggerScripts();

    SF_LOG_INFO("server.loading", "Loading LFG entrance positions..."); // Must be after areatriggers
    sLFGMgr->LoadLFGDungeons();

    SF_LOG_INFO("server.loading", "Loading Dungeon boss data...");
    sObjectMgr->LoadInstanceEncounters();

    SF_LOG_INFO("server.loading", "Loading LFG rewards...");
    sLFGMgr->LoadRewards();

    SF_LOG_INFO("server.loading", "Loading Graveyard-zone links...");
    sObjectMgr->LoadGraveyardZones();

    SF_LOG_INFO("server.loading", "Loading Graveyard Orientations...");
    sObjectMgr->LoadGraveyardOrientations();

    SF_LOG_INFO("server.loading", "Loading spell pet auras...");
    sSpellMgr->LoadSpellPetAuras();

    SF_LOG_INFO("server.loading", "Loading Spell target coordinates...");
    sSpellMgr->LoadSpellTargetPositions();

    SF_LOG_INFO("server.loading", "Loading enchant custom attributes...");
    sSpellMgr->LoadEnchantCustomAttr();

    SF_LOG_INFO("server.loading", "Loading linked spells...");
    sSpellMgr->LoadSpellLinked();

    SF_LOG_INFO("server.loading", "Loading Player Create Data...");
    sObjectMgr->LoadPlayerInfo();

    SF_LOG_INFO("server.loading", "Loading Exploration BaseXP Data...");
    sObjectMgr->LoadExplorationBaseXP();

    SF_LOG_INFO("server.loading", "Loading Pet Name Parts...");
    sObjectMgr->LoadPetNames();

    CharacterDatabaseCleaner::CleanDatabase();

    SF_LOG_INFO("server.loading", "Loading the max pet number...");
    sObjectMgr->LoadPetNumber();

    SF_LOG_INFO("server.loading", "Loading pet level stats...");
    sObjectMgr->LoadPetLevelInfo();

    SF_LOG_INFO("server.loading", "Loading Player Corpses...");
    sObjectMgr->LoadCorpses();

    SF_LOG_INFO("server.loading", "Loading Player level dependent mail rewards...");
    sObjectMgr->LoadMailLevelRewards();

    // Loot tables
    LoadLootTables();

    SF_LOG_INFO("server.loading", "Loading Skill Discovery Table...");
    LoadSkillDiscoveryTable();

    SF_LOG_INFO("server.loading", "Loading Skill Extra Item Table...");
    LoadSkillExtraItemTable();

    SF_LOG_INFO("server.loading", "Loading Skill Fishing base level requirements...");
    sObjectMgr->LoadFishingBaseSkillLevel();

    SF_LOG_INFO("server.loading", "Loading Achievements...");
    sAchievementMgr->LoadAchievementReferenceList();
    SF_LOG_INFO("server.loading", "Loading Achievement Criteria Lists...");
    sAchievementMgr->LoadAchievementCriteriaList();
    SF_LOG_INFO("server.loading", "Loading Achievement Criteria Data...");
    sAchievementMgr->LoadAchievementCriteriaData();
    SF_LOG_INFO("server.loading", "Loading Achievement Rewards...");
    sAchievementMgr->LoadRewards();
    SF_LOG_INFO("server.loading", "Loading Achievement Reward Locales...");
    sAchievementMgr->LoadRewardLocales();
    SF_LOG_INFO("server.loading", "Loading Completed Achievements...");
    sAchievementMgr->LoadCompletedAchievements();

    // Delete expired auctions before loading
    SF_LOG_INFO("server.loading", "Deleting expired auctions...");
    sAuctionMgr->DeleteExpiredAuctionsAtStartup();

    ///- Load dynamic data tables from the database
    SF_LOG_INFO("server.loading", "Loading Item Auctions...");
    sAuctionMgr->LoadAuctionItems();

    SF_LOG_INFO("server.loading", "Loading Auctions...");
    sAuctionMgr->LoadAuctions();

    SF_LOG_INFO("server.loading", "Loading Guild XP for level...");
    sGuildMgr->LoadGuildXpForLevel();

    SF_LOG_INFO("server.loading", "Loading Guild rewards...");
    sGuildMgr->LoadGuildRewards();

    SF_LOG_INFO("server.loading", "Loading Guilds...");
    sGuildMgr->LoadGuilds();

    sGuildFinderMgr->LoadFromDB();

    SF_LOG_INFO("server.loading", "Loading Groups...");
    sGroupMgr->LoadGroups();

    SF_LOG_INFO("server.loading", "Loading ReservedNames...");
    sObjectMgr->LoadReservedPlayersNames();

    SF_LOG_INFO("server.loading", "Loading GameObjects for quests...");
    sObjectMgr->LoadGameObjectForQuests();

    SF_LOG_INFO("server.loading", "Loading BattleMasters...");
    sBattlegroundMgr->LoadBattleMastersEntry();

    SF_LOG_INFO("server.loading", "Loading GameTeleports...");
    sObjectMgr->LoadGameTele();

    SF_LOG_INFO("server.loading", "Loading Gossip menu...");
    sObjectMgr->LoadGossipMenu();

    SF_LOG_INFO("server.loading", "Loading Gossip menu options...");
    sObjectMgr->LoadGossipMenuItems();

    SF_LOG_INFO("server.loading", "Loading Vendors...");
    sObjectMgr->LoadVendors();                                   // must be after load CreatureTemplate and ItemTemplate

    SF_LOG_INFO("server.loading", "Loading Trainers...");
    sObjectMgr->LoadTrainerSpell();                              // must be after load CreatureTemplate

    SF_LOG_INFO("server.loading", "Loading Waypoints...");
    sWaypointMgr->Load();

    SF_LOG_INFO("server.loading", "Loading SmartAI Waypoints...");
    sSmartWaypointMgr->LoadFromDB();

    SF_LOG_INFO("server.loading", "Loading Creature Formations...");
    sFormationMgr->LoadCreatureFormations();

    SF_LOG_INFO("server.loading", "Loading World States...");              // must be loaded before battleground, outdoor PvP and conditions
    LoadWorldStates();

    SF_LOG_INFO("server.loading", "Loading Terrain Phase definitions...");
    sObjectMgr->LoadTerrainPhaseInfo();

    SF_LOG_INFO("server.loading", "Loading Terrain Swap Default definitions...");
    sObjectMgr->LoadTerrainSwapDefaults();

    SF_LOG_INFO("server.loading", "Loading Terrain World Map definitions...");
    sObjectMgr->LoadTerrainWorldMaps();

    SF_LOG_INFO("server.loading", "Loading Phase Area definitions...");
    sObjectMgr->LoadAreaPhases();

    SF_LOG_INFO("server.loading", "Loading Conditions...");
    sConditionMgr->LoadConditions();

    SF_LOG_INFO("server.loading", "Loading faction change achievement pairs...");
    sObjectMgr->LoadFactionChangeAchievements();

    SF_LOG_INFO("server.loading", "Loading faction change spell pairs...");
    sObjectMgr->LoadFactionChangeSpells();

    SF_LOG_INFO("server.loading", "Loading faction change item pairs...");
    sObjectMgr->LoadFactionChangeItems();

    SF_LOG_INFO("server.loading", "Loading faction change reputation pairs...");
    sObjectMgr->LoadFactionChangeReputations();

    SF_LOG_INFO("server.loading", "Loading faction change title pairs...");
    sObjectMgr->LoadFactionChangeTitles();

    SF_LOG_INFO("server.loading", "Loading GM tickets...");
    sTicketMgr->LoadGmTickets();

    SF_LOG_INFO("server.loading", "Loading Support bugs tickets...");
    sTicketMgr->LoadBugTickets();

    SF_LOG_INFO("server.loading", "Loading Support suggest tickets...");
    sTicketMgr->LoadSuggestTickets();

    SF_LOG_INFO("server.loading", "Loading client addons...");
    AddonMgr::LoadFromDB();

    ///- Handle outdated emails (delete/return)
    SF_LOG_INFO("server.loading", "Returning old mails...");
    sObjectMgr->ReturnOrDeleteOldMails(false);

    SF_LOG_INFO("server.loading", "Loading Autobroadcasts...");
    LoadAutobroadcasts();

    ///- Load and initialize scripts
    sObjectMgr->LoadSpellScripts();                              // must be after load Creature/Gameobject(Template/Data)
    sObjectMgr->LoadEventScripts();                              // must be after load Creature/Gameobject(Template/Data)
    sObjectMgr->LoadWaypointScripts();

    SF_LOG_INFO("server.loading", "Loading Scripts text locales...");      // must be after Load*Scripts calls
    sObjectMgr->LoadDbScriptStrings();

    SF_LOG_INFO("server.loading", "Loading spell script names...");
    sObjectMgr->LoadSpellScriptNames();

    SF_LOG_INFO("server.loading", "Loading Creature Texts...");
    sCreatureTextMgr->LoadCreatureTexts();

    SF_LOG_INFO("server.loading", "Loading Creature Text Locales...");
    sCreatureTextMgr->LoadCreatureTextLocales();

    SF_LOG_INFO("server.loading", "Initializing Scripts...");
    sScriptMgr->Initialize();
    sScriptMgr->OnConfigLoad(false);                                // must be done after the ScriptMgr has been properly initialized

    SF_LOG_INFO("server.loading", "Validating spell scripts...");
    sObjectMgr->ValidateSpellScripts();

    SF_LOG_INFO("server.loading", "Loading SmartAI scripts...");
    sSmartScriptMgr->LoadSmartAIFromDB();

    SF_LOG_INFO("server.loading", "Loading Calendar data...");
    sCalendarMgr->LoadFromDB();

    //SF_LOG_INFO("server.loading", "Loading Research Digsite info...");
    //sObjectMgr->LoadResearchDigsiteInfo();

    //SF_LOG_INFO("server.loading", "Loading Archaeology Find info...");
    //sObjectMgr->LoadArchaeologyFindInfo();

    //SF_LOG_INFO("server.loading", "Loading Research Project requirements...");
    //sObjectMgr->LoadResearchProjectRequirements();

    SF_LOG_INFO("server.loading", "Loading Battle Pet breed data...");
    sObjectMgr->LoadBattlePetBreedData();

    SF_LOG_INFO("server.loading", "Loading Battle Pet quality data...");
    sObjectMgr->LoadBattlePetQualityData();

    ///- Initialize game time and timers
    SF_LOG_INFO("server.loading", "Initialize game time and timers");
    m_gameTime = time(NULL);
    m_startTime = m_gameTime;

    LoginDatabase.PExecute("INSERT INTO uptime (realmid, starttime, uptime, revision) VALUES(%u, %u, 0, '%s')",
                            realmID, uint32(m_startTime), _FULLVERSION);       // One-time query

    m_timers[WUPDATE_WEATHERS].SetInterval(1*IN_MILLISECONDS);
    m_timers[WUPDATE_AUCTIONS].SetInterval(MINUTE*IN_MILLISECONDS);
    m_timers[WUPDATE_BLACK_MARKET].SetInterval(MINUTE*IN_MILLISECONDS);
    m_timers[WUPDATE_UPTIME].SetInterval(getIntConfig(WorldIntConfigs::CONFIG_UPTIME_UPDATE)*MINUTE*IN_MILLISECONDS);
                                                            //Update "uptime" table based on configuration entry in minutes.
    m_timers[WUPDATE_CORPSES].SetInterval(20 * MINUTE * IN_MILLISECONDS);
                                                            //erase corpses every 20 minutes
    m_timers[WUPDATE_CLEANDB].SetInterval(getIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARINTERVAL)*MINUTE*IN_MILLISECONDS);
                                                            // clean logs table every 14 days by default
    m_timers[WUPDATE_AUTOBROADCAST].SetInterval(getIntConfig(WorldIntConfigs::CONFIG_AUTOBROADCAST_INTERVAL));
    m_timers[WUPDATE_DELETECHARS].SetInterval(DAY*IN_MILLISECONDS); // check for chars to delete every day

    m_timers[WUPDATE_PINGDB].SetInterval(getIntConfig(WorldIntConfigs::CONFIG_DB_PING_INTERVAL)*MINUTE*IN_MILLISECONDS);    // Mysql ping time in minutes

    m_timers[WUPDATE_GUILDSAVE].SetInterval(getIntConfig(WorldIntConfigs::CONFIG_GUILD_SAVE_INTERVAL) * MINUTE * IN_MILLISECONDS);

    //to set mailtimer to return mails every day between 4 and 5 am
    //mailtimer is increased when updating auctions
    //one second is 1000 -(tested on win system)
    /// @todo Get rid of magic numbers
    tm localTm;
    ACE_OS::localtime_r(&m_gameTime, &localTm);
    mail_timer = ((((localTm.tm_hour + 20) % 24)* HOUR * IN_MILLISECONDS) / m_timers[WUPDATE_AUCTIONS].GetInterval());
                                                            //1440
    mail_timer_expires = ((DAY * IN_MILLISECONDS) / (m_timers[WUPDATE_AUCTIONS].GetInterval()));
    SF_LOG_INFO("server.loading", "Mail timer set to: " UI64FMTD ", mail return is called every " UI64FMTD " minutes", uint64(mail_timer), uint64(mail_timer_expires));

    ///- Initilize static helper structures
    AIRegistry::Initialize();

    ///- Initialize MapManager
    SF_LOG_INFO("server.loading", "Starting Map System");
    sMapMgr->Initialize();

    SF_LOG_INFO("server.loading", "Starting Game Event system...");
    uint32 nextGameEvent = sGameEventMgr->StartSystem();
    m_timers[WUPDATE_EVENTS].SetInterval(nextGameEvent);    //depend on next event

    // Delete all characters which have been deleted X days before
    Player::DeleteOldCharacters();

    // Delete all custom channels which haven't been used for PreserveCustomChannelDuration days.
    Channel::CleanOldChannelsInDB();

    SF_LOG_INFO("server.loading", "Starting Arena Season...");
    sGameEventMgr->StartArenaSeason();

    sTicketMgr->Initialize();

    ///- Initialize Battlegrounds
    SF_LOG_INFO("server.loading", "Starting Battleground System");
    sBattlegroundMgr->CreateInitialBattlegrounds();

    ///- Initialize outdoor pvp
    SF_LOG_INFO("server.loading", "Starting Outdoor PvP System");
    sOutdoorPvPMgr->InitOutdoorPvP();

    ///- Initialize Battlefield
    SF_LOG_INFO("server.loading", "Starting Battlefield System");
    sBattlefieldMgr->InitBattlefield();

    SF_LOG_INFO("server.loading", "Loading Transports...");
    sTransportMgr->SpawnContinentTransports();

    ///- Initialize Warden
    SF_LOG_INFO("server.loading", "Loading Warden Checks...");
    sWardenCheckMgr->LoadWardenChecks();

    SF_LOG_INFO("server.loading", "Loading Warden Action Overrides...");
    sWardenCheckMgr->LoadWardenOverrides();

    SF_LOG_INFO("server.loading", "Deleting expired bans...");
    LoginDatabase.Execute("DELETE FROM ip_banned WHERE unbandate <= UNIX_TIMESTAMP() AND unbandate<>bandate");      // One-time query

    SF_LOG_INFO("server.loading", "Calculate next daily quest reset time...");
    InitDailyQuestResetTime();

    SF_LOG_INFO("server.loading", "Calculate next weekly quest reset time...");
    InitWeeklyQuestResetTime();

    SF_LOG_INFO("server.loading", "Calculate next monthly quest reset time...");
    InitMonthlyQuestResetTime();

    SF_LOG_INFO("server.loading", "Calculate random battleground reset time...");
    InitRandomBGResetTime();

    SF_LOG_INFO("server.loading", "Calculate guild limitation(s) reset time...");
    InitGuildResetTime();

    SF_LOG_INFO("server.loading", "Calculate next currency reset time...");
    InitCurrencyResetTime();

    LoadCharacterNameData();

    SF_LOG_INFO("misc", "Initializing Opcodes...");
    serverOpcodeTable.InitializeServerTable();
    clientOpcodeTable.InitializeClientTable();

    SF_LOG_INFO("misc", "Loading hotfix info...");
    sObjectMgr->LoadHotfixData();

    SF_LOG_INFO("server.loading", "Loading BlackMarket Templates...");
    sBlackMarketMgr->LoadBlackMarketTemplates();

    SF_LOG_INFO("server.loading", "Loading missing KeyChains...");
    sObjectMgr->LoadMissingKeyChains();

    uint32 startupDuration = GetMSTimeDiffToNow(startupBegin);

    SF_LOG_INFO("server.worldserver", "World initialized in %u minutes %u seconds", (startupDuration / 60000), ((startupDuration % 60000) / 1000));

    if (uint32 realmId = sConfigMgr->GetIntDefault("RealmID", 0)) // 0 reserved for auth
        sLog->SetRealmId(realmId);
}

void World::RecordTimeDiff(const char *text, ...)
{
    if (m_updateTimeCount != 1)
        return;
    if (!text)
    {
        m_currentTime = getMSTime();
        return;
    }

    uint32 thisTime = getMSTime();
    uint32 diff = getMSTimeDiff(m_currentTime, thisTime);

    if (diff > getIntConfig(WorldIntConfigs::CONFIG_MIN_LOG_UPDATE))
    {
        va_list ap;
        char str[256];
        va_start(ap, text);
        vsnprintf(str, 256, text, ap);
        va_end(ap);
        SF_LOG_INFO("misc", "Difftime %s: %u.", str, diff);
    }

    m_currentTime = thisTime;
}

void World::LoadAutobroadcasts()
{
    uint32 oldMSTime = getMSTime();

    m_Autobroadcasts.clear();
    m_AutobroadcastsWeights.clear();

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_AUTOBROADCAST);
    stmt->setInt32(0, realmID);
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    if (!result)
    {
        SF_LOG_INFO("server.loading", ">> Loaded 0 autobroadcasts definitions. DB table `autobroadcast` is empty for this realm!");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint8 id = fields[0].GetUInt8();

        m_Autobroadcasts[id] = fields[2].GetString();
        m_AutobroadcastsWeights[id] = fields[1].GetUInt8();

        ++count;
    } while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u autobroadcast definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

/// Update the World !
void World::Update(uint32 diff)
{
    m_updateTime = diff;

    if (getIntConfig(WorldIntConfigs::CONFIG_INTERVAL_LOG_UPDATE) && diff > getIntConfig(WorldIntConfigs::CONFIG_MIN_LOG_UPDATE))
    {
        if (m_updateTimeSum > getIntConfig(WorldIntConfigs::CONFIG_INTERVAL_LOG_UPDATE))
        {
            SF_LOG_DEBUG("misc", "Update time diff: %u. Players online: %u.", m_updateTimeSum / m_updateTimeCount, GetActiveSessionCount());
            m_updateTimeSum = m_updateTime;
            m_updateTimeCount = 1;
        }
        else
        {
            m_updateTimeSum += m_updateTime;
            ++m_updateTimeCount;
        }
    }

    ///- Update the different timers
    for (int i = 0; i < WUPDATE_COUNT; ++i)
    {
        if (m_timers[i].GetCurrent() >= 0)
            m_timers[i].Update(diff);
        else
            m_timers[i].SetCurrent(0);
    }

    ///- Update the game time and check for shutdown time
    _UpdateGameTime();

    /// Handle daily quests reset time
    if (m_gameTime > m_NextDailyQuestReset)
    {
        ResetDailyQuests();
        m_NextDailyQuestReset += DAY;
    }

    /// Handle weekly quests reset time
    if (m_gameTime > m_NextWeeklyQuestReset)
        ResetWeeklyQuests();

    /// Handle monthly quests reset time
    if (m_gameTime > m_NextMonthlyQuestReset)
        ResetMonthlyQuests();

    if (m_gameTime > m_NextRandomBGReset)
        ResetRandomBG();

    if (m_gameTime > m_NextGuildReset)
        ResetGuildCap();

    if (m_gameTime > m_NextCurrencyReset)
        ResetCurrencyWeekCap();

    /// <ul><li> Handle auctions when the timer has passed
    if (m_timers[WUPDATE_BLACK_MARKET].Passed())
    {
        m_timers[WUPDATE_BLACK_MARKET].Reset();

        ///- Update mails (return old mails with item, or delete them)
        //(tested... works on win)
        if (++mail_timer > mail_timer_expires)
        {
            mail_timer = 0;
            sObjectMgr->ReturnOrDeleteOldMails(true);
        }

        ///- Handle expired Blackmarket auctions
        sBlackMarketMgr->Update();
    }

    /// <ul><li> Handle auctions when the timer has passed
    if (m_timers[WUPDATE_AUCTIONS].Passed())
    {
        m_timers[WUPDATE_AUCTIONS].Reset();

        ///- Update mails (return old mails with item, or delete them)
        //(tested... works on win)
        if (++mail_timer > mail_timer_expires)
        {
            mail_timer = 0;
            sObjectMgr->ReturnOrDeleteOldMails(true);
        }

        ///- Handle expired auctions
        sAuctionMgr->Update();
    }

    /// <li> Handle session updates when the timer has passed
    RecordTimeDiff(NULL);
    UpdateSessions(diff);
    RecordTimeDiff("UpdateSessions");

    /// <li> Handle weather updates when the timer has passed
    if (m_timers[WUPDATE_WEATHERS].Passed())
    {
        m_timers[WUPDATE_WEATHERS].Reset();
        WeatherMgr::Update(uint32(m_timers[WUPDATE_WEATHERS].GetInterval()));
    }

    /// <li> Update uptime table
    if (m_timers[WUPDATE_UPTIME].Passed())
    {
        uint32 tmpDiff = uint32(m_gameTime - m_startTime);
        uint32 maxOnlinePlayers = GetMaxPlayerCount();

        m_timers[WUPDATE_UPTIME].Reset();

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_UPTIME_PLAYERS);

        stmt->setUInt32(0, tmpDiff);
        stmt->setUInt16(1, uint16(maxOnlinePlayers));
        stmt->setUInt32(2, realmID);
        stmt->setUInt32(3, uint32(m_startTime));

        LoginDatabase.Execute(stmt);
    }

    /// <li> Clean logs table
    if (sWorld->getIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARTIME) > 0) // if not enabled, ignore the timer
    {
        if (m_timers[WUPDATE_CLEANDB].Passed())
        {
            m_timers[WUPDATE_CLEANDB].Reset();

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_OLD_LOGS);

            stmt->setUInt32(0, sWorld->getIntConfig(WorldIntConfigs::CONFIG_LOGDB_CLEARTIME));
            stmt->setUInt32(1, uint32(time(0)));

            LoginDatabase.Execute(stmt);
        }
    }

    /// <li> Handle all other objects
    ///- Update objects when the timer has passed (maps, transport, creatures, ...)
    RecordTimeDiff(NULL);
    sMapMgr->Update(diff);
    RecordTimeDiff("UpdateMapMgr");

    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_AUTOBROADCAST))
    {
        if (m_timers[WUPDATE_AUTOBROADCAST].Passed())
        {
            m_timers[WUPDATE_AUTOBROADCAST].Reset();
            SendAutoBroadcast();
        }
    }

    sBattlegroundMgr->Update(diff);
    RecordTimeDiff("UpdateBattlegroundMgr");

    sOutdoorPvPMgr->Update(diff);
    RecordTimeDiff("UpdateOutdoorPvPMgr");

    sBattlefieldMgr->Update(diff);
    RecordTimeDiff("BattlefieldMgr");

    ///- Delete all characters which have been deleted X days before
    if (m_timers[WUPDATE_DELETECHARS].Passed())
    {
        m_timers[WUPDATE_DELETECHARS].Reset();
        Player::DeleteOldCharacters();
    }

    sLFGMgr->Update(diff);
    RecordTimeDiff("UpdateLFGMgr");

    // execute callbacks from sql queries that were queued recently
    ProcessQueryCallbacks();
    RecordTimeDiff("ProcessQueryCallbacks");

    ///- Erase corpses once every 20 minutes
    if (m_timers[WUPDATE_CORPSES].Passed())
    {
        m_timers[WUPDATE_CORPSES].Reset();
        sObjectAccessor->RemoveOldCorpses();
    }

    ///- Process Game events when necessary
    if (m_timers[WUPDATE_EVENTS].Passed())
    {
        m_timers[WUPDATE_EVENTS].Reset();                   // to give time for Update() to be processed
        uint32 nextGameEvent = sGameEventMgr->Update();
        m_timers[WUPDATE_EVENTS].SetInterval(nextGameEvent);
        m_timers[WUPDATE_EVENTS].Reset();
    }

    ///- Ping to keep MySQL connections alive
    if (m_timers[WUPDATE_PINGDB].Passed())
    {
        m_timers[WUPDATE_PINGDB].Reset();
        SF_LOG_DEBUG("misc", "Ping MySQL to keep connection alive");
        CharacterDatabase.KeepAlive();
        LoginDatabase.KeepAlive();
        WorldDatabase.KeepAlive();
    }

    if (m_timers[WUPDATE_GUILDSAVE].Passed())
    {
        m_timers[WUPDATE_GUILDSAVE].Reset();
        sGuildMgr->SaveGuilds();
    }

    // update the instance reset times
    sInstanceSaveMgr->Update();

    // And last, but not least handle the issued cli commands
    ProcessCliCommands();

    sScriptMgr->OnWorldUpdate(diff);
}

void World::ForceGameEventUpdate()
{
    m_timers[WUPDATE_EVENTS].Reset();                   // to give time for Update() to be processed
    uint32 nextGameEvent = sGameEventMgr->Update();
    m_timers[WUPDATE_EVENTS].SetInterval(nextGameEvent);
    m_timers[WUPDATE_EVENTS].Reset();
}

/// Send a packet to all players (except self if mentioned)
void World::SendGlobalMessage(WorldPacket* packet, WorldSession* self, uint32 team)
{
    SessionMap::const_iterator itr;
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (itr->second &&
            itr->second->GetPlayer() &&
            itr->second->GetPlayer()->IsInWorld() &&
            itr->second != self &&
            (team == 0 || itr->second->GetPlayer()->GetTeam() == team))
        {
            itr->second->SendPacket(packet);
        }
    }
}

/// Send a packet to all GMs (except self if mentioned)
void World::SendGlobalGMMessage(WorldPacket* packet, WorldSession* self, uint32 team)
{
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        // check if session and can receive global GM Messages and its not self
        WorldSession* session = itr->second;
        if (!session || session == self || !session->HasPermission(rbac::RBAC_PERM_RECEIVE_GLOBAL_GM_TEXTMESSAGE))
            continue;

        // Player should be in world
        Player* player = session->GetPlayer();
        if (!player || !player->IsInWorld())
            continue;

        // Send only to same team, if team is given
        if (!team || player->GetTeam() == team)
            session->SendPacket(packet);
    }
}

namespace Skyfire
{
    class WorldWorldTextBuilder
    {
        public:
            typedef std::vector<WorldPacket*> WorldPacketList;
            explicit WorldWorldTextBuilder(int32 textId, va_list* args = NULL) : i_textId(textId), i_args(args) { }
            void operator()(WorldPacketList& data_list, LocaleConstant loc_idx)
            {
                char const* text = sObjectMgr->GetSkyFireString(i_textId, loc_idx);

                if (i_args)
                {
                    // we need copy va_list before use or original va_list will corrupted
                    va_list ap;
                    va_copy(ap, *i_args);

                    char str[2048];
                    vsnprintf(str, 2048, text, ap);
                    va_end(ap);

                    do_helper(data_list, &str[0]);
                }
                else
                    do_helper(data_list, (char*)text);
            }
        private:
            char* lineFromMessage(char*& pos) { char* start = strtok(pos, "\n"); pos = NULL; return start; }
            void do_helper(WorldPacketList& data_list, char* text)
            {
                char* pos = text;

                while (char* line = lineFromMessage(pos))
                {
                    WorldPacket* data = new WorldPacket();
                    ChatHandler::BuildChatPacket(*data, ChatMsg::CHAT_MSG_SYSTEM, Language::LANG_UNIVERSAL, NULL, NULL, line);
                    data_list.push_back(data);
                }
            }

            int32 i_textId;
            va_list* i_args;
    };
}                                                           // namespace Skyfire

/// Send a System Message to all players (except self if mentioned)
void World::SendWorldText(int32 string_id, ...)
{
    va_list ap;
    va_start(ap, string_id);

    Skyfire::WorldWorldTextBuilder wt_builder(string_id, &ap);
    Skyfire::LocalizedPacketListDo<Skyfire::WorldWorldTextBuilder> wt_do(wt_builder);
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld())
            continue;

        wt_do(itr->second->GetPlayer());
    }

    va_end(ap);
}

/// Send a System Message to all GMs (except self if mentioned)
void World::SendGMText(int32 string_id, ...)
{
    va_list ap;
    va_start(ap, string_id);

    Skyfire::WorldWorldTextBuilder wt_builder(string_id, &ap);
    Skyfire::LocalizedPacketListDo<Skyfire::WorldWorldTextBuilder> wt_do(wt_builder);
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        // Session should have permissions to receive global gm messages
        WorldSession* session = itr->second;
        if (!session || !session->HasPermission(rbac::RBAC_PERM_RECEIVE_GLOBAL_GM_TEXTMESSAGE))
            continue;

        // Player should be in world
        Player* player = session->GetPlayer();
        if (!player || !player->IsInWorld())
            continue;

        wt_do(player);
    }

    va_end(ap);
}

/// DEPRECATED, only for debug purpose. Send a System Message to all players (except self if mentioned)
void World::SendGlobalText(const char* text, WorldSession* self)
{
    WorldPacket data;

    // need copy to prevent corruption by strtok call in LineFromMessage original string
    char* buf = strdup(text);
    char* pos = buf;

    while (char* line = ChatHandler::LineFromMessage(pos))
    {
        ChatHandler::BuildChatPacket(data, ChatMsg::CHAT_MSG_SYSTEM, Language::LANG_UNIVERSAL, NULL, NULL, line);
        SendGlobalMessage(&data, self);
    }

    free(buf);
}

/// Send a packet to all players (or players selected team) in the zone (except self if mentioned)
void World::SendZoneMessage(uint32 zone, WorldPacket* packet, WorldSession* self, uint32 team)
{
    SessionMap::const_iterator itr;
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (itr->second &&
            itr->second->GetPlayer() &&
            itr->second->GetPlayer()->IsInWorld() &&
            itr->second->GetPlayer()->GetZoneId() == zone &&
            itr->second != self &&
            (team == 0 || itr->second->GetPlayer()->GetTeam() == team))
        {
            itr->second->SendPacket(packet);
        }
    }
}

/// Send a System Message to all players in the zone (except self if mentioned)
void World::SendZoneText(uint32 zone, const char* text, WorldSession* self, uint32 team)
{
    WorldPacket data;
    ChatHandler::BuildChatPacket(data, ChatMsg::CHAT_MSG_SYSTEM, Language::LANG_UNIVERSAL, NULL, NULL, text);
    SendZoneMessage(zone, &data, self, team);
}

/// Kick (and save) all players
void World::KickAll()
{
    m_QueuedPlayer.clear();                                 // prevent send queue update packet and login queued sessions

    // session not removed at kick and will removed in next update tick
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        itr->second->KickPlayer();
}

/// Kick (and save) all players with security level less `sec`
void World::KickAllLess(AccountTypes sec)
{
    // session not removed at kick and will removed in next update tick
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetSecurity() < sec)
            itr->second->KickPlayer();
}

/// Ban an account or ban an IP address, duration will be parsed using TimeStringToSecs if it is positive, otherwise permban
BanReturn World::BanAccount(BanMode mode, std::string const& nameOrIP, std::string const& duration, std::string const& reason, std::string const& author)
{
    uint32 duration_secs = TimeStringToSecs(duration);
    return BanAccount(mode, nameOrIP, duration_secs, reason, author);
}

/// Ban an account or ban an IP address, duration is in seconds if positive, otherwise permban
BanReturn World::BanAccount(BanMode mode, std::string const& nameOrIP, uint32 duration_secs, std::string const& reason, std::string const& author)
{
    PreparedQueryResult resultAccounts = PreparedQueryResult(NULL); //used for kicking
    PreparedStatement* stmt = NULL;

    ///- Update the database with ban information
    switch (mode)
    {
        case BAN_IP:
            // No SQL injection with prepared statements
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BY_IP);
            stmt->setString(0, nameOrIP);
            resultAccounts = LoginDatabase.Query(stmt);
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_IP_BANNED);
            stmt->setString(0, nameOrIP);
            stmt->setUInt32(1, duration_secs);
            stmt->setString(2, author);
            stmt->setString(3, reason);
            LoginDatabase.Execute(stmt);
            break;
        case BAN_ACCOUNT:
            // No SQL injection with prepared statements
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_ID_BY_NAME);
            stmt->setString(0, nameOrIP);
            resultAccounts = LoginDatabase.Query(stmt);
            break;
        case BAN_CHARACTER:
            // No SQL injection with prepared statements
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BY_NAME);
            stmt->setString(0, nameOrIP);
            resultAccounts = CharacterDatabase.Query(stmt);
            break;
        default:
            return BAN_SYNTAX_ERROR;
    }

    if (!resultAccounts)
    {
        if (mode == BAN_IP)
            return BAN_SUCCESS;                             // ip correctly banned but nobody affected (yet)
        else
            return BAN_NOTFOUND;                            // Nobody to ban
    }

    ///- Disconnect all affected players (for IP it can be several)
    SQLTransaction trans = LoginDatabase.BeginTransaction();
    do
    {
        Field* fieldsAccount = resultAccounts->Fetch();
        uint32 account = fieldsAccount[0].GetUInt32();

        if (mode != BAN_IP)
        {
            // make sure there is only one active ban
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_NOT_BANNED);
            stmt->setUInt32(0, account);
            trans->Append(stmt);
            // No SQL injection with prepared statements
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_BANNED);
            stmt->setUInt32(0, account);
            stmt->setUInt32(1, duration_secs);
            stmt->setString(2, author);
            stmt->setString(3, reason);
            trans->Append(stmt);
        }

        if (WorldSession* sess = FindSession(account))
            if (std::string(sess->GetPlayerName()) != author)
                sess->KickPlayer();
    } while (resultAccounts->NextRow());

    LoginDatabase.CommitTransaction(trans);

    return BAN_SUCCESS;
}

/// Remove a ban from an account or IP address
bool World::RemoveBanAccount(BanMode mode, std::string const& nameOrIP)
{
    PreparedStatement* stmt = NULL;
    if (mode == BAN_IP)
    {
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_IP_NOT_BANNED);
        stmt->setString(0, nameOrIP);
        LoginDatabase.Execute(stmt);
    }
    else
    {
        uint32 account = 0;
        if (mode == BAN_ACCOUNT)
            account = AccountMgr::GetId(nameOrIP);
        else if (mode == BAN_CHARACTER)
            account = sObjectMgr->GetPlayerAccountIdByPlayerName(nameOrIP);

        if (!account)
            return false;

        //NO SQL injection as account is uint32
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_NOT_BANNED);
        stmt->setUInt32(0, account);
        LoginDatabase.Execute(stmt);
    }
    return true;
}

/// Ban an account or ban an IP address, duration will be parsed using TimeStringToSecs if it is positive, otherwise permban
BanReturn World::BanCharacter(std::string const& name, std::string const& duration, std::string const& reason, std::string const& author)
{
    Player* pBanned = sObjectAccessor->FindPlayerByName(name);
    uint32 guid = 0;

    uint32 duration_secs = TimeStringToSecs(duration);

    /// Pick a player to ban if not online
    if (!pBanned)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);
        stmt->setString(0, name);
        PreparedQueryResult resultCharacter = CharacterDatabase.Query(stmt);

        if (!resultCharacter)
            return BAN_NOTFOUND;                                    // Nobody to ban

        guid = (*resultCharacter)[0].GetUInt32();
    }
    else
        guid = pBanned->GetGUIDLow();

    // make sure there is only one active ban
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_BAN);
    stmt->setUInt32(0, guid);
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_BAN);
    stmt->setUInt32(0, guid);
    stmt->setUInt32(1, duration_secs);
    stmt->setString(2, author);
    stmt->setString(3, reason);
    CharacterDatabase.Execute(stmt);

    if (pBanned)
        pBanned->GetSession()->KickPlayer();

    return BAN_SUCCESS;
}

/// Remove a ban from a character
bool World::RemoveBanCharacter(std::string const& name)
{
    Player* pBanned = sObjectAccessor->FindPlayerByName(name);
    uint32 guid = 0;

    /// Pick a player to ban if not online
    if (!pBanned)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);
        stmt->setString(0, name);
        PreparedQueryResult resultCharacter = CharacterDatabase.Query(stmt);

        if (!resultCharacter)
            return false;

        guid = (*resultCharacter)[0].GetUInt32();
    }
    else
        guid = pBanned->GetGUIDLow();

    if (!guid)
        return false;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_BAN);
    stmt->setUInt32(0, guid);
    CharacterDatabase.Execute(stmt);
    return true;
}

/// Update the game time
void World::_UpdateGameTime()
{
    ///- update the time
    time_t thisTime = time(NULL);
    uint32 elapsed = uint32(thisTime - m_gameTime);
    m_gameTime = thisTime;

    ///- if there is a shutdown timer
    if (!IsStopped() && m_ShutdownTimer > 0 && elapsed > 0)
    {
        ///- ... and it is overdue, stop the world (set m_stopEvent)
        if (m_ShutdownTimer <= elapsed)
        {
            if (!(m_ShutdownMask & SHUTDOWN_MASK_IDLE) || GetActiveAndQueuedSessionCount() == 0)
                m_stopEvent = true;                         // exist code already set
            else
                m_ShutdownTimer = 1;                        // minimum timer value to wait idle state
        }
        ///- ... else decrease it and if necessary display a shutdown countdown to the users
        else
        {
            m_ShutdownTimer -= elapsed;

            ShutdownMsg();
        }
    }
}

/// Shutdown the server
void World::ShutdownServ(uint32 time, uint32 options, uint8 exitcode)
{
    // ignore if server shutdown at next tick
    if (IsStopped())
        return;

    m_ShutdownMask = options;
    m_ExitCode = exitcode;

    ///- If the shutdown time is 0, set m_stopEvent (except if shutdown is 'idle' with remaining sessions)
    if (time == 0)
    {
        if (!(options & SHUTDOWN_MASK_IDLE) || GetActiveAndQueuedSessionCount() == 0)
            m_stopEvent = true;                             // exist code already set
        else
            m_ShutdownTimer = 1;                            //So that the session count is re-evaluated at next world tick
    }
    ///- Else set the shutdown timer and warn users
    else
    {
        m_ShutdownTimer = time;
        ShutdownMsg(true);
    }

    sScriptMgr->OnShutdownInitiate(ShutdownExitCode(exitcode), ShutdownMask(options));
}

/// Display a shutdown message to the user(s)
void World::ShutdownMsg(bool show, Player* player)
{
    // not show messages for idle shutdown mode
    if (m_ShutdownMask & SHUTDOWN_MASK_IDLE)
        return;

    ///- Display a message every 12 hours, hours, 5 minutes, minute, 5 seconds and finally seconds
    if (show ||
        (m_ShutdownTimer < 5* MINUTE && (m_ShutdownTimer % 15) == 0) || // < 5 min; every 15 sec
        (m_ShutdownTimer < 15 * MINUTE && (m_ShutdownTimer % MINUTE) == 0) || // < 15 min ; every 1 min
        (m_ShutdownTimer < 30 * MINUTE && (m_ShutdownTimer % (5 * MINUTE)) == 0) || // < 30 min ; every 5 min
        (m_ShutdownTimer < 12 * HOUR && (m_ShutdownTimer % HOUR) == 0) || // < 12 h ; every 1 h
        (m_ShutdownTimer > 12 * HOUR && (m_ShutdownTimer % (12 * HOUR)) == 0)) // > 12 h ; every 12 h
    {
        std::string str = secsToTimeString(m_ShutdownTimer);

        ServerMessageType msgid = (m_ShutdownMask & SHUTDOWN_MASK_RESTART) ? SERVER_MSG_RESTART_TIME : SERVER_MSG_SHUTDOWN_TIME;

        SendServerMessage(msgid, str.c_str(), player);
        SF_LOG_DEBUG("misc", "Server is %s in %s", (m_ShutdownMask & SHUTDOWN_MASK_RESTART ? "restart" : "shuttingdown"), str.c_str());
    }
}

/// Cancel a planned server shutdown
void World::ShutdownCancel()
{
    // nothing cancel or too later
    if (!m_ShutdownTimer || m_stopEvent.value())
        return;

    ServerMessageType msgid = (m_ShutdownMask & SHUTDOWN_MASK_RESTART) ? SERVER_MSG_RESTART_CANCELLED : SERVER_MSG_SHUTDOWN_CANCELLED;

    m_ShutdownMask = 0;
    m_ShutdownTimer = 0;
    m_ExitCode = SHUTDOWN_EXIT_CODE;                       // to default value
    SendServerMessage(msgid);

    SF_LOG_DEBUG("misc", "Server %s cancelled.", (m_ShutdownMask & SHUTDOWN_MASK_RESTART ? "restart" : "shuttingdown"));

    sScriptMgr->OnShutdownCancel();
}

/// Send a server message to the user(s)
void World::SendServerMessage(ServerMessageType type, const char *text, Player* player)
{
    WorldPacket data(SMSG_SERVER_MESSAGE, 50);              // guess size
    data << uint32(type);
    if (type <= SERVER_MSG_STRING)
        data << text;

    if (player)
        player->GetSession()->SendPacket(&data);
    else
        SendGlobalMessage(&data);
}

void World::UpdateSessions(uint32 diff)
{
    ///- Add new sessions
    WorldSession* sess = NULL;
    while (addSessQueue.next(sess))
        AddSession_ (sess);

    ///- Then send an update signal to remaining ones
    for (SessionMap::iterator itr = m_sessions.begin(), next; itr != m_sessions.end(); itr = next)
    {
        next = itr;
        ++next;

        ///- and remove not active sessions from the list
        WorldSession* pSession = itr->second;
        WorldSessionFilter updater(pSession);

        if (!pSession->Update(diff, updater))    // As interval = 0
        {
            if (!RemoveQueuedPlayer(itr->second) && itr->second && getIntConfig(WorldIntConfigs::CONFIG_INTERVAL_DISCONNECT_TOLERANCE))
                m_disconnects[itr->second->GetAccountId()] = time(NULL);
            RemoveQueuedPlayer(pSession);
            m_sessions.erase(itr);
            delete pSession;
        }
    }
}

// This handles the issued and queued CLI commands
void World::ProcessCliCommands()
{
    CliCommandHolder::Print* zprint = NULL;
    void* callbackArg = NULL;
    CliCommandHolder* command = NULL;
    while (cliCmdQueue.next(command))
    {
        SF_LOG_INFO("misc", "CLI command under processing...");
        zprint = command->m_print;
        callbackArg = command->m_callbackArg;
        CliHandler handler(callbackArg, zprint);
        handler.ParseCommands(command->m_command);
        if (command->m_commandFinished)
            command->m_commandFinished(callbackArg, !handler.HasSentErrorMessage());
        delete command;
    }
}

void World::SendAutoBroadcast()
{
    if (m_Autobroadcasts.empty())
        return;

    uint32 weight = 0;
    AutobroadcastsWeightMap selectionWeights;
    std::string msg;

    for (AutobroadcastsWeightMap::const_iterator it = m_AutobroadcastsWeights.begin(); it != m_AutobroadcastsWeights.end(); ++it)
    {
        if (it->second)
        {
            weight += it->second;
            selectionWeights[it->first] = it->second;
        }
    }

    if (weight)
    {
        uint32 selectedWeight = urand(0, weight - 1);
        weight = 0;
        for (AutobroadcastsWeightMap::const_iterator it = selectionWeights.begin(); it != selectionWeights.end(); ++it)
        {
            weight += it->second;
            if (selectedWeight < weight)
            {
                msg = m_Autobroadcasts[it->first];
                break;
            }
        }
    }
    else
        msg = m_Autobroadcasts[urand(0, m_Autobroadcasts.size())];

    uint32 abcenter = sWorld->getIntConfig(WorldIntConfigs::CONFIG_AUTOBROADCAST_CENTER);

    if (abcenter == 0)
        sWorld->SendWorldText(LANG_AUTO_BROADCAST, msg.c_str());
    else if (abcenter == 1)
    {
        WorldPacket data(SMSG_NOTIFICATION, 2 + msg.length());
        data.WriteBits(msg.length(), 12);
        data.FlushBits();
        data.WriteString(msg);
        sWorld->SendGlobalMessage(&data);
    }
    else if (abcenter == 2)
    {
        sWorld->SendWorldText(LANG_AUTO_BROADCAST, msg.c_str());

        WorldPacket data(SMSG_NOTIFICATION, 2 + msg.length());
        data.WriteBits(msg.length(), 12);
        data.FlushBits();
        data.WriteString(msg);
        sWorld->SendGlobalMessage(&data);
    }

    SF_LOG_DEBUG("misc", "AutoBroadcast: '%s'", msg.c_str());
}

void World::UpdateRealmCharCount(uint32 accountId)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_COUNT);
    stmt->setUInt32(0, accountId);
    PreparedQueryResultFuture result = CharacterDatabase.AsyncQuery(stmt);
    m_realmCharCallbacks.insert(result);
}

void World::_UpdateRealmCharCount(PreparedQueryResult resultCharCount)
{
    if (resultCharCount)
    {
        Field* fields = resultCharCount->Fetch();
        uint32 accountId = fields[0].GetUInt32();
        uint8 charCount = uint8(fields[1].GetUInt64());

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_REALM_CHARACTERS_BY_REALM);
        stmt->setUInt32(0, accountId);
        stmt->setUInt32(1, realmID);
        LoginDatabase.Execute(stmt);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_REALM_CHARACTERS);
        stmt->setUInt8(0, charCount);
        stmt->setUInt32(1, accountId);
        stmt->setUInt32(2, realmID);
        LoginDatabase.Execute(stmt);
    }
}

void World::InitWeeklyQuestResetTime()
{
    time_t wstime = uint64(sWorld->getWorldState(WS_WEEKLY_QUEST_RESET_TIME));
    time_t curtime = time(NULL);
    m_NextWeeklyQuestReset = wstime < curtime ? curtime : time_t(wstime);
}

void World::InitDailyQuestResetTime()
{
    time_t mostRecentQuestTime;

    QueryResult result = CharacterDatabase.Query("SELECT MAX(time) FROM character_queststatus_daily");
    if (result)
    {
        Field* fields = result->Fetch();
        mostRecentQuestTime = time_t(fields[0].GetUInt32());
    }
    else
        mostRecentQuestTime = 0;

    // client built-in time for reset is 6:00 AM
    // FIX ME: client not show day start time
    time_t curTime = time(NULL);
    tm localTm;
    ACE_OS::localtime_r(&curTime, &localTm);
    localTm.tm_hour = 6;
    localTm.tm_min  = 0;
    localTm.tm_sec  = 0;

    // current day reset time
    time_t curDayResetTime = mktime(&localTm);

    // last reset time before current moment
    time_t resetTime = (curTime < curDayResetTime) ? curDayResetTime - DAY : curDayResetTime;

    // need reset (if we have quest time before last reset time (not processed by some reason)
    if (mostRecentQuestTime && mostRecentQuestTime <= resetTime)
        m_NextDailyQuestReset = mostRecentQuestTime;
    else // plan next reset time
        m_NextDailyQuestReset = (curTime >= curDayResetTime) ? curDayResetTime + DAY : curDayResetTime;
}

void World::InitMonthlyQuestResetTime()
{
    time_t wstime = uint64(sWorld->getWorldState(WS_MONTHLY_QUEST_RESET_TIME));
    time_t curtime = time(NULL);
    m_NextMonthlyQuestReset = wstime < curtime ? curtime : time_t(wstime);
}

void World::InitRandomBGResetTime()
{
    time_t bgtime = uint64(sWorld->getWorldState(WS_BG_DAILY_RESET_TIME));
    if (!bgtime)
        m_NextRandomBGReset = time_t(time(NULL));         // game time not yet init

    // generate time by config
    time_t curTime = time(NULL);
    tm localTm;
    ACE_OS::localtime_r(&curTime, &localTm);
    localTm.tm_hour = getIntConfig(WorldIntConfigs::CONFIG_RANDOM_BG_RESET_HOUR);
    localTm.tm_min = 0;
    localTm.tm_sec = 0;

    // current day reset time
    time_t nextDayResetTime = mktime(&localTm);

    // next reset time before current moment
    if (curTime >= nextDayResetTime)
        nextDayResetTime += DAY;

    // normalize reset time
    m_NextRandomBGReset = bgtime < curTime ? nextDayResetTime - DAY : nextDayResetTime;

    if (!bgtime)
        sWorld->setWorldState(WS_BG_DAILY_RESET_TIME, uint64(m_NextRandomBGReset));
}

void World::InitGuildResetTime()
{
    time_t gtime = uint64(getWorldState(WS_GUILD_DAILY_RESET_TIME));
    if (!gtime)
        m_NextGuildReset = time_t(time(NULL));         // game time not yet init

    // generate time by config
    time_t curTime = time(NULL);
    tm localTm;
    ACE_OS::localtime_r(&curTime, &localTm);
    localTm.tm_hour = getIntConfig(WorldIntConfigs::CONFIG_GUILD_RESET_HOUR);
    localTm.tm_min = 0;
    localTm.tm_sec = 0;

    // current day reset time
    time_t nextDayResetTime = mktime(&localTm);

    // next reset time before current moment
    if (curTime >= nextDayResetTime)
        nextDayResetTime += DAY;

    // normalize reset time
    m_NextGuildReset = gtime < curTime ? nextDayResetTime - DAY : nextDayResetTime;

    if (!gtime)
        sWorld->setWorldState(WS_GUILD_DAILY_RESET_TIME, uint64(m_NextGuildReset));
}

void World::InitCurrencyResetTime()
{
    time_t currencytime = uint64(sWorld->getWorldState(WS_CURRENCY_RESET_TIME));
    if (!currencytime)
        m_NextCurrencyReset = time_t(time(NULL));         // game time not yet init

    // generate time by config
    time_t curTime = time(NULL);
    tm localTm = *localtime(&curTime);

    localTm.tm_wday = getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_DAY);
    localTm.tm_hour = getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_HOUR);
    localTm.tm_min = 0;
    localTm.tm_sec = 0;

    // current week reset time
    time_t nextWeekResetTime = mktime(&localTm);

    // next reset time before current moment
    if (curTime >= nextWeekResetTime)
        nextWeekResetTime += getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_INTERVAL) * DAY;

    // normalize reset time
    m_NextCurrencyReset = currencytime < curTime ? nextWeekResetTime - getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_INTERVAL) * DAY : nextWeekResetTime;

    if (!currencytime)
        sWorld->setWorldState(WS_CURRENCY_RESET_TIME, uint64(m_NextCurrencyReset));
}

void World::ResetDailyQuests()
{
    SF_LOG_INFO("misc", "Daily quests reset for all characters.");

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_DAILY);
    CharacterDatabase.Execute(stmt);

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->ResetDailyQuestStatus();

    // change available dailies
    sPoolMgr->ChangeDailyQuests();
}

void World::ResetCurrencyWeekCap()
{
    CharacterDatabase.Execute("UPDATE `character_currency` SET `weekly_quantity` = 0");

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->ResetCurrencyWeekCap();

    m_NextCurrencyReset = time_t(m_NextCurrencyReset + DAY * getIntConfig(WorldIntConfigs::CONFIG_CURRENCY_RESET_INTERVAL));
    sWorld->setWorldState(WS_CURRENCY_RESET_TIME, uint64(m_NextCurrencyReset));
}

void World::LoadDBAllowedSecurityLevel()
{
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_REALMLIST_SECURITY_LEVEL);
    stmt->setInt32(0, int32(realmID));
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    if (result)
        SetPlayerSecurityLimit(AccountTypes(result->Fetch()->GetUInt8()));
}

void World::SetPlayerSecurityLimit(AccountTypes _sec)
{
    AccountTypes sec = _sec < AccountTypes::SEC_CONSOLE ? _sec : AccountTypes::SEC_PLAYER;
    bool update = sec > m_allowedSecurityLevel;
    m_allowedSecurityLevel = sec;
    if (update)
        KickAllLess(m_allowedSecurityLevel);
}

void World::ResetWeeklyQuests()
{
    SF_LOG_INFO("misc", "Weekly quests reset for all characters.");

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_WEEKLY);
    CharacterDatabase.Execute(stmt);

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->ResetWeeklyQuestStatus();

    m_NextWeeklyQuestReset = time_t(m_NextWeeklyQuestReset + WEEK);
    sWorld->setWorldState(WS_WEEKLY_QUEST_RESET_TIME, uint64(m_NextWeeklyQuestReset));

    // change available weeklies
    sPoolMgr->ChangeWeeklyQuests();
}

void World::ResetMonthlyQuests()
{
    SF_LOG_INFO("misc", "Monthly quests reset for all characters.");

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_MONTHLY);
    CharacterDatabase.Execute(stmt);

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->ResetMonthlyQuestStatus();

    // generate time
    time_t curTime = time(NULL);
    tm localTm;
    ACE_OS::localtime_r(&curTime, &localTm);

    int month   = localTm.tm_mon;
    int year    = localTm.tm_year;

    ++month;

    // month 11 is december, next is january (0)
    if (month > 11)
    {
        month = 0;
        year += 1;
    }

    // reset time for next month
    localTm.tm_year     = year;
    localTm.tm_mon      = month;
    localTm.tm_mday     = 1;        // don't know if we really need config option for day / hour
    localTm.tm_hour     = 0;
    localTm.tm_min      = 0;
    localTm.tm_sec      = 0;

    time_t nextMonthResetTime = mktime(&localTm);

    // plan next reset time
    m_NextMonthlyQuestReset = (curTime >= nextMonthResetTime) ? nextMonthResetTime + MONTH : nextMonthResetTime;

    sWorld->setWorldState(WS_MONTHLY_QUEST_RESET_TIME, uint64(m_NextMonthlyQuestReset));
}

void World::ResetEventSeasonalQuests(uint16 event_id)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_SEASONAL);
    stmt->setUInt16(0, event_id);
    CharacterDatabase.Execute(stmt);

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->ResetSeasonalQuestStatus(event_id);
}

void World::ResetRandomBG()
{
    SF_LOG_INFO("misc", "Random BG status reset for all characters.");

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_BATTLEGROUND_RANDOM);
    CharacterDatabase.Execute(stmt);

    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second->GetPlayer())
            itr->second->GetPlayer()->SetRandomWinner(false);

    m_NextRandomBGReset = time_t(m_NextRandomBGReset + DAY);
    sWorld->setWorldState(WS_BG_DAILY_RESET_TIME, uint64(m_NextRandomBGReset));
}

void World::ResetGuildCap()
{
    m_NextGuildReset = time_t(m_NextGuildReset + DAY);
    sWorld->setWorldState(WS_GUILD_DAILY_RESET_TIME, uint64(m_NextGuildReset));
    uint32 week = getWorldState(WS_GUILD_WEEKLY_RESET_TIME);
    week = week < 7 ? week + 1 : 1;

    SF_LOG_INFO("misc", "Guild Daily Cap reset. Week: %u", week == 1);
    sWorld->setWorldState(WS_GUILD_WEEKLY_RESET_TIME, week);
    sGuildMgr->ResetTimes(week == 1);
}

void World::UpdateMaxSessionCounters()
{
    m_maxActiveSessionCount = std::max(m_maxActiveSessionCount, uint32(m_sessions.size()-m_QueuedPlayer.size()));
    m_maxQueuedSessionCount = std::max(m_maxQueuedSessionCount, uint32(m_QueuedPlayer.size()));
}

void World::LoadDBVersion()
{
    QueryResult result = WorldDatabase.Query("SELECT db_version, cache_id FROM version LIMIT 1");
    if (result)
    {
        Field* fields = result->Fetch();

        m_DBVersion = fields[0].GetString();
        // will be overwrite by config values if different and non-0
        setIntConfig(WorldIntConfigs::CONFIG_CLIENTCACHE_VERSION, fields[1].GetUInt32());
    }

    if (m_DBVersion.empty())
        m_DBVersion = "Unknown world database.";
}

void World::ProcessStartEvent()
{
    isEventKillStart = true;
}

void World::ProcessStopEvent()
{
    isEventKillStart = false;
}

void World::UpdateAreaDependentAuras()
{
    SessionMap::const_iterator itr;
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (itr->second && itr->second->GetPlayer() && itr->second->GetPlayer()->IsInWorld())
        {
            itr->second->GetPlayer()->UpdateAreaDependentAuras(itr->second->GetPlayer()->GetAreaId());
            itr->second->GetPlayer()->UpdateZoneDependentAuras(itr->second->GetPlayer()->GetZoneId());
        }
}

void World::LoadWorldStates()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT entry, value FROM worldstates");

    if (!result)
    {
        SF_LOG_INFO("server.loading", ">> Loaded 0 world states. DB table `worldstates` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        m_worldstates[fields[0].GetUInt32()] = fields[1].GetUInt32();
        ++count;
    }
    while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u world states in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

// Setting a worldstate will save it to DB
void World::setWorldState(uint32 index, uint64 value)
{
    WorldStatesMap::const_iterator it = m_worldstates.find(index);
    if (it != m_worldstates.end())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_WORLDSTATE);

        stmt->setUInt32(0, uint32(value));
        stmt->setUInt32(1, index);

        CharacterDatabase.Execute(stmt);
    }
    else
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_WORLDSTATE);

        stmt->setUInt32(0, index);
        stmt->setUInt32(1, uint32(value));

        CharacterDatabase.Execute(stmt);
    }
    m_worldstates[index] = value;
}

uint64 World::getWorldState(uint32 index) const
{
    WorldStatesMap::const_iterator it = m_worldstates.find(index);
    return it != m_worldstates.end() ? it->second : 0;
}

void World::ProcessQueryCallbacks()
{
    PreparedQueryResult result;

    while (!m_realmCharCallbacks.is_empty())
    {
        ACE_Future<PreparedQueryResult> lResult;
        ACE_Time_Value timeout = ACE_Time_Value::zero;
        if (m_realmCharCallbacks.next_readable(lResult, &timeout) != 1)
            break;

        if (lResult.ready())
        {
            lResult.get(result);
            _UpdateRealmCharCount(result);
            lResult.cancel();
        }
    }
}

/**
* @brief Loads several pieces of information on server startup with the low GUID
* There is no further database query necessary.
* These are a number of methods that work into the calling function.
*
* @param guid Requires a lowGUID to call
* @return Name, Gender, Race, Class and Level of player character
* Example Usage:
* @code
*    CharacterNameData const* nameData = sWorld->GetCharacterNameData(lowGUID);
*    if (!nameData)
*        return;
*
* std::string playerName = nameData->m_name;
* uint8 playerGender = nameData->m_gender;
* uint8 playerRace = nameData->m_race;
* uint8 playerClass = nameData->m_class;
* uint8 playerLevel = nameData->m_level;
* @endcode
**/

void World::LoadCharacterNameData()
{
    SF_LOG_INFO("server.loading", "Loading character name data");

    QueryResult result = CharacterDatabase.Query("SELECT guid, name, race, gender, class, level FROM characters WHERE deleteDate IS NULL");
    if (!result)
    {
        SF_LOG_INFO("server.loading", "No character name data loaded, empty query");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        AddCharacterNameData(fields[0].GetUInt32(), fields[1].GetString(),
            fields[3].GetUInt8() /*gender*/, fields[2].GetUInt8() /*race*/, fields[4].GetUInt8() /*class*/, fields[5].GetUInt8() /*level*/);
        ++count;
    } while (result->NextRow());

    SF_LOG_INFO("server.loading", "Loaded name data for %u characters", count);
}

void World::AddCharacterNameData(uint32 guid, std::string const& name, uint8 gender, uint8 race, uint8 playerClass, uint8 level)
{
    CharacterNameData& data = _characterNameDataMap[guid];
    data.m_name = name;
    data.m_race = race;
    data.m_gender = gender;
    data.m_class = playerClass;
    data.m_level = level;
}

void World::UpdateCharacterNameData(uint32 guid, std::string const& name, uint8 gender /*= GENDER_NONE*/, uint8 race /*= RACE_NONE*/)
{
    std::map<uint32, CharacterNameData>::iterator itr = _characterNameDataMap.find(guid);
    if (itr == _characterNameDataMap.end())
        return;

    itr->second.m_name = name;

    if (gender != GENDER_NONE)
        itr->second.m_gender = gender;

    if (race != RACE_NONE)
        itr->second.m_race = race;
}

void World::UpdateCharacterNameDataLevel(uint32 guid, uint8 level)
{
    std::map<uint32, CharacterNameData>::iterator itr = _characterNameDataMap.find(guid);
    if (itr == _characterNameDataMap.end())
        return;

    itr->second.m_level = level;
}

CharacterNameData const* World::GetCharacterNameData(uint32 guid) const
{
    std::map<uint32, CharacterNameData>::const_iterator itr = _characterNameDataMap.find(guid);
    if (itr != _characterNameDataMap.end())
        return &itr->second;
    else
        return NULL;
}

void World::ReloadRBAC()
{
    // Passive reload, we mark the data as invalidated and next time a permission is checked it will be reloaded
    SF_LOG_INFO("rbac", "World::ReloadRBAC()");
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        if (WorldSession* session = itr->second)
            session->InvalidateRBACData();
}
