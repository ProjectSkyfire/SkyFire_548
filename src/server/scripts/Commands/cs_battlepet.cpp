/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BattlePet.h"
#include "BattlePetMgr.h"
#include "Chat.h"
#include "Creature.h"
#include "Player.h"
#include "ScriptMgr.h"

#include <string>
#include <vector>

namespace
{
    std::string TrimCommandText(std::string text)
    {
        std::string::size_type first = text.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
            return "";

        std::string::size_type last = text.find_last_not_of(" \t\r\n");
        text = text.substr(first, last - first + 1);

        if (text.size() >= 2 && text[0] == '"' && text[text.size() - 1] == '"')
            text = text.substr(1, text.size() - 2);

        first = text.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
            return "";

        last = text.find_last_not_of(" \t\r\n");
        return text.substr(first, last - first + 1);
    }

    std::string GetBattlePetCommandName(char const* args)
    {
        return TrimCommandText(args ? args : "");
    }

    bool IsAllArgument(std::string const& text)
    {
        return !text.empty() && !stricmp(text.c_str(), "all");
    }

    Player* GetBattlePetCommandTarget(ChatHandler* handler)
    {
        Player* target = handler->getSelectedPlayer();
        if (!target)
            target = handler->GetSession()->GetPlayer();

        if (!target)
        {
            handler->PSendSysMessage("No player selected.");
            handler->SetSentErrorMessage(true);
            return NULL;
        }

        if (handler->HasLowerSecurity(target, 0))
            return NULL;

        return target;
    }

    std::string GetBattlePetDisplayName(BattlePetMgr* battlePetMgr, BattlePet* battlePet)
    {
        if (!battlePet)
            return "<unknown>";

        std::string nickname = battlePet->GetNickname();
        if (!nickname.empty())
            return nickname;

        std::string speciesName = battlePetMgr->GetBattlePetSpeciesName(battlePet);
        return speciesName.empty() ? std::string("<unknown>") : speciesName;
    }

    void SendBattlePetDetails(ChatHandler* handler, BattlePetMgr* battlePetMgr, BattlePet* battlePet)
    {
        std::string displayName = GetBattlePetDisplayName(battlePetMgr, battlePet);
        std::string speciesName = battlePetMgr->GetBattlePetSpeciesName(battlePet);

        handler->PSendSysMessage("Battle pet: %s", displayName.c_str());
        handler->PSendSysMessage("  ID: " UI64FMTD " Species: %u (%s)",
            uint64(battlePet->GetId()), uint32(battlePet->GetSpecies()),
            speciesName.empty() ? "unknown" : speciesName.c_str());
        handler->PSendSysMessage("  Level: %u XP: %u/%u Health: %u/%u",
            uint32(battlePet->GetLevel()), uint32(battlePet->GetXp()),
            uint32(BattlePetExperienceForNextLevel(battlePet->GetLevel())),
            uint32(battlePet->GetCurrentHealth()), uint32(battlePet->GetMaxHealth()));
        handler->PSendSysMessage("  Power: %u Speed: %u Quality: %u Breed: %u Flags: %u",
            uint32(battlePet->GetPower()), uint32(battlePet->GetSpeed()),
            uint32(battlePet->GetQuality()), uint32(battlePet->GetBreed()),
            uint32(battlePet->GetFlags()));
    }

    BattlePet* GetUniqueBattlePetByName(ChatHandler* handler, BattlePetMgr* battlePetMgr, std::string const& name)
    {
        if (name.empty())
        {
            handler->PSendSysMessage("Usage: .battlepet levelup \"name\" or .battlepet heal \"name\"");
            handler->SetSentErrorMessage(true);
            return NULL;
        }

        std::vector<BattlePet*> matches = battlePetMgr->FindBattlePetNameMatches(name);
        if (matches.empty())
        {
            handler->PSendSysMessage("No battle pet named '%s' was found.", name.c_str());
            handler->SetSentErrorMessage(true);
            return NULL;
        }

        if (matches.size() > 1)
        {
            handler->PSendSysMessage("Multiple battle pets named '%s' were found. Rename one or use a unique species name.", name.c_str());
            for (std::vector<BattlePet*>::const_iterator itr = matches.begin(); itr != matches.end(); ++itr)
                handler->PSendSysMessage("  " UI64FMTD " - %s level %u",
                    uint64((*itr)->GetId()), GetBattlePetDisplayName(battlePetMgr, *itr).c_str(),
                    uint32((*itr)->GetLevel()));

            handler->SetSentErrorMessage(true);
            return NULL;
        }

        return matches.front();
    }
}

