/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "CreatureAI.h"
#include "GameObjectAI.h"
#include "GuildMgr.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "Totem.h"
#include "WorldPacket.h"
#include "WorldSession.h"

namespace
{
struct GuidRequest
{
    ObjectGuid guid;
};

struct OpenItemRequest
{
    uint8 bagIndex;
    uint8 slot;
};

struct UseItemRequest
{
    uint8 bagIndex;
    uint8 slot;
    uint8 castCount;
    uint8 castFlags;
    uint32 spellId;
    uint32 glyphIndex;
    uint32 targetMask;
    float elevation;
    float missileSpeed;
    ObjectGuid itemGuid;
    ObjectGuid targetGuid;
    ObjectGuid itemTargetGuid;
    ObjectGuid destTransportGuid;
    ObjectGuid srcTransportGuid;
    Position srcPos;
    Position destPos;
    std::string targetString;
    SpellResearchData researchData;
    bool hasResearchData;
};

struct CastSpellRequest
{
    uint8 castCount;
    uint8 castFlags;
    uint32 spellId;
    uint32 glyphIndex;
    uint32 targetMask;
    float elevation;
    float missileSpeed;
    ObjectGuid targetGuid;
    ObjectGuid itemTargetGuid;
    ObjectGuid destTransportGuid;
    ObjectGuid srcTransportGuid;
    Position srcPos;
    Position destPos;
    std::string targetString;
    SpellResearchData researchData;
    bool hasResearchData;
};

struct CancelCastRequest
{
    uint32 spellId;
    uint8 counter;
};

struct CancelAuraRequest
{
    ObjectGuid guid;
    uint32 spellId;
};

struct PetCancelAuraRequest
{
    ObjectGuid guid;
    uint32 spellId;
};

struct TotemDestroyedRequest
{
    ObjectGuid guid;
    uint8 slotId;
};

struct SpellClickRequest
{
    ObjectGuid guid;
    bool tryAutoDismount;
};

struct ProjectilePositionRequest
{
    uint64 casterGuid;
    uint32 spellId;
    uint8 castCount;
    float x;
    float y;
    float z;
};

UseItemRequest ReadUseItemRequest(WorldPacket& recvPacket, Unit* caster)
{
    UseItemRequest request = UseItemRequest();
    request.hasResearchData = false;
    memset(&request.researchData, 0, sizeof(SpellResearchData));
    uint32 targetStringLength = 0;
    uint8 researchTypes[4] = { 0, 0, 0, 0 };

    // Movement data
    MovementInfo movementInfo;
    ObjectGuid movementTransportGuid = 0;
    ObjectGuid movementGuid = 0;
    bool hasTransport = false;
    bool hasTransportTime2 = false;
    bool hasTransportTime3 = false;
    bool hasFallData = false;
    bool hasFallDirection = false;
    bool hasTimestamp = false;
    bool hasSplineElevation = false;
    bool hasPitch = false;
    bool hasOrientation = false;
    bool hasUnkMovementField = false;
    uint32 unkMovementLoopCounter = 0;

    recvPacket >> request.slot >> request.bagIndex;

    bool hasElevation = !recvPacket.ReadBit();
    request.itemGuid[6] = recvPacket.ReadBit();
    bool hasTargetString = !recvPacket.ReadBit();
    request.itemGuid[1] = recvPacket.ReadBit();
    bool hasCastFlags = !recvPacket.ReadBit();
    bool hasDestLocation = recvPacket.ReadBit();
    request.itemGuid[2] = recvPacket.ReadBit();
    request.itemGuid[7] = recvPacket.ReadBit();
    request.itemGuid[0] = recvPacket.ReadBit();
    bool hasTargetMask = !recvPacket.ReadBit();
    bool hasMissileSpeed = !recvPacket.ReadBit();
    bool hasMovement = recvPacket.ReadBit();
    bool hasCastCount = !recvPacket.ReadBit();
    bool hasSpellId = !recvPacket.ReadBit();
    recvPacket.ReadBit();
    bool hasGlyphIndex = !recvPacket.ReadBit();
    recvPacket.ReadBit();
    request.itemGuid[4] = recvPacket.ReadBit();
    bool hasSrcLocation = recvPacket.ReadBit();
    request.itemGuid[3] = recvPacket.ReadBit();
    request.itemGuid[5] = recvPacket.ReadBit();
    uint8 researchDataCount = recvPacket.ReadBits(2);

    for (uint8 i = 0; i < researchDataCount; ++i)
        researchTypes[i] = recvPacket.ReadBits(2);

    if (hasMovement)
    {
        hasPitch = !recvPacket.ReadBit();
        hasTransport = recvPacket.ReadBit();
        recvPacket.ReadBit();

        if (hasTransport)
        {
            movementTransportGuid[7] = recvPacket.ReadBit();
            movementTransportGuid[2] = recvPacket.ReadBit();
            movementTransportGuid[4] = recvPacket.ReadBit();
            movementTransportGuid[5] = recvPacket.ReadBit();
            movementTransportGuid[6] = recvPacket.ReadBit();
            movementTransportGuid[0] = recvPacket.ReadBit();
            movementTransportGuid[1] = recvPacket.ReadBit();
            hasTransportTime3 = recvPacket.ReadBit();
            movementTransportGuid[4] = recvPacket.ReadBit();
            hasTransportTime2 = recvPacket.ReadBit();
        }

        movementGuid[6] = recvPacket.ReadBit();
        movementGuid[2] = recvPacket.ReadBit();
        movementGuid[1] = recvPacket.ReadBit();
        unkMovementLoopCounter = recvPacket.ReadBits(22);
        recvPacket.ReadBit();
        bool hasMovementFlags2 = !recvPacket.ReadBit();
        hasFallData = recvPacket.ReadBit();
        movementGuid[5] = recvPacket.ReadBit();
        hasSplineElevation = !recvPacket.ReadBit();
        recvPacket.ReadBit();
        movementGuid[7] = recvPacket.ReadBit();
        movementGuid[0] = recvPacket.ReadBit();

        if (hasFallData)
            hasFallDirection = recvPacket.ReadBit();

        hasOrientation = !recvPacket.ReadBit();
        movementGuid[4] = recvPacket.ReadBit();
        movementGuid[3] = recvPacket.ReadBit();
        hasTimestamp = !recvPacket.ReadBit();
        hasUnkMovementField = !recvPacket.ReadBit();
        bool hasMovementFlags = !recvPacket.ReadBit();

        if (hasMovementFlags2)
            movementInfo.flags2 = recvPacket.ReadBits(13);

        if (hasMovement)
            movementInfo.flags = recvPacket.ReadBits(30);
    }

    if (hasSrcLocation)
    {
        request.srcTransportGuid[3] = recvPacket.ReadBit();
        request.srcTransportGuid[1] = recvPacket.ReadBit();
        request.srcTransportGuid[7] = recvPacket.ReadBit();
        request.srcTransportGuid[4] = recvPacket.ReadBit();
        request.srcTransportGuid[2] = recvPacket.ReadBit();
        request.srcTransportGuid[0] = recvPacket.ReadBit();
        request.srcTransportGuid[6] = recvPacket.ReadBit();
        request.srcTransportGuid[5] = recvPacket.ReadBit();
    }

    if (hasDestLocation)
    {
        request.destTransportGuid[2] = recvPacket.ReadBit();
        request.destTransportGuid[4] = recvPacket.ReadBit();
        request.destTransportGuid[1] = recvPacket.ReadBit();
        request.destTransportGuid[7] = recvPacket.ReadBit();
        request.destTransportGuid[6] = recvPacket.ReadBit();
        request.destTransportGuid[0] = recvPacket.ReadBit();
        request.destTransportGuid[3] = recvPacket.ReadBit();
        request.destTransportGuid[5] = recvPacket.ReadBit();
    }

    if (hasTargetString)
        targetStringLength = recvPacket.ReadBits(7);

    request.targetGuid[1] = recvPacket.ReadBit();
    request.targetGuid[0] = recvPacket.ReadBit();
    request.targetGuid[5] = recvPacket.ReadBit();
    request.targetGuid[3] = recvPacket.ReadBit();
    request.targetGuid[6] = recvPacket.ReadBit();
    request.targetGuid[4] = recvPacket.ReadBit();
    request.targetGuid[7] = recvPacket.ReadBit();
    request.targetGuid[2] = recvPacket.ReadBit();

    request.itemTargetGuid[4] = recvPacket.ReadBit();
    request.itemTargetGuid[5] = recvPacket.ReadBit();
    request.itemTargetGuid[0] = recvPacket.ReadBit();
    request.itemTargetGuid[1] = recvPacket.ReadBit();
    request.itemTargetGuid[3] = recvPacket.ReadBit();
    request.itemTargetGuid[7] = recvPacket.ReadBit();
    request.itemTargetGuid[6] = recvPacket.ReadBit();
    request.itemTargetGuid[2] = recvPacket.ReadBit();

    if (hasCastFlags)
        request.castFlags = recvPacket.ReadBits(5);

    if (hasTargetMask)
        request.targetMask = recvPacket.ReadBits(20);

    recvPacket.ReadByteSeq(request.itemGuid[0]);
    recvPacket.ReadByteSeq(request.itemGuid[5]);
    recvPacket.ReadByteSeq(request.itemGuid[6]);
    recvPacket.ReadByteSeq(request.itemGuid[3]);
    recvPacket.ReadByteSeq(request.itemGuid[4]);
    recvPacket.ReadByteSeq(request.itemGuid[2]);
    recvPacket.ReadByteSeq(request.itemGuid[1]);

    for (uint8 i = 0; i < researchDataCount; ++i)
    {
        // Client sends (usedCount, entryId); type 1 = keystone item, type 2 = fragment currency.
        uint32 usedCount = 0;
        uint32 entry = 0;
        recvPacket >> usedCount;
        recvPacket >> entry;

        switch (researchTypes[i])
        {
            case 1: // Keystones
                request.researchData.keystoneItemId = entry;
                request.researchData.keystoneCount = usedCount;
                request.hasResearchData = true;
                break;
            case 2: // Fragments
                request.researchData.fragmentCurrencyId = entry;
                request.researchData.fragmentCount = usedCount;
                request.hasResearchData = true;
                break;
            default:
                break;
        }
    }

    recvPacket.ReadByteSeq(request.itemGuid[7]);

    if (hasMovement)
    {
        for (uint8 i = 0; i != unkMovementLoopCounter; i++)
            recvPacket.read_skip<uint32>();

        if (hasTransport)
        {
            recvPacket >> movementInfo.transport.pos.m_positionY;
            recvPacket >> movementInfo.transport.pos.m_positionZ;
            recvPacket.ReadByteSeq(movementTransportGuid[1]);

            if (hasTransportTime3)
                recvPacket >> movementInfo.transport.time3;

            recvPacket.ReadByteSeq(movementTransportGuid[7]);
            recvPacket.ReadByteSeq(movementTransportGuid[5]);
            recvPacket.ReadByteSeq(movementTransportGuid[2]);
            recvPacket.ReadByteSeq(movementTransportGuid[4]);
            recvPacket >> movementInfo.transport.pos.m_positionX;
            movementInfo.transport.pos.SetOrientation(recvPacket.read<float>());
            recvPacket.ReadByteSeq(movementTransportGuid[0]);
            recvPacket >> movementInfo.transport.seat;
            recvPacket >> movementInfo.transport.time;
            recvPacket.ReadByteSeq(movementTransportGuid[6]);
            recvPacket.ReadByteSeq(movementTransportGuid[3]);

            if (hasTransportTime2)
                recvPacket >> movementInfo.transport.time2;
        }

        if (hasFallData)
        {
            recvPacket >> movementInfo.jump.zspeed;

            if (hasFallDirection)
            {
                recvPacket >> movementInfo.jump.sinAngle;
                recvPacket >> movementInfo.jump.cosAngle;
                recvPacket >> movementInfo.jump.xyspeed;
            }

            recvPacket >> movementInfo.jump.fallTime;
        }

        recvPacket.ReadByteSeq(movementTransportGuid[3]);
        recvPacket.ReadByteSeq(movementTransportGuid[7]);
        recvPacket.ReadByteSeq(movementTransportGuid[6]);
        recvPacket.ReadByteSeq(movementTransportGuid[1]);
        recvPacket >> movementInfo.pos.m_positionY;

        if (hasSplineElevation)
            recvPacket >> movementInfo.splineElevation;

        if (hasUnkMovementField)
            recvPacket.read_skip<uint32>();

        if (hasOrientation)
            movementInfo.pos.SetOrientation(recvPacket.read<float>());

        recvPacket.ReadByteSeq(movementTransportGuid[2]);
        recvPacket >> movementInfo.pos.m_positionZ;

        if (hasTimestamp)
            recvPacket >> movementInfo.time;

        recvPacket >> movementInfo.pos.m_positionX;
        recvPacket.ReadByteSeq(movementTransportGuid[5]);
        recvPacket.ReadByteSeq(movementTransportGuid[0]);

        if (hasPitch)
            movementInfo.pitch = G3D::wrap(recvPacket.read<float>(), float(-M_PI), float(M_PI));

        recvPacket.ReadByteSeq(movementTransportGuid[4]);
    }

    if (hasDestLocation)
    {
        float x, y, z;

        recvPacket.ReadByteSeq(request.destTransportGuid[7]);
        recvPacket >> x;
        recvPacket.ReadByteSeq(request.destTransportGuid[0]);
        recvPacket.ReadByteSeq(request.destTransportGuid[6]);
        recvPacket.ReadByteSeq(request.destTransportGuid[1]);
        recvPacket.ReadByteSeq(request.destTransportGuid[3]);
        recvPacket >> y;
        recvPacket.ReadByteSeq(request.destTransportGuid[5]);
        recvPacket >> z;
        recvPacket.ReadByteSeq(request.destTransportGuid[4]);
        recvPacket.ReadByteSeq(request.destTransportGuid[2]);

        request.destPos.Relocate(x, y, z);
    }
    else
    {
        request.destTransportGuid = caster->GetTransGUID();

        if (request.destTransportGuid)
            request.destPos.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            request.destPos.Relocate(caster);
    }

    recvPacket.ReadByteSeq(request.itemTargetGuid[6]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[7]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[2]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[0]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[3]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[4]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[1]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[5]);

    if (hasSrcLocation)
    {
        float x, y, z;

        recvPacket.ReadByteSeq(request.srcTransportGuid[7]);
        recvPacket >> x;
        recvPacket.ReadByteSeq(request.srcTransportGuid[1]);
        recvPacket.ReadByteSeq(request.srcTransportGuid[5]);
        recvPacket.ReadByteSeq(request.srcTransportGuid[4]);
        recvPacket >> z;
        recvPacket.ReadByteSeq(request.srcTransportGuid[6]);
        recvPacket.ReadByteSeq(request.srcTransportGuid[0]);
        recvPacket.ReadByteSeq(request.srcTransportGuid[3]);
        recvPacket >> y;
        recvPacket.ReadByteSeq(request.srcTransportGuid[2]);

        request.srcPos.Relocate(x, y, z);
    }
    else
    {
        request.srcTransportGuid = caster->GetTransGUID();
        if (request.srcTransportGuid)
            request.srcPos.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            request.srcPos.Relocate(caster);
    }

    if (hasSpellId)
        recvPacket >> request.spellId;

    recvPacket.ReadByteSeq(request.targetGuid[1]);
    recvPacket.ReadByteSeq(request.targetGuid[4]);
    recvPacket.ReadByteSeq(request.targetGuid[3]);
    recvPacket.ReadByteSeq(request.targetGuid[6]);
    recvPacket.ReadByteSeq(request.targetGuid[2]);
    recvPacket.ReadByteSeq(request.targetGuid[0]);
    recvPacket.ReadByteSeq(request.targetGuid[7]);
    recvPacket.ReadByteSeq(request.targetGuid[5]);

    if (hasTargetString)
        request.targetString = recvPacket.ReadString(targetStringLength);

    if (hasElevation)
        recvPacket >> request.elevation;

    if (hasGlyphIndex)
        recvPacket >> request.glyphIndex;

    if (hasMissileSpeed)
        recvPacket >> request.missileSpeed;

    if (hasCastCount)
        recvPacket >> request.castCount;

    return request;
}

OpenItemRequest ReadOpenItemRequest(WorldPacket& recvPacket)
{
    OpenItemRequest request = OpenItemRequest();
    recvPacket >> request.bagIndex >> request.slot;
    return request;
}

GuidRequest ReadGameObjectUseRequest(WorldPacket& recvData)
{
    GuidRequest request = GuidRequest();

    request.guid[6] = recvData.ReadBit();
    request.guid[1] = recvData.ReadBit();
    request.guid[3] = recvData.ReadBit();
    request.guid[4] = recvData.ReadBit();
    request.guid[0] = recvData.ReadBit();
    request.guid[5] = recvData.ReadBit();
    request.guid[7] = recvData.ReadBit();
    request.guid[2] = recvData.ReadBit();

    recvData.ReadByteSeq(request.guid[0]);
    recvData.ReadByteSeq(request.guid[1]);
    recvData.ReadByteSeq(request.guid[6]);
    recvData.ReadByteSeq(request.guid[2]);
    recvData.ReadByteSeq(request.guid[3]);
    recvData.ReadByteSeq(request.guid[4]);
    recvData.ReadByteSeq(request.guid[5]);
    recvData.ReadByteSeq(request.guid[7]);

    return request;
}

GuidRequest ReadGameObjectReportUseRequest(WorldPacket& recvPacket)
{
    GuidRequest request = GuidRequest();

    request.guid[4] = recvPacket.ReadBit();
    request.guid[7] = recvPacket.ReadBit();
    request.guid[5] = recvPacket.ReadBit();
    request.guid[3] = recvPacket.ReadBit();
    request.guid[6] = recvPacket.ReadBit();
    request.guid[1] = recvPacket.ReadBit();
    request.guid[2] = recvPacket.ReadBit();
    request.guid[0] = recvPacket.ReadBit();

    recvPacket.ReadByteSeq(request.guid[7]);
    recvPacket.ReadByteSeq(request.guid[1]);
    recvPacket.ReadByteSeq(request.guid[6]);
    recvPacket.ReadByteSeq(request.guid[5]);
    recvPacket.ReadByteSeq(request.guid[0]);
    recvPacket.ReadByteSeq(request.guid[3]);
    recvPacket.ReadByteSeq(request.guid[2]);
    recvPacket.ReadByteSeq(request.guid[4]);

    return request;
}

CastSpellRequest ReadCastSpellRequest(WorldPacket& recvPacket, Unit* caster)
{
    CastSpellRequest request = CastSpellRequest();
    request.hasResearchData = false;
    memset(&request.researchData, 0, sizeof(SpellResearchData));
    uint32 targetStringLength = 0;
    uint8 researchTypes[4] = { 0, 0, 0, 0 };

    // Movement data
    MovementInfo movementInfo;
    ObjectGuid movementTransportGuid = 0;
    ObjectGuid movementGuid = 0;
    bool hasTransport = false;
    bool hasTransportTime2 = false;
    bool hasTransportTime3 = false;
    bool hasFallData = false;
    bool hasFallDirection = false;
    bool hasTimestamp = false;
    bool hasSplineElevation = false;
    bool hasPitch = false;
    bool hasOrientation = false;
    bool hasUnkMovementField = false;
    uint32 unkMovementLoopCounter = 0;

    recvPacket.ReadBit(); // Fake bit
    bool hasTargetString = !recvPacket.ReadBit();
    recvPacket.ReadBit(); // Fake Bit
    bool hasCastCount = !recvPacket.ReadBit();
    bool hasSrcLocation = recvPacket.ReadBit();
    bool hasDestLocation = recvPacket.ReadBit();
    bool hasSpellId = !recvPacket.ReadBit();
    uint8 researchDataCount = recvPacket.ReadBits(2);
    bool hasTargetMask = !recvPacket.ReadBit();
    bool hasMissileSpeed = !recvPacket.ReadBit();

    for (uint8 i = 0; i < researchDataCount; ++i)
        researchTypes[i] = recvPacket.ReadBits(2);

    bool hasGlyphIndex = !recvPacket.ReadBit();
    bool hasMovement = recvPacket.ReadBit();
    bool hasElevation = !recvPacket.ReadBit();
    bool hasCastFlags = !recvPacket.ReadBit();

    request.targetGuid[5] = recvPacket.ReadBit();
    request.targetGuid[4] = recvPacket.ReadBit();
    request.targetGuid[2] = recvPacket.ReadBit();
    request.targetGuid[7] = recvPacket.ReadBit();
    request.targetGuid[1] = recvPacket.ReadBit();
    request.targetGuid[6] = recvPacket.ReadBit();
    request.targetGuid[3] = recvPacket.ReadBit();
    request.targetGuid[0] = recvPacket.ReadBit();

    if (hasDestLocation)
    {
        request.destTransportGuid[1] = recvPacket.ReadBit();
        request.destTransportGuid[3] = recvPacket.ReadBit();
        request.destTransportGuid[5] = recvPacket.ReadBit();
        request.destTransportGuid[0] = recvPacket.ReadBit();
        request.destTransportGuid[2] = recvPacket.ReadBit();
        request.destTransportGuid[6] = recvPacket.ReadBit();
        request.destTransportGuid[7] = recvPacket.ReadBit();
        request.destTransportGuid[4] = recvPacket.ReadBit();
    }


    if (hasMovement)
    {
        unkMovementLoopCounter = recvPacket.ReadBits(22);
        recvPacket.ReadBit();
        movementGuid[4] = recvPacket.ReadBit();
        hasTransport = recvPacket.ReadBit();

        if (hasTransport)
        {
            hasTransportTime2 = recvPacket.ReadBit();
            movementTransportGuid[7] = recvPacket.ReadBit();
            movementTransportGuid[4] = recvPacket.ReadBit();
            movementTransportGuid[1] = recvPacket.ReadBit();
            movementTransportGuid[0] = recvPacket.ReadBit();
            movementTransportGuid[6] = recvPacket.ReadBit();
            movementTransportGuid[3] = recvPacket.ReadBit();
            movementTransportGuid[5] = recvPacket.ReadBit();
            hasTransportTime3 = recvPacket.ReadBit();
            movementTransportGuid[2] = recvPacket.ReadBit();
        }

        recvPacket.ReadBit();
        movementGuid[7] = recvPacket.ReadBit();
        hasOrientation = !recvPacket.ReadBit();
        movementGuid[6] = recvPacket.ReadBit();
        hasSplineElevation = !recvPacket.ReadBit();
        hasPitch = !recvPacket.ReadBit();
        movementGuid[0] = recvPacket.ReadBit();
        recvPacket.ReadBit();
        bool hasMovementFlags = !recvPacket.ReadBit();
        hasTimestamp = !recvPacket.ReadBit();
        hasUnkMovementField = !recvPacket.ReadBit();

        if (hasMovementFlags)
            movementInfo.flags = recvPacket.ReadBits(30);

        movementGuid[1] = recvPacket.ReadBit();
        movementGuid[3] = recvPacket.ReadBit();
        movementGuid[2] = recvPacket.ReadBit();
        movementGuid[5] = recvPacket.ReadBit();
        hasFallData = recvPacket.ReadBit();

        if (hasFallData)
            hasFallDirection = recvPacket.ReadBit();

        bool hasMovementFlags2 = !recvPacket.ReadBit();

        if (hasMovementFlags2)
            movementInfo.flags2 = recvPacket.ReadBits(13);
    }

    request.itemTargetGuid[1] = recvPacket.ReadBit();
    request.itemTargetGuid[0] = recvPacket.ReadBit();
    request.itemTargetGuid[7] = recvPacket.ReadBit();
    request.itemTargetGuid[4] = recvPacket.ReadBit();
    request.itemTargetGuid[6] = recvPacket.ReadBit();
    request.itemTargetGuid[5] = recvPacket.ReadBit();
    request.itemTargetGuid[3] = recvPacket.ReadBit();
    request.itemTargetGuid[2] = recvPacket.ReadBit();

    if (hasSrcLocation)
    {
        request.srcTransportGuid[4] = recvPacket.ReadBit();
        request.srcTransportGuid[5] = recvPacket.ReadBit();
        request.srcTransportGuid[3] = recvPacket.ReadBit();
        request.srcTransportGuid[0] = recvPacket.ReadBit();
        request.srcTransportGuid[7] = recvPacket.ReadBit();
        request.srcTransportGuid[1] = recvPacket.ReadBit();
        request.srcTransportGuid[6] = recvPacket.ReadBit();
        request.srcTransportGuid[2] = recvPacket.ReadBit();
    }

    if (hasTargetMask)
        request.targetMask = recvPacket.ReadBits(20);

    if (hasCastFlags)
        request.castFlags = recvPacket.ReadBits(5);

    if (hasTargetString)
        targetStringLength = recvPacket.ReadBits(7);

    for (uint8 i = 0; i < researchDataCount; ++i)
    {
        // Client sends (usedCount, entryId); type 1 = keystone item, type 2 = fragment currency.
        uint32 usedCount = 0;
        uint32 entry = 0;
        recvPacket >> usedCount;
        recvPacket >> entry;

        switch (researchTypes[i])
        {
            case 1: // Keystones
                request.researchData.keystoneItemId = entry;
                request.researchData.keystoneCount = usedCount;
                request.hasResearchData = true;
                break;
            case 2: // Fragments
                request.researchData.fragmentCurrencyId = entry;
                request.researchData.fragmentCount = usedCount;
                request.hasResearchData = true;
                break;
            default:
                break;
        }
    }

    if (hasMovement)
    {
        recvPacket >> movementInfo.pos.m_positionX;
        recvPacket.ReadByteSeq(movementGuid[0]);

        if (hasTransport)
        {
            recvPacket.ReadByteSeq(movementTransportGuid[2]);
            recvPacket >> movementInfo.transport.seat;
            recvPacket.ReadByteSeq(movementTransportGuid[3]);
            recvPacket.ReadByteSeq(movementTransportGuid[7]);
            recvPacket >> movementInfo.transport.pos.m_positionX;
            recvPacket.ReadByteSeq(movementTransportGuid[5]);

            if (hasTransportTime3)
                recvPacket >> movementInfo.transport.time3;

            recvPacket >> movementInfo.transport.pos.m_positionZ;
            recvPacket >> movementInfo.transport.pos.m_positionY;

            recvPacket.ReadByteSeq(movementTransportGuid[6]);
            recvPacket.ReadByteSeq(movementTransportGuid[1]);
            movementInfo.transport.pos.SetOrientation(recvPacket.read<float>());

            recvPacket.ReadByteSeq(movementTransportGuid[4]);

            if (hasTransportTime2)
                recvPacket >> movementInfo.transport.time2;

            recvPacket.ReadByteSeq(movementTransportGuid[0]);
            recvPacket >> movementInfo.transport.time;
        }

        recvPacket.ReadByteSeq(movementGuid[5]);

        if (hasFallData)
        {
            recvPacket >> movementInfo.jump.fallTime;
            recvPacket >> movementInfo.jump.zspeed;

            if (hasFallDirection)
            {
                recvPacket >> movementInfo.jump.sinAngle;
                recvPacket >> movementInfo.jump.xyspeed;
                recvPacket >> movementInfo.jump.cosAngle;
            }
        }

        if (hasSplineElevation)
            recvPacket >> movementInfo.splineElevation;

        recvPacket.ReadByteSeq(movementGuid[6]);

        if (hasUnkMovementField)
            recvPacket.read_skip<uint32>();

        recvPacket.ReadByteSeq(movementGuid[4]);

        if (hasOrientation)
            movementInfo.pos.SetOrientation(recvPacket.read<float>());

        if (hasTimestamp)
            recvPacket >> movementInfo.time;

        recvPacket.ReadByteSeq(movementGuid[1]);

        if (hasPitch)
            movementInfo.pitch = G3D::wrap(recvPacket.read<float>(), float(-M_PI), float(M_PI));

        recvPacket.ReadByteSeq(movementGuid[3]);

        for (uint8 i = 0; i != unkMovementLoopCounter; i++)
            recvPacket.read_skip<uint32>();

        recvPacket >> movementInfo.pos.m_positionY;
        recvPacket.ReadByteSeq(movementGuid[7]);
        recvPacket >> movementInfo.pos.m_positionZ;
        recvPacket.ReadByteSeq(movementGuid[2]);
    }

    recvPacket.ReadByteSeq(request.itemTargetGuid[4]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[2]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[1]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[5]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[7]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[3]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[6]);
    recvPacket.ReadByteSeq(request.itemTargetGuid[0]);

    if (hasDestLocation)
    {
        float x, y, z;
        recvPacket.ReadByteSeq(request.destTransportGuid[2]);
        recvPacket >> x;
        recvPacket.ReadByteSeq(request.destTransportGuid[4]);
        recvPacket.ReadByteSeq(request.destTransportGuid[1]);
        recvPacket.ReadByteSeq(request.destTransportGuid[0]);
        recvPacket.ReadByteSeq(request.destTransportGuid[3]);
        recvPacket >> y;
        recvPacket.ReadByteSeq(request.destTransportGuid[7]);
        recvPacket >> z;
        recvPacket.ReadByteSeq(request.destTransportGuid[5]);
        recvPacket.ReadByteSeq(request.destTransportGuid[6]);
        request.destPos.Relocate(x, y, z);
    }
    else
    {
        request.destTransportGuid = caster->GetTransGUID();
        if (request.destTransportGuid)
            request.destPos.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            request.destPos.Relocate(caster);
    }

    recvPacket.ReadByteSeq(request.targetGuid[3]);
    recvPacket.ReadByteSeq(request.targetGuid[4]);
    recvPacket.ReadByteSeq(request.targetGuid[7]);
    recvPacket.ReadByteSeq(request.targetGuid[6]);
    recvPacket.ReadByteSeq(request.targetGuid[2]);
    recvPacket.ReadByteSeq(request.targetGuid[0]);
    recvPacket.ReadByteSeq(request.targetGuid[1]);
    recvPacket.ReadByteSeq(request.targetGuid[5]);

    if (hasSrcLocation)
    {
        float x, y, z;
        recvPacket >> y;
        recvPacket.ReadByteSeq(request.srcTransportGuid[5]);
        recvPacket.ReadByteSeq(request.srcTransportGuid[1]);
        recvPacket.ReadByteSeq(request.srcTransportGuid[7]);
        recvPacket.ReadByteSeq(request.srcTransportGuid[6]);
        recvPacket >> x;
        recvPacket.ReadByteSeq(request.srcTransportGuid[3]);
        recvPacket.ReadByteSeq(request.srcTransportGuid[2]);
        recvPacket.ReadByteSeq(request.srcTransportGuid[0]);
        recvPacket.ReadByteSeq(request.srcTransportGuid[4]);
        recvPacket >> z;
        request.srcPos.Relocate(x, y, z);
    }
    else
    {
        request.srcTransportGuid = caster->GetTransGUID();
        if (request.srcTransportGuid)
            request.srcPos.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            request.srcPos.Relocate(caster);
    }

    if (hasTargetString)
        request.targetString = recvPacket.ReadString(targetStringLength);

    if (hasMissileSpeed)
        recvPacket >> request.missileSpeed;

    if (hasElevation)
        recvPacket >> request.elevation;

    if (hasCastCount)
        recvPacket >> request.castCount;

    if (hasSpellId)
        recvPacket >> request.spellId;

    if (hasGlyphIndex)
        recvPacket >> request.glyphIndex;

    return request;
}

CancelCastRequest ReadCancelCastRequest(WorldPacket& recvPacket)
{
    CancelCastRequest request = CancelCastRequest();

    bool hasCounter = !recvPacket.ReadBit();
    bool hasSpellId = !recvPacket.ReadBit();

    recvPacket.FlushBits();

    if (hasSpellId)
        recvPacket >> request.spellId;

    if (hasCounter)
        recvPacket >> request.counter;

    return request;
}

CancelAuraRequest ReadCancelAuraRequest(WorldPacket& recvPacket)
{
    CancelAuraRequest request = CancelAuraRequest();

    recvPacket >> request.spellId;

    recvPacket.ReadBit(); // Fake Bit
    request.guid[6] = recvPacket.ReadBit();
    request.guid[5] = recvPacket.ReadBit();
    request.guid[1] = recvPacket.ReadBit();
    request.guid[0] = recvPacket.ReadBit();
    request.guid[4] = recvPacket.ReadBit();
    request.guid[3] = recvPacket.ReadBit();
    request.guid[2] = recvPacket.ReadBit();
    request.guid[7] = recvPacket.ReadBit();

    recvPacket.FlushBits();

    recvPacket.ReadByteSeq(request.guid[3]);
    recvPacket.ReadByteSeq(request.guid[2]);
    recvPacket.ReadByteSeq(request.guid[1]);
    recvPacket.ReadByteSeq(request.guid[0]);
    recvPacket.ReadByteSeq(request.guid[4]);
    recvPacket.ReadByteSeq(request.guid[7]);
    recvPacket.ReadByteSeq(request.guid[5]);
    recvPacket.ReadByteSeq(request.guid[6]);

    return request;
}

PetCancelAuraRequest ReadPetCancelAuraRequest(WorldPacket& recvPacket)
{
    PetCancelAuraRequest request = PetCancelAuraRequest();

    request.spellId = recvPacket.read<uint32>();
    recvPacket.ReadGuidMask(request.guid, 0, 3, 1, 4, 6, 2, 7, 5);
    recvPacket.ReadGuidBytes(request.guid, 0, 5, 3, 4, 7, 2, 6, 1);

    return request;
}

void ReadCancelChannelingRequest(WorldPacket& recvData)
{
    recvData.read_skip<uint32>();                          // spellid, not used
}

TotemDestroyedRequest ReadTotemDestroyedRequest(WorldPacket& recvPacket)
{
    TotemDestroyedRequest request = TotemDestroyedRequest();

    recvPacket >> request.slotId;

    request.guid[4] = recvPacket.ReadBit();
    request.guid[2] = recvPacket.ReadBit();
    request.guid[1] = recvPacket.ReadBit();
    request.guid[3] = recvPacket.ReadBit();
    request.guid[0] = recvPacket.ReadBit();
    request.guid[6] = recvPacket.ReadBit();
    request.guid[7] = recvPacket.ReadBit();
    request.guid[5] = recvPacket.ReadBit();

    recvPacket.ReadByteSeq(request.guid[6]);
    recvPacket.ReadByteSeq(request.guid[2]);
    recvPacket.ReadByteSeq(request.guid[4]);
    recvPacket.ReadByteSeq(request.guid[1]);
    recvPacket.ReadByteSeq(request.guid[5]);
    recvPacket.ReadByteSeq(request.guid[0]);
    recvPacket.ReadByteSeq(request.guid[3]);
    recvPacket.ReadByteSeq(request.guid[7]);

    return request;
}

SpellClickRequest ReadSpellClickRequest(WorldPacket& recvData)
{
    SpellClickRequest request = SpellClickRequest();

    recvData.ReadGuidMask(request.guid, 7, 4, 0, 3, 6, 5);
    request.tryAutoDismount = recvData.ReadBit();
    recvData.ReadGuidMask(request.guid, 1, 2);
    recvData.ReadGuidBytes(request.guid, 6, 1, 5, 4, 7, 2, 3, 0);

    return request;
}

GuidRequest ReadMirrorImageDataRequest(WorldPacket& recvData)
{
    GuidRequest request = GuidRequest();

    recvData.read_skip<uint32>(); // DisplayId

    request.guid[0] = recvData.ReadBit();
    request.guid[2] = recvData.ReadBit();
    request.guid[1] = recvData.ReadBit();
    request.guid[6] = recvData.ReadBit();
    request.guid[5] = recvData.ReadBit();
    request.guid[4] = recvData.ReadBit();
    request.guid[7] = recvData.ReadBit();
    request.guid[3] = recvData.ReadBit();

    recvData.ReadByteSeq(request.guid[6]);
    recvData.ReadByteSeq(request.guid[0]);
    recvData.ReadByteSeq(request.guid[3]);
    recvData.ReadByteSeq(request.guid[5]);
    recvData.ReadByteSeq(request.guid[4]);
    recvData.ReadByteSeq(request.guid[2]);
    recvData.ReadByteSeq(request.guid[1]);
    recvData.ReadByteSeq(request.guid[7]);

    return request;
}

ProjectilePositionRequest ReadUpdateProjectilePositionRequest(WorldPacket& recvPacket)
{
    ProjectilePositionRequest request = ProjectilePositionRequest();

    recvPacket >> request.casterGuid;
    recvPacket >> request.spellId;
    recvPacket >> request.castCount;
    recvPacket >> request.x;
    recvPacket >> request.y;
    recvPacket >> request.z;

    return request;
}

bool CanProcessUseItemRequest(Player* player, Unit* mover)
{
    return mover == player;
}

bool ValidateUseItemSelection(Player* player, UseItemRequest& request, Item*& item)
{
    item = NULL;

    if (request.glyphIndex >= MAX_GLYPH_SLOT_INDEX)
    {
        player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return false;
    }

    item = player->GetUseableItemByPos(request.bagIndex, request.slot);
    if (!item)
    {
        player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return false;
    }

    if (item->GetGUID() != request.itemGuid)
    {
        player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return false;
    }

    return true;
}

bool ValidateUseItemCombat(Player* player, Item* item, ItemTemplate const* proto)
{
    if (!player->IsInCombat())
        return true;

    for (int i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
    {
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(proto->Spells[i].SpellId))
        {
            if (!spellInfo->CanBeUsedInCombat())
            {
                player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, item, NULL);
                return false;
            }
        }
    }

