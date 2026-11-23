/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Instance_Karazhan
SD%Complete: 70
SDComment: Instance Script for Karazhan to help in various encounters. @todo GameObject visibility for Opera event.
SDCategory: Karazhan
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "InstanceScript.h"
#include "karazhan.h"

#define MAX_ENCOUNTER      12

/*
0  - Attumen + Midnight (optional)
1  - Moroes
2  - Maiden of Virtue (optional)
3  - Hyakiss the Lurker /  Rokad the Ravager  / Shadikith the Glider
4  - Opera Event
5  - Curator
6  - Shade of Aran (optional)
7  - Terestian Illhoof (optional)
8  - Netherspite (optional)
9  - Chess Event
10 - Prince Malchezzar
11 - Nightbane
*/

class instance_karazhan : public InstanceMapScript
{
public:
    instance_karazhan() : InstanceMapScript("instance_karazhan", 532) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
    {
        return new instance_karazhan_InstanceMapScript(map);
    }

    struct instance_karazhan_InstanceMapScript : public InstanceScript, public karazhan_accessor
    {
        instance_karazhan_InstanceMapScript(Map* map) : InstanceScript(map) { }

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strSaveData;

        uint32 m_uiOperaEvent;
        uint32 m_uiOzDeathCount;

        uint64 m_uiCurtainGUID;
        uint64 m_uiStageDoorLeftGUID;
        uint64 m_uiStageDoorRightGUID;
        uint64 m_uiKilrekGUID;
        uint64 m_uiTerestianGUID;
        uint64 m_uiMoroesGUID;
        uint64 m_uiLibraryDoor;                                     // Door at Shade of Aran
        uint64 m_uiMassiveDoor;                                     // Door at Netherspite
        uint64 m_uiSideEntranceDoor;                                // Side Entrance
        uint64 m_uiGamesmansDoor;                                   // Door before Chess
        uint64 m_uiGamesmansExitDoor;                               // Door after Chess
        uint64 m_uiNetherspaceDoor;                                // Door at Malchezaar
        uint64 MastersTerraceDoor[2];
        uint64 ImageGUID;
        uint64 DustCoveredChest;

        uint32 teamInInstance = 0;                                  // Team of the first player to enter, used for the Chess event
        uint32 chessResetTimer = 1;
        uint32 chessKickTimer = 0;
        uint8 allianceStalkerCount = 0;
        uint8 hordeStalkerCount = 0;
        bool friendlyGame = false;

        uint64 hordeStatusGuid = 0;
        uint64 allianceStatusGuid = 0;
        uint64 echoOfMedivhGUID = 0;
        uint64 chessVictoryControllerGUID = 0;

        std::list<uint64> chessHordeStalkerList;
        std::list<uint64> chessAllianceStalkerList;
        std::list<uint64> chessPiecesAlliance;
        std::list<uint64> chessPiecesHorde;
        std::list<uint64> boardSquares;
        std::vector<uint64> hordeStalkers;
        std::vector<uint64> allianceStalkers;

        void Initialize() OVERRIDE
        {
            memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

            // 1 - OZ, 2 - HOOD, 3 - RAJ, this never gets altered.
            m_uiOperaEvent      = urand(1, 3);
            m_uiOzDeathCount    = 0;

            m_uiCurtainGUID         = 0;
            m_uiStageDoorLeftGUID   = 0;
            m_uiStageDoorRightGUID  = 0;

            m_uiKilrekGUID      = 0;
            m_uiTerestianGUID   = 0;
            m_uiMoroesGUID      = 0;

            m_uiLibraryDoor         = 0;
            m_uiMassiveDoor         = 0;
            m_uiSideEntranceDoor    = 0;
            m_uiGamesmansDoor       = 0;
            m_uiGamesmansExitDoor   = 0;
            m_uiNetherspaceDoor     = 0;
            MastersTerraceDoor[0]= 0;
            MastersTerraceDoor[1]= 0;
            ImageGUID = 0;
            DustCoveredChest    = 0;
        }

