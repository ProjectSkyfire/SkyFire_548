/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BlackMarketMgr.h"
#include "Common.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "DatabaseEnv.h"
#include "GossipDef.h"
#include "Language.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Pet.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "UpdateMask.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <vector>

enum StableResultCode
{
    STABLE_ERR_MONEY = 0x01,                         // "you don't have enough money"
    STABLE_ERR_INVALID_SLOT = 0x03,                         // "That slot is locked"
    STABLE_SUCCESS_STABLE = 0x08,                         // stable success
    STABLE_SUCCESS_UNSTABLE = 0x09,                         // unstable/swap success
    STABLE_SUCCESS_BUY_SLOT = 0x0A,                         // buy slot success
    STABLE_ERR_EXOTIC = 0x0B,                         // "you are unable to control exotic creatures"
    STABLE_ERR_STABLE = 0x0C,                         // "Internal pet error"
};

void WorldSession::SendTabardVendorActivate(uint64 guid)
{
    ObjectGuid Guid = guid;
    WorldPacket data(SMSG_TABARD_VENDOR_ACTIVATE, 8);

    data.WriteGuidMask(Guid, 1, 5, 0, 7, 4, 6, 3, 2);
    data.WriteGuidBytes(Guid, 5, 4, 2, 3, 6, 0, 1, 7);

    SendPacket(&data);
}

void WorldSession::HandleBankerActivateOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BANKER_ACTIVATE");

    recvData.ReadGuidMask(guid, 4, 5, 0, 6, 1, 2, 7, 3);
    recvData.ReadGuidBytes(guid, 1, 7, 2, 5, 6, 3, 0, 4);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_BANKER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleBankerActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendShowBank(guid);
}

void WorldSession::SendShowBank(ObjectGuid guid)
{
    WorldPacket data(SMSG_SHOW_BANK, 1 + 8);

    data.WriteGuidMask(guid, 2, 4, 3, 6, 5, 1, 7, 0);
    data.WriteGuidBytes(guid, 7, 0, 5, 3, 6, 1, 4, 2);

    SendPacket(&data);
}

void WorldSession::HandleTrainerListOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    recvData.ReadGuidMask(guid, 0, 2, 7, 6, 1, 4, 5, 3);
    recvData.ReadGuidBytes(guid, 3, 6, 7, 5, 1, 0, 2, 4);

    SendTrainerList(guid);
}

void WorldSession::SendTrainerList(uint64 guid)
{
    std::string str = GetSkyFireString(LANG_NPC_TAINER_HELLO);
    SendTrainerList(guid, str);
}

