/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "the_botanica.h"

class instance_the_botanica : public InstanceMapScript
{
    public:
        instance_the_botanica() : InstanceMapScript("instance_the_botanica", 553) { }

        struct instance_the_botanica_InstanceMapScript : public InstanceScript
        {
            instance_the_botanica_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                CommanderSarannisGUID       = 0;
                HighBotanistFreywinnGUID    = 0;
                ThorngrinTheTenderGUID      = 0;
                LajGUID                     = 0;
                WarpSplinterGUID            = 0;
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch (creature->GetEntry())
                {
                    case NPC_COMMANDER_SARANNIS:
                        CommanderSarannisGUID = creature->GetGUID();
                        break;
                    case NPC_HIGH_BOTANIST_FREYWINN:
                        HighBotanistFreywinnGUID = creature->GetGUID();
                        break;
                    case NPC_THORNGRIN_THE_TENDER:
                        ThorngrinTheTenderGUID = creature->GetGUID();
                        break;
                    case NPC_LAJ:
                        LajGUID = creature->GetGUID();
                        break;
                    case NPC_WARP_SPLINTER:
                        WarpSplinterGUID = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            uint64 GetData64(uint32 type) const override
            {
                switch (type)
                {
                    case DATA_COMMANDER_SARANNIS:
                        return CommanderSarannisGUID;
                    case DATA_HIGH_BOTANIST_FREYWINN:
                        return HighBotanistFreywinnGUID;
                    case DATA_THORNGRIN_THE_TENDER:
                        return ThorngrinTheTenderGUID;
                    case DATA_LAJ:
                        return LajGUID;
                    case DATA_WARP_SPLINTER:
                        return WarpSplinterGUID;
                    default:
                        break;
                }

                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_COMMANDER_SARANNIS:
                    case DATA_HIGH_BOTANIST_FREYWINN:
                    case DATA_THORNGRIN_THE_TENDER:
                    case DATA_LAJ:
                    case DATA_WARP_SPLINTER:
                        break;
                    default:
                        break;
                }

                return true;
            }

            std::string GetSaveData() override
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "B O " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* str) override
            {
                if (!str)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(str);

                char dataHead1, dataHead2;

                std::istringstream loadStream(str);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'B' && dataHead2 == 'O')
                {
                    for (uint8 i = 0; i < EncounterCount; ++i)
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

        protected:
            uint64 CommanderSarannisGUID;
            uint64 HighBotanistFreywinnGUID;
            uint64 ThorngrinTheTenderGUID;
            uint64 LajGUID;
            uint64 WarpSplinterGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_the_botanica_InstanceMapScript(map);
        }
};

void AddSC_instance_the_botanica()
{
    new instance_the_botanica;
}
