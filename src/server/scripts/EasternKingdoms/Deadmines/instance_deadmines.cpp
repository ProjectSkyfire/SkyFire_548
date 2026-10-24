/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Player.h"
#include "deadmines.h"

#define MAX_ENCOUNTER 6

DoorData const doorData[] =
{
    { GO_FACTORY_DOOR,   DATA_GLUBTOK,   DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_MAST_ROOM_DOOR, DATA_HELIX,     DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_HEAVY_DOOR,     DATA_HELIX,     DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { GO_FOUNDRY_DOOR,   DATA_FOEREAPER, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_HEAVY_DOOR_2,   DATA_FOEREAPER, DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { 0,                 0,              DOOR_TYPE_ROOM,    BOUNDARY_NONE },
};

class instance_deadmines : public InstanceMapScript
{
    public:
        instance_deadmines() : InstanceMapScript("instance_deadmines", 36) { }

        struct instance_deadmines_InstanceMapScript : public InstanceScript
        {
            instance_deadmines_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(MAX_ENCOUNTER);
                LoadDoorData(doorData);

                uiGlubtokGUID        = 0;
                uiHelixGUID          = 0;
                uiOafGUID            = 0;
                uiFoereaperGUID      = 0;
                uiAdmiralGUID        = 0;
                uiCaptainGUID        = 0;

                GoblinTeleporterGUID = 0;
                HeavyDoorGUID        = 0;
                HeavyDoor2GUID       = 0;
                IronCladDoorGUID     = 0;
                DefiasCannonGUID     = 0;
                DoorLeverGUID        = 0;
                TeamInInstance       = 0;
                uiVanessaEvent       = 0;
                OverchargeCount      = 0;
                State                = CANNON_NOT_USED;
                instance->SetWorldState(WORLDSTATE_PROTOTYPE_PRODIGY, 1);
            }

            void OnPlayerEnter(Player* player) OVERRIDE
            {
                if (TeamInInstance)
                    return;

                TeamInInstance = player->GetTeam();

                for (uint64 guid : FactionNpcGUIDs)
                    if (Creature* creature = instance->GetCreature(guid))
                        UpdateFactionNpc(creature);
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
                    case NPC_GLUBTOK:
                        uiGlubtokGUID = creature->GetGUID();
                        break;
                    case NPC_HELIX_GEARBREAKER:
                        uiHelixGUID = creature->GetGUID();
                        break;
                    case NPC_LUMBERING_OAF:
                        uiOafGUID = creature->GetGUID();
                        break;
                    case NPC_FOE_REAPER_5000:
                        uiFoereaperGUID = creature->GetGUID();
                        break;
                    case NPC_ADMIRAL_RIPSNARL:
                        uiAdmiralGUID = creature->GetGUID();
                        break;
                    case NPC_CAPTAIN_COOKIE:
                        uiCaptainGUID = creature->GetGUID();
                        break;
                    case NPC_KAGTHA:
                    case NPC_SLINKY_SHARPSHIV:
                    case NPC_MISS_MAYHEM:
                    case NPC_MAYHEM_REAPER:
                    case NPC_HAND_ASSASIN:
                    case NPC_HORATIO_LAINE:
                    case NPC_DEFENDER:
                    case NPC_INVESTIGATOR:
                    case NPC_CRIME_SCENE_BOT:
                        FactionNpcGUIDs.push_back(creature->GetGUID());
                        UpdateFactionNpc(creature);
                        break;
                }
            }

            void UpdateFactionNpc(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_KAGTHA:
                    case NPC_SLINKY_SHARPSHIV:
                    case NPC_MISS_MAYHEM:
                    case NPC_MAYHEM_REAPER:
                    case NPC_HAND_ASSASIN:
                        if (TeamInInstance == ALLIANCE)
                            HideFactionNpc(creature);
                        break;
                    default:
                        if (TeamInInstance == HORDE)
                            HideFactionNpc(creature);
                        break;
                }
            }

            // The other faction's NPCs are hostile to the group, so they must not engage while hidden
            void HideFactionNpc(Creature* creature)
            {
                if (creature->IsInCombat())
                    creature->CombatStop(true);
                creature->DeleteThreatList();

                creature->SetVisible(false);
                creature->setFaction(35);
                creature->SetReactState(REACT_PASSIVE);
                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                creature->SetUInt32Value(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
            }

            void OnUnitDeath(Unit* unit) OVERRIDE
            {
                if (!unit->ToCreature())
                    return;

                switch (unit->GetEntry())
                {
                    case NPC_MINING_POWDER:
                        unit->CastSpell(unit, SPELL_EXPLODE_MINE, false);
                        break;
                    case NPC_DEFIAS_WATCHER:
                    case NPC_DEFIAS_REAPER:
                        OverchargeCount++;

                        if (OverchargeCount >= 4)
                            if (Creature* FoeReaper = instance->GetCreature(uiFoereaperGUID))
                                FoeReaper->RemoveAurasDueToSpell(SPELL_OFF_LINE);
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_HEAVY_DOOR:
                        HeavyDoorGUID = go->GetGUID();
                        AddDoor(go, true);
                        break;
                    case GO_HEAVY_DOOR_2:
                        HeavyDoor2GUID = go->GetGUID();
                        AddDoor(go, true);
                        break;
                    case GO_FACTORY_DOOR:
                    case GO_MAST_ROOM_DOOR:
                    case GO_FOUNDRY_DOOR:
                        AddDoor(go, true);
                        break;
                    case GO_GOBLIN_TELEPORTER:
                        GoblinTeleporterGUID = go->GetGUID();
                        break;
                    case GO_IRONCLAD_DOOR:
                        IronCladDoorGUID = go->GetGUID();
                        if (State != CANNON_NOT_USED)
                            go->SetGoState(GOState::GO_STATE_ACTIVE_ALTERNATIVE);
                        break;
                    case GO_DEFIAS_CANNON:
                        DefiasCannonGUID = go->GetGUID();
                        break;
                    case GO_DOOR_LEVER:
                        DoorLeverGUID = go->GetGUID();
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_HEAVY_DOOR:
                    case GO_HEAVY_DOOR_2:
                    case GO_FACTORY_DOOR:
                    case GO_MAST_ROOM_DOOR:
                    case GO_FOUNDRY_DOOR:
                        AddDoor(go, false);
                        break;
                }
            }

            void ShootCannon()
            {
                if (GameObject* pDefiasCannon = instance->GetGameObject(DefiasCannonGUID))
                {
                    pDefiasCannon->SetGoState(GOState::GO_STATE_ACTIVE);
                    pDefiasCannon->PlayDirectSound(SOUND_CANNONFIRE);
                }
            }

            void BlastOutDoor()
            {
                if (GameObject* pIronCladDoor = instance->GetGameObject(IronCladDoorGUID))
                {
                    pIronCladDoor->SetGoState(GOState::GO_STATE_ACTIVE_ALTERNATIVE);
                    pIronCladDoor->PlayDirectSound(SOUND_DESTROYDOOR);
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_CANNON_EVENT:
                        State = data;
                        if (data == CANNON_BLAST_INITIATED)
                        {
                            ShootCannon();
                            BlastOutDoor();
                            SaveToDB();
                        }
                        break;
                    case DATA_VANESSA_EVENT:
                        uiVanessaEvent = data;
                        if (data == DONE)
                            SaveToDB();
                        break;
                }
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_CANNON_EVENT:
                        return State;
                    case DATA_VANESSA_EVENT:
                        return uiVanessaEvent;
                    case DATA_TEAM_IN_INSTANCE:
                        return TeamInInstance;
                }

                return 0;
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_GLUBTOK:
                        return uiGlubtokGUID;
                    case DATA_HELIX:
                        return uiHelixGUID;
                    case DATA_OAF:
                        return uiOafGUID;
                    case DATA_FOEREAPER:
                        return uiFoereaperGUID;
                    case DATA_ADMIRAL:
                        return uiAdmiralGUID;
                    case DATA_CAPTAIN:
                        return uiCaptainGUID;
                }

                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_HELIX:
                        if (GameObject* go = instance->GetGameObject(HeavyDoorGUID))
                        {
                            if (state == IN_PROGRESS)
                                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                            else if (state == DONE)
                                go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        }
                        break;
                    case DATA_FOEREAPER:
                        if (GameObject* go = instance->GetGameObject(HeavyDoor2GUID))
                        {
                            if (state == IN_PROGRESS)
                                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                            else if (state == DONE)
                                go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        }
                        break;
                }

                return true;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "D M " << GetBossSaveData() << State << ' ' << uiVanessaEvent << ' ';

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

                char dataHead1, dataHead2;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'D' && dataHead2 == 'M')
                {
                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }

                    loadStream >> State;

                    if (State != CANNON_NOT_USED)
                        if (GameObject* pIronCladDoor = instance->GetGameObject(IronCladDoorGUID))
                            pIronCladDoor->SetGoState(GOState::GO_STATE_ACTIVE_ALTERNATIVE);

                    loadStream >> uiVanessaEvent;
                    if (uiVanessaEvent != DONE)
                        uiVanessaEvent = NOT_STARTED;
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            uint64 uiGlubtokGUID;
            uint64 uiHelixGUID;
            uint64 uiOafGUID;
            uint64 uiFoereaperGUID;
            uint64 uiAdmiralGUID;
            uint64 uiCaptainGUID;

            uint64 GoblinTeleporterGUID;
            uint64 HeavyDoorGUID;
            uint64 HeavyDoor2GUID;
            uint64 IronCladDoorGUID;
            uint64 DefiasCannonGUID;
            uint64 DoorLeverGUID;

            uint32 State;
            uint32 uiVanessaEvent;
            uint32 OverchargeCount;

            uint32 TeamInInstance;
            std::vector<uint64> FactionNpcGUIDs;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_deadmines_InstanceMapScript(map);
        }
};

void AddSC_instance_deadmines()
{
    new instance_deadmines();
}