void WorldSession::SendTrainerList(uint64 guid, const std::string& strTitle)
{
    SF_LOG_DEBUG("network", "WORLD: SendTrainerList");

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TRAINER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: SendTrainerList - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    CreatureTemplate const* ci = unit->GetCreatureTemplate();

    if (!ci)
    {
        SF_LOG_DEBUG("network", "WORLD: SendTrainerList - (GUID: %u) NO CREATUREINFO!", GUID_LOPART(guid));
        return;
    }

    TrainerSpellData const* trainer_spells = unit->GetTrainerSpells();
    if (!trainer_spells)
    {
        SF_LOG_DEBUG("network", "WORLD: SendTrainerList - Training spells not found for creature (GUID: %u Entry: %u)",
            GUID_LOPART(guid), unit->GetEntry());
        return;
    }

    ObjectGuid oGuid = guid;

    WorldPacket data(SMSG_TRAINER_LIST, 1 + 8 + 4 + (trainer_spells->spellList.size() * 30) + 4 + 4 + strTitle.size());
    data.WriteGuidMask(oGuid, 4, 5);

    size_t count_pos = data.bitwpos();
    data.WriteBits(0, 19);                  // placeholder

    data.WriteBits(strTitle.size(), 11);
    data.WriteGuidMask(oGuid, 6, 2, 7, 1, 3, 0);
    data.FlushBits();

    data.WriteGuidBytes(oGuid, 4);

    // reputation discount
    float fDiscountMod = _player->GetReputationPriceDiscount(unit);

    uint32 count = 0;
    for (TrainerSpellMap::const_iterator itr = trainer_spells->spellList.begin(); itr != trainer_spells->spellList.end(); ++itr)
    {
        TrainerSpell const* tSpell = &itr->second;

        bool valid = true;
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (!tSpell->learnedSpell[i])
                continue;
            if (!_player->IsSpellFitByClassAndRace(tSpell->learnedSpell[i]))
            {
                valid = false;
                break;
            }
        }
        if (!valid)
            continue;

        data << uint8(tSpell->reqLevel);
        data << uint32(floor(tSpell->spellCost * fDiscountMod));
        data << uint32(tSpell->spell);      // learned spell (or cast-spell in profession case)

        // spells required (3 max)
        uint8 maxReq = 0;
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (!tSpell->learnedSpell[i])
                continue;
            SpellsRequiringSpellMapBounds spellsRequired = sSpellMgr->GetSpellsRequiredForSpellBounds(tSpell->learnedSpell[i]);
            for (SpellsRequiringSpellMap::const_iterator itr2 = spellsRequired.first; itr2 != spellsRequired.second && maxReq < 3; ++itr2)
            {
                data << uint32(itr2->second);
                ++maxReq;
            }
            if (maxReq == 3)
                break;
        }
        while (maxReq < 3)
        {
            data << uint32(0);
            ++maxReq;
        }

        data << uint32(tSpell->reqSkill);
        data << uint32(tSpell->reqSkillValue);

        TrainerSpellState state = _player->GetTrainerSpellState(tSpell);
        data << uint8(state == TRAINER_SPELL_GREEN_DISABLED ? TRAINER_SPELL_GREEN : state);

        ++count;
    }

    data.WriteString(strTitle);
    data.WriteGuidBytes(oGuid, 6, 7, 1, 3);
    data << uint32(1);                      // different value for each trainer, also found in CMSG_TRAINER_BUY_SPELL
    data.WriteGuidBytes(oGuid, 5, 0, 2);
    data << uint32(trainer_spells->trainerType);

    data.PutBits(count_pos, count, 19);
    SendPacket(&data);
}

void WorldSession::HandleTrainerBuySpellOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint32 spellId;
    uint32 trainerId;

    recvData >> spellId >> trainerId;

    recvData.ReadGuidMask(guid, 1, 4, 0, 6, 3, 2, 5, 7);
    recvData.ReadGuidBytes(guid, 3, 1, 4, 7, 0, 5, 6, 2);

    SF_LOG_DEBUG("network", "WORLD: Received CMSG_TRAINER_BUY_SPELL NpcGUID=%u, learn spell id is: %u", uint32(GUID_LOPART(guid)), spellId);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TRAINER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleTrainerBuySpellOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // check present spell in trainer spell list
    TrainerSpellData const* trainer_spells = unit->GetTrainerSpells();
    if (!trainer_spells)
    {
        SendTrainerBuyFailed(guid, spellId, 0);
        return;
    }

    // not found, cheat?
    TrainerSpell const* trainer_spell = trainer_spells->Find(spellId);
    if (!trainer_spell)
    {
        SendTrainerBuyFailed(guid, spellId, 0);
        return;
    }

    // can't be learn, cheat? Or double learn with lags...
    //if (_player->GetTrainerSpellState(trainer_spell) != TRAINER_SPELL_GREEN)
     //   SendTrainerBuyFailed(guid, spellId, 0);
     //   return;
    //}

    // apply reputation discount
    uint32 nSpellCost = uint32(floor(trainer_spell->spellCost * _player->GetReputationPriceDiscount(unit)));

    // check money requirement
    if (!_player->HasEnoughMoney(uint64(nSpellCost)))
    {
        SendTrainerBuyFailed(guid, spellId, 1);
        return;
    }

    _player->ModifyMoney(-int64(nSpellCost));

    unit->SendPlaySpellVisualKit(179, 0, 0);       // 53 SpellCastDirected
    _player->SendPlaySpellVisualKit(362, 1, 0);    // 113 EmoteSalute

    // learn explicitly or cast explicitly
    if (trainer_spell->IsCastable())
        _player->CastSpell(_player, trainer_spell->spell, true);
    else
        _player->learnSpell(spellId, false);
}

