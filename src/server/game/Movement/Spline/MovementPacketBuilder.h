/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_PACKET_BUILDER_H
#define SKYFIRESERVER_PACKET_BUILDER_H

#include "Define.h" // for uint32
#include "G3D/Vector3.h"
#include "Unit.h"
using G3D::Vector3;

class ByteBuffer;
class WorldPacket;

namespace Movement
{
    enum MonsterMoveType
    {
        MonsterMoveNormal = 0,
        MonsterMoveStop = 1,
        MonsterMoveFacingPoint = 2,
        MonsterMoveFacingTarget = 3,
        MonsterMoveFacingAngle = 4
    };

    class MoveSpline;
    class PacketBuilder
    {
    public:

        static void WriteMonsterMove(const MoveSpline& mov, WorldPacket& data, Unit* unit);
        static void WriteStopMovement(Vector3 const& loc, uint32 splineId, ByteBuffer& data, Unit* unit);
        static void WriteCreateBits(MoveSpline const& moveSpline, ByteBuffer& data);
        static void WriteCreateData(MoveSpline const& moveSpline, ByteBuffer& data);
        static void WriteFacingTargetPart(MoveSpline const& moveSpline, ByteBuffer& data);
        static MonsterMoveType GetMonsterMoveType(MoveSpline const& moveSpline);
    };
}
#endif // SKYFIRESERVER_PACKET_BUILDER_H
