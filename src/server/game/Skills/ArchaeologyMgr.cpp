#include "ArchaeologyMgr.h"
#include "Common.h"
#include "Containers.h"

const static uint16 _mapIds[4] = { 0, 1, 530, 571};

const static uint8 _races[9] = {1, 2, 3, 4, 5, 6, 7, 8, 27};

const static int q_patt[2][2] = { {0,1}, {3,2} };



namespace WoWSource
{
    bool IsPointInZone(const ResearchPOIPoint &test, const ResearchPOIPoints &polygon)
    {
        if (polygon.size() < 3)
            return false;

        ResearchPOIPoints::const_iterator end = polygon.end();
        ResearchPOIPoint pred_pt = polygon.back();
        pred_pt.x -= test.x;
        pred_pt.y -= test.y;

        int pred_q = q_patt[pred_pt.y < 0][pred_pt.x<  0];

        int w = 0;

        for (std::vector<ResearchPOIPoint>::const_iterator iter = polygon.begin(); iter != end; ++iter)
        {
            ResearchPOIPoint cur_pt = *iter;

            cur_pt.x -= test.x;
            cur_pt.y -= test.y;

            int q = q_patt[cur_pt.y < 0][cur_pt.x < 0];

            switch (q - pred_q)
            {
                case -3:
                    ++w;
                    break;
                case 3:
                    --w;
                    break;
                case -2:
                    if (pred_pt.x * cur_pt.y >= pred_pt.y * cur_pt.x)
                        ++w;
                    break;
                case 2:
                    if (!(pred_pt.x * cur_pt.y >= pred_pt.y * cur_pt.x))
                        --w;
                    break;
            }

            pred_pt = cur_pt;
            pred_q = q;
        }
        return w != 0;
    }
}

bool ArchaeologyMgr::GenerateDigitLoot(uint16 zoneid, DigitSite &site)
{
    ResearchLootVector const& loot = sObjectMgr->GetResearchLoot();
    if (loot.empty())
        return false;

    site.loot_id = 0;

    ResearchLootVector lootList;
    for (ResearchLootVector::const_iterator itr = loot.begin(); itr != loot.end(); ++itr)
    {
        ResearchLootEntry entry = (*itr);
        if (entry.id != zoneid)
            continue;

        if (site.loot_id == 0)
        {
            switch (entry.race)
            {
            case 1: site.loot_id = 204282; break;
            case 2: site.loot_id = 207188; break;
            case 3: site.loot_id = 206836; break;
            case 4: site.loot_id = 203071; break;
            case 5: site.loot_id = 203078; break;
            case 6: site.loot_id = 207187; break;
            case 7: site.loot_id = 207190; break;
            case 8: site.loot_id = 202655; break;
            case 27: site.loot_id = 207189; break;
            default: site.loot_id = 0; break;
            }
        }

        lootList.push_back(entry);
    }

    if (lootList.empty())
        return false;

    ResearchLootVector::const_iterator entry = lootList.begin();
    std::advance(entry, urand(0, lootList.size() - 1));

    site.loot_x = entry->x;
    site.loot_y = entry->y;
    site.loot_z = entry->z;

    return true;
}

