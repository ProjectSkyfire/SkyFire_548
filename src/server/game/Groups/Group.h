/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_GROUP_H
#define SKYFIRESERVER_GROUP_H

#include "DBCEnums.h"
#include "GroupRefManager.h"
#include "LootMgr.h"
#include "QueryResult.h"
#include "SharedDefines.h"

class Battlefield;
class Battleground;
class Creature;
class GroupReference;
class InstanceSave;
class Map;
class Player;
class Unit;
class WorldObject;
class WorldPacket;
class WorldSession;

struct MapEntry;

#define MAXGROUPSIZE 5
#define MAXRAIDSIZE 40
#define MAX_RAID_SUBGROUPS MAXRAIDSIZE/MAXGROUPSIZE
#define TARGETICONCOUNT 8

enum GroupMemberOnlineStatus
{
    MEMBER_STATUS_OFFLINE   = 0x0000,
    MEMBER_STATUS_ONLINE    = 0x0001,                       // Lua_UnitIsConnected
    MEMBER_STATUS_PVP       = 0x0002,                       // Lua_UnitIsPVP
    MEMBER_STATUS_DEAD      = 0x0004,                       // Lua_UnitIsDead
    MEMBER_STATUS_GHOST     = 0x0008,                       // Lua_UnitIsGhost
    MEMBER_STATUS_PVP_FFA   = 0x0010,                       // Lua_UnitIsPVPFreeForAll
    MEMBER_STATUS_UNK3      = 0x0020,                       // used in calls from Lua_GetPlayerMapPosition/Lua_GetBattlefieldFlagPosition
    MEMBER_STATUS_AFK       = 0x0040,                       // Lua_UnitIsAFK
    MEMBER_STATUS_DND       = 0x0080                        // Lua_UnitIsDND
};

enum class GroupMemberFlags
{
    MEMBER_FLAG_ASSISTANT   = 0x01,
    MEMBER_FLAG_MAINTANK    = 0x02,
    MEMBER_FLAG_MAINASSIST  = 0x04
};

enum class GroupMemberAssignment
{
    GROUP_ASSIGN_MAINTANK   = 0,
    GROUP_ASSIGN_MAINASSIST = 1
};

enum GroupType
{
    GROUPTYPE_NORMAL                = 0x00,
    GROUPTYPE_BG                    = 0x01,
    GROUPTYPE_RAID                  = 0x02,
    GROUPTYPE_BGRAID = GROUPTYPE_BG | GROUPTYPE_RAID,       // mask
    GROUPTYPE_UNK1                  = 0x04,
    GROUPTYPE_LFG                   = 0x08,
    GROUPTYPE_EVERYONE_IS_ASSISTANT = 0x40
    // 0x10, leave/change group?, I saw this flag when leaving group and after leaving BG while in group
};

enum GroupUpdateFlags
{
    GROUP_UPDATE_FLAG_NONE              = 0x00000000,       // nothing
    GROUP_UPDATE_FLAG_STATUS            = 0x00000001,       // uint16 (GroupMemberStatusFlag)
    GROUP_UPDATE_FLAG_UNK               = 0x00000002,       // uint8 (), uint8 ()
    GROUP_UPDATE_FLAG_CUR_HP            = 0x00000004,       // uint32 (HP)
    GROUP_UPDATE_FLAG_MAX_HP            = 0x00000008,       // uint32 (HP)
    GROUP_UPDATE_FLAG_POWER_TYPE        = 0x00000010,       // uint8 (PowerType)
    GROUP_UPDATE_FLAG_UNK2              = 0x00000020,
    GROUP_UPDATE_FLAG_CUR_POWER         = 0x00000040,       // int16 (power value)
    GROUP_UPDATE_FLAG_MAX_POWER         = 0x00000080,       // int16 (power value)
    GROUP_UPDATE_FLAG_LEVEL             = 0x00000100,       // uint16 (level value)
    GROUP_UPDATE_FLAG_ZONE              = 0x00000200,       // uint16 (zone id)
    GROUP_UPDATE_FLAG_UNK400            = 0x00000400,       // int16 ()
    GROUP_UPDATE_FLAG_POSITION          = 0x00000800,       // uint16 (x), uint16 (y), uint16 (z)
    GROUP_UPDATE_FLAG_AURAS             = 0x00001000,       // uint8 (unk), uint64 (mask), uint32 (count), for each bit set: uint32 (spell id) + uint16 (AuraFlags)  (if has flags Scalable -> 3x int32 (bps))
    GROUP_UPDATE_FLAG_PET_GUID          = 0x00002000,       // uint64 (pet guid)
    GROUP_UPDATE_FLAG_PET_NAME          = 0x00004000,       // cstring (name, NULL terminated string)
    GROUP_UPDATE_FLAG_PET_MODEL_ID      = 0x00008000,       // uint16 (model id)
    GROUP_UPDATE_FLAG_PET_CUR_HP        = 0x00010000,       // uint32 (HP)
    GROUP_UPDATE_FLAG_PET_MAX_HP        = 0x00020000,       // uint32 (HP)
    GROUP_UPDATE_FLAG_PET_POWER_TYPE    = 0x00040000,       // uint8 (PowerType)
    GROUP_UPDATE_FLAG_PET_UNK2          = 0x00080000,
    GROUP_UPDATE_FLAG_PET_CUR_POWER     = 0x00100000,       // uint16 (power value)
    GROUP_UPDATE_FLAG_PET_MAX_POWER     = 0x00200000,       // uint16 (power value)
    GROUP_UPDATE_FLAG_PET_AURAS         = 0x00400000,       // [see GROUP_UPDATE_FLAG_AURAS]
    GROUP_UPDATE_FLAG_VEHICLE_SEAT      = 0x00800000,       // int32 (vehicle seat id)
    GROUP_UPDATE_FLAG_PHASE             = 0x01000000,       // int32 (unk), uint32 (phase count), for (count) uint16(phaseId)

