/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AccountMgr.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "GuildMgr.h"
#include "Item.h"
#include "Language.h"
#include "Log.h"
#include "Mail.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

namespace
{
struct SendMailRequest
{
    ObjectGuid mailbox;
    ObjectGuid itemGuids[MAX_MAIL_ITEMS];
    uint64 money;
    uint64 COD;
    std::string receiverName;
    std::string subject;
    std::string body;
    uint32 unk1;
    uint32 unk2;
    uint8 itemCount;
};

struct MailDeleteRequest
{
    uint32 mailId;
};

struct MailboxMailRequest
{
    ObjectGuid mailbox;
    uint32 mailId;
};

struct MailTakeItemRequest
{
    ObjectGuid mailbox;
    uint32 mailId;
    uint32 itemId;
};

struct MailTakeMoneyRequest
{
    ObjectGuid mailbox;
    uint64 money;
    uint32 mailId;
};

struct MailboxRequest
{
    ObjectGuid mailbox;
};

bool ReadSendMailRequest(WorldPacket& recvData, SendMailRequest& request)
{
    uint32 bodyLength;
    uint32 subjectLength;
    uint32 receiverLength;

    recvData >> request.unk1 >> request.unk2;       // both unknown
    recvData >> request.COD >> request.money;       // money and cod

    request.mailbox[0] = recvData.ReadBit();
    request.mailbox[6] = recvData.ReadBit();
    request.mailbox[4] = recvData.ReadBit();
    request.mailbox[1] = recvData.ReadBit();
    bodyLength = recvData.ReadBits(11);
    request.mailbox[3] = recvData.ReadBit();
    receiverLength = recvData.ReadBits(9);
    request.mailbox[7] = recvData.ReadBit();
    request.mailbox[5] = recvData.ReadBit();

    uint32 itemCount = recvData.ReadBits(5);        // attached items count
    if (itemCount > MAX_MAIL_ITEMS)                 // client limit
        return false;

    request.itemCount = uint8(itemCount);

    for (uint8 i = 0; i < request.itemCount; ++i)
    {
        request.itemGuids[i][1] = recvData.ReadBit();
        request.itemGuids[i][7] = recvData.ReadBit();
        request.itemGuids[i][2] = recvData.ReadBit();
        request.itemGuids[i][5] = recvData.ReadBit();
        request.itemGuids[i][0] = recvData.ReadBit();
        request.itemGuids[i][6] = recvData.ReadBit();
        request.itemGuids[i][3] = recvData.ReadBit();
        request.itemGuids[i][4] = recvData.ReadBit();
    }

    subjectLength = recvData.ReadBits(9);
    request.mailbox[2] = recvData.ReadBit();

    for (uint8 i = 0; i < request.itemCount; ++i)
    {
        recvData.read_skip<uint8>();                // item slot in mail, not used
        recvData.ReadByteSeq(request.itemGuids[i][3]);
        recvData.ReadByteSeq(request.itemGuids[i][0]);
        recvData.ReadByteSeq(request.itemGuids[i][2]);
        recvData.ReadByteSeq(request.itemGuids[i][1]);
        recvData.ReadByteSeq(request.itemGuids[i][6]);
        recvData.ReadByteSeq(request.itemGuids[i][5]);
        recvData.ReadByteSeq(request.itemGuids[i][7]);
        recvData.ReadByteSeq(request.itemGuids[i][4]);
    }

    recvData.ReadByteSeq(request.mailbox[1]);
    request.body = recvData.ReadString(bodyLength);
    recvData.ReadByteSeq(request.mailbox[0]);
    request.subject = recvData.ReadString(subjectLength);
    recvData.ReadByteSeq(request.mailbox[2]);
    recvData.ReadByteSeq(request.mailbox[6]);
    recvData.ReadByteSeq(request.mailbox[5]);
    recvData.ReadByteSeq(request.mailbox[7]);
    recvData.ReadByteSeq(request.mailbox[3]);
    recvData.ReadByteSeq(request.mailbox[4]);
    request.receiverName = recvData.ReadString(receiverLength);

    return true;
}

MailboxMailRequest ReadMailMarkAsReadRequest(WorldPacket& recvData)
{
    MailboxMailRequest request;
    recvData >> request.mailId;

    request.mailbox[0] = recvData.ReadBit();
    request.mailbox[2] = recvData.ReadBit();
    request.mailbox[3] = recvData.ReadBit();
    recvData.ReadBit();
    request.mailbox[4] = recvData.ReadBit();
    request.mailbox[6] = recvData.ReadBit();
    request.mailbox[7] = recvData.ReadBit();
    request.mailbox[1] = recvData.ReadBit();
    request.mailbox[5] = recvData.ReadBit();
    recvData.FlushBits();

    recvData.ReadByteSeq(request.mailbox[1]);
    recvData.ReadByteSeq(request.mailbox[7]);
    recvData.ReadByteSeq(request.mailbox[2]);
    recvData.ReadByteSeq(request.mailbox[5]);
    recvData.ReadByteSeq(request.mailbox[6]);
    recvData.ReadByteSeq(request.mailbox[3]);
    recvData.ReadByteSeq(request.mailbox[4]);
    recvData.ReadByteSeq(request.mailbox[0]);

    return request;
}

MailDeleteRequest ReadMailDeleteRequest(WorldPacket& recvData)
{
    MailDeleteRequest request;
    recvData >> request.mailId;
    recvData.read_skip<uint32>();                   // mailTemplateId
    return request;
}

MailboxMailRequest ReadMailReturnToSenderRequest(WorldPacket& recvData)
{
    MailboxMailRequest request;
    recvData >> request.mailId;

    request.mailbox[2] = recvData.ReadBit();
    request.mailbox[0] = recvData.ReadBit();
    request.mailbox[4] = recvData.ReadBit();
    request.mailbox[6] = recvData.ReadBit();
    request.mailbox[3] = recvData.ReadBit();
    request.mailbox[1] = recvData.ReadBit();
    request.mailbox[7] = recvData.ReadBit();
    request.mailbox[5] = recvData.ReadBit();

    recvData.ReadByteSeq(request.mailbox[5]);
    recvData.ReadByteSeq(request.mailbox[6]);
    recvData.ReadByteSeq(request.mailbox[2]);
    recvData.ReadByteSeq(request.mailbox[0]);
    recvData.ReadByteSeq(request.mailbox[3]);
    recvData.ReadByteSeq(request.mailbox[1]);
    recvData.ReadByteSeq(request.mailbox[4]);
    recvData.ReadByteSeq(request.mailbox[7]);

    return request;
}

MailTakeItemRequest ReadMailTakeItemRequest(WorldPacket& recvData)
{
    MailTakeItemRequest request;
    recvData >> request.mailId;
    recvData >> request.itemId;

    request.mailbox[6] = recvData.ReadBit();
    request.mailbox[5] = recvData.ReadBit();
    request.mailbox[2] = recvData.ReadBit();
    request.mailbox[3] = recvData.ReadBit();
    request.mailbox[0] = recvData.ReadBit();
    request.mailbox[1] = recvData.ReadBit();
    request.mailbox[4] = recvData.ReadBit();
    request.mailbox[7] = recvData.ReadBit();

    recvData.ReadByteSeq(request.mailbox[0]);
    recvData.ReadByteSeq(request.mailbox[1]);
    recvData.ReadByteSeq(request.mailbox[4]);
    recvData.ReadByteSeq(request.mailbox[2]);
    recvData.ReadByteSeq(request.mailbox[5]);
    recvData.ReadByteSeq(request.mailbox[6]);
    recvData.ReadByteSeq(request.mailbox[3]);
    recvData.ReadByteSeq(request.mailbox[7]);

    return request;
}

MailTakeMoneyRequest ReadMailTakeMoneyRequest(WorldPacket& recvData)
{
    MailTakeMoneyRequest request;
    recvData >> request.mailId;
    recvData >> request.money;

    request.mailbox[7] = recvData.ReadBit();
    request.mailbox[6] = recvData.ReadBit();
    request.mailbox[3] = recvData.ReadBit();
    request.mailbox[2] = recvData.ReadBit();
    request.mailbox[4] = recvData.ReadBit();
    request.mailbox[5] = recvData.ReadBit();
    request.mailbox[0] = recvData.ReadBit();
    request.mailbox[1] = recvData.ReadBit();

    recvData.ReadByteSeq(request.mailbox[7]);
    recvData.ReadByteSeq(request.mailbox[1]);
    recvData.ReadByteSeq(request.mailbox[4]);
    recvData.ReadByteSeq(request.mailbox[0]);
    recvData.ReadByteSeq(request.mailbox[3]);
    recvData.ReadByteSeq(request.mailbox[2]);
    recvData.ReadByteSeq(request.mailbox[6]);
    recvData.ReadByteSeq(request.mailbox[5]);

    return request;
}

MailboxRequest ReadGetMailListRequest(WorldPacket& recvData)
{
    MailboxRequest request;

    request.mailbox[6] = recvData.ReadBit();
    request.mailbox[3] = recvData.ReadBit();
    request.mailbox[7] = recvData.ReadBit();
    request.mailbox[5] = recvData.ReadBit();
    request.mailbox[4] = recvData.ReadBit();
    request.mailbox[1] = recvData.ReadBit();
    request.mailbox[2] = recvData.ReadBit();
    request.mailbox[0] = recvData.ReadBit();

    recvData.ReadByteSeq(request.mailbox[7]);
    recvData.ReadByteSeq(request.mailbox[1]);
    recvData.ReadByteSeq(request.mailbox[6]);
    recvData.ReadByteSeq(request.mailbox[5]);
    recvData.ReadByteSeq(request.mailbox[4]);
    recvData.ReadByteSeq(request.mailbox[2]);
    recvData.ReadByteSeq(request.mailbox[3]);
    recvData.ReadByteSeq(request.mailbox[0]);

    return request;
}

MailboxMailRequest ReadMailCreateTextItemRequest(WorldPacket& recvData)
{
    MailboxMailRequest request;
    recvData >> request.mailId;

    request.mailbox[4] = recvData.ReadBit();
    request.mailbox[1] = recvData.ReadBit();
    request.mailbox[6] = recvData.ReadBit();
    request.mailbox[2] = recvData.ReadBit();
    request.mailbox[5] = recvData.ReadBit();
    request.mailbox[3] = recvData.ReadBit();
    request.mailbox[0] = recvData.ReadBit();
    request.mailbox[7] = recvData.ReadBit();

    recvData.ReadByteSeq(request.mailbox[6]);
    recvData.ReadByteSeq(request.mailbox[5]);
    recvData.ReadByteSeq(request.mailbox[4]);
    recvData.ReadByteSeq(request.mailbox[3]);
    recvData.ReadByteSeq(request.mailbox[0]);
    recvData.ReadByteSeq(request.mailbox[7]);
    recvData.ReadByteSeq(request.mailbox[2]);
    recvData.ReadByteSeq(request.mailbox[1]);

    return request;
}

bool HasMailboxAccess(Player* player, ObjectGuid mailbox)
{
    return player->GetGameObjectIfCanInteractWith(mailbox, GAMEOBJECT_TYPE_MAILBOX) != NULL;
}

bool IsDeliveredMail(Mail const* mail)
{
    return mail && mail->state != MailState::MAIL_STATE_DELETED && mail->deliver_time <= time(NULL);
}

Mail* GetDeliveredMailOrSendResult(Player* player, uint32 mailId, MailResponseType action)
{
    Mail* mail = player->GetMail(mailId);
    if (!IsDeliveredMail(mail))
    {
        player->SendMailResult(mailId, action, MAIL_ERR_INTERNAL_ERROR);
        return NULL;
    }

    return mail;
}

Mail* GetMoneyMailOrSendResult(Player* player, uint32 mailId, uint64 money)
{
    Mail* mail = player->GetMail(mailId);
    if (!IsDeliveredMail(mail) || (money > 0 && mail->money != money))
    {
        player->SendMailResult(mailId, MAIL_MONEY_TAKEN, MAIL_ERR_INTERNAL_ERROR);
        return NULL;
    }

    return mail;
}

Mail* GetTextMailOrSendResult(Player* player, uint32 mailId)
{
    Mail* mail = player->GetMail(mailId);
    if (!IsDeliveredMail(mail) || (mail->body.empty() && !mail->mailTemplateId))
    {
        player->SendMailResult(mailId, MAIL_MADE_PERMANENT, MAIL_ERR_INTERNAL_ERROR);
        return NULL;
    }

    return mail;
}

bool ValidateMailDelete(Player* player, uint32 mailId, Mail const* mail)
{
    if (mail && mail->COD)
    {
        player->SendMailResult(mailId, MAIL_DELETED, MAIL_ERR_INTERNAL_ERROR);
        return false;
    }

    return true;
}

bool ValidateSendMailCost(Player* player, uint64 requiredMoney)
{
    if (player->HasEnoughMoney(requiredMoney) || player->IsGameMaster())
        return true;

    player->SendMailResult(0, MAIL_SEND, MAIL_ERR_NOT_ENOUGH_MONEY);
    return false;
}

bool ValidateReceiverMailboxCapacity(Player* player, uint8 mailsCount)
{
    // Mail count is sent as uint8 in the opcode; keep the historical cap below that limit.
    if (mailsCount <= 100)
        return true;

    player->SendMailResult(0, MAIL_SEND, MAIL_ERR_RECIPIENT_CAP_REACHED);
    return false;
}

bool AreAttachedItemsAccountBound(Player* player, ObjectGuid* itemGuids, uint8 itemCount)
{
    if (!itemCount)
        return false;

    for (uint8 i = 0; i < itemCount; ++i)
    {
        if (Item* item = player->GetItemByGuid(itemGuids[i]))
        {
            ItemTemplate const* itemProto = item->GetTemplate();
            if (!itemProto || !(itemProto->Flags & ITEM_PROTO_FLAG_BIND_TO_ACCOUNT))
                return false;
        }
    }

    return true;
}

bool ValidateReceiverFaction(Player* player, uint32 receiverTeam, bool accountBound, bool allowTwoSideMail)
{
    if (accountBound || player->GetTeam() == receiverTeam || allowTwoSideMail)
        return true;

    player->SendMailResult(0, MAIL_SEND, MAIL_ERR_NOT_YOUR_TEAM);
    return false;
}

bool ValidateSendMailAttachments(Player* player, ObjectGuid* itemGuids, uint8 itemCount, uint64 COD, uint32 receiverAccountId, Item** items)
{
    for (uint8 i = 0; i < itemCount; ++i)
    {
        if (!itemGuids[i])
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_MAIL_ATTACHMENT_INVALID);
            return false;
        }

