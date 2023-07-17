/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "Object.h"
#include "Common.h"
#include "SharedDefines.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "Creature.h"
#include "Player.h"
#include "Vehicle.h"
#include "ObjectMgr.h"
#include "UpdateData.h"
#include "UpdateMask.h"
#include "Util.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "Log.h"
#include "Transport.h"
#include "TargetedMovementGenerator.h"
#include "WaypointMovementGenerator.h"
#include "VMapFactory.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellAuraEffects.h"
#include "UpdateFieldFlags.h"
#include "TemporarySummon.h"
#include "Totem.h"
#include "OutdoorPvPMgr.h"
#include "MovementPacketBuilder.h"
#include "DynamicTree.h"
#include "Unit.h"
#include "Group.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Chat.h"

TypeID GuidHigh2TypeId(uint32 guid_hi)
{
    switch (guid_hi)
    {
        case HIGHGUID_ITEM:         return TypeID::TYPEID_ITEM;
        //case HIGHGUID_CONTAINER:    return TYPEID_CONTAINER; HIGHGUID_CONTAINER == HIGHGUID_ITEM currently
        case HIGHGUID_UNIT:         return TypeID::TYPEID_UNIT;
        case HIGHGUID_PET:          return TypeID::TYPEID_UNIT;
        case HIGHGUID_PLAYER:       return TypeID::TYPEID_PLAYER;
        case HIGHGUID_GAMEOBJECT:   return TypeID::TYPEID_GAMEOBJECT;
        case HIGHGUID_DYNAMICOBJECT:return TypeID::TYPEID_DYNAMICOBJECT;
        case HIGHGUID_CORPSE:       return TypeID::TYPEID_CORPSE;
        case HIGHGUID_AREATRIGGER:  return TypeID::TYPEID_AREATRIGGER;
        case HIGHGUID_MO_TRANSPORT: return TypeID::TYPEID_GAMEOBJECT;
        case HIGHGUID_VEHICLE:      return TypeID::TYPEID_UNIT;
    }
    return TypeID::NUM_CLIENT_OBJECT_TYPES;                         // unknown
}

Object::Object() : m_PackGUID(sizeof(uint64)+1), m_objectTypeId(TypeID::TYPEID_OBJECT), m_objectType(TYPEMASK_OBJECT), m_uint32Values(NULL),
                    m_valuesCount(0), m_fieldNotifyFlags(UF_FLAG_URGENT), m_inWorld(false), m_objectUpdated(false)
{
    m_PackGUID.appendPackGUID(0);
}

WorldObject::~WorldObject()
{
    // this may happen because there are many !create/delete
    if (IsWorldObject() && m_currMap)
    {
        if (GetTypeId() == TypeID::TYPEID_CORPSE)
        {
            SF_LOG_FATAL("misc", "Object::~Object Corpse guid=" UI64FMTD ", type=%d, entry=%u deleted but still in map!!",
                GetGUID(), uint8(((Corpse*)this)->GetType()), GetEntry());
            ASSERT(false);
        }
        ResetMap();
    }
}

Object::~Object()
{
    if (IsInWorld())
    {
        SF_LOG_FATAL("misc", "Object::~Object - guid=" UI64FMTD ", typeid=%d, entry=%u deleted but still in world!!", GetGUID(), uint8(GetTypeId()), GetEntry());
        if (isType(TYPEMASK_ITEM))
            SF_LOG_FATAL("misc", "Item slot %u", ((Item*)this)->GetSlot());
        ASSERT(false);
        RemoveFromWorld();
    }

    if (m_objectUpdated)
    {
        SF_LOG_FATAL("misc", "Object::~Object - guid=" UI64FMTD ", typeid=%d, entry=%u deleted but still in update list!!", GetGUID(), uint8(GetTypeId()), GetEntry());
        ASSERT(false);
        sObjectAccessor->RemoveUpdateObject(this);
    }

    delete [] m_uint32Values;
    m_uint32Values = 0;

    for (size_t i = 0; i < m_dynamicTab.size(); ++i)
    {
        delete [] m_dynamicTab[i];
        delete [] m_dynamicChange[i];
    }
}

void Object::_InitValues()
{
    m_uint32Values = new uint32[m_valuesCount];
    memset(m_uint32Values, 0, m_valuesCount*sizeof(uint32));

    _changesMask.SetCount(m_valuesCount);

    for (size_t i = 0; i < m_dynamicTab.size(); ++i)
    {
        memset(m_dynamicTab[i], 0, 32 * sizeof(uint32));
        memset(m_dynamicChange[i], 0, 32 * sizeof(bool));
    }

    m_objectUpdated = false;
}

void Object::_Create(uint32 guidlow, uint32 entry, HighGuid guidhigh)
{
    if (!m_uint32Values) _InitValues();

    uint64 guid = MAKE_NEW_GUID(guidlow, entry, guidhigh);
    SetUInt64Value(OBJECT_FIELD_GUID, guid);
    SetUInt16Value(OBJECT_FIELD_TYPE, 0, m_objectType);
    m_PackGUID.clear();
    m_PackGUID.appendPackGUID(GetGUID());
}

std::string Object::_ConcatFields(uint16 startIndex, uint16 size) const
{
    std::ostringstream ss;
    for (uint16 index = 0; index < size; ++index)
        ss << GetUInt32Value(index + startIndex) << ' ';
    return ss.str();
}

void Object::AddToWorld()
{
    if (m_inWorld)
        return;

    ASSERT(m_uint32Values);

    m_inWorld = true;

    // synchronize values mirror with values array (changes will send in updatecreate opcode any way
    ClearUpdateMask(true);
}

void Object::RemoveFromWorld()
{
    if (!m_inWorld)
        return;

    m_inWorld = false;

    // if we remove from world then sending changes not required
    ClearUpdateMask(true);
}

void Object::BuildCreateUpdateBlockForPlayer(UpdateData* data, Player* target) const
{
    if (!target)
        return;

    uint8  updateType = UPDATETYPE_CREATE_OBJECT;
    uint16 flags      = m_updateFlag;

    /** lower flag1 **/
    if (target == this)                                      // building packet for yourself
        flags |= UPDATEFLAG_SELF;

    switch (GetGUIDHigh())
    {
        case HIGHGUID_PLAYER:
        case HIGHGUID_PET:
        case HIGHGUID_CORPSE:
        case HIGHGUID_DYNAMICOBJECT:
        case HIGHGUID_AREATRIGGER:
            updateType = UPDATETYPE_CREATE_OBJECT2;
            break;
        case HIGHGUID_UNIT:
        case HIGHGUID_VEHICLE:
        {
            if (TempSummon const* summon = ToUnit()->ToTempSummon())
                if (IS_PLAYER_GUID(summon->GetSummonerGUID()))
                    updateType = UPDATETYPE_CREATE_OBJECT2;

            break;
        }
        case HIGHGUID_GAMEOBJECT:
        {
            if (IS_PLAYER_GUID(ToGameObject()->GetOwnerGUID()))
                updateType = UPDATETYPE_CREATE_OBJECT2;
            break;
        }
        default:
            break;
    }

    if (flags & UPDATEFLAG_STATIONARY_POSITION)
    {
        // UPDATETYPE_CREATE_OBJECT2 for some gameobject types...
        if (isType(TYPEMASK_GAMEOBJECT))
        {
            switch (ToGameObject()->GetGoType())
            {
                case GAMEOBJECT_TYPE_TRAP:
                case GAMEOBJECT_TYPE_DUEL_ARBITER:
                case GAMEOBJECT_TYPE_FLAGSTAND:
                case GAMEOBJECT_TYPE_FLAGDROP:
                    updateType = UPDATETYPE_CREATE_OBJECT2;
                    break;
                default:
                    break;
            }
        }
    }

    if (Unit const* unit = ToUnit())
        if (unit->GetVictim())
            flags |= UPDATEFLAG_HAS_TARGET;

    ByteBuffer buf(500);
    buf << uint8(updateType);
    buf.append(GetPackGUID());
    buf << uint8(m_objectTypeId);

    BuildMovementUpdate(&buf, flags);
    BuildValuesUpdate(updateType, &buf, target);
    data->AddUpdateBlock(buf);
}

void Object::SendUpdateToPlayer(Player* player)
{
    // send create update to player
    UpdateData upd(player->GetMapId());
    WorldPacket packet;

    BuildCreateUpdateBlockForPlayer(&upd, player);
    upd.BuildPacket(&packet);
    player->GetSession()->SendPacket(&packet);
}

void Object::BuildValuesUpdateBlockForPlayer(UpdateData* data, Player* target) const
{
    ByteBuffer buf(500);

    buf << uint8(UPDATETYPE_VALUES);
    buf.append(GetPackGUID());

    BuildValuesUpdate(UPDATETYPE_VALUES, &buf, target);

    data->AddUpdateBlock(buf);
}

void Object::BuildOutOfRangeUpdateBlock(UpdateData* data) const
{
    data->AddOutOfRangeGUID(GetGUID());
}

void Object::DestroyForPlayer(Player* target, bool onDeath) const
{
    ASSERT(target);

    if (isType(TYPEMASK_UNIT) || isType(TYPEMASK_PLAYER))
    {
        if (Battleground* bg = target->GetBattleground())
        {
            if (bg->isArena())
            {
                WorldPacket data(SMSG_ARENA_UNIT_DESTROYED, 8);
                data << uint64(GetGUID());
                target->GetSession()->SendPacket(&data);
            }
        }
    }
    SendDestroyObject(target, GetObjectGUID(), onDeath);
}

void Object::SendDestroyObject(Player* target, ObjectGuid Guid, bool onDeath) const
{
    WorldPacket data(SMSG_DESTROY_OBJECT, 1 + 8);
    data.WriteGuidMask(Guid, 3, 2, 4, 1);
    //! If the following bool is true, the client will call "void CGUnit_C::OnDeath()" for this object.
    //! OnDeath() does for eg trigger death animation and interrupts certain spells/missiles/auras/sounds...
    data.WriteBit(onDeath);
    data.WriteGuidMask(Guid, 7, 0, 6, 5);
    data.FlushBits();
    data.WriteGuidBytes(Guid, 0, 4, 7, 2, 6, 3, 1, 5);
    target->GetSession()->SendPacket(&data);
}

int32 Object::GetInt32Value(uint16 index) const
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, false));
    return m_int32Values[index];
}

uint32 Object::GetUInt32Value(uint16 index) const
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, false));
    return m_uint32Values[index];
}

uint64 Object::GetUInt64Value(uint16 index) const
{
    ASSERT(index + 1 < m_valuesCount || PrintIndexError(index, false));
    return *((uint64*)&(m_uint32Values[index]));
}

float Object::GetFloatValue(uint16 index) const
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, false));
    return m_floatValues[index];
}

uint8 Object::GetByteValue(uint16 index, uint8 offset) const
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, false));
    ASSERT(offset < 4);
    return *(((uint8*)&m_uint32Values[index])+offset);
}

uint16 Object::GetUInt16Value(uint16 index, uint8 offset) const
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, false));
    ASSERT(offset < 2);
    return *(((uint16*)&m_uint32Values[index])+offset);
}

uint32 Object::GetDynamicUInt32Value(uint32 tab, uint16 index) const
{
    ASSERT(tab < m_dynamicTab.size() || index < 32);
    return m_dynamicTab[tab][index];
}

