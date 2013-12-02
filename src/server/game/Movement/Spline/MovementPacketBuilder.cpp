/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "MovementPacketBuilder.h"
#include "MoveSpline.h"
#include "WorldPacket.h"
#include "Object.h"

namespace Movement
{
    inline void operator << (ByteBuffer& b, const Vector3& v)
    {
        b << v.x << v.y << v.z;
    }

    inline void operator >> (ByteBuffer& b, Vector3& v)
    {
        b >> v.x >> v.y >> v.z;
    }

    enum MonsterMoveType
    {
        MonsterMoveNormal       = 0,
        MonsterMoveStop         = 1,
        MonsterMoveFacingSpot   = 2,
        MonsterMoveFacingTarget = 3,
        MonsterMoveFacingAngle  = 4
    };

    void PacketBuilder::WriteCommonMonsterMovePart(const MoveSpline& move_spline, WorldPacket& data)
    {
        MoveSplineFlag splineflags = move_spline.splineflags;

        data << uint8(0);                                       // sets/unsets MOVEMENTFLAG2_UNK7 (0x40)
        data << move_spline.spline.getPoint(move_spline.spline.first());
        data << move_spline.GetId();

        switch (splineflags & MoveSplineFlag::Mask_Final_Facing)
        {
            case MoveSplineFlag::Final_Target:
                data << uint8(MonsterMoveFacingTarget);
                data << move_spline.facing.target;
                break;
            case MoveSplineFlag::Final_Angle:
                data << uint8(MonsterMoveFacingAngle);
                data << move_spline.facing.angle;
                break;
            case MoveSplineFlag::Final_Point:
                data << uint8(MonsterMoveFacingSpot);
                data << move_spline.facing.f.x << move_spline.facing.f.y << move_spline.facing.f.z;
                break;
            default:
                data << uint8(MonsterMoveNormal);
                break;
        }

        // add fake Enter_Cycle flag - needed for client-side cyclic movement (client will erase first spline vertex after first cycle done)
        splineflags.enter_cycle = move_spline.isCyclic();
        data << uint32(splineflags & uint32(~MoveSplineFlag::Mask_No_Monster_Move));

        if (splineflags.animation)
        {
            data << splineflags.getAnimationId();
            data << move_spline.effect_start_time;
        }

        data << move_spline.Duration();

        if (splineflags.parabolic)
        {
            data << move_spline.vertical_acceleration;
            data << move_spline.effect_start_time;
        }
    }

    void PacketBuilder::WriteStopMovement(Vector3 const& pos, uint32 splineId, ByteBuffer& data, Unit* unit)
    {

        ObjectGuid guid = unit->GetGUID();
        ObjectGuid transport = unit->GetTransGUID();

        data.WriteBit(0); // 0x30

        data.WriteBit(guid[0]);

        data.WriteBit(1); // 0x70
        data.WriteBit(1); // 0x3D
        data.WriteBit(1); // 0x64
        data.WriteBit(0);

        data.WriteBit(transport[1]);
        data.WriteBit(transport[5]);
        data.WriteBit(transport[6]);
        data.WriteBit(transport[2]);
        data.WriteBit(transport[3]);
        data.WriteBit(transport[7]);
        data.WriteBit(transport[0]);
        data.WriteBit(transport[4]);

        data.WriteBit(guid[4]);
        data.WriteBit(1);

        data.WriteBits(0, 22); // WP count

        data.WriteBit(guid[3]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[6]);
        data.WriteBit(guid[5]);
        data.WriteBit(0); // Unk

        data.WriteBit(guid[1]);
        data.WriteBit(guid[2]);
        data.WriteBit(1); // Parabolic speed
        data.WriteBit(!unit->movespline->splineflags.raw());

        data.WriteBit(1); // 0x44
        data.WriteBits(0, 20);
        data.WriteBit(1); // 0x50

        data.WriteBits(MonsterMoveStop, 3);


        data.WriteBit(1); // 0x65
        data.WriteBit(1); // 0x40

        data.FlushBits();


        data << uint32(getMSTime());
        data.WriteByteSeq(transport[1]);
        data.WriteByteSeq(transport[7]);
        data.WriteByteSeq(transport[4]);
        data.WriteByteSeq(transport[6]);
        data.WriteByteSeq(transport[0]);
        data.WriteByteSeq(transport[2]);
        data.WriteByteSeq(transport[5]);
        data.WriteByteSeq(transport[3]);


        data << float(0.f);

        data << float(0.f);
        data.WriteByteSeq(guid[0]);

        if (unit->movespline->splineflags.raw())
            data << uint32(unit->movespline->splineflags.raw());

        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guid[6]);

        data << float(0.f);

        data.WriteByteSeq(guid[4]);
        data.WriteByteSeq(guid[2]);

        data << float(pos.y);
        data << float(pos.z);

        data.WriteByteSeq(guid[3]);
        data.WriteByteSeq(guid[7]);

        data << float(pos.x);