        Item* item = player->GetItemByGuid(itemGuids[i]);

        // prevent sending bag with items (cheat: can be placed in bag after adding equipped empty bag to mail)
        if (!item)
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_MAIL_ATTACHMENT_INVALID);
            return false;
        }

        if (!item->CanBeTraded(true))
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_MAIL_BOUND_ITEM);
            return false;
        }

        if (item->IsBoundAccountWide() && item->IsSoulBound() && player->GetSession()->GetAccountId() != receiverAccountId)
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_NOT_SAME_ACCOUNT);
            return false;
        }

        if (item->GetTemplate()->Flags & ITEM_PROTO_FLAG_CONJURED || item->GetUInt32Value(ITEM_FIELD_EXPIRATION))
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_MAIL_BOUND_ITEM);
            return false;
        }

        if (COD && item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_CANT_SEND_WRAPPED_COD);
            return false;
        }

        if (item->IsNotEmptyBag())
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_DESTROY_NONEMPTY_BAG);
            return false;
        }

        items[i] = item;
    }

    return true;
}

bool ValidateMailCODPayment(Player* player, uint32 mailId, Mail const* mail)
{
    if (player->HasEnoughMoney(uint64(mail->COD)))
        return true;

    player->SendMailResult(mailId, MAIL_ITEM_TAKEN, MAIL_ERR_NOT_ENOUGH_MONEY);
    return false;
}
}