void Object::BuildMovementUpdate(ByteBuffer* data, uint16 flags) const
{
    bool hasFallData = false;
    bool hasFallDirection = false;
    bool hasSpline = false;
    bool hasPitch = false;
    bool hasSplineElevation = false;
    bool hasUnitTransport = false;

    uint32 movementFlags;
    uint32 movementFlagsExtra;

    data->WriteBit(0);                                        // 676  UPDATEFLAG_HAS_GAMEOBJECT          // NEW
    data->WriteBit(false /*flags & UPDATEFLAG_ANIMKITS*/);    // 498  UPDATEFLAG_ANIMKITS                // OLD
    data->WriteBit(flags & UPDATEFLAG_LIVING);                // 368  UPDATEFLAG_LIVING                  // OLD
    data->WriteBit(0);                                        // 810  UPDATEFLAG_SCENE_LOCAL_SCRIPT_DATA // NEW
    data->WriteBit(0);
    data->WriteBits(0, 22);                                   // 1068 UPDATEFLAG_TRANSPORT_FRAME_COUNT   // NEW
    data->WriteBit(flags & UPDATEFLAG_VEHICLE);               // 488  UPDATEFLAG_VEHICLE                 // OLD
    data->WriteBit(0);                                        // 1044 UNK
    data->WriteBit(0);
    data->WriteBit(flags & UPDATEFLAG_TRANSPORT);             // 476  UPDATEFLAG_TRANSPORT               // OLD
    data->WriteBit(flags & UPDATEFLAG_ROTATION);              // 512  UPDATEFLAG_ROTATION                // OLD
    data->WriteBit(0);
    data->WriteBit(flags & UPDATEFLAG_SELF);                  // 680  UPDATEFLAG_SELF                    // OLD
    data->WriteBit(flags & UPDATEFLAG_HAS_TARGET);            // 464  UPDATEFLAG_HAS_TARGET              // OLD
    data->WriteBit(0);                                        // 1032 UPDATEFLAG_SCENE_OBJECT            // NEW
    data->WriteBit(0);                                        // 1064 UPDATEFLAG_SCENE_PENDING_INSTANCES // NEW
    data->WriteBit(0);
    data->WriteBit(0);                                        // 668  UPDATEFLAG_HAS_AREATRIGGER         // NEW
    data->WriteBit(flags & UPDATEFLAG_GO_TRANSPORT_POSITION); // 424  UPDATEFLAG_GO_TRANSPORT_POSITION   // OLD
    data->WriteBit(0);                                        // 681  UPDATEFLAG_REPLACE_YOU             // NEW
    data->WriteBit(flags & UPDATEFLAG_STATIONARY_POSITION);   // 448  UPDATEFLAG_STATIONARY_POSITION     // OLD

    if (flags & UPDATEFLAG_LIVING) // 368
    {
        Unit const* self = ToUnit();
        ObjectGuid guid = GetGUID();

        hasFallDirection = self->HasUnitMovementFlag(MOVEMENTFLAG_FALLING);
        hasFallData = hasFallDirection || self->m_movementInfo.jump.fallTime != 0;
        movementFlags = self->GetUnitMovementFlags();
        movementFlagsExtra = self->GetExtraUnitMovementFlags();
        hasSpline = self->IsSplineEnabled() && self->GetTypeId() != TypeID::TYPEID_PLAYER;
        hasPitch = self->HasUnitMovementFlag(MovementFlags(MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_FLYING)) || self->HasExtraUnitMovementFlag(MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING);
        hasSplineElevation = self->HasUnitMovementFlag(MOVEMENTFLAG_SPLINE_ELEVATION);
        hasUnitTransport = self->m_movementInfo.transport.guid;

        if (GetTypeId() == TypeID::TYPEID_UNIT)
            movementFlags &= MOVEMENTFLAG_MASK_CREATURE_ALLOWED;

        data->WriteBit(guid[2]);
        data->WriteBit(0);
        data->WriteBit(!hasPitch);
        data->WriteBit(hasUnitTransport);
        data->WriteBit(0);

        if (hasUnitTransport)
        {
            ObjectGuid transGuid = self->m_movementInfo.transport.guid;
            data->WriteBit(transGuid[4]);
            data->WriteBit(transGuid[2]);
            data->WriteBit(self->m_movementInfo.transport.time3 && self->m_movementInfo.transport.guid);
            data->WriteBit(transGuid[0]);
            data->WriteBit(transGuid[1]);
            data->WriteBit(transGuid[3]);
            data->WriteBit(transGuid[6]);
            data->WriteBit(transGuid[7]);
            data->WriteBit(self->m_movementInfo.transport.time2 && self->m_movementInfo.transport.guid);
            data->WriteBit(transGuid[5]);
        }

        data->WriteBit(!self->m_movementInfo.time);
        data->WriteBit(guid[6]);
        data->WriteBit(guid[4]);
        data->WriteBit(guid[3]);

        data->WriteBit(G3D::fuzzyEq(self->GetOrientation(), 0.0f)); //!G3D::fuzzyEq(self->GetOrientation(), 0.0f)

        data->WriteBit(!self->GetMovementCounter());
        data->WriteBit(guid[5]);
        data->WriteBits(0, 22); // Forces
        data->WriteBit(!movementFlags);
        data->WriteBits(0, 19);
        data->WriteBit(hasFallData);

        if (movementFlags)
            data->WriteBits(movementFlags, 30);

        data->WriteBit(!hasSplineElevation);
        data->WriteBit(hasSpline);
        data->WriteBit(0);
        data->WriteBit(guid[0]);
        data->WriteBit(guid[7]);
        data->WriteBit(guid[1]);

        if (hasSpline)
            Movement::PacketBuilder::WriteCreateBits(*self->movespline, *data);

        data->WriteBit(!movementFlagsExtra);

        if (hasFallData)
            data->WriteBit(hasFallDirection);

        if (movementFlagsExtra)
           data->WriteBits(movementFlagsExtra, 13);
    }

    // 1032 {}

    if (flags & UPDATEFLAG_GO_TRANSPORT_POSITION) // 424
    {
        WorldObject const* self = static_cast<WorldObject const*>(this);
        ObjectGuid transGuid = self->m_movementInfo.transport.guid;
        data->WriteBit(transGuid[4]);
        data->WriteBit(transGuid[1]);
        data->WriteBit(transGuid[0]);
        data->WriteBit(self->m_movementInfo.transport.time2 && self->m_movementInfo.transport.guid);
        data->WriteBit(transGuid[6]);
        data->WriteBit(transGuid[5]);
        data->WriteBit(transGuid[3]);
        data->WriteBit(transGuid[2]);
        data->WriteBit(transGuid[7]);
        data->WriteBit(self->m_movementInfo.transport.time3 && self->m_movementInfo.transport.guid);
    }

    /*
    if (flags & UPDATEFLAG_HAS_AREATRIGGER) // 668
    {
        data->WriteBit(0); // 528
        data->WriteBit(0); // 600
        data->WriteBit(0); // 544
        data->WriteBit(0); // 526
        data->WriteBit(0); // 552
        data->WriteBit(0); // 524
        data->WriteBit(0); // 572
        data->WriteBit(0); // 525
        data->WriteBit(0); // 664 has664
        data->WriteBit(0); // 527
        
        if (664) // 664
            data->WriteBits(0, 20); // v70
            
        data->WriteBit(0); // 536
        data->WriteBit(0); // 644 has644
        data->WriteBit(0); // 560
        
        if (644) // 644
        {
            //v4 = sub_6652D0(v7, v6) << (13 - v3);
            //v3 += 8;
            //v2 |= v4;
            data->WriteBits(0, 21); // v71 // 151
            data->WriteBits(0, 21); // v72 // 155
        }
    }
    */

    /*
    if (flags & UPDATEFLAG_ANIMKITS) // 498
    {
        data->WriteBit(1);  // Missing AnimKit1
        data->WriteBit(1);  // Missing AnimKit2
        data->WriteBit(1);  // Missing AnimKit3
    }
    */

    if (flags & UPDATEFLAG_HAS_TARGET) // 464
    {
        Unit const* self = ToUnit();
        ObjectGuid victimGuid = self->GetVictim()->GetGUID();
        data->WriteBit(victimGuid[4]);
        data->WriteBit(victimGuid[6]);
        data->WriteBit(victimGuid[5]);
        data->WriteBit(victimGuid[2]);
        data->WriteBit(victimGuid[0]);
        data->WriteBit(victimGuid[1]);
        data->WriteBit(victimGuid[3]);
        data->WriteBit(victimGuid[7]);
    }

    // 1064 {}
    // 810 {}

    data->FlushBits();

    // 1068 {}
    // 1032 {}

    if (flags & UPDATEFLAG_LIVING)
    {
        Unit const* self = ToUnit();
        ObjectGuid guid = GetGUID();

        if (hasUnitTransport)
        {
            ObjectGuid transGuid = self->m_movementInfo.transport.guid;

            data->WriteByteSeq(transGuid[7]);
            *data << float(self->GetTransOffsetX());

            if (self->m_movementInfo.transport.time3 && self->m_movementInfo.transport.guid)
                *data << uint32(self->m_movementInfo.transport.time3);

            *data << float(self->GetTransOffsetO());
            *data << float(self->GetTransOffsetY());
            data->WriteByteSeq(transGuid[4]);
            data->WriteByteSeq(transGuid[1]);
            data->WriteByteSeq(transGuid[3]);
            *data << float(self->GetTransOffsetZ());
            data->WriteByteSeq(transGuid[5]);

            if (self->m_movementInfo.transport.time2 && self->m_movementInfo.transport.guid)
                *data << uint32(self->m_movementInfo.transport.time2);

            data->WriteByteSeq(transGuid[0]);
            *data << int8(self->GetTransSeat());
            data->WriteByteSeq(transGuid[6]);
            data->WriteByteSeq(transGuid[2]);
            *data << uint32(self->GetTransTime());
        }

        data->WriteByteSeq(guid[4]);

        if (hasSpline)
            Movement::PacketBuilder::WriteCreateData(*self->movespline, *data);

        *data << float(self->GetSpeed(MOVE_FLIGHT));

        if (self->GetMovementCounter())
            *data << uint32(self->GetMovementCounter());

        data->WriteByteSeq(guid[2]);

        if (hasFallData)
        {
            if (hasFallDirection)
            {
                *data << float(self->m_movementInfo.jump.xyspeed);
                *data << float(self->m_movementInfo.jump.cosAngle);
                *data << float(self->m_movementInfo.jump.sinAngle);
            }

            *data << uint32(self->m_movementInfo.jump.fallTime);
            *data << float(self->m_movementInfo.jump.zspeed);
        }

        data->WriteByteSeq(guid[1]);
        *data << float(self->GetSpeed(MOVE_TURN_RATE));

        if (self->m_movementInfo.time)
            *data << uint32(self->m_movementInfo.time);

        *data << float(self->GetSpeed(MOVE_RUN_BACK));

        if (hasSplineElevation)
            *data << float(self->m_movementInfo.splineElevation);

        data->WriteByteSeq(guid[7]);
        *data << float(self->GetSpeed(MOVE_PITCH_RATE));
        *data << float(self->GetPositionX());

        if (hasPitch)
            *data << float(self->m_movementInfo.pitch);

        if (!G3D::fuzzyEq(self->GetOrientation(), 0.0f))
            *data << float(Position::NormalizeOrientation(self->GetOrientation()));

        *data << float(self->GetSpeed(MOVE_WALK));
        *data << float(self->GetPositionY());
        *data << float(self->GetSpeed(MOVE_FLIGHT_BACK));
        data->WriteByteSeq(guid[3]);
        data->WriteByteSeq(guid[5]);
        data->WriteByteSeq(guid[6]);
        data->WriteByteSeq(guid[0]);
        *data << float(self->GetSpeed(MOVE_SWIM_BACK));
        *data << float(self->GetSpeed(MOVE_RUN));
        *data << float(self->GetSpeed(MOVE_SWIM));
        *data << float(self->GetPositionZMinusOffset());
    }

    if (flags & UPDATEFLAG_GO_TRANSPORT_POSITION)
    {
        WorldObject const* self = static_cast<WorldObject const*>(this);
        ObjectGuid transGuid = self->m_movementInfo.transport.guid;

        if (self->m_movementInfo.transport.time2 && self->m_movementInfo.transport.guid)
            *data << uint32(self->m_movementInfo.transport.time2);

        *data << float(self->GetTransOffsetY());
        *data << int8(self->GetTransSeat());
        *data << float(self->GetTransOffsetX());
        data->WriteBit(transGuid[2]);
        data->WriteBit(transGuid[4]);
        data->WriteBit(transGuid[1]);

        if (self->m_movementInfo.transport.time3 && self->m_movementInfo.transport.guid)
            *data << uint32(self->m_movementInfo.transport.time3);

        *data << uint32(self->GetTransTime());

        *data << float(self->GetTransOffsetO());
        *data << float(self->GetTransOffsetZ());

        data->WriteBit(transGuid[6]);
        data->WriteBit(transGuid[0]);
        data->WriteBit(transGuid[5]);
        data->WriteBit(transGuid[3]);
        data->WriteBit(transGuid[7]);
    }

    /*
    if (flags & UPDATEFLAG_HAS_AREATRIGGER) // 668
    {
        if (664) 
        {
            //forloop12 162
            *data << float(0.0f); //v107 // 4
            *data << float(0.0f); //v108 // 0
            *data << float(0.0f); //v109 // 8
        }
        if (600)
        {
            *data << float(0.0f); // 146
            *data << float(0.0f); // 145
            *data << float(0.0f); // 149
            *data << float(0.0f); // 148
            *data << float(0.0f); // 144
            *data << float(0.0f); // 147
        }
        if (644)
        {
            if(151)
            {
                //forloop 151
                *data << float(0.0f); //v110 // 0 // 152
                *data << float(0.0f); //v111 // 4 // 152
            }
            
            if (155)
            {
                //forloop 155
                *data << float(0.0f); //v112 // 0 // 156
                *data << float(0.0f); //v113 // 4 // 156
            }
        
            *data << float(0.0f); // 160
            *data << float(0.0f); // 159
        }
        *data << uint32(0); //130

        if (544)
            *data << uint32(0); //135

        if (552)
            *data << uint32(0); //137

        if (536)
            *data << uint32(0); //133

        if (560)
            *data << uint32(0); //139

        if (572)
        {
            *data << float(0.0f); // 141
            *data << float(0.0f); // 142
        }
    }
    */

    if (flags & UPDATEFLAG_HAS_TARGET) // 464
    {
        Unit const* self = ToUnit();
        ObjectGuid victimGuid = self->GetVictim()->GetGUID();
        data->WriteByteSeq(victimGuid[7]);
        data->WriteByteSeq(victimGuid[1]);
        data->WriteByteSeq(victimGuid[5]);
        data->WriteByteSeq(victimGuid[2]);
        data->WriteByteSeq(victimGuid[6]);
        data->WriteByteSeq(victimGuid[3]);
        data->WriteByteSeq(victimGuid[0]);
        data->WriteByteSeq(victimGuid[4]);
    }
    
    if (flags & UPDATEFLAG_VEHICLE)
    {
        Unit const* self = ToUnit();
        *data << uint32(self->GetVehicleKit()->GetVehicleInfo()->m_ID);
        *data << float(self->GetOrientation());
    }

    if (flags & UPDATEFLAG_STATIONARY_POSITION)
    {
        WorldObject const* self = static_cast<WorldObject const*>(this);
        *data << float(self->GetPositionY());
        if (Unit const* unit = ToUnit())
            *data << float(unit->GetPositionZMinusOffset());
        else
            *data << float(self->GetPositionZ());
        *data << float(Position::NormalizeOrientation(self->GetOrientation()));
        *data << float(self->GetPositionX());
    }

    // 676 {}
    /*
    if (flags & UPDATEFLAG_ANIMKITS)
    {
        if (hasAnimKit3)
            *data << uint16(animKit3);
        if (hasAnimKit2)
            *data << uint16(animKit2);
        if (hasAnimKit1)
            *data << uint16(animKit1);
    }*/

    // 810 {}
    if (flags & UPDATEFLAG_TRANSPORT)
    {
        GameObject const* go = ToGameObject();

        if (go && go->ToTransport())
            *data << uint32(go->GetGOValue()->Transport.PathProgress);
        else
            *data << uint32(getMSTime());
    }
    // 1064 {}
    if (flags & UPDATEFLAG_ROTATION)
        *data << uint64(ToGameObject()->GetRotation());
    // 1044 {}
    if (flags & UPDATEFLAG_LIVING && hasSpline)
        Movement::PacketBuilder::WriteFacingTargetPart(*ToUnit()->movespline, *data);
}

void Object::BuildValuesUpdate(uint8 updateType, ByteBuffer* data, Player* target) const
{
    if (!target)
        return;

    ByteBuffer fieldBuffer;
    UpdateMask updateMask;
    updateMask.SetCount(m_valuesCount);

    uint32* flags = NULL;
    uint32 visibleFlag = GetUpdateFieldData(target, flags);

    for (uint16 index = 0; index < m_valuesCount; ++index)
    {
        if ((m_fieldNotifyFlags & flags[index] ||
             ((updateType == UPDATETYPE_VALUES ? _changesMask.GetBit(index) : m_uint32Values[index]) && (flags[index] & visibleFlag))))
        {
            updateMask.SetBit(index);
            fieldBuffer << m_uint32Values[index];
        }
    }

    *data << uint8(updateMask.GetBlockCount());
    updateMask.AppendToPacket(data);
    data->append(fieldBuffer);
    BuildDynamicValuesUpdate(data);
}

