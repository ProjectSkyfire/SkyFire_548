/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifndef SKYFIRE_SOURCE_DIR
#define SKYFIRE_SOURCE_DIR "."
#endif

namespace
{
    std::string ReadFile(char const* relativePath)
    {
        std::ifstream file(std::string(SKYFIRE_SOURCE_DIR) + "/" + relativePath, std::ios::in | std::ios::binary);
        std::ostringstream stream;
        stream << file.rdbuf();
        std::string text = stream.str();
        std::string::size_type pos = 0;
        while ((pos = text.find("\r\n", pos)) != std::string::npos)
            text.replace(pos, 2, "\n");
        while ((pos = text.find('\r')) != std::string::npos)
            text.replace(pos, 1, "\n");
        return text;
    }

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << "FAIL: " << message << std::endl;

        return condition;
    }

    bool Contains(std::string const& haystack, char const* needle)
    {
        return haystack.find(needle) != std::string::npos;
    }

    bool TestTransitionSwimFlyOpcodeMappings()
    {
        std::string const opcodesHeader = ReadFile("src/server/game/Server/Protocol/Opcodes.h");
        std::string const opcodesSource = ReadFile("src/server/game/Server/Protocol/Opcodes.cpp");
        std::string const movementSource = ReadFile("src/server/game/Movement/MovementStructures.cpp");

        bool passed = true;
        passed &= Expect(Contains(opcodesHeader, "CMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY_ACK,"),
            "Transition swim/fly client ACK should exist in the CMSG opcode enum");
        passed &= Expect(Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(CMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY_ACK, 0x11DB"),
            "Transition swim/fly client ACK should use opcode 0x11DB");
        passed &= Expect(Contains(opcodesSource,
            "&WorldSession::HandleMoveSetCanTransitionBetweenSwimAndFlyAck"),
            "Transition swim/fly client ACK should have an explicit movement handler");
        passed &= Expect(Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY,   0x11DB"),
            "Transition swim/fly server set packet should use opcode 0x11DB");
        passed &= Expect(!Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY,   0x0A03"),
            "Transition swim/fly server set packet should not use the stale opcode 0x0A03");
        passed &= Expect(Contains(movementSource,
            "case SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY:\n            return MoveSetCanTransitionBetweenSwimAndFly;"),
            "Transition swim/fly server set packet should have a movement sequence");
        passed &= Expect(Contains(movementSource,
            "case SMSG_MOVE_UNSET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY:\n            return MoveUnsetCanTransitionBetweenSwimAndFly;"),
            "Transition swim/fly server unset packet should have a movement sequence");

        return passed;
    }

    bool TestCollisionHeightUpdateOpcodeMapping()
    {
        std::string const opcodesSource = ReadFile("src/server/game/Server/Protocol/Opcodes.cpp");
        std::string const movementSource = ReadFile("src/server/game/Movement/MovementStructures.cpp");
        std::string const playerSource = ReadFile("src/server/game/Entities/Player/Player.cpp");

        bool passed = true;
        passed &= Expect(Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_COLLISION_HEIGHT,                   0x1812, STATUS_NEVER"),
            "Collision height update packet should be enabled for movement broadcasts");
        passed &= Expect(Contains(movementSource,
            "case SMSG_MOVE_UPDATE_COLLISION_HEIGHT:\n            return MovementUpdateCollisionHeight;"),
            "Collision height update packet should have a movement sequence");
        passed &= Expect(Contains(playerSource,
            "SMSG_MOVE_SET_COLLISION_HEIGHT, SMSG_MOVE_UPDATE_COLLISION_HEIGHT"),
            "Collision height updates should use the broadcast opcode from the player sender");

        return passed;
    }

    bool TestCompoundStateOpcodeMapping()
    {
        std::string const opcodesHeader = ReadFile("src/server/game/Server/Protocol/Opcodes.h");
        std::string const opcodesSource = ReadFile("src/server/game/Server/Protocol/Opcodes.cpp");
        std::string const movementSource = ReadFile("src/server/game/Movement/MovementStructures.cpp");

        bool passed = true;
        passed &= Expect(Contains(opcodesHeader, "SMSG_MOVE_SET_COMPOUND_STATE,"),
            "Compound state movement packet should exist in the SMSG opcode enum");
        passed &= Expect(Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_COMPOUND_STATE,                        0x0061, STATUS_UNHANDLED"),
            "Compound state movement packet should use opcode 0x0061 and remain unhandled until its payload is decoded");
        passed &= Expect(!Contains(movementSource, "case SMSG_MOVE_SET_COMPOUND_STATE:"),
            "Compound state movement packet should not resolve to a guessed movement sequence");

        return passed;
    }

    bool TestUpdateApplyMovementForceOpcodeMapping()
    {
        std::string const opcodesSource = ReadFile("src/server/game/Server/Protocol/Opcodes.cpp");
        std::string const movementSource = ReadFile("src/server/game/Movement/MovementStructures.cpp");
        std::string const playerHeader = ReadFile("src/server/game/Entities/Player/Player.h");
        std::string const playerSource = ReadFile("src/server/game/Entities/Player/Player.cpp");

        bool passed = true;
        passed &= Expect(Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_APPLY_MOVEMENT_FORCE,               0x0AB6, STATUS_NEVER"),
            "Update apply movement force packet should be enabled for movement broadcasts");
        passed &= Expect(Contains(movementSource,
            "MovementStatusElements const MovementUpdateApplyMovementForce[]"),
            "Update apply movement force packet should have a movement sequence");
        passed &= Expect(Contains(movementSource,
            "case SMSG_MOVE_UPDATE_APPLY_MOVEMENT_FORCE:\n            return MovementUpdateApplyMovementForce;"),
            "Update apply movement force packet should resolve to its movement sequence");
        passed &= Expect(Contains(playerHeader,
            "void SendApplyMovementForce(bool apply, Position const& source, float force = 0.0f);"),
            "Player should expose a movement-force send helper");
        passed &= Expect(Contains(playerHeader,
            "bool HasForcedMovement() const { return hasForcedMovement_; }"),
            "Player should expose active movement-force state");
        passed &= Expect(Contains(playerHeader,
            "bool hasForcedMovement_;"),
            "Player should store active movement-force state");
        passed &= Expect(Contains(playerSource,
            "hasForcedMovement_ = false;"),
            "Player should initialize movement-force state");
        passed &= Expect(Contains(playerSource,
            "WorldPacket data(SMSG_MOVE_APPLY_MOVEMENT_FORCE"),
            "Movement-force helper should send the direct apply packet to the controlled player");
        passed &= Expect(Contains(playerSource,
            "SMSG_MOVE_UPDATE_APPLY_MOVEMENT_FORCE"),
            "Movement-force helper should broadcast the update apply packet to observers");

        return passed;
    }

    bool TestUpdateRemoveMovementForceOpcodeMapping()
    {
        std::string const opcodesSource = ReadFile("src/server/game/Server/Protocol/Opcodes.cpp");
        std::string const movementSource = ReadFile("src/server/game/Movement/MovementStructures.cpp");
        std::string const playerSource = ReadFile("src/server/game/Entities/Player/Player.cpp");

        bool passed = true;
        passed &= Expect(Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(SMSG_MOVE_UPDATE_REMOVE_MOVEMENT_FORCE,              0x1464, STATUS_NEVER"),
            "Update remove movement force packet should be enabled for movement broadcasts");
        passed &= Expect(Contains(movementSource,
            "MovementStatusElements const MovementUpdateRemoveMovementForce[]"),
            "Update remove movement force packet should have a movement sequence");
        passed &= Expect(Contains(movementSource,
            "case SMSG_MOVE_UPDATE_REMOVE_MOVEMENT_FORCE:\n            return MovementUpdateRemoveMovementForce;"),
            "Update remove movement force packet should resolve to its movement sequence");
        passed &= Expect(Contains(playerSource,
            "SMSG_MOVE_UPDATE_REMOVE_MOVEMENT_FORCE"),
            "Movement-force helper should broadcast remove updates to observers");

        return passed;
    }

    bool TestSplineMoveCollisionEnableOpcodeMapping()
    {
        std::string const opcodesHeader = ReadFile("src/server/game/Server/Protocol/Opcodes.h");
        std::string const opcodesSource = ReadFile("src/server/game/Server/Protocol/Opcodes.cpp");
        std::string const movementSource = ReadFile("src/server/game/Movement/MovementStructures.cpp");

        bool passed = true;
        passed &= Expect(Contains(opcodesHeader, "SMSG_SPLINE_MOVE_COLLISION_ENABLE,"),
            "Spline move collision enable packet should exist in the SMSG opcode enum");
        passed &= Expect(Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_COLLISION_ENABLE,                   0x1826, STATUS_NEVER"),
            "Spline move collision enable packet should use opcode 0x1826");
        passed &= Expect(Contains(movementSource,
            "MovementStatusElements const SplineMoveCollisionEnable[]"),
            "Spline move collision enable packet should have a movement sequence");
        passed &= Expect(Contains(movementSource,
            "case SMSG_SPLINE_MOVE_COLLISION_ENABLE:\n            return SplineMoveCollisionEnable;"),
            "Spline move collision enable packet should resolve to its movement sequence");

        return passed;
    }

    bool TestSplineMoveCollisionDisableOpcodeMapping()
    {
        std::string const opcodesHeader = ReadFile("src/server/game/Server/Protocol/Opcodes.h");
        std::string const opcodesSource = ReadFile("src/server/game/Server/Protocol/Opcodes.cpp");
        std::string const movementSource = ReadFile("src/server/game/Movement/MovementStructures.cpp");

        bool passed = true;
        passed &= Expect(Contains(opcodesHeader, "SMSG_SPLINE_MOVE_COLLISION_DISABLE,"),
            "Spline move collision disable packet should exist in the SMSG opcode enum");
        passed &= Expect(Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(SMSG_SPLINE_MOVE_COLLISION_DISABLE,                  0x15B8, STATUS_NEVER"),
            "Spline move collision disable packet should use opcode 0x15B8");
        passed &= Expect(Contains(movementSource,
            "MovementStatusElements const SplineMoveCollisionDisable[]"),
            "Spline move collision disable packet should have a movement sequence");
        passed &= Expect(Contains(movementSource,
            "case SMSG_SPLINE_MOVE_COLLISION_DISABLE:\n            return SplineMoveCollisionDisable;"),
            "Spline move collision disable packet should resolve to its movement sequence");

        return passed;
    }
}

int main()
{
    bool const passed = TestTransitionSwimFlyOpcodeMappings()
        && TestCollisionHeightUpdateOpcodeMapping()
        && TestCompoundStateOpcodeMapping()
        && TestUpdateApplyMovementForceOpcodeMapping()
        && TestUpdateRemoveMovementForceOpcodeMapping()
        && TestSplineMoveCollisionEnableOpcodeMapping()
        && TestSplineMoveCollisionDisableOpcodeMapping();
    std::cout << (passed ? "Movement opcode table tests passed" : "Movement opcode table tests failed") << std::endl;
    return passed ? 0 : 1;
}