uint32 ArchaeologyMgr::GetSurveyBotEntry(float &orientation)
{
    uint16 skill_now = _player->GetSkillValue(SKILL_ARCHAEOLOGY);
    if (!skill_now)
        return 0;

    uint16 zoneid = GetResearchSiteID();
    if (!zoneid)
        return 0;

    uint32 at_pos = 0xFFFF;

    for(uint8 i = 0; i < MAX_RESEARCH_SITES / 2; ++i)
    {
        //Replace by GetUInt16Value
        uint32 site_now_1 = _player->GetDynamicUInt32Value(PLAYER_DYNAMIC_RESEARCH_SITES, i) & 0xFFFF;
        uint32 site_now_2 = _player->GetDynamicUInt32Value(PLAYER_DYNAMIC_RESEARCH_SITES, i) >> 16;

        if (zoneid == site_now_1)
        {
            at_pos = i * 2;
            break;
        }
        if (zoneid == site_now_2)
        {
            at_pos = i * 2 + 1;
            break;
        }
    }

    if (at_pos == 0xFFFF)
        return 0;

    ASSERT(at_pos < 16);

    DigitSite &site = _digSites[at_pos];
    if (site.site_id != zoneid)
    {
        if (!GenerateDigitLoot(zoneid, site))
            return 0;

        site.site_id = zoneid;
    }

    orientation = _player->GetAngle(site.loot_x, site.loot_y);
    float dist_now = _player->GetDistance2d(site.loot_x, site.loot_y);

    if (dist_now >= ARCHAEOLOGY_DIG_SITE_FAR_DIST)
        return ARCHAEOLOGY_DIG_SITE_FAR_SURVEYBOT;
    if (dist_now >= ARCHAEOLOGY_DIG_SITE_MED_DIST)
        return ARCHAEOLOGY_DIG_SITE_MEDIUM_SURVEYBOT;
    if (dist_now >= ARCHAEOLOGY_DIG_SITE_CLOSE_DIST)
        return ARCHAEOLOGY_DIG_SITE_CLOSE_SURVEYBOT;

    if (skill_now < 50)
        _player->UpdateSkill(SKILL_ARCHAEOLOGY, 1);

    _player->SummonGameObject(site.loot_id, site.loot_x, site.loot_y, site.loot_z, 0, 0, 0, 0, 0, 30000);

    if (site.count < 2)
    {
        site.count++;
        if (!GenerateDigitLoot(zoneid, site))
            return 0;
    }
    else
    {
        site.clear();
        UseResearchSite(zoneid);
    }

    _archaeologyChanged = true;

    return 0;
}

uint16 ArchaeologyMgr::GetResearchSiteID()
{
    ResearchZoneMap const& zoneMap = sObjectMgr->GetResearchZoneMap();

    for (std::map<uint32, ResearchZoneEntry>::const_iterator itr = zoneMap.begin(); itr != zoneMap.end(); ++itr)
    {
        if ((*itr).second.map == _player->GetMapId() && (*itr).second.zone == _player->GetZoneId())
        {
            if (WoWSource::IsPointInZone(ResearchPOIPoint(_player->GetPositionX(), _player->GetPositionY()), itr->second.coords))
                return (*itr).first;
        }
    }
    return 0;
}

void ArchaeologyMgr::UseResearchSite(uint32 id)
{
    uint32 map = 0;
    switch (_player->GetMapId())
    {
        case 0: map = 0; break;
        case 1: map = 1; break;
        case 530: map = 2; break;
        case 571: map = 3; break;
        //case 870: map = 4; break;
        default: break;
    }
    _researchSites[map].erase(id);
    GenerateResearchSiteInMap(_player->GetMapId(), map);
}

void ArchaeologyMgr::ShowResearchSites()
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    uint8 idx = 0;
    for (uint8 i = 0; i < 4; ++i)
        for (ResearchSiteSet::const_iterator itr = _researchSites[i].begin(); itr != _researchSites[i].end(); ++itr)
        {
            uint32 id = (*itr);
            if (CanResearchWithLevel((*itr)) == RS_RESULT_HIDE)
                id = 0;

            _player->SetDynamicUInt32Value(PLAYER_DYNAMIC_RESEARCH_SITES, idx, id);

            ++idx;
        }
}

void ArchaeologyMgr::ShowResearchProjects()
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    uint8 count = 0;
    uint32 newvalue;

    for (ResearchProjectSet::const_iterator itr = _researchProjects.begin(); itr != _researchProjects.end(); ++itr)
    {
        if (count % 2 == 1)
        {
            newvalue |= (*itr);
            _player->SetUInt32Value(PLAYER_FIELD_RESEARCHING + count / 2, newvalue);
        }
        else if (count == 8)
        {
            _player->SetUInt32Value(PLAYER_FIELD_RESEARCHING + count / 2, (*itr));
            break;
        }
        else
            newvalue = (*itr) << 16;

        ++count;
    }
}