void Object::BuildDynamicValuesUpdate(ByteBuffer *data) const
{
    // Only handle Item type
    // TODO: Implement Player dynamis fields
    if (m_objectTypeId != TypeID::TYPEID_ITEM)
    {
        *data << uint8(0);
        return;
    }

    // Dynamic Fields (5.0.5 MoP new fields)
    uint32 dynamicTabMask = 0;
    std::vector<uint32> dynamicFieldsMask;
    dynamicFieldsMask.resize(m_dynamicTab.size());

    for (size_t i = 0; i < m_dynamicTab.size(); i++)
        dynamicFieldsMask[i] = 0;

    for (size_t i = 0; i < m_dynamicChange.size(); i++)
    {
        for (int index = 0; index < 32; index++)
        {
            if (m_dynamicChange[i][index])
            {
                dynamicTabMask |= 1 << i;
                dynamicFieldsMask[i] |= 1 << index;
            }
        }
    }

    *data << uint8(bool(dynamicTabMask));
    if (dynamicTabMask)
    {
        *data << uint32(dynamicTabMask);

        for (size_t i = 0; i < m_dynamicTab.size(); i++)
        {
            if (dynamicTabMask & (1 << i))
            {
                *data << uint8(1); // Always 1 or (1<<i)?
                *data << uint32(dynamicFieldsMask[i]);

                for (int index = 0; index < 32; index++)
                {
                    if (dynamicFieldsMask[i] & (1 << index))
                    {
                        *data << uint32(m_dynamicTab[i][index]);
                    }
                }
            }
        }
    }
}

void Object::ClearUpdateMask(bool remove)
{
    _changesMask.Clear();

    if (m_objectUpdated)
    {
        for (size_t i = 0; i < m_dynamicTab.size(); i++)
            memset(m_dynamicChange[i], 0, 32 * sizeof(bool));

        if (remove)
            sObjectAccessor->RemoveUpdateObject(this);
        m_objectUpdated = false;
    }
}

void Object::BuildFieldsUpdate(Player* player, UpdateDataMapType& data_map) const
{
    UpdateDataMapType::iterator iter = data_map.find(player);

    if (iter == data_map.end())
    {
        std::pair<UpdateDataMapType::iterator, bool> p = data_map.insert(UpdateDataMapType::value_type(player, UpdateData(player->GetMapId())));
        ASSERT(p.second);
        iter = p.first;
    }

    BuildValuesUpdateBlockForPlayer(&iter->second, iter->first);
}

