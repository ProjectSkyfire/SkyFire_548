/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* mod-prabowow: welcome mail for every new character.
*
* PlayerScript::OnCreate runs after the character row is saved, so the mail
* is written straight to the characters database and shows up in the mailbox
* on first login. Default gift: item 23162 (36-slot bag), configurable.
*/

#include "PraboWoWConfig.h"

#include "DatabaseEnv.h"
#include "Item.h"
#include "ItemPrototype.h"
#include "Log.h"
#include "Mail.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"

#include <algorithm>
#include <string>

class prabowow_starter_mail : public PlayerScript
{
public:
    prabowow_starter_mail() : PlayerScript("prabowow_starter_mail") { }

    void OnCreate(Player* player) override
    {
        if (!PraboWoW::GetBool("PraboWoW.StarterMail.Enable", true))
            return;

        uint32 itemId = uint32(std::max<int32>(PraboWoW::GetInt("PraboWoW.StarterMail.ItemId", 23162), 0));
        uint32 count = uint32(std::max<int32>(PraboWoW::GetInt("PraboWoW.StarterMail.ItemCount", 1), 1));
        if (!itemId)
            return;

        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);
        if (!proto)
        {
            SF_LOG_ERROR(PraboWoW::LOG, "[mod-prabowow] Starter mail item %u does not exist in Item-sparse.db2/item_template; no mail sent to %s.", itemId, player->GetName().c_str());
            return;
        }

        std::string subject = PraboWoW::GetString("PraboWoW.StarterMail.Subject", "Welcome to PraboWoW");
        std::string body = PraboWoW::GetString("PraboWoW.StarterMail.Body",
            "Welcome to PraboWoW! Here is a little something to get you started.\n\n"
            "Handy commands: .xp rate <1-5> sets your XP rate, .chat <message> talks to the whole realm.\n"
            "Grey loot is sold automatically and every flight path is already known.");

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        MailDraft draft(subject, body);

        uint32 remaining = count;
        uint32 stacks = 0;
        while (remaining > 0 && stacks < MAX_MAIL_ITEMS)
        {
            uint32 stack = std::min(remaining, proto->GetMaxStackSize());
            Item* item = Item::CreateItem(itemId, stack, player);
            if (!item)
                break;

            item->SaveToDB(trans); // must exist in item_instance before the mail references it
            draft.AddItem(item);
            remaining -= stack;
            ++stacks;
        }

        if (!stacks)
        {
            SF_LOG_ERROR(PraboWoW::LOG, "[mod-prabowow] Could not create starter item %u for %s.", itemId, player->GetName().c_str());
            return;
        }

        // Sender shown in the client: the heirloom vendor NPC when it exists, otherwise a GM letter.
        uint32 senderEntry = uint32(std::max<int32>(PraboWoW::GetInt("PraboWoW.StarterMail.SenderEntry", 900001), 0));
        if (senderEntry && sObjectMgr->GetCreatureTemplate(senderEntry))
            draft.SendMailTo(trans, MailReceiver(player->GetGUIDLow()), MailSender(MAIL_CREATURE, senderEntry));
        else
            draft.SendMailTo(trans, MailReceiver(player->GetGUIDLow()), MailSender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM));

        CharacterDatabase.CommitTransaction(trans);

        SF_LOG_INFO(PraboWoW::LOG, "[mod-prabowow] Starter mail with item %u x%u sent to new character %s (guid %u).",
            itemId, count - remaining, player->GetName().c_str(), player->GetGUIDLow());
    }
};

void AddSC_prabowow_starter_mail()
{
    new prabowow_starter_mail();
}