        data.WriteByteSeq(guid[1]);
    }

    void WriteLinearPath(Spline<int32> const& spline, ByteBuffer& data)
    {
        uint32 last_idx = spline.getPointCount() - 3;
        Vector3 const* real_path = &spline.getPoint(1);

        if (last_idx > 0)
        {
            Vector3 middle = (real_path[0] + real_path[last_idx]) / 2.f;
            Vector3 offset;
            // first and last points already appended
            for (uint32 i = 0; i < last_idx; ++i)
            {
                offset = middle - real_path[i];
                data.appendPackXYZ(offset.x, offset.y, offset.z);
            }
        }
    }

    void WriteUncompressedPath(Spline<int32> const& spline, ByteBuffer& data)
    {
        uint32 count = spline.getPointCount() - 3;
        data.append<Vector3>(&spline.getPoint(2), count);
    }

    void WriteUncompressedCyclicPath(Spline<int32> const& spline, ByteBuffer& data)
    {
        uint32 count = spline.getPointCount() - 3;
        data << spline.getPoint(1); // fake point, client will erase it from the spline after first cycle done
        data.append<Vector3>(&spline.getPoint(1), count);
    }

    void PacketBuilder::WriteMonsterMove(const MoveSpline& move_spline, WorldPacket& data, Unit* unit)
    {
        ObjectGuid guid = unit->GetGUID();
        ObjectGuid transport = unit->GetTransGUID();
        uint32 type;

        data.WriteBit(0); // 0x30

        data.WriteBit(guid[0]);

        data.WriteBit(1); // 0x70
        data.WriteBit(1); // 0x3D
        data.WriteBit(1); // 0x64
        data.WriteBit(0);

        data.WriteBit(transport[1]);
        data.WriteBit(transport[5]);
        data.WriteBit(transport[6]);
        data.WriteBit(transport[2]);
        data.WriteBit(transport[3]);
        data.WriteBit(transport[7]);
        data.WriteBit(transport[0]);
        data.WriteBit(transport[4]);

        data.WriteBit(guid[4]);
        data.WriteBit(!move_spline.Duration());

        int32 splineWpCount = move_spline.splineflags & MoveSplineFlag::UncompressedPath ? 1 : move_spline.spline.getPointCount() - 3;
        data.WriteBits(splineWpCount, 22); // WP count

        data.WriteBit(guid[3]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[6]);
        data.WriteBit(guid[5]);
        data.WriteBit(0); // Unk

        data.WriteBit(guid[1]);
        data.WriteBit(guid[2]);
        data.WriteBit(1); // Parabolic speed
        data.WriteBit(!move_spline.splineflags.raw());

        data.WriteBit(1); // 0x44
        data.WriteBits(!splineWpCount ? move_spline.spline.getPointCount() - 2 : 1, 20);
        data.WriteBit(1); // 0x50

        switch (move_spline.splineflags & MoveSplineFlag::Mask_Final_Facing)
        {
            case MoveSplineFlag::Final_Target:
                type = 3;
                break;
            case MoveSplineFlag::Final_Angle:
                type = 4;
                break;
            case MoveSplineFlag::Final_Point:
                type = 2;
                break;
            default:
                type = 0;
                break;
        }

        data.WriteBits(type, 3);

        if (type == 3)
        {
            ObjectGuid targetGuid = move_spline.facing.target;
            data.WriteBit(targetGuid[0]);
            data.WriteBit(targetGuid[5]);
            data.WriteBit(targetGuid[7]);
            data.WriteBit(targetGuid[1]);
            data.WriteBit(targetGuid[2]);
            data.WriteBit(targetGuid[4]);
            data.WriteBit(targetGuid[6]);
            data.WriteBit(targetGuid[3]);
        }

        data.WriteBit(1); // 0x65
        data.WriteBit(1); // 0x40

        data.FlushBits();

        if (type == 3)
        {
            ObjectGuid targetGuid = move_spline.facing.target;
            data.WriteBit(targetGuid[0]);
            data.WriteBit(targetGuid[1]);
            data.WriteBit(targetGuid[3]);
            data.WriteBit(targetGuid[7]);
            data.WriteBit(targetGuid[6]);
            data.WriteBit(targetGuid[5]);
            data.WriteBit(targetGuid[4]);
            data.WriteBit(targetGuid[2]);
        }

        if (move_spline.splineflags & MoveSplineFlag::UncompressedPath)
        {
            if (move_spline.splineflags.cyclic)
                WriteUncompressedCyclicPath(move_spline.spline, data);
            else
                WriteUncompressedPath(move_spline.spline, data);
        }
        else
        {
            uint32 last_idx = move_spline.spline.getPointCount() - 2;
            const Vector3 * real_path = &move_spline.spline.getPoint(1);
            data << real_path[last_idx].x << real_path[last_idx].y << real_path[last_idx].z; // destination
        }

        data << uint32(getMSTime());
        data.WriteByteSeq(transport[1]);
        data.WriteByteSeq(transport[7]);
        data.WriteByteSeq(transport[4]);
        data.WriteByteSeq(transport[6]);
        data.WriteByteSeq(transport[0]);
        data.WriteByteSeq(transport[2]);
        data.WriteByteSeq(transport[5]);
        data.WriteByteSeq(transport[3]);

        if (type == 2)
            data << move_spline.facing.f.z << move_spline.facing.f.y << move_spline.facing.f.x;

        data << float(0.f);

        WriteLinearPath(move_spline.spline, data);

        data << float(0.f);
        data.WriteByteSeq(guid[0]);

        if (move_spline.splineflags.raw())
            data << uint32(move_spline.splineflags.raw());

        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guid[6]);

        if (type == 4)
            data << float(move_spline.facing.angle);

        data << float(0.f);

        if (move_spline.Duration())
            data << uint32(move_spline.Duration());

        data.WriteByteSeq(guid[4]);
        data.WriteByteSeq(guid[2]);

        data << float(unit->GetPositionY());
        data << float(unit->GetPositionZ());
        data.WriteByteSeq(guid[3]);
        data.WriteByteSeq(guid[7]);

        data << float(unit->GetPositionX());

        data.WriteByteSeq(guid[1]);
    }

    void PacketBuilder::WriteCreateBits(MoveSpline const& moveSpline, ByteBuffer& data)
    {
        if (!data.WriteBit(!moveSpline.Finalized()))
            return;

        data.WriteBits(uint8(moveSpline.spline.mode()), 2);
        data.WriteBit(moveSpline.splineflags & (MoveSplineFlag::Parabolic | MoveSplineFlag::Animation));
        data.WriteBits(moveSpline.getPath().size(), 22);
        switch (moveSpline.splineflags & MoveSplineFlag::Mask_Final_Facing)
        {
            case MoveSplineFlag::Final_Target:
            {
                ObjectGuid targetGuid = moveSpline.facing.target;
                data.WriteBits(2, 2);
                data.WriteBit(targetGuid[4]);
                data.WriteBit(targetGuid[3]);
                data.WriteBit(targetGuid[7]);
                data.WriteBit(targetGuid[2]);
                data.WriteBit(targetGuid[6]);
                data.WriteBit(targetGuid[1]);
                data.WriteBit(targetGuid[0]);
                data.WriteBit(targetGuid[5]);
                break;
            }
            case MoveSplineFlag::Final_Angle:
                data.WriteBits(0, 2);
                break;
            case MoveSplineFlag::Final_Point:
                data.WriteBits(1, 2);
                break;
            default:
                data.WriteBits(3, 2);
                break;
        }

        data.WriteBit((moveSpline.splineflags & MoveSplineFlag::Parabolic) && moveSpline.effect_start_time < moveSpline.Duration());
        data.WriteBits(moveSpline.splineflags.raw(), 25);
    }

    void PacketBuilder::WriteCreateData(MoveSpline const& moveSpline, ByteBuffer& data)
    {
        if (!moveSpline.Finalized())
        {
            MoveSplineFlag const& splineFlags = moveSpline.splineflags;

            if ((splineFlags & MoveSplineFlag::Parabolic) && moveSpline.effect_start_time < moveSpline.Duration())
                data << moveSpline.vertical_acceleration;   // added in 3.1

            data << moveSpline.timePassed();

            if (splineFlags.final_angle)
                data << moveSpline.facing.angle;
            else if (splineFlags.final_target)
            {
                ObjectGuid facingGuid = moveSpline.facing.target;
                data.WriteByteSeq(facingGuid[5]);
                data.WriteByteSeq(facingGuid[3]);
                data.WriteByteSeq(facingGuid[7]);
                data.WriteByteSeq(facingGuid[1]);
                data.WriteByteSeq(facingGuid[6]);
                data.WriteByteSeq(facingGuid[4]);
                data.WriteByteSeq(facingGuid[2]);
                data.WriteByteSeq(facingGuid[0]);
            }

            uint32 nodes = moveSpline.getPath().size();
            for (uint32 i = 0; i < nodes; ++i)
            {
                data << float(moveSpline.getPath()[i].z);
                data << float(moveSpline.getPath()[i].x);
                data << float(moveSpline.getPath()[i].y);
            }

            if (splineFlags.final_point)
                data << moveSpline.facing.f.x << moveSpline.facing.f.z << moveSpline.facing.f.y;

            data << float(1.f);                             // splineInfo.duration_mod_next; added in 3.1
            data << moveSpline.Duration();
            if (splineFlags & (MoveSplineFlag::Parabolic | MoveSplineFlag::Animation))
                data << moveSpline.effect_start_time;       // added in 3.1

            data << float(1.f);                             // splineInfo.duration_mod; added in 3.1
        }

        if (!moveSpline.isCyclic())
        {
            Vector3 dest = moveSpline.FinalDestination();
            data << float(dest.z);
            data << float(dest.x);
            data << float(dest.y);
        }
        else
            data << Vector3::zero();

        data << moveSpline.GetId();
    }
}