uint32 Object::GetUpdateFieldData(Player const* target, uint32*& flags) const
{
    uint32 visibleFlag = UF_FLAG_PUBLIC | UF_FLAG_VIEWER_DEPENDENT;

    if (target == this)
        visibleFlag |= UF_FLAG_PRIVATE;

    switch (GetTypeId())
    {
        case TypeID::TYPEID_ITEM:
        case TypeID::TYPEID_CONTAINER:
            flags = ItemUpdateFieldFlags;
            if (((Item*)this)->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER | UF_FLAG_ITEM_OWNER;
            break;
        case TypeID::TYPEID_UNIT:
        case TypeID::TYPEID_PLAYER:
        {
            Player* plr = ToUnit()->GetCharmerOrOwnerPlayerOrPlayerItself();
            flags = UnitUpdateFieldFlags;
            if (ToUnit()->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;

            if (HasFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_SPECIALINFO))
                if (ToUnit()->HasAuraTypeWithCaster(SPELL_AURA_EMPATHY, target->GetGUID()))
                    visibleFlag |= UF_FLAG_SPECIAL_INFO;

            if (plr && plr->IsInSameRaidWith(target))
                visibleFlag |= UF_FLAG_PARTY_MEMBER;
            break;
        }
        case TypeID::TYPEID_GAMEOBJECT:
            flags = GameObjectUpdateFieldFlags;
            if (ToGameObject()->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TypeID::TYPEID_DYNAMICOBJECT:
            flags = DynamicObjectUpdateFieldFlags;
            if (((DynamicObject*)this)->GetCasterGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TypeID::TYPEID_CORPSE:
            flags = CorpseUpdateFieldFlags;
            if (ToCorpse()->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TypeID::TYPEID_AREATRIGGER:
            flags = AreaTriggerUpdateFieldFlags;
            break;
        case TypeID::TYPEID_OBJECT:
            break;
        default:
            break;
    }

    return visibleFlag;
}

void Object::_LoadIntoDataField(std::string const& data, uint32 startOffset, uint32 count)
{
    if (data.empty())
        return;

    Tokenizer tokens(data, ' ', count);

    if (tokens.size() != count)
        return;

    for (uint32 index = 0; index < count; ++index)
    {
        m_uint32Values[startOffset + index] = atol(tokens[index]);
        _changesMask.SetBit(startOffset + index);
    }
}

void Object::SetInt32Value(uint16 index, int32 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (m_int32Values[index] != value)
    {
        m_int32Values[index] = value;
        _changesMask.SetBit(index);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetUInt32Value(uint16 index, uint32 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (m_uint32Values[index] != value)
    {
        m_uint32Values[index] = value;
        _changesMask.SetBit(index);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetDynamicUInt32Value(uint32 tab, uint16 index, uint32 value)
{
    ASSERT(tab < m_dynamicTab.size() || index < 32);

    if (m_dynamicTab[tab][index] != value)
    {
        m_dynamicTab[tab][index] = value;
        m_dynamicChange[tab][index] = true;
        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::UpdateUInt32Value(uint16 index, uint32 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    m_uint32Values[index] = value;
    _changesMask.SetBit(index);
}

void Object::SetUInt64Value(uint16 index, uint64 value)
{
    ASSERT(index + 1 < m_valuesCount || PrintIndexError(index, true));
    if (*((uint64*)&(m_uint32Values[index])) != value)
    {
        m_uint32Values[index] = PAIR64_LOPART(value);
        m_uint32Values[index + 1] = PAIR64_HIPART(value);
        _changesMask.SetBit(index);
        _changesMask.SetBit(index + 1);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

bool Object::AddUInt64Value(uint16 index, uint64 value)
{
    ASSERT(index + 1 < m_valuesCount || PrintIndexError(index, true));
    if (value && !*((uint64*)&(m_uint32Values[index])))
    {
        m_uint32Values[index] = PAIR64_LOPART(value);
        m_uint32Values[index + 1] = PAIR64_HIPART(value);
        _changesMask.SetBit(index);
        _changesMask.SetBit(index + 1);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }

        return true;
    }

    return false;
}

bool Object::RemoveUInt64Value(uint16 index, uint64 value)
{
    ASSERT(index + 1 < m_valuesCount || PrintIndexError(index, true));
    if (value && *((uint64*)&(m_uint32Values[index])) == value)
    {
        m_uint32Values[index] = 0;
        m_uint32Values[index + 1] = 0;
        _changesMask.SetBit(index);
        _changesMask.SetBit(index + 1);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }

        return true;
    }

    return false;
}

void Object::SetFloatValue(uint16 index, float value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (m_floatValues[index] != value)
    {
        m_floatValues[index] = value;
        _changesMask.SetBit(index);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetByteValue(uint16 index, uint8 offset, uint8 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 4)
    {
        SF_LOG_ERROR("misc", "Object::SetByteValue: wrong offset %u", offset);
        return;
    }

    if (uint8(m_uint32Values[index] >> (offset * 8)) != value)
    {
        m_uint32Values[index] &= ~uint32(uint32(0xFF) << (offset * 8));
        m_uint32Values[index] |= uint32(uint32(value) << (offset * 8));
        _changesMask.SetBit(index);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetUInt16Value(uint16 index, uint8 offset, uint16 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 2)
    {
        SF_LOG_ERROR("misc", "Object::SetUInt16Value: wrong offset %u", offset);
        return;
    }

    if (uint16(m_uint32Values[index] >> (offset * 16)) != value)
    {
        m_uint32Values[index] &= ~uint32(uint32(0xFFFF) << (offset * 16));
        m_uint32Values[index] |= uint32(uint32(value) << (offset * 16));
        _changesMask.SetBit(index);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetStatFloatValue(uint16 index, float value)
{
    if (value < 0)
        value = 0.0f;

    SetFloatValue(index, value);
}

void Object::SetStatInt32Value(uint16 index, int32 value)
{
    if (value < 0)
        value = 0;

    SetUInt32Value(index, uint32(value));
}

void Object::ApplyModUInt32Value(uint16 index, int32 val, bool apply)
{
    int32 cur = GetUInt32Value(index);
    cur += (apply ? val : -val);
    if (cur < 0)
        cur = 0;
    SetUInt32Value(index, cur);
}

void Object::ApplyModInt32Value(uint16 index, int32 val, bool apply)
{
    int32 cur = GetInt32Value(index);
    cur += (apply ? val : -val);
    SetInt32Value(index, cur);
}

void Object::ApplyModSignedFloatValue(uint16 index, float  val, bool apply)
{
    float cur = GetFloatValue(index);
    cur += (apply ? val : -val);
    SetFloatValue(index, cur);
}

void Object::ApplyPercentModFloatValue(uint16 index, float val, bool apply)
{
    float value = GetFloatValue(index);
    ApplyPercentModFloatVar(value, val, apply);
    SetFloatValue(index, value);
}

void Object::ApplyModPositiveFloatValue(uint16 index, float  val, bool apply)
{
    float cur = GetFloatValue(index);
    cur += (apply ? val : -val);
    if (cur < 0)
        cur = 0;
    SetFloatValue(index, cur);
}

void Object::SetFlag(uint16 index, uint32 newFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));
    uint32 oldval = m_uint32Values[index];
    uint32 newval = oldval | newFlag;

    if (oldval != newval)
    {
        m_uint32Values[index] = newval;
        _changesMask.SetBit(index);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::RemoveFlag(uint16 index, uint32 oldFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));
    ASSERT(m_uint32Values);

    uint32 oldval = m_uint32Values[index];
    uint32 newval = oldval & ~oldFlag;

    if (oldval != newval)
    {
        m_uint32Values[index] = newval;
        _changesMask.SetBit(index);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::ToggleFlag(uint16 index, uint32 flag)
{
    if (HasFlag(index, flag))
        RemoveFlag(index, flag);
    else
        SetFlag(index, flag);
}

bool Object::HasFlag(uint16 index, uint32 flag) const
{
    if (index >= m_valuesCount && !PrintIndexError(index, false))
        return false;

    return (m_uint32Values[index] & flag) != 0;
}

void Object::ApplyModFlag(uint16 index, uint32 flag, bool apply)
{
    if (apply) SetFlag(index, flag); else RemoveFlag(index, flag);
}

void Object::SetByteFlag(uint16 index, uint8 offset, uint8 newFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 4)
    {
        SF_LOG_ERROR("misc", "Object::SetByteFlag: wrong offset %u", offset);
        return;
    }

    if (!(uint8(m_uint32Values[index] >> (offset * 8)) & newFlag))
    {
        m_uint32Values[index] |= uint32(uint32(newFlag) << (offset * 8));
        _changesMask.SetBit(index);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::RemoveByteFlag(uint16 index, uint8 offset, uint8 oldFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 4)
    {
        SF_LOG_ERROR("misc", "Object::RemoveByteFlag: wrong offset %u", offset);
        return;
    }

    if (uint8(m_uint32Values[index] >> (offset * 8)) & oldFlag)
    {
        m_uint32Values[index] &= ~uint32(uint32(oldFlag) << (offset * 8));
        _changesMask.SetBit(index);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::ToggleByteFlag(uint16 index, uint8 offset, uint8 flag)
{
    if (HasByteFlag(index, offset, flag))
        RemoveByteFlag(index, offset, flag);
    else
        SetByteFlag(index, offset, flag);
}

bool Object::HasByteFlag(uint16 index, uint8 offset, uint8 flag) const
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, false));
    ASSERT(offset < 4);
    return (((uint8*)&m_uint32Values[index])[offset] & flag) != 0;
}

void Object::SetFlag64(uint16 index, uint64 newFlag)
{
    uint64 oldval = GetUInt64Value(index);
    uint64 newval = oldval | newFlag;
    SetUInt64Value(index, newval);
}

void Object::RemoveFlag64(uint16 index, uint64 oldFlag)
{
    uint64 oldval = GetUInt64Value(index);
    uint64 newval = oldval & ~oldFlag;
    SetUInt64Value(index, newval);
}

void Object::ToggleFlag64(uint16 index, uint64 flag)
{
    if (HasFlag64(index, flag))
        RemoveFlag64(index, flag);
    else
        SetFlag64(index, flag);
}

bool Object::HasFlag64(uint16 index, uint64 flag) const
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, false));
    return (GetUInt64Value(index) & flag) != 0;
}

void Object::ApplyModFlag64(uint16 index, uint64 flag, bool apply)
{
    if (apply) SetFlag64(index, flag); else RemoveFlag64(index, flag);
}

bool Object::PrintIndexError(uint32 index, bool set) const
{
    SF_LOG_ERROR("misc", "Attempt %s non-existed value field: %u (count: %u) for object typeid: %u type mask: %u", (set ? "set value to" : "get value from"), index, m_valuesCount, uint8(GetTypeId()), m_objectType);

    // ASSERT must fail after function call
    return false;
}

bool Position::operator==(Position const &a)
{
    return (G3D::fuzzyEq(a.m_positionX, m_positionX) &&
            G3D::fuzzyEq(a.m_positionY, m_positionY) &&
            G3D::fuzzyEq(a.m_positionZ, m_positionZ) &&
            G3D::fuzzyEq(a.m_orientation, m_orientation));
}

bool Position::HasInLine(WorldObject const* target, float width) const
{
    if (!HasInArc(M_PI, target))
        return false;
    width += target->GetObjectSize();
    float angle = GetRelativeAngle(target);
    return fabs(sin(angle)) * GetExactDist2d(target->GetPositionX(), target->GetPositionY()) < width;
}

std::string Position::ToString() const
{
    std::stringstream sstr;
    sstr << "X: " << m_positionX << " Y: " << m_positionY << " Z: " << m_positionZ << " O: " << m_orientation;
    return sstr.str();
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer)
{
    float x, y, z, o;
    buf >> x >> y >> z >> o;
    streamer.m_pos->Relocate(x, y, z, o);
    return buf;
}
ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer)
{
    float x, y, z;
    streamer.m_pos->GetPosition(x, y, z);
    buf << x << y << z;
    return buf;
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer)
{
    float x, y, z;
    buf >> x >> y >> z;
    streamer.m_pos->Relocate(x, y, z);
    return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer)
{
    float x, y, z, o;
    streamer.m_pos->GetPosition(x, y, z, o);
    buf << x << y << z << o;
    return buf;
}

void MovementInfo::OutDebug()
{
    SF_LOG_INFO("misc", "MOVEMENT INFO");
    SF_LOG_INFO("misc", "guid " UI64FMTD, guid);
    SF_LOG_INFO("misc", "flags %s (%u)", Movement::MovementFlags_ToString(flags).c_str(), flags);
    SF_LOG_INFO("misc", "flags2 %s (%u)", Movement::MovementFlagsExtra_ToString(flags2).c_str(), flags2);
    SF_LOG_INFO("misc", "time %u current time %u", time, getMSTime());
    SF_LOG_INFO("misc", "position: `%s`", pos.ToString().c_str());
    if (transport.guid)
    {
        SF_LOG_INFO("misc", "TRANSPORT:");
        SF_LOG_INFO("misc", "guid: " UI64FMTD, transport.guid);
        SF_LOG_INFO("misc", "position: `%s`", transport.pos.ToString().c_str());
        SF_LOG_INFO("misc", "seat: %i", transport.seat);
        SF_LOG_INFO("misc", "time: %u", transport.time);
        if (flags2 & MOVEMENTFLAG2_INTERPOLATED_MOVEMENT)
            SF_LOG_INFO("misc", "time2: %u", transport.time2);
        if (transport.time3)
            SF_LOG_INFO("misc", "time3: %u", transport.time3);
    }

    if ((flags & (MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_FLYING)) || (flags2 & MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING))
        SF_LOG_INFO("misc", "pitch: %f", pitch);

    if (flags & MOVEMENTFLAG_FALLING || jump.fallTime)
    {
        SF_LOG_INFO("misc", "fallTime: %u j_zspeed: %f", jump.fallTime, jump.zspeed);
        if (flags & MOVEMENTFLAG_FALLING)
            SF_LOG_INFO("misc", "j_sinAngle: %f j_cosAngle: %f j_xyspeed: %f", jump.sinAngle, jump.cosAngle, jump.xyspeed);
    }

    if (flags & MOVEMENTFLAG_SPLINE_ELEVATION)
        SF_LOG_INFO("misc", "splineElevation: %f", splineElevation);
}

WorldObject::WorldObject(bool isWorldObject): WorldLocation(),
m_name(""), m_isActive(false), m_isWorldObject(isWorldObject), m_zoneScript(NULL),
m_transport(NULL), m_currMap(NULL), m_InstanceId(0),
m_phaseMask(PHASEMASK_NORMAL), m_notifyflags(0), m_executed_notifies(0)
{
    m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE | GHOST_VISIBILITY_GHOST);
    m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE);
}

void WorldObject::SetWorldObject(bool on)
{
    if (!IsInWorld())
        return;

    GetMap()->AddObjectToSwitchList(this, on);
}

bool WorldObject::IsWorldObject() const
{
    if (m_isWorldObject)
        return true;

    if (ToCreature() && ToCreature()->m_isTempWorldObject)
        return true;

    return false;
}

void WorldObject::setActive(bool on)
{
    if (m_isActive == on)
        return;

    if (GetTypeId() == TypeID::TYPEID_PLAYER)
        return;

    m_isActive = on;

    if (!IsInWorld())
        return;

    Map* map = FindMap();
    if (!map)
        return;

    if (on)
    {
        if (GetTypeId() == TypeID::TYPEID_UNIT)
            map->AddToActive(this->ToCreature());
        else if (GetTypeId() == TypeID::TYPEID_DYNAMICOBJECT)
            map->AddToActive((DynamicObject*)this);
    }
    else
    {
        if (GetTypeId() == TypeID::TYPEID_UNIT)
            map->RemoveFromActive(this->ToCreature());
        else if (GetTypeId() == TypeID::TYPEID_DYNAMICOBJECT)
            map->RemoveFromActive((DynamicObject*)this);
    }
}

void WorldObject::CleanupsBeforeDelete(bool /*finalCleanup*/)
{
    if (IsInWorld())
        RemoveFromWorld();
}

void WorldObject::_Create(uint32 guidlow, HighGuid guidhigh, uint32 phaseMask)
{
    Object::_Create(guidlow, 0, guidhigh);
    m_phaseMask = phaseMask;
}

void WorldObject::RemoveFromWorld()
{
    if (!IsInWorld())
        return;

    DestroyForNearbyPlayers();

    Object::RemoveFromWorld();
}

uint32 WorldObject::GetZoneId() const
{
    return GetBaseMap()->GetZoneId(m_positionX, m_positionY, m_positionZ);
}

uint32 WorldObject::GetAreaId() const
{
    return GetBaseMap()->GetAreaId(m_positionX, m_positionY, m_positionZ);
}

void WorldObject::GetZoneAndAreaId(uint32& zoneid, uint32& areaid) const
{
    GetBaseMap()->GetZoneAndAreaId(zoneid, areaid, m_positionX, m_positionY, m_positionZ);
}

InstanceScript* WorldObject::GetInstanceScript()
{
    Map* map = GetMap();
    return map->IsInstance() ? ((InstanceMap*)map)->GetInstanceScript() : NULL;
}

float WorldObject::GetDistanceZ(const WorldObject* obj) const
{
    float dz = fabs(GetPositionZ() - obj->GetPositionZ());
    float sizefactor = GetObjectSize() + obj->GetObjectSize();
    float dist = dz - sizefactor;
    return (dist > 0 ? dist : 0);
}

bool WorldObject::_IsWithinDist(WorldObject const* obj, float dist2compare, bool is3D) const
{
    float sizefactor = GetObjectSize() + obj->GetObjectSize();
    float maxdist = dist2compare + sizefactor;

    if (m_transport && obj->GetTransport() &&  obj->GetTransport()->GetGUIDLow() == m_transport->GetGUIDLow())
    {
        float dtx = m_movementInfo.transport.pos.m_positionX - obj->m_movementInfo.transport.pos.m_positionX;
        float dty = m_movementInfo.transport.pos.m_positionY - obj->m_movementInfo.transport.pos.m_positionY;
        float disttsq = dtx * dtx + dty * dty;
        if (is3D)
        {
            float dtz = m_movementInfo.transport.pos.m_positionZ - obj->m_movementInfo.transport.pos.m_positionZ;
            disttsq += dtz * dtz;
        }
        return disttsq < (maxdist * maxdist);
    }

    float dx = GetPositionX() - obj->GetPositionX();
    float dy = GetPositionY() - obj->GetPositionY();
    float distsq = dx*dx + dy*dy;
    if (is3D)
    {
        float dz = GetPositionZ() - obj->GetPositionZ();
        distsq += dz*dz;
    }

    return distsq < maxdist * maxdist;
}

bool WorldObject::IsWithinLOSInMap(const WorldObject* obj) const
{
    if (!IsInMap(obj))
        return false;

    float ox, oy, oz;
    obj->GetPosition(ox, oy, oz);
    return IsWithinLOS(ox, oy, oz);
}

float WorldObject::GetDistance(const WorldObject* obj) const
{
    float d = GetExactDist(obj) - GetObjectSize() - obj->GetObjectSize();
    return d > 0.0f ? d : 0.0f;
}

float WorldObject::GetDistance(const Position &pos) const
{
    float d = GetExactDist(&pos) - GetObjectSize();
    return d > 0.0f ? d : 0.0f;
}

float WorldObject::GetDistance(float x, float y, float z) const
{
    float d = GetExactDist(x, y, z) - GetObjectSize();
    return d > 0.0f ? d : 0.0f;
}

float WorldObject::GetDistance2d(const WorldObject* obj) const
{
    float d = GetExactDist2d(obj) - GetObjectSize() - obj->GetObjectSize();
    return d > 0.0f ? d : 0.0f;
}

float WorldObject::GetDistance2d(float x, float y) const
{
    float d = GetExactDist2d(x, y) - GetObjectSize();
    return d > 0.0f ? d : 0.0f;
}

bool WorldObject::IsSelfOrInSameMap(const WorldObject* obj) const
{
    if (this == obj)
        return true;
    return IsInMap(obj);
}

bool WorldObject::IsInMap(const WorldObject* obj) const
{
    if (obj)
        return IsInWorld() && obj->IsInWorld() && (GetMap() == obj->GetMap());
    return false;
}

bool WorldObject::IsWithinDist3d(float x, float y, float z, float dist) const
{
    return IsInDist(x, y, z, dist + GetObjectSize());
}

bool WorldObject::IsWithinDist3d(const Position* pos, float dist) const
{
    return IsInDist(pos, dist + GetObjectSize());
}

bool WorldObject::IsWithinDist2d(float x, float y, float dist) const
{
    return IsInDist2d(x, y, dist + GetObjectSize());
}

bool WorldObject::IsWithinDist2d(const Position* pos, float dist) const
{
    return IsInDist2d(pos, dist + GetObjectSize());
}

bool WorldObject::IsWithinDist(WorldObject const* obj, float dist2compare, bool is3D /*= true*/) const
{
    return obj && _IsWithinDist(obj, dist2compare, is3D);
}

bool WorldObject::IsWithinDistInMap(WorldObject const* obj, float dist2compare, bool is3D /*= true*/) const
{
    return obj && IsInMap(obj) && InSamePhase(obj) && _IsWithinDist(obj, dist2compare, is3D);
}

bool WorldObject::IsWithinLOS(float ox, float oy, float oz) const
{
    /*float x, y, z;
    GetPosition(x, y, z);
    VMAP::IVMapManager* vMapManager = VMAP::VMapFactory::createOrGetVMapManager();
    return vMapManager->isInLineOfSight(GetMapId(), x, y, z+2.0f, ox, oy, oz+2.0f);*/
    if (IsInWorld())
        return GetMap()->isInLineOfSight(GetPositionX(), GetPositionY(), GetPositionZ()+2.f, ox, oy, oz+2.f, GetPhaseMask());

    return true;
}

bool WorldObject::GetDistanceOrder(WorldObject const* obj1, WorldObject const* obj2, bool is3D /* = true */) const
{
    float dx1 = GetPositionX() - obj1->GetPositionX();
    float dy1 = GetPositionY() - obj1->GetPositionY();
    float distsq1 = dx1*dx1 + dy1*dy1;
    if (is3D)
    {
        float dz1 = GetPositionZ() - obj1->GetPositionZ();
        distsq1 += dz1*dz1;
    }

    float dx2 = GetPositionX() - obj2->GetPositionX();
    float dy2 = GetPositionY() - obj2->GetPositionY();
    float distsq2 = dx2*dx2 + dy2*dy2;
    if (is3D)
    {
        float dz2 = GetPositionZ() - obj2->GetPositionZ();
        distsq2 += dz2*dz2;
    }

    return distsq1 < distsq2;
}

bool WorldObject::IsInRange(WorldObject const* obj, float minRange, float maxRange, bool is3D /* = true */) const
{
    float dx = GetPositionX() - obj->GetPositionX();
    float dy = GetPositionY() - obj->GetPositionY();
    float distsq = dx*dx + dy*dy;
    if (is3D)
    {
        float dz = GetPositionZ() - obj->GetPositionZ();
        distsq += dz*dz;
    }

    float sizefactor = GetObjectSize() + obj->GetObjectSize();

    // check only for real range
    if (minRange > 0.0f)
    {
        float mindist = minRange + sizefactor;
        if (distsq < mindist * mindist)
            return false;
    }

    float maxdist = maxRange + sizefactor;
    return distsq < maxdist * maxdist;
}

bool WorldObject::IsInRange2d(float x, float y, float minRange, float maxRange) const
{
    float dx = GetPositionX() - x;
    float dy = GetPositionY() - y;
    float distsq = dx*dx + dy*dy;

    float sizefactor = GetObjectSize();

    // check only for real range
    if (minRange > 0.0f)
    {
        float mindist = minRange + sizefactor;
        if (distsq < mindist * mindist)
            return false;
    }

    float maxdist = maxRange + sizefactor;
    return distsq < maxdist * maxdist;
}

bool WorldObject::IsInRange3d(float x, float y, float z, float minRange, float maxRange) const
{
    float dx = GetPositionX() - x;
    float dy = GetPositionY() - y;
    float dz = GetPositionZ() - z;
    float distsq = dx*dx + dy*dy + dz*dz;

    float sizefactor = GetObjectSize();

    // check only for real range
    if (minRange > 0.0f)
    {
        float mindist = minRange + sizefactor;
        if (distsq < mindist * mindist)
            return false;
    }

    float maxdist = maxRange + sizefactor;
    return distsq < maxdist * maxdist;
}

void Position::RelocateOffset(const Position & offset)
{
    m_positionX = GetPositionX() + (offset.GetPositionX() * std::cos(GetOrientation()) + offset.GetPositionY() * std::sin(GetOrientation() + M_PI));
    m_positionY = GetPositionY() + (offset.GetPositionY() * std::cos(GetOrientation()) + offset.GetPositionX() * std::sin(GetOrientation()));
    m_positionZ = GetPositionZ() + offset.GetPositionZ();
    SetOrientation(GetOrientation() + offset.GetOrientation());
}

void Position::GetPositionOffsetTo(const Position & endPos, Position & retOffset) const
{
    float dx = endPos.GetPositionX() - GetPositionX();
    float dy = endPos.GetPositionY() - GetPositionY();

    retOffset.m_positionX = dx * std::cos(GetOrientation()) + dy * std::sin(GetOrientation());
    retOffset.m_positionY = dy * std::cos(GetOrientation()) - dx * std::sin(GetOrientation());
    retOffset.m_positionZ = endPos.GetPositionZ() - GetPositionZ();
    retOffset.SetOrientation(endPos.GetOrientation() - GetOrientation());
}

float Position::GetAngle(const Position* obj) const
{
    if (!obj)
        return 0;

    return GetAngle(obj->GetPositionX(), obj->GetPositionY());
}

// Return angle in range 0..2*pi
float Position::GetAngle(const float x, const float y) const
{
    float dx = x - GetPositionX();
    float dy = y - GetPositionY();

    float ang = atan2(dy, dx);
    ang = (ang >= 0) ? ang : 2 * M_PI + ang;
    return ang;
}

void Position::GetSinCos(const float x, const float y, float &vsin, float &vcos) const
{
    float dx = GetPositionX() - x;
    float dy = GetPositionY() - y;

    if (fabs(dx) < 0.001f && fabs(dy) < 0.001f)
    {
        float angle = (float)rand_norm()*static_cast<float>(2*M_PI);
        vcos = std::cos(angle);
        vsin = std::sin(angle);
    }
    else
    {
        float dist = sqrt((dx*dx) + (dy*dy));
        vcos = dx / dist;
        vsin = dy / dist;
    }
}

bool Position::HasInArc(float arc, const Position* obj, float border) const
{
    // always have self in arc
    if (obj == this)
        return true;

    // move arc to range 0.. 2*pi
    arc = NormalizeOrientation(arc);

    float angle = GetAngle(obj);
    angle -= m_orientation;

    // move angle to range -pi ... +pi
    angle = NormalizeOrientation(angle);
    if (angle > M_PI)
        angle -= 2.0f*M_PI;

    float lborder = -1 * (arc/border);                        // in range -pi..0
    float rborder = (arc/border);                             // in range 0..pi
    return ((angle >= lborder) && (angle <= rborder));
}

bool WorldObject::IsInBetween(const WorldObject* obj1, const WorldObject* obj2, float size) const
{
    if (!obj1 || !obj2)
        return false;

    float dist = GetExactDist2d(obj1->GetPositionX(), obj1->GetPositionY());

    // not using sqrt() for performance
    if ((dist * dist) >= obj1->GetExactDist2dSq(obj2->GetPositionX(), obj2->GetPositionY()))
        return false;

    if (!size)
        size = GetObjectSize() / 2;

    float angle = obj1->GetAngle(obj2);

    // not using sqrt() for performance
    return (size * size) >= GetExactDist2dSq(obj1->GetPositionX() + std::cos(angle) * dist, obj1->GetPositionY() + std::sin(angle) * dist);
}

bool WorldObject::isInFront(WorldObject const* target,  float arc) const
{
    return HasInArc(arc, target);
}

bool WorldObject::isInBack(WorldObject const* target, float arc) const
{
    return !HasInArc(2 * M_PI - arc, target);
}

void WorldObject::GetRandomPoint(const Position &pos, float distance, float &rand_x, float &rand_y, float &rand_z) const
{
    if (!distance)
    {
        pos.GetPosition(rand_x, rand_y, rand_z);
        return;
    }

    // angle to face `obj` to `this`
    float angle = (float)rand_norm()*static_cast<float>(2*M_PI);
    float new_dist = (float)rand_norm()*static_cast<float>(distance);

    rand_x = pos.m_positionX + new_dist * std::cos(angle);
    rand_y = pos.m_positionY + new_dist * std::sin(angle);
    rand_z = pos.m_positionZ;

    Skyfire::NormalizeMapCoord(rand_x);
    Skyfire::NormalizeMapCoord(rand_y);
    UpdateGroundPositionZ(rand_x, rand_y, rand_z);            // update to LOS height if available
}

void WorldObject::GetRandomPoint(const Position &srcPos, float distance, Position &pos) const
{
    float x, y, z;
    GetRandomPoint(srcPos, distance, x, y, z);
    pos.Relocate(x, y, z, GetOrientation());
}

void WorldObject::UpdateGroundPositionZ(float x, float y, float &z) const
{
    float new_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
    if (new_z > INVALID_HEIGHT)
        z = new_z+ 0.05f;                                   // just to be sure that we are not a few pixel under the surface
}

void WorldObject::UpdateAllowedPositionZ(float x, float y, float &z) const
{
    switch (GetTypeId())
    {
        case TypeID::TYPEID_UNIT:
        {
            // non fly unit don't must be in air
            // non swim unit must be at ground (mostly speedup, because it don't must be in water and water level check less fast
            if (!ToCreature()->CanFly())
            {
                bool canSwim = ToCreature()->CanSwim();
                float ground_z = z;
                float max_z = canSwim
                    ? GetBaseMap()->GetWaterOrGroundLevel(x, y, z, &ground_z, !ToUnit()->HasAuraType(SPELL_AURA_WATER_WALK))
                    : ((ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true)));
                if (max_z > INVALID_HEIGHT)
                {
                    if (z > max_z)
                        z = max_z;
                    else if (z < ground_z)
                        z = ground_z;
                }
            }
            else
            {
                float ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
                if (z < ground_z)
                    z = ground_z;
            }
            break;
        }
        case TypeID::TYPEID_PLAYER:
        {
            // for server controlled moves playr work same as creature (but it can always swim)
            if (!ToPlayer()->CanFly())
            {
                float ground_z = z;
                float max_z = GetBaseMap()->GetWaterOrGroundLevel(x, y, z, &ground_z, !ToUnit()->HasAuraType(SPELL_AURA_WATER_WALK));
                if (max_z > INVALID_HEIGHT)
                {
                    if (z > max_z)
                        z = max_z;
                    else if (z < ground_z)
                        z = ground_z;
                }
            }
            else
            {
                float ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
                if (z < ground_z)
                    z = ground_z;
            }
            break;
        }
        default:
        {
            float ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
            if (ground_z > INVALID_HEIGHT)
                z = ground_z;
            break;
        }
    }
}

bool Position::IsPositionValid() const
{
    return Skyfire::IsValidMapCoord(m_positionX, m_positionY, m_positionZ, m_orientation);
}

float WorldObject::GetGridActivationRange() const
{
    if (isActiveObject())
    {
        if (GetTypeId() == TypeID::TYPEID_PLAYER && ToPlayer()->IsInCinematic())
            return std::max(DEFAULT_VISIBILITY_INSTANCE, GetMap()->GetVisibilityRange());

        return GetMap()->GetVisibilityRange();
    }

    if (Creature const* thisCreature = ToCreature())
        return thisCreature->m_SightDistance;

    return 0.0f;
}

float WorldObject::GetVisibilityRange() const
{
    if (isActiveObject() && !ToPlayer())
        return MAX_VISIBILITY_DISTANCE;
    else
        return GetMap()->GetVisibilityRange();
}

float WorldObject::GetSightRange(const WorldObject* target) const
{
    if (ToUnit())
    {
        if (ToPlayer())
        {
            if (target && target->isActiveObject() && !target->ToPlayer())
                return MAX_VISIBILITY_DISTANCE;
            else if (ToPlayer()->IsInCinematic())
                return DEFAULT_VISIBILITY_INSTANCE;
            else
                return GetMap()->GetVisibilityRange();
        }
        else if (ToCreature())
            return ToCreature()->m_SightDistance;
        else
            return SIGHT_RANGE_UNIT;
    }

    if (ToDynObject() && isActiveObject())
    {
        return GetMap()->GetVisibilityRange();
    }

    return 0.0f;
}

bool WorldObject::CanSeeOrDetect(WorldObject const* obj, bool ignoreStealth, bool distanceCheck) const
{
    if (this == obj)
        return true;

    if (obj->IsNeverVisible() || CanNeverSee(obj))
        return false;

    if (obj->IsAlwaysVisibleFor(this) || CanAlwaysSee(obj))
        return true;

    bool corpseVisibility = false;
    if (distanceCheck)
    {
        bool corpseCheck = false;
        if (Player const* thisPlayer = ToPlayer())
        {
            if (thisPlayer->isDead() && thisPlayer->GetHealth() > 0 && // Cheap way to check for ghost state
                !(obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GHOST) & m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GHOST) & GHOST_VISIBILITY_GHOST))
            {
                if (Corpse* corpse = thisPlayer->GetCorpse())
                {
                    corpseCheck = true;
                    if (corpse->IsWithinDist(thisPlayer, GetSightRange(obj), false))
                        if (corpse->IsWithinDist(obj, GetSightRange(obj), false))
                            corpseVisibility = true;
                }
            }
        }

        WorldObject const* viewpoint = this;
        if (Player const* player = this->ToPlayer())
            viewpoint = player->GetViewpoint();

        if (!viewpoint)
            viewpoint = this;

        if (!corpseCheck && !viewpoint->IsWithinDist(obj, GetSightRange(obj), false))
            return false;
    }

    // GM visibility off or hidden NPC
    if (!obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GM))
    {
        // Stop checking other things for GMs
        if (m_serverSideVisibilityDetect.GetValue(SERVERSIDE_VISIBILITY_GM))
            return true;
    }
    else
        return m_serverSideVisibilityDetect.GetValue(SERVERSIDE_VISIBILITY_GM) >= obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GM);

    // Ghost players, Spirit Healers, and some other NPCs
    if (!corpseVisibility && !(obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GHOST) & m_serverSideVisibilityDetect.GetValue(SERVERSIDE_VISIBILITY_GHOST)))
    {
        // Alive players can see dead players in some cases, but other objects can't do that
        if (Player const* thisPlayer = ToPlayer())
        {
            if (Player const* objPlayer = obj->ToPlayer())
            {
                if (thisPlayer->GetTeam() != objPlayer->GetTeam() || !thisPlayer->IsGroupVisibleFor(objPlayer))
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }

    if (obj->IsInvisibleDueToDespawn())
        return false;

    if (!CanDetect(obj, ignoreStealth))
        return false;

    return true;
}

bool WorldObject::CanNeverSee(WorldObject const* obj) const
{
    return GetMap() != obj->GetMap() || !InSamePhase(obj);
}

bool WorldObject::CanDetect(WorldObject const* obj, bool ignoreStealth) const
{
    const WorldObject* seer = this;

    // Pets don't have detection, they use the detection of their masters
    if (Unit const* thisUnit = ToUnit())
        if (Unit* controller = thisUnit->GetCharmerOrOwner())
            seer = controller;

    if (obj->IsAlwaysDetectableFor(seer))
        return true;

    if (!ignoreStealth && !seer->CanDetectInvisibilityOf(obj))
        return false;

    if (!ignoreStealth && !seer->CanDetectStealthOf(obj))
        return false;

    return true;
}

bool WorldObject::CanDetectInvisibilityOf(WorldObject const* obj) const
{
    uint32 mask = obj->m_invisibility.GetFlags() & m_invisibilityDetect.GetFlags();

    // Check for not detected types
    if (mask != obj->m_invisibility.GetFlags())
        return false;

    for (uint32 i = 0; i < TOTAL_INVISIBILITY_TYPES; ++i)
    {
        if (!(mask & (1 << i)))
            continue;

        int32 objInvisibilityValue = obj->m_invisibility.GetValue(InvisibilityType(i));
        int32 ownInvisibilityDetectValue = m_invisibilityDetect.GetValue(InvisibilityType(i));

        // Too low value to detect
        if (ownInvisibilityDetectValue < objInvisibilityValue)
            return false;
    }

    return true;
}

bool WorldObject::CanDetectStealthOf(WorldObject const* obj) const
{
    // Combat reach is the minimal distance (both in front and behind),
    //   and it is also used in the range calculation.
    // One stealth point increases the visibility range by 0.3 yard.

    if (!obj->m_stealth.GetFlags())
        return true;

    float distance = GetExactDist(obj);
    float combatReach = 0.0f;

    Unit const* unit = ToUnit();
    if (unit)
        combatReach = unit->GetCombatReach();

    if (distance < combatReach)
        return true;

    if (!HasInArc(M_PI, obj))
        return false;

    GameObject const* go = ToGameObject();
    for (uint32 i = 0; i < TOTAL_STEALTH_TYPES; ++i)
    {
        if (!(obj->m_stealth.GetFlags() & (1 << i)))
            continue;

        if (unit && unit->HasAuraTypeWithMiscvalue(SPELL_AURA_DETECT_STEALTH, i))
            return true;

        // Starting points
        int32 detectionValue = 30;

        // Level difference: 5 point / level, starting from level 1.
        // There may be spells for this and the starting points too, but
        // not in the DBCs of the client.
        detectionValue += int32(getLevelForTarget(obj) - 1) * 5;

        // Apply modifiers
        detectionValue += m_stealthDetect.GetValue(StealthType(i));
        if (go)
            if (Unit* owner = go->GetOwner())
                detectionValue -= int32(owner->getLevelForTarget(this) - 1) * 5;

        detectionValue -= obj->m_stealth.GetValue(StealthType(i));

        // Calculate max distance
        float visibilityRange = float(detectionValue) * 0.3f + combatReach;

        if (visibilityRange > MAX_PLAYER_STEALTH_DETECT_RANGE)
            visibilityRange = MAX_PLAYER_STEALTH_DETECT_RANGE;

        if (distance > visibilityRange)
            return false;
    }

    return true;
}

void WorldObject::SendPlaySound(uint32 Sound, bool OnlySelf)
{
    ObjectGuid guid = GetGUID();

    WorldPacket data(SMSG_PLAY_SOUND, 4 + 9);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[1]);
    data << uint32(Sound);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[1]);
    if (OnlySelf && GetTypeId() == TypeID::TYPEID_PLAYER)
        this->ToPlayer()->GetSession()->SendPacket(&data);
    else
        SendMessageToSet(&data, true); // ToSelf ignored in this case
}

void Object::ForceValuesUpdateAtIndex(uint32 i)
{
    _changesMask.SetBit(i);
    if (m_inWorld && !m_objectUpdated)
    {
        sObjectAccessor->AddUpdateObject(this);
        m_objectUpdated = true;
    }
}

namespace Skyfire
{
    class MonsterChatBuilder
    {
        public:
            MonsterChatBuilder(WorldObject const* obj, ChatMsg msgtype, int32 textId, Language language, WorldObject const* target)
                : i_object(obj), i_msgtype(msgtype), i_textId(textId), i_language(language), i_target(target) { }

            void operator()(WorldPacket& data, LocaleConstant loc_idx)
            {
                char const* text = sObjectMgr->GetSkyFireString(i_textId, loc_idx);

                /// @todo i_object.GetName() also must be localized?
                ChatHandler::BuildChatPacket(data, i_msgtype, i_language, i_object, i_target, text, 0, "", loc_idx);
            }

        private:
            WorldObject const* i_object;
            ChatMsg i_msgtype;
            int32 i_textId;
            Language i_language;
            WorldObject const* i_target;
    };

    class MonsterCustomChatBuilder
    {
        public:
            MonsterCustomChatBuilder(WorldObject const* obj, ChatMsg msgtype, const char* text, Language language, WorldObject const* target)
                : i_object(obj), i_msgtype(msgtype), i_text(text), i_language(language), i_target(target) { }

            void operator()(WorldPacket& data, LocaleConstant loc_idx)
            {
                /// @todo i_object.GetName() also must be localized?
                ChatHandler::BuildChatPacket(data, i_msgtype, i_language, i_object, i_target, i_text, 0, "", loc_idx);
            }

        private:
            WorldObject const* i_object;
            ChatMsg i_msgtype;
            const char* i_text;
            Language i_language;
            WorldObject const* i_target;
    };
}                                                           // namespace Skyfire

void WorldObject::MonsterSay(const char* text, Language language, WorldObject const* target)
{
    CellCoord p = Skyfire::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Skyfire::MonsterCustomChatBuilder say_build(this, ChatMsg::CHAT_MSG_MONSTER_SAY, text, language, target);
    Skyfire::LocalizedPacketDo<Skyfire::MonsterCustomChatBuilder> say_do(say_build);
    Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::MonsterCustomChatBuilder> > say_worker(this, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_SAY), say_do);
    TypeContainerVisitor<Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::MonsterCustomChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_SAY));
}

