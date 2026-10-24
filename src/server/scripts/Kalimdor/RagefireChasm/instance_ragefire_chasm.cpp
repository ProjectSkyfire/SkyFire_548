/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Player.h"
#include "ragefire_chasm.h"

class instance_ragefire_chasm : public InstanceMapScript
{
    public:
        instance_ragefire_chasm() : InstanceMapScript("instance_ragefire_chasm", 389) { }

        struct instance_ragefire_chasm_InstanceMapScript : public InstanceScript
        {
            instance_ragefire_chasm_InstanceMapScript(Map* map) : InstanceScript(map) { }

            uint64 AdaroggGUID;
            uint64 CoranthalGUID;
            uint64 SlagmawGUID;
            uint64 GordothGUID;

            void Initialize() OVERRIDE
            {
                SetBossNumber(TOTAL_ENCOUNTERS);

                AdaroggGUID = 0;
                CoranthalGUID = 0;
                SlagmawGUID = 0;
                GordothGUID = 0;
                TeamInInstance = 0;
            }

            uint32 TeamInInstance;
            std::vector<uint64> HordeNpcGUIDs;

            void OnPlayerEnter(Player* player) OVERRIDE
            {
                if (TeamInInstance)
                    return;

                TeamInInstance = player->GetTeam();

                for (uint64 guid : HordeNpcGUIDs)
                    if (Creature* creature = instance->GetCreature(guid))
                        UpdateHordeNpc(creature);
            }

            void UpdateHordeNpc(Creature* creature)
            {
                if (TeamInInstance != ALLIANCE)
                    return;

                creature->SetVisible(false);
                creature->SetReactState(REACT_PASSIVE);
                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                creature->SetUInt32Value(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                if (!TeamInInstance)
                {
                    Map::PlayerList const& players = instance->GetPlayers();
                    if (!players.isEmpty())
                        if (Player* player = players.begin()->GetSource())
                            TeamInInstance = player->GetTeam();
                }

                switch (creature->GetEntry())
                {
                    case NPC_BOVAAL_WHITEHORN:
                    case NPC_STONE_GUARD_KURJACK:
                    case NPC_ORGRIMMAR_DEFENDER:
                    case NPC_COMMANDER_BAGRAN:
                        HordeNpcGUIDs.push_back(creature->GetGUID());
                        UpdateHordeNpc(creature);
                        break;
                    case NPC_ADAROGG:
                        AdaroggGUID = creature->GetGUID();
                        break;
                    case NPC_DARK_SHAMAN_CORANTHAL:
                        CoranthalGUID = creature->GetGUID();
                        break;
                    case NPC_SLAGMAW:
                        SlagmawGUID = creature->GetGUID();
                        break;
                    case NPC_GORDOTH:
                        GordothGUID = creature->GetGUID();
                        break;
                }
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case BOSS_ADAROGG:
                        return AdaroggGUID;
                    case BOSS_DARK_SHAMAN_CORANTHAL:
                        return CoranthalGUID;
                    case BOSS_SLAGMAW:
                        return SlagmawGUID;
                    case BOSS_GORDOTH:
                        return GordothGUID;
                }

                return 0;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "R F C " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* in) OVERRIDE
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2, dataHead3;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2 >> dataHead3;

                if (dataHead1 == 'R' && dataHead2 == 'F' && dataHead3 == 'C')
                {
                    for (uint8 i = 0; i < TOTAL_ENCOUNTERS; ++i)
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
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_ragefire_chasm_InstanceMapScript(map);
        }
};

void AddSC_instance_ragefire_chasm()
{
    new instance_ragefire_chasm();
}