void WorldSession::SendTrainerBuyFailed(ObjectGuid guid, uint32 spellId, uint32 reason)
{
    WorldPacket data(SMSG_TRAINER_BUY_FAILED, 8 + 4 + 4);
    data.WriteGuidMask(guid, 3, 0, 4, 7, 6, 1, 5, 2);

    data.WriteGuidBytes(guid, 1, 2, 0, 3, 4);
    data << uint32(reason);         // 1 == "Not enough money for trainer service." 0 == "Trainer service %d unavailable."
    data.WriteGuidBytes(guid, 5, 6, 7);
    data << uint32(spellId);        // should be same as in packet from client
    SendPacket(&data);
}

void WorldSession::HandleGossipHelloOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GOSSIP_HELLO");

    ObjectGuid guid;
    recvData.ReadGuidMask(guid, 2, 4, 0, 3, 6, 7, 5, 1);
    recvData.ReadGuidBytes(guid, 4, 7, 1, 0, 5, 3, 6, 2);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleGossipHelloOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // set faction visible if needed
    if (FactionTemplateEntry const* factionTemplateEntry = sFactionTemplateStore.LookupEntry(unit->getFaction()))
        _player->GetReputationMgr().SetVisible(factionTemplateEntry);

    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_TALK);
    // remove fake death
    //if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
    //    GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (unit->IsArmorer() || unit->IsCivilian() || unit->IsQuestGiver() || unit->IsServiceProvider() || unit->IsGuard())
        unit->StopMoving();

    if (unit->IsBMAuctioner())
    {
        _player->GetSession()->SendBlackMarketHello(unit->GetGUID(), sBlackMarketMgr->isBlackMarketOpen());
        return;
    }

    // If spiritguide, no need for gossip menu, just put player into resurrect queue
    if (unit->IsSpiritGuide())
    {
        Battleground* bg = _player->GetBattleground();
        if (bg)
        {
            bg->AddPlayerToResurrectQueue(unit->GetGUID(), _player->GetGUID());
            sBattlegroundMgr->SendAreaSpiritHealerQueryOpcode(_player, bg, unit->GetGUID());
            return;
        }
    }

    _player->PlayerTalkClass->ClearMenus();
    if (!sScriptMgr->OnGossipHello(_player, unit) && !unit->AI()->OnGossipHello(_player))
    {
        _player->TalkedToCreature(unit->GetEntry(), unit->GetGUID());
        _player->PrepareGossipMenu(unit, unit->GetCreatureTemplate()->GossipMenuId, true);
        _player->SendPreparedGossip(unit);
    }
}

/*void WorldSession::HandleGossipSelectOptionOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_GOSSIP_SELECT_OPTION");

    uint32 option;
    uint32 unk;
    uint64 guid;
    std::string code = "";

    recvData >> guid >> unk >> option;

    if (_player->PlayerTalkClass->GossipOptionCoded(option))
    {
        SF_LOG_DEBUG("network", "reading string");
        recvData >> code;
        SF_LOG_DEBUG("network", "string read: %s", code.c_str());
    }

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleGossipSelectOptionOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (!code.empty())
    {
        if (!Script->GossipSelectWithCode(_player, unit, _player->PlayerTalkClass->GossipOptionSender (option), _player->PlayerTalkClass->GossipOptionAction(option), code.c_str()))
            unit->OnGossipSelect (_player, option);
    }
    else
    {
        if (!Script->OnGossipSelect (_player, unit, _player->PlayerTalkClass->GossipOptionSender (option), _player->PlayerTalkClass->GossipOptionAction (option)))
           unit->OnGossipSelect (_player, option);
    }
}*/

void WorldSession::HandleSpiritHealerActivateOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_SPIRIT_HEALER_ACTIVATE");

    ObjectGuid UnitGUID;

    recvData.ReadGuidMask(UnitGUID, 2, 7, 6, 0, 5, 4, 1, 3);
    recvData.ReadGuidBytes(UnitGUID, 1, 5, 6, 3, 2, 0, 7, 4);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(UnitGUID, UNIT_NPC_FLAG_SPIRITHEALER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleSpiritHealerActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(UnitGUID)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendSpiritResurrect();
}

