////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "gate_setting_sun.hpp"

static BossScenarios const g_BossScenarios[] =
{
    { DataTypes::DATA_KIPTILAK, eScenarioDatas::Kiptilak    },
    { DataTypes::DATA_GADOK,    eScenarioDatas::Gadok       },
    { DataTypes::DATA_RIMOK,    eScenarioDatas::Rimok       },
    { DataTypes::DATA_RAIGONN,  eScenarioDatas::Raigonn     },
    { 0,                        0                           }
};

DoorData const doorData[] =
{
    { GO_KIPTILAK_WALLS,                     DATA_KIPTILAK,              DOOR_TYPE_ROOM,         BOUNDARY_E    },
    { GO_KIPTILAK_WALLS,                     DATA_KIPTILAK,              DOOR_TYPE_ROOM,         BOUNDARY_N    },
    { GO_KIPTILAK_WALLS,                     DATA_KIPTILAK,              DOOR_TYPE_ROOM,         BOUNDARY_S    },
    { GO_KIPTILAK_WALLS,                     DATA_KIPTILAK,              DOOR_TYPE_ROOM,         BOUNDARY_W    },
    { GO_KIPTILAK_EXIT_DOOR,                 DATA_KIPTILAK,              DOOR_TYPE_PASSAGE,      BOUNDARY_N    },
    { GO_RIMAK_AFTER_DOOR,                   DATA_RIMOK,                 DOOR_TYPE_ROOM,         BOUNDARY_S    },
    { GO_RAIGONN_DOOR,                       DATA_RAIGONN,               DOOR_TYPE_ROOM,         BOUNDARY_NE   },
    { GO_RAIGONN_AFTER_DOOR,                 DATA_RAIGONN,               DOOR_TYPE_PASSAGE,      BOUNDARY_E    },
    { 0,                                     0,                          DOOR_TYPE_ROOM,         BOUNDARY_NONE }, ///< END
};

class instance_gate_setting_sun : public InstanceMapScript
{
    public:
        instance_gate_setting_sun() : InstanceMapScript("instance_gate_setting_sun", 962) { }

        struct instance_gate_setting_sun_InstanceMapScript : public InstanceScript
        {
            uint64 kiptilakGuid;
            uint64 gadokGuid;
            uint64 rimokGuid;
            uint64 raigonnGuid;
            uint64 raigonWeakGuid;

            uint64 firstDoorGuid;
            uint64 fireSignalGuid;

            uint64 wallCGuid;
            uint64 m_WallBGuid;
            uint64 portalTempGadokGuid;

            uint32 cinematicTimer;
            uint8 cinematicEventProgress;

            std::list<uint64> bombarderGuids;
            std::list<uint64> bombStalkerGuids;
            std::list<uint64> mantidBombsGUIDs;
            std::list<uint64> rimokAddGenetarorsGUIDs;
            std::list<uint64> artilleryGUIDs;
            std::list<uint64> secondaryDoorGUIDs;

            uint32 dataStorage[MAX_DATA];

            uint32 m_CreatureKilled;

            uint64 m_GreatWallGuid;
            uint64 m_GreatWall2Guid;

            instance_gate_setting_sun_InstanceMapScript(Map* map) : InstanceScript(map) { }

            void Initialize()
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);

                kiptilakGuid            = 0;
                gadokGuid               = 0;
                rimokGuid               = 0;
                raigonnGuid             = 0;
                raigonWeakGuid          = 0;
            
                firstDoorGuid           = 0;

                cinematicTimer          = 0;
                cinematicEventProgress  = 0;

                wallCGuid               = 0;
                m_WallBGuid             = 0;
                portalTempGadokGuid     = 0;

                memset(dataStorage, 0, MAX_DATA * sizeof (uint32));

                bombarderGuids.clear();
                bombStalkerGuids.clear();
                mantidBombsGUIDs.clear();
                rimokAddGenetarorsGUIDs.clear();
                artilleryGUIDs.clear();
                secondaryDoorGUIDs.clear();

                if (instance->IsChallengeMode())
                    LoadScenariosInfos(g_BossScenarios, eScenarioDatas::ScenarioID);

                m_CreatureKilled = 0;

