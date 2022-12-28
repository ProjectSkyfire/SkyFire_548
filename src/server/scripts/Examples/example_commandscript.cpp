/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
Name: Example_Commandscript
%Complete: 100
Comment: Short custom scripting example
Category: Script Examples
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"

// **** This script is designed as an example for others to build on ****
// **** Please modify whatever you'd like to as this script is only for developement ****

// **** Script Info* ***
// This script's primary purpose is to show just how much you can really do with commandscripts

class example_commandscript : public CommandScript
{
    public:
        example_commandscript() : CommandScript("example_commandscript") { }

        static bool HandleHelloWorldCommand(ChatHandler* handler, const char* /*args*/)
        {
            handler->PSendSysMessage("Hello World");
            return true;
        }

        std::vector<ChatCommand> GetCommands() const OVERRIDE
        {
            static std::vector<ChatCommand> HelloWorldCommandTable =
            {
                { "hello",          SEC_PLAYER,         true,   &HandleHelloWorldCommand,        "", },
            };
            return HelloWorldCommandTable;
        }
};

void AddSC_example_commandscript()
{
    new example_commandscript();
}