void WorldSession::HandleSendMail(WorldPacket& recvData)
{
    SendMailRequest request;
    if (!ReadSendMailRequest(recvData, request))
    {
        GetPlayer()->SendMailResult(0, MAIL_SEND, MAIL_ERR_TOO_MANY_ATTACHMENTS);
        recvData.rfinish();                         // set to end to avoid warnings spam
        return;
    }

    ObjectGuid mailbox = request.mailbox;
    ObjectGuid* itemGuids = request.itemGuids;
    uint64 money = request.money;
    uint64 COD = request.COD;
    std::string& receiverName = request.receiverName;
    std::string const& subject = request.subject;
    std::string const& body = request.body;
    uint32 unk1 = request.unk1;
    uint32 unk2 = request.unk2;
    uint8 itemCount = request.itemCount;

    // packet read complete, now do check

    Player* player = _player;

    if (!HasMailboxAccess(player, mailbox))
        return;

    if (receiverName.empty())
        return;

    if (player->getLevel() < sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAIL_LEVEL_REQ))
    {
        SendNotification(GetSkyFireString(LANG_MAIL_SENDER_REQ), sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAIL_LEVEL_REQ));
        return;
    }

    uint64 receiverGuid = 0;
    if (normalizePlayerName(receiverName))
        receiverGuid = sObjectMgr->GetPlayerGUIDByName(receiverName);

    if (!receiverGuid)
    {
        SF_LOG_INFO("network", "Player %u is sending mail to %s (GUID: not existed!) with subject %s "
            "and body %s includes %u items, " UI64FMTD " copper and " UI64FMTD " COD copper with unk1 = %u, unk2 = %u",
            player->GetGUIDLow(), receiverName.c_str(), subject.c_str(), body.c_str(),
            itemCount, money, COD, unk1, unk2);
        player->SendMailResult(0, MAIL_SEND, MAIL_ERR_RECIPIENT_NOT_FOUND);
        return;
    }

    SF_LOG_INFO("network", "Player %u is sending mail to %s (GUID: %u) with subject %s and body %s "
        "includes %u items, " UI64FMTD " copper and " UI64FMTD " COD copper with unk1 = %u, unk2 = %u",
        player->GetGUIDLow(), receiverName.c_str(), GUID_LOPART(receiverGuid), subject.c_str(),
        body.c_str(), itemCount, money, COD, unk1, unk2);

    if (player->GetGUID() == receiverGuid)
    {
        player->SendMailResult(0, MAIL_SEND, MAIL_ERR_CANNOT_SEND_TO_SELF);
        return;
    }

    uint32 cost = itemCount ? 30 * itemCount : 30;  // price hardcoded in client

    uint64 reqmoney = cost + money;

    if (!ValidateSendMailCost(player, reqmoney))
        return;

    Player* receiver = ObjectAccessor::FindPlayer(receiverGuid);

    uint32 receiverTeam = 0;
    uint8 mailsCount = 0;                                  //do not allow to send to one player more than 100 mails
    uint8 receiverLevel = 0;
    uint32 receiverAccountId = 0;

    if (receiver)
    {
        receiverTeam = receiver->GetTeam();
        mailsCount = receiver->GetMailSize();
        receiverLevel = receiver->getLevel();
        receiverAccountId = receiver->GetSession()->GetAccountId();
    }
    else
    {
        receiverTeam = sObjectMgr->GetPlayerTeamByGUID(receiverGuid);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MAIL_COUNT);
        stmt->setUInt32(0, GUID_LOPART(receiverGuid));

        PreparedQueryResult result = CharacterDatabase.Query(stmt);
        if (result)
        {
            Field* fields = result->Fetch();
            mailsCount = fields[0].GetUInt64();
        }

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_LEVEL);
        stmt->setUInt32(0, GUID_LOPART(receiverGuid));

        result = CharacterDatabase.Query(stmt);
        if (result)
        {
            Field* fields = result->Fetch();
            receiverLevel = fields[0].GetUInt8();
        }

        receiverAccountId = sObjectMgr->GetPlayerAccountIdByGUID(receiverGuid);
    }

    // do not allow to have more than 100 mails in mailbox.. mails count is in opcode uint8!!! - so max can be 255..
    if (!ValidateReceiverMailboxCapacity(player, mailsCount))
        return;

    // test the receiver's Faction... or all items are account bound
    bool accountBound = AreAttachedItemsAccountBound(player, itemGuids, itemCount);
    if (!ValidateReceiverFaction(player, receiverTeam, accountBound, HasPermission(rbac::RBAC_PERM_TWO_SIDE_INTERACTION_MAIL)))
        return;

    if (receiverLevel < sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAIL_LEVEL_REQ))
    {
        SendNotification(GetSkyFireString(LANG_MAIL_RECEIVER_REQ), sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAIL_LEVEL_REQ));
        return;
    }

    Item* items[MAX_MAIL_ITEMS];

    if (!ValidateSendMailAttachments(player, itemGuids, itemCount, COD, receiverAccountId, items))
        return;

    player->SendMailResult(0, MAIL_SEND, MAIL_OK);

    player->ModifyMoney(-int64(reqmoney));
    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL, cost);

    bool needItemDelay = false;

    MailDraft draft(subject, body);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    if (itemCount > 0 || money > 0)
    {
        bool log = HasPermission(rbac::RBAC_PERM_LOG_GM_TRADE);
        if (itemCount > 0)
        {
            for (uint8 i = 0; i < itemCount; ++i)
            {
                Item* item = items[i];
                if (log)
                {
                    sLog->outCommand(GetAccountId(), "GM %s (GUID: %u) (Account: %u) mail item: %s (Entry: %u Count: %u) "
                        "to player: %s (GUID: %u) (Account: %u)", GetPlayerName().c_str(), GetGuidLow(), GetAccountId(),
                        item->GetTemplate()->Name1.c_str(), item->GetEntry(), item->GetCount(),
                        receiverName.c_str(), GUID_LOPART(receiverGuid), receiverAccountId);
                }

                item->SetNotRefundable(GetPlayer()); // makes the item no longer refundable
                player->MoveItemFromInventory(items[i]->GetBagSlot(), item->GetSlot(), true);

                item->DeleteFromInventoryDB(trans);     // deletes item from character's inventory
                item->SetOwnerGUID(receiverGuid);
                item->SaveToDB(trans);                  // recursive and not have transaction guard into self, item not in inventory and can be save standalone

                draft.AddItem(item);
            }

            // if item send to character at another account, then apply item delivery delay
            needItemDelay = player->GetSession()->GetAccountId() != receiverAccountId;
        }

        if (log && money > 0)
        {
            sLog->outCommand(GetAccountId(), "GM %s (GUID: %u) (Account: %u) mail money: " UI64FMTD " to player: %s (GUID: %u) (Account: %u)",
                GetPlayerName().c_str(), GetGuidLow(), GetAccountId(), money, receiverName.c_str(), GUID_LOPART(receiverGuid), receiverAccountId);
        }
    }

    // If theres is an item, there is a one hour delivery delay if sent to another account's character.
    uint32 deliver_delay = needItemDelay ? sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAIL_DELIVERY_DELAY) : 0;

    // Mail sent between guild members arrives instantly if they have the guild perk "Guild Mail"
    if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
        if (guild->GetLevel() >= 17 && guild->IsMember(receiverGuid))
            deliver_delay = 0;

    // will delete item or place to receiver mail list
    draft
        .AddMoney(money)
        .AddCOD(COD)
        .SendMailTo(trans, MailReceiver(receiver, GUID_LOPART(receiverGuid)), MailSender(player), body.empty() ? MAIL_CHECK_MASK_COPIED : MAIL_CHECK_MASK_HAS_BODY, deliver_delay);

    player->SaveInventoryAndGoldToDB(trans);
    CharacterDatabase.CommitTransaction(trans);
}