                instance->SetVisibilityRange(500.0f);
            }

            void OnDestroy(InstanceMap* pMap)
            {
                if (Creature* weakSpot = instance->GetCreature(GetData64(NPC_WEAK_SPOT)))
                    weakSpot->_ExitVehicle();
            }

            void OnPlayerEnter(Player* player)
            {
                InstanceScript::OnPlayerEnter(player);

                if (GetData(DATA_BRASIER_CLICKED) == NOT_STARTED)
                    player->SetPhaseMask(1, true);
                else
                    player->SetPhaseMask(2, true);
            }

            void OnPlayerExit(Player* player)
            {
                player->SetPhaseMask(1, true);
            }

            void OnCreatureKilled(Creature* p_Creature, Player* p_Player) override
            {
                if (!instance->IsChallengeMode() || !IsChallengeModeStarted() || m_CreatureKilled >= eScenarioDatas::KillCount)
                    return;

                if (p_Creature == nullptr)
                    return;

                /// Ennemies already killed
                if (m_ConditionCompleted)
                    return;

                if (!p_Creature->isElite() || p_Creature->IsDungeonBoss())
                    return;

                ++m_CreatureKilled;
                SendScenarioProgressUpdate(CriteriaProgressData(eScenarioDatas::Ennemies, m_CreatureKilled, m_InstanceGuid, time(NULL), m_BeginningTime, 0));

                if (m_CreatureKilled >= eScenarioDatas::KillCount)
                    m_ConditionCompleted = true;
            }

            void OnCreatureCreate(Creature* p_Creature)
            {
                switch (p_Creature->GetEntry())
                {
                    case NPC_KIPTILAK:
                    {
                        kiptilakGuid = p_Creature->GetGUID();
                        return;
                    }
                    case NPC_GADOK:
                    {
                        gadokGuid = p_Creature->GetGUID();
                        return;
                    }
                    case NPC_RIMOK:
                    {
                        rimokGuid = p_Creature->GetGUID();
                        p_Creature->SetDisplayId(INVISIBLE_CREATURE_MODEL);
                        p_Creature->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NOT_SELECTABLE);
                        p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                        return;
                    }
                    case NPC_RAIGONN:
                    {
                        raigonnGuid = p_Creature->GetGUID();
                        return;
                    }
                    case NPC_KRITHUK_BOMBARDER:
                    {
                        bombarderGuids.push_back(p_Creature->GetGUID());
                        return;
                    }
                    case NPC_BOMB_STALKER:
                    {
                        bombStalkerGuids.push_back(p_Creature->GetGUID());
                        return;
                    }
                    case NPC_ADD_GENERATOR:
                    {
                        rimokAddGenetarorsGUIDs.push_back(p_Creature->GetGUID());
                        return;
                    }
                    case NPC_ARTILLERY:
                    {
                        artilleryGUIDs.push_back(p_Creature->GetGUID());
                        return;
                    }
                    default:
                        return;
                }
            }

            virtual void OnCreatureRemove(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_KRITHUK_BOMBARDER:
                        for (std::list<uint64>::iterator it = bombarderGuids.begin(); it != bombarderGuids.end(); ++it)
                        {
                            if (*it == creature->GetGUID())
                            {
                                bombarderGuids.erase(it);
                                break;
                            }
                        }
                        break;
                    default:
                        return;
                }
            }

            void OnGameObjectCreate(GameObject* p_GameObject)
            {
                switch (p_GameObject->GetEntry())
                {
                    case GO_KIPTILAK_ENTRANCE_DOOR:
                        firstDoorGuid = p_GameObject->GetGUID();
                        break;
                    case GO_SIGNAL_FIRE:
                        fireSignalGuid = p_GameObject->GetGUID();
                        break;
                    case GO_KIPTILAK_WALLS:
                    case GO_KIPTILAK_EXIT_DOOR:
                    case GO_RIMAK_AFTER_DOOR:
                    case GO_RAIGONN_AFTER_DOOR:
                        AddDoor(p_GameObject, true);
                        return;
                    case GO_KIPTILAK_MANTID_BOMBS:
                        mantidBombsGUIDs.push_back(p_GameObject->GetGUID());
                        return;
                    case GO_GREATDOOR_SECOND_DOOR:
                        secondaryDoorGUIDs.push_back(p_GameObject->GetGUID());
                        HandleGameObject(p_GameObject->GetGUID(), true, p_GameObject);
                        return;
                    case GO_WALL_C:
                        wallCGuid = p_GameObject->GetGUID();
                        return;
                    case GO_PORTAL_TEMP_GADOK:
                        portalTempGadokGuid = p_GameObject->GetGUID();
                        return;
                    case ObjectsIds::ChallengeModeDoor:
                        m_ChallengeDoorGuid = p_GameObject->GetGUID();
                        return;
                    case GO_WALL_A:
                        p_GameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_DAMAGED);
                        return;
                    case GO_WALL_B:
                        m_WallBGuid = p_GameObject->GetGUID();
                        break;
                    case GO_GREAT_DOOR:
                        m_GreatWallGuid = p_GameObject->GetGUID();
                        break;
                    case GO_GREAT_DOOR_PHASE_2:
                        m_GreatWall2Guid = p_GameObject->GetGUID();
                        break;
                    default:
                        return;
                }
            }

            void OnGameObjectRemove(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case ObjectsIds::GO_KIPTILAK_WALLS:
                    case ObjectsIds::GO_KIPTILAK_EXIT_DOOR:
                    case ObjectsIds::GO_RIMAK_AFTER_DOOR:
                    case ObjectsIds::GO_RAIGONN_AFTER_DOOR:
                        AddDoor(p_GameObject, false);
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 id, EncounterState state)
            {
                if (!InstanceScript::SetBossState(id, state))
                    return false;

                switch (id)
                {
                    case DATA_KIPTILAK:
                    {
                        if (state == DONE)
                            for (auto itr: mantidBombsGUIDs)
                                if (GameObject* bomb = instance->GetGameObject(itr))
                                    bomb->SetPhaseMask(32768, true); // Set Invisible
                        break;
                    }
                    case DATA_GADOK:
                    {
                        if (GameObject* portal = instance->GetGameObject(portalTempGadokGuid))
                            portal->SetPhaseMask(state == IN_PROGRESS ? 4 : 3, true);
                        break;
                    }
                    case DATA_RIMOK:
                    {
                        uint8 generatorsCount = 0;

                        for (auto itr: secondaryDoorGUIDs)
                            HandleGameObject(itr, state != DONE);

                        for (auto itr: rimokAddGenetarorsGUIDs)
                        {
                            if (Creature* generator = instance->GetCreature(itr))
                            {
                                if (generator->AI())
                                {
                                    // There is 7 add generators, the middle one spawn saboteur
                                    if (state == IN_PROGRESS && (++generatorsCount == 4))
                                        generator->AI()->DoAction(SPECIAL);
                                    else
                                        generator->AI()->DoAction(state);
                                }
                            }
                        }

                        if (GameObject* l_GreatDoor = instance->GetGameObject(m_GreatWallGuid))
                            l_GreatDoor->SendGameObjectActivateAnimKit(2215);

                        if (GameObject* l_GreatDoor2 = instance->GetGameObject(m_GreatWall2Guid))
                            l_GreatDoor2->SendGameObjectActivateAnimKit(2349);

                        break;
                    }
                    case DATA_RAIGONN:
                    {
                        for (auto itr: artilleryGUIDs)
                        {
                            if (Creature* artillery = instance->GetCreature(itr))
                            {
                                artillery->ApplyModFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE, state != IN_PROGRESS);
                                artillery->ApplyModFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK, state == IN_PROGRESS);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }

                return true;
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_OPEN_FIRST_DOOR:
                    {
                        if (dataStorage[type] == DONE)
                            return;

                        HandleGameObject(firstDoorGuid, true);

                        dataStorage[type] = data;
                    
                        if (GameObject* firstDoor = instance->GetGameObject(firstDoorGuid))
                        {
                            if (Creature* trigger = firstDoor->SummonTrigger(firstDoor->GetPositionX(), firstDoor->GetPositionY(), firstDoor->GetPositionZ(), 0, 500))
                            {
                                std::list<Creature*> defensorList;
                                GetCreatureListWithEntryInGrid(defensorList, trigger, 65337, 20.0f);

                                trigger->CastSpell(trigger, 115456); // Explosion

                                for (auto itr: defensorList)
                                {
                                    uint8 random = rand() % 2;

                                    float posX = random ? 814.0f:  640.0f;
                                    float posY = random ? 2102.0f: 2112.0f;
                                    itr->KnockbackFrom(posX, posY, 25.0f, 20.0f);
                                    itr->DespawnOrUnsummon(1000);
                                }
                            }
                        }
                        break;
                    }
                    case DATA_BRASIER_CLICKED:
                    {
                        if (dataStorage[type] == DONE)
                            return;

                        if (GetBossState(DATA_GADOK) != DONE)
                            return;

                        Map::PlayerList const &PlayerList = instance->GetPlayers();
                        for (Map::PlayerList::const_iterator it = PlayerList.begin(); it != PlayerList.end(); ++it)
                        {
                            if (Player* player = it->getSource())
                            {
                                player->SendCinematicStart(CINEMATIC_SETTING_SUN);
                                player->SetPhaseMask(2, true);
                                player->NearTeleportTo(1370.0f, 2283.6f, 402.328f, 2.70f);
                            }
                        }

                        cinematicTimer = 100;
                        dataStorage[type] = data;

                        if (Creature* l_Rimok = instance->GetCreature(rimokGuid))
                        {
                            l_Rimok->RestoreDisplayId();
                            l_Rimok->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NOT_SELECTABLE);
                            l_Rimok->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        }

                        if (GameObject* l_WallC = instance->GetGameObject(wallCGuid))
                            l_WallC->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_DAMAGED);

                        break;
                    }
                    default:
                        if (type < MAX_DATA)
                            dataStorage[type] = data;
                        break;
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_OPEN_FIRST_DOOR:
                    default:
                        if (type < MAX_DATA)
                            return dataStorage[type];
                        break;
                }

                return 0;
            }

            void SetData64(uint32 type, uint64 value)
            {
                switch (type)
                {
                    case NPC_WEAK_SPOT:     raigonWeakGuid = value;     break;
                    default:                                            break;
                }
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case NPC_KIPTILAK:
                        return kiptilakGuid;
                    case NPC_GADOK:
                        return gadokGuid;
                    case NPC_RIMOK:
                        return rimokGuid;
                    case NPC_RAIGONN:
                        return raigonnGuid;
                    case NPC_WEAK_SPOT:
                        return raigonWeakGuid;
                    case DATA_RANDOM_BOMBARDER:
                        return JadeCore::Containers::SelectRandomContainerElement(bombarderGuids);
                    case DATA_RANDOM_BOMB_STALKER:
                        return JadeCore::Containers::SelectRandomContainerElement(bombStalkerGuids);
                    case GO_WALL_C:
                        return wallCGuid;
                    case GO_WALL_B:
                        return m_WallBGuid;
                    case GO_GREAT_DOOR:
                        return m_GreatWallGuid;
                    case GO_GREAT_DOOR_PHASE_2:
                        return m_GreatWall2Guid;
                    default:
                        break;
                }

                return 0;
            }

            void doEventCinematic()
            {
                switch (cinematicEventProgress)
                {
                    case 0:
                        // On allume le brasier & la meche
                        cinematicTimer = 6000;
                        break;
                    case 1:
                        if (GameObject* go = instance->GetGameObject(fireSignalGuid))
                            go->UseDoorOrButton();
                        cinematicTimer = 5000;
                        break;
                }

                ++cinematicEventProgress;
            }

            void Update(uint32 p_Diff)
            {
                ScheduleBeginningTimeUpdate(p_Diff);
                ScheduleChallengeStartup(p_Diff);
                ScheduleChallengeTimeUpdate(p_Diff);

                if (cinematicTimer)
                {
                    if (cinematicTimer <= p_Diff)
                        doEventCinematic();
                    else
                        cinematicTimer -= p_Diff;
                }
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_gate_setting_sun_InstanceMapScript(map);
        }
};

void AddSC_instance_gate_setting_sun()
{
    new instance_gate_setting_sun();
}