void WorldSession::SendSpiritResurrect()
{
    _player->ResurrectPlayer(0.5f, true);

    _player->DurabilityLossAll(0.25f, true);

    // get corpse nearest graveyard
    WorldSafeLocsEntry const* corpseGrave = NULL;
    Corpse* corpse = _player->GetCorpse();
    if (corpse)
        corpseGrave = sObjectMgr->GetClosestGraveYard(
            corpse->GetPositionX(), corpse->GetPositionY(), corpse->GetPositionZ(), corpse->GetMapId(), _player->GetTeam());

    // now can spawn bones
    _player->SpawnCorpseBones();

    // teleport to nearest from corpse graveyard, if different from nearest to player ghost
    if (corpseGrave)
    {
        WorldSafeLocsEntry const* ghostGrave = sObjectMgr->GetClosestGraveYard(
            _player->GetPositionX(), _player->GetPositionY(), _player->GetPositionZ(), _player->GetMapId(), _player->GetTeam());

        if (corpseGrave != ghostGrave)
            _player->TeleportTo(corpseGrave->map_id, corpseGrave->x, corpseGrave->y, corpseGrave->z, _player->GetOrientation());
        // or update at original position
        else
            _player->UpdateObjectVisibility();
    }
    // or update at original position
    else
        _player->UpdateObjectVisibility();
}

void WorldSession::HandleBinderActivateOpcode(WorldPacket& recvData)
{
    ObjectGuid npcGuid;

    recvData.ReadGuidMask(npcGuid, 0, 5, 4, 7, 6, 2, 1, 3);
    recvData.ReadGuidBytes(npcGuid, 0, 4, 2, 3, 7, 1, 5, 6);

    if (!GetPlayer()->IsInWorld() || !GetPlayer()->IsAlive())
        return;

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_INNKEEPER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleBinderActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(npcGuid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendBindPoint(unit);
}

void WorldSession::SendBindPoint(Creature* npc)
{
    // prevent set homebind to instances in any case
    if (GetPlayer()->GetMap()->Instanceable())
        return;

    uint32 bindspell = 3286;

    // send spell for homebinding (3286)
    npc->CastSpell(_player, bindspell, true);

    _player->PlayerTalkClass->SendCloseGossip();
}

void WorldSession::HandleRequestStabledPetsOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recv CMSG_REQUEST_STABLED_PETS");
    ObjectGuid npcGUID;

    recvData.ReadGuidMask(npcGUID, 0, 5, 1, 3, 6, 7, 2, 4);
    recvData.ReadGuidBytes(npcGUID, 0, 5, 7, 1, 2, 3, 4, 6);

    if (!CheckStableMaster(npcGUID))
        return;

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // remove mounts this fix bug where getting pet from stable while mounted deletes pet.
    if (GetPlayer()->IsMounted())
        GetPlayer()->RemoveAurasByType(SPELL_AURA_MOUNTED);

    SendStablePet(npcGUID);
}

void WorldSession::SendStablePet(uint64 guid)
{
    // Stable master: full active + stable list for UI.
    SendPetList(guid, PET_SAVE_FIRST_ACTIVE_SLOT, PET_SAVE_LAST_STABLE_SLOT);

    // Client SetPetSlot sets an in-flight lock cleared only by SMSG_STABLE_RESULT (or
    // PET_SLOT_UPDATED). If the server dies mid-move, that lock sticks for the WoW process
    // and later drops silently no-op with no CMSG. Any result byte clears it; 0 is silent.
    if (guid)
        SendStableResult(0);
}

void WorldSession::SendStablePetCallback(PreparedQueryResult /*result*/, uint64 guid)
{
    // Kept for query callback wiring; MoP list is built synchronously in SendPetList.
    SendStablePet(guid);
}

