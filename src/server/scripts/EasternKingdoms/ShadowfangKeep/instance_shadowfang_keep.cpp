/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "Player.h"
#include "shadowfang_keep.h"

DoorData const doorData[] =
{
    { GO_COURTYARD_DOOR, DATA_ASHBURY, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_SORCERER_DOOR,  DATA_VALDEN,  DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_ARUGAL_DOOR,    DATA_VALDEN,  DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_ARUGAL_DOOR,    DATA_GODFREY, DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { 0,                 0,            DOOR_TYPE_ROOM,    BOUNDARY_NONE },
};

class instance_shadowfang_keep : public InstanceMapScript
{
    public:
        instance_shadowfang_keep() : InstanceMapScript("instance_shadowfang_keep", 33) { }

        struct instance_shadowfang_keep_InstanceMapScript : public InstanceScript
        {
            instance_shadowfang_keep_InstanceMapScript(Map* map) : InstanceScript(map) { }

            void Initialize() OVERRIDE
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);
                uiAshburyGUID     = 0;
                uiSilverlaineGUID = 0;
                uiSpringvaleGUID  = 0;
                uiValdenGUID      = 0;
                uiGodfreyGUID     = 0;
                followerGUID      = 0;

                uiFryeGUID        = 0;
                uiHummelGUID      = 0;
                uiBaxterGUID      = 0;
                spawnCrazedTimer  = 0;
                uiCrownEvent      = NOT_STARTED;
                teamInInstance    = 0;

                m_sFangIntro.clear();
                m_sFangPhasing.clear();
            }

            void OnPlayerEnter(Player* player) OVERRIDE
            {
                if (!teamInInstance)
                    teamInInstance = player->GetTeam();
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                if (!teamInInstance)
                {
                    Map::PlayerList const& players = instance->GetPlayers();
                    if (!players.isEmpty())
                        if (Player* player = players.begin()->GetSource())
                            teamInInstance = player->GetTeam();
                }

                switch (creature->GetEntry())
                {
                    case NPC_BELMONT:
                        if (teamInInstance == ALLIANCE)
                            creature->UpdateEntry(NPC_IVAR, ALLIANCE);

                        // Exclude spawn at entrance
                        if (creature->GetPositionZ() > 82.47f || creature->GetPositionZ() < 79.23f)
                        {
                            m_sFangPhasing.push_back(creature->GetGUID());
                            creature->SetVisible(false);

                            // Quest follower at Godfrey
                            if (creature->GetPositionZ() > 150.0f)
                                followerGUID = creature->GetGUID();
                            break;
                        }

                        m_sFangIntro.push_back(creature->GetGUID());
                        break;
                    case NPC_GUARD_HORDE1:
                    case NPC_GUARD_HORDE2:
                        if (teamInInstance == ALLIANCE)
                            creature->UpdateEntry(NPC_GUARD_ALLY, ALLIANCE);

                        if (creature->GetPositionZ() > 82.47f || creature->GetPositionZ() < 79.23f)
                        {
                            m_sFangPhasing.push_back(creature->GetGUID());
                            creature->SetVisible(false);
                            break;
                        }

                        m_sFangIntro.push_back(creature->GetGUID());
                        break;
                    case NPC_CROMUSH:
                        creature->SetVisible(false);
                        break;
                    case NPC_ASHBURY:
                        uiAshburyGUID = creature->GetGUID();
                        break;
                    case NPC_SILVERLAINE:
                        uiSilverlaineGUID = creature->GetGUID();
                        break;
                    case NPC_SPRINGVALE:
                        uiSpringvaleGUID = creature->GetGUID();
                        break;
                    case NPC_VALDEN:
                        uiValdenGUID = creature->GetGUID();
                        break;
                    case NPC_GODFREY:
                        uiGodfreyGUID = creature->GetGUID();
                        break;
                    case NPC_APOTHECARY_FRYE:
                        uiFryeGUID = creature->GetGUID();
                        break;
                    case NPC_APOTHECARY_HUMMEL:
                        uiHummelGUID = creature->GetGUID();
                        break;
                    case NPC_APOTHECARY_BAXTER:
                        uiBaxterGUID = creature->GetGUID();
                        break;
                }
            }

            // Shows the followers waiting next to the boss that was just defeated
            void HandleSetNextPhase(uint32 type)
            {
                for (uint64 guid : m_sFangPhasing)
                    if (Creature* guard = instance->GetCreature(guid))
                        guard->SetVisible(false);

                if (Creature* encounter = instance->GetCreature(GetData64(type)))
                {
                    std::list<Creature*> phaseList;
                    for (uint32 entry : FangEntries)
                        GetCreatureListWithEntryInGrid(phaseList, encounter, entry, 28.0f);

                    for (Creature* follower : phaseList)
                    {
                        // only Belmont/Ivar have an AI, the guards just appear
                        if (follower->AI())
                            follower->AI()->DoAction(encounter->GetEntry());

                        follower->SetVisible(true);
                    }
                }

                if (type == DATA_ASHBURY)
                    for (uint64 guid : m_sFangIntro)
                        if (Creature* guard = instance->GetCreature(guid))
                            guard->SetVisible(false);
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_COURTYARD_DOOR:
                    case GO_SORCERER_DOOR:
                    case GO_ARUGAL_DOOR:
                        AddDoor(go, true);
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_COURTYARD_DOOR:
                    case GO_SORCERER_DOOR:
                    case GO_ARUGAL_DOOR:
                        AddDoor(go, false);
                        break;
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_CROWN:
                        uiCrownEvent = data;
                        if (data == NOT_STARTED)
                            spawnCrazedTimer = urand(7000, 14000);
                        break;
                    case DATA_ASHBURY:
                    case DATA_SILVERLAINE:
                    case DATA_SPRINGVALE:
                    case DATA_VALDEN:
                    case DATA_GODFREY:
                        if (data == DONE)
                            HandleSetNextPhase(type);
                        break;
                }
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                if (type == DATA_TEAM)
                    return teamInInstance;
                else if (type == DATA_CROWN)
                    return uiCrownEvent;

                return 0;
            }

            void Update(uint32 diff) OVERRIDE
            {
                if (GetData(DATA_CROWN) != IN_PROGRESS)
                    return;

                if (spawnCrazedTimer <= diff)
                {
                    if (Creature* hummel = instance->GetCreature(uiHummelGUID))
                        hummel->AI()->DoAction(3); // ACTION_SPAWN_CRAZED
                    spawnCrazedTimer = urand(2000, 5000);
                }
                else
                    spawnCrazedTimer -= diff;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S K " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_ASHBURY:
                        return uiAshburyGUID;
                    case DATA_SILVERLAINE:
                        return uiSilverlaineGUID;
                    case DATA_SPRINGVALE:
                        return uiSpringvaleGUID;
                    case DATA_VALDEN:
                        return uiValdenGUID;
                    case DATA_GODFREY:
                        return uiGodfreyGUID;
                    case DATA_FRYE:
                        return uiFryeGUID;
                    case DATA_HUMMEL:
                        return uiHummelGUID;
                    case DATA_BAXTER:
                        return uiBaxterGUID;
                    case DATA_FOLLOWER:
                        return followerGUID;
                }
                return 0;
            }

            void Load(char const* in) OVERRIDE
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'S' && dataHead2 == 'K')
                {
                    for (uint32 i = 0; i < EncounterCount; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            uint64 uiAshburyGUID;
            uint64 uiSilverlaineGUID;
            uint64 uiSpringvaleGUID;
            uint64 uiValdenGUID;
            uint64 uiGodfreyGUID;
            uint64 uiFryeGUID;
            uint64 uiHummelGUID;
            uint64 uiBaxterGUID;
            uint64 followerGUID;
            uint32 spawnCrazedTimer;
            uint32 uiCrownEvent;
            uint32 teamInInstance;

            std::vector<uint64> m_sFangPhasing, m_sFangIntro;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_shadowfang_keep_InstanceMapScript(map);
        }
};

void AddSC_instance_shadowfang_keep()
{
    new instance_shadowfang_keep();
}