//called when mail is read
void WorldSession::HandleMailMarkAsRead(WorldPacket& recvData)
{
    MailboxMailRequest request = ReadMailMarkAsReadRequest(recvData);
    ObjectGuid mailbox = request.mailbox;
    uint32 mailId = request.mailId;

    if (!HasMailboxAccess(_player, mailbox))
        return;

    if (Mail* mail = _player->GetMail(mailId))
    {
        if (_player->unReadMails)
            --_player->unReadMails;

        mail->checked |= MAIL_CHECK_MASK_READ;
        mail->state = MailState::MAIL_STATE_CHANGED;

        _player->m_mailsUpdated = true;
    }
}

//called when client deletes mail
void WorldSession::HandleMailDelete(WorldPacket& recvData)
{
    MailDeleteRequest request = ReadMailDeleteRequest(recvData);
    uint32 mailId = request.mailId;

    if (Mail* mail = _player->GetMail(mailId))
    {
        // delete shouldn't show up for COD mails
        if (!ValidateMailDelete(_player, mailId, mail))
            return;

        mail->state = MailState::MAIL_STATE_DELETED;
    }

    _player->m_mailsUpdated = true;
    _player->SendMailResult(mailId, MAIL_DELETED, MAIL_OK);
}

void WorldSession::HandleMailReturnToSender(WorldPacket& recvData)
{
    MailboxMailRequest request = ReadMailReturnToSenderRequest(recvData);
    ObjectGuid mailbox = request.mailbox;
    uint32 mailId = request.mailId;

    if (!HasMailboxAccess(_player, mailbox))
        return;

    Player* player = _player;
    Mail* m = GetDeliveredMailOrSendResult(player, mailId, MAIL_RETURNED_TO_SENDER);
    if (!m)
        return;
    //we can return mail now, so firstly delete the old one
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_BY_ID);
    stmt->setUInt32(0, mailId);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_ITEM_BY_ID);
    stmt->setUInt32(0, mailId);
    trans->Append(stmt);

    player->RemoveMail(mailId);

    // only return mail if the player exists (and delete if not existing)
    if (m->messageType == MAIL_NORMAL && m->sender)
    {
        MailDraft draft(m->subject, m->body);
        if (m->mailTemplateId)
            draft = MailDraft(m->mailTemplateId, false);     // items already included

        if (m->HasItems())
        {
            for (MailItemInfoVec::iterator itr2 = m->items.begin(); itr2 != m->items.end(); ++itr2)
            {
                if (Item* const item = player->GetMItem(itr2->item_guid))
                    draft.AddItem(item);
                player->RemoveMItem(itr2->item_guid);
            }
        }
        draft.AddMoney(m->money).SendReturnToSender(GetAccountId(), m->receiver, m->sender, trans);
    }

    CharacterDatabase.CommitTransaction(trans);

    delete m;                                               //we can deallocate old mail
    player->SendMailResult(mailId, MAIL_RETURNED_TO_SENDER, MAIL_OK);
}