void WorldSession::SendPetList(uint64 npcGuid, uint8 firstSlot, uint8 lastSlot)
{
    if (!_player)
        return;

    SF_LOG_DEBUG("network", "WORLD: Send SMSG_PET_STABLE_LIST (slots %u-%u).", firstSlot, lastSlot);

    // Layout verified against retail 18414 SMSG_PET_STABLE_LIST in private_parser example sniffs.
    // Non-zero StableMaster GUID makes the client treat this as an open stable-master interaction
    // (PetStable UI). Login / tame / dismiss / Call Pet must send GUID 0 so m_petList still updates
    // for Call Pet icons without popping the stable window. Only real stable-master visits pass npcGuid.
    ObjectGuid guid = npcGuid;

    QueryResult result = CharacterDatabase.PQuery(
        "SELECT id, entry, level, name, modelid, slot FROM character_pet WHERE owner = %u AND slot >= %u AND slot <= %u ORDER BY slot",
        _player->GetGUIDLow(), uint32(firstSlot), uint32(lastSlot));

    struct PetListEntry
    {
        uint32 petNumber = 0;
        uint32 entry = 0;
        uint32 level = 0;
        uint32 modelId = 0;
        uint32 slot = 0;
        uint8 petStableState = 1;
        std::string name;
    };

    std::vector<PetListEntry> pets;
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            PetListEntry pet;
            pet.petNumber = fields[0].GetUInt32();
            pet.entry = fields[1].GetUInt32();
            pet.level = fields[2].GetUInt16();
            pet.name = fields[3].GetString();
            pet.modelId = fields[4].GetUInt32();
            pet.slot = fields[5].GetUInt8();

            if (!pet.modelId)
            {
                if (CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(pet.entry))
                    pet.modelId = cInfo->GetFirstValidModelId();
            }

            // 1 = active Call Pet slot, 2 = stabled (matches retail 18414)
            pet.petStableState = IsActivePetSlot(int8(pet.slot)) ? 1 : 2;
            pets.push_back(std::move(pet));
        } while (result->NextRow());
    }

    WorldPacket data(SMSG_PET_STABLE_LIST, 200);
    ByteBuffer buff;

    data.WriteGuidMask(guid, 3, 0, 4, 7, 2, 1, 6, 5);
    data.WriteBits(pets.size(), 19);
    for (PetListEntry const& pet : pets)
        data.WriteBits(pet.name.size(), 8);
    data.FlushBits();

    for (PetListEntry const& pet : pets)
    {
        buff << uint32(pet.entry);
        buff << uint32(pet.level);
        buff << uint8(pet.petStableState);
        buff << uint32(pet.modelId);
        buff.WriteString(pet.name);
        buff << uint32(pet.petNumber);
        buff << uint32(pet.slot);
    }

    data.append(buff);
    data.WriteGuidBytes(guid, 3, 5, 7, 2, 0, 4, 1, 6);
    SendPacket(&data);
}

void WorldSession::SendStableResult(uint8 res)
{
    WorldPacket data(SMSG_STABLE_RESULT, 1);
    data << uint8(res);
    SendPacket(&data);
}

