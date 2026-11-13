/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Player.h"
#include "WorldSession.h"
#include "SpellScript.h"
#include "SpellMgr.h"
#include "azjol_nerub.h"

DoorData const doorData[] =
{
    { GO_KRIKTHIR_DOOR,     DATA_KRIKTHIR_THE_GATEWATCHER,  DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GO_ANUBARAK_DOOR_1,   DATA_ANUBARAK,                  DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { GO_ANUBARAK_DOOR_2,   DATA_ANUBARAK,                  DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { GO_ANUBARAK_DOOR_3,   DATA_ANUBARAK,                  DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { 0,                    0,                              DOOR_TYPE_ROOM,     BOUNDARY_NONE } // END
};

class instance_azjol_nerub : public InstanceMapScript
{
    public:
        instance_azjol_nerub() : InstanceMapScript(AzjolNerubScriptName, 601) { }

        struct instance_azjol_nerub_InstanceScript : public InstanceScript
        {
            instance_azjol_nerub_InstanceScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);

                KrikthirGUID        = 0;
                HadronoxGUID        = 0;
                AnubarakGUID        = 0;
                WatcherGashraGUID   = 0;
                WatcherSilthikGUID  = 0;
                WatcherNarjilGUID   = 0;
                AnubarakWallGUID    = 0;
                AnubarakWall2GUID   = 0;
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_KRIKTHIR:
                        KrikthirGUID = creature->GetGUID();
                        break;
                    case NPC_HADRONOX:
                        HadronoxGUID = creature->GetGUID();
                        break;
                    case NPC_ANUBARAK:
                        AnubarakGUID = creature->GetGUID();
                        break;
                    case NPC_WATCHER_NARJIL:
                        WatcherNarjilGUID = creature->GetGUID();
                        break;
                    case NPC_WATCHER_GASHRA:
                        WatcherGashraGUID = creature->GetGUID();
                        break;
                    case NPC_WATCHER_SILTHIK:
                        WatcherSilthikGUID = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_ANUBARAK_DOOR_1:
                        AnubarakWallGUID = go->GetGUID();
                        AddDoor(go, true);
                        break;
                    case GO_ANUBARAK_DOOR_3:
                        AnubarakWall2GUID = go->GetGUID();
                        AddDoor(go, true);
                        break;
                    case GO_KRIKTHIR_DOOR:
                    case GO_ANUBARAK_DOOR_2:
                        AddDoor(go, true);
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_KRIKTHIR_DOOR:
                    case GO_ANUBARAK_DOOR_1:
                    case GO_ANUBARAK_DOOR_2:
                    case GO_ANUBARAK_DOOR_3:
                        AddDoor(go, false);
                        break;
                    default:
                        break;
                }
            }

            bool CheckRequiredBosses(uint32 bossId, Player const* player = NULL) const OVERRIDE
            {
                if (player && player->GetSession()->HasPermission(rbac::RBAC_PERM_SKIP_CHECK_INSTANCE_REQUIRED_BOSSES))
                    return true;

                if (bossId > DATA_KRIKTHIR_THE_GATEWATCHER && GetBossState(DATA_KRIKTHIR_THE_GATEWATCHER) != DONE)
                    return false;

                return true;
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_KRIKTHIR_THE_GATEWATCHER:
                        return KrikthirGUID;
                    case DATA_HADRONOX:
                        return HadronoxGUID;
                    case DATA_ANUBARAK:
                        return AnubarakGUID;
                    case DATA_WATCHER_GASHRA:
                        return WatcherGashraGUID;
                    case DATA_WATCHER_SILTHIK:
                        return WatcherSilthikGUID;
                    case DATA_WATCHER_NARJIL:
                        return WatcherNarjilGUID;
                    case DATA_ANUBARAK_WALL:
                        return AnubarakWallGUID;
                    case DATA_ANUBARAK_WALL_2:
                        return AnubarakWall2GUID;
                    default:
                        break;
                }

                return 0;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "A N " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* str) OVERRIDE
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

                if (dataHead1 == 'A' && dataHead2 == 'N')
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

        protected:
            uint64 KrikthirGUID;
            uint64 HadronoxGUID;
            uint64 AnubarakGUID;
            uint64 WatcherGashraGUID;
            uint64 WatcherSilthikGUID;
            uint64 WatcherNarjilGUID;
            uint64 AnubarakWallGUID;
            uint64 AnubarakWall2GUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_azjol_nerub_InstanceScript(map);
        }
};

enum DrainPowerSpells
{
    SPELL_DRAIN_POWER_AURA = 54315
};

class spell_azjol_drain_power : public SpellScript
{
    PrepareSpellScript(spell_azjol_drain_power);

    bool Validate(SpellInfo const* /*spell*/) OVERRIDE
    {
        return sSpellMgr->GetSpellInfo(SPELL_DRAIN_POWER_AURA) != NULL;
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetCaster(), SPELL_DRAIN_POWER_AURA, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_azjol_drain_power::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    }
};

void AddSC_instance_azjol_nerub()
{
   new instance_azjol_nerub();
   RegisterSpellScript(spell_azjol_drain_power);
}
