/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Example_Misc
SD%Complete: 100
SDComment: Item, Areatrigger and other small code examples
SDCategory: Script Examples
EndScriptData */

#include "ScriptMgr.h"
#include "Player.h"

class AT_example_areatrigger : public AreaTriggerScript
{
    public:
        AT_example_areatrigger() : AreaTriggerScript("example_areatrigger") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            player->Kill(player);
            return true;
        }
};

class ItemUse_example_item : public ItemScript
{
    public:
        ItemUse_example_item() : ItemScript("example_item") { }

        bool OnUse(Player* /*player*/, Item* /*item*/, SpellCastTargets const& /*targets*/) OVERRIDE
        {
            sScriptMgr->LoadDatabase();
            return true;
        }
};

class GOHello_example_go_teleporter : public GameObjectScript
{
    public:
        GOHello_example_go_teleporter() : GameObjectScript("example_go_teleporter") { }

        bool OnGossipHello(Player* player, GameObject* /*go*/) OVERRIDE
        {
            player->TeleportTo(0, 1807.07f, 336.105f, 70.3975f, 0.0f);
            return false;
        }
};

void AddSC_example_misc()
{
    new AT_example_areatrigger();
    new ItemUse_example_item();
    new GOHello_example_go_teleporter();
}
