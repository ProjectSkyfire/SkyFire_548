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

void WorldSession::HandleConfirmRespecWipe(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "CMSG_CONFIRM_RESPEC_WIPE");

    ObjectGuid respecMasterGUID;
    RespecType respecType;

    respecType = (RespecType)recvData.read<uint8>();
    respecMasterGUID[7] = recvData.ReadBit();  // 23
    respecMasterGUID[2] = recvData.ReadBit();  // 18
    respecMasterGUID[6] = recvData.ReadBit();  // 22
    respecMasterGUID[1] = recvData.ReadBit();  // 17
    respecMasterGUID[4] = recvData.ReadBit();  // 20
    respecMasterGUID[0] = recvData.ReadBit();  // 16
    respecMasterGUID[3] = recvData.ReadBit();  // 19
    respecMasterGUID[5] = recvData.ReadBit();  // 21

    recvData.ReadByteSeq(respecMasterGUID[2]);  // 18
    recvData.ReadByteSeq(respecMasterGUID[4]);  // 20
    recvData.ReadByteSeq(respecMasterGUID[1]);  // 17
    recvData.ReadByteSeq(respecMasterGUID[3]);  // 19
    recvData.ReadByteSeq(respecMasterGUID[0]);  // 16
    recvData.ReadByteSeq(respecMasterGUID[5]);  // 21
    recvData.ReadByteSeq(respecMasterGUID[7]);  // 23
    recvData.ReadByteSeq(respecMasterGUID[6]);  // 22

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(respecMasterGUID, UNIT_NPC_FLAG_TRAINER);
    if (!unit)
    {
        TC_LOG_DEBUG("network", "WORLD: HandleTalentWipeConfirmOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(respecMasterGUID)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (respecType == RESPEC_TYPE_TALENT)
    {
        if (!_player->ResetTalents(false, true))
        {
            WorldPacket data(SMSG_RESPEC_WIPE_CONFIRM, 8+4);    //you have not any talent
            data << uint8(0);
            data << uint8(0);
            data << uint32(0);
            SendPacket(&data);
            return;
        }
    }
    else
    {
        _player->ResetTalents(false, false, true);
    }

    _player->SendTalentsInfoData();
    unit->CastSpell(_player, 14867, true);                  //spell: "Untalent Visual Effect"
}

void WorldSession::HandleUnlearnSkillOpcode(WorldPacket& recvData)
{
    uint32 skillId;
    recvData >> skillId;

    SkillRaceClassInfoEntry const* rcEntry = GetSkillRaceClassInfo(skillId, GetPlayer()->getRace(), GetPlayer()->getClass());
    if (!rcEntry || !(rcEntry->Flags & SKILL_FLAG_UNLEARNABLE))
        return;

    GetPlayer()->SetSkill(skillId, 0, 0, 0);
}