    GROUP_UPDATE_PET = GROUP_UPDATE_FLAG_PET_GUID | GROUP_UPDATE_FLAG_PET_NAME | GROUP_UPDATE_FLAG_PET_MODEL_ID |
                       GROUP_UPDATE_FLAG_PET_CUR_HP | GROUP_UPDATE_FLAG_PET_MAX_HP | GROUP_UPDATE_FLAG_PET_POWER_TYPE |
                       GROUP_UPDATE_FLAG_PET_CUR_POWER | GROUP_UPDATE_FLAG_PET_MAX_POWER /*|  GROUP_UPDATE_FLAG_PET_AURAS*/, // all pet flags

    GROUP_UPDATE_FULL = GROUP_UPDATE_FLAG_STATUS | GROUP_UPDATE_FLAG_CUR_HP | GROUP_UPDATE_FLAG_MAX_HP |
                        GROUP_UPDATE_FLAG_POWER_TYPE | GROUP_UPDATE_FLAG_CUR_POWER | GROUP_UPDATE_FLAG_MAX_POWER |
                        GROUP_UPDATE_FLAG_LEVEL | GROUP_UPDATE_FLAG_ZONE | GROUP_UPDATE_FLAG_POSITION |
                     /* GROUP_UPDATE_FLAG_AURAS | */GROUP_UPDATE_PET | GROUP_UPDATE_FLAG_PHASE // all known flags, VEHICLE_SEAT
};

class Roll : public LootValidatorRef
{
    public:
        Roll(uint64 _guid, LootItem const& li);
        ~Roll();
        void setLoot(Loot* pLoot);
        Loot* getLoot();
        void targetObjectBuildLink();

        uint64 itemGUID;
        uint32 itemid;
        int32  itemRandomPropId;
        uint32 itemRandomSuffix;
        uint8 itemCount;
        typedef std::map<uint64, RollType> PlayerVote;
        PlayerVote playerVote;                              //vote position correspond with player position (in group)
        uint8 totalPlayersRolling;
        uint8 totalNeed;
        uint8 totalGreed;
        uint8 totalPass;
        uint8 itemSlot;
        uint8 rollVoteMask;
};

struct InstanceGroupBind
{
    InstanceSave* save;
    bool perm;
    /* permanent InstanceGroupBinds exist if the leader has a permanent
       PlayerInstanceBind for the same instance. */
    InstanceGroupBind() : save(NULL), perm(false) { }
};

/** request member stats checken **/
/// @todo uninvite people that not accepted invite
class Group
{
    public:
        struct MemberSlot
        {
            MemberSlot() : guid(0), group(0), flags(0), roles(0), readyCheckHasResponded(false) { }
            uint64      guid;
            std::string name;
            uint8       group;
            uint8       flags;
            uint8       roles;
            bool        readyCheckHasResponded;
        };
        typedef std::list<MemberSlot> MemberSlotList;
        typedef MemberSlotList::const_iterator member_citerator;

        typedef UNORDERED_MAP< uint32 /*mapId*/, InstanceGroupBind> BoundInstancesMap;
    protected:
        typedef MemberSlotList::iterator member_witerator;
        typedef std::set<Player*> InvitesList;

        typedef std::vector<Roll*> Rolls;