void WorldObject::MonsterSay(int32 textId, Language language, WorldObject const* target)
{
    CellCoord p = Skyfire::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Skyfire::MonsterChatBuilder say_build(this, ChatMsg::CHAT_MSG_MONSTER_SAY, textId, language, target);
    Skyfire::LocalizedPacketDo<Skyfire::MonsterChatBuilder> say_do(say_build);
    Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::MonsterChatBuilder> > say_worker(this, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_SAY), say_do);
    TypeContainerVisitor<Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::MonsterChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_SAY));
}

void WorldObject::MonsterYell(const char* text, Language language, WorldObject const* target)
{
    CellCoord p = Skyfire::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Skyfire::MonsterCustomChatBuilder say_build(this, ChatMsg::CHAT_MSG_MONSTER_YELL, text, language, target);
    Skyfire::LocalizedPacketDo<Skyfire::MonsterCustomChatBuilder> say_do(say_build);
    Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::MonsterCustomChatBuilder> > say_worker(this, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_YELL), say_do);
    TypeContainerVisitor<Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::MonsterCustomChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_YELL));
}

void WorldObject::MonsterYell(int32 textId, Language language, WorldObject const* target)
{
    CellCoord p = Skyfire::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Skyfire::MonsterChatBuilder say_build(this, ChatMsg::CHAT_MSG_MONSTER_YELL, textId, language, target);
    Skyfire::LocalizedPacketDo<Skyfire::MonsterChatBuilder> say_do(say_build);
    Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::MonsterChatBuilder> > say_worker(this, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_YELL), say_do);
    TypeContainerVisitor<Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::MonsterChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_YELL));
}


