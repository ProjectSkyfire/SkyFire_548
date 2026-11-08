/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
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

#include "end_time.h"
#include "GameObjectAI.h"

enum Spells
{
    SPELL_TELEPORT_TO_START                 = 102564, // Start
    SPELL_TELEPORT_TO_RUBY_DRAGONSHIRE      = 102579, // Sylvanas
    SPELL_TELEPORT_TO_EMERALD_DRAGONSHIRE   = 104761, // Tyrande
    SPELL_TELEPORT_TO_BLUE_DRAGONSHIRE      = 102126, // Jaina
    SPELL_TELEPORT_TO_OBSIDIAN_DRAGONSHIRE  = 103868, // Baine
    SPELL_TELEPORT_TO_BRONZE_DRAGONSHIRE    = 104764, // Murozond
};

enum InstanceTeleporter
{
    START_TELEPORT          = 1,
    JAINA_TELEPORT          = 2,
    SYLVANAS_TELEPORT       = 3,
    TYRANDE_TELEPORT        = 4,
    BAINE_TELEPORT          = 5,
    MUROZOND_TELEPORT       = 6,
};

class go_end_time_teleport : public GameObjectScript
{
    public:
        go_end_time_teleport() : GameObjectScript("go_end_time_teleport") { }

        bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
        {

            if (player->IsInCombat())
                return true;

            if (InstanceScript* instance = go->GetInstanceScript())
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Entryway of Time.", GOSSIP_SENDER_MAIN, START_TELEPORT);

                if (player->IsGameMaster())
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Emerald Dragonshrine.", GOSSIP_SENDER_MAIN, TYRANDE_TELEPORT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Azure Dragonshrine.", GOSSIP_SENDER_MAIN, JAINA_TELEPORT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Obsidian Dragonshrine.", GOSSIP_SENDER_MAIN, BAINE_TELEPORT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Ruby Dragonshrine.", GOSSIP_SENDER_MAIN, SYLVANAS_TELEPORT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bronze Dragonshrine.", GOSSIP_SENDER_MAIN, MUROZOND_TELEPORT);
                }
                else
                {
                    std::list<uint32> echo_list;
                    uint32 echo1 = instance->GetData(DATA_ECHO_1);
                    uint32 echo2 = instance->GetData(DATA_ECHO_2);

                    switch (echo1)
                    {
                        case DATA_ECHO_OF_JAINA:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Azure Dragonshrine.", GOSSIP_SENDER_MAIN, JAINA_TELEPORT);
                            break;
                        case DATA_ECHO_OF_BAINE:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Obsidian Dragonshrine.", GOSSIP_SENDER_MAIN, BAINE_TELEPORT);
                            break;
                        case DATA_ECHO_OF_TYRANDE:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Emerald Dragonshrine.", GOSSIP_SENDER_MAIN, TYRANDE_TELEPORT);
                            break;
                        case DATA_ECHO_OF_SYLVANAS:
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Ruby Dragonshrine.", GOSSIP_SENDER_MAIN, SYLVANAS_TELEPORT);
                            break;
                    }

                    if (instance->GetData(DATA_FIRST_ENCOUNTER) == DONE)
                    {
                        switch (echo2)
                        {
                            case DATA_ECHO_OF_JAINA:
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Azure Dragonshrine.", GOSSIP_SENDER_MAIN, JAINA_TELEPORT);
                                break;
                            case DATA_ECHO_OF_BAINE:
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Obsidian Dragonshrine.", GOSSIP_SENDER_MAIN, BAINE_TELEPORT);
                                break;
                            case DATA_ECHO_OF_TYRANDE:
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Emerald Dragonshrine.", GOSSIP_SENDER_MAIN, TYRANDE_TELEPORT);
                                break;
                            case DATA_ECHO_OF_SYLVANAS:
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Ruby Dragonshrine.", GOSSIP_SENDER_MAIN, SYLVANAS_TELEPORT);
                                break;
                        }
                    }

                    if (instance->GetData(DATA_SECOND_ENCOUNTER) == DONE)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bronze Dragonshrine.", GOSSIP_SENDER_MAIN, MUROZOND_TELEPORT);
                }
            }

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            //player->PlayerTalkClass->ClearMenus();
            if (player->IsInCombat())
                return true;

            InstanceScript* instance = player->GetInstanceScript();
            if (!instance)
                return true;

            switch (action)
            {
                case START_TELEPORT:
                    player->CastSpell(player, SPELL_TELEPORT_TO_START, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case JAINA_TELEPORT:
                    instance->SetData(DATA_JAINA_EVENT, IN_PROGRESS);
                    if (instance->GetData(DATA_NOZDORMU_3) != DONE)
                        instance->SetData(DATA_NOZDORMU_3, IN_PROGRESS);
                    player->CastSpell(player, SPELL_TELEPORT_TO_BLUE_DRAGONSHIRE, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case SYLVANAS_TELEPORT:
                    if (instance->GetData(DATA_NOZDORMU_4) != DONE)
                        instance->SetData(DATA_NOZDORMU_4, IN_PROGRESS);
                    player->CastSpell(player, SPELL_TELEPORT_TO_RUBY_DRAGONSHIRE, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case TYRANDE_TELEPORT:
                    if (instance->GetData(DATA_NOZDORMU_1) != DONE)
                        instance->SetData(DATA_NOZDORMU_1, IN_PROGRESS);
                    player->CastSpell(player, SPELL_TELEPORT_TO_EMERALD_DRAGONSHIRE, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case BAINE_TELEPORT:
                    if (instance->GetData(DATA_NOZDORMU_2) != DONE)
                        instance->SetData(DATA_NOZDORMU_2, IN_PROGRESS);
                    player->CastSpell(player, SPELL_TELEPORT_TO_OBSIDIAN_DRAGONSHIRE, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case MUROZOND_TELEPORT:
                    player->CastSpell(player, SPELL_TELEPORT_TO_BRONZE_DRAGONSHIRE, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
            }

            return true;
        }
};

void AddSC_end_time_teleport()
{
    new go_end_time_teleport();
}