    return true;
}

bool ValidateUseItemTemplate(Player* player, Item* item, ItemTemplate const*& proto)
{
    proto = item->GetTemplate();
    if (!proto)
    {
        player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, item, NULL);
        return false;
    }

    // some item classes can be used only in equipped state
    if (proto->InventoryType != INVTYPE_NON_EQUIP && !item->IsEquipped())
    {
        player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, item, NULL);
        return false;
    }

    InventoryResult msg = player->CanUseItem(item);
    if (msg != EQUIP_ERR_OK)
    {
        player->SendEquipError(msg, item, NULL);
        return false;
    }

    // only allow conjured consumable, bandage, poisons (all should have the 2^21 item flag set in DB)
    if (proto->Class == ITEM_CLASS_CONSUMABLE && !(proto->Flags & ITEM_PROTO_FLAG_USEABLE_IN_ARENA) && player->InArena())
    {
        player->SendEquipError(EQUIP_ERR_NOT_DURING_ARENA_MATCH, item, NULL);
        return false;
    }

    // don't allow items banned in arena
    if (proto->Flags & ITEM_PROTO_FLAG_NOT_USEABLE_IN_ARENA && player->InArena())
    {
        player->SendEquipError(EQUIP_ERR_NOT_DURING_ARENA_MATCH, item, NULL);
        return false;
    }

    return ValidateUseItemCombat(player, item, proto);
}

