/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "Database/DatabaseEnv.h"
#include "Util.h"
#include "WardenCheckMgr.h"
#include "Warden.h"

WardenCheckMgr::WardenCheckMgr() { }

WardenCheckMgr::~WardenCheckMgr()
{
    for (uint16 i = 0; i < CheckStore.size(); ++i)
        delete CheckStore[i];

    for (CheckResultContainer::iterator itr = CheckResultStore.begin(); itr != CheckResultStore.end(); ++itr)
        delete itr->second;
}

void WardenCheckMgr::LoadWardenChecks()
{
    // Check if Warden is enabled by config before loading anything
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_WARDEN_ENABLED))
    {
        SF_LOG_INFO("warden", ">> Warden disabled, loading checks skipped.");
        return;
    }

    QueryResult result = WorldDatabase.Query("SELECT MAX(id) FROM warden_checks");

    if (!result)
    {
        SF_LOG_INFO("server.loading", ">> Loaded 0 Warden checks. DB table `warden_checks` is empty!");
        return;
    }

    Field* fields = result->Fetch();

    uint16 maxCheckId = fields[0].GetUInt16();

    CheckStore.resize(maxCheckId + 1);

    //                                    0    1     2     3        4       5      6      7
    result = WorldDatabase.Query("SELECT id, type, data, result, address, length, str, comment FROM warden_checks ORDER BY id ASC");

    uint32 count = 0;
    do
    {
        fields = result->Fetch();

        uint16 id               = fields[0].GetUInt16();
        uint8 checkType         = fields[1].GetUInt8();
        std::string data        = fields[2].GetString();
        std::string checkResult = fields[3].GetString();
        uint32 address          = fields[4].GetUInt32();
        uint8 length            = fields[5].GetUInt8();
        std::string str         = fields[6].GetString();
        std::string comment     = fields[7].GetString();

        WardenCheck* wardenCheck = new WardenCheck();
        wardenCheck->Type = checkType;
        wardenCheck->CheckId = id;

        // Initialize action with default action from config
        wardenCheck->Action = WardenActions(sWorld->getIntConfig(WorldIntConfigs::CONFIG_WARDEN_CLIENT_FAIL_ACTION));

        if (checkType == PAGE_CHECK_A || checkType == PAGE_CHECK_B || checkType == DRIVER_CHECK)
        {
            wardenCheck->Data.SetHexStr(data.c_str());
            int len = data.size() / 2;

            if (wardenCheck->Data.GetNumBytes() < len)
            {
                uint8 temp[24];
                memset(temp, 0, len);
                memcpy(temp, wardenCheck->Data.AsByteArray(), wardenCheck->Data.GetNumBytes());
                std::reverse(temp, temp + len);
                wardenCheck->Data.SetBinary((uint8*)temp, len);
            }
        }

        if (checkType == MEM_CHECK || checkType == MODULE_CHECK)
            MemChecksIdPool.push_back(id);
        else
            OtherChecksIdPool.push_back(id);

        if (checkType == MEM_CHECK || checkType == PAGE_CHECK_A || checkType == PAGE_CHECK_B || checkType == PROC_CHECK)
        {
            wardenCheck->Address = address;
            wardenCheck->Length = length;
        }

        // PROC_CHECK support missing
        if (checkType == MEM_CHECK || checkType == MPQ_CHECK || checkType == LUA_STR_CHECK || checkType == DRIVER_CHECK || checkType == MODULE_CHECK)
            wardenCheck->Str = str;

        CheckStore[id] = wardenCheck;

        if (checkType == MPQ_CHECK || checkType == MEM_CHECK)
        {
            WardenCheckResult* wr = new WardenCheckResult();
            wr->Result.SetHexStr(checkResult.c_str());
            int len = checkResult.size() / 2;
            if (wr->Result.GetNumBytes() < len)
            {
                uint8 *temp = new uint8[len];
                memset(temp, 0, len);
                memcpy(temp, wr->Result.AsByteArray(), wr->Result.GetNumBytes());
                std::reverse(temp, temp + len);
                wr->Result.SetBinary((uint8*)temp, len);
                delete [] temp;
            }
            CheckResultStore[id] = wr;
        }

        if (comment.empty())
            wardenCheck->Comment = "Undocumented Check";
        else
            wardenCheck->Comment = comment;

        ++count;
    }
    while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u warden checks.", count);
}

void WardenCheckMgr::LoadWardenOverrides()
{
    // Check if Warden is enabled by config before loading anything
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_WARDEN_ENABLED))
    {
        SF_LOG_INFO("warden", ">> Warden disabled, loading check overrides skipped.");
        return;
    }

    //                                                      0        1
    QueryResult result = CharacterDatabase.Query("SELECT wardenId, action FROM warden_action");

    if (!result)
    {
        SF_LOG_INFO("server.loading", ">> Loaded 0 Warden action overrides. DB table `warden_action` is empty!");
        return;
    }

    uint32 count = 0;

    ACE_WRITE_GUARD(ACE_RW_Mutex, g, _checkStoreLock);

    do
    {
        Field* fields = result->Fetch();

        uint16 checkId = fields[0].GetUInt16();
        uint8  action  = fields[1].GetUInt8();

        // Check if action value is in range (0-2, see WardenActions enum)
        if (action > WARDEN_ACTION_BAN)
            SF_LOG_ERROR("warden", "Warden check override action out of range (ID: %u, action: %u)", checkId, action);
        // Check if check actually exists before accessing the CheckStore vector
        else if (checkId > CheckStore.size())
            SF_LOG_ERROR("warden", "Warden check action override for non-existing check (ID: %u, action: %u), skipped", checkId, action);
        else
        {
            CheckStore[checkId]->Action = WardenActions(action);
            ++count;
        }
    }
    while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u warden action overrides.", count);
}

WardenCheck* WardenCheckMgr::GetWardenDataById(uint16 Id)
{
    if (Id < CheckStore.size())
        return CheckStore[Id];

    return NULL;
}

WardenCheckResult* WardenCheckMgr::GetWardenResultById(uint16 Id)
{
    CheckResultContainer::const_iterator itr = CheckResultStore.find(Id);
    if (itr != CheckResultStore.end())
        return itr->second;
    return NULL;
}