class battlepet_commandscript : public CommandScript
{
public:
    battlepet_commandscript() : CommandScript("battlepet_commandscript") { }

    std::vector<ChatCommand> GetCommands() const OVERRIDE
    {
        static std::vector<ChatCommand> battlePetCommandTable =
        {
            { "levelup", rbac::RBAC_PERM_COMMAND_BATTLEPET_LEVELUP, false, &HandleBattlePetLevelupCommand, "", },
            { "heal",    rbac::RBAC_PERM_COMMAND_BATTLEPET_HEAL,    false, &HandleBattlePetHealCommand,    "", },
            { "debug",   rbac::RBAC_PERM_COMMAND_BATTLEPET_DEBUG,   false, &HandleBattlePetDebugCommand,   "", },
            { "npc",     rbac::RBAC_PERM_COMMAND_BATTLEPET_NPC,     false, &HandleBattlePetNpcCommand,     "", },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "battlepet", rbac::RBAC_PERM_COMMAND_BATTLEPET, false, NULL, "", battlePetCommandTable },
        };
        return commandTable;
    }

    static bool HandleBattlePetLevelupCommand(ChatHandler* handler, char const* args)
    {
        Player* target = GetBattlePetCommandTarget(handler);
        if (!target)
            return false;

        BattlePetMgr* battlePetMgr = target->GetBattlePetMgr();
        BattlePet* battlePet = GetUniqueBattlePetByName(handler, battlePetMgr, GetBattlePetCommandName(args));
        if (!battlePet)
            return false;

        if (battlePet->GetLevel() >= BATTLE_PET_MAX_LEVEL)
        {
            handler->PSendSysMessage("%s is already max level.", GetBattlePetDisplayName(battlePetMgr, battlePet).c_str());
            return true;
        }

        uint8 const oldLevel = battlePet->GetLevel();
        battlePet->SetLevel(oldLevel + 1);
        battlePet->SetXp(0);
        battlePetMgr->UpdateBattlePetLevelAchievement(*battlePet, oldLevel, battlePet->GetLevel());
        battlePetMgr->SendBattlePetUpdate(battlePet, true);
        battlePetMgr->SaveToDb();

        handler->PSendSysMessage("Leveled %s from %u to %u for %s.",
            GetBattlePetDisplayName(battlePetMgr, battlePet).c_str(), uint32(oldLevel),
            uint32(battlePet->GetLevel()), target->GetName().c_str());
        return true;
    }

    static bool HandleBattlePetHealCommand(ChatHandler* handler, char const* args)
    {
        Player* target = GetBattlePetCommandTarget(handler);
        if (!target)
            return false;

        BattlePetMgr* battlePetMgr = target->GetBattlePetMgr();
        std::string name = GetBattlePetCommandName(args);
        if (IsAllArgument(name))
        {
            battlePetMgr->HealBattlePets(100);
            battlePetMgr->SaveToDb();
            handler->PSendSysMessage("Healed all battle pets for %s.", target->GetName().c_str());
            return true;
        }

        BattlePet* battlePet = GetUniqueBattlePetByName(handler, battlePetMgr, name);
        if (!battlePet)
            return false;

        battlePet->SetCurrentHealth(battlePet->GetMaxHealth());
        battlePetMgr->SendBattlePetUpdate(battlePet, false);
        battlePetMgr->SaveToDb();

        handler->PSendSysMessage("Healed %s for %s.",
            GetBattlePetDisplayName(battlePetMgr, battlePet).c_str(), target->GetName().c_str());
        return true;
    }

    // .battlepet npc <entry>: fights the nearest living creature with that entry as a tamer.
    static bool HandleBattlePetNpcCommand(ChatHandler* handler, char const* args)
    {
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : NULL;
        uint32 const entry = (args && *args) ? uint32(atoi(args)) : 0;
        Creature* tamer = (player && entry) ? player->FindNearestCreature(entry, 50.0f) : NULL;
        if (!tamer)
        {
            handler->SendSysMessage("Usage: .battlepet npc <creature entry>, the creature must be within 50 yards.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string result;
        bool const ok = BattlePetStartNpcBattle(*player, *tamer, result);
        handler->PSendSysMessage("%s", result.c_str());
        handler->SetSentErrorMessage(!ok);
        return ok;
    }

    static bool HandleBattlePetDebugCommand(ChatHandler* handler, char const* args)
    {
        Player* target = GetBattlePetCommandTarget(handler);
        if (!target)
            return false;

        BattlePetMgr* battlePetMgr = target->GetBattlePetMgr();
        handler->PSendSysMessage("Battle pet debug for %s:", target->GetName().c_str());
        handler->PSendSysMessage("  Collection: %u pets", battlePetMgr->GetBattlePetCount());
        handler->PSendSysMessage("  Loadout flags: %u Trap ability: %u Active battle: %s PvP queued: %s",
            uint32(battlePetMgr->GetLoadoutFlags()), battlePetMgr->GetTrapAbility(),
            battlePetMgr->HasActivePetBattle() ? "yes" : "no",
            battlePetMgr->IsPetBattlePvpQueued() ? "yes" : "no");
        handler->PSendSysMessage("  Current summon: " UI64FMTD, uint64(battlePetMgr->GetCurrentSummonId()));

        if (battlePetMgr->HasActivePetBattle())
        {
            ActivePetBattle const& activeBattle = battlePetMgr->GetActivePetBattle();
            BattlePetAchievementContext const achievementContext =
                battlePetMgr->BuildActivePetBattleAchievementContext(
                    activeBattle.EnemySpecies, activeBattle.EnemyQuality,
                    activeBattle.Winner == PET_BATTLE_WINNER_ALLY,
                    activeBattle.Captured);
            handler->PSendSysMessage("  Active source: %s Round: %u Winner: %u",
                BattlePetAchievementSourceName(activeBattle.GetAchievementSource()),
                activeBattle.RoundID, uint32(activeBattle.Winner));
            handler->PSendSysMessage("  Enemy species: %u quality: %u health: %u/%u payload: %u",
                uint32(activeBattle.EnemySpecies), uint32(activeBattle.EnemyQuality),
                activeBattle.EnemyHealth, activeBattle.EnemyMaxHealth,
                achievementContext.Payload());

            BattlePet* frontPet = battlePetMgr->GetBattlePet(activeBattle.AllyPetID);
            if (frontPet)
                handler->PSendSysMessage("  XP preview: %u from enemy level %u",
                    uint32(BattlePetExperienceReward(frontPet->GetLevel(), activeBattle.EnemyLevel)),
                    uint32(activeBattle.EnemyLevel));
        }

        for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
        {
            if (!battlePetMgr->HasLoadoutSlot(slot))
            {
                handler->PSendSysMessage("  Slot %u: locked", uint32(slot + 1));
                continue;
            }

            BattlePet* battlePet = battlePetMgr->GetBattlePet(battlePetMgr->GetLoadoutSlot(slot));
            if (!battlePet)
            {
                handler->PSendSysMessage("  Slot %u: empty", uint32(slot + 1));
                continue;
            }

            handler->PSendSysMessage("  Slot %u: %s level %u health %u/%u",
                uint32(slot + 1), GetBattlePetDisplayName(battlePetMgr, battlePet).c_str(),
                uint32(battlePet->GetLevel()), uint32(battlePet->GetCurrentHealth()),
                uint32(battlePet->GetMaxHealth()));
        }

        std::string name = GetBattlePetCommandName(args);
        if (!name.empty())
        {
            BattlePet* battlePet = GetUniqueBattlePetByName(handler, battlePetMgr, name);
            if (!battlePet)
                return false;

            SendBattlePetDetails(handler, battlePetMgr, battlePet);
        }

        return true;
    }
};

void AddSC_battlepet_commandscript()
{
    new battlepet_commandscript();
}