bool CanProcessCastSpellRequest(WorldPacket& recvPacket, Player* player, Unit* mover)
{
    if (mover != player && mover->GetTypeId() == TypeID::TYPEID_PLAYER)
    {
        recvPacket.rfinish(); // prevent spam at ignore packet
        return false;
    }

    return true;
}

SpellInfo const* ValidateCastSpellInfo(WorldPacket& recvPacket, uint32 spellId)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        SF_LOG_ERROR("network", "WORLD: unknown spell id %u", spellId);
        recvPacket.rfinish(); // prevent spam at ignore packet
        return NULL;
    }

    if (spellInfo->IsPassive())
    {
        recvPacket.rfinish(); // prevent spam at ignore packet
        return NULL;
    }

    return spellInfo;
}

bool ValidateCastSource(WorldPacket& recvPacket, Player* player, Unit*& caster, SpellInfo const* spellInfo, uint32 spellId)
{
    if (caster->GetTypeId() == TypeID::TYPEID_UNIT && !caster->ToCreature()->HasSpell(spellId))
    {
        // If the vehicle creature does not have the spell but it allows the passenger to cast own spells
        // change caster to player and let him cast
        if (!player->IsOnVehicle(caster) || spellInfo->CheckVehicle(player) != SpellCastResult::SPELL_CAST_OK)
        {
            recvPacket.rfinish(); // prevent spam at ignore packet
            return false;
        }

        caster = player;
    }

    if (caster->GetTypeId() == TypeID::TYPEID_PLAYER && !caster->ToPlayer()->HasActiveSpell(spellId))
    {
        // Archaeology solves are cast from the journal UI, not the spellbook.
        Player* casterPlayer = caster->ToPlayer();
        if (!spellInfo->ResearchProject || !casterPlayer->HasResearchingProject(spellInfo->ResearchProject))
        {
            recvPacket.rfinish(); // prevent spam at ignore packet
            return false;
        }
    }

    return true;
}

