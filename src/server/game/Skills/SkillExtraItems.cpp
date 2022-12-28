/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "SkillExtraItems.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Player.h"
#include <map>

// some type definitions
// no use putting them in the header file, they're only used in this .cpp

// struct to store information about extra item creation
// one entry for every spell that is able to create an extra item
struct SkillExtraItemEntry
{
    // the spell id of the specialization required to create extra items
    uint32 requiredSpecialization;
    // the chance to create one additional item
    float additionalCreateChance;
    // maximum number of extra items created per crafting
    uint8 additionalMaxNum;

    SkillExtraItemEntry()
        : requiredSpecialization(0), additionalCreateChance(0.0f), additionalMaxNum(0) { }

    SkillExtraItemEntry(uint32 rS, float aCC, uint8 aMN)
        : requiredSpecialization(rS), additionalCreateChance(aCC), additionalMaxNum(aMN) { }
};

// map to store the extra item creation info, the key is the spellId of the creation spell, the mapped value is the assigned SkillExtraItemEntry
typedef std::map<uint32, SkillExtraItemEntry> SkillExtraItemMap;

SkillExtraItemMap SkillExtraItemStore;

// loads the extra item creation info from DB
void LoadSkillExtraItemTable()
{
    uint32 oldMSTime = getMSTime();

    SkillExtraItemStore.clear();                            // need for reload

    //                                                  0               1                       2                    3
    QueryResult result = WorldDatabase.Query("SELECT spellId, requiredSpecialization, additionalCreateChance, additionalMaxNum FROM skill_extra_item_template");

    if (!result)
    {
        SF_LOG_ERROR("server.loading", ">> Loaded 0 spell specialization definitions. DB table `skill_extra_item_template` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 spellId = fields[0].GetUInt32();

        if (!sSpellMgr->GetSpellInfo(spellId))
        {
            SF_LOG_ERROR("sql.sql", "Skill specialization %u has non-existent spell id in `skill_extra_item_template`!", spellId);
            continue;
        }

        uint32 requiredSpecialization = fields[1].GetUInt32();
        if (!sSpellMgr->GetSpellInfo(requiredSpecialization))
        {
            SF_LOG_ERROR("sql.sql", "Skill specialization %u have not existed required specialization spell id %u in `skill_extra_item_template`!", spellId, requiredSpecialization);
            continue;
        }

        float additionalCreateChance = fields[2].GetFloat();
        if (additionalCreateChance <= 0.0f)
        {
            SF_LOG_ERROR("sql.sql", "Skill specialization %u has too low additional create chance in `skill_extra_item_template`!", spellId);
            continue;
        }

        uint8 additionalMaxNum = fields[3].GetUInt8();
        if (!additionalMaxNum)
        {
            SF_LOG_ERROR("sql.sql", "Skill specialization %u has 0 max number of extra items in `skill_extra_item_template`!", spellId);
            continue;
        }

        SkillExtraItemEntry& skillExtraItemEntry = SkillExtraItemStore[spellId];

        skillExtraItemEntry.requiredSpecialization = requiredSpecialization;
        skillExtraItemEntry.additionalCreateChance = additionalCreateChance;
        skillExtraItemEntry.additionalMaxNum       = additionalMaxNum;

        ++count;
    }
    while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u spell specialization definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool CanCreateExtraItems(Player* player, uint32 spellId, float &additionalChance, uint8 &additionalMax)
{
    // get the info for the specified spell
    SkillExtraItemMap::const_iterator ret = SkillExtraItemStore.find(spellId);
    if (ret == SkillExtraItemStore.end())
        return false;

    SkillExtraItemEntry const* specEntry = &ret->second;

    // if no entry, then no extra items can be created
    if (!specEntry)
        return false;

    // the player doesn't have the required specialization, return false
    if (!player->HasSpell(specEntry->requiredSpecialization))
        return false;

    // set the arguments to the appropriate values
    additionalChance = specEntry->additionalCreateChance;
    additionalMax = specEntry->additionalMaxNum;

    // enable extra item creation
    return true;
}
