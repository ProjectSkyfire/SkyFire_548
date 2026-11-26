/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Creature.h"
#include "CreatureGroups.h"
#include "ObjectMgr.h"

#include "CreatureAI.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include <limits>

#define MAX_DESYNC 5.0f

FormationMgr::~FormationMgr()
{
    for (CreatureGroupInfoType::iterator itr = CreatureGroupMap.begin(); itr != CreatureGroupMap.end(); ++itr)
        delete itr->second;
}

void FormationMgr::AddCreatureToGroup(uint32 groupId, Creature* member)
{
    Map* map = member->FindMap();
    if (!map)
        return;

    CreatureGroupHolderType::iterator itr = map->CreatureGroupHolder.find(groupId);

    //Add member to an existing group
    if (itr != map->CreatureGroupHolder.end())
    {
        SF_LOG_DEBUG("entities.unit", "Group found: %u, inserting creature GUID: %u, Group InstanceID %u", groupId, member->GetGUIDLow(), member->GetInstanceId());
        itr->second->AddMember(member);
    }
    //Create new group
    else
    {
        SF_LOG_DEBUG("entities.unit", "Group not found: %u. Creating new group.", groupId);
        CreatureGroup* group = new CreatureGroup(groupId);
        map->CreatureGroupHolder[groupId] = group;
        group->AddMember(member);
    }
}

void FormationMgr::RemoveCreatureFromGroup(CreatureGroup* group, Creature* member)
{
    SF_LOG_DEBUG("entities.unit", "Deleting member pointer to GUID: %u from group %u", group->GetId(), member->GetDBTableGUIDLow());
    group->RemoveMember(member);

    if (group->isEmpty())
    {
        Map* map = member->FindMap();
        if (!map)
            return;

        SF_LOG_DEBUG("entities.unit", "Deleting group with InstanceID %u", member->GetInstanceId());
        map->CreatureGroupHolder.erase(group->GetId());
        delete group;
    }
}