bool ValidateCastExecution(WorldPacket& recvPacket, Player* player, Unit* caster, SpellInfo const* spellInfo)
{
    // Client is resending autoshot cast opcode when other spell is casted during shoot rotation.
    // Skip it to prevent "interrupt" message.
    if (spellInfo->IsAutoRepeatRangedSpell() && caster->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL)
        && caster->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL)->m_spellInfo == spellInfo)
    {
        recvPacket.rfinish();
        return false;
    }

    // can't use our own spells when we're in possession of another unit,
    if (player->isPossessing())
    {
        recvPacket.rfinish(); // prevent spam at ignore packet
        return false;
    }

    return true;
}

SpellInfo const* ValidateCancelAuraSpell(CancelAuraRequest const& request)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(request.spellId);
    if (!spellInfo)
        return NULL;

    // not allow remove spells with attr SPELL_ATTR0_CANT_CANCEL
    if (spellInfo->Attributes & SPELL_ATTR0_CANT_CANCEL)
        return NULL;

    return spellInfo;
}

bool TryInterruptMatchingChannel(Player* player, SpellInfo const* spellInfo, uint32 spellId)
{
    if (!spellInfo->IsChanneled())
        return false;

    if (Spell* curSpell = player->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
        if (curSpell->m_spellInfo->Id == spellId)
            player->InterruptSpell(CURRENT_CHANNELED_SPELL);

    return true;
}

bool IsPlayerCancelableAura(SpellInfo const* spellInfo)
{
    // don't allow remove non positive spells
    // don't allow cancelling passive auras (some of them are visible)
    return spellInfo->IsPositive() && !spellInfo->IsPassive();
}

Creature* ValidatePetCancelAuraRequest(Player* player, PetCancelAuraRequest& request)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(request.spellId);
    if (!spellInfo)
    {
        SF_LOG_ERROR("network", "WORLD: unknown PET spell id %u", request.spellId);
        return NULL;
    }

    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*player, request.guid);

    if (!pet)
    {
        SF_LOG_ERROR("network", "HandlePetCancelAura: Attempt to cancel an aura for non-existant pet %u by player '%s'", uint32(GUID_LOPART(request.guid)), player->GetName().c_str());
        return NULL;
    }

    if (pet != player->GetGuardianPet() && pet != player->GetCharm())
    {
        SF_LOG_ERROR("network", "HandlePetCancelAura: Pet %u is not a pet of player '%s'", uint32(GUID_LOPART(request.guid)), player->GetName().c_str());
        return NULL;
    }

    if (!pet->IsAlive())
    {
        pet->SendPetActionFeedback(FEEDBACK_PET_DEAD);
        return NULL;
    }

    return pet;
}
}