        bool IsEncounterInProgress() const OVERRIDE
        {
            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (m_auiEncounter[i] == IN_PROGRESS)
                    return true;

            return false;
        }

        void OnPlayerEnter(Player* player) OVERRIDE
        {
            if (!teamInInstance)
                teamInInstance = player->GetTeam();

            player->RemoveAurasDueToSpell(SPELL_CONTROL_PIECE);
        }

        // A creature that was unloaded with its grid gets a new guid when it spawns again
        void AddChessGuid(std::list<uint64>& guids, uint64 guid)
        {
            guids.remove_if([this](uint64 oldGuid) { return !instance->GetCreature(oldGuid); });
            guids.push_back(guid);
        }

        void OnCreatureCreate(Creature* creature) OVERRIDE
        {
            switch (creature->GetEntry())
            {
                case 17229:   m_uiKilrekGUID = creature->GetGUID();      break;
                case 15688:   m_uiTerestianGUID = creature->GetGUID();   break;
                case 15687:   m_uiMoroesGUID = creature->GetGUID();      break;
                case NPC_INVISIBLE_STALKER:
                    AddChessGuid(creature->GetPositionY() < -1870.0f ? chessHordeStalkerList : chessAllianceStalkerList, creature->GetGUID());
                    break;
                case NPC_CHESS_STATUS_BAR:
                    if (creature->GetPositionY() < -1870.0f)
                        hordeStatusGuid = creature->GetGUID();
                    else
                        allianceStatusGuid = creature->GetGUID();
                    break;
                case NPC_HUMAN_CHARGER:
                case NPC_HUMAN_CLERIC:
                case NPC_HUMAN_CONJURER:
                case NPC_HUMAN_FOOTMAN:
                case NPC_CONJURED_WATER_ELEMENTAL:
                case NPC_KING_LLANE:
                    AddChessGuid(chessPiecesAlliance, creature->GetGUID());
                    break;
                case NPC_ORC_GRUNT:
                case NPC_ORC_NECROLYTE:
                case NPC_ORC_WARLOCK:
                case NPC_ORC_WOLF:
                case NPC_SUMMONED_DAEMON:
                case NPC_WARCHIEF_BLACKHAND:
                    AddChessGuid(chessPiecesHorde, creature->GetGUID());
                    break;
                case NPC_ECHO_MEDIVH:
                    echoOfMedivhGUID = creature->GetGUID();
                    break;
                case NPC_CHESS_VICTORY_CONTROLLER:
                    chessVictoryControllerGUID = creature->GetGUID();
                    break;
                case NPC_SQUARE_WHITE:
                case NPC_SQUARE_BLACK:
                    creature->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 0.0f);
                    AddChessGuid(boardSquares, creature->GetGUID());
                    break;
            }
        }