//called when player takes item attached in mail
void WorldSession::HandleMailTakeItem(WorldPacket& recvData)
{
    MailTakeItemRequest request = ReadMailTakeItemRequest(recvData);
    ObjectGuid mailbox = request.mailbox;
    uint32 mailId = request.mailId;
    uint32 itemId = request.itemId;

    if (!HasMailboxAccess(_player, mailbox))
        return;

    Player* player = _player;

    Mail* m = GetDeliveredMailOrSendResult(player, mailId, MAIL_ITEM_TAKEN);
    if (!m)
        return;

    // prevent cheating with skip client money check
    if (!ValidateMailCODPayment(player, mailId, m))
        return;

    // GetMItem finds the items of all the player's mails, so check that this mail holds the item
    Item* it = nullptr;
    for (MailItemInfo const& info : m->items)
        if (info.item_guid == itemId)
            it = player->GetMItem(itemId);

    ItemPosCountVec dest;
    uint8 msg = _player->CanStoreItem(NULL_BAG, NULL_SLOT, dest, it, false);
    if (msg == EQUIP_ERR_OK)
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        m->RemoveItem(itemId);
        m->removedItems.push_back(itemId);

        if (m->COD > 0)                                     //if there is COD, take COD money from player and send them to sender by mail
        {
            uint64 sender_guid = MAKE_NEW_GUID(m->sender, 0, HIGHGUID_PLAYER);
            Player* receiver = ObjectAccessor::FindPlayer(sender_guid);

            uint32 sender_accId = 0;

            if (HasPermission(rbac::RBAC_PERM_LOG_GM_TRADE))
            {
                std::string sender_name;
                if (receiver)
                {
                    sender_accId = receiver->GetSession()->GetAccountId();
                    sender_name = receiver->GetName();
                }
                else
                {
                    // can be calculated early
                    sender_accId = sObjectMgr->GetPlayerAccountIdByGUID(sender_guid);

                    if (!sObjectMgr->GetPlayerNameByGUID(sender_guid, sender_name))
                        sender_name = sObjectMgr->GetSkyFireStringForDBCLocale(LANG_UNKNOWN);
                }
                sLog->outCommand(GetAccountId(), "GM %s (Account: %u) receiver mail item: %s (Entry: %u Count: %u) and send COD money: " UI64FMTD " to player: %s (Account: %u)",
                    GetPlayerName().c_str(), GetAccountId(), it->GetTemplate()->Name1.c_str(), it->GetEntry(), it->GetCount(), m->COD, sender_name.c_str(), sender_accId);
            }
            else if (!receiver)
                sender_accId = sObjectMgr->GetPlayerAccountIdByGUID(sender_guid);

            // check player existence
            if (receiver || sender_accId)
            {
                MailDraft(m->subject, "")
                    .AddMoney(m->COD)
                    .SendMailTo(trans, MailReceiver(receiver, m->sender), MailSender(MAIL_NORMAL, m->receiver), MAIL_CHECK_MASK_COD_PAYMENT);
            }

            player->ModifyMoney(-int32(m->COD));
        }
        m->COD = 0;
        m->state = MailState::MAIL_STATE_CHANGED;
        player->m_mailsUpdated = true;
        player->RemoveMItem(it->GetGUIDLow());

        uint32 count = it->GetCount();                      // save counts before store and possible merge with deleting
        it->SetState(ITEM_UNCHANGED);                       // need to set this state, otherwise item cannot be removed later, if neccessary
        player->MoveItemToInventory(dest, it, true);

        player->SaveInventoryAndGoldToDB(trans);
        player->_SaveMail(trans);
        CharacterDatabase.CommitTransaction(trans);

        player->SendMailResult(mailId, MAIL_ITEM_TAKEN, MAIL_OK, 0, itemId, count);
    }
    else
        player->SendMailResult(mailId, MAIL_ITEM_TAKEN, MAIL_ERR_EQUIP_ERROR, msg);
}