void WorldSession::HandleClientCastFlags(WorldPacket& recvPacket, uint8 castFlags, SpellCastTargets& targets)
{
    // some spell cast packet including more data (for projectiles?)
    if (castFlags & 0x02)
    {
        // not sure about these two
        float elevation, speed;
        recvPacket >> elevation;
        recvPacket >> speed;

        targets.SetElevation(elevation);
        targets.SetSpeed(speed);

        uint8 hasMovementData;
        recvPacket >> hasMovementData;
        if (hasMovementData)
            HandleMovementOpcodes(recvPacket);
    }
    else if (castFlags & 0x8)   // Archaeology
    {
        uint32 count, entry, usedCount;
        uint8 type;
        recvPacket >> count;
        for (uint32 i = 0; i < count; ++i)
        {
            recvPacket >> type;
            switch (type)
            {
                case 2: // Keystones
                    recvPacket >> entry;        // Item id
                    recvPacket >> usedCount;    // Item count
                    break;
                case 1: // Fragments
                    recvPacket >> entry;        // Currency id
                    recvPacket >> usedCount;    // Currency count
                    break;
            }
        }
    }
}

void WorldSession::HandleUseItemOpcode(WorldPacket& recvPacket)
{
    /// @todo add targets.read() check
    Player* pUser = _player;
    Unit* mover = _player->m_mover;

    // ignore for remote control state
    if (!CanProcessUseItemRequest(pUser, mover))
        return;

    UseItemRequest request = ReadUseItemRequest(recvPacket, mover);
    Unit* caster = mover;

    Item* pItem = NULL;
    if (!ValidateUseItemSelection(pUser, request, pItem))
        return;

    SF_LOG_DEBUG("network", "WORLD: CMSG_USE_ITEM packet, bagIndex: %u, slot: %u, castCount: %u, spellId: %u, Item: %u, glyphIndex: %u, data length = %i", request.bagIndex, request.slot, request.castCount, request.spellId, pItem->GetEntry(), request.glyphIndex, (uint32)recvPacket.size());

    ItemTemplate const* proto = NULL;
    if (!ValidateUseItemTemplate(pUser, pItem, proto))
        return;

    // check also  BIND_WHEN_PICKED_UP and BIND_QUEST_ITEM for .additem or .additemset case by GM (not binded at adding to inventory)
    if (proto->Bonding == BIND_WHEN_USE || proto->Bonding == BIND_WHEN_PICKED_UP || proto->Bonding == BIND_QUEST_ITEM)
    {
        if (!pItem->IsSoulBound())
        {
            pItem->SetState(ITEM_CHANGED, pUser);
            pItem->SetBinding(true);
        }
    }

    SpellCastTargets targets(caster, request.targetMask, request.targetGuid, request.itemTargetGuid, request.srcTransportGuid, request.destTransportGuid, request.srcPos, request.destPos, request.elevation, request.missileSpeed, request.targetString);

    // Note: If script stop casting it must send appropriate data to client to prevent stuck item in gray state.
    if (!sScriptMgr->OnItemUse(pUser, pItem, targets))
    {
        // no script or script not process request by self
        pUser->CastItemUseSpell(pItem, targets, request.castCount, request.glyphIndex,
            request.hasResearchData ? &request.researchData : NULL);
    }
}