    public:
        Group();
        ~Group();

        // group manipulation methods
        bool   Create(Player* leader);
        void   LoadGroupFromDB(Field* field);
        void   LoadMemberFromDB(uint32 guidLow, uint8 memberFlags, uint8 subgroup, uint8 roles);
        bool   AddInvite(Player* player);
        void   RemoveInvite(Player* player);
        void   RemoveAllInvites();
        bool   AddLeaderInvite(Player* player);
        bool   AddMember(Player* player);
        bool   RemoveMember(uint64 guid, const RemoveMethod &method = GROUP_REMOVEMETHOD_DEFAULT, uint64 kicker = 0, const char* reason = NULL);
        void   ChangeLeader(uint64 guid);
        void   SetLootMethod(LootMethod method);
        void   SetLooterGuid(uint64 guid);
        void   UpdateLooterGuid(WorldObject* pLootedObject, bool ifneed = false);
        void   SetLootThreshold(ItemQualities threshold);
        void   Disband(bool hideDestroy=false);
        void   SetLfgRoles(uint64 guid, const uint8 roles);

        // properties accessories
        bool IsFull() const;
        bool isLFGGroup()  const;
        bool isRaidGroup() const;
        bool isBGGroup()   const;
        bool isBFGroup()   const;
        bool IsCreated()   const;
        uint64 GetLeaderGUID() const;
        uint64 GetGUID() const;
        uint32 GetLowGUID() const;
        const char * GetLeaderName() const;
        LootMethod GetLootMethod() const;
        uint64 GetLooterGuid() const;
        ItemQualities GetLootThreshold() const;

        uint32 GetDbStoreId() const { return m_dbStoreId; };

        // member manipulation methods
        bool IsMember(uint64 guid) const;
        bool IsLeader(uint64 guid) const;
        uint64 GetMemberGUID(const std::string& name);
        bool IsAssistant(uint64 guid) const;
        bool ReadyCheckInProgress() const { return _readyCheckInProgress; }
        bool ReadyCheckAllResponded() const;

        Player* GetInvited(uint64 guid) const;
        Player* GetInvited(const std::string& name) const;

        bool SameSubGroup(uint64 guid1, uint64 guid2) const;
        bool SameSubGroup(uint64 guid1, MemberSlot const* slot2) const;
        bool SameSubGroup(Player const* member1, Player const* member2) const;
        bool HasFreeSlotSubGroup(uint8 subgroup) const;

        MemberSlotList const& GetMemberSlots() const { return m_memberSlots; }
        GroupReference* GetFirstMember() { return m_memberMgr.getFirst(); }
        GroupReference const* GetFirstMember() const { return m_memberMgr.getFirst(); }
        uint32 GetMembersCount() const { return m_memberSlots.size(); }

        uint8 GetMemberGroup(uint64 guid) const;

        void ChangeFlagEveryoneAssistant(bool apply);
        void ConvertToLFG();
        void ConvertToRaid();
        void ConvertToGroup();

        void SetBattlegroundGroup(Battleground* bg);
        void SetBattlefieldGroup(Battlefield* bf);
        GroupJoinBattlegroundResult CanJoinBattlegroundQueue(Battleground const* bgOrTemplate, BattlegroundQueueTypeId bgQueueTypeId, uint32 MinPlayerCount, uint32 MaxPlayerCount, bool isRated, uint32 arenaSlot);

        void ChangeMembersGroup(uint64 guid, uint8 group);
        void ChangeMembersGroup(Player* player, uint8 group);
        void SetTargetIcon(uint8 id, ObjectGuid whoGuid, ObjectGuid targetGuid, uint8 Index);
        void SetGroupMemberFlag(uint64 guid, bool apply, GroupMemberFlags flag);
        void RemoveUniqueGroupMemberFlag(GroupMemberFlags flag);

        void SetMemberRole(uint64 guid, uint32 role);
        uint32 GetMemberRole(uint64 guid) const;
        bool RoleCheckAllResponded() const;

        DifficultyID GetDifficulty(MapEntry const* mapEntry) const;
        DifficultyID GetDungeonDifficulty() const { return m_dungeonDifficulty; }
        DifficultyID GetRaidDifficulty() const { return m_raidDifficulty; }
        void SetDungeonDifficulty(DifficultyID difficulty);
        void SetRaidDifficulty(DifficultyID difficulty);
        bool InCombatToInstance(uint32 instanceId);
        void ResetInstances(InstanceResetMethod method, bool isRaid, Player* SendMsgTo);