void WorldSession::HandleMailTakeMoney(WorldPacket& recvData)
{
    MailTakeMoneyRequest request = ReadMailTakeMoneyRequest(recvData);
    ObjectGuid mailbox = request.mailbox;
    uint64 money = request.money;
    uint32 mailId = request.mailId;

    if (!HasMailboxAccess(_player, mailbox))
        return;

    Player* player = _player;

    Mail* m = GetMoneyMailOrSendResult(player, mailId, money);
    if (!m)
        return;

    if (!player->ModifyMoney(m->money, false))
    {
        player->SendMailResult(mailId, MAIL_MONEY_TAKEN, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_TOO_MUCH_GOLD);
        return;
    }

    m->money = 0;
    m->state = MailState::MAIL_STATE_CHANGED;
    player->m_mailsUpdated = true;

    player->SendMailResult(mailId, MAIL_MONEY_TAKEN, MAIL_OK);

    // save money and mail to prevent cheating
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    player->SaveGoldToDB(trans);
    player->_SaveMail(trans);
    CharacterDatabase.CommitTransaction(trans);
}

//called when player lists his received mails
void WorldSession::HandleGetMailList(WorldPacket& recvData)
{
    MailboxRequest request = ReadGetMailListRequest(recvData);
    ObjectGuid mailbox = request.mailbox;

    if (!HasMailboxAccess(_player, mailbox))
        return;

    Player* player = _player;

    //load players mails, and mailed items
    if (!player->m_mailsLoaded)
        player->_LoadMail();

    // client can't work with packets > max int16 value
    const uint32 maxPacketSize = 32767;

    uint32 mailCount = 0;
    uint32 realCount = 0;                               // true mail count (includes any skipped mail)
    time_t cur_time = time(NULL);
    ByteBuffer mailData;

    WorldPacket data(SMSG_MAIL_LIST_RESULT, 200);       // guess size
    data << uint32(0);                                  // placeholder

    size_t mailCountPos = data.bitwpos();
    data.WriteBits(0, 18);                              // placeholder

    for (PlayerMails::iterator itr = player->GetMailBegin(); itr != player->GetMailEnd(); ++itr)
    {
        Mail* mail = *itr;

        // Only first 50 mails are displayed
        if (mailCount >= 50)
        {
            realCount += 1;
            continue;
        }

        // skip deleted or not delivered (deliver delay not expired) mails
        if (mail->state == MailState::MAIL_STATE_DELETED || cur_time < mail->deliver_time)
            continue;

        // skip mail with more than MAX_MAIL_ITEMS items (should not occur)
        uint8 itemCount = mail->items.size();
        if (itemCount > MAX_MAIL_ITEMS)
        {
            realCount += 1;
            continue;
        }

        // skip mail if the packet has become too large (should not occur)
        size_t nextMailSize = 6 + 1 + 8 + itemCount * (4 + 4 + 4 + 4 + 4 + MAX_INSPECTED_ENCHANTMENT_SLOT * (4 + 4 + 4) +
            4 + 4 + 4 + 4 + 1 + 4) + mail->body.size() + mail->subject.size() + 4 + 4 + 8 + 4 + 8 + 4 + 4 + 1 + 4;

        if (data.wpos() + nextMailSize > maxPacketSize)
        {
            realCount += 1;
            continue;
        }

        data.WriteBit(mail->messageType != MAIL_NORMAL ? 1 : 0);
        data.WriteBits(mail->subject.size(), 8);
        data.WriteBits(mail->body.size(), 13);
        data.WriteBit(0);
        data.WriteBit(0);

        size_t itemCountPos = data.bitwpos();
        data.WriteBits(0, 17);                          // placeholder

        data.WriteBit(1);                               // has guid

        ObjectGuid guid = mail->messageType == MAIL_NORMAL ? MAKE_NEW_GUID(mail->sender, 0, HIGHGUID_PLAYER) : 0;
        data.WriteBit(guid[2]);
        data.WriteBit(guid[6]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[5]);
        data.WriteBit(guid[3]);
        data.WriteBit(guid[1]);
        data.WriteBit(guid[4]);

        uint8 trueItemCount = 0;
        for (uint8 i = 0; i < itemCount; i++)
        {
            Item* item = player->GetMItem(mail->items[i].item_guid);
            if (!item)
                continue;

            data.WriteBit(0);

            mailData << uint32(item->GetGUIDLow());
            mailData << uint32(4);                      // unknown
            mailData << uint32(item->GetSpellCharges());
            mailData << uint32(item->GetUInt32Value(ITEM_FIELD_DURABILITY));
            mailData << uint32(0);                      // unknown

            for (uint8 j = 0; j < MAX_INSPECTED_ENCHANTMENT_SLOT; j++)
            {
                mailData << uint32(item->GetEnchantmentCharges((EnchantmentSlot)j));
                mailData << uint32(item->GetEnchantmentDuration((EnchantmentSlot)j));
                mailData << uint32(item->GetEnchantmentId((EnchantmentSlot)j));
            }

            mailData << uint32(item->GetItemSuffixFactor());
            mailData << int32(item->GetItemRandomPropertyId());
            mailData << uint32(item->GetUInt32Value(ITEM_FIELD_MAX_DURABILITY));
            mailData << uint32(item->GetCount());
            mailData << uint8(i);
            mailData << uint32(item->GetEntry());

            trueItemCount++;
        }

        data.PutBits(itemCountPos, trueItemCount, 17);

        mailData.WriteString(mail->body);
        mailData << uint32(mail->messageID);
        mailData.WriteByteSeq(guid[4]);
        mailData.WriteByteSeq(guid[0]);
        mailData.WriteByteSeq(guid[5]);
        mailData.WriteByteSeq(guid[3]);
        mailData.WriteByteSeq(guid[1]);
        mailData.WriteByteSeq(guid[7]);
        mailData.WriteByteSeq(guid[2]);
        mailData.WriteByteSeq(guid[6]);
        mailData << uint32(mail->mailTemplateId);
        mailData << uint64(mail->COD);
        mailData.WriteString(mail->subject);
        mailData << uint32(mail->stationery);
        mailData << float(float(mail->expire_time - time(NULL)) / float(DAY));
        mailData << uint64(mail->money);
        mailData << uint32(mail->checked);

        if (mail->messageType != MAIL_NORMAL)
            mailData << uint32(mail->sender);

        mailData << uint8(mail->messageType);
        mailData << uint32(0);                          // unknown

        realCount++;
        mailCount++;
    }

    data.FlushBits();
    data.append(mailData);

    data.put<uint32>(0, realCount);
    data.PutBits(mailCountPos, mailCount, 18);

    SendPacket(&data);

    // recalculate m_nextMailDelivereTime and unReadMails
    _player->UpdateNextMailTimeAndUnreads();
}