void WorldSession::HandleOpenItemOpcode(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_OPEN_ITEM packet, data length = %i", (uint32)recvPacket.size());

    Player* pUser = _player;

    // ignore for remote control state
    if (pUser->m_mover != pUser)
        return;

    OpenItemRequest request = ReadOpenItemRequest(recvPacket);

    SF_LOG_INFO("network", "bagIndex: %u, slot: %u", request.bagIndex, request.slot);

    Item* item = pUser->GetItemByPos(request.bagIndex, request.slot);
    if (!item)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    ItemTemplate const* proto = item->GetTemplate();
    if (!proto)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, item, NULL);
        return;
    }

    // Verify that the bag is an actual bag or wrapped item that can be used "normally"
    if (!(proto->Flags & ITEM_PROTO_FLAG_OPENABLE) && !item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
    {
        pUser->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, NULL);
        SF_LOG_ERROR("network", "Possible hacking attempt: Player %s [guid: %u] tried to open item [guid: %u, entry: %u] which is not openable!",
            pUser->GetName().c_str(), pUser->GetGUIDLow(), item->GetGUIDLow(), proto->ItemId);
        return;
    }

    // locked item
    uint32 lockId = proto->LockID;
    if (lockId)
    {
        LockEntry const* lockInfo = sLockStore.LookupEntry(lockId);

        if (!lockInfo)
        {
            pUser->SendEquipError(EQUIP_ERR_ITEM_LOCKED, item, NULL);
            SF_LOG_ERROR("network", "WORLD::OpenItem: item [guid = %u] has an unknown lockId: %u!", item->GetGUIDLow(), lockId);
            return;
        }

        // was not unlocked yet
        if (item->IsLocked())
        {
            pUser->SendEquipError(EQUIP_ERR_ITEM_LOCKED, item, NULL);
            return;
        }
    }

    if (item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))// wrapped?
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GIFT_BY_ITEM);

        stmt->setUInt32(0, item->GetGUIDLow());

        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (result)
        {
            Field* fields = result->Fetch();
            uint32 entry = fields[0].GetUInt32();
            uint32 flags = fields[1].GetUInt32();

            item->SetUInt64Value(ITEM_FIELD_GIFT_CREATOR, 0);
            item->SetEntry(entry);
            item->SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS, flags);
            item->SetState(ITEM_CHANGED, pUser);
        }
        else
        {
            SF_LOG_ERROR("network", "Wrapped item %u don't have record in character_gifts table and will deleted", item->GetGUIDLow());
            pUser->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
            return;
        }

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GIFT);

        stmt->setUInt32(0, item->GetGUIDLow());

        CharacterDatabase.Execute(stmt);
    }
    else
        pUser->SendLoot(item->GetGUID(), LootType::LOOT_CORPSE);
}

void WorldSession::HandleGameObjectUseOpcode(WorldPacket& recvData)
{
    GuidRequest request = ReadGameObjectUseRequest(recvData);
    ObjectGuid guid = request.guid;

    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_GAME_OBJ_REPORT_USE Message [guid=%u]", GUID_LOPART(guid));

    if (GameObject* obj = GetPlayer()->GetMap()->GetGameObject(guid))
    {
        // ignore for remote control state
        if (_player->m_mover != _player)
            if (!(_player->IsOnVehicle(_player->m_mover) || _player->IsMounted()) && !obj->GetGOInfo()->IsUsableMounted())
                return;

        obj->Use(_player);
    }
}

void WorldSession::HandleGameobjectReportUse(WorldPacket& recvPacket)
{
    GuidRequest request = ReadGameObjectReportUseRequest(recvPacket);
    ObjectGuid guid = request.guid;

    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_GAME_OBJ_USE Message [in game guid: %u]", GUID_LOPART(guid));

    // ignore for remote control state
    if (_player->m_mover != _player)
        return;

    GameObject* go = GetPlayer()->GetMap()->GetGameObject(guid);
    if (!go)
        return;

    // we cannot use go with not selectable flags
    if (go->HasFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE))
        return;

    if (!go->IsWithinDistInMap(_player, INTERACTION_DISTANCE))
        return;

    if (go->AI()->GossipHello(_player))
        return;

    _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT, go->GetEntry());
}

