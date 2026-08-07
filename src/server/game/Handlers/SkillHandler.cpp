/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "Pet.h"
#include "Player.h"
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
    _player->UpdateTalentSpecializationManaBonus();
    _player->UpdatePvpPower();
    _player->SendTalentsInfoData();

    // Do not auto-push newly learned specialization spells onto the action bar;
    // dual-spec keeps per-spec bars that the player arranged.
    _player->SetSuppressSpellLearnMessages(true);
    std::list<uint32> learnList = GetSpellsForLevels(0, _player->getRaceMask(), _player->GetTalentSpecialization(_player->GetActiveSpec()), 0, _player->getLevel());
    for (std::list<uint32>::const_iterator iter = learnList.begin(); iter != learnList.end(); ++iter)
    {
        if (!_player->HasSpell(*iter))
            _player->learnSpell(*iter, true);
    }
    _player->SetSuppressSpellLearnMessages(false);

    _player->SaveToDB();
}

void WorldSession::HandleLearnTalentOpcode(WorldPacket& recvData)
{
    uint32 talentCount = recvData.ReadBits(23);
    uint16 talentId;
    bool anythingLearned = false;

    for (uint32 i = 0; i != talentCount; i++)
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
    SF_LOG_DEBUG("network", "CMSG_LEARN_PREVIEW_TALENTS");
}

void WorldSession::HandleRespecWipeConfirmOpcode(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "CMSG_CONFIRM_RESPEC_WIPE");
    ObjectGuid guid;
    uint8 RespecType = 0;
    uint32 Cost = 0;

    recvPacket >> RespecType;
    recvPacket.ReadGuidMask(guid, 7, 2, 6, 1, 4, 0, 3, 5);
    recvPacket.ReadGuidBytes(guid, 2, 4, 1, 3, 0, 5, 7, 6);

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (!RespecType)
    {
        if (!_player->ResetTalents(Cost, true, false))
        {
            GetPlayer()->SendTalentWipeConfirm(guid, false);
            return;
        }
    }
    else
    {
        _player->ResetTalents(Cost, false, true);
    }

    _player->SendTalentsInfoData();

    if (Unit* unit = _player->GetSelectedUnit())
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

void WorldSession::HandleRequestResearchHistory(WorldPacket& /*recvPacket*/)
{
    if (!_player)
        return;

    _player->SendResearchHistory();
}