//used when player copies mail body to his inventory
void WorldSession::HandleMailCreateTextItem(WorldPacket& recvData)
{
    MailboxMailRequest request = ReadMailCreateTextItemRequest(recvData);
    ObjectGuid mailbox = request.mailbox;
    uint32 mailId = request.mailId;

    if (!HasMailboxAccess(_player, mailbox))
        return;

    Player* player = _player;

    Mail* m = GetTextMailOrSendResult(player, mailId);
    if (!m)
        return;

    Item* bodyItem = new Item;                              // This is not bag and then can be used new Item.
    if (!bodyItem->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), MAIL_BODY_ITEM_TEMPLATE, player))
    {
        delete bodyItem;
        return;
    }

    // in mail template case we need create new item text
    if (m->mailTemplateId)
    {
        MailTemplateEntry const* mailTemplateEntry = sMailTemplateStore.LookupEntry(m->mailTemplateId);
        if (!mailTemplateEntry)
        {
            player->SendMailResult(mailId, MAIL_MADE_PERMANENT, MAIL_ERR_INTERNAL_ERROR);
            return;
        }

        bodyItem->SetText(mailTemplateEntry->content);
    }
    else
        bodyItem->SetText(m->body);

    bodyItem->SetUInt32Value(ITEM_FIELD_CREATOR, m->sender);
    bodyItem->SetFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_MAIL_TEXT_MASK);

    SF_LOG_INFO("network", "HandleMailCreateTextItem mailid=%u", mailId);

    ItemPosCountVec dest;
    uint8 msg = _player->CanStoreItem(NULL_BAG, NULL_SLOT, dest, bodyItem, false);
    if (msg == EQUIP_ERR_OK)
    {
        m->checked = m->checked | MAIL_CHECK_MASK_COPIED;
        m->state = MailState::MAIL_STATE_CHANGED;
        player->m_mailsUpdated = true;

        player->StoreItem(dest, bodyItem, true);
        player->SendMailResult(mailId, MAIL_MADE_PERMANENT, MAIL_OK);
    }
    else
    {
        player->SendMailResult(mailId, MAIL_MADE_PERMANENT, MAIL_ERR_EQUIP_ERROR, msg);
        delete bodyItem;
    }
}