void WorldSession::HandleCastSpellOpcode(WorldPacket& recvPacket)
{
    // ignore for remote control state (for player case)
    Unit* mover = _player->m_mover;
    if (!CanProcessCastSpellRequest(recvPacket, _player, mover))
        return;

    CastSpellRequest request = ReadCastSpellRequest(recvPacket, mover);
    Unit* caster = mover;
    uint8 castCount = request.castCount;
    uint8 castFlags = request.castFlags;
    uint32 spellId = request.spellId;
    uint32 glyphIndex = request.glyphIndex;

    SF_LOG_DEBUG("network", "WORLD: got cast spell packet, castCount: %u, spellId: %u, castFlags: %u, data length = %u", castCount, spellId, castFlags, (uint32)recvPacket.size());

    SpellInfo const* spellInfo = ValidateCastSpellInfo(recvPacket, spellId);
    if (!spellInfo)
        return;

    if (!ValidateCastSource(recvPacket, _player, caster, spellInfo, spellId))
        return;

    // Aura Overriden Spells
    Unit::AuraEffectList swaps = mover->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
    Unit::AuraEffectList const& swaps2 = mover->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2);
    if (!swaps2.empty())
        swaps.insert(swaps.end(), swaps2.begin(), swaps2.end());

    if (!swaps.empty())
    {
        for (Unit::AuraEffectList::const_iterator itr = swaps.begin(); itr != swaps.end(); ++itr)
        {
            // Prefer MiscValue as the overridden spell id when set (modern cores);
            // otherwise match by SpellClassMask via IsAffectingSpell.
            bool matches = (*itr)->GetMiscValue()
                ? uint32((*itr)->GetMiscValue()) == spellInfo->Id
                : (*itr)->IsAffectingSpell(spellInfo);
            if (!matches)
                continue;

            if (SpellInfo const* newInfo = sSpellMgr->GetSpellInfo((*itr)->GetAmount()))
            {
                if (caster->ToPlayer()->getLevel() < spellInfo->SpellLevel ||
                    caster->ToPlayer()->getLevel() < spellInfo->BaseLevel)
                    continue;

                if (caster->ToPlayer()->getLevel() < newInfo->SpellLevel ||
                    caster->ToPlayer()->getLevel() < newInfo->BaseLevel)
                    continue;

                spellInfo = newInfo;
                spellId = newInfo->Id;
            }
            break;
        }
    }

    // Specialization Overriden Spells
    uint32 SpecializationID = caster->ToPlayer()->GetTalentSpecialization(caster->ToPlayer()->GetActiveSpec());
    if (SpecializationID)
    {
        for (uint32 i = 0; i < sSpecializationSpellsStore.GetNumRows(); i++)
        {
            SpecializationSpellsEntry const* specializationInfo = sSpecializationSpellsStore.LookupEntry(i);
            if (!specializationInfo)
                continue;

            if (specializationInfo->SpecializationId != SpecializationID)
                continue;

            if (specializationInfo->RemovesSpellId == spellId)
            {
                if (SpellInfo const* newInfo = sSpellMgr->GetSpellInfo(specializationInfo->SpellId))
                {
                    if (caster->ToPlayer()->getLevel() < spellInfo->SpellLevel ||
                        caster->ToPlayer()->getLevel() < spellInfo->BaseLevel)
                        continue;

                    if (caster->ToPlayer()->getLevel() < newInfo->SpellLevel ||
                        caster->ToPlayer()->getLevel() < newInfo->BaseLevel)
                        continue;

                    spellInfo = newInfo;
                    spellId = newInfo->Id;
                }
                break;
            }
        }
    }

    // Talent Overriden Spells
    uint32 ClassID = caster->ToPlayer()->getClass();
    if (ClassID)
    {
        for (uint32 i = 0; i < sTalentStore.GetNumRows(); i++)
        {
            TalentEntry const* talentInfo = sTalentStore.LookupEntry(i);
            if (!talentInfo)
                continue;

            if (talentInfo->playerClass != ClassID)
                continue;

            if (!caster->ToPlayer()->HasActiveSpell(talentInfo->SpellId))
                continue;

            if (talentInfo->OverrideSpellID == spellId)
            {
                // Talent.SpellId may be a passive OVERRIDE_ACTIONBAR aura (e.g. Faerie Swarm
                // 106707 -> 102355). Prefer the action-bar replacement amount when present.
                uint32 replacementId = talentInfo->SpellId;
                if (SpellInfo const* talentSpell = sSpellMgr->GetSpellInfo(talentInfo->SpellId))
                {
                    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    {
                        if (talentSpell->Effects[i].IsAura(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS) ||
                            talentSpell->Effects[i].IsAura(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2))
                        {
                            replacementId = uint32(talentSpell->Effects[i].BasePoints);
                            break;
                        }
                    }
                }

                if (SpellInfo const* newInfo = sSpellMgr->GetSpellInfo(replacementId))
                {
                    if (caster->ToPlayer()->getLevel() < spellInfo->SpellLevel ||
                        caster->ToPlayer()->getLevel() < spellInfo->BaseLevel)
                        continue;

                    if (caster->ToPlayer()->getLevel() < newInfo->SpellLevel ||
                        caster->ToPlayer()->getLevel() < newInfo->BaseLevel)
                        continue;

                    spellInfo = newInfo;
                    spellId = newInfo->Id;
                }
                break;
            }
        }
    }

    if (!ValidateCastExecution(recvPacket, _player, caster, spellInfo))
        return;

    // client provided targets
    SpellCastTargets targets(caster, request.targetMask, request.targetGuid, request.itemTargetGuid, request.srcTransportGuid, request.destTransportGuid, request.srcPos, request.destPos, request.elevation, request.missileSpeed, request.targetString);

    // auto-selection buff level base at target level (in spellInfo)
    if (targets.GetUnitTarget())
    {
        SpellInfo const* actualSpellInfo = spellInfo->GetAuraRankForLevel(targets.GetUnitTarget()->getLevel());

        // if rank not found then function return NULL but in explicit cast case original spell can be casted and later failed with appropriate error message
        if (actualSpellInfo)
            spellInfo = actualSpellInfo;
    }

    Spell* spell = new Spell(caster, spellInfo, TRIGGERED_NONE, 0, false);
    spell->m_cast_count = castCount;                       // set count of casts
    spell->m_glyphIndex = glyphIndex;

    if (request.hasResearchData)
    {
        SpellResearchData* researchData = new SpellResearchData();
        *researchData = request.researchData;
        spell->m_researchData = researchData;
    }

    spell->prepare(&targets);
}

void WorldSession::HandleCancelCastOpcode(WorldPacket& recvPacket)
{
    CancelCastRequest request = ReadCancelCastRequest(recvPacket);

    if (_player->IsNonMeleeSpellCasted(false))
        _player->InterruptNonMeleeSpells(false, request.spellId, false);
}

void WorldSession::HandleCancelAuraOpcode(WorldPacket& recvPacket)
{
    CancelAuraRequest request = ReadCancelAuraRequest(recvPacket);

    SpellInfo const* spellInfo = ValidateCancelAuraSpell(request);
    if (!spellInfo)
        return;

    // channeled spell case (it currently casted then)
    if (TryInterruptMatchingChannel(_player, spellInfo, request.spellId))
        return;

    if (!IsPlayerCancelableAura(spellInfo))
        return;

    // maybe should only remove one buff when there are multiple?
    _player->RemoveOwnedAura(request.spellId, 0, 0, AURA_REMOVE_BY_CANCEL);
}

void WorldSession::HandlePetCancelAuraOpcode(WorldPacket& recvPacket)
{
    PetCancelAuraRequest request = ReadPetCancelAuraRequest(recvPacket);

    Creature* pet = ValidatePetCancelAuraRequest(_player, request);
    if (!pet)
        return;

    pet->RemoveOwnedAura(request.spellId, 0, 0, AURA_REMOVE_BY_CANCEL);

    pet->AddCreatureSpellCooldown(request.spellId);
}

void WorldSession::HandleCancelGrowthAuraOpcode(WorldPacket& /*recvPacket*/) { }

void WorldSession::HandleCancelAutoRepeatSpellOpcode(WorldPacket& /*recvPacket*/)
{
    // may be better send SMSG_CANCEL_AUTO_REPEAT?
    // cancel and prepare for deleting
    _player->InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
}

void WorldSession::HandleCancelChanneling(WorldPacket& recvData)
{
    ReadCancelChannelingRequest(recvData);

    // ignore for remote control state (for player case)
    Unit* mover = _player->m_mover;
    if (mover != _player && mover->GetTypeId() == TypeID::TYPEID_PLAYER)
        return;

    mover->InterruptSpell(CURRENT_CHANNELED_SPELL);
}

void WorldSession::HandleTotemDestroyed(WorldPacket& recvPacket)
{
    // ignore for remote control state
    if (_player->m_mover != _player)
        return;

    TotemDestroyedRequest request = ReadTotemDestroyedRequest(recvPacket);

    ++request.slotId;
    if (request.slotId >= MAX_TOTEM_SLOT)
        return;

    if (!_player->m_SummonSlot[request.slotId])
        return;

    Creature* totem = GetPlayer()->GetMap()->GetCreature(_player->m_SummonSlot[request.slotId]);
    if (totem && totem->IsTotem() && totem->GetGUID() == request.guid)
        totem->ToTotem()->UnSummon();
}

void WorldSession::HandleSelfResOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_SELF_RES");                  // empty opcode

    if (_player->HasAuraType(SPELL_AURA_PREVENT_RESURRECTION))
        return; // silent return, client should display error by itself and not send this opcode

    if (_player->GetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL))
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(_player->GetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL));
        if (spellInfo)
            _player->CastSpell(_player, spellInfo, false, 0);

        _player->SetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL, 0);
    }
}

