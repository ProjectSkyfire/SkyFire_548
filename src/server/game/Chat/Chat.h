/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_CHAT_H
#define SKYFIRESERVER_CHAT_H

#include "RBAC.h"
#include "SharedDefines.h"
#include "WorldSession.h"

#include <vector>

class ChatHandler;
class Creature;
class Group;
class Player;
class Unit;
class WorldSession;
class WorldObject;

struct GameTele;

class ChatCommand
{
    typedef bool(*pHandler)(ChatHandler*, char const*);

public:
    ChatCommand(char const* name, uint32 permission, bool allowConsole, pHandler handler, std::string help, std::vector<ChatCommand> childCommands = std::vector<ChatCommand>())
        : Name(name), Permission(permission), AllowConsole(allowConsole), Handler(handler), Help(std::move(help)), ChildCommands(std::move(childCommands)) { }
    const char* Name;
    uint32             Permission;                   // function pointer required correct align (use uint32)
    bool               AllowConsole;
    pHandler Handler;
    std::string        Help;
    std::vector<ChatCommand> ChildCommands;
};

class ChatHandler
{
public:
    WorldSession* GetSession() { return m_session; }
    explicit ChatHandler(WorldSession* session) : m_session(session), sentErrorMessage(false) { }
    virtual ~ChatHandler() { }

    // Builds chat packet and returns receiver guid position in the packet to substitute in whisper builders
    static size_t BuildChatPacket(WorldPacket& data, ChatMsg chatType, Language language, ObjectGuid senderGUID, ObjectGuid receiverGUID, std::string const& message, uint8 chatTag,
        std::string const& senderName = "", std::string const& receiverName = "",
        uint32 achievementId = 0, bool gmMessage = false, std::string const& channelName = "", std::string const& addonPrefix = "");

    // Builds chat packet and returns receiver guid position in the packet to substitute in whisper builders
    static size_t BuildChatPacket(WorldPacket& data, ChatMsg chatType, Language language, WorldObject const* sender, WorldObject const* receiver, std::string const& message, uint32 achievementId = 0, std::string const& channelName = "", LocaleConstant locale = DEFAULT_LOCALE, std::string const& addonPrefix = "");


    static char* LineFromMessage(char*& pos) { char* start = strtok(pos, "\n"); pos = NULL; return start; }

    // function with different implementation for chat/console
    virtual const char* GetSkyFireString(int32 entry) const;
    virtual void SendSysMessage(const char* str);

    void SendSysMessage(int32     entry);
    void PSendSysMessage(const char* format, ...) ATTR_PRINTF(2, 3);
    void PSendSysMessage(int32     entry, ...);
    std::string PGetParseString(int32 entry, ...) const;

    bool ParseCommands(const char* text);

    static std::vector<ChatCommand> const& getCommandTable();

    bool isValidChatMessage(const char* msg);
    void SendGlobalSysMessage(const char* str);

    bool hasStringAbbr(const char* name, const char* part);

    // function with different implementation for chat/console
    virtual bool isAvailable(ChatCommand const& cmd) const;
    virtual bool HasPermission(uint32 permission) const { return m_session->HasPermission(permission); }
    virtual std::string GetNameLink() const { return GetNameLink(m_session->GetPlayer()); }
    virtual bool needReportToTarget(Player* chr) const;
    virtual LocaleConstant GetSessionDbcLocale() const;
    virtual int GetSessionDbLocaleIndex() const;

    bool HasLowerSecurity(Player* target, uint64 guid, bool strong = false);
    bool HasLowerSecurityAccount(WorldSession* target, uint32 account, bool strong = false);

    void SendGlobalGMSysMessage(const char* str);
    Player* getSelectedPlayer();
    Creature* getSelectedCreature();
    Unit* getSelectedUnit();
    WorldObject* getSelectedObject();

    char* extractKeyFromLink(char* text, char const* linkType, char** something1 = NULL);
    char* extractKeyFromLink(char* text, char const* const* linkTypes, int* found_idx, char** something1 = NULL);

    // if args have single value then it return in arg2 and arg1 == NULL
    void      extractOptFirstArg(char* args, char** arg1, char** arg2);
    char* extractQuotedArg(char* args);

    uint32    extractSpellIdFromLink(char* text);
    uint64    extractGuidFromLink(char* text);
    GameTele const* extractGameTeleFromLink(char* text);
    bool GetPlayerGroupAndGUIDByName(const char* cname, Player*& player, Group*& group, uint64& guid, bool offline = false);
    std::string extractPlayerNameFromLink(char* text);
    // select by arg (name/link) or in-game selection online/offline player
    bool extractPlayerTarget(char* args, Player** player, uint64* player_guid = NULL, std::string* player_name = NULL);

    std::string playerLink(std::string const& name) const { return m_session ? "|cffffffff|Hplayer:" + name + "|h[" + name + "]|h|r" : name; }
    std::string GetNameLink(Player* chr) const;

    GameObject* GetNearbyGameObject();
    GameObject* GetObjectGlobalyWithGuidOrNearWithDbGuid(uint32 lowguid, uint32 entry);
    bool HasSentErrorMessage() const { return sentErrorMessage; }
    void SetSentErrorMessage(bool val) { sentErrorMessage = val; }
    static bool LoadCommandTable() { return load_command_table; }
    static void SetLoadCommandTable(bool val) { load_command_table = val; }

    bool ShowHelpForCommand(std::vector<ChatCommand> const& table, const char* cmd);
protected:
    explicit ChatHandler() : m_session(NULL), sentErrorMessage(false) { }     // for CLI subclass
    static bool SetDataForCommandInTable(std::vector<ChatCommand>& table, const char* text, uint32 permission, std::string const& help, std::string const& fullcommand);
    bool ExecuteCommandInTable(std::vector<ChatCommand> const& table, const char* text, std::string const& fullcmd);
    bool ShowHelpForSubCommands(std::vector<ChatCommand> const& table, char const* cmd, char const* subcmd);

private:
    WorldSession* m_session;                           // != NULL for chat command call and NULL for CLI command

    // common global flag
    static bool load_command_table;
    bool sentErrorMessage;
};

class CliHandler : public ChatHandler
{
public:
    typedef void Print(void*, char const*);
    explicit CliHandler(void* callbackArg, Print* zprint) : m_callbackArg(callbackArg), m_print(zprint) { }

    // overwrite functions
    const char* GetSkyFireString(int32 entry) const;
    bool isAvailable(ChatCommand const& cmd) const;
    bool HasPermission(uint32 /*permission*/) const { return true; }
    void SendSysMessage(const char* str);
    std::string GetNameLink() const;
    bool needReportToTarget(Player* chr) const;
    LocaleConstant GetSessionDbcLocale() const;
    int GetSessionDbLocaleIndex() const;

private:
    void* m_callbackArg;
    Print* m_print;
};

#endif
