/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* mod-prabowow loader. Discovered and invoked automatically by
* AddModulesScripts(); the function name MUST match the folder name
* (mod-prabowow -> mod_prabowow).
*/

void AddSC_prabowow_xp_rate();
void AddSC_prabowow_world_chat();
void AddSC_prabowow_auto_sell();
void AddSC_prabowow_flight_paths();
void AddSC_prabowow_heirloom_vendor();
void AddSC_prabowow_starter_mail();

void Addmod_prabowowScripts()
{
    AddSC_prabowow_xp_rate();
    AddSC_prabowow_world_chat();
    AddSC_prabowow_auto_sell();
    AddSC_prabowow_flight_paths();
    AddSC_prabowow_heirloom_vendor();
    AddSC_prabowow_starter_mail();
}