ResearchWithLevelResult ArchaeologyMgr::CanResearchWithLevel(uint32 siteId)
{
    uint32 skill_now = _player->GetSkillValue(SKILL_ARCHAEOLOGY);
    if (!skill_now)
        return RS_RESULT_FAIL;

    ResearchZoneMap const& zoneMap = sObjectMgr->GetResearchZoneMap();

    for (std::map<uint32, ResearchZoneEntry>::const_iterator itr = zoneMap.begin(); itr != zoneMap.end(); ++itr)
    {
        if ((*itr).first == siteId)
        {
            uint16 skill_cap = 0;
            uint8 level_cap = 0;
            uint8 cur_level = _player->getLevel();
            switch ((*itr).second.map)
            {
                case 0:
                    if ((*itr).second.zone == 4922) // Twilight Hightlands
                        skill_cap = 450;
                    level_cap = (*itr).second.level;
                    if (skill_now < skill_cap || (cur_level + 29 < level_cap))
                        return RS_RESULT_FAIL;
                    break;
                case 1:
                    if ((*itr).second.zone == 616) // Hyjal
                        skill_cap = 450;
                    else if ((*itr).second.zone == 5034) // Uldum
                        skill_cap = 450;
                    level_cap = (*itr).second.level;
                    if (skill_now < skill_cap || (cur_level + 29 < level_cap))
                        return RS_RESULT_FAIL;
                    break;
                case 530: // Outland
                    skill_cap = 275;
                    level_cap = 58;
                    if (skill_now < skill_cap || (cur_level < level_cap))
                        return RS_RESULT_HIDE;
                    break;
                case 571: // Northrend
                    skill_cap = 350;
                    level_cap = 68;
                    if (skill_now < skill_cap || (cur_level < level_cap))
                        return RS_RESULT_HIDE;
                    break;
                default:
                    return RS_RESULT_FAIL;
            }

            return RS_RESULT_OK;
        }
    }
    // If get there, so POIid is wrong
    return RS_RESULT_FAIL;
}

void ArchaeologyMgr::GenerateResearchSiteInMap(uint32 mapId, uint32 map)
{
    if (sResearchSiteSet.empty())
        return;

    SiteSet tempSites;

    for (std::set<ResearchSiteEntry const*>::const_iterator itr = sResearchSiteSet.begin(); itr != sResearchSiteSet.end(); ++itr)
        if (!HasResearchSite((*itr)->ID, mapId) && (*itr)->mapId == mapId && CanResearchWithLevel((*itr)->ID))
            tempSites.insert((*itr)->ID);

    if (tempSites.empty())
        return;

    _researchSites[map].insert(WoWSource::Containers::SelectRandomContainerElement(tempSites));
    _archaeologyChanged = true;

    ShowResearchSites();
}

void ArchaeologyMgr::GenerateResearchSites()
{
    if (sResearchSiteSet.empty())
        return;

    for (uint8 i = 0; i < 4; ++i)
        _researchSites[i].clear();

    Sites tempSites;
    for (std::set<ResearchSiteEntry const*>::const_iterator itr = sResearchSiteSet.begin(); itr != sResearchSiteSet.end(); ++itr)
        if (CanResearchWithLevel((*itr)->ID))
        {
            switch ((*itr)->mapId)
            {
                case 0: _researchSites[0].insert((*itr)->ID); break;
                case 1: _researchSites[1].insert((*itr)->ID); break;
                case 530: _researchSites[2].insert((*itr)->ID); break;
                case 571: _researchSites[3].insert((*itr)->ID); break;
                default: break;
            }
        }

    for (uint8 i = 0; i < 4; ++i)
        WoWSource::Containers::RandomResizeSet(_researchSites[i], RESEARCH_SITES_PER_MAP);

    _archaeologyChanged = true;

    ShowResearchSites();
}

void ArchaeologyMgr::GenerateResearchProjects()
{
    if (sResearchProjectSet.empty())
        return;

    uint16 skill_now = _player->GetSkillValue(SKILL_ARCHAEOLOGY);
    if (!skill_now)
        return;

    _researchProjects.clear();

    Projects tempProjects;
    uint32 chance = skill_now / 10;

    for (std::set<ResearchProjectEntry const*>::const_iterator itr = sResearchProjectSet.begin(); itr != sResearchProjectSet.end(); ++itr)
    {
        ResearchProjectEntry const* entry = (*itr);
        if ((entry->rare && !roll_chance_i(chance)) || IsCompletedProject(entry->ID))
            continue;

        tempProjects[entry->branchId].insert(entry->ID);
    }

    uint8 const* race = _races;
    ProjectSet::const_iterator itr;
    do
    {
        itr = tempProjects[*race].begin();
        std::advance(itr, urand(0, tempProjects[*race].size() - 1));
        _researchProjects.insert((*itr));
    }
    while(*++race);

    _archaeologyChanged = true;

    ShowResearchProjects();
}

