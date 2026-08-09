/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    std::string ReadFile(std::string const& path)
    {
        std::ifstream input(path.c_str());
        if (!input)
        {
            std::cerr << "Could not open file: " << path << '\n';
            return "";
        }

        std::ostringstream buffer;
        buffer << input.rdbuf();
        return buffer.str();
    }

    std::string ExtractFunction(std::string const& source, char const* signature)
    {
        size_t const start = source.find(signature);
        if (start == std::string::npos)
            return "";

        size_t const openBrace = source.find('{', start);
        if (openBrace == std::string::npos)
            return "";

        uint32_t depth = 0;
        for (size_t i = openBrace; i < source.size(); ++i)
        {
            if (source[i] == '{')
                ++depth;
            else if (source[i] == '}')
            {
                if (--depth == 0)
                    return source.substr(start, i - start + 1);
            }
        }

        return "";
    }

    bool Contains(std::string const& haystack, char const* needle)
    {
        return haystack.find(needle) != std::string::npos;
    }
}

int main()
{
    std::string const miscHandler =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Handlers/MiscHandler.cpp");
    std::string const setPvPHandler =
        ExtractFunction(miscHandler, "void WorldSession::HandleSetPvP(WorldPacket& recvData)");
    std::string const player =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Entities/Player/Player.cpp");
    std::string const loadFromDB =
        ExtractFunction(player, "bool Player::LoadFromDB(uint32 guid, SQLQueryHolder* holder)");
    std::string const outdoorPvP =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/OutdoorPvP/OutdoorPvP.cpp");
    std::string const capturePointEnter =
        ExtractFunction(outdoorPvP, "bool OPvPCapturePoint::HandlePlayerEnter(Player* player)");
    std::string const objectMgr =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Globals/ObjectMgr.cpp");
    std::string const addGOData =
        ExtractFunction(objectMgr, "uint32 ObjectMgr::AddGOData(uint32 entry, uint32 mapId, float x, float y, float z, float o, uint32 spawntimedelay, float rotation0, float rotation1, float rotation2, float rotation3)");
    std::string const outdoorPvPHP =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/scripts/OutdoorPvP/OutdoorPvPHP.cpp");
    std::string const outdoorPvPHPHeader =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/scripts/OutdoorPvP/OutdoorPvPHP.h");
    std::string const hellfireChangeState =
        ExtractFunction(outdoorPvPHP, "void OPvPCapturePointHP::ChangeState()");
    std::string const hellfireRestoreTowerState =
        ExtractFunction(outdoorPvPHP, "void OPvPCapturePointHP::RestoreTowerState()");
    std::string const hellfireSyncTowerFlagArtKit =
        ExtractFunction(outdoorPvPHP, "void OPvPCapturePointHP::SyncTowerFlagArtKit(uint32 artkit)");

    bool passed = true;
    passed &= Expect(!setPvPHandler.empty(),
        "HandleSetPvP should exist in MiscHandler.cpp.");
    passed &= Expect(Contains(setPvPHandler, "UpdatePvP(true, true)"),
        "CMSG_SET_PVP enable should update the server-side PvP state used by OutdoorPvP.");
    passed &= Expect(Contains(setPvPHandler, "pvpInfo.EndTimer = time(NULL)"),
        "CMSG_SET_PVP disable should use the normal delayed PvP-off timer.");
    passed &= Expect(!loadFromDB.empty(),
        "Player::LoadFromDB should exist in Player.cpp.");
    passed &= Expect(Contains(loadFromDB, "HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP)") &&
        Contains(loadFromDB, "SetPvP(true)"),
        "Player load should restore server-side PvP state from persisted player flags.");
    passed &= Expect(!capturePointEnter.empty(),
        "OutdoorPvP capture point enter handler should exist.");
    passed &= Expect(Contains(capturePointEnter, "PLAYER_FLAGS_IN_PVP") &&
        Contains(capturePointEnter, "PLAYER_FLAGS_PVP_TIMER") &&
        Contains(capturePointEnter, "UpdatePvP(true, true)"),
        "OutdoorPvP capture points should cancel the PvP-off timer while players are actively contesting them.");
    passed &= Expect(!addGOData.empty(),
        "ObjectMgr::AddGOData should exist.");
    passed &= Expect(Contains(addGOData, "data.phaseid = 0") &&
        Contains(addGOData, "data.phaseGroup = 0"),
        "Dynamically spawned outdoor PvP gameobjects should use the normal phase by default.");
    passed &= Expect(Contains(outdoorPvPHPHeader, "183514") &&
        Contains(outdoorPvPHPHeader, "182525") &&
        Contains(outdoorPvPHPHeader, "183515") &&
        Contains(outdoorPvPHPHeader, "{65, 62, 67}") &&
        Contains(outdoorPvPHPHeader, "{64, 61, 68}") &&
        Contains(outdoorPvPHPHeader, "{66, 63, 69}"),
        "Hellfire should define art-kit controlled tower banners for Broken Hill, Overlook, and Stadium.");
    passed &= Expect(!hellfireChangeState.empty(),
        "Hellfire OutdoorPvP ChangeState should exist.");
    passed &= Expect(Contains(outdoorPvPHP, "SyncTowerFlagArtKit") &&
        Contains(hellfireChangeState, "SyncTowerFlagArtKit(artkit2)"),
        "Hellfire tower ownership changes should update every nearby matching tower banner object.");
    passed &= Expect(!hellfireSyncTowerFlagArtKit.empty(),
        "Hellfire tower banner art kit sync helper should exist.");
    passed &= Expect(Contains(hellfireSyncTowerFlagArtKit, "GameObject::SetGoArtKit") &&
        Contains(hellfireSyncTowerFlagArtKit, "GUID_LOPART"),
        "Hellfire tower banner art kit sync should update GameObjectData even when the tower flag object is not loaded.");
    passed &= Expect(Contains(hellfireRestoreTowerState, "SyncTowerFlagArtKit"),
        "Hellfire tower ownership restore should refresh tower banner visuals after server restart.");
    passed &= Expect(Contains(outdoorPvPHP, "GetGameObjectListWithEntryInGrid"),
        "Hellfire tower banner visual sync should search nearby duplicate/static banner objects.");
    passed &= Expect(!Contains(outdoorPvPHP, "AddObject(GetCapturedTowerFlagType()") &&
        !Contains(hellfireChangeState, "UpdateCapturedTowerFlag()"),
        "Hellfire captured tower banners should use the existing pole art kits, not extra misplaced faction banner objects.");
    passed &= Expect(Contains(outdoorPvPHP, "SaveTowerState") &&
        Contains(outdoorPvPHP, "RestoreTowerState") &&
        Contains(outdoorPvPHP, "sWorld->setWorldState") &&
        Contains(outdoorPvPHP, "sWorld->getWorldState"),
        "Hellfire tower ownership should persist stable captured states across server restarts.");

    return passed ? 0 : 1;
}
