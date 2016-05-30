#ifndef __TRINITY_ARCHAEOLOGYMGR_H
#define __TRINITY_ARCHAEOLOGYMGR_H

#include <map>
#include <string>

#include "Common.h"
#include <ace/Singleton.h>
#include "DatabaseEnv.h"
#include "DBCEnums.h"
#include "DBCStores.h"

class Player;

#define MAX_RESEARCH_SITES                      16
#define RESEARCH_SITES_PER_MAP                  4
#define MAX_RESEARCH_PROJECTS                   9
#define ARCHAEOLOGY_DIG_SITE_FAR_DIST           40
#define ARCHAEOLOGY_DIG_SITE_MED_DIST           20
#define ARCHAEOLOGY_DIG_SITE_CLOSE_DIST         8
#define ARCHAEOLOGY_DIG_SITE_FAR_SURVEYBOT      206590
#define ARCHAEOLOGY_DIG_SITE_MEDIUM_SURVEYBOT   206589
#define ARCHAEOLOGY_DIG_SITE_CLOSE_SURVEYBOT    204272

enum ResearchWithLevelResult
{
    RS_RESULT_FAIL  = 0,
    RS_RESULT_OK    = 1,
    RS_RESULT_HIDE  = 2,
};

struct DigitSite
{
    uint8 count;
    uint16 site_id;
    uint32 loot_id;
    float loot_x;
    float loot_y;
    float loot_z;

    void clear()
    {
        site_id = loot_id = 0;
        loot_x = loot_y = loot_z = 0.0f;
        count = 0;
    }

    bool empty() { return site_id == 0; }
};

struct ProjectCost
{
    ProjectCost(uint32 _itemId, uint32 _count, bool isCurrency)
    {
        itemId = _itemId;
        count = _count;
        currency = isCurrency;
    }

    uint32 itemId;
    uint32 count;
    bool currency;
};

typedef std::set<uint32> SiteSet;
typedef std::map<uint32, SiteSet> Sites;
typedef std::set<uint32> ProjectSet;
typedef std::map<uint32, ProjectSet> Projects;

typedef std::set<uint32> ResearchSiteSet;
typedef std::set<uint32> ResearchProjectSet;
typedef std::set<uint32> CompletedProjectSet;

class ArchaeologyMgr
{
    public:
        ArchaeologyMgr(Player* player) : _player(player) 
        { 
            for (uint8 i = 0; i < MAX_RESEARCH_SITES; ++i)
                _digSites[i].clear();
        }
        ~ArchaeologyMgr() {}

        void LoadArchaeology(PreparedQueryResult result);
        void SaveArchaeology(SQLTransaction& trans);
        
        void AddProjectCost(uint32 entry, uint32 count, bool isCurrency)
        {
            costData.push_back(ProjectCost(entry, count, isCurrency));
        }
        void ClearProjectCost() { costData.clear(); }

        bool ValidateCostData();
        bool SolveResearchProject(uint32 projectId);
        uint32 GetSurveyBotEntry(float &orientation);

        CompletedProjectSet & GetCompletedProjects() { return _completedProjects; }

        void GenerateResearchProjects();
        void GenerateResearchSites();

        void ShowResearchProjects();
        void ShowResearchSites();

        void UseResearchSite(uint32 id);
    private:
        Player* _player;
        std::vector<ProjectCost> costData;
        DigitSite _digSites[16];
        ResearchSiteSet _researchSites[4];
        ResearchProjectSet _researchProjects;
        CompletedProjectSet _completedProjects;
        bool _archaeologyChanged;

        bool HasResearchSite(uint32 id, uint32 mapId) const
        {
            switch (mapId)
            {
                case 0: return _researchSites[0].find(id) != _researchSites[0].end();
                case 1: return _researchSites[1].find(id) != _researchSites[1].end();
                case 530: return _researchSites[2].find(id) != _researchSites[2].end();
                case 571: return _researchSites[3].find(id) != _researchSites[3].end();
                default: return false;
            }
            return false;
        }

        bool HasResearchProject(uint32 id) const
        {
            return _researchProjects.find(id) != _researchProjects.end();
        }

        bool GenerateDigitLoot(uint16 zoneid, DigitSite &site);
        
        uint16 GetResearchSiteID();
        bool IsCompletedProject(uint32 id);
        
        void GenerateResearchSiteInMap(uint32 mapId, uint32 map);
        ResearchWithLevelResult CanResearchWithLevel(uint32 siteId);

        void ValidateSites();
        void ValidateProjects();
};

#endif