bool ArchaeologyMgr::SolveResearchProject(uint32 projectId)
{
    if (!projectId)
        return false;

    // Check for skill
    uint16 skill_now = _player->GetSkillValue(SKILL_ARCHAEOLOGY);
    if (!skill_now)
        return false;

    // Check for correct cost data
    if (!ValidateCostData())
        return false;

    // Check for project id
    ResearchProjectEntry const* entry = NULL;
    for(std::set<ResearchProjectEntry const*>::const_iterator itr = sResearchProjectSet.begin(); itr != sResearchProjectSet.end(); ++itr)
        if ((*itr)->ID == projectId)
            entry = (*itr);
    
    if (!entry)
        return false;

    // Check for existing project
    if (!HasResearchProject(projectId))
        return false;

    // All done
    for (std::vector<ProjectCost>::const_iterator itr = costData.begin(); itr != costData.end(); ++itr)
    {
        if ((*itr).currency)
            _player->ModifyCurrency((*itr).itemId, -int32((*itr).count));
        else
            _player->DestroyItemCount((*itr).itemId, (*itr).count, true);
    }

    ClearProjectCost();

    _researchProjects.erase(projectId);

    _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS, projectId, 1);

    if (entry->rare)
        _completedProjects.insert(projectId);

    // Add new project
    ProjectSet tempProjects;
    uint32 chance = skill_now / 10;

    for (std::set<ResearchProjectEntry const*>::const_iterator itr = sResearchProjectSet.begin(); itr != sResearchProjectSet.end(); ++itr)
    {
        if ((*itr)->branchId == entry->branchId)
        {
            if (((*itr)->rare && !roll_chance_i(chance)) || IsCompletedProject((*itr)->ID))
                continue;

            tempProjects.insert((*itr)->ID);
        }
    }

    _researchProjects.insert(WoWSource::Containers::SelectRandomContainerElement(tempProjects));
    _archaeologyChanged = true;

    ShowResearchProjects();
    return true;
}

bool ArchaeologyMgr::IsCompletedProject(uint32 id)
{
    CompletedProjectSet::const_iterator itr = _completedProjects.find(id);
    return (itr != _completedProjects.end()); 
}

void ArchaeologyMgr::SaveArchaeology(SQLTransaction& trans)
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    if (!_archaeologyChanged)
        return;

    trans->PAppend("DELETE FROM character_archaeology WHERE guid = '%u'", GUID_LOPART(_player->GetGUID()));

    std::ostringstream ss;

    ss << "INSERT INTO character_archaeology  (guid, sites0, sites1, sites2, sites3, counts, projects, completed) VALUES (";

    ss << GUID_LOPART(_player->GetGUID()) << ", '";

    if (!_researchSites[0].empty())
        for (ResearchSiteSet::const_iterator itr = _researchSites[0].begin(); itr != _researchSites[0].end(); ++itr)
            ss << (*itr) << " ";
    
    ss << "', '";

    if (!_researchSites[1].empty())
        for (ResearchSiteSet::const_iterator itr = _researchSites[1].begin(); itr != _researchSites[1].end(); ++itr)
            ss << (*itr) << " ";

    ss << "', '";

    if (!_researchSites[2].empty())
        for (ResearchSiteSet::const_iterator itr = _researchSites[2].begin(); itr != _researchSites[2].end(); ++itr)
            ss << (*itr) << " ";

    ss << "', '";

    if (!_researchSites[3].empty())
        for (ResearchSiteSet::const_iterator itr = _researchSites[3].begin(); itr != _researchSites[3].end(); ++itr)
            ss << (*itr) << " ";

    ss << "', '";

    for (uint8 j = 0; j < MAX_RESEARCH_SITES; ++j)
        ss << uint32(_digSites[j].count) << " ";

    ss << "', '";

    for (ResearchProjectSet::const_iterator itr = _researchProjects.begin(); itr != _researchProjects.end(); ++itr)
        ss << (*itr) << " ";

    ss << "', '";

    for (CompletedProjectSet::const_iterator itr = _completedProjects.begin(); itr != _completedProjects.end(); ++itr)
        ss << (*itr) << " ";

    ss << "')";

    trans->Append(ss.str().c_str());
    _archaeologyChanged = false;
}