        void ReadyCheck(bool state) { _readyCheckInProgress = state; }
        void ReadyCheckMemberHasResponded(uint64 guid);
        void ReadyCheckResetResponded();

        // -no description-
        //void SendInit(WorldSession* session);
        void SendTargetIconList(WorldSession* session);
        void SendUpdate();
        void SendUpdateToPlayer(uint64 playerGUID, MemberSlot* slot = NULL);
        void UpdatePlayerOutOfRange(Player* player);
        void SendReadyCheckCompleted();
                                                            // ignore: GUID of player that will be ignored
        void BroadcastPacket(WorldPacket* packet, bool ignorePlayersInBGRaid, int group = -1, uint64 ignore = 0);
        void BroadcastAddonMessagePacket(WorldPacket* packet, const std::string& prefix, bool ignorePlayersInBGRaid, int group = -1, uint64 ignore = 0);
        void BroadcastReadyCheck(WorldPacket* packet);
        void OfflineReadyCheck();

        /*********************************************************/
        /***                   LOOT SYSTEM                     ***/
        /*********************************************************/

        bool isRollLootActive() const;
        void SendLootStartRoll(uint32 CountDown, uint32 mapid, const Roll &r);
        void SendLootStartRollToPlayer(uint32 countDown, uint32 mapId, Player* p, bool canNeed, Roll const& r);
        void SendLootRoll(uint64 SourceGuid, uint64 TargetGuid, uint8 RollNumber, RollType RollType, const Roll &r);
        void SendLootRollWon(uint64 SourceGuid, uint64 TargetGuid, uint8 RollNumber, RollType RollType, const Roll &r);
        void SendLootAllPassed(Roll const& roll);
        void SendLooter(Creature* creature, Player* pLooter);
        void GroupLoot(Loot* loot, WorldObject* pLootedObject);
        void NeedBeforeGreed(Loot* loot, WorldObject* pLootedObject);
        void MasterLoot(Loot* loot, WorldObject* pLootedObject);
        Rolls::iterator GetRoll(uint64 Guid);
        void CountTheRoll(Rolls::iterator roll);
        void CountRollVote(uint64 playerGUID, uint64 Guid, RollType Choise);
        void EndRoll(Loot* loot);

        // related to disenchant rolls
        void ResetMaxEnchantingLevel();

        void LinkMember(GroupReference* pRef);
        void DelinkMember(uint64 guid);

        InstanceGroupBind* BindToInstance(InstanceSave* save, bool permanent, bool load = false);
        void UnbindInstance(uint32 mapid, uint8 difficulty, bool unload = false);
        InstanceGroupBind* GetBoundInstance(Player* player);
        InstanceGroupBind* GetBoundInstance(Map* aMap);
        InstanceGroupBind* GetBoundInstance(MapEntry const* mapEntry);
        InstanceGroupBind* GetBoundInstance(DifficultyID difficulty, uint32 mapId);
        BoundInstancesMap& GetBoundInstances(DifficultyID difficulty);

        // FG: evil hacks
        void BroadcastGroupUpdate(void);

    protected:
        bool _setMembersGroup(uint64 guid, uint8 group);
        void _homebindIfInstance(Player* player);

        void _initRaidSubGroupsCounter();
        member_citerator _getMemberCSlot(uint64 Guid) const;
        member_witerator _getMemberWSlot(uint64 Guid);
        void SubGroupCounterIncrease(uint8 subgroup);
        void SubGroupCounterDecrease(uint8 subgroup);
        void ToggleGroupMemberFlag(member_witerator slot, GroupMemberFlags flag, bool apply);

        MemberSlotList      m_memberSlots;
        GroupRefManager     m_memberMgr;
        InvitesList         m_invitees;
        uint64              m_leaderGuid;
        std::string         m_leaderName;
        GroupType           m_groupType;
        DifficultyID        m_dungeonDifficulty;
        DifficultyID        m_raidDifficulty;
        Battleground*       m_bgGroup;
        Battlefield*        m_bfGroup;
        uint64              m_targetIcons[TARGETICONCOUNT] = { };
        LootMethod          m_lootMethod;
        ItemQualities       m_lootThreshold;
        uint64              m_looterGuid;
        Rolls               RollId;
        BoundInstancesMap   m_boundInstances[15];
        uint8*              m_subGroupsCounts;
        uint64              m_guid;
        uint32              m_counter;                      // used only in SMSG_GROUP_LIST
        uint32              m_maxEnchantingLevel;
        uint32              m_dbStoreId;                    // Represents the ID used in database (Can be reused by other groups if group was disbanded)
        bool                _readyCheckInProgress;
};
#endif
