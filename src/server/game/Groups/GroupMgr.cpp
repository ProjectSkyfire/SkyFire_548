/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "GroupMgr.h"
#include "InstanceSaveMgr.h"
#include "World.h"
#include "DBCStores.h"

GroupMgr::~GroupMgr()
{
    for (GroupContainer::iterator itr = GroupStore.begin(); itr != GroupStore.end(); ++itr)
        delete itr->second;
}

uint32 GroupMgr::GenerateNewGroupDbStoreId()
{
    uint32 newStorageId = NextGroupDbStoreId;

    for (uint32 i = ++NextGroupDbStoreId; i < 0xFFFFFFFF; ++i)
    {
        if ((i < GroupDbStore.size() && GroupDbStore[i] == NULL) || i >= GroupDbStore.size())
        {
            NextGroupDbStoreId = i;
            break;
        }
    }

    if (newStorageId == NextGroupDbStoreId)
    {
        SF_LOG_ERROR("misc", "Group storage ID overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }

    return newStorageId;
}

void GroupMgr::RegisterGroupDbStoreId(uint32 storageId, Group* group)
{
    // Allocate space if necessary.
    if (storageId >= uint32(GroupDbStore.size()))
        GroupDbStore.resize(storageId + 1);

    GroupDbStore[storageId] = group;
}

void GroupMgr::FreeGroupDbStoreId(Group* group)
{
    uint32 storageId = group->GetDbStoreId();

    if (storageId < NextGroupDbStoreId)
        NextGroupDbStoreId = storageId;

    GroupDbStore[storageId] = NULL;
}

Group* GroupMgr::GetGroupByDbStoreId(uint32 storageId) const
{
    if (storageId < GroupDbStore.size())
        return GroupDbStore[storageId];

    return NULL;
}

uint32 GroupMgr::GenerateGroupId()
{
    if (NextGroupId >= 0xFFFFFFFE)
    {
        SF_LOG_ERROR("misc", "Group guid overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }
    return NextGroupId++;
}

Group* GroupMgr::GetGroupByGUID(uint32 groupId) const
{
    GroupContainer::const_iterator itr = GroupStore.find(groupId);
    if (itr != GroupStore.end())
        return itr->second;

    return NULL;
}

void GroupMgr::AddGroup(Group* group)
{
    GroupStore[group->GetLowGUID()] = group;
}

void GroupMgr::RemoveGroup(Group* group)
{
    GroupStore.erase(group->GetLowGUID());
}

void GroupMgr::LoadGroups()
{
    {
        uint32 oldMSTime = getMSTime();

        // Delete all groups whose leader does not exist
        CharacterDatabase.DirectExecute("DELETE FROM parties WHERE leaderGuid NOT IN (SELECT guid FROM characters)");
        // Delete all groups with less than 2 members
        CharacterDatabase.DirectExecute("DELETE FROM parties WHERE guid NOT IN (SELECT guid FROM party_member GROUP BY guid HAVING COUNT(guid) > 1)");

        //                                                        0              1           2             3                 4      5          6      7         8       9
        QueryResult result = CharacterDatabase.Query("SELECT g.leaderGuid, g.lootMethod, g.looterGuid, g.lootThreshold, g.icon1, g.icon2, g.icon3, g.icon4, g.icon5, g.icon6"
            //  10         11          12         13              14            15         16           17
            ", g.icon7, g.icon8, g.partyType, g.difficulty, g.raiddifficulty, g.guid, lfg.dungeon, lfg.state FROM parties g LEFT JOIN lfg_data lfg ON lfg.guid = g.guid ORDER BY g.guid ASC");
        if (!result)
        {
            SF_LOG_INFO("server.loading", ">> Loaded 0 group definitions. DB table `parties` is empty!");
            return;
        }

        uint32 count = 0;
        do
        {
            Field* fields = result->Fetch();
            Group* group = new Group;
            group->LoadGroupFromDB(fields);
            AddGroup(group);

            // Get the ID used for storing the group in the database and register it in the pool.
            uint32 storageId = group->GetDbStoreId();

            RegisterGroupDbStoreId(storageId, group);

            // Increase the next available storage ID
            if (storageId == NextGroupDbStoreId)
                NextGroupDbStoreId++;

            ++count;
        }
        while (result->NextRow());

        SF_LOG_INFO("server.loading", ">> Loaded %u group definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }

    SF_LOG_INFO("server.loading", "Loading Group members...");
    {
        uint32 oldMSTime = getMSTime();

        // Delete all rows from party_member and party_instance with no group
        CharacterDatabase.DirectExecute("DELETE FROM party_member WHERE guid NOT IN (SELECT guid FROM parties)");
        CharacterDatabase.DirectExecute("DELETE FROM party_instance WHERE guid NOT IN (SELECT guid FROM parties)");
        // Delete all members that does not exist
        CharacterDatabase.DirectExecute("DELETE FROM party_member WHERE memberGuid NOT IN (SELECT guid FROM characters)");

        //                                                    0        1           2            3       4
        QueryResult result = CharacterDatabase.Query("SELECT guid, memberGuid, memberFlags, subparty, roles FROM party_member ORDER BY guid");
        if (!result)
        {
            SF_LOG_INFO("server.loading", ">> Loaded 0 group members. DB table `party_member` is empty!");
            return;
        }

        uint32 count = 0;

        do
        {
            Field* fields = result->Fetch();
            Group* group = GetGroupByDbStoreId(fields[0].GetUInt32());

            if (group)
                group->LoadMemberFromDB(fields[1].GetUInt32(), fields[2].GetUInt8(), fields[3].GetUInt8(), fields[4].GetUInt8());
            else
                SF_LOG_ERROR("misc", "GroupMgr::LoadGroups: Consistency failed, can't find group (storage id: %u)", fields[0].GetUInt32());

            ++count;
        }
        while (result->NextRow());

        SF_LOG_INFO("server.loading", ">> Loaded %u group members in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }

    SF_LOG_INFO("server.loading", "Loading Group instance saves...");
    {
        uint32 oldMSTime = getMSTime();
        //                                                   0           1        2              3             4             5            6
        QueryResult result = CharacterDatabase.Query("SELECT gi.guid, i.map, gi.instance, gi.permanent, i.difficulty, i.resettime, COUNT(g.guid) "
            "FROM party_instance gi INNER JOIN instance i ON gi.instance = i.id "
            "LEFT JOIN character_instance ci LEFT JOIN parties g ON g.leaderGuid = ci.guid ON ci.instance = gi.instance AND ci.permanent = 1 GROUP BY gi.instance ORDER BY gi.guid");
        if (!result)
        {
            SF_LOG_INFO("server.loading", ">> Loaded 0 group-instance saves. DB table `party_instance` is empty!");
            return;
        }

        uint32 count = 0;
        do
        {
            Field* fields = result->Fetch();
            Group* group = GetGroupByDbStoreId(fields[0].GetUInt32());
            // group will never be NULL (we have run consistency sql's before loading)

            MapEntry const* mapEntry = sMapStore.LookupEntry(fields[1].GetUInt16());
            if (!mapEntry || !mapEntry->IsInstance())
            {
                SF_LOG_ERROR("sql.sql", "Incorrect entry in party_instance table : no dungeon map %d", fields[1].GetUInt16());
                continue;
            }

            uint32 diff = fields[4].GetUInt8();
            DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(diff);
            if (!difficultyEntry || difficultyEntry->maptype != mapEntry->map_type)
                continue;

            InstanceSave* save = sInstanceSaveMgr->AddInstanceSave(mapEntry->MapID, fields[2].GetUInt32(), DifficultyID(diff), time_t(fields[5].GetUInt32()), (bool)fields[6].GetUInt64(), true);
            group->BindToInstance(save, fields[3].GetBool(), true);
            ++count;
        }
        while (result->NextRow());

        SF_LOG_INFO("server.loading", ">> Loaded %u group-instance saves in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }
}