/// @todo Fix me! ... this void has probably bad condition, but good data are sent
void WorldSession::HandleQueryNextMailTime(WorldPacket& /*recvData*/)
{
    WorldPacket data(SMSG_MAIL_QUERY_NEXT_TIME_RESULT);

    if (!_player->m_mailsLoaded)
        _player->_LoadMail();

    if (_player->unReadMails > 0)
    {
        ByteBuffer dataBuffer;
        uint8 count = 0;
        time_t now = time(NULL);
        bool hasVirtualRealmAddress = false, hasNativeRealmAddress = false;

        size_t pos = data.bitwpos();
        data.WriteBits(count, 20);

        for (PlayerMails::iterator itr = _player->GetMailBegin(); itr != _player->GetMailEnd(); ++itr)
        {
            Mail* m = (*itr);
            // must be not checked yet
            if (m->checked & MAIL_CHECK_MASK_READ)
                continue;

            // and already delivered
            if (now < m->deliver_time)
                continue;

            ObjectGuid senderGuid = (m->messageType == MAIL_NORMAL) ? MAKE_NEW_GUID(m->sender, 0, HIGHGUID_PLAYER) : 0;

            data.WriteBit(senderGuid[3]);
            data.WriteBit(hasVirtualRealmAddress);
            data.WriteBit(senderGuid[2]);
            data.WriteBit(hasNativeRealmAddress);
            data.WriteBit(senderGuid[6]);
            data.WriteBit(senderGuid[1]);
            data.WriteBit(senderGuid[4]);
            data.WriteBit(senderGuid[0]);
            data.WriteBit(senderGuid[5]);
            data.WriteBit(senderGuid[7]);

            dataBuffer << uint32(m->messageType != MAIL_NORMAL ? m->sender : 0);  // non-player entries
            dataBuffer.WriteByteSeq(senderGuid[5]);
            dataBuffer.WriteByteSeq(senderGuid[4]);
            dataBuffer.WriteByteSeq(senderGuid[6]);
            dataBuffer.WriteByteSeq(senderGuid[1]);
            dataBuffer << uint8(m->messageType);
            dataBuffer.WriteByteSeq(senderGuid[0]);
            dataBuffer << float(m->deliver_time - now);
            if (hasNativeRealmAddress)
                dataBuffer << uint32(realmID);
            dataBuffer << uint32(m->stationery);
            dataBuffer.WriteByteSeq(senderGuid[3]);
            dataBuffer.WriteByteSeq(senderGuid[2]);
            if (hasVirtualRealmAddress)
                dataBuffer << uint32(realmID);
            dataBuffer.WriteByteSeq(senderGuid[7]);

            count++;
            if (count == 3)                                  // do not display more than 3 mails
                break;
        }

        data.FlushBits();
        data.PutBits(pos, count, 20);
        data.append(dataBuffer);

        data << float(0);
    }
    else
    {
        data.WriteBits(0, 20);
        data.FlushBits();
        data << float(-1);
    }

    SendPacket(&data);
}