void WorldSession::HandleSetPetSlot(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recv CMSG_SET_PET_SLOT");

    uint32 petNumber = 0;
    int8 newSlot = -1;
    ObjectGuid npcGUID;

    recvData >> petNumber;
    recvData >> newSlot;
    recvData.ReadGuidMask(npcGUID, 5, 7, 3, 2, 6, 1, 0, 4);
    recvData.ReadGuidBytes(npcGUID, 0, 3, 2, 6, 5, 7, 4, 1);

    if (!CheckStableMaster(npcGUID) ||
        newSlot < int8(PET_SAVE_FIRST_ACTIVE_SLOT) ||
        newSlot > int8(PET_SAVE_LAST_STABLE_SLOT))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    if (!_player->IsAlive())
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    if (_player->HasUnitState(UNIT_STATE_DIED))
        _player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // Sync query — CHAR_SEL_PET_SLOT_BY_ID is ASYNC-prepared and must not use Query().
    QueryResult result = CharacterDatabase.PQuery(
        "SELECT slot, entry FROM character_pet WHERE owner = %u AND id = %u",
        _player->GetGUIDLow(), petNumber);
    if (!result)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    Field* fields = result->Fetch();
    uint8 const oldSlot = fields[0].GetUInt8();
    uint32 const petEntry = fields[1].GetUInt32();

    if (oldSlot == uint8(newSlot))
    {
        SendStableResult(STABLE_SUCCESS_STABLE);
        return;
    }

    if (!IsActivePetSlot(oldSlot) && !IsStabledPetSlot(oldSlot))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // Moving into Call Pet slots requires the pet to be tameable for this hunter.
    if (IsActivePetSlot(newSlot))
    {
        CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(petEntry);
        if (!creatureInfo || !creatureInfo->IsTameable(_player->CanTameExoticPets()))
        {
            SendStableResult(creatureInfo && creatureInfo->IsTameable(true) ? STABLE_ERR_EXOTIC : STABLE_ERR_STABLE);
            return;
        }
    }

    uint32 destPetNumber = 0;
    uint32 destPetEntry = 0;
    if (QueryResult destResult = CharacterDatabase.PQuery(
        "SELECT id, entry FROM character_pet WHERE owner = %u AND slot = %u",
        _player->GetGUIDLow(), uint32(newSlot)))
    {
        Field* destFields = destResult->Fetch();
        destPetNumber = destFields[0].GetUInt32();
        destPetEntry = destFields[1].GetUInt32();
        if (destPetNumber == petNumber)
            destPetNumber = 0;
    }

    // Destination pet moving into an active slot also needs the exotic check.
    if (destPetNumber && IsActivePetSlot(oldSlot))
    {
        CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(destPetEntry);
        if (!creatureInfo || !creatureInfo->IsTameable(_player->CanTameExoticPets()))
        {
            SendStableResult(creatureInfo && creatureInfo->IsTameable(true) ? STABLE_ERR_EXOTIC : STABLE_ERR_STABLE);
            return;
        }
    }

    Pet* currentPet = _player->GetPet();
    uint32 currentPetNumber = 0;
    uint8 currentPetSlot = 0;
    if (currentPet && currentPet->getPetType() == PetType::HUNTER_PET && currentPet->GetCharmInfo())
    {
        currentPetNumber = currentPet->GetCharmInfo()->GetPetNumber();
        currentPetSlot = currentPet->GetSlot();
    }

    bool const dismissCurrent =
        currentPetNumber &&
        ((currentPetNumber == petNumber && IsStabledPetSlot(newSlot)) ||
         (currentPetNumber == destPetNumber && IsStabledPetSlot(oldSlot)) ||
         (uint8(newSlot) == currentPetSlot && IsStabledPetSlot(oldSlot)));

    if (dismissCurrent)
    {
        if (!currentPet || !currentPet->IsAlive())
        {
            SendStableResult(STABLE_ERR_STABLE);
            return;
        }

        // Save summoned pet into the slot it will occupy after the move/swap.
        uint8 const dismissSlot = (currentPetNumber == petNumber) ? uint8(newSlot) : oldSlot;
        _player->RemovePet(currentPet, PetSaveMode(dismissSlot));
        currentPet = nullptr;

        if (currentPetNumber == petNumber)
        {
            // Source pet already saved to newSlot by RemovePet; move displaced pet if any.
            if (destPetNumber)
                CharacterDatabase.DirectPExecute(
                    "UPDATE character_pet SET slot = %u WHERE owner = %u AND id = %u",
                    uint32(oldSlot), _player->GetGUIDLow(), destPetNumber);
        }
        else
        {
            // Summoned pet was the destination occupant; source pet takes newSlot.
            CharacterDatabase.DirectPExecute(
                "UPDATE character_pet SET slot = %u WHERE owner = %u AND id = %u",
                uint32(newSlot), _player->GetGUIDLow(), petNumber);
        }

        SendStableResult(STABLE_SUCCESS_STABLE);
        SendPetList(npcGUID, PET_SAVE_FIRST_ACTIVE_SLOT, PET_SAVE_LAST_STABLE_SLOT);
        return;
    }

    // Apply immediately so SendPetList sees the new layout.
    CharacterDatabase.DirectPExecute(
        "UPDATE character_pet SET slot = %u WHERE owner = %u AND id = %u",
        uint32(newSlot), _player->GetGUIDLow(), petNumber);

    if (destPetNumber)
        CharacterDatabase.DirectPExecute(
            "UPDATE character_pet SET slot = %u WHERE owner = %u AND id = %u",
            uint32(oldSlot), _player->GetGUIDLow(), destPetNumber);

    // Keep the live pet's slot in sync when rearranging active pets.
    if (currentPet && currentPetNumber == petNumber)
        currentPet->SetSlot(uint8(newSlot));
    else if (currentPet && currentPetNumber == destPetNumber)
        currentPet->SetSlot(oldSlot);

    SendStableResult(STABLE_SUCCESS_STABLE);
    SendPetList(npcGUID, PET_SAVE_FIRST_ACTIVE_SLOT, PET_SAVE_LAST_STABLE_SLOT);
}