void WorldObject::MonsterTextEmote(const char* text, WorldObject const* target, bool IsBossEmote)
{
    WorldPacket data;
    ChatHandler::BuildChatPacket(data, IsBossEmote ? ChatMsg::CHAT_MSG_RAID_BOSS_EMOTE : ChatMsg::CHAT_MSG_MONSTER_EMOTE, Language::LANG_UNIVERSAL, this, target, text);
    SendMessageToSetInRange(&data, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_TEXTEMOTE), true);
}

void WorldObject::MonsterTextEmote(int32 textId, WorldObject const* target, bool IsBossEmote)
{
    CellCoord p = Skyfire::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Skyfire::MonsterChatBuilder say_build(this, IsBossEmote ? ChatMsg::CHAT_MSG_RAID_BOSS_EMOTE : ChatMsg::CHAT_MSG_MONSTER_EMOTE, textId, Language::LANG_UNIVERSAL, target);
    Skyfire::LocalizedPacketDo<Skyfire::MonsterChatBuilder> say_do(say_build);
    Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::MonsterChatBuilder> > say_worker(this, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_TEXTEMOTE), say_do);
    TypeContainerVisitor<Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::MonsterChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_TEXTEMOTE));
}

void WorldObject::MonsterWhisper(const char* text, Player const* target, bool IsBossWhisper)
{
    if (!target)
        return;

    LocaleConstant loc_idx = target->GetSession()->GetSessionDbLocaleIndex();
    WorldPacket data;
    ChatHandler::BuildChatPacket(data, IsBossWhisper ? ChatMsg::CHAT_MSG_RAID_BOSS_WHISPER : ChatMsg::CHAT_MSG_MONSTER_WHISPER, Language::LANG_UNIVERSAL, this, target, text, 0, "", loc_idx);
    target->GetSession()->SendPacket(&data);
}

void WorldObject::MonsterWhisper(int32 textId, Player const* target, bool IsBossWhisper)
{
    if (!target)
        return;

    LocaleConstant loc_idx = target->GetSession()->GetSessionDbLocaleIndex();
    char const* text = sObjectMgr->GetSkyFireString(textId, loc_idx);

    WorldPacket data;
    ChatHandler::BuildChatPacket(data, IsBossWhisper ? ChatMsg::CHAT_MSG_RAID_BOSS_WHISPER : ChatMsg::CHAT_MSG_MONSTER_WHISPER, Language::LANG_UNIVERSAL, this, target, text, 0, "", loc_idx);
    target->GetSession()->SendPacket(&data);
}

void WorldObject::SendMessageToSet(WorldPacket* data, bool self)
{
    if (IsInWorld())
        SendMessageToSetInRange(data, GetVisibilityRange(), self);
}

void WorldObject::SendMessageToSetInRange(WorldPacket* data, float dist, bool /*self*/)
{
    Skyfire::MessageDistDeliverer notifier(this, data, dist);
    VisitNearbyWorldObject(dist, notifier);
}

void WorldObject::SendMessageToSet(WorldPacket* data, Player const* skipped_rcvr)
{
    Skyfire::MessageDistDeliverer notifier(this, data, GetVisibilityRange(), false, skipped_rcvr);
    VisitNearbyWorldObject(GetVisibilityRange(), notifier);
}

void WorldObject::SendObjectDeSpawnAnim(uint64 guid)
{
    ObjectGuid Guid = guid;
    WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);

    data.WriteBit(Guid[0]);
    data.WriteBit(Guid[2]);
    data.WriteBit(Guid[4]);
    data.WriteBit(Guid[1]);
    data.WriteBit(Guid[7]);
    data.WriteBit(Guid[3]);
    data.WriteBit(Guid[6]);
    data.WriteBit(Guid[5]);

    data.WriteByteSeq(Guid[0]);
    data.WriteByteSeq(Guid[2]);
    data.WriteByteSeq(Guid[4]);
    data.WriteByteSeq(Guid[5]);
    data.WriteByteSeq(Guid[7]);
    data.WriteByteSeq(Guid[3]);
    data.WriteByteSeq(Guid[1]);
    data.WriteByteSeq(Guid[6]);

    SendMessageToSet(&data, true);
}

void WorldObject::SetMap(Map* map)
{
    ASSERT(map);
    ASSERT(!IsInWorld() || GetTypeId() == TypeID::TYPEID_CORPSE);
    if (m_currMap == map) // command add npc: first create, than loadfromdb
        return;
    if (m_currMap)
    {
        SF_LOG_FATAL("misc", "WorldObject::SetMap: obj %u new map %u %u, old map %u %u", (uint32)GetTypeId(), map->GetId(), map->GetInstanceId(), m_currMap->GetId(), m_currMap->GetInstanceId());
        ASSERT(false);
    }
    m_currMap = map;
    m_mapId = map->GetId();
    m_InstanceId = map->GetInstanceId();
    if (IsWorldObject())
        m_currMap->AddWorldObject(this);
}

void WorldObject::ResetMap()
{
    ASSERT(m_currMap);
    ASSERT(!IsInWorld());
    if (IsWorldObject())
        m_currMap->RemoveWorldObject(this);
    m_currMap = NULL;
    //maybe not for corpse
    //m_mapId = 0;
    //m_InstanceId = 0;
}

Map const* WorldObject::GetBaseMap() const
{
    ASSERT(m_currMap);
    return m_currMap->GetParent();
}

void WorldObject::AddObjectToRemoveList()
{
    ASSERT(m_uint32Values);

    Map* map = FindMap();
    if (!map)
    {
        SF_LOG_ERROR("misc", "Object (TypeId: %u Entry: %u GUID: %u) at attempt add to move list not have valid map (Id: %u).", uint8(GetTypeId()), GetEntry(), GetGUIDLow(), GetMapId());
        return;
    }

    map->AddObjectToRemoveList(this);
}

TempSummon* Map::SummonCreature(uint32 entry, Position const& pos, SummonPropertiesEntry const* properties /*= NULL*/, uint32 duration /*= 0*/, Unit* summoner /*= NULL*/, uint32 spellId /*= 0*/, uint32 vehId /*= 0*/)
{
    uint32 mask = UNIT_MASK_SUMMON;
    if (properties)
    {
        switch (properties->Category)
        {
            case SUMMON_CATEGORY_PET:
                mask = UNIT_MASK_GUARDIAN;
                break;
            case SUMMON_CATEGORY_PUPPET:
                mask = UNIT_MASK_PUPPET;
                break;
            case SUMMON_CATEGORY_VEHICLE:
                mask = UNIT_MASK_MINION;
                break;
            case SUMMON_CATEGORY_WILD:
            case SUMMON_CATEGORY_ALLY:
            case SUMMON_CATEGORY_UNK:
            {
                switch (properties->Type)
                {
                case SUMMON_TYPE_MINION:
                case SUMMON_TYPE_GUARDIAN:
                case SUMMON_TYPE_GUARDIAN2:
                    mask = UNIT_MASK_GUARDIAN;
                    break;
                case SUMMON_TYPE_STATUE:
                case SUMMON_TYPE_TOTEM:
                case SUMMON_TYPE_LIGHTWELL:
                    mask = UNIT_MASK_TOTEM;
                    break;
                case SUMMON_TYPE_VEHICLE:
                case SUMMON_TYPE_VEHICLE2:
                    mask = UNIT_MASK_SUMMON;
                    break;
                case SUMMON_TYPE_MINIPET:
                    mask = UNIT_MASK_MINION;
                    break;
                default:
                    if (properties->Flags & 512) // Mirror Image, Summon Gargoyle
                        mask = UNIT_MASK_GUARDIAN;
                    break;
                }
                break;
            }
            default:
                return NULL;
        }
    }

    uint32 phase = PHASEMASK_NORMAL;
    std::set<uint32> phases;
    uint32 team = 0;
    if (summoner)
    {
        phase = summoner->GetPhaseMask();
        if (summoner->GetTypeId() == TypeID::TYPEID_PLAYER)
            team = summoner->ToPlayer()->GetTeam();

        phases = summoner->GetPhases();
    }

    TempSummon* summon = NULL;
    switch (mask)
    {
        case UNIT_MASK_SUMMON:
            summon = new TempSummon(properties, summoner, false);
            break;
        case UNIT_MASK_GUARDIAN:
            summon = new Guardian(properties, summoner, false);
            break;
        case UNIT_MASK_PUPPET:
            summon = new Puppet(properties, summoner);
            break;
        case UNIT_MASK_TOTEM:
            summon = new Totem(properties, summoner);
            break;
        case UNIT_MASK_MINION:
            summon = new Minion(properties, summoner, false);
            break;
    }

    if (!summon->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_UNIT), this, entry, vehId, team, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation()))
    {
        delete summon;
        return NULL;
    }

    for (auto phaseId : phases)
        summon->SetPhased(phaseId, false, true);
    summon->SetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL, spellId);

    summon->SetHomePosition(pos);

    summon->InitStats(duration);
    AddToMap(summon->ToCreature());
    summon->InitSummon();

    //ObjectAccessor::UpdateObjectVisibility(summon);

    return summon;
}

/**
* Summons group of creatures.
*
* @param group Id of group to summon.
* @param list  List to store pointers to summoned creatures.
*/

void Map::SummonCreatureGroup(uint8 group, std::list<TempSummon*>* list /*= NULL*/)
{
    std::vector<TempSummonData> const* data = sObjectMgr->GetSummonGroup(GetId(), SummonerType::SUMMONER_TYPE_MAP, group);
    if (!data)
        return;

    for (std::vector<TempSummonData>::const_iterator itr = data->begin(); itr != data->end(); ++itr)
        if (TempSummon* summon = SummonCreature(itr->entry, itr->pos, NULL, itr->time))
            if (list)
                list->push_back(summon);
}

void WorldObject::SetZoneScript()
{
    if (Map* map = FindMap())
    {
        if (map->IsInstance())
            m_zoneScript = (ZoneScript*)((InstanceMap*)map)->GetInstanceScript();
        else if (!map->IsBattlegroundOrArena())
        {
            if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(GetZoneId()))
                m_zoneScript = bf;
            else
                m_zoneScript = sOutdoorPvPMgr->GetZoneScript(GetZoneId());
        }
    }
}

TempSummon* WorldObject::SummonCreature(uint32 entry, const Position &pos, TempSummonType spwtype, uint32 duration, uint32 /*vehId*/) const
{
    if (Map* map = FindMap())
    {
        if (TempSummon* summon = map->SummonCreature(entry, pos, NULL, duration, isType(TYPEMASK_UNIT) ? (Unit*)this : NULL))
        {
            summon->SetTempSummonType(spwtype);
            return summon;
        }
    }

    return NULL;
}

TempSummon* WorldObject::SummonCreature(uint32 id, float x, float y, float z, float ang /*= 0*/, TempSummonType spwtype /*= TEMPSUMMON_MANUAL_DESPAWN*/, uint32 despwtime /*= 0*/) const
{
    if (!x && !y && !z)
    {
        GetClosePoint(x, y, z, GetObjectSize());
        ang = GetOrientation();
    }
    Position pos;
    pos.Relocate(x, y, z, ang);
    return SummonCreature(id, pos, spwtype, despwtime, 0);
}

GameObject* WorldObject::SummonGameObject(uint32 entry, float x, float y, float z, float ang, float rotation0, float rotation1, float rotation2, float rotation3, uint32 respawnTime)
{
    if (!IsInWorld())
        return NULL;

    GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(entry);
    if (!goinfo)
    {
        SF_LOG_ERROR("sql.sql", "Gameobject template %u not found in database!", entry);
        return NULL;
    }

    Map* map = GetMap();
    GameObject* go = new GameObject();
    if (!go->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT), entry, map, x, y, z, ang, rotation0, rotation1, rotation2, rotation3, 100, GOState::GO_STATE_READY))
    {
        delete go;
        return NULL;
    }

    for (auto phase : GetPhases())
        go->SetPhased(phase, false, true);

    go->SetRespawnTime(respawnTime);
    if (GetTypeId() == TypeID::TYPEID_PLAYER || GetTypeId() == TypeID::TYPEID_UNIT) //not sure how to handle this
        ToUnit()->AddGameObject(go);
    else
        go->SetSpawnedByDefault(false);

    map->AddToMap(go);
    return go;
}

Creature* WorldObject::SummonTrigger(float x, float y, float z, float ang, uint32 duration, CreatureAI* (*GetAI)(Creature*))
{
    TempSummonType summonType = (duration == 0) ? TempSummonType::TEMPSUMMON_DEAD_DESPAWN : TempSummonType::TEMPSUMMON_TIMED_DESPAWN;
    Creature* summon = SummonCreature(WORLD_TRIGGER, x, y, z, ang, summonType, duration);
    if (!summon)
        return NULL;

    //summon->SetName(GetName());
    if (GetTypeId() == TypeID::TYPEID_PLAYER || GetTypeId() == TypeID::TYPEID_UNIT)
    {
        summon->setFaction(((Unit*)this)->getFaction());
        summon->SetLevel(((Unit*)this)->getLevel());
    }

    if (GetAI)
        summon->AIM_Initialize(GetAI(summon));
    return summon;
}