void ArchaeologyMgr::LoadArchaeology(PreparedQueryResult result)
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    for (uint8 i = 0; i < MAX_RESEARCH_SITES; ++i)
        _digSites[i].clear();

    if (!result)
        return;

    Field* fields = result->Fetch();

    for (uint8 i = 0; i < 4; ++i)
        _researchSites[i].clear();

    // Loading current zones
    if (fields[0].GetCString())
    {
        Tokenizer tokens(fields[0].GetCString(), ' ', RESEARCH_SITES_PER_MAP);
        if (tokens.size() == RESEARCH_SITES_PER_MAP)
            for (uint8 i = 0; i < tokens.size(); ++i)
                _researchSites[0].insert(uint32(atoi(tokens[i])));
    }
    if (fields[1].GetCString())
    {
        Tokenizer tokens(fields[1].GetCString(), ' ', RESEARCH_SITES_PER_MAP);
        if (tokens.size() == RESEARCH_SITES_PER_MAP)
            for (uint8 i = 0; i < tokens.size(); ++i)
                _researchSites[1].insert(uint32(atoi(tokens[i])));
    }
    if (fields[2].GetCString())
    {
        Tokenizer tokens(fields[2].GetCString(), ' ', RESEARCH_SITES_PER_MAP);
        if (tokens.size() == RESEARCH_SITES_PER_MAP)
            for (uint8 i = 0; i < tokens.size(); ++i)
                _researchSites[2].insert(uint32(atoi(tokens[i])));
    }
    if (fields[3].GetCString())
    {
        Tokenizer tokens(fields[3].GetCString(), ' ', RESEARCH_SITES_PER_MAP);
        if (tokens.size() == RESEARCH_SITES_PER_MAP)
            for (uint8 i = 0; i < tokens.size(); ++i)
                _researchSites[3].insert(uint32(atoi(tokens[i])));
    }

    ValidateSites();

    // Loading current zone info
    if (fields[4].GetCString())
    {
        Tokenizer tokens(fields[4].GetCString(), ' ', MAX_RESEARCH_SITES);
        if (tokens.size() == MAX_RESEARCH_SITES)
        {
            for (uint8 i = 0; i < MAX_RESEARCH_SITES; ++i)
                _digSites[i].count = uint32(atoi(tokens[i]));
        }
    }

    _researchProjects.clear();
    // Loading current projects
    if (fields[5].GetCString())
    {
        Tokenizer tokens(fields[5].GetCString(), ' ', MAX_RESEARCH_PROJECTS);
        if (tokens.size() == MAX_RESEARCH_PROJECTS)
            for (uint8 i = 0; i < MAX_RESEARCH_PROJECTS; ++i)
                _researchProjects.insert(uint32(atoi(tokens[i])));
    }

    ValidateProjects();

    // Loading completed projects
    if (fields[6].GetCString())
    {
        Tokenizer tokens(fields[6].GetCString(), ' ');
        if (tokens.size() > 0)
            for (Tokenizer::const_iterator itr = tokens.begin(); itr != tokens.end(); ++itr)
                _completedProjects.insert(atoi((*itr)));
    }
}

void ArchaeologyMgr::ValidateSites()
{
    bool err = false;

    for (uint8 i = 0; i < 4; ++i)
    {
        if (_researchSites[i].empty())
        {
            err = true;
            break;
        }
        for (ResearchSiteSet::const_iterator itr = _researchSites[i].begin(); itr != _researchSites[i].end(); ++itr)
        {
            if (!sResearchSiteStore.LookupEntry((*itr)))
            {
                err = true;
                break;
            }
        }
    }

    if (err)
        GenerateResearchSites();
}

void ArchaeologyMgr::ValidateProjects()
{
    bool err = false;
    if (_researchProjects.empty())
        err = true;

    for (ResearchProjectSet::const_iterator itr = _researchProjects.begin(); itr != _researchProjects.end(); ++itr)
    {
        if (!sResearchProjectStore.LookupEntry((*itr)))
        {
            err = true;
            break;
        }
    }

    if (err)
        GenerateResearchProjects();
}

bool ArchaeologyMgr::ValidateCostData()
{
    if (costData.empty())
        return false;
    
    for (std::vector<ProjectCost>::const_iterator itr = costData.begin(); itr != costData.end(); ++itr)
    {
        if ((*itr).currency)
        {
            if (!_player->HasCurrency((*itr).itemId, (*itr).count))
                return false;
        }
        else
        {
            if (!_player->HasItemCount((*itr).itemId, (*itr).count))
                return false;
        }
    }
    return true;
}