void WorldSession::HandleStablePet(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recv CMSG_STABLE_PET");
    uint64 npcGUID;

    recvData >> npcGUID;

    if (!GetPlayer()->IsAlive())
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    if (!CheckStableMaster(npcGUID))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    Pet* pet = _player->GetPet();

    // can't place in stable dead pet
    if (!pet || !pet->IsAlive() || pet->getPetType() != PetType::HUNTER_PET)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PET_SLOTS);

    stmt->setUInt32(0, _player->GetGUIDLow());
    stmt->setUInt8(1, PET_SAVE_FIRST_STABLE_SLOT);
    stmt->setUInt8(2, PET_SAVE_LAST_STABLE_SLOT);

    _stablePetCallback = CharacterDatabase.AsyncQuery(stmt);
}

void WorldSession::HandleStablePetCallback(PreparedQueryResult result)
{
    if (!GetPlayer())
        return;

    uint8 freeSlot = PET_SAVE_FIRST_STABLE_SLOT;
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            uint8 slot = fields[1].GetUInt8();

            // slots ordered in query, and if not equal then free
            if (slot != freeSlot)
                break;

            // this slot not free, skip
            ++freeSlot;
        } while (result->NextRow());
    }

    uint8 const lastUnlockedStable = uint8(PET_SAVE_FIRST_STABLE_SLOT + GetPlayer()->m_stableSlots);
    if (GetPlayer()->m_stableSlots > 0 &&
        freeSlot >= PET_SAVE_FIRST_STABLE_SLOT &&
        freeSlot < lastUnlockedStable &&
        freeSlot <= PET_SAVE_LAST_STABLE_SLOT)
    {
        _player->RemovePet(_player->GetPet(), PetSaveMode(freeSlot));
        SendStableResult(STABLE_SUCCESS_STABLE);
        SendPetList(0, PET_SAVE_FIRST_ACTIVE_SLOT, PET_SAVE_LAST_STABLE_SLOT);
    }
    else
        SendStableResult(STABLE_ERR_INVALID_SLOT);
}

