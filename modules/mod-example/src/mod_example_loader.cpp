/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* Module loader. Discovered and invoked automatically by AddModulesScripts().
* The function name MUST match the folder name (mod-example -> mod_example).
*/

// Registration functions provided by this module.
void AddSC_mod_example();

// Aggregate loader for the mod-example module.
void Addmod_exampleScripts()
{
    AddSC_mod_example();
}
