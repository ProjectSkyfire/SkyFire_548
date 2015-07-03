/*
 * Copyright (C) 2011-2015 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2015 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "Player.h"
#include "Pet.h"
#include "UpdateMask.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandeSetTalentSpecialization(WorldPacket& recvData)
{
    uint32 specializationTabId;
    recvData >> specializationTabId;

    if (specializationTabId > MAX_TALENT_TABS)
        return;

    if (_player->GetTalentSpecialization(_player->GetActiveSpec()))
        return;

    uint32 specializationId = GetClassSpecializations(_player->getClass())[specializationTabId];

    _player->SetTalentSpecialization(_player->GetActiveSpec(), specializationId);
    _player->SetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID, specializationId);
    _player->SendTalentsInfoData();

    std::list<uint32> learnList = GetSpellsForLevels(0, _player->getRaceMask(), _player->GetTalentSpecialization(_player->GetActiveSpec()), 0, _player->getLevel());
    for (std::list<uint32>::const_iterator iter = learnList.begin(); iter != learnList.end(); iter++)
    {
        if (!_player->HasSpell(*iter))
            _player->learnSpell(*iter, true);
    }

    _player->SaveToDB();
}

void WorldSession::HandleLearnTalentOpcode(WorldPacket& recvData)
{
    uint32 talentCount = recvData.ReadBits(23);
    uint16 talentId;
    bool anythingLearned = false;

    for (int i = 0; i != talentCount; i++)
    {
        recvData >> talentId;
        if (_player->LearnTalent(talentId))
            anythingLearned = true;
    }

    if (anythingLearned)
        _player->SendTalentsInfoData();
}

void WorldSession::HandleLearnPreviewTalents(WorldPacket& recvPacket)
{
    TC_LOG_DEBUG("network", "CMSG_LEARN_PREVIEW_TALENTS");
}

void WorldSession::HandleRespecWipeConfirmOpcode(WorldPacket& recvPacket)
{
    TC_LOG_DEBUG("network", "CMSG_CONFIRM_RESPEC_WIPE");
    ObjectGuid guid;
    uint8 RespecType = 0;
    uint32 Cost = 0;

    recvPacket >> RespecType;
    guid[2] = recvPacket.ReadBit();
    guid[3] = recvPacket.ReadBit();
    guid[1] = recvPacket.ReadBit();
    guid[0] = recvPacket.ReadBit();
    guid[4] = recvPacket.ReadBit();
    guid[7] = recvPacket.ReadBit();
    guid[6] = recvPacket.ReadBit();
    guid[5] = recvPacket.ReadBit();

    recvPacket.ReadByteSeq(guid[0]);
    recvPacket.ReadByteSeq(guid[4]);
    recvPacket.ReadByteSeq(guid[7]);
    recvPacket.ReadByteSeq(guid[5]);
    recvPacket.ReadByteSeq(guid[1]);
    recvPacket.ReadByteSeq(guid[6]);
    recvPacket.ReadByteSeq(guid[3]);
    recvPacket.ReadByteSeq(guid[2]);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TRAINER);
    if (!unit)
    {
        TC_LOG_DEBUG("network", "WORLD: HandleRespecWipeConfirmOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (!_player->ResetTalents())
    {
        WorldPacket data(SMSG_RESPEC_WIPE_CONFIRM, 8 + 4);    //you have not any talent

        data.WriteBit(guid[5]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[3]);
        data.WriteBit(guid[2]);
        data.WriteBit(guid[1]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[4]);
        data.WriteBit(guid[6]);

        data.WriteByteSeq(guid[1]);
        data.WriteByteSeq(guid[0]);
        data << uint8(RespecType);
        data.WriteByteSeq(guid[7]);
        data.WriteByteSeq(guid[3]);
        data.WriteByteSeq(guid[2]);
        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guid[6]);
        data.WriteByteSeq(guid[4]);
        data << uint32(Cost);

        SendPacket(&data);
        return;
    }

    _player->SendTalentsInfoData();
    unit->CastSpell(_player, 14867, true);                  //spell: "Untalent Visual Effect"
}

void WorldSession::HandleUnlearnSkillOpcode(WorldPacket& recvData)
{
    uint32 skillId;
    recvData >> skillId;

    if (!IsPrimaryProfessionSkill(skillId))
        return;

    GetPlayer()->SetSkill(skillId, 0, 0, 0);
}