        void SetData(uint32 type, uint32 uiData) OVERRIDE
        {
            switch (type)
            {
                case TYPE_ATTUMEN:              m_auiEncounter[0] = uiData; break;
                case TYPE_MOROES:
                    if (m_auiEncounter[1] == DONE)
                        break;
                    m_auiEncounter[1] = uiData;
                    break;
                case TYPE_MAIDEN:               m_auiEncounter[2] = uiData; break;
                case TYPE_OPTIONAL_BOSS:        m_auiEncounter[3] = uiData; break;
                case TYPE_OPERA:
                    m_auiEncounter[4] = uiData;
                    if (uiData == DONE)
                        UpdateEncounterState(EncounterCreditType::ENCOUNTER_CREDIT_KILL_CREATURE, 16812, NULL);
                    break;
                case TYPE_CURATOR:              m_auiEncounter[5] = uiData; break;
                case TYPE_ARAN:                 m_auiEncounter[6] = uiData; break;
                case TYPE_TERESTIAN:            m_auiEncounter[7] = uiData; break;
                case TYPE_NETHERSPITE:          m_auiEncounter[8] = uiData; break;
                case TYPE_CHESS:
                    if (uiData == DONE)
                    {
                        // A friendly game (SPECIAL) gives neither the chest nor the credit again
                        if (GetData(TYPE_CHESS) != SPECIAL)
                        {
                            HandleGameObject(m_uiGamesmansExitDoor, true);
                            DoRespawnGameObject(DustCoveredChest, DAY);
                            UpdateEncounterState(EncounterCreditType::ENCOUNTER_CREDIT_KILL_CREATURE, NPC_CHESS_STATUS_BAR, NULL);
                        }

                        if (Creature* medivh = instance->GetCreature(echoOfMedivhGUID))
                        {
                            medivh->CastSpell(medivh, SPELL_FORCE_KILL_BUNNY, true);
                            medivh->CastSpell(medivh, SPELL_GAME_OVER, true);
                            medivh->CastSpell(medivh, SPELL_CLEAR_BOARD, true);
                        }
                        if (Creature* controller = instance->GetCreature(chessVictoryControllerGUID))
                            controller->CastSpell(controller, SPELL_VICTORY_VISUAL_CHESS, true);

                        friendlyGame = false;
                        chessResetTimer = GetData(TYPE_CHESS) != SPECIAL ? 35000 : 5000;
                        chessKickTimer = chessResetTimer + 1 - 2500;
                    }
                    else if (uiData == FAIL)
                    {
                        if (Creature* medivh = instance->GetCreature(echoOfMedivhGUID))
                        {
                            medivh->CastSpell(medivh, SPELL_GAME_OVER, true);
                            medivh->CastSpell(medivh, SPELL_CLEAR_BOARD, true);
                        }

                        chessResetTimer = 5000;
                        chessKickTimer = chessResetTimer + 1 - 2500;
                    }

                    m_auiEncounter[9] = uiData;
                    if (uiData == IN_PROGRESS || uiData == SPECIAL)
                        DoPrepareChessEvent();
                    break;
                case TYPE_MALCHEZZAR:           m_auiEncounter[10] = uiData; break;
                case TYPE_NIGHTBANE:
                    if (m_auiEncounter[11] != DONE)
                        m_auiEncounter[11] = uiData;
                    break;
                case DATA_OPERA_OZ_DEATHCOUNT:
                    if (uiData == SPECIAL)
                        ++m_uiOzDeathCount;
                    else if (uiData == IN_PROGRESS)
                        m_uiOzDeathCount = 0;
                    break;
            }

            if (uiData == DONE)
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << m_auiEncounter[0] << ' ' << m_auiEncounter[1] << ' ' << m_auiEncounter[2] << ' '
                    << m_auiEncounter[3] << ' ' << m_auiEncounter[4] << ' ' << m_auiEncounter[5] << ' ' << m_auiEncounter[6] << ' '
                    << m_auiEncounter[7] << ' ' << m_auiEncounter[8] << ' ' << m_auiEncounter[9] << ' ' << m_auiEncounter[10] << ' ' << m_auiEncounter[11];

                strSaveData = saveStream.str();

                SaveToDB();
                OUT_SAVE_INST_DATA_COMPLETE;
            }
        }

         void SetData64(uint32 identifier, uint64 data) OVERRIDE
         {
             switch (identifier)
             {
             case DATA_IMAGE_OF_MEDIVH: ImageGUID = data;
             }
         }

        void OnGameObjectCreate(GameObject* go) OVERRIDE
        {
            switch (go->GetEntry())
            {
                case 183932:   m_uiCurtainGUID          = go->GetGUID();         break;
                case 184278:
                    m_uiStageDoorLeftGUID = go->GetGUID();
                    if (m_auiEncounter[4] == DONE)
                        go->SetGoState(GOState::GO_STATE_ACTIVE);
                    break;
                case 184279:
                    m_uiStageDoorRightGUID = go->GetGUID();
                    if (m_auiEncounter[4] == DONE)
                        go->SetGoState(GOState::GO_STATE_ACTIVE);
                    break;
                case 184517:   m_uiLibraryDoor          = go->GetGUID();         break;
                case 185521:   m_uiMassiveDoor          = go->GetGUID();         break;
                case 184276:   m_uiGamesmansDoor        = go->GetGUID();         break;
                case 184277:
                    m_uiGamesmansExitDoor = go->GetGUID();
                    if (m_auiEncounter[9] == DONE || m_auiEncounter[9] == SPECIAL)
                        HandleGameObject(m_uiGamesmansExitDoor, true, go);
                    break;
                case 185134:   m_uiNetherspaceDoor      = go->GetGUID();         break;
                case 184274:    MastersTerraceDoor[0] = go->GetGUID();  break;
                case 184280:    MastersTerraceDoor[1] = go->GetGUID();  break;
                case 184275:
                    m_uiSideEntranceDoor = go->GetGUID();
                    if (m_auiEncounter[4] == DONE)
                        go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_LOCKED);
                    else
                        go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_LOCKED);
                    break;
                case 185119: DustCoveredChest = go->GetGUID(); break;
            }

            switch (m_uiOperaEvent)
            {
                /// @todo Set Object visibilities for Opera based on performance
                case EVENT_OZ:
                    break;

                case EVENT_HOOD:
                    break;

                case EVENT_RAJ:
                    break;
            }
        }

        std::string GetSaveData() OVERRIDE
        {
            return strSaveData;
        }

        uint32 GetData(uint32 uiData) const OVERRIDE
        {
            switch (uiData)
            {
                case TYPE_ATTUMEN:              return m_auiEncounter[0];
                case TYPE_MOROES:               return m_auiEncounter[1];
                case TYPE_MAIDEN:               return m_auiEncounter[2];
                case TYPE_OPTIONAL_BOSS:        return m_auiEncounter[3];
                case TYPE_OPERA:                return m_auiEncounter[4];
                case TYPE_CURATOR:              return m_auiEncounter[5];
                case TYPE_ARAN:                 return m_auiEncounter[6];
                case TYPE_TERESTIAN:            return m_auiEncounter[7];
                case TYPE_NETHERSPITE:          return m_auiEncounter[8];
                case TYPE_CHESS:                return m_auiEncounter[9];
                case TYPE_MALCHEZZAR:           return m_auiEncounter[10];
                case TYPE_NIGHTBANE:            return m_auiEncounter[11];
                case DATA_OPERA_PERFORMANCE:    return m_uiOperaEvent;
                case DATA_OPERA_OZ_DEATHCOUNT:  return m_uiOzDeathCount;
            }

            return 0;
        }

        uint64 GetData64(uint32 uiData) const OVERRIDE
        {
            switch (uiData)
            {
                case DATA_KILREK:                   return m_uiKilrekGUID;
                case DATA_TERESTIAN:                return m_uiTerestianGUID;
                case DATA_MOROES:                   return m_uiMoroesGUID;
                case DATA_GO_STAGEDOORLEFT:         return m_uiStageDoorLeftGUID;
                case DATA_GO_STAGEDOORRIGHT:        return m_uiStageDoorRightGUID;
                case DATA_GO_CURTAINS:              return m_uiCurtainGUID;
                case DATA_GO_LIBRARY_DOOR:          return m_uiLibraryDoor;
                case DATA_GO_MASSIVE_DOOR:          return m_uiMassiveDoor;
                case DATA_GO_SIDE_ENTRANCE_DOOR:    return m_uiSideEntranceDoor;
                case DATA_GO_GAME_DOOR:             return m_uiGamesmansDoor;
                case DATA_GO_GAME_EXIT_DOOR:        return m_uiGamesmansExitDoor;
                case DATA_GO_NETHER_DOOR:           return m_uiNetherspaceDoor;
                case DATA_MASTERS_TERRACE_DOOR_1:   return MastersTerraceDoor[0];
                case DATA_MASTERS_TERRACE_DOOR_2:   return MastersTerraceDoor[1];
                case DATA_IMAGE_OF_MEDIVH:          return ImageGUID;
                case NPC_ECHO_MEDIVH:               return echoOfMedivhGUID;
                case NPC_CHESS_VICTORY_CONTROLLER:  return chessVictoryControllerGUID;
            }

            return 0;
        }

        bool SetChessPiecesToSquares(std::list<uint64> const& pieces)
        {
            for (uint64 guid : pieces)
            {
                Creature* chessPiece = instance->GetCreature(guid);
                if (!chessPiece)
                    continue;

                chessPiece->RemoveAllAuras();

                Creature* square = GetClosestCreatureWithEntry(chessPiece, NPC_SQUARE_BLACK, 2.0f);
                if (!square)
                    square = GetClosestCreatureWithEntry(chessPiece, NPC_SQUARE_WHITE, 2.0f);
                if (!square)
                {
                    SF_LOG_ERROR("scripts", "Instance Karazhan: Failed to find the chess square for creature %u.", chessPiece->GetDBTableGUIDLow());
                    return false;
                }

                // lets the piece prepare for the start position of its square
                chessPiece->AI()->SetGUID(square->GetGUID());
            }

            return true;
        }

        void SortChessStalkers(std::list<uint64> const& stalkerGuids, uint64 statusGuid, std::vector<uint64>& sorted)
        {
            std::list<Creature*> stalkers;
            for (uint64 guid : stalkerGuids)
                if (Creature* stalker = instance->GetCreature(guid))
                    stalkers.push_back(stalker);

            if (stalkers.empty())
            {
                SF_LOG_ERROR("scripts", "Instance Karazhan: Failed to find the stalkers of one side for the Chess Event.");
                return;
            }

            Creature* status = instance->GetCreature(statusGuid);
            if (!status)
                return;

            stalkers.sort(Skyfire::ObjectDistanceOrderPred(status));
            for (Creature* stalker : stalkers)
                sorted.push_back(stalker->GetGUID());
        }

        void DoPrepareChessEvent() OVERRIDE
        {
            for (uint64 guid : boardSquares)
                if (Creature* square = instance->GetCreature(guid))
                    square->RemoveAllAuras();

            if (!SetChessPiecesToSquares(chessPiecesAlliance) || !SetChessPiecesToSquares(chessPiecesHorde))
                return;

            allianceStalkerCount = 0;
            hordeStalkerCount = 0;
            hordeStalkers.clear();
            allianceStalkers.clear();

            // the dead pieces line up next to the board in the order of the stalkers from their status bar
            SortChessStalkers(chessHordeStalkerList, hordeStatusGuid, hordeStalkers);
            SortChessStalkers(chessAllianceStalkerList, allianceStatusGuid, allianceStalkers);
        }

        uint32 GetPlayerTeam() OVERRIDE
        {
            if (!teamInInstance)
                if (!instance->GetPlayers().isEmpty())
                    if (Player* player = instance->GetPlayers().begin()->GetSource())
                        teamInInstance = player->GetTeam();

            return teamInInstance;
        }

        bool IsFriendlyGameReady() const OVERRIDE { return friendlyGame; }

        void DoMoveChessPieceToSides(uint32 spellId, uint32 faction) OVERRIDE
        {
            std::vector<uint64> const& stalkers = faction == FACTION_ID_CHESS_ALLIANCE ? allianceStalkers : hordeStalkers;
            uint8& count = faction == FACTION_ID_CHESS_ALLIANCE ? allianceStalkerCount : hordeStalkerCount;

            Creature* status = instance->GetCreature(faction == FACTION_ID_CHESS_ALLIANCE ? allianceStatusGuid : hordeStatusGuid);
            if (!status || stalkers.size() < count + 1U)
                return;

            // the transform spell needs this exact stalker, or the pieces would not line up
            if (Creature* stalker = instance->GetCreature(stalkers[count]))
            {
                status->CastSpell(stalker, spellId, true);
                ++count;
            }
        }

        void FinishChessEvent(uint32 faction) OVERRIDE
        {
            // the side of the lost king applauds the winner
            for (uint64 guid : (faction == FACTION_ID_CHESS_ALLIANCE ? hordeStalkers : allianceStalkers))
                if (Creature* stalker = instance->GetCreature(guid))
                    stalker->SetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE, EMOTE_STATE_APPLAUD);
        }

        std::list<uint64> const& GetChessPiecesByFaction(uint32 faction) const OVERRIDE
        {
            return faction == FACTION_ID_CHESS_ALLIANCE ? chessPiecesAlliance : chessPiecesHorde;
        }

        void ResetChessPieces(std::list<uint64> const& pieces)
        {
            for (uint64 guid : pieces)
            {
                if (Creature* piece = instance->GetCreature(guid))
                {
                    piece->StopMoving();
                    piece->GetMotionMaster()->Clear();
                    piece->GetMotionMaster()->MoveIdle();
                    piece->RemoveAllAuras();
                    piece->Respawn(true);
                }
            }
        }

        void ResetChessStalkers(std::list<uint64> const& stalkers)
        {
            for (uint64 guid : stalkers)
            {
                if (Creature* stalker = instance->GetCreature(guid))
                {
                    stalker->RemoveAllAuras();
                    stalker->Respawn(true);
                    stalker->SetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                }
            }
        }

        void Update(uint32 diff) OVERRIDE
        {
            if (chessKickTimer)
            {
                if (chessKickTimer <= diff)
                {
                    for (uint64 guid : chessPiecesAlliance)
                        if (Creature* piece = instance->GetCreature(guid))
                            piece->RemoveAurasByType(SPELL_AURA_MOD_CHARM);

                    for (uint64 guid : chessPiecesHorde)
                        if (Creature* piece = instance->GetCreature(guid))
                            piece->RemoveAurasByType(SPELL_AURA_MOD_CHARM);

                    chessKickTimer = 0;
                }
                else
                    chessKickTimer -= diff;
            }

            if (chessResetTimer)
            {
                // the first reset waits until the board is completely loaded
                if (chessPiecesAlliance.size() != 16 || chessPiecesHorde.size() != 16 || chessAllianceStalkerList.size() != 16 || chessHordeStalkerList.size() != 16)
                    return;

                if (chessResetTimer <= diff)
                {
                    ResetChessPieces(chessPiecesAlliance);
                    ResetChessPieces(chessPiecesHorde);
                    ResetChessStalkers(chessAllianceStalkerList);
                    ResetChessStalkers(chessHordeStalkerList);

                    if (Creature* controller = instance->GetCreature(chessVictoryControllerGUID))
                        controller->RemoveAurasDueToSpell(SPELL_VICTORY_VISUAL_CHESS);

                    if (GetData(TYPE_CHESS) == FAIL)
                        SetData(TYPE_CHESS, NOT_STARTED);
                    else if (GetData(TYPE_CHESS) == DONE)
                        friendlyGame = true;

                    chessResetTimer = 0;
                }
                else
                    chessResetTimer -= diff;
            }
        }

        void Load(char const* chrIn) OVERRIDE
        {
            if (!chrIn)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(chrIn);
            std::istringstream loadStream(chrIn);

            loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
                >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
                >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11];
            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (m_auiEncounter[i] == IN_PROGRESS)                // Do not load an encounter as "In Progress" - reset it instead.
                    m_auiEncounter[i] = NOT_STARTED;

            if (m_auiEncounter[9] == SPECIAL)
                m_auiEncounter[9] = DONE;
            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };
};

void AddSC_instance_karazhan()
{
    new instance_karazhan();
}