void FormationMgr::LoadCreatureFormations()
{
    uint32 oldMSTime = getMSTime();

    for (CreatureGroupInfoType::iterator itr = CreatureGroupMap.begin(); itr != CreatureGroupMap.end(); ++itr) // for reload case
        delete itr->second;
    CreatureGroupMap.clear();

    //Get group data
    QueryResult result = WorldDatabase.Query("SELECT leaderGUID, memberGUID, dist, angle, groupAI, point_1, point_2 FROM creature_formations ORDER BY leaderGUID");

    if (!result)
    {
        SF_LOG_ERROR("server.loading", ">>  Loaded 0 creatures in formations. DB table `creature_formations` is empty!");
        return;
    }

    uint32 count = 0;
    Field* fields;
    FormationInfo* group_member;

    do
    {
        fields = result->Fetch();

        //Load group member data
        group_member = new FormationInfo();
        group_member->leaderGUID = fields[0].GetUInt32();
        uint32 memberGUID = fields[1].GetUInt32();
        group_member->groupAI = fields[4].GetUInt32();
        group_member->point_1 = fields[5].GetUInt16();
        group_member->point_2 = fields[6].GetUInt16();
        //If creature is group leader we may skip loading of dist/angle
        if (group_member->leaderGUID != memberGUID)
        {
            group_member->follow_dist = fields[2].GetFloat();
            group_member->follow_angle = fields[3].GetFloat() * M_PI / 180;
        }
        else
        {
            group_member->follow_dist = 0;
            group_member->follow_angle = 0;
        }

        // check data correctness
        {
            if (!sObjectMgr->GetCreatureData(group_member->leaderGUID))
            {
                SF_LOG_ERROR("sql.sql", "creature_formations table leader guid %u incorrect (not exist)", group_member->leaderGUID);
                delete group_member;
                continue;
            }

            if (!sObjectMgr->GetCreatureData(memberGUID))
            {
                SF_LOG_ERROR("sql.sql", "creature_formations table member guid %u incorrect (not exist)", memberGUID);
                delete group_member;
                continue;
            }
        }

        CreatureGroupMap[memberGUID] = group_member;
        ++count;
    } while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u creatures in formations in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void CreatureGroup::AddMember(Creature* member)
{
    SF_LOG_DEBUG("entities.unit", "CreatureGroup::AddMember: Adding unit GUID: %u.", member->GetGUIDLow());

    //Check if it is a leader
    if (member->GetDBTableGUIDLow() == m_groupID)
    {
        SF_LOG_DEBUG("entities.unit", "Unit GUID: %u is formation leader. Adding group.", member->GetGUIDLow());
        m_leader = member;
    }

    m_members[member] = sFormationMgr->CreatureGroupMap.find(member->GetDBTableGUIDLow())->second;
    member->SetFormation(this);
}

void CreatureGroup::RemoveMember(Creature* member)
{
    m_smoothFlightMembers.erase(member);
    if (m_leader == member)
        m_leader = NULL;

    m_members.erase(member);
    member->SetFormation(NULL);
}

void CreatureGroup::MemberAttackStart(Creature* member, Unit* target)
{
    uint8 groupAI = sFormationMgr->CreatureGroupMap[member->GetDBTableGUIDLow()]->groupAI;
    if (!groupAI)
        return;

    if (groupAI == 1 && member != m_leader)
        return;

    for (CreatureGroupMemberType::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
    {
        if (m_leader) // avoid crash if leader was killed and reset.
            SF_LOG_DEBUG("entities.unit", "GROUP ATTACK: group instance id %u calls member instid %u", m_leader->GetInstanceId(), member->GetInstanceId());

        Creature* other = itr->first;

        // Skip self
        if (other == member)
            continue;

        if (!other->IsAlive())
            continue;

        if (other->GetVictim())
            continue;

        if (other->IsValidAttackTarget(target))
            other->AI()->AttackStart(target);
    }
}

void CreatureGroup::FormationReset(bool dismiss)
{
    m_smoothFlightMembers.clear();
    m_smoothLeaderSplineId = 0;
    for (CreatureGroupMemberType::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
    {
        if (itr->first != m_leader && itr->first->IsAlive())
        {
            if (dismiss)
                itr->first->GetMotionMaster()->Initialize();
            else
                itr->first->GetMotionMaster()->MoveIdle();
            SF_LOG_DEBUG("entities.unit", "Set %s movement for member GUID: %u", dismiss ? "default" : "idle", itr->first->GetGUIDLow());
        }
    }
    m_Formed = !dismiss;
}

void CreatureGroup::UpdateSmoothFlightFollowers()
{
    if (!m_leader || !m_leader->movespline->isCyclic() || m_leader->movespline->Finalized())
        return;
    if (m_smoothLeaderSplineId != m_leader->movespline->GetId())
    {
        m_smoothFlightMembers.clear();
        m_smoothLeaderSplineId = m_leader->movespline->GetId();
    }
    CreatureData const* leaderData = sObjectMgr->GetCreatureData(m_leader->GetDBTableGUIDLow());
    if (!leaderData)
        return;
    auto const& spline = m_leader->movespline->_Spline();
    uint32 const count = spline.last() - spline.first();
    uint32 const duration = m_leader->movespline->Duration();
    uint32 const elapsed = m_leader->movespline->TimePassed();
    if (!count || !duration)
        return;

    for (auto const& entry : m_members)
    {
        Creature* member = entry.first;
        if (member == m_leader || !member->IsAlive() || !member->CanFly() || member->IsInCombat() ||
            member->HasUnitState(UNIT_STATE_NOT_MOVE))
            continue;
        CreatureData const* data = sObjectMgr->GetCreatureData(member->GetDBTableGUIDLow());
        if (!data)
            continue;
        G3D::Vector3 offset(data->posX - leaderData->posX, data->posY - leaderData->posY, data->posZ - leaderData->posZ);
        SmoothFlightMember& state = m_smoothFlightMembers[member];
        if (state.splineId == member->movespline->GetId() && !member->movespline->Finalized())
            continue;

        uint32 start = 0;
        bool launchCycle = false;
        if (state.joining && state.splineId == member->movespline->GetId())
        {
            start = state.joinNode;
            launchCycle = true;
        }
        else if (elapsed < 100 && member->GetExactDistSq(
            spline.getPoint(spline.first()).x + offset.x, spline.getPoint(spline.first()).y + offset.y,
            spline.getPoint(spline.first()).z + offset.z) < 1.0f)
            launchCycle = true;

        Movement::MoveSplineInit init(member);
        if (launchCycle)
        {
            Movement::PointsArray points;
            for (uint32 i = 0; i < count; ++i)
                points.push_back(spline.getPoint(spline.first() + (start + i) % count) + offset);
            init.MovebyPath(points);
            init.SetCyclic();
            init.SetDuration(duration);
            state.joining = false;
        }
        else
        {
            // A late-loading or returning guard intercepts a future control point
            // before joining the shared lap clock. No teleports or deformed loops.
            uint32 bestTime = std::numeric_limits<uint32>::max();
            float bestDistance = 0.0f;
            G3D::Vector3 destination;
            float const maxSpeed = std::max(1.0f, m_leader->GetSpeed(MOVE_RUN) * 2.0f);
            for (uint32 i = 0; i < count; ++i)
            {
                G3D::Vector3 point = spline.getPoint(spline.first() + i) + offset;
                float dist = member->GetExactDist(point.x, point.y, point.z);
                if (dist < 0.25f)
                    continue;
                uint32 time = (uint32(spline.length(spline.first() + i)) + duration - elapsed) % duration;
                while (time < 500 || float(time) * maxSpeed < dist * 1000.0f)
                    time += duration;
                if (time < bestTime)
                {
                    bestTime = time;
                    bestDistance = dist;
                    destination = point;
                    state.joinNode = i;
                }
            }
            if (bestTime == std::numeric_limits<uint32>::max())
                continue;
            init.MoveTo(destination.x, destination.y, destination.z, false);
            init.SetVelocity(std::max(0.11f, bestDistance * 1000.0f / float(bestTime)));
            init.SetDuration(bestTime);
            state.joining = true;
        }
        init.SetFly();
        init.SetSmooth();
        init.SetUncompressed();
        init.SetWalk(m_leader->IsWalking());
        member->SetWalk(m_leader->IsWalking());
        if (init.Launch())
            state.splineId = member->movespline->GetId();
    }
}

void CreatureGroup::LeaderMoveTo(float x, float y, float z)
{
    //! To do: This should probably get its own movement generator or use WaypointMovementGenerator.
    //! If the leader's path is known, member's path can be plotted as well using formation offsets.
    if (!m_leader)
        return;

    float pathangle = atan2(m_leader->GetPositionY() - y, m_leader->GetPositionX() - x);

    for (CreatureGroupMemberType::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
    {
        Creature* member = itr->first;
        if (member == m_leader || !member->IsAlive() || member->GetVictim())
            continue;

        if (itr->second->point_1)
        {
            if (m_leader->GetCurrentWaypointID() == itr->second->point_1)
                itr->second->follow_angle = (2 * M_PI) - itr->second->follow_angle;
            if (m_leader->GetCurrentWaypointID() == itr->second->point_2)
                itr->second->follow_angle = (2 * M_PI) + itr->second->follow_angle;
        }

        float angle = itr->second->follow_angle;
        float dist = itr->second->follow_dist;

        float dx = x + std::cos(angle + pathangle) * dist;
        float dy = y + std::sin(angle + pathangle) * dist;
        float dz = z;

        Skyfire::NormalizeMapCoord(dx);
        Skyfire::NormalizeMapCoord(dy);

        // Airborne formations must retain the leader's flight altitude. Ground
        // followers still need projection onto the terrain at their offset.
        if (member->CanFly())
            member->UpdateAllowedPositionZ(dx, dy, dz);
        else
            member->UpdateGroundPositionZ(dx, dy, dz);

        if (member->IsWithinDist(m_leader, dist + MAX_DESYNC))
            member->SetUnitMovementFlags(m_leader->GetUnitMovementFlags());
        else
            member->SetWalk(false);

        member->GetMotionMaster()->MovePoint(0, dx, dy, dz);
        member->SetHomePosition(dx, dy, dz, pathangle);
    }
}