void WorldSession::HandleUnstablePet(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recv CMSG_UNSTABLE_PET.");
    uint64 npcGUID;
    uint32 petnumber;

    recvData >> npcGUID >> petnumber;

    if (!CheckStableMaster(npcGUID))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PET_ENTRY);

    stmt->setUInt32(0, _player->GetGUIDLow());
    stmt->setUInt32(1, petnumber);
    stmt->setUInt8(2, PET_SAVE_FIRST_STABLE_SLOT);
    stmt->setUInt8(3, PET_SAVE_LAST_STABLE_SLOT);

    _unstablePetCallback.SetParam(petnumber);
    _unstablePetCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleUnstablePetCallback(PreparedQueryResult result, uint32 petId)
{
    if (!GetPlayer())
        return;

    uint32 petEntry = 0;
    if (result)
    {
        Field* fields = result->Fetch();
        petEntry = fields[0].GetUInt32();
    }

    if (!petEntry)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(petEntry);
    if (!creatureInfo || !creatureInfo->IsTameable(_player->CanTameExoticPets()))
    {
        // if problem in exotic pet
        if (creatureInfo && creatureInfo->IsTameable(true))
            SendStableResult(STABLE_ERR_EXOTIC);
        else
            SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    Pet* pet = _player->GetPet();
    if (pet && pet->IsAlive())
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // delete dead pet
    if (pet)
        _player->RemovePet(pet, PET_SAVE_AS_DELETED);

    Pet* newPet = new Pet(_player, PetType::HUNTER_PET);
    if (!newPet->LoadPetFromDB(_player, petEntry, petId))
    {
        delete newPet;
        newPet = NULL;
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    SendStableResult(STABLE_SUCCESS_UNSTABLE);
}

void WorldSession::HandleBuyStableSlot(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recv CMSG_BUY_STABLE_SLOT.");
    uint64 npcGUID;

    recvData >> npcGUID;

    if (!CheckStableMaster(npcGUID))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (GetPlayer()->m_stableSlots < MAX_PET_STABLES)
    {
        /*StableSlotPricesEntry const* SlotPrice = sStableSlotPricesStore.LookupEntry(GetPlayer()->m_stableSlots+1);
        if (_player->HasEnoughMoney(SlotPrice->Price))
        {
            ++GetPlayer()->m_stableSlots;
            _player->ModifyMoney(-int32(SlotPrice->Price));
            SendStableResult(STABLE_SUCCESS_BUY_SLOT);
        }
        else
            SendStableResult(STABLE_ERR_MONEY);*/
    }
    else
        SendStableResult(STABLE_ERR_STABLE);
}

void WorldSession::HandleStableRevivePet(WorldPacket&/* recvData */)
{
    SF_LOG_DEBUG("network", "HandleStableRevivePet: Not implemented");
}

void WorldSession::HandleStableSwapPet(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recv CMSG_STABLE_SWAP_PET.");
    uint64 npcGUID;
    uint32 petId;

    recvData >> npcGUID >> petId;

    if (!CheckStableMaster(npcGUID))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    Pet* pet = _player->GetPet();

    if (!pet || pet->getPetType() != PetType::HUNTER_PET)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // Find swapped pet slot in stable

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PET_SLOT_BY_ID);

    stmt->setUInt32(0, _player->GetGUIDLow());
    stmt->setUInt32(1, petId);

    _stableSwapCallback.SetParam(petId);
    _stableSwapCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleStableSwapPetCallback(PreparedQueryResult result, uint32 petId)
{
    if (!GetPlayer())
        return;

    if (!result)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    Field* fields = result->Fetch();

    uint32 slot = fields[0].GetUInt8();
    uint32 petEntry = fields[1].GetUInt32();

    if (!petEntry)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(petEntry);
    if (!creatureInfo || !creatureInfo->IsTameable(true))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    if (!creatureInfo->IsTameable(_player->CanTameExoticPets()))
    {
        SendStableResult(STABLE_ERR_EXOTIC);
        return;
    }

    Pet* pet = _player->GetPet();
    // The player's pet could have been removed during the delay of the DB callback
    if (!pet)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // move alive pet to slot or delete dead pet
    _player->RemovePet(pet, pet->IsAlive() ? PetSaveMode(slot) : PET_SAVE_AS_DELETED);

    // summon unstabled pet
    Pet* newPet = new Pet(_player);
    if (!newPet->LoadPetFromDB(_player, petEntry, petId))
    {
        delete newPet;
        SendStableResult(STABLE_ERR_STABLE);
    }
    else
        SendStableResult(STABLE_SUCCESS_UNSTABLE);
}

void WorldSession::HandleRepairItemOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_REPAIR_ITEM");

    ObjectGuid npcGuid, itemGuid;
    bool guildBank;                                         // new in 2.3.2, bool that means from guild bank money

    recvData.ReadGuidMask(itemGuid, 2, 5);
    recvData.ReadGuidMask(npcGuid, 3);
    guildBank = recvData.ReadBit();
    recvData.ReadGuidMask(npcGuid, 7);
    recvData.ReadGuidMask(itemGuid, 4);
    recvData.ReadGuidMask(npcGuid, 2);
    recvData.ReadGuidMask(itemGuid, 0, 3);
    recvData.ReadGuidMask(npcGuid, 6, 1, 4);
    recvData.ReadGuidMask(itemGuid, 6);
    recvData.ReadGuidMask(npcGuid, 5, 0);
    recvData.ReadGuidMask(itemGuid, 7, 1);

    recvData.ReadGuidBytes(itemGuid, 2);
    recvData.ReadGuidBytes(npcGuid, 1);
    recvData.ReadGuidBytes(itemGuid, 1);
    recvData.ReadGuidBytes(npcGuid, 4, 7, 3, 2);
    recvData.ReadGuidBytes(itemGuid, 7);
    recvData.ReadGuidBytes(npcGuid, 5, 0);
    recvData.ReadGuidBytes(itemGuid, 5, 3, 4, 6);
    recvData.ReadGuidBytes(npcGuid, 6);
    recvData.ReadGuidBytes(itemGuid, 0);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_REPAIR);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleRepairItemOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(npcGuid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // reputation discount
    float discountMod = _player->GetReputationPriceDiscount(unit);

    if (itemGuid)
    {
        SF_LOG_DEBUG("network", "ITEM: Repair item, itemGUID = %u, npcGUID = %u", GUID_LOPART(itemGuid), GUID_LOPART(npcGuid));

        Item* item = _player->GetItemByGuid(itemGuid);
        if (item)
            _player->DurabilityRepair(item->GetPos(), true, discountMod, guildBank);
    }
    else
    {
        SF_LOG_DEBUG("network", "ITEM: Repair all items, npcGUID = %u", GUID_LOPART(npcGuid));
        _player->DurabilityRepairAll(true, discountMod, guildBank);
    }
}