void WorldSession::HandleSpellClick(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_SPELLCLICK");

    SpellClickRequest request = ReadSpellClickRequest(recvData);

    // this will get something not in world. crash
    Creature* unit = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, request.guid);

    if (!unit)
        return;

    /// @todo Unit::SetCharmedBy: 28782 is not in world but 0 is trying to charm it! -> crash
    if (!unit->IsInWorld())
        return;

    if (_player->IsMounted() && request.tryAutoDismount)
        _player->Dismount();

    unit->HandleSpellClick(_player);
}

void WorldSession::HandleMirrorImageDataRequest(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_GET_MIRROR_IMAGE_DATA");

    GuidRequest request = ReadMirrorImageDataRequest(recvData);
    ObjectGuid guid = request.guid;

    // Get unit for which data is needed by client
    Unit* unit = ObjectAccessor::GetObjectInWorld(guid, (Unit*)NULL);
    if (!unit)
        return;

    if (!unit->HasAuraType(SPELL_AURA_CLONE_CASTER))
        return;

    // Get creator of the unit (SPELL_AURA_CLONE_CASTER does not stack)
    Unit* creator = unit->GetAuraEffectsByType(SPELL_AURA_CLONE_CASTER).front()->GetCaster();
    if (!creator)
        return;

    if (Player* player = creator->ToPlayer())
    {
        WorldPacket data(SMSG_MIRROR_IMAGE_COMPONENTED_DATA, 8 + 4 + 8 * 1 + 8 + 11 * 4);
        Guild* guild = player->GetGuild();
        ObjectGuid guildGuid = guild ? guild->GetGUID() : 0;

        data.WriteBit(guid[4]);
        data.WriteBit(guildGuid[3]);
        data.WriteBit(guildGuid[6]);
        data.WriteBit(guid[0]);
        data.WriteBit(guildGuid[7]);
        data.WriteBit(guid[1]);
        data.WriteBit(guid[5]);
        data.WriteBit(guildGuid[2]);
        data.WriteBit(guildGuid[1]);
        data.WriteBit(guid[7]);
        data.WriteBit(guildGuid[4]);
        data.WriteBit(guildGuid[0]);
        data.WriteBit(guid[2]);
        data.WriteBit(guildGuid[5]);
        data.WriteBit(guid[3]);
        data.WriteBits(11, 22); // item slots count
        data.WriteBit(guid[6]);
        data.FlushBits();

        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 3)); // haircolor
        data << uint32(creator->GetDisplayId());
        data << uint8(player->GetByteValue(PLAYER_FIELD_REST_STATE, 0));    // facial hair

        data.WriteByteSeq(guildGuid[6]);
        data.WriteByteSeq(guildGuid[4]);
        data.WriteByteSeq(guid[7]);
        data.WriteByteSeq(guildGuid[1]);
        data.WriteByteSeq(guid[3]);
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 2)); // hair 
        data.WriteByteSeq(guid[2]);
        data.WriteByteSeq(guid[0]);
        data << uint8(creator->getRace());
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 0)); // skin
        data.WriteByteSeq(guildGuid[7]);

        static EquipmentSlots const itemSlots[] =
        {
            EQUIPMENT_SLOT_HEAD,
            EQUIPMENT_SLOT_SHOULDERS,
            EQUIPMENT_SLOT_BODY,
            EQUIPMENT_SLOT_CHEST,
            EQUIPMENT_SLOT_WAIST,
            EQUIPMENT_SLOT_LEGS,
            EQUIPMENT_SLOT_FEET,
            EQUIPMENT_SLOT_WRISTS,
            EQUIPMENT_SLOT_HANDS,
            EQUIPMENT_SLOT_TABARD,
            EQUIPMENT_SLOT_BACK,
            EQUIPMENT_SLOT_END
        };

        // Display items in visible slots
        for (EquipmentSlots const* itr = &itemSlots[0]; *itr != EQUIPMENT_SLOT_END; ++itr)
        {
            if (*itr == EQUIPMENT_SLOT_HEAD && player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM))
                data << uint32(0);
            else if (*itr == EQUIPMENT_SLOT_BACK && player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK))
                data << uint32(0);
            else if (Item const* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, *itr))
                data << uint32(item->GetTemplate()->DisplayInfoID);
            else
                data << uint32(0);
        }

        data.WriteByteSeq(guid[4]);
        data << uint8(creator->getClass());
        data << uint8(creator->getGender());
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 1)); // face
        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guildGuid[3]);
        data.WriteByteSeq(guildGuid[2]);
        data.WriteByteSeq(guid[1]);
        data.WriteByteSeq(guildGuid[0]);
        data.WriteByteSeq(guildGuid[5]);
        data.WriteByteSeq(guid[6]);

        SendPacket(&data);
    }
    else
    {
        WorldPacket data(SMSG_MIRROR_IMAGE_CREATURE_DATA, 8 + 4);
        data.WriteGuidMask(guid, 0, 1, 3, 5, 7, 6, 4, 2);

        data.WriteGuidBytes(guid, 0, 3, 6, 5, 7);
        data << uint32(creator->GetDisplayId());
        data.WriteGuidBytes(guid, 4, 2, 1);

        SendPacket(&data);
    }
}

void WorldSession::HandleUpdateProjectilePosition(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_UPDATE_PROJECTILE_POSITION");

    ProjectilePositionRequest request = ReadUpdateProjectilePositionRequest(recvPacket);

    Unit* caster = ObjectAccessor::GetUnit(*_player, request.casterGuid);
    if (!caster)
        return;

    Spell* spell = caster->FindCurrentSpellBySpellId(request.spellId);
    if (!spell || !spell->m_targets.HasDst())
        return;

    Position pos = *spell->m_targets.GetDstPos();
    pos.Relocate(request.x, request.y, request.z);
    spell->m_targets.ModDst(pos);

    WorldPacket data(SMSG_SET_PROJECTILE_POSITION, 21);
    data << uint64(request.casterGuid);
    data << uint8(request.castCount);
    data << float(request.x);
    data << float(request.y);
    data << float(request.z);
    caster->SendMessageToSet(&data, true);
}

void WorldSession::HandleRequestCategoryCooldowns(WorldPacket& /*recvPacket*/)
{
    std::map<uint32, int32> categoryMods;
    Unit::AuraEffectList const& categoryCooldownAuras = _player->GetAuraEffectsByType(SPELL_AURA_MOD_SPELL_CATEGORY_COOLDOWN);
    for (Unit::AuraEffectList::const_iterator itr = categoryCooldownAuras.begin(); itr != categoryCooldownAuras.end(); ++itr)
    {
        std::map<uint32, int32>::iterator cItr = categoryMods.find((*itr)->GetMiscValue());
        if (cItr == categoryMods.end())
            categoryMods[(*itr)->GetMiscValue()] = (*itr)->GetAmount();
        else
            cItr->second += (*itr)->GetAmount();
    }

    WorldPacket data(SMSG_CATEGORY_COOLDOWN, 11);
    data.WriteBits(categoryMods.size(), 21);
    data.FlushBits();

    for (std::map<uint32, int32>::const_iterator itr = categoryMods.begin(); itr != categoryMods.end(); ++itr)
    {
        data << int32(-itr->second);
        data << uint32(itr->first);
    }

    SendPacket(&data);
}

void WorldSession::SendTotemCreated(ObjectGuid TotemGUID, uint32 Duration, uint32 SpellID, uint8 Slot)
{
    WorldPacket data(SMSG_TOTEM_CREATED, 17);
    data.WriteGuidMask(TotemGUID, 6, 1, 2, 5, 3, 4, 7, 0);
    data << uint32(Duration);
    data << uint32(SpellID);
    data.WriteGuidBytes(TotemGUID, 3, 4, 5, 6, 0, 2);
    data << uint8(Slot);
    data.WriteGuidBytes(TotemGUID, 1, 7);
    _player->SendDirectMessage(&data);
}

void WorldSession::SendModifyCooldown(ObjectGuid UnitGUID, int32 DeltaTime, int32 SpellID)
{
    WorldPacket data(SMSG_MODIFY_COOLDOWN, 4 + 8 + 4);
    data.WriteGuidMask(UnitGUID, 2, 1, 0, 4, 7, 3, 6, 5);
    data.WriteGuidBytes(UnitGUID, 4, 1);
    data << int32(DeltaTime);        // Cooldown mod in milliseconds
    data.WriteGuidBytes(UnitGUID, 3, 6, 7, 5, 0);
    data << uint32(SpellID);         // Spell ID
    data.WriteGuidBytes(UnitGUID, 2);
    SendPacket(&data);
}