/**
* Summons group of creatures. Should be called only by instances of Creature and GameObject classes.
*
* @param group Id of group to summon.
* @param list  List to store pointers to summoned creatures.
*/
void WorldObject::SummonCreatureGroup(uint8 group, std::list<TempSummon*>* list /*= NULL*/)
{
    ASSERT((GetTypeId() == TypeID::TYPEID_GAMEOBJECT || GetTypeId() == TypeID::TYPEID_UNIT) && "Only GOs and creatures can summon npc groups!");

    std::vector<TempSummonData> const* data = sObjectMgr->GetSummonGroup(GetEntry(), GetTypeId() == TypeID::TYPEID_GAMEOBJECT ? SummonerType::SUMMONER_TYPE_GAMEOBJECT : SummonerType::SUMMONER_TYPE_CREATURE, group);
    if (!data)
        return;

    for (std::vector<TempSummonData>::const_iterator itr = data->begin(); itr != data->end(); ++itr)
        if (TempSummon* summon = SummonCreature(itr->entry, itr->pos, itr->type, itr->time))
            if (list)
                list->push_back(summon);
}

Creature* WorldObject::FindNearestCreature(uint32 entry, float range, bool alive) const
{
    Creature* creature = NULL;
    Skyfire::NearestCreatureEntryWithLiveStateInObjectRangeCheck checker(*this, entry, alive, range);
    Skyfire::CreatureLastSearcher<Skyfire::NearestCreatureEntryWithLiveStateInObjectRangeCheck> searcher(this, creature, checker);
    VisitNearbyObject(range, searcher);
    return creature;
}

GameObject* WorldObject::FindNearestGameObject(uint32 entry, float range) const
{
    GameObject* go = NULL;
    Skyfire::NearestGameObjectEntryInObjectRangeCheck checker(*this, entry, range);
    Skyfire::GameObjectLastSearcher<Skyfire::NearestGameObjectEntryInObjectRangeCheck> searcher(this, go, checker);
    VisitNearbyGridObject(range, searcher);
    return go;
}

GameObject* WorldObject::FindNearestGameObjectOfType(GameobjectTypes type, float range) const
{
    GameObject* go = NULL;
    Skyfire::NearestGameObjectTypeInObjectRangeCheck checker(*this, type, range);
    Skyfire::GameObjectLastSearcher<Skyfire::NearestGameObjectTypeInObjectRangeCheck> searcher(this, go, checker);
    VisitNearbyGridObject(range, searcher);
    return go;
}

void WorldObject::GetGameObjectListWithEntryInGrid(std::list<GameObject*>& gameobjectList, uint32 entry, float maxSearchRange) const
{
    CellCoord pair(Skyfire::ComputeCellCoord(this->GetPositionX(), this->GetPositionY()));
    Cell cell(pair);
    cell.SetNoCreate();

    Skyfire::AllGameObjectsWithEntryInRange check(this, entry, maxSearchRange);
    Skyfire::GameObjectListSearcher<Skyfire::AllGameObjectsWithEntryInRange> searcher(this, gameobjectList, check);
    TypeContainerVisitor<Skyfire::GameObjectListSearcher<Skyfire::AllGameObjectsWithEntryInRange>, GridTypeMapContainer> visitor(searcher);

    cell.Visit(pair, visitor, *(this->GetMap()), *this, maxSearchRange);
}

void WorldObject::GetCreatureListWithEntryInGrid(std::list<Creature*>& creatureList, uint32 entry, float maxSearchRange) const
{
    CellCoord pair(Skyfire::ComputeCellCoord(this->GetPositionX(), this->GetPositionY()));
    Cell cell(pair);
    cell.SetNoCreate();

    Skyfire::AllCreaturesOfEntryInRange check(this, entry, maxSearchRange);
    Skyfire::CreatureListSearcher<Skyfire::AllCreaturesOfEntryInRange> searcher(this, creatureList, check);
    TypeContainerVisitor<Skyfire::CreatureListSearcher<Skyfire::AllCreaturesOfEntryInRange>, GridTypeMapContainer> visitor(searcher);

    cell.Visit(pair, visitor, *(this->GetMap()), *this, maxSearchRange);
}

void WorldObject::GetPlayerListInGrid(std::list<Player*>& playerList, float maxSearchRange) const
{
    Skyfire::AnyPlayerInObjectRangeCheck checker(this, maxSearchRange);
    Skyfire::PlayerListSearcher<Skyfire::AnyPlayerInObjectRangeCheck> searcher(this, playerList, checker);
    this->VisitNearbyWorldObject(maxSearchRange, searcher);
}

/*
namespace Skyfire
{
    class NearUsedPosDo
    {
        public:
            NearUsedPosDo(WorldObject const& obj, WorldObject const* searcher, float angle, ObjectPosSelector& selector)
                : i_object(obj), i_searcher(searcher), i_angle(angle), i_selector(selector) { }

            void operator()(Corpse*) const { }
            void operator()(DynamicObject*) const { }

            void operator()(Creature* c) const
            {
                // skip self or target
                if (c == i_searcher || c == &i_object)
                    return;

                float x, y, z;

                if (!c->IsAlive() || c->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DISTRACTED) ||
                    !c->GetMotionMaster()->GetDestination(x, y, z))
                {
                    x = c->GetPositionX();
                    y = c->GetPositionY();
                }

                add(c, x, y);
            }

            template<class T>
                void operator()(T* u) const
            {
                // skip self or target
                if (u == i_searcher || u == &i_object)
                    return;

                float x, y;

                x = u->GetPositionX();
                y = u->GetPositionY();

                add(u, x, y);
            }

            // we must add used pos that can fill places around center
            void add(WorldObject* u, float x, float y) const
            {
                // u is too nearest/far away to i_object
                if (!i_object.IsInRange2d(x, y, i_selector.m_dist - i_selector.m_size, i_selector.m_dist + i_selector.m_size))
                    return;

                float angle = i_object.GetAngle(u)-i_angle;

                // move angle to range -pi ... +pi
                while (angle > M_PI)
                    angle -= 2.0f * M_PI;
                while (angle < -M_PI)
                    angle += 2.0f * M_PI;

                // dist include size of u
                float dist2d = i_object.GetDistance2d(x, y);
                i_selector.AddUsedPos(u->GetObjectSize(), angle, dist2d + i_object.GetObjectSize());
            }
        private:
            WorldObject const& i_object;
            WorldObject const* i_searcher;
            float              i_angle;
            ObjectPosSelector& i_selector;
    };
}                                                           // namespace Skyfire
*/

//===================================================================================================

void WorldObject::GetNearPoint2D(float &x, float &y, float distance2d, float absAngle) const
{
    x = GetPositionX() + (GetObjectSize() + distance2d) * std::cos(absAngle);
    y = GetPositionY() + (GetObjectSize() + distance2d) * std::sin(absAngle);

    Skyfire::NormalizeMapCoord(x);
    Skyfire::NormalizeMapCoord(y);
}

void WorldObject::GetNearPoint(WorldObject const* /*searcher*/, float &x, float &y, float &z, float searcher_size, float distance2d, float absAngle) const
{
    GetNearPoint2D(x, y, distance2d+searcher_size, absAngle);
    z = GetPositionZ();
    UpdateAllowedPositionZ(x, y, z);

    /*
    // if detection disabled, return first point
    if (!sWorld->getIntConfig(CONFIG_DETECT_POS_COLLISION))
    {
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available
        return;
    }

    // or remember first point
    float first_x = x;
    float first_y = y;
    bool first_los_conflict = false;                        // first point LOS problems

    // prepare selector for work
    ObjectPosSelector selector(GetPositionX(), GetPositionY(), GetObjectSize(), distance2d+searcher_size);

    // adding used positions around object
    {
        CellCoord p(Skyfire::ComputeCellCoord(GetPositionX(), GetPositionY()));
        Cell cell(p);
        cell.SetNoCreate();

        Skyfire::NearUsedPosDo u_do(*this, searcher, absAngle, selector);
        Skyfire::WorldObjectWorker<Skyfire::NearUsedPosDo> worker(this, u_do);

        TypeContainerVisitor<Skyfire::WorldObjectWorker<Skyfire::NearUsedPosDo>, GridTypeMapContainer  > grid_obj_worker(worker);
        TypeContainerVisitor<Skyfire::WorldObjectWorker<Skyfire::NearUsedPosDo>, WorldTypeMapContainer > world_obj_worker(worker);

        CellLock<GridReadGuard> cell_lock(cell, p);
        cell_lock->Visit(cell_lock, grid_obj_worker,  *GetMap(), *this, distance2d);
        cell_lock->Visit(cell_lock, world_obj_worker, *GetMap(), *this, distance2d);
    }

    // maybe can just place in primary position
    if (selector.CheckOriginal())
    {
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;

        first_los_conflict = true;                          // first point have LOS problems
    }

    float angle;                                            // candidate of angle for free pos

    // special case when one from list empty and then empty side preferred
    if (selector.FirstAngle(angle))
    {
        GetNearPoint2D(x, y, distance2d, absAngle+angle);
        z = GetPositionZ();
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;
    }

    // set first used pos in lists
    selector.InitializeAngle();

    // select in positions after current nodes (selection one by one)
    while (selector.NextAngle(angle))                        // angle for free pos
    {
        GetNearPoint2D(x, y, distance2d, absAngle+angle);
        z = GetPositionZ();
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;
    }

    // BAD NEWS: not free pos (or used or have LOS problems)
    // Attempt find _used_ pos without LOS problem

    if (!first_los_conflict)
    {
        x = first_x;
        y = first_y;

        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available
        return;
    }

    // special case when one from list empty and then empty side preferred
    if (selector.IsNonBalanced())
    {
        if (!selector.FirstAngle(angle))                     // _used_ pos
        {
            GetNearPoint2D(x, y, distance2d, absAngle+angle);
            z = GetPositionZ();
            UpdateGroundPositionZ(x, y, z);                   // update to LOS height if available

            if (IsWithinLOS(x, y, z))
                return;
        }
    }

    // set first used pos in lists
    selector.InitializeAngle();

    // select in positions after current nodes (selection one by one)
    while (selector.NextUsedAngle(angle))                    // angle for used pos but maybe without LOS problem
    {
        GetNearPoint2D(x, y, distance2d, absAngle+angle);
        z = GetPositionZ();
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;
    }

    // BAD BAD NEWS: all found pos (free and used) have LOS problem :(
    x = first_x;
    y = first_y;

    UpdateGroundPositionZ(x, y, z);                           // update to LOS height if available
    */
}

void WorldObject::GetClosePoint(float &x, float &y, float &z, float size, float distance2d /*= 0*/, float angle /*= 0*/) const
{
    // angle calculated from current orientation
    GetNearPoint(NULL, x, y, z, size, distance2d, GetOrientation() + angle);
}

void WorldObject::GetNearPosition(Position &pos, float dist, float angle)
{
    GetPosition(&pos);
    MovePosition(pos, dist, angle);
}

void WorldObject::GetFirstCollisionPosition(Position &pos, float dist, float angle)
{
    GetPosition(&pos);
    MovePositionToFirstCollision(pos, dist, angle);
}

void WorldObject::GetRandomNearPosition(Position &pos, float radius)
{
    GetPosition(&pos);
    MovePosition(pos, radius * (float)rand_norm(), (float)rand_norm() * static_cast<float>(2 * M_PI));
}

void WorldObject::GetContactPoint(const WorldObject* obj, float &x, float &y, float &z, float distance2d /*= CONTACT_DISTANCE*/) const
{
    // angle to face `obj` to `this` using distance includes size of `obj`
    GetNearPoint(obj, x, y, z, obj->GetObjectSize(), distance2d, GetAngle(obj));
}

float WorldObject::GetObjectSize() const
{
    return (m_valuesCount > UNIT_FIELD_COMBAT_REACH) ? m_floatValues[UNIT_FIELD_COMBAT_REACH] : DEFAULT_WORLD_OBJECT_SIZE;
}

void WorldObject::MovePosition(Position &pos, float dist, float angle)
{
    angle += GetOrientation();
    float destx, desty, destz, ground, floor;
    destx = pos.m_positionX + dist * std::cos(angle);
    desty = pos.m_positionY + dist * std::sin(angle);

    // Prevent invalid coordinates here, position is unchanged
    if (!Skyfire::IsValidMapCoord(destx, desty, pos.m_positionZ))
    {
        SF_LOG_FATAL("misc", "WorldObject::MovePosition: Object (TypeId: %u Entry: %u GUID: %u) has invalid coordinates X: %f and Y: %f were passed!",
            uint8(GetTypeId()), GetEntry(), GetGUIDLow(), destx, desty);
        return;
    }

    ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
    floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
    destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

    float step = dist/10.0f;

    for (uint8 j = 0; j < 10; ++j)
    {
        // do not allow too big z changes
        if (fabs(pos.m_positionZ - destz) > 6)
        {
            destx -= step * std::cos(angle);
            desty -= step * std::sin(angle);
            ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
            floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;
        }
        // we have correct destz now
        else
        {
            pos.Relocate(destx, desty, destz);
            break;
        }
    }

    Skyfire::NormalizeMapCoord(pos.m_positionX);
    Skyfire::NormalizeMapCoord(pos.m_positionY);
    UpdateGroundPositionZ(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
    pos.SetOrientation(GetOrientation());
}

void WorldObject::MovePositionToFirstCollision(Position &pos, float dist, float angle)
{
    angle += GetOrientation();
    float destx, desty, destz, ground, floor;
    pos.m_positionZ += 2.0f;
    destx = pos.m_positionX + dist * std::cos(angle);
    desty = pos.m_positionY + dist * std::sin(angle);

    // Prevent invalid coordinates here, position is unchanged
    if (!Skyfire::IsValidMapCoord(destx, desty))
    {
        SF_LOG_FATAL("misc", "WorldObject::MovePositionToFirstCollision invalid coordinates X: %f and Y: %f were passed!", destx, desty);
        return;
    }

    ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
    floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
    destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

    bool col = VMAP::VMapFactory::createOrGetVMapManager()->getObjectHitPos(GetMapId(), pos.m_positionX, pos.m_positionY, pos.m_positionZ+0.5f, destx, desty, destz+0.5f, destx, desty, destz, -0.5f);

    // collision occured
    if (col)
    {
        // move back a bit
        destx -= CONTACT_DISTANCE * std::cos(angle);
        desty -= CONTACT_DISTANCE * std::sin(angle);
        dist = sqrt((pos.m_positionX - destx)*(pos.m_positionX - destx) + (pos.m_positionY - desty)*(pos.m_positionY - desty));
    }

    // check dynamic collision
    col = GetMap()->getObjectHitPos(GetPhaseMask(), pos.m_positionX, pos.m_positionY, pos.m_positionZ+0.5f, destx, desty, destz+0.5f, destx, desty, destz, -0.5f);

    // Collided with a gameobject
    if (col)
    {
        destx -= CONTACT_DISTANCE * std::cos(angle);
        desty -= CONTACT_DISTANCE * std::sin(angle);
        dist = sqrt((pos.m_positionX - destx)*(pos.m_positionX - destx) + (pos.m_positionY - desty)*(pos.m_positionY - desty));
    }

    float step = dist/10.0f;

    for (uint8 j = 0; j < 10; ++j)
    {
        // do not allow too big z changes
        if (fabs(pos.m_positionZ - destz) > 6)
        {
            destx -= step * std::cos(angle);
            desty -= step * std::sin(angle);
            ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
            floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;
        }
        // we have correct destz now
        else
        {
            pos.Relocate(destx, desty, destz);
            break;
        }
    }

    Skyfire::NormalizeMapCoord(pos.m_positionX);
    Skyfire::NormalizeMapCoord(pos.m_positionY);
    UpdateAllowedPositionZ(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
    pos.SetOrientation(GetOrientation());
}

void WorldObject::SetPhaseMask(uint32 newPhaseMask, bool update)
{
    m_phaseMask = newPhaseMask;

    if (update && IsInWorld())
        UpdateObjectVisibility();
}

void WorldObject::RebuildTerrainSwaps()
{
    // Clear all terrain swaps, will be rebuilt below
    // Reason for this is, multiple phases can have the same terrain swap, we should not remove the swap if another phase still use it
    _terrainSwaps.clear();

    // Check all applied phases for terrain swap and add it only once
    for (uint32 phaseId : _phases)
    {
        if (std::vector<PhaseInfoStruct> const* swaps = sObjectMgr->GetPhaseTerrainSwaps(phaseId))
        {
            for (PhaseInfoStruct const& swap : *swaps)
            {
                // only add terrain swaps for current map
                MapEntry const* mapEntry = sMapStore.LookupEntry(swap.id);
                if (!mapEntry || mapEntry->rootPhaseMap != int32(GetMapId()))
                    continue;

                if (sConditionMgr->IsObjectMeetToConditions(this, swap.Conditions))
                    _terrainSwaps.insert(swap.id);
            }
        }
    }

    // get default terrain swaps, only for current map always
    if (std::vector<PhaseInfoStruct> const* mapSwaps = sObjectMgr->GetDefaultTerrainSwaps(GetMapId()))
        for (PhaseInfoStruct const& swap : *mapSwaps)
            if (sConditionMgr->IsObjectMeetToConditions(this, swap.Conditions))
                _terrainSwaps.insert(swap.id);

    // online players have a game client with world map display
    if (GetTypeId() == TypeID::TYPEID_PLAYER)
        RebuildWorldMapAreaSwaps();
}

void WorldObject::RebuildWorldMapAreaSwaps()
{
    // Clear all world map area swaps, will be rebuilt below
    _worldMapSwaps.clear();

    // get ALL default terrain swaps, if we are using it (condition is true) 
    // send the worldmaparea for it, to see swapped worldmaparea in client from other maps too, not just from our current
    TerrainPhaseInfo const& defaults = sObjectMgr->GetDefaultTerrainSwapStore();
    for (TerrainPhaseInfo::const_iterator itr = defaults.begin(); itr != defaults.end(); ++itr)
        for (PhaseInfoStruct const& swap : itr->second)
            if (std::vector<uint32> const* uiMapSwaps = sObjectMgr->GetTerrainWorldMaps(swap.id))
                if (sConditionMgr->IsObjectMeetToConditions(this, swap.Conditions))
                    for (uint32 worldMapAreaId : *uiMapSwaps)
                        _worldMapSwaps.insert(worldMapAreaId);

    // Check all applied phases for world map area swaps
    for (uint32 phaseId : _phases)
        if (std::vector<PhaseInfoStruct> const* swaps = sObjectMgr->GetPhaseTerrainSwaps(phaseId))
            for (PhaseInfoStruct const& swap : *swaps)
                if (std::vector<uint32> const* uiMapSwaps = sObjectMgr->GetTerrainWorldMaps(swap.id))
                    if (sConditionMgr->IsObjectMeetToConditions(this, swap.Conditions))
                        for (uint32 worldMapAreaId : *uiMapSwaps)
                            _worldMapSwaps.insert(worldMapAreaId);
}

void WorldObject::UpdateAreaPhase()
{
    bool updateNeeded = false;
    PhaseInfo const& phases = sObjectMgr->GetAreaPhases();
    for (PhaseInfo::const_iterator itr = phases.begin(); itr != phases.end(); ++itr)
    {
        uint32 areaId = itr->first;
        for (PhaseInfoStruct const& phase : itr->second)
        {
            if (areaId == GetAreaId())
            {
                if (sConditionMgr->IsObjectMeetToConditions(this, phase.Conditions))
                {
                    // add new phase if condition passed, true if it wasnt added before
                    bool up = SetPhased(phase.id, false, true);
                    if (!updateNeeded && up)
                        updateNeeded = true;
                }
                else
                {
                    // condition failed, remove phase, true if there was something removed
                    bool up = SetPhased(phase.id, false, false);
                    if (!updateNeeded && up)
                        updateNeeded = true;
                }
            }
            else
            {
                // not in area, remove phase, true if there was something removed
                bool up = SetPhased(phase.id, false, false);
                if (!updateNeeded && up)
                    updateNeeded = true;
            }
        }
    }

    // do not remove a phase if it would be removed by an area but we have the same phase from an aura
    if (Unit* unit = ToUnit())
    {
        Unit::AuraEffectList const& auraPhaseList = unit->GetAuraEffectsByType(SPELL_AURA_PHASE);
        for (Unit::AuraEffectList::const_iterator itr = auraPhaseList.begin(); itr != auraPhaseList.end(); ++itr)
        {
            uint32 phase = uint32((*itr)->GetMiscValueB());
            bool up = SetPhased(phase, false, true);
            if (!updateNeeded && up)
                updateNeeded = true;
        }
        Unit::AuraEffectList const& auraPhaseGroupList = unit->GetAuraEffectsByType(SPELL_AURA_PHASE_GROUP);
        for (Unit::AuraEffectList::const_iterator itr = auraPhaseGroupList.begin(); itr != auraPhaseGroupList.end(); ++itr)
        {
            bool up = false;
            uint32 phaseGroup = uint32((*itr)->GetMiscValueB());
            std::set<uint32> const& phases = GetPhasesForGroup(phaseGroup);
            for (uint32 phase : phases)
                up = SetPhased(phase, false, true);
            if (!updateNeeded && up)
                updateNeeded = true;
        }
    }

    // only update visibility and send packets if there was a change in the phase list

    if (updateNeeded && GetTypeId() == TypeID::TYPEID_PLAYER && IsInWorld())
        ToPlayer()->GetSession()->SendSetPhaseShift(GetPhases(), GetTerrainSwaps(), GetWorldMapSwaps());

    // only update visibilty once, to prevent objects appearing for a moment while adding in multiple phases
    if (updateNeeded && IsInWorld())
        UpdateObjectVisibility();
}
bool WorldObject::HasPhaseList(uint32 phase)
{
    return _phases.find(phase) != _phases.end();
}
bool WorldObject::SetPhased(uint32 id, bool update, bool apply)
{
    if (id)
    {
        if (apply)
        {
            if (HasPhaseList(id)) // do not run the updates if we are already in this phase
                return false;
            _phases.insert(id);
        }
        else
        {
            if (std::vector<PhaseInfoStruct> const* phases = sObjectMgr->GetPhasesForArea(GetAreaId()))
                for (PhaseInfoStruct const& phase : *phases)
                    if (id == phase.id)
                        if (sConditionMgr->IsObjectMeetToConditions(this, phase.Conditions))
                            return false;

            if (!HasPhaseList(id)) // do not run the updates if we are not in this phase
                return false;
            _phases.erase(id);
        }
    }
    RebuildTerrainSwaps();

    if (update && IsInWorld())
        UpdateObjectVisibility();
    return true;
}

void WorldObject::ClearPhases(bool update)
{
    _phases.clear();

    RebuildTerrainSwaps();

    if (update && IsInWorld())
        UpdateObjectVisibility();
}

bool WorldObject::IsPhased(WorldObject const* obj) const
{
    // PhaseId 169 is the default fallback phase
    if (_phases.empty() && obj->GetPhases().empty())
        return true;

    if (_phases.empty() && obj->IsPhased(169))
        return true;

    if (obj->GetPhases().empty() && IsPhased(169))
        return true;

    if (GetTypeId() == TypeID::TYPEID_PLAYER && ToPlayer()->IsGameMaster())
        return true;

    return Skyfire::Containers::Intersects(_phases.begin(), _phases.end(), obj->GetPhases().begin(), obj->GetPhases().end());
}

//Temp Speshul Tactics 
bool WorldObject::InSamePhase(WorldObject const* obj) const
{
    return IsPhased(obj);
}

void WorldObject::PlayDistanceSound(uint32 sound_id, Player* target)
{
    ObjectGuid TargetGUID = target->GetGUID();
    ObjectGuid SourceGUID = GetGUID();

    WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 8 + 8 + 4);
    data.WriteGuidMask(SourceGUID, 5);
    data.WriteGuidMask(TargetGUID, 7, 0, 3);
    data.WriteGuidMask(SourceGUID, 1);
    data.WriteGuidMask(TargetGUID, 4);
    data.WriteGuidMask(SourceGUID, 7, 2, 4, 3);
    data.WriteGuidMask(TargetGUID, 5, 1, 6, 2);
    data.WriteGuidMask(SourceGUID, 6, 0);

    data.WriteGuidBytes(TargetGUID, 6, 2);
    data.WriteGuidBytes(SourceGUID, 2, 5);
    data.WriteGuidBytes(TargetGUID, 7, 5, 3, 1);
    data.WriteGuidBytes(SourceGUID, 3, 1);
    data << uint32(sound_id);
    data.WriteGuidBytes(TargetGUID, 4);
    data.WriteGuidBytes(SourceGUID, 4, 7, 0, 6);
    data.WriteGuidBytes(TargetGUID, 0);
    target->SendDirectMessage(&data);
}

void WorldObject::PlayDirectSound(uint32 sound_id, Player* target /*= NULL*/)
{
    WorldPacket data(SMSG_PLAY_SOUND, 4);
    data << uint32(sound_id);
    data << uint64(GetGUID());
    if (target)
        target->SendDirectMessage(&data);
    else
        SendMessageToSet(&data, true);
}

void WorldObject::DestroyForNearbyPlayers()
{
    if (!IsInWorld())
        return;

    std::list<Player*> targets;
    Skyfire::AnyPlayerInObjectRangeCheck check(this, GetVisibilityRange(), false);
    Skyfire::PlayerListSearcher<Skyfire::AnyPlayerInObjectRangeCheck> searcher(this, targets, check);
    VisitNearbyWorldObject(GetVisibilityRange(), searcher);
    for (std::list<Player*>::const_iterator iter = targets.begin(); iter != targets.end(); ++iter)
    {
        Player* player = (*iter);

        if (player == this)
            continue;

        if (!player->HaveAtClient(this))
            continue;

        if (isType(TYPEMASK_UNIT) && ((Unit*)this)->GetCharmerGUID() == player->GetGUID()) /// @todo this is for puppet
            continue;

        DestroyForPlayer(player);
        player->m_clientGUIDs.erase(GetGUID());
    }
}

void WorldObject::UpdateObjectVisibility(bool /*forced*/)
{
    //updates object's visibility for nearby players
    Skyfire::VisibleChangesNotifier notifier(*this);
    VisitNearbyWorldObject(GetVisibilityRange(), notifier);
}

struct WorldObjectChangeAccumulator
{
    UpdateDataMapType& i_updateDatas;
    WorldObject& i_object;
    std::set<uint64> plr_list;
    WorldObjectChangeAccumulator(WorldObject &obj, UpdateDataMapType &d) : i_updateDatas(d), i_object(obj) { }
    void Visit(PlayerMapType &m)
    {
        Player* source = NULL;
        for (PlayerMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        {
            source = iter->GetSource();

            BuildPacket(source);

            if (!source->GetSharedVisionList().empty())
            {
                SharedVisionList::const_iterator it = source->GetSharedVisionList().begin();
                for (; it != source->GetSharedVisionList().end(); ++it)
                    BuildPacket(*it);
            }
        }
    }

    void Visit(CreatureMapType &m)
    {
        Creature* source = NULL;
        for (CreatureMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        {
            source = iter->GetSource();
            if (!source->GetSharedVisionList().empty())
            {
                SharedVisionList::const_iterator it = source->GetSharedVisionList().begin();
                for (; it != source->GetSharedVisionList().end(); ++it)
                    BuildPacket(*it);
            }
        }
    }

    void Visit(DynamicObjectMapType &m)
    {
        DynamicObject* source = NULL;
        for (DynamicObjectMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        {
            source = iter->GetSource();
            uint64 guid = source->GetCasterGUID();

            if (IS_PLAYER_GUID(guid))
            {
                //Caster may be NULL if DynObj is in removelist
                if (Player* caster = ObjectAccessor::FindPlayer(guid))
                    if (caster->GetUInt64Value(PLAYER_FIELD_FARSIGHT_OBJECT) == source->GetGUID())
                        BuildPacket(caster);
            }
        }
    }

    void BuildPacket(Player* player)
    {
        // Only send update once to a player
        if (plr_list.find(player->GetGUID()) == plr_list.end() && player->HaveAtClient(&i_object))
        {
            i_object.BuildFieldsUpdate(player, i_updateDatas);
            plr_list.insert(player->GetGUID());
        }
    }

    template<class SKIP> void Visit(GridRefManager<SKIP> &) { }
};

void WorldObject::BuildUpdate(UpdateDataMapType& data_map)
{
    CellCoord p = Skyfire::ComputeCellCoord(GetPositionX(), GetPositionY());
    Cell cell(p);
    cell.SetNoCreate();
    WorldObjectChangeAccumulator notifier(*this, data_map);
    TypeContainerVisitor<WorldObjectChangeAccumulator, WorldTypeMapContainer > player_notifier(notifier);
    Map& map = *GetMap();
    //we must build packets for all visible players
    cell.Visit(p, player_notifier, map, *this, GetVisibilityRange());

    ClearUpdateMask(false);
}

uint64 WorldObject::GetTransGUID() const
{
    if (GetTransport())
        return GetTransport()->GetGUID();
    return 0;
}
