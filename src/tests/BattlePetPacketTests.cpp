/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BattlePetPackets.h"

#include "BattlePet.h"
#include "BattlePetBattleState.h"
#include "BattlePetMgr.h"
#include "DBCEnums.h"
#include "DB2Enums.h"

#include <cmath>
#include <iostream>
#include <string>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << "FAIL: " << message << std::endl;

        return condition;
    }

    bool ExpectNear(float actual, float expected, float epsilon, char const* message)
    {
        return Expect(std::fabs(actual - expected) <= epsilon, message);
    }

    bool TestPetBattleRequestFailedPacketLayout()
    {
        bool passed = true;

        WorldPacket packet = Skyfire::BattlePetPackets::BuildRequestFailedPacket(16);
        passed &= Expect(packet.GetOpcode() == SMSG_PET_BATTLE_REQUEST_FAILED,
            "Pet battle request failure packet should use SMSG_PET_BATTLE_REQUEST_FAILED");

        packet.rpos(0);
        passed &= Expect(packet.ReadBit() == false,
            "Pet battle request failure packet should clear the unknown bit");

        uint8 reason = 0;
        packet >> reason;
        passed &= Expect(reason == 16,
            "Pet battle request failure packet should write the failure reason");

        return passed;
    }

    uint32 ReadPetBattleQueueStatusAndSlots(WorldPacket& packet, uint32 (&slotResults)[3])
    {
        packet.rpos(0);

        packet.ReadBit(); // requester guid byte 7
        packet.ReadBit(); // requester guid byte 2
        packet.ReadBit(); // requester guid byte 6
        packet.ReadBit(); // requester guid byte 1
        packet.ReadBit(); // has average wait time
        packet.ReadBit(); // requester guid byte 4
        packet.ReadBits(22);
        packet.ReadBit(); // requester guid byte 0
        packet.ReadBit(); // has client wait time
        packet.ReadBit(); // requester guid byte 3
        packet.ReadBit(); // requester guid byte 5

        uint8 guidByte = 0;
        uint32 value = 0;

        packet >> guidByte; // requester guid byte 2
        packet >> guidByte; // requester guid byte 4
        packet >> value; // ticket time
        packet >> guidByte; // requester guid byte 3
        packet >> value; // queue status
        uint32 const status = value;
        packet >> guidByte; // requester guid byte 6
        packet >> guidByte; // requester guid byte 1
        packet >> value; // ticket type
        packet >> guidByte; // requester guid byte 5
        packet >> guidByte; // requester guid byte 7
        packet >> value; // ticket id
        packet >> guidByte; // requester guid byte 0

        for (uint8 i = 0; i < 3; i++)
            packet >> slotResults[i];

        return status;
    }

    uint32 ReadPetBattleQueueStatus(WorldPacket& packet)
    {
        uint32 slotResults[3] = { };
        return ReadPetBattleQueueStatusAndSlots(packet, slotResults);
    }

    bool TestPetBattleQueueStatusPacketWritesRemovedStatus()
    {
        bool passed = true;

        WorldPacket queuedPacket = Skyfire::BattlePetPackets::BuildQueueStatusPacket(
            ObjectGuid(uint64(0x1122334455667788)), Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_QUEUED);
        passed &= Expect(ReadPetBattleQueueStatus(queuedPacket) == 1,
            "Pet battle queue status packet should write queued status");

        WorldPacket removedPacket = Skyfire::BattlePetPackets::BuildQueueStatusPacket(
            ObjectGuid(uint64(0x1122334455667788)), Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_REMOVED);
        passed &= Expect(ReadPetBattleQueueStatus(removedPacket) == 5,
            "Pet battle queue status packet should write removed status when leaving matchmaking");

        return passed;
    }

    bool TestPetBattleQueueStatusPacketWritesSlotResults()
    {
        bool passed = true;

        uint32 slotResults[3] =
        {
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_QUEUED,
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_SLOT_DEAD,
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_SLOT_EMPTY
        };

        WorldPacket packet = Skyfire::BattlePetPackets::BuildQueueStatusPacket(
            ObjectGuid(uint64(0x1122334455667788)),
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_QUEUED,
            slotResults);

        uint32 serializedSlotResults[3] = { };
        passed &= Expect(ReadPetBattleQueueStatusAndSlots(packet, serializedSlotResults)
            == Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_QUEUED,
            "Pet battle queue status packet should preserve queue status when slot results are supplied");
        passed &= Expect(serializedSlotResults[0] == Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_QUEUED,
            "Pet battle queue status packet should write first slot result");
        passed &= Expect(serializedSlotResults[1] == Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_SLOT_DEAD,
            "Pet battle queue status packet should write second slot result");
        passed &= Expect(serializedSlotResults[2] == Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_SLOT_EMPTY,
            "Pet battle queue status packet should write third slot result");

        return passed;
    }

    bool TestPetBattleFinalizeLocationPacketLayout()
    {
        bool passed = true;

        G3D::Vector3 origin(1.25f, 2.5f, 3.75f);
        G3D::Vector3 positions[2] =
        {
            G3D::Vector3(10.0f, 11.0f, 12.0f),
            G3D::Vector3(20.0f, 21.0f, 22.0f)
        };

        WorldPacket packet = Skyfire::BattlePetPackets::BuildFinalizeLocationPacket(
            origin, positions, true, 4.5f, true, 7);

        passed &= Expect(packet.GetOpcode() == SMSG_PET_BATTLE_FINALIZE_LOCATION,
            "Pet battle finalize location packet should use SMSG_PET_BATTLE_FINALIZE_LOCATION");

        packet.rpos(0);
        float value = 0.0f;

        packet >> value;
        passed &= ExpectNear(value, 1.25f, 0.001f,
            "Pet battle finalize location packet should write origin x first");
        packet >> value;
        passed &= ExpectNear(value, 2.5f, 0.001f,
            "Pet battle finalize location packet should write origin y second");

        packet >> value;
        passed &= ExpectNear(value, 11.0f, 0.001f,
            "Pet battle finalize location packet should write first team y before x");
        packet >> value;
        passed &= ExpectNear(value, 10.0f, 0.001f,
            "Pet battle finalize location packet should write first team x after y");
        packet >> value;
        passed &= ExpectNear(value, 12.0f, 0.001f,
            "Pet battle finalize location packet should write first team z after x");

        packet >> value;
        passed &= ExpectNear(value, 21.0f, 0.001f,
            "Pet battle finalize location packet should write second team y before x");
        packet >> value;
        passed &= ExpectNear(value, 20.0f, 0.001f,
            "Pet battle finalize location packet should write second team x after y");
        packet >> value;
        passed &= ExpectNear(value, 22.0f, 0.001f,
            "Pet battle finalize location packet should write second team z after x");

        packet >> value;
        passed &= ExpectNear(value, 3.75f, 0.001f,
            "Pet battle finalize location packet should write origin z after team positions");

        passed &= Expect(packet.ReadBit() == false,
            "Pet battle finalize location packet should clear missing-orientation bit when orientation follows");
        passed &= Expect(packet.ReadBit() == false,
            "Pet battle finalize location packet should clear missing-location-result bit when location result follows");

        uint32 locationResult = 0;
        packet >> locationResult;
        passed &= Expect(locationResult == 7,
            "Pet battle finalize location packet should write location result");

        packet >> value;
        passed &= ExpectNear(value, 4.5f, 0.001f,
            "Pet battle finalize location packet should write orientation");

        return passed;
    }

    bool TestPetBattleFacingLooksAtTeamCenter()
    {
        bool passed = true;

        G3D::Vector3 origin(0.0f, 0.0f, 0.0f);
        G3D::Vector3 positions[2] =
        {
            G3D::Vector3(10.0f, 2.0f, 0.0f),
            G3D::Vector3(10.0f, -2.0f, 0.0f)
        };

        passed &= ExpectNear(Skyfire::BattlePetPackets::CalculateBattlePetFacing(origin, positions),
            0.0f, 0.001f, "Pet battle facing should point from origin to the center of both team positions");

        positions[0] = G3D::Vector3(-2.0f, 10.0f, 0.0f);
        positions[1] = G3D::Vector3(2.0f, 10.0f, 0.0f);

        passed &= ExpectNear(Skyfire::BattlePetPackets::CalculateBattlePetFacing(origin, positions),
            1.57079637f, 0.001f, "Pet battle facing should use map-space atan2 orientation");

        return passed;
    }

    bool TestPetBattleFacingPreservesClientValue()
    {
        bool passed = true;

        G3D::Vector3 origin(0.0f, 0.0f, 0.0f);
        G3D::Vector3 positions[2] =
        {
            G3D::Vector3(10.0f, 2.0f, 0.0f),
            G3D::Vector3(10.0f, -2.0f, 0.0f)
        };

        passed &= ExpectNear(Skyfire::BattlePetPackets::ResolveBattlePetFacing(true, 3.25f, origin, positions),
            3.25f, 0.001f, "Pet battle facing should preserve the client-provided camera facing");
        passed &= ExpectNear(Skyfire::BattlePetPackets::ResolveBattlePetFacing(false, 3.25f, origin, positions),
            0.0f, 0.001f, "Pet battle facing should fall back to team-center facing when the client omits it");

        return passed;
    }

    bool TestPetBattleInitialUpdatePacketStartsWithEnvironmentAndTeamBits()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::InitialUpdatePetData pet = { };
        pet.Id = 0x0102030405060708;
        pet.Level = 25;
        pet.Quality = 3;
        pet.Power = 111;
        pet.MaxHealth = 999;
        pet.Speed = 222;
        pet.CurrentHealth = 777;
        pet.Xp = 12;
        pet.Flags = 5;
        pet.Nickname = "Biter";
        pet.Species = 456;

        WorldPacket packet = Skyfire::BattlePetPackets::BuildInitialUpdatePacket(
            ObjectGuid(uint64(0x1122334455667788)), pet);

        passed &= Expect(packet.GetOpcode() == SMSG_PET_BATTLE_INITIAL_UPDATE,
            "Pet battle initial update packet should use SMSG_PET_BATTLE_INITIAL_UPDATE");

        packet.rpos(0);
        for (uint8 i = 0; i < 3; i++)
        {
            passed &= Expect(packet.ReadBits(21) == 0,
                "Pet battle initial update packet should begin with empty environment aura counts");
            passed &= Expect(packet.ReadBits(21) == 0,
                "Pet battle initial update packet should begin with empty environment state counts");
        }

        passed &= Expect(packet.ReadBit() == true,
            "Pet battle initial update packet should set trap status for the first team");
        passed &= Expect(packet.ReadBits(2) == 1,
            "Pet battle initial update packet should write one battle pet for the first team");
        passed &= Expect(packet.ReadBit() == false,
            "Pet battle initial update packet should clear the zero character guid bit");

        passed &= Expect(packet.ReadBits(21) == 0,
            "Pet battle initial update packet should write empty pet aura counts");
        passed &= Expect(packet.ReadBit() == true,
            "Pet battle initial update packet should write pet guid byte 3 mask");
        passed &= Expect(packet.ReadBits(21) == 0,
            "Pet battle initial update packet should write empty pet state counts");
        passed &= Expect(packet.ReadBit() == true,
            "Pet battle initial update packet should write pet guid byte 0 mask");
        passed &= Expect(packet.ReadBit() == false,
            "Pet battle initial update packet should clear pet flags bit");
        passed &= Expect(packet.ReadBit() == true,
            "Pet battle initial update packet should write pet guid byte 5 mask");
        passed &= Expect(packet.ReadBit() == true,
            "Pet battle initial update packet should write pet guid byte 1 mask");

        return passed;
    }

    void SkipInitialUpdatePetBits(WorldPacket& packet)
    {
        packet.ReadBits(21);
        packet.ReadBit();
        packet.ReadBits(21);
        packet.ReadBit();
        packet.ReadBit();
        packet.ReadBits(2);
        packet.ReadBits(20);
        packet.ReadBit();
        packet.ReadBits(7);
        packet.ReadBits(2);
        packet.ReadBits(3);
        packet.ReadBits(2);
    }

    void SkipInitialUpdateTeamTailBits(WorldPacket& packet)
    {
        packet.ReadBit();
        packet.ReadBit();
        packet.ReadBits(7);
    }

    void SkipInitialUpdateBitSection(WorldPacket& packet, uint8 firstTeamPetCount, uint8 secondTeamPetCount)
    {
        uint8 teamPetCounts[2] = { firstTeamPetCount, secondTeamPetCount };

        for (uint8 i = 0; i < 3; i++)
        {
            packet.ReadBits(21);
            packet.ReadBits(21);
        }

        for (uint8 i = 0; i < 2; i++)
        {
            packet.ReadBit();
            packet.ReadBits(2);
            packet.ReadBit();

            for (uint8 j = 0; j < teamPetCounts[i]; j++)
                SkipInitialUpdatePetBits(packet);

            SkipInitialUpdateTeamTailBits(packet);
        }

        packet.ReadBit();
        packet.ReadBit();
        packet.ReadBit();
        packet.ReadBit();
        packet.ReadBit();
        packet.ReadBit();
        packet.ReadBit();
        packet.ReadBits(8);
        packet.ReadBit();
        packet.ReadBit();
    }

    void ReadInitialUpdatePetBytes(WorldPacket& packet, uint8& teamIndex, uint32& firstAbility)
    {
        uint32 value32 = 0;
        uint16 value16 = 0;
        uint8 value8 = 0;

        packet >> teamIndex;

        for (uint8 i = 0; i < 3; i++)
        {
            uint32 abilityId = 0;
            uint8 abilityIndex = 0;
            uint8 cooldown = 0;
            uint16 lockout = 0;
            uint16 flags = 0;

            packet >> abilityId;
            packet >> abilityIndex;
            packet >> cooldown;
            packet >> lockout;
            packet >> flags;

            if (i == 0)
                firstAbility = abilityId;
        }

        packet >> value32;
        packet >> value16;
        packet >> value16;
        packet >> value32;
        packet >> value32;
        packet >> value32;
        packet >> value32;
        packet >> value32;
        packet >> value32;
        packet >> value16;
        packet >> value8;
        packet >> value32;
    }

    void SkipInitialUpdateTeamBytes(WorldPacket& packet)
    {
        uint32 value32 = 0;
        uint8 value8 = 0;

        packet >> value32;
        packet >> value32;
        packet >> value8;
        packet >> value8;
    }

    bool TestPetBattleInitialUpdatePacketWritesPetAbilityIds()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::InitialUpdatePetData pet = { };
        pet.Level = 25;
        pet.Quality = 3;
        pet.Power = 111;
        pet.MaxHealth = 999;
        pet.Speed = 222;
        pet.CurrentHealth = 777;
        pet.Xp = 12;
        pet.Flags = 5;
        pet.Nickname = "Biter";
        pet.Species = 456;
        pet.Abilities[0] = 1001;
        pet.Abilities[1] = 1002;
        pet.Abilities[2] = 1003;

        WorldPacket packet = Skyfire::BattlePetPackets::BuildInitialUpdatePacket(
            ObjectGuid(uint64(0x1122334455667788)), &pet, 1);

        packet.rpos(0);
        SkipInitialUpdateBitSection(packet, 1, 0);

        uint8 teamIndex = 0;
        packet >> teamIndex;
        passed &= Expect(teamIndex == 0,
            "Pet battle initial update packet should write the first team pet index before abilities");

        uint32 expectedAbilities[3] = { 1001, 1002, 1003 };
        for (uint8 i = 0; i < 3; i++)
        {
            uint32 abilityId = 0;
            uint8 abilityIndex = 0;
            uint8 cooldown = 0;
            uint16 lockout = 0;
            uint16 flags = 0;

            packet >> abilityId;
            packet >> abilityIndex;
            packet >> cooldown;
            packet >> lockout;
            packet >> flags;

            passed &= Expect(abilityId == expectedAbilities[i],
                "Pet battle initial update packet should write configured pet ability ids");
            passed &= Expect(abilityIndex == i,
                "Pet battle initial update packet should preserve ability slot indexes");
            passed &= Expect(cooldown == 0,
                "Pet battle initial update packet should leave ability cooldown clear");
            passed &= Expect(lockout == 0,
                "Pet battle initial update packet should leave ability lockout clear");
            passed &= Expect(flags == 0,
                "Pet battle initial update packet should leave ability flags clear");
        }

        return passed;
    }

    bool TestPetBattleInitialUpdatePacketWritesMultipleTeamPets()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::InitialUpdateTeamData teams[2];
        teams[0].PetCount = 2;
        teams[0].FrontPet = 1;
        teams[0].Pets[0].Abilities[0] = 1001;
        teams[0].Pets[1].Abilities[0] = 2001;

        teams[1].PetCount = 1;
        teams[1].Pets[0].Abilities[0] = 3001;

        WorldPacket packet = Skyfire::BattlePetPackets::BuildInitialUpdatePacket(
            ObjectGuid(uint64(0x1122334455667788)), teams, 2);

        packet.rpos(0);
        SkipInitialUpdateBitSection(packet, 2, 1);

        uint8 teamIndex = 0;
        uint32 firstAbility = 0;
        ReadInitialUpdatePetBytes(packet, teamIndex, firstAbility);
        passed &= Expect(teamIndex == 0,
            "Pet battle initial update packet should write first ally pet team index");
        passed &= Expect(firstAbility == 1001,
            "Pet battle initial update packet should write first ally pet ability data");

        ReadInitialUpdatePetBytes(packet, teamIndex, firstAbility);
        passed &= Expect(teamIndex == 1,
            "Pet battle initial update packet should write second ally pet team index");
        passed &= Expect(firstAbility == 2001,
            "Pet battle initial update packet should write second ally pet ability data");

        SkipInitialUpdateTeamBytes(packet);

        ReadInitialUpdatePetBytes(packet, teamIndex, firstAbility);
        passed &= Expect(teamIndex == 0,
            "Pet battle initial update packet should reset pet team index for the enemy team");
        passed &= Expect(firstAbility == 3001,
            "Pet battle initial update packet should write enemy pet ability data after ally team data");

        return passed;
    }

    bool TestInitialUpdatePetDataUsesSpeciesAbilityLookup()
    {
        bool passed = true;
        uint16 const flags = BATTLE_PET_JOURNAL_FLAG_ABILITY_1 | BATTLE_PET_JOURNAL_FLAG_ABILITY_3;

        BattlePet battlePet(42, 7, "Biter", 123, 25, 4, 55, 66, 77, 88, ITEM_QUALITY_NORMAL, 3, flags);
        Skyfire::BattlePetPackets::InitialUpdatePetData petData =
            Skyfire::BattlePetPackets::BuildInitialUpdatePetData(battlePet);

        passed &= Expect(petData.Abilities[0] == 7330,
            "Initial update pet data should populate the first ability from species lookup");
        passed &= Expect(petData.Abilities[1] == 7331,
            "Initial update pet data should populate the second ability from species lookup");
        passed &= Expect(petData.Abilities[2] == 7332,
            "Initial update pet data should populate the third ability from species lookup");

        return passed;
    }

    bool TestWildInitialUpdatePetDataUsesGeneratedBreedAndQuality()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::InitialUpdatePetData normalPet =
            Skyfire::BattlePetPackets::BuildWildInitialUpdatePetData(0x100, 39, 5, ITEM_QUALITY_NORMAL, 0);
        Skyfire::BattlePetPackets::InitialUpdatePetData generatedPet =
            Skyfire::BattlePetPackets::BuildWildInitialUpdatePetData(0x101, 39, 5, ITEM_QUALITY_RARE, 8);

        passed &= Expect(generatedPet.Id == 0x101,
            "Wild initial update pet data should preserve the generated battle pet id");
        passed &= Expect(generatedPet.Species == 39,
            "Wild initial update pet data should preserve the species");
        passed &= Expect(generatedPet.Level == 5,
            "Wild initial update pet data should preserve the normalized level");
        passed &= Expect(generatedPet.Quality == ITEM_QUALITY_RARE,
            "Wild initial update pet data should preserve the generated quality");
        passed &= Expect(generatedPet.Power > normalPet.Power,
            "Wild initial update pet data should apply generated breed and quality to power");
        passed &= Expect(generatedPet.MaxHealth > normalPet.MaxHealth,
            "Wild initial update pet data should apply generated breed and quality to max health");
        passed &= Expect(generatedPet.Speed > normalPet.Speed,
            "Wild initial update pet data should apply generated breed and quality to speed");
        passed &= Expect(generatedPet.CurrentHealth == generatedPet.MaxHealth,
            "Wild initial update pet data should start the generated pet at full health");

        return passed;
    }

    bool TestBattlePetAbilityJournalFlagSlots()
    {
        bool passed = true;

        passed &= Expect(BattlePetAbilitySlotForJournalFlag(BATTLE_PET_JOURNAL_FLAG_ABILITY_1) == 0,
            "First battle pet ability flag should map to slot 0");
        passed &= Expect(BattlePetAbilitySlotForJournalFlag(BATTLE_PET_JOURNAL_FLAG_ABILITY_2) == 1,
            "Second battle pet ability flag should map to slot 1");
        passed &= Expect(BattlePetAbilitySlotForJournalFlag(BATTLE_PET_JOURNAL_FLAG_ABILITY_3) == 2,
            "Third battle pet ability flag should map to slot 2");
        passed &= Expect(BattlePetAbilitySlotForJournalFlag(BATTLE_PET_JOURNAL_FLAG_FAVORITES) == BATTLE_PET_ABILITY_SLOT_INVALID,
            "Non-ability battle pet journal flags should not map to an ability slot");

        return passed;
    }

    bool TestBattlePetHealthPercentUsesCeilingForNonZeroHeals()
    {
        bool passed = true;

        passed &= Expect(BattlePetHealthFromPercent(1000, 100) == 1000,
            "Full battle pet heal should restore max health");
        passed &= Expect(BattlePetHealthFromPercent(1000, 50) == 500,
            "Half battle pet heal should restore half health");
        passed &= Expect(BattlePetHealthFromPercent(1, 1) == 1,
            "Nonzero battle pet heal should restore at least one health when max health is one");
        passed &= Expect(BattlePetHealthFromPercent(1000, 0) == 0,
            "Zero percent battle pet heal should restore no health");

        return passed;
    }

    bool TestBattlePetExperienceLevelsAtThreshold()
    {
        bool passed = true;

        BattlePet battlePet(42, 39, 2, ITEM_QUALITY_NORMAL, 0);
        battlePet.SetDbState(BattlePetDbState::BATTLE_PET_DB_STATE_NONE);

        uint16 const awarded = battlePet.AddExperience(BattlePetExperienceForNextLevel(2) + 25);

        passed &= Expect(awarded == 225,
            "Battle pet experience should report the awarded amount");
        passed &= Expect(battlePet.GetLevel() == 3,
            "Battle pet experience should level up after crossing the threshold");
        passed &= Expect(battlePet.GetXp() == 25,
            "Battle pet experience should retain overflow xp after leveling");
        passed &= Expect(battlePet.GetCurrentHealth() == battlePet.GetMaxHealth(),
            "Battle pet experience level-up should refresh calculated health");
        passed &= Expect(battlePet.GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_SAVE,
            "Battle pet experience changes should mark the pet for save");

        return passed;
    }

    bool TestBattlePetExperienceCapsAtMaxLevel()
    {
        bool passed = true;

        BattlePet battlePet(43, 39, BATTLE_PET_MAX_LEVEL - 1, ITEM_QUALITY_NORMAL, 0);
        uint16 const awarded = battlePet.AddExperience(
            BattlePetExperienceForNextLevel(BATTLE_PET_MAX_LEVEL - 1) + 100);
        uint16 const ignored = battlePet.AddExperience(100);

        passed &= Expect(awarded == 2500,
            "Battle pet experience should report the final level award");
        passed &= Expect(battlePet.GetLevel() == BATTLE_PET_MAX_LEVEL,
            "Battle pet experience should cap at max level");
        passed &= Expect(battlePet.GetXp() == 0,
            "Battle pet experience should clear stored xp at max level");
        passed &= Expect(ignored == 0,
            "Battle pet experience should ignore awards after max level");

        return passed;
    }

    bool TestBattlePetExperienceRewardScalesByLevelDifference()
    {
        bool passed = true;

        passed &= Expect(BattlePetExperienceReward(5, 5) == 70,
            "Battle pet experience reward should use same-level enemy scaling");
        passed &= Expect(BattlePetExperienceReward(5, 8) == 119,
            "Battle pet experience reward should cap higher enemy level scaling at two levels");
        passed &= Expect(BattlePetExperienceReward(10, 9) == 72,
            "Battle pet experience reward should reduce for an enemy one level lower");
        passed &= Expect(BattlePetExperienceReward(10, 6) == 15,
            "Battle pet experience reward should keep the minimum low-level enemy multiplier");
        passed &= Expect(BattlePetExperienceReward(5, 5, 2) == 35,
            "Battle pet experience reward should split across participating pets");
        passed &= Expect(BattlePetExperienceReward(BATTLE_PET_MAX_LEVEL, BATTLE_PET_MAX_LEVEL) == 0,
            "Battle pet experience reward should ignore max-level pets");

        return passed;
    }

    bool TestBattlePetCapturedLevelPenalty()
    {
        bool passed = true;

        passed &= Expect(BattlePetCapturedLevel(15) == 15,
            "Captured battle pets below level 16 should keep their level");
        passed &= Expect(BattlePetCapturedLevel(16) == 15,
            "Captured battle pets from level 16 through 20 should lose one level");
        passed &= Expect(BattlePetCapturedLevel(20) == 19,
            "Captured battle pets at level 20 should lose one level");
        passed &= Expect(BattlePetCapturedLevel(21) == 19,
            "Captured battle pets from level 21 through 25 should lose two levels");
        passed &= Expect(BattlePetCapturedLevel(BATTLE_PET_MAX_LEVEL) == BATTLE_PET_MAX_LEVEL - 2,
            "Captured max-level battle pets should lose two levels");

        return passed;
    }

    bool TestBattlePetTrapAbilityForLoadoutFlags()
    {
        bool passed = true;

        passed &= Expect(BattlePetTrapAbilityForLoadoutFlags(BATTLE_PET_LOADOUT_SLOT_FLAG_NONE) == 0,
            "Battle pet loadouts without a trap flag should not expose a trap ability");
        passed &= Expect(BattlePetTrapAbilityForLoadoutFlags(BATTLE_PET_LOADOUT_TRAP) == BATTLE_PET_ABILITY_TRAP,
            "Battle pet loadouts with the basic trap flag should expose the trap ability");
        passed &= Expect(BattlePetTrapAbilityForLoadoutFlags(BATTLE_PET_LOADOUT_TRAP | BATTLE_PET_LOADOUT_STRONG_TRAP) == BATTLE_PET_ABILITY_STRONG_TRAP,
            "Battle pet loadouts with a stronger trap flag should prefer the stronger trap ability");
        passed &= Expect(BattlePetTrapAbilityForLoadoutFlags(BATTLE_PET_LOADOUT_TRAP | BATTLE_PET_LOADOUT_PRISTINE_TRAP) == BATTLE_PET_ABILITY_PRISTINE_TRAP,
            "Battle pet loadouts with a pristine trap flag should prefer the pristine trap ability");
        passed &= Expect(BattlePetTrapAbilityForLoadoutFlags(BATTLE_PET_LOADOUT_TRAP | BATTLE_PET_LOADOUT_GM_TRAP) == BATTLE_PET_ABILITY_GM_TRAP,
            "Battle pet loadouts with a GM trap flag should prefer the GM trap ability");

        return passed;
    }

    bool TestBattlePetAchievementLoadoutRewards()
    {
        bool passed = true;

        passed &= Expect(BattlePetLoadoutSlotForAchievement(BATTLE_PET_ACHIEVEMENT_NEWBIE) == BATTLE_PET_LOADOUT_SLOT_2,
            "Newbie should unlock the second battle pet loadout slot");
        passed &= Expect(BattlePetLoadoutSlotForAchievement(BATTLE_PET_ACHIEVEMENT_JUST_A_PUP) == BATTLE_PET_LOADOUT_SLOT_3,
            "Just a Pup should unlock the third battle pet loadout slot");
        passed &= Expect(BattlePetLoadoutSlotForAchievement(0) == BATTLE_PET_LOADOUT_SLOT_NONE,
            "Unrelated achievements should not unlock battle pet loadout slots");

        return passed;
    }

    bool TestBattlePetSpeciesFlagsAllowWildCapture()
    {
        bool passed = true;

        passed &= Expect(BattlePetSpeciesFlagsAllowWildCapture(BATTLE_PET_FLAG_NONE),
            "Battle pet species without restrictions should allow wild capture");
        passed &= Expect(!BattlePetSpeciesFlagsAllowWildCapture(BATTLE_PET_FLAG_NOT_TAMEABLE),
            "Battle pet species flagged not tameable should not allow wild capture");
        passed &= Expect(!BattlePetSpeciesFlagsAllowWildCapture(BATTLE_PET_FLAG_NOT_TAMEABLE | BATTLE_PET_FLAG_RELEASABLE),
            "Battle pet species should keep not tameable precedence over other flags");

        return passed;
    }

    bool TestBattlePetAchievementCriteriaIds()
    {
        bool passed = true;

        passed &= Expect(ACHIEVEMENT_CRITERIA_TYPE_PET_BATTLE_ACHIEVEMENT_POINTS == 81,
            "Pet battle achievement points should use criteria type 81");
        passed &= Expect(ACHIEVEMENT_CRITERIA_TYPE_COLLECT_BATTLE_PET == 96,
            "Collect battle pet should use criteria type 96");
        passed &= Expect(ACHIEVEMENT_CRITERIA_TYPE_COLLECT_BATTLE_PET_SPECIES == 155,
            "Collect battle pet species should use criteria type 155");
        passed &= Expect(ACHIEVEMENT_CRITERIA_TYPE_OWN_BATTLE_PET_COUNT == 156,
            "Own battle pet count should use criteria type 156");
        passed &= Expect(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLE_PET == 157,
            "Capture battle pet should use criteria type 157");
        passed &= Expect(ACHIEVEMENT_CRITERIA_TYPE_WIN_PET_BATTLE == 158,
            "Win pet battle should use criteria type 158");
        passed &= Expect(ACHIEVEMENT_CRITERIA_TYPE_REACH_BATTLE_PET_LEVEL == 160,
            "Reach battle pet level should use criteria type 160");
        passed &= Expect(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLE_PET2 == 161,
            "Capture battle pet zone credit should use criteria type 161");

        return passed;
    }

    bool TestBattlePetAchievementCriteriaPayload()
    {
        bool passed = true;
        uint32 const payload = BattlePetAchievementCriteriaPayload(ITEM_QUALITY_RARE, true, 34);
        uint32 const wildPayload = BattlePetAchievementCriteriaPayload(
            ITEM_QUALITY_UNCOMMON, BATTLE_PET_ACHIEVEMENT_SOURCE_WILD, 72);
        uint32 const duelPayload = BattlePetAchievementCriteriaPayload(
            ITEM_QUALITY_RARE, BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_DUEL, 34);
        uint32 const matchmakingPayload = BattlePetAchievementCriteriaPayload(
            ITEM_QUALITY_EPIC, BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING, 12);

        passed &= Expect(BattlePetAchievementCriteriaQuality(payload) == ITEM_QUALITY_RARE,
            "Battle pet achievement payload should keep quality in the low byte");
        passed &= Expect(BattlePetAchievementCriteriaIsPvp(payload),
            "Battle pet achievement payload should keep the PvP flag");
        passed &= Expect(BattlePetAchievementCriteriaHealthPercent(payload) == 34,
            "Battle pet achievement payload should keep health percentage");
        passed &= Expect(BattlePetAchievementCriteriaSource(wildPayload) == BATTLE_PET_ACHIEVEMENT_SOURCE_WILD,
            "Battle pet achievement payload should keep wild battle source");
        passed &= Expect(!BattlePetAchievementCriteriaIsPvp(wildPayload),
            "Battle pet achievement payload should not treat wild battles as PvP");
        passed &= Expect(!BattlePetAchievementCriteriaIsMatchmaking(wildPayload),
            "Battle pet achievement payload should not treat wild battles as matchmaking");
        passed &= Expect(BattlePetAchievementCriteriaSource(duelPayload) == BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_DUEL,
            "Battle pet achievement payload should keep PvP duel source");
        passed &= Expect(BattlePetAchievementCriteriaIsPvp(duelPayload),
            "Battle pet achievement payload should treat PvP duels as PvP");
        passed &= Expect(!BattlePetAchievementCriteriaIsMatchmaking(duelPayload),
            "Battle pet achievement payload should not treat PvP duels as matchmaking");
        passed &= Expect(BattlePetAchievementCriteriaSource(matchmakingPayload) == BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING,
            "Battle pet achievement payload should keep PvP matchmaking source");
        passed &= Expect(BattlePetAchievementCriteriaIsPvp(matchmakingPayload),
            "Battle pet achievement payload should treat PvP matchmaking as PvP");
        passed &= Expect(BattlePetAchievementCriteriaIsMatchmaking(matchmakingPayload),
            "Battle pet achievement payload should treat PvP matchmaking as matchmaking");
        passed &= Expect(BattlePetAchievementCriteriaQuality(matchmakingPayload) == ITEM_QUALITY_EPIC,
            "Battle pet achievement payload should keep quality with matchmaking source");
        passed &= Expect(BattlePetAchievementCriteriaHealthPercent(matchmakingPayload) == 12,
            "Battle pet achievement payload should keep health percentage with matchmaking source");
        passed &= Expect(std::string(BattlePetAchievementSourceName(BATTLE_PET_ACHIEVEMENT_SOURCE_WILD)) == "wild",
            "Battle pet achievement source name should describe wild battles");
        passed &= Expect(std::string(BattlePetAchievementSourceName(BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_DUEL)) == "pvp-duel",
            "Battle pet achievement source name should describe PvP duels");
        passed &= Expect(std::string(BattlePetAchievementSourceName(BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING)) == "pvp-matchmaking",
            "Battle pet achievement source name should describe PvP matchmaking");

        BattlePetAchievementContext context;
        context.Species = 42;
        context.FamilyMask = 1 << 8;
        context.Quality = ITEM_QUALITY_RARE;
        context.Source = BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING;
        context.HealthPercent = 25;
        context.Won = true;
        context.Captured = true;

        passed &= Expect(context.IsValid(),
            "Battle pet achievement context should require a species");
        passed &= Expect(context.IsPvp(),
            "Battle pet achievement context should expose PvP source");
        passed &= Expect(context.IsMatchmaking(),
            "Battle pet achievement context should expose matchmaking source");
        passed &= Expect(BattlePetAchievementCriteriaQuality(context.Payload()) == ITEM_QUALITY_RARE,
            "Battle pet achievement context payload should include quality");
        passed &= Expect(BattlePetAchievementCriteriaSource(context.Payload()) == BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING,
            "Battle pet achievement context payload should include source");
        passed &= Expect(BattlePetAchievementCriteriaHealthPercent(context.Payload()) == 25,
            "Battle pet achievement context payload should include health percentage");

        return passed;
    }

    bool TestBattlePetTimestampMarksPetForSave()
    {
        bool passed = true;

        BattlePet battlePet(44, 39, "", 0, 1, 0, 100, 100, 10, 10, ITEM_QUALITY_NORMAL, 0, 0);

        passed &= Expect(battlePet.GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_NONE,
            "Loaded battle pet should start with no pending save");

        battlePet.SetTimestamp(12345);

        passed &= Expect(battlePet.GetTimestamp() == 12345,
            "Battle pet timestamp setter should update the timestamp");
        passed &= Expect(battlePet.GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_SAVE,
            "Battle pet timestamp changes should mark the pet for save");

        return passed;
    }

    bool TestPetBattleInitialUpdatePacketAllowsEmptySecondTeam()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::InitialUpdatePetData pet = { };
        pet.Id = 0x0102030405060708;
        pet.Level = 25;
        pet.Quality = 3;
        pet.Power = 111;
        pet.MaxHealth = 999;
        pet.Speed = 222;
        pet.CurrentHealth = 777;
        pet.Xp = 12;
        pet.Flags = 5;
        pet.Nickname = "Biter";
        pet.Species = 456;

        WorldPacket packet = Skyfire::BattlePetPackets::BuildInitialUpdatePacket(
            ObjectGuid(uint64(0x1122334455667788)), &pet, 1);

        packet.rpos(0);
        for (uint8 i = 0; i < 3; i++)
        {
            packet.ReadBits(21);
            packet.ReadBits(21);
        }

        passed &= Expect(packet.ReadBit() == true,
            "Pet battle initial update packet should set trap status for the populated first team");
        passed &= Expect(packet.ReadBits(2) == 1,
            "Pet battle initial update packet should write one pet for the populated first team");
        packet.ReadBit();
        SkipInitialUpdatePetBits(packet);
        SkipInitialUpdateTeamTailBits(packet);

        passed &= Expect(packet.ReadBit() == true,
            "Pet battle initial update packet should still set trap status for the empty second team");
        passed &= Expect(packet.ReadBits(2) == 0,
            "Pet battle initial update packet should write zero pets for an empty second team");

        return passed;
    }

    bool TestPetBattleFirstRoundPacketSelectsFrontPets()
    {
        bool passed = true;

        WorldPacket packet = Skyfire::BattlePetPackets::BuildFirstRoundPacket(0, 0, 3);

        passed &= Expect(packet.GetOpcode() == SMSG_PET_BATTLE_FIRST_ROUND,
            "Pet battle first round packet should use SMSG_PET_BATTLE_FIRST_ROUND");

        packet.rpos(0);
        for (uint8 i = 0; i < 2; i++)
        {
            uint16 roundTime = 0;
            uint8 trapStatus = 0;
            uint8 inputFlags = 0;

            packet >> roundTime;
            packet >> trapStatus;
            packet >> inputFlags;

            passed &= Expect(roundTime == 0,
                "Pet battle first round packet should clear round time");
            passed &= Expect(trapStatus == 2,
                "Pet battle first round packet should keep initial trap status unavailable");
            passed &= Expect(inputFlags == 0,
                "Pet battle first round packet should clear initial input flags");
        }

        uint32 roundId = 1;
        packet >> roundId;
        passed &= Expect(roundId == 0,
            "Pet battle first round packet should write the supplied round id");

        passed &= Expect(packet.ReadBits(3) == 0,
            "Pet battle first round packet should start with no dead pets");
        passed &= Expect(packet.ReadBit() == false,
            "Pet battle first round packet should clear the unknown round bit");
        passed &= Expect(packet.ReadBits(22) == 2,
            "Pet battle first round packet should write two front-pet effects");

        for (uint8 i = 0; i < 2; i++)
        {
            passed &= Expect(packet.ReadBit() == false,
                "Front-pet effect should include an effect type");
            passed &= Expect(packet.ReadBit() == true,
                "Front-pet effect should omit source aura instance");
            passed &= Expect(packet.ReadBit() == true,
                "Front-pet effect should omit ability effect id");
            passed &= Expect(packet.ReadBits(25) == 1,
                "Front-pet effect should target one pet");
            passed &= Expect(packet.ReadBit() == true,
                "Front-pet effect should set the target block marker");
            passed &= Expect(packet.ReadBit() == false,
                "Front-pet effect should include a target pet index");
            passed &= Expect(packet.ReadBits(3) == 3,
                "Front-pet effect should use target type 3");
            passed &= Expect(packet.ReadBit() == true,
                "Front-pet effect should omit flags");
            passed &= Expect(packet.ReadBit() == true,
                "Front-pet effect should omit stack depth");
            passed &= Expect(packet.ReadBit() == false,
                "Front-pet effect should include a caster pet index");
        }

        passed &= Expect(packet.ReadBits(20) == 0,
            "Pet battle first round packet should start with no cooldowns");

        uint8 expectedPetIndexes[2] = { 0, 3 };
        for (uint8 i = 0; i < 2; i++)
        {
            uint8 targetPet = 0;
            uint8 casterPet = 0;
            uint8 effectType = 0;

            packet >> targetPet;
            packet >> casterPet;
            packet >> effectType;

            passed &= Expect(targetPet == expectedPetIndexes[i],
                "Front-pet effect target should match the selected pet index");
            passed &= Expect(casterPet == expectedPetIndexes[i],
                "Front-pet effect caster should match the selected pet index");
            passed &= Expect(effectType == 4,
                "Front-pet effect should use the front-pet change effect type");
        }

        uint8 nextState = 0;
        packet >> nextState;
        passed &= Expect(nextState == 2,
            "Pet battle first round packet should advance to the input state");

        return passed;
    }

    bool TestPetBattleFirstRoundPacketWritesTrapStatuses()
    {
        bool passed = true;

        WorldPacket packet = Skyfire::BattlePetPackets::BuildFirstRoundPacket(
            0, 0, 3, PET_BATTLE_TRAP_STATUS_HEALTH_TOO_HIGH, PET_BATTLE_TRAP_STATUS_UNAVAILABLE);

        packet.rpos(0);

        uint16 roundTime = 0;
        uint8 trapStatus = 0;
        uint8 inputFlags = 0;

        packet >> roundTime;
        packet >> trapStatus;
        packet >> inputFlags;

        passed &= Expect(trapStatus == PET_BATTLE_TRAP_STATUS_HEALTH_TOO_HIGH,
            "Pet battle first round packet should write ally trap status");

        packet >> roundTime;
        packet >> trapStatus;
        packet >> inputFlags;

        passed &= Expect(trapStatus == PET_BATTLE_TRAP_STATUS_UNAVAILABLE,
            "Pet battle first round packet should write enemy trap status");

        return passed;
    }

    bool TestPetBattleInputReadsAbilityMove()
    {
        bool passed = true;

        WorldPacket packet(CMSG_BATTLE_PET_WILD_REQUEST, 32);
        packet.WriteBit(0);                  // AbilityID present
        packet.WriteBit(0);                  // MoveType present
        packet.WriteBit(1);                  // DebugFlags omitted
        packet.WriteBit(1);                  // IgnoreAbandonPenalty
        packet.WriteBit(1);                  // NewFrontPet omitted
        packet.WriteBit(1);                  // BattleInterrupted omitted
        packet.WriteBit(0);                  // RoundID present
        packet.FlushBits();
        packet << uint32(42);
        packet << uint8(1);
        packet << uint32(7330);

        packet.rpos(0);
        Skyfire::BattlePetPackets::BattlePetInput input = Skyfire::BattlePetPackets::ReadBattlePetInput(packet);

        passed &= Expect(input.IgnoreAbandonPenalty,
            "Pet battle input should read the ignore-abandon-penalty bit");
        passed &= Expect(input.HasRoundID && input.RoundID == 42,
            "Pet battle input should read a present round id");
        passed &= Expect(input.HasMoveType && input.MoveType == 1,
            "Pet battle input should read a present move type");
        passed &= Expect(input.HasAbilityID && input.AbilityID == 7330,
            "Pet battle input should read a present ability id");
        passed &= Expect(!input.HasDebugFlags,
            "Pet battle input should preserve omitted debug flags");
        passed &= Expect(!input.HasNewFrontPet,
            "Pet battle input should preserve omitted front-pet swaps");
        passed &= Expect(!input.HasBattleInterrupted,
            "Pet battle input should preserve omitted interruption data");

        return passed;
    }

    bool TestPetBattleInputReadsSwapAndInterruption()
    {
        bool passed = true;

        WorldPacket packet(CMSG_BATTLE_PET_WILD_REQUEST, 32);
        packet.WriteBit(1);                  // AbilityID omitted
        packet.WriteBit(0);                  // MoveType present
        packet.WriteBit(0);                  // DebugFlags present
        packet.WriteBit(0);                  // IgnoreAbandonPenalty clear
        packet.WriteBit(0);                  // NewFrontPet present
        packet.WriteBit(0);                  // BattleInterrupted present
        packet.WriteBit(0);                  // RoundID present
        packet.FlushBits();
        packet << uint8(2);
        packet << uint32(99);
        packet << uint8(3);
        packet << uint8(0xAB);
        packet << int8(1);

        packet.rpos(0);
        Skyfire::BattlePetPackets::BattlePetInput input = Skyfire::BattlePetPackets::ReadBattlePetInput(packet);

        passed &= Expect(!input.IgnoreAbandonPenalty,
            "Pet battle input should preserve a clear ignore-abandon-penalty bit");
        passed &= Expect(input.HasBattleInterrupted && input.BattleInterrupted == 2,
            "Pet battle input should read interruption data before round data");
        passed &= Expect(input.HasRoundID && input.RoundID == 99,
            "Pet battle input should read the round id after interruption data");
        passed &= Expect(input.HasMoveType && input.MoveType == 3,
            "Pet battle input should read move type before optional debug flags");
        passed &= Expect(!input.HasAbilityID,
            "Pet battle input should preserve omitted ability id");
        passed &= Expect(input.HasDebugFlags && input.DebugFlags == 0xAB,
            "Pet battle input should read debug flags when present");
        passed &= Expect(input.HasNewFrontPet && input.NewFrontPet == 1,
            "Pet battle input should read new front pet after debug flags");

        return passed;
    }

    bool TestPetBattleInputCommandBuildsAbilityCommand()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetInput input;
        input.HasRoundID = true;
        input.RoundID = 42;
        input.HasAbilityID = true;
        input.AbilityID = 7330;

        Skyfire::BattlePetPackets::BattlePetInputCommand command =
            Skyfire::BattlePetPackets::BuildBattlePetInputCommand(input);

        passed &= Expect(command.Action == Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_ABILITY,
            "Pet battle input command should classify ability input when an ability id is present");
        passed &= Expect(command.HasRoundID && command.RoundID == 42,
            "Pet battle input command should preserve the round id for ability input");
        passed &= Expect(command.AbilityID == 7330,
            "Pet battle input command should preserve the ability id");

        return passed;
    }

    bool TestPetBattleInputCommandPrefersSwapCommand()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetInput input;
        input.HasRoundID = true;
        input.RoundID = 7;
        input.HasAbilityID = true;
        input.AbilityID = 7330;
        input.HasNewFrontPet = true;
        input.NewFrontPet = 2;

        Skyfire::BattlePetPackets::BattlePetInputCommand command =
            Skyfire::BattlePetPackets::BuildBattlePetInputCommand(input);

        passed &= Expect(command.Action == Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_SWAP,
            "Pet battle input command should prefer front-pet swaps over ability data");
        passed &= Expect(command.NewFrontPet == 2,
            "Pet battle input command should preserve the requested front pet");
        passed &= Expect(command.RoundID == 7,
            "Pet battle input command should preserve the round id for swap input");

        return passed;
    }

    bool TestPetBattleInputCommandBuildsForfeitCommand()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetInput input;
        input.HasRoundID = true;
        input.RoundID = 9;
        input.IgnoreAbandonPenalty = true;

        Skyfire::BattlePetPackets::BattlePetInputCommand command =
            Skyfire::BattlePetPackets::BuildBattlePetInputCommand(input);

        passed &= Expect(command.Action == Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_FORFEIT,
            "Pet battle input command should classify abandon input without ability or swap data as forfeit");
        passed &= Expect(command.IgnoreAbandonPenalty,
            "Pet battle input command should preserve the abandon-penalty flag");
        passed &= Expect(command.RoundID == 9,
            "Pet battle input command should preserve the round id for forfeit input");

        return passed;
    }

    bool TestPetBattleInputCommandBuildsTrapCommand()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetInput input;
        input.HasRoundID = true;
        input.RoundID = 11;
        input.HasMoveType = true;
        input.MoveType = Skyfire::BattlePetPackets::BATTLE_PET_INPUT_MOVE_TRAP;

        Skyfire::BattlePetPackets::BattlePetInputCommand command =
            Skyfire::BattlePetPackets::BuildBattlePetInputCommand(input);

        passed &= Expect(command.Action == Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_TRAP,
            "Pet battle input command should classify trap move input");
        passed &= Expect(command.MoveType == Skyfire::BattlePetPackets::BATTLE_PET_INPUT_MOVE_TRAP,
            "Pet battle input command should preserve trap move type");
        passed &= Expect(command.RoundID == 11,
            "Pet battle input command should preserve the round id for trap input");

        return passed;
    }

    bool TestPetBattleInputCommandBuildsMoveTypeForfeitCommand()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetInput input;
        input.HasRoundID = true;
        input.RoundID = 12;
        input.HasMoveType = true;
        input.MoveType = Skyfire::BattlePetPackets::BATTLE_PET_INPUT_MOVE_FORFEIT;

        Skyfire::BattlePetPackets::BattlePetInputCommand command =
            Skyfire::BattlePetPackets::BuildBattlePetInputCommand(input);

        passed &= Expect(command.Action == Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_FORFEIT,
            "Pet battle input command should classify forfeit move input");
        passed &= Expect(command.MoveType == Skyfire::BattlePetPackets::BATTLE_PET_INPUT_MOVE_FORFEIT,
            "Pet battle input command should preserve forfeit move type");
        passed &= Expect(command.RoundID == 12,
            "Pet battle input command should preserve the round id for forfeit input");

        return passed;
    }

    bool TestPetBattleInputCommandRequiresRound()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetInput input;
        input.HasAbilityID = true;
        input.AbilityID = 7330;

        Skyfire::BattlePetPackets::BattlePetInputCommand command =
            Skyfire::BattlePetPackets::BuildBattlePetInputCommand(input);

        passed &= Expect(command.Action == Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_NONE,
            "Pet battle input command should ignore actionable input without a round id");
        passed &= Expect(!command.HasRoundID,
            "Pet battle input command should preserve the missing round id marker");

        return passed;
    }

    bool TestPetBattleFirstPetSelectionReadsPetId()
    {
        bool passed = true;

        WorldPacket packet(CMSG_BATTLE_PET_WILD_REQUEST, 1);
        packet << uint8(2);

        packet.rpos(0);
        Skyfire::BattlePetPackets::BattlePetFirstPetSelection selection =
            Skyfire::BattlePetPackets::ReadBattlePetFirstPetSelection(packet);

        passed &= Expect(selection.PetID == 2,
            "Pet battle first pet selection should read the selected pet id");

        return passed;
    }

    bool TestPetBattleRequestUpdateReadsGuidAndCancelled()
    {
        bool passed = true;

        ObjectGuid guid = uint64(0x1122334455667788);

        WorldPacket packet(CMSG_BATTLE_PET_WILD_REQUEST, 16);
        packet.WriteGuidMask(guid, 6, 2, 3, 7, 0, 4);
        packet.WriteBit(1);
        packet.WriteGuidMask(guid, 5, 1);
        packet.WriteGuidBytes(guid, 3, 5, 6, 7, 1, 0, 2, 4);

        packet.rpos(0);
        Skyfire::BattlePetPackets::BattlePetRequestUpdate request =
            Skyfire::BattlePetPackets::ReadBattlePetRequestUpdate(packet);

        passed &= Expect(uint64(request.Guid) == uint64(guid),
            "Pet battle request update should reconstruct the request guid");
        passed &= Expect(request.Cancelled,
            "Pet battle request update should read the cancelled bit");

        return passed;
    }

    bool TestPetBattleFinishedPacketIsEmpty()
    {
        bool passed = true;

        WorldPacket packet = Skyfire::BattlePetPackets::BuildFinishedPacket();

        passed &= Expect(packet.GetOpcode() == SMSG_PET_BATTLE_FINISHED,
            "Pet battle finished packet should use SMSG_PET_BATTLE_FINISHED");
        passed &= Expect(packet.empty(),
            "Pet battle finished packet should not contain payload bytes");

        return passed;
    }

    bool TestPetBattleRoundResultPacketWritesEmptyRound()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetRoundResult round;
        round.RoundID = 5;
        round.InputFlags[1] = 4;
        round.TrapStatus[0] = 2;
        round.TrapStatus[1] = 1;

        WorldPacket packet = Skyfire::BattlePetPackets::BuildRoundResultPacket(round);

        passed &= Expect(packet.GetOpcode() == SMSG_PET_BATTLE_ROUND_RESULT,
            "Pet battle round result packet should use SMSG_PET_BATTLE_ROUND_RESULT");

        packet.rpos(0);
        passed &= Expect(packet.ReadBit() == false,
            "Pet battle round result packet should include the next battle state");
        passed &= Expect(packet.ReadBits(20) == 0,
            "Pet battle round result packet should start with no cooldowns");
        passed &= Expect(packet.ReadBits(22) == 0,
            "Pet battle round result packet should write an empty effect list");
        passed &= Expect(packet.ReadBits(3) == 0,
            "Pet battle round result packet should write no dead pets");

        for (uint8 i = 0; i < 2; i++)
        {
            uint8 inputFlags = 0;
            uint16 roundTime = 1;
            uint8 trapStatus = 0;

            packet >> inputFlags;
            packet >> roundTime;
            packet >> trapStatus;

            passed &= Expect(inputFlags == round.InputFlags[i],
                "Pet battle round result packet should write next input flags");
            passed &= Expect(roundTime == 0,
                "Pet battle round result packet should clear round time");
            passed &= Expect(trapStatus == round.TrapStatus[i],
                "Pet battle round result packet should write next trap status");
        }

        uint32 roundId = 0;
        uint8 nextState = 0;
        packet >> roundId;
        packet >> nextState;

        passed &= Expect(roundId == 5,
            "Pet battle round result packet should write the round id");
        passed &= Expect(nextState == 2,
            "Pet battle round result packet should write the next battle state");

        return passed;
    }

    bool TestPetBattleRoundResultPacketWritesCooldowns()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetRoundResult round;
        round.RoundID = 8;

        Skyfire::BattlePetPackets::BattlePetRoundCooldown cooldown;
        cooldown.AbilityID = 1234;
        cooldown.AbilitySlot = 1;
        cooldown.PetPBOID = 0;
        cooldown.Cooldown = 3;
        cooldown.Lockdown = 0;
        round.Cooldowns.push_back(cooldown);

        WorldPacket packet = Skyfire::BattlePetPackets::BuildRoundResultPacket(round);

        packet.rpos(0);
        passed &= Expect(packet.ReadBits(22) == 0,
            "Pet battle round result cooldown packet should still write an empty effect list");
        packet.ReadBit(); // has next battle state
        passed &= Expect(packet.ReadBits(3) == 0,
            "Pet battle round result cooldown packet should write no dead pets");
        passed &= Expect(packet.ReadBits(20) == 1,
            "Pet battle round result packet should write cooldown count");
        passed &= Expect(packet.ReadBit() == false,
            "Pet battle round result cooldown should include the source pet index");

        uint16 cooldownTurns = 0;
        uint16 lockdownTurns = 1;
        uint32 abilityId = 0;
        uint8 abilitySlot = 0;
        uint8 petPboid = 0;

        packet >> cooldownTurns;
        packet >> lockdownTurns;
        packet >> abilityId;
        packet >> abilitySlot;
        packet >> petPboid;

        passed &= Expect(cooldownTurns == 3,
            "Pet battle round result cooldown should write remaining cooldown turns");
        passed &= Expect(lockdownTurns == 0,
            "Pet battle round result cooldown should write lockdown turns");
        passed &= Expect(abilityId == 1234,
            "Pet battle round result cooldown should write ability id");
        passed &= Expect(abilitySlot == 1,
            "Pet battle round result cooldown should write ability slot");
        passed &= Expect(petPboid == 0,
            "Pet battle round result cooldown should write source pet index");

        return passed;
    }

    bool TestPetBattleRoundResultPacketWritesNonzeroLockdown()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetRoundResult round;
        round.RoundID = 9;

        Skyfire::BattlePetPackets::BattlePetRoundCooldown cooldown;
        cooldown.AbilityID = 170;
        cooldown.AbilitySlot = 0;
        cooldown.PetPBOID = 0;
        cooldown.Cooldown = 0;
        cooldown.Lockdown = 1;
        round.Cooldowns.push_back(cooldown);

        WorldPacket packet = Skyfire::BattlePetPackets::BuildRoundResultPacket(round);
        packet.rpos(0);
        packet.ReadBits(22);
        packet.ReadBit();
        packet.ReadBits(3);
        passed &= Expect(packet.ReadBits(20) == 1,
            "Pet battle round result lockdown packet should write cooldown count");
        packet.ReadBit();

        uint16 cooldownTurns = 0;
        uint16 lockdownTurns = 0;
        packet >> cooldownTurns;
        packet >> lockdownTurns;

        passed &= Expect(cooldownTurns == 0,
            "Pet battle round result should keep real cooldown at zero while channeling");
        passed &= Expect(lockdownTurns == 1,
            "Pet battle round result helper can still encode lockdown bytes when set manually");

        return passed;
    }

    bool TestPetBattleRoundResultPacketWritesMultipleDamageEffects()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetRoundResult round;
        round.RoundID = 2;
        round.Effects.push_back(Skyfire::BattlePetPackets::BuildDamageEffect(0, 3, 90, 333, 1));
        round.Effects.push_back(Skyfire::BattlePetPackets::BuildDamageEffect(0, 3, 80, 854, 2));
        round.Effects.push_back(Skyfire::BattlePetPackets::BuildDamageEffect(0, 3, 70, 855, 3));

        WorldPacket packet = Skyfire::BattlePetPackets::BuildRoundResultPacket(round);
        packet.rpos(0);
        passed &= Expect(packet.ReadBits(22) == 3,
            "Pet battle multi-hit round should write three damage effects");

        return passed;
    }

    bool TestPetBattleRoundResultPacketWritesDamageEffect()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetRoundEffectTarget target;
        target.PetX = 3;
        target.Type = 6;
        target.HasRemainingHealth = true;
        target.RemainingHealth = 77;

        Skyfire::BattlePetPackets::BattlePetRoundEffect effect;
        effect.CasterPBOID = 0;
        effect.PetBattleEffectType = 5;
        effect.Flags = 2;
        effect.TurnInstanceID = 1;
        effect.StackDepth = 1;
        effect.AbilityEffectID = 700;
        effect.SourceAuraInstanceID = 3;
        effect.Targets.push_back(target);

        Skyfire::BattlePetPackets::BattlePetRoundResult round;
        round.RoundID = 6;
        round.Effects.push_back(effect);
        round.DeadPets.push_back(3);

        WorldPacket packet = Skyfire::BattlePetPackets::BuildRoundResultPacket(round);

        packet.rpos(0);
        packet.ReadBit();
        packet.ReadBits(20);
        passed &= Expect(packet.ReadBits(22) == 1,
            "Pet battle round result packet should write one effect");

        passed &= Expect(packet.ReadBit() == false,
            "Round effect should include caster PBOID");
        passed &= Expect(packet.ReadBit() == false,
            "Round effect should include effect type");
        passed &= Expect(packet.ReadBit() == false,
            "Round effect should include effect flags");
        passed &= Expect(packet.ReadBit() == false,
            "Round effect should include turn instance id");
        passed &= Expect(packet.ReadBit() == false,
            "Round effect should include stack depth");
        passed &= Expect(packet.ReadBit() == false,
            "Round effect should include ability effect id");
        passed &= Expect(packet.ReadBits(25) == 1,
            "Round effect should write one target");
        passed &= Expect(packet.ReadBits(3) == 6,
            "Round effect target should write target type");
        passed &= Expect(packet.ReadBit() == false,
            "Damage target should include remaining health");
        passed &= Expect(packet.ReadBit() == false,
            "Round effect target should include pet index");
        passed &= Expect(packet.ReadBit() == false,
            "Round effect should include source aura instance");
        passed &= Expect(packet.ReadBits(3) == 1,
            "Round result should write one dead pet");

        uint16 turnInstance = 0;
        uint8 targetPet = 0;
        int32 remainingHealth = 0;
        uint16 flags = 0;
        uint16 sourceAura = 0;
        uint8 caster = 0;
        uint8 stackDepth = 0;
        uint32 abilityEffect = 0;
        uint8 effectType = 0;

        packet >> turnInstance;
        packet >> targetPet;
        packet >> remainingHealth;
        packet >> flags;
        packet >> sourceAura;
        packet >> caster;
        packet >> stackDepth;
        packet >> abilityEffect;
        packet >> effectType;

        passed &= Expect(turnInstance == 1,
            "Round effect should write turn instance id before target data");
        passed &= Expect(targetPet == 3,
            "Round effect target should write pet index");
        passed &= Expect(remainingHealth == 77,
            "Damage target should write remaining health");
        passed &= Expect(flags == 2,
            "Round effect should write flags after target data");
        passed &= Expect(sourceAura == 3,
            "Round effect should write source aura instance");
        passed &= Expect(caster == 0,
            "Round effect should write caster PBOID");
        passed &= Expect(stackDepth == 1,
            "Round effect should write stack depth");
        passed &= Expect(abilityEffect == 700,
            "Round effect should write ability effect id");
        passed &= Expect(effectType == 5,
            "Round effect should write effect type last");

        for (uint8 i = 0; i < 2; i++)
        {
            packet.read_skip<uint8>();
            packet.read_skip<uint16>();
            packet.read_skip<uint8>();
        }

        packet.read_skip<uint32>();
        packet.read_skip<uint8>();

        uint8 deadPet = 0;
        packet >> deadPet;
        passed &= Expect(deadPet == 3,
            "Round result should write dead pet indexes after round state");

        return passed;
    }

    bool TestPetBattleDamageRoundHelperMarksDeadTarget()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetRoundResult round =
            Skyfire::BattlePetPackets::BuildDamageRoundResult(7, 0, 3, 0, 700, true);

        passed &= Expect(round.RoundID == 7,
            "Damage round helper should preserve the supplied round id");
        passed &= Expect(round.Effects.size() == 1,
            "Damage round helper should create one damage effect");
        passed &= Expect(round.DeadPets.size() == 1 && round.DeadPets[0] == 3,
            "Damage round helper should mark defeated target pets as dead");

        Skyfire::BattlePetPackets::BattlePetRoundEffect const& effect = round.Effects[0];
        passed &= Expect(effect.CasterPBOID == 0,
            "Damage round helper should preserve the caster pet index");
        passed &= Expect(effect.StackDepth == 1,
            "Damage round helper should default to one stack depth");
        passed &= Expect(effect.AbilityEffectID == 700,
            "Damage round helper should preserve the ability effect id");
        passed &= Expect(effect.Targets.size() == 1,
            "Damage round helper should create one target");

        Skyfire::BattlePetPackets::BattlePetRoundEffectTarget const& target = effect.Targets[0];
        passed &= Expect(target.PetX == 3,
            "Damage round helper should preserve the target pet index");
        passed &= Expect(target.Type == 6,
            "Damage round helper should use the remaining-health target type");
        passed &= Expect(target.HasRemainingHealth && target.RemainingHealth == 0,
            "Damage round helper should write the target remaining health");

        return passed;
    }

    bool TestPetBattleSwapEffectHelperSelectsNewFrontPet()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetRoundEffect effect =
            Skyfire::BattlePetPackets::BuildPetSwapEffect(1, 2);

        passed &= Expect(effect.CasterPBOID == 1,
            "Swap effect helper should preserve the old front pet as caster");
        passed &= Expect(effect.PetBattleEffectType == 4,
            "Swap effect helper should use the front-pet change effect type");
        passed &= Expect(effect.Targets.size() == 1,
            "Swap effect helper should target one pet");
        passed &= Expect(effect.Targets[0].PetX == 2,
            "Swap effect helper should preserve the new front pet target");
        passed &= Expect(effect.Targets[0].Type == 3,
            "Swap effect helper should use target type 3 for selected front pets");

        return passed;
    }

    bool TestPetBattleFinalRoundHelperMarksWinnerAndAbandoned()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetFinalRoundPet allyPet;
        allyPet.Pboid = 0;
        allyPet.RemainingHealth = 50;
        allyPet.InitialLevel = 5;
        allyPet.NewLevel = 5;
        allyPet.MaxHealth = 100;

        Skyfire::BattlePetPackets::BattlePetFinalRoundPet enemyPet;
        enemyPet.Pboid = 3;
        enemyPet.RemainingHealth = 0;
        enemyPet.InitialLevel = 6;
        enemyPet.NewLevel = 6;
        enemyPet.MaxHealth = 120;

        Skyfire::BattlePetPackets::BattlePetFinalRound finalRound =
            Skyfire::BattlePetPackets::BuildFinalRoundState(false, true, allyPet, enemyPet);

        passed &= Expect(!finalRound.Winners[0] && finalRound.Winners[1],
            "Final round helper should mark the enemy team as winner when allyWon is false");
        passed &= Expect(finalRound.Abandoned,
            "Final round helper should preserve the abandoned flag");
        passed &= Expect(finalRound.Pets.size() == 2,
            "Final round helper should include both battle pets");
        passed &= Expect(finalRound.Pets[0].Pboid == 0 && finalRound.Pets[1].Pboid == 3,
            "Final round helper should preserve ally then enemy pet order");
        passed &= Expect(finalRound.Pets[1].RemainingHealth == 0,
            "Final round helper should preserve pet health fields");

        return passed;
    }

    bool TestPetBattleFinalRoundHelperPreservesMultiplePets()
    {
        bool passed = true;

        std::vector<Skyfire::BattlePetPackets::BattlePetFinalRoundPet> pets;
        for (uint8 i = 0; i < 3; ++i)
        {
            Skyfire::BattlePetPackets::BattlePetFinalRoundPet pet;
            pet.Pboid = i == 2 ? 3 : i;
            pet.RemainingHealth = 100 + i;
            pet.MaxHealth = 200 + i;
            pet.SeenAction = true;
            pets.push_back(pet);
        }

        Skyfire::BattlePetPackets::BattlePetFinalRound finalRound =
            Skyfire::BattlePetPackets::BuildFinalRoundState(true, false, pets);

        passed &= Expect(finalRound.Winners[0] && !finalRound.Winners[1],
            "Final round helper should mark the ally team as winner");
        passed &= Expect(finalRound.Pets.size() == pets.size(),
            "Final round helper should preserve all battle pets");
        for (size_t i = 0; i < pets.size(); ++i)
        {
            passed &= Expect(finalRound.Pets[i].Pboid == pets[i].Pboid,
                "Final round helper should preserve multi-pet order");
            passed &= Expect(finalRound.Pets[i].RemainingHealth == pets[i].RemainingHealth,
                "Final round helper should preserve multi-pet health");
        }

        return passed;
    }

    bool TestActivePetBattleStartsWithHealthState()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620, 42, 6, ITEM_QUALITY_RARE, 8);

        passed &= Expect(battle.IsActive(),
            "Active pet battle state should become active after starting a wild battle");
        passed &= Expect(!battle.IsFinished(),
            "Active pet battle state should not be finished immediately after start");
        passed &= Expect(battle.EnemyGUID == 0x100,
            "Active pet battle state should preserve the enemy guid");
        passed &= Expect(battle.AllyPetID == 0x200 && battle.EnemyPetID == 0x300,
            "Active pet battle state should preserve both battle pet ids");
        passed &= Expect(battle.AllyMaxHealth == 500 && battle.AllyHealth == 450,
            "Active pet battle state should preserve ally health fields");
        passed &= Expect(battle.EnemyMaxHealth == 700 && battle.EnemyHealth == 620,
            "Active pet battle state should preserve enemy health fields");
        passed &= Expect(battle.EnemySpecies == 42 && battle.EnemyLevel == 6
            && battle.EnemyQuality == ITEM_QUALITY_RARE && battle.EnemyBreed == 8,
            "Active pet battle state should preserve wild pet species, level, quality, and breed");
        passed &= Expect(battle.Winner == PET_BATTLE_WINNER_NONE,
            "Active pet battle state should start without a winner");

        return passed;
    }

    bool TestActivePetBattleSelectsInitialFrontPetWithoutAdvancingRound()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);
        battle.SetAllyPet(2, 0x202, 600, 540);

        bool selected = battle.SelectAllyFrontPet(2);

        passed &= Expect(selected,
            "Active pet battle should accept valid initial front pet selection");
        passed &= Expect(battle.AllyFrontPet == 2,
            "Active pet battle should store the selected ally front pet");
        passed &= Expect(battle.RoundID == 0,
            "Active pet battle should not advance the round when selecting initial front pet");
        passed &= Expect(!battle.SelectAllyFrontPet(3),
            "Active pet battle should reject front pet selections outside the active team");
        passed &= Expect(battle.AllyFrontPet == 2,
            "Active pet battle should preserve the current front pet after invalid selection");

        return passed;
    }

    bool TestActivePetBattleRejectsUnregisteredFrontPet()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);

        bool selected = battle.SelectAllyFrontPet(1);
        ActivePetBattleTurn turn = battle.ApplySwapRound(0, 2);

        passed &= Expect(!selected,
            "Active pet battle should reject front pets that are not in the current team");
        passed &= Expect(!turn.Accepted,
            "Active pet battle swap turn should reject missing loadout pets");
        passed &= Expect(battle.AllyFrontPet == 0 && battle.RoundID == 0,
            "Active pet battle should preserve front pet and round after missing-pet swap");

        return passed;
    }

    bool TestActivePetBattleRejectsDeadSwapPet()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);
        battle.SetAllyPet(1, 0x201, 600, 0);

        ActivePetBattleTurn turn = battle.ApplySwapRound(0, 1);

        passed &= Expect(!turn.Accepted,
            "Active pet battle swap turn should reject dead ally pets");
        passed &= Expect(battle.AllyFrontPet == 0 && battle.RoundID == 0,
            "Active pet battle should not advance after a dead-pet swap request");

        return passed;
    }

    bool TestActivePetBattleStartsWithRequestedFrontPet()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x202, 600, 540, 0x300, 700, 620, 0, 0, 0, 0, 2);

        passed &= Expect(battle.AllyFrontPet == 2,
            "Active pet battle should start with the requested ally front pet");
        passed &= Expect(battle.AllyPetID == 0x202 && battle.AllyMaxHealth == 600 && battle.AllyHealth == 540,
            "Active pet battle should mirror requested front pet health into active ally fields");
        passed &= Expect(battle.HasAllyPet(2),
            "Active pet battle should register the requested front pet in the ally team");

        return passed;
    }

    bool TestActivePetBattleDamageFinishesEnemyWithoutUnderflow()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);

        uint32 remainingHealth = battle.ApplyEnemyDamage(800);

        passed &= Expect(remainingHealth == 0 && battle.EnemyHealth == 0,
            "Active pet battle state should clamp defeated enemy health to zero");
        passed &= Expect(battle.IsFinished(),
            "Active pet battle state should finish when the enemy pet is defeated");
        passed &= Expect(battle.Winner == PET_BATTLE_WINNER_ALLY,
            "Active pet battle state should mark the ally team as winner after enemy defeat");

        return passed;
    }

    bool TestActivePetBattleAllyDeathRequiresReplacementWhenBackupAlive()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);
        battle.SetAllyPet(1, 0x201, 600, 540);

        uint32 remainingHealth = battle.ApplyAllyDamage(500);

        passed &= Expect(remainingHealth == 0 && battle.AllyHealth == 0 && battle.AllyTeam[0].Health == 0,
            "Active pet battle state should clamp defeated ally pet health to zero");
        passed &= Expect(!battle.IsFinished() && battle.Winner == PET_BATTLE_WINNER_NONE,
            "Active pet battle state should stay active when another ally pet can fight");
        passed &= Expect(battle.WaitingForAllyFrontPet,
            "Active pet battle state should require a new ally front pet after active ally death");
        passed &= Expect(!battle.ApplyAbilityRound(0, 100).Accepted,
            "Active pet battle should reject ability input while waiting for a replacement pet");

        bool selected = battle.SelectAllyFrontPet(1);
        passed &= Expect(selected && !battle.WaitingForAllyFrontPet,
            "Active pet battle should clear replacement wait after selecting a live ally pet");
        passed &= Expect(battle.AllyPetID == 0x201 && battle.AllyHealth == 540,
            "Active pet battle should mirror the replacement pet into active ally fields");

        return passed;
    }

    bool TestActivePetBattleAllyDeathFinishesWhenNoBackupAlive()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);

        uint32 remainingHealth = battle.ApplyAllyDamage(500);

        passed &= Expect(remainingHealth == 0 && battle.AllyHealth == 0,
            "Active pet battle state should clamp defeated final ally pet health to zero");
        passed &= Expect(battle.IsFinished() && battle.Winner == PET_BATTLE_WINNER_ENEMY,
            "Active pet battle state should finish for the enemy when no ally pets remain");
        passed &= Expect(!battle.WaitingForAllyFrontPet,
            "Active pet battle state should not request replacement when no ally pets remain");

        return passed;
    }

    bool TestActivePetBattleEnemyAbilityTurnRequiresReplacement()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);
        battle.SetAllyPet(1, 0x201, 600, 540);

        ActivePetBattleTurn turn = battle.ApplyEnemyAbilityRound(0, 500);

        passed &= Expect(turn.Accepted && turn.HasRoundResult,
            "Enemy ability turn should accept current round input");
        passed &= Expect(!turn.HasFinalRound && turn.RequiresFrontPet,
            "Enemy ability turn should request replacement instead of final state when backup pets remain");
        passed &= Expect(turn.EffectKind == ACTIVE_PET_BATTLE_TURN_EFFECT_DAMAGE,
            "Enemy ability turn should report a damage effect");
        passed &= Expect(turn.CasterPet == 3 && turn.TargetPet == 0,
            "Enemy ability turn should use enemy and ally front pet indexes");
        passed &= Expect(turn.TargetDied && turn.RemainingHealth == 0,
            "Enemy ability turn should report defeated ally health");
        passed &= Expect(battle.RoundID == 1 && battle.WaitingForAllyFrontPet,
            "Enemy ability turn should advance the round and wait for a replacement pet");
        passed &= Expect(!battle.ApplyEnemyAbilityRound(1, 100).Accepted,
            "Enemy ability turn should reject repeat damage while waiting for replacement");

        return passed;
    }

    bool TestActivePetBattleDeadPetReplacementClearsWait()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);
        battle.SetAllyPet(1, 0x201, 600, 540);

        ActivePetBattleTurn deathTurn = battle.ApplyEnemyAbilityRound(0, 500);
        ActivePetBattleTurn swapTurn = battle.ApplySwapRound(1, 1);

        passed &= Expect(deathTurn.Accepted && deathTurn.RequiresFrontPet,
            "Enemy ability turn should request replacement after defeating the active ally pet");
        passed &= Expect(swapTurn.Accepted && swapTurn.HasRoundResult,
            "Dead-pet replacement swap should accept the next round input");
        passed &= Expect(!battle.WaitingForAllyFrontPet,
            "Dead-pet replacement swap should clear the replacement wait");
        passed &= Expect(battle.AllyFrontPet == 1 && battle.AllyPetID == 0x201 && battle.AllyHealth == 540,
            "Dead-pet replacement swap should activate the selected live pet");
        passed &= Expect(battle.RoundID == 2,
            "Dead-pet replacement swap should advance the round after selection");

        return passed;
    }

    bool TestActivePetBattleFindsAllyPetHealthByPetId()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);
        battle.SetAllyPet(1, 0x201, 600, 540);
        battle.SelectAllyFrontPet(1);
        battle.ApplyAllyDamage(200);

        uint32 health = 0;
        passed &= Expect(battle.GetAllyPetHealth(0x201, health) && health == 340,
            "Active pet battle should find updated ally health by battle pet id");
        passed &= Expect(battle.GetAllyPetHealth(0x200, health) && health == 450,
            "Active pet battle should preserve inactive ally pet health by battle pet id");
        passed &= Expect(!battle.GetAllyPetHealth(0x202, health),
            "Active pet battle should reject unknown ally battle pet ids");

        return passed;
    }

    bool TestActivePetBattleAdvanceRoundStopsAfterFinished()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);

        battle.AdvanceRound();
        passed &= Expect(battle.RoundID == 1,
            "Active pet battle state should advance round id for active unfinished battles");

        battle.ApplyAllyDamage(500);
        battle.AdvanceRound();
        passed &= Expect(battle.RoundID == 1,
            "Active pet battle state should not advance round id after the battle is finished");
        passed &= Expect(battle.Winner == PET_BATTLE_WINNER_ENEMY,
            "Active pet battle state should mark the enemy team as winner after ally defeat");

        return passed;
    }

    bool TestActivePetBattleAbilityTurnBuildsRoundAndFinalState()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);

        ActivePetBattleTurn turn = battle.ApplyAbilityRound(0, 620);

        passed &= Expect(turn.Accepted,
            "Active pet battle ability turn should accept the current round");
        passed &= Expect(turn.HasRoundResult,
            "Active pet battle ability turn should request a round result");
        passed &= Expect(turn.HasFinalRound,
            "Active pet battle ability turn should request final state when the enemy dies");
        passed &= Expect(turn.EffectKind == ACTIVE_PET_BATTLE_TURN_EFFECT_DAMAGE,
            "Active pet battle ability turn should report a damage effect");
        passed &= Expect(turn.RoundID == 0,
            "Active pet battle ability turn should preserve the completed round id");
        passed &= Expect(turn.CasterPet == 0 && turn.TargetPet == 3,
            "Active pet battle ability turn should use ally and enemy front pets");
        passed &= Expect(turn.RemainingHealth == 0 && turn.TargetDied,
            "Active pet battle ability turn should report defeated target health");
        passed &= Expect(battle.IsFinished() && battle.Winner == PET_BATTLE_WINNER_ALLY,
            "Active pet battle ability turn should finish the battle for the ally team");

        return passed;
    }

    bool TestActivePetBattleAbilityCooldownBlocksOnlyUntilExpired()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 1400, 1200);

        ActivePetBattleTurn first = battle.ApplyAbilityRound(0, 50, 1, 1234, 3);
        passed &= Expect(first.Accepted,
            "Active pet battle should accept an ability before its cooldown starts");
        passed &= Expect(first.Cooldowns.size() == 1 && first.Cooldowns[0].Cooldown == 3,
            "Active pet battle should report the full ability cooldown in the completed round");
        passed &= Expect(battle.GetAllyAbilityCooldown(0, 1) == 2,
            "Active pet battle should decrement the used ability cooldown after the round");

        Skyfire::BattlePetPackets::BattlePetRoundResult firstRound =
            Skyfire::BattlePetPackets::BuildRoundResultFromTurn(first, 77);
        passed &= Expect(firstRound.Cooldowns.size() == 1 && firstRound.Cooldowns[0].AbilityID == 1234,
            "Active pet battle damage round should preserve the ability cooldown from the completed turn");

        ActivePetBattleTurn blocked = battle.ApplyAbilityRound(1, 50, 1, 1234, 3);
        passed &= Expect(!blocked.Accepted,
            "Active pet battle should reject an ability while its slot is cooling down");

        ActivePetBattleTurn second = battle.ApplyAbilityRound(1, 50, 2, 5678, 0);
        passed &= Expect(second.Accepted && battle.GetAllyAbilityCooldown(0, 1) == 1,
            "Active pet battle should continue decrementing cooldowns after other accepted actions");

        ActivePetBattleTurn third = battle.ApplyAbilityRound(2, 50, 2, 5678, 0);
        passed &= Expect(third.Accepted && battle.GetAllyAbilityCooldown(0, 1) == 0,
            "Active pet battle should expire cooldowns after enough accepted rounds");

        ActivePetBattleTurn clear = battle.ApplyAbilityRound(3, 50, 2, 5678, 0);
        passed &= Expect(clear.Accepted && clear.Cooldowns.size() == 1 && clear.Cooldowns[0].Cooldown == 0,
            "Active pet battle should report an expired cooldown once so the client can clear it");

        ActivePetBattleTurn reused = battle.ApplyAbilityRound(4, 50, 1, 1234, 3);
        passed &= Expect(reused.Accepted,
            "Active pet battle should accept the ability again after cooldown expiration");

        return passed;
    }

    bool TestActivePetBattleAbilityChannelLockdownContinues()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 600, 600, 0x300, 700, 700);
        battle.SetAllyPet(0, 0x200, 600, 600);

        ActivePetBattleTurn turn1;
        ActivePetBattleTurn enemy1;
        passed &= Expect(battle.ApplyAbilityExchange(0, 0, 0, 0, 170, 0, 1, 2, turn1, enemy1),
            "Lift-Off turn 1 should accept with lockdown and no crash damage");
        passed &= Expect(turn1.Accepted && turn1.DamageDealt == 0,
            "Lift-Off turn 1 should not deal crash damage");
        passed &= Expect(battle.EnemyHealth == 700,
            "Lift-Off turn 1 should leave enemy health unchanged");
        passed &= Expect(turn1.Cooldowns.size() == 1
            && turn1.Cooldowns[0].Lockdown == 1
            && turn1.Cooldowns[0].NextTurnIndex == 2
            && turn1.Cooldowns[0].Cooldown == 0,
            "Lift-Off turn 1 should snapshot lockdown without starting the real cooldown");

        battle.ActivateAllyCombatAura(341, 1, true);

        uint8 channelSlot = 0;
        uint32 channelAbility = 0;
        uint8 nextTurn = 0;
        passed &= Expect(battle.GetAllyAbilityChannel(0, channelSlot, channelAbility, nextTurn),
            "Lift-Off should expose an active channel after turn 1");
        passed &= Expect(channelAbility == 170 && nextTurn == 2 && channelSlot == 0,
            "Lift-Off channel should point at turn 2");

        // Crash turn clears Flying before the enemy reply (handled by the turn executor in-game).
        battle.ClearAllyCombatAura(341);

        ActivePetBattleTurn turn2;
        ActivePetBattleTurn enemy2;
        passed &= Expect(battle.ApplyAbilityExchange(1, 35, 40, 0, 170, 4, 0, 0, turn2, enemy2),
            "Lift-Off turn 2 should continue the channel and deal damage");
        passed &= Expect(turn2.Accepted && turn2.DamageDealt == 35,
            "Lift-Off turn 2 should deal crash damage");
        passed &= Expect(battle.EnemyHealth == 665,
            "Lift-Off turn 2 should reduce enemy health");
        passed &= Expect(turn2.Cooldowns.size() == 1
            && turn2.Cooldowns[0].Lockdown == 0
            && turn2.Cooldowns[0].NextTurnIndex == 0
            && turn2.Cooldowns[0].Cooldown == 4,
            "Lift-Off turn 2 should clear lockdown and start the real cooldown");
        // Flying is cleared on the crash turn before the enemy replies, so the chicken can connect.
        passed &= Expect(enemy2.DamageDealt == 40 && battle.AllyHealth == 560,
            "Lift-Off turn 2 should allow enemy damage after landing");

        return passed;
    }

    bool TestActivePetBattleUntargetableSkipsAllyDamage()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 600, 600, 0x300, 700, 700);
        battle.SetAllyPet(0, 0x200, 600, 600);
        battle.ActivateAllyCombatAura(341, 1, true);

        uint32 const remaining = battle.ApplyAllyDamage(250);
        passed &= Expect(remaining == 600 && battle.AllyHealth == 600,
            "Untargetable ally should ignore direct enemy damage");

        battle.ClearAllyCombatAura(341);
        uint32 const after = battle.ApplyAllyDamage(100);
        passed &= Expect(after == 500 && battle.AllyHealth == 500,
            "Ally should take damage again after Flying aura is cleared");

        return passed;
    }

    bool TestActivePetBattleTeamHealTargetsAllySlots()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 600, 400, 0x300, 700, 700);
        battle.SetAllyPet(0, 0x200, 600, 400);
        battle.SetAllyPet(1, 0x201, 500, 100);
        battle.SetAllyPet(2, 0x202, 550, 0); // dead — heal should skip

        uint32 const slot0 = battle.ApplyAllyHealToPet(0, 250);
        uint32 const slot1 = battle.ApplyAllyHealToPet(1, 250);
        uint32 const slot2 = battle.ApplyAllyHealToPet(2, 250);

        passed &= Expect(slot0 == 600 && battle.AllyTeam[0].Health == 600,
            "Team heal should clamp front pet to max health");
        passed &= Expect(slot1 == 350 && battle.AllyTeam[1].Health == 350,
            "Team heal should restore backup pet by amount");
        passed &= Expect(slot2 == 0 && battle.AllyTeam[2].Health == 0,
            "Team heal should skip dead ally slots");
        passed &= Expect(battle.AllyHealth == 600,
            "Front-pet heal should sync mirrored AllyHealth");

        return passed;
    }

    bool TestActivePetBattleWeatherBoostsMatchingFamilyDamage()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 600, 600, 0x300, 700, 700);
        // Aquatic family 9, +25% — mirrors Cleansing Rain aura 229 without DB2.
        battle.ActivateWeather(229, 9, 9, 25);

        passed &= Expect(battle.Weather.IsActive() && battle.Weather.Rounds == 9,
            "Weather should activate with configured duration");
        passed &= Expect(battle.ScaleAllyOutgoingDamage(100, 9) == 125,
            "Weather should boost Aquatic family damage by 25%");
        passed &= Expect(battle.ScaleAllyOutgoingDamage(100, 5) == 100,
            "Weather should not boost non-Aquatic family damage");

        battle.ActivateWeather(300, 4, 9, 25);
        passed &= Expect(battle.Weather.AuraAbilityId == 300 && battle.Weather.Rounds == 4,
            "New weather should replace previous weather");

        for (uint8 i = 0; i < 4; ++i)
            battle.ApplyAbilityRound(battle.RoundID, 1);

        passed &= Expect(!battle.Weather.IsActive() && battle.Weather.Rounds == 0,
            "Weather should clear after its round duration elapses");

        return passed;
    }

    bool TestActivePetBattleRejectsStaleRoundInput()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);
        battle.AdvanceRound();

        ActivePetBattleTurn turn = battle.ApplyAbilityRound(0, 100);

        passed &= Expect(!turn.Accepted,
            "Active pet battle should reject stale round input");
        passed &= Expect(battle.EnemyHealth == 620,
            "Active pet battle should not mutate health for stale round input");
        passed &= Expect(battle.RoundID == 1,
            "Active pet battle should not rewind round id for stale round input");

        return passed;
    }

    bool TestActivePetBattleSwapTurnSelectsNewFrontPet()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);
        battle.SetAllyPet(2, 0x202, 600, 540);

        ActivePetBattleTurn turn = battle.ApplySwapRound(0, 2);

        passed &= Expect(turn.Accepted && turn.HasRoundResult,
            "Active pet battle swap turn should accept current round input");
        passed &= Expect(turn.EffectKind == ACTIVE_PET_BATTLE_TURN_EFFECT_SWAP,
            "Active pet battle swap turn should report a swap effect");
        passed &= Expect(turn.CasterPet == 0 && turn.TargetPet == 2,
            "Active pet battle swap turn should preserve old and new front pets");
        passed &= Expect(battle.AllyFrontPet == 2,
            "Active pet battle swap turn should update ally front pet");
        passed &= Expect(battle.AllyPetID == 0x202 && battle.AllyHealth == 540,
            "Active pet battle swap turn should mirror the selected pet into active ally fields");
        passed &= Expect(battle.RoundID == 1,
            "Active pet battle swap turn should advance the round id");

        return passed;
    }

    bool TestActivePetBattleForfeitFinishesForEnemy()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);

        ActivePetBattleTurn turn = battle.ApplyForfeit(0);

        passed &= Expect(turn.Accepted,
            "Active pet battle forfeit should accept current round input");
        passed &= Expect(!turn.HasRoundResult && turn.HasFinalRound,
            "Active pet battle forfeit should request final state without a round result");
        passed &= Expect(turn.Abandoned,
            "Active pet battle forfeit should mark the turn as abandoned");
        passed &= Expect(battle.IsFinished() && battle.Winner == PET_BATTLE_WINNER_ENEMY,
            "Active pet battle forfeit should finish the battle for the enemy team");

        return passed;
    }

    bool TestPetBattleRoundResultHelperBuildsFromResolvedDamageTurn()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);

        ActivePetBattleTurn turn = battle.ApplyAbilityRound(0, 620);
        Skyfire::BattlePetPackets::BattlePetRoundResult round =
            Skyfire::BattlePetPackets::BuildRoundResultFromTurn(turn, 700);

        passed &= Expect(round.RoundID == 0,
            "Resolved turn round helper should preserve the turn round id");
        passed &= Expect(round.Effects.size() == 1,
            "Resolved turn round helper should create one effect");
        passed &= Expect(round.Effects[0].AbilityEffectID == 700,
            "Resolved turn round helper should preserve ability effect id for damage");
        passed &= Expect(round.DeadPets.size() == 1 && round.DeadPets[0] == 3,
            "Resolved turn round helper should preserve defeated pet indexes");

        return passed;
    }

    bool TestPetBattleRoundResultHelperBuildsFromResolvedSwapTurn()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);
        battle.SetAllyPet(2, 0x202, 600, 540);

        ActivePetBattleTurn turn = battle.ApplySwapRound(0, 2);
        Skyfire::BattlePetPackets::BattlePetRoundResult round =
            Skyfire::BattlePetPackets::BuildRoundResultFromTurn(turn, 0);

        passed &= Expect(round.RoundID == 0,
            "Resolved swap turn helper should preserve the turn round id");
        passed &= Expect(round.Effects.size() == 1,
            "Resolved swap turn helper should create one effect");
        passed &= Expect(round.Effects[0].PetBattleEffectType == 4,
            "Resolved swap turn helper should build a front-pet change effect");
        passed &= Expect(round.Effects[0].CasterPBOID == 0 && round.Effects[0].Targets[0].PetX == 2,
            "Resolved swap turn helper should preserve old and new front pet indexes");

        return passed;
    }

    bool TestPetBattleRoundResultHelperFlagsAllyReplacement()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620);
        battle.SetAllyPet(1, 0x201, 600, 540);

        ActivePetBattleTurn turn = battle.ApplyEnemyAbilityRound(0, 500);
        Skyfire::BattlePetPackets::BattlePetRoundResult round =
            Skyfire::BattlePetPackets::BuildRoundResultFromTurn(turn, 900);

        passed &= Expect(round.RoundID == 0,
            "Resolved ally death round helper should preserve the turn round id");
        passed &= Expect(round.DeadPets.size() == 1 && round.DeadPets[0] == 0,
            "Resolved ally death round helper should mark the defeated ally pet");
        passed &= Expect(round.InputFlags[0] == Skyfire::BattlePetPackets::BATTLE_PET_ROUND_INPUT_FLAG_SELECT_NEW_FRONT_PET,
            "Resolved ally death round helper should request ally front pet selection");
        passed &= Expect(round.InputFlags[0] == 0x08,
            "Resolved ally death round helper should use the client select-new-pet input flag");
        passed &= Expect(round.InputFlags[1] == Skyfire::BattlePetPackets::BATTLE_PET_ROUND_INPUT_FLAG_NONE,
            "Resolved ally death round helper should leave enemy input flags clear");

        return passed;
    }

    bool TestPetBattleFinalRoundPacketWritesWinnerAndPetState()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetFinalRound finalRound;
        finalRound.Winners[0] = true;
        finalRound.Winners[1] = false;
        finalRound.NpcCreatureID[1] = 12345;

        Skyfire::BattlePetPackets::BattlePetFinalRoundPet pet;
        pet.Pboid = 0;
        pet.RemainingHealth = 450;
        pet.InitialLevel = 4;
        pet.NewLevel = 5;
        pet.Xp = 220;
        pet.MaxHealth = 500;
        pet.SeenAction = true;
        pet.AwardedXP = true;
        finalRound.Pets.push_back(pet);

        WorldPacket packet = Skyfire::BattlePetPackets::BuildFinalRoundPacket(finalRound);

        passed &= Expect(packet.GetOpcode() == SMSG_PET_BATTLE_FINAL_ROUND,
            "Pet battle final round packet should use SMSG_PET_BATTLE_FINAL_ROUND");

        packet.rpos(0);
        passed &= Expect(packet.ReadBits(20) == 1,
            "Pet battle final round packet should write pet count");
        passed &= Expect(packet.ReadBit() == false,
            "Final round pet should clear captured/caged");
        passed &= Expect(packet.ReadBit() == true,
            "Final round pet should write seen action");
        passed &= Expect(packet.ReadBit() == false,
            "Final round pet should include xp");
        passed &= Expect(packet.ReadBit() == false,
            "Final round pet should clear duplicate captured/caged bit");
        passed &= Expect(packet.ReadBit() == false,
            "Final round pet should include initial level");
        passed &= Expect(packet.ReadBit() == false,
            "Final round pet should include new level");
        passed &= Expect(packet.ReadBit() == true,
            "Final round pet should write awarded xp");
        passed &= Expect(packet.ReadBit() == false,
            "Final round should clear pvp battle");
        passed &= Expect(packet.ReadBit() == true,
            "Final round should mark first team as winner");
        passed &= Expect(packet.ReadBit() == false,
            "Final round should leave second team as loser");
        passed &= Expect(packet.ReadBit() == false,
            "Final round should clear abandoned");

        uint16 xp = 0;
        uint8 pboid = 0;
        uint32 remainingHealth = 0;
        uint16 newLevel = 0;
        uint32 maxHealth = 0;
        uint16 initialLevel = 0;
        uint32 npcCreatureId[2] = { };

        packet >> xp;
        packet >> pboid;
        packet >> remainingHealth;
        packet >> newLevel;
        packet >> maxHealth;
        packet >> initialLevel;
        packet >> npcCreatureId[0];
        packet >> npcCreatureId[1];

        passed &= Expect(xp == 220,
            "Final round pet should write earned xp");
        passed &= Expect(pboid == 0,
            "Final round pet should write pet battle object id");
        passed &= Expect(remainingHealth == 450,
            "Final round pet should write remaining health");
        passed &= Expect(newLevel == 5,
            "Final round pet should write new level");
        passed &= Expect(maxHealth == 500,
            "Final round pet should write max health");
        passed &= Expect(initialLevel == 4,
            "Final round pet should write initial level");
        passed &= Expect(npcCreatureId[0] == 0 && npcCreatureId[1] == 12345,
            "Final round should write npc creature ids");

        return passed;
    }

    bool TestPetBattleFinalRoundPacketMarksCapturedPet()
    {
        bool passed = true;

        Skyfire::BattlePetPackets::BattlePetFinalRound finalRound;
        finalRound.Winners[0] = true;
        finalRound.Winners[1] = false;

        Skyfire::BattlePetPackets::BattlePetFinalRoundPet pet;
        pet.Pboid = 3;
        pet.RemainingHealth = 75;
        pet.InitialLevel = 5;
        pet.NewLevel = 5;
        pet.MaxHealth = 120;
        pet.SeenAction = true;
        pet.Captured = true;
        finalRound.Pets.push_back(pet);

        WorldPacket packet = Skyfire::BattlePetPackets::BuildFinalRoundPacket(finalRound);

        packet.rpos(0);
        passed &= Expect(packet.ReadBits(20) == 1,
            "Captured final round packet should write pet count");
        passed &= Expect(packet.ReadBit() == true,
            "Captured final round pet should set the captured/caged bit");
        passed &= Expect(packet.ReadBit() == true,
            "Captured final round pet should preserve seen action");
        passed &= Expect(packet.ReadBit() == true,
            "Captured final round pet should omit xp when none was awarded");
        passed &= Expect(packet.ReadBit() == true,
            "Captured final round pet should set the duplicate captured/caged bit");

        return passed;
    }

    bool TestActivePetBattleTrapFinishesAndMarksCapture()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 140, 42, 6, ITEM_QUALITY_NORMAL);

        ActivePetBattleTurn turn = battle.ApplyTrapRound(0, true);

        passed &= Expect(turn.Accepted,
            "Active pet battle trap should accept current round input");
        passed &= Expect(!turn.HasRoundResult && turn.HasFinalRound,
            "Active pet battle trap should request final state without a round result");
        passed &= Expect(turn.Captured,
            "Active pet battle trap should mark the turn as captured");
        passed &= Expect(battle.IsFinished() && battle.Winner == PET_BATTLE_WINNER_ALLY,
            "Active pet battle trap should finish the battle for the ally team");
        passed &= Expect(battle.Captured,
            "Active pet battle trap should mark the battle as captured");
        passed &= Expect(battle.EnemySpecies == 42 && battle.EnemyLevel == 6 && battle.EnemyQuality == ITEM_QUALITY_NORMAL,
            "Active pet battle should preserve wild pet capture metadata");

        return passed;
    }

    bool TestActivePetBattleTrapRejectsHighHealthEnemy()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 620, 42, 6, ITEM_QUALITY_NORMAL);

        passed &= Expect(battle.GetTrapStatus() == PET_BATTLE_TRAP_STATUS_HEALTH_TOO_HIGH,
            "Active pet battle trap status should reject enemies above capture health");

        ActivePetBattleTurn turn = battle.ApplyTrapRound(0, true);

        passed &= Expect(!turn.Accepted,
            "Active pet battle trap should reject high-health enemies");
        passed &= Expect(!battle.IsFinished() && !battle.Captured,
            "Rejected trap should not finish or capture the battle");

        battle.ApplyEnemyDamage(480);

        passed &= Expect(battle.GetTrapStatus() == PET_BATTLE_TRAP_STATUS_ACTIVE,
            "Active pet battle trap status should become active at capture health");

        return passed;
    }

    bool TestActivePetBattleTrapStatusHandlesCaptureFailures()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 140, 42, 6, ITEM_QUALITY_NORMAL);

        passed &= Expect(battle.GetTrapStatus(false, true) == PET_BATTLE_TRAP_STATUS_JOURNAL_FULL,
            "Active pet battle trap status should reject full journals or species limits");
        passed &= Expect(battle.GetTrapStatus(true, false) == PET_BATTLE_TRAP_STATUS_NOT_CAPTURABLE,
            "Active pet battle trap status should reject non-capturable enemies");

        battle.AllyHealth = 0;
        passed &= Expect(battle.GetTrapStatus(true, true) == PET_BATTLE_TRAP_STATUS_DEAD_PET,
            "Active pet battle trap status should reject when an active pet is dead");

        battle.AllyHealth = 450;
        ActivePetBattleTurn turn = battle.ApplyTrapRound(0, true);
        passed &= Expect(turn.Accepted,
            "Active pet battle trap should accept once capture rules pass");
        passed &= Expect(battle.GetTrapStatus() == PET_BATTLE_TRAP_STATUS_ALREADY_TRAPPED,
            "Active pet battle trap status should reject a second trap attempt");

        return passed;
    }

    bool TestActivePetBattleFailedTrapAllowsEnemyCounterattack()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 140, 42, 6, ITEM_QUALITY_NORMAL);

        ActivePetBattleTurn trapTurn;
        ActivePetBattleTurn enemyTurn;
        bool accepted = battle.ApplyTrapRoundWithEnemyResponse(0, false, 90, trapTurn, enemyTurn);

        passed &= Expect(accepted && trapTurn.Accepted && trapTurn.HasRoundResult,
            "Failed trap should accept current round and produce a round result");
        passed &= Expect(!trapTurn.Captured && !trapTurn.HasFinalRound,
            "Failed trap should not capture or finish the battle");
        passed &= Expect(enemyTurn.Accepted && enemyTurn.HasRoundResult,
            "Failed trap should allow the enemy pet to counterattack in the same round");
        passed &= Expect(enemyTurn.EffectKind == ACTIVE_PET_BATTLE_TURN_EFFECT_DAMAGE,
            "Failed trap counterattack should report a damage effect");
        passed &= Expect(enemyTurn.CasterPet == 3 && enemyTurn.TargetPet == 0,
            "Failed trap counterattack should use enemy and ally front pet indexes");
        passed &= Expect(enemyTurn.RemainingHealth == 360 && battle.AllyHealth == 360,
            "Failed trap counterattack should damage the active ally pet");
        passed &= Expect(battle.TrapFailedAttempts == 1,
            "Failed trap should increment failed capture attempts");
        passed &= Expect(battle.RoundID == 1,
            "Failed trap counterattack should advance the round once");

        return passed;
    }

    bool TestActivePetBattleFailedTrapCounterattackCanForceReplacement()
    {
        bool passed = true;

        ActivePetBattle battle;
        battle.StartWild(0x100, 0x200, 500, 450, 0x300, 700, 140, 42, 6, ITEM_QUALITY_NORMAL);
        battle.SetAllyPet(1, 0x201, 600, 540);

        ActivePetBattleTurn trapTurn;
        ActivePetBattleTurn enemyTurn;
        bool accepted = battle.ApplyTrapRoundWithEnemyResponse(0, false, 500, trapTurn, enemyTurn);

        passed &= Expect(accepted && trapTurn.Accepted,
            "Failed trap should accept even when the enemy counterattack defeats the active ally pet");
        passed &= Expect(enemyTurn.Accepted && enemyTurn.TargetDied && enemyTurn.RequiresFrontPet,
            "Failed trap counterattack should request a replacement when backup pets remain");
        passed &= Expect(!battle.IsFinished() && battle.WaitingForAllyFrontPet,
            "Failed trap counterattack should keep the battle active while waiting for a replacement pet");
        passed &= Expect(battle.RoundID == 1,
            "Failed trap replacement request should advance to the replacement input round");

        return passed;
    }
}

int main()
{
    bool passed = TestPetBattleRequestFailedPacketLayout();
    passed &= TestPetBattleQueueStatusPacketWritesRemovedStatus();
    passed &= TestPetBattleQueueStatusPacketWritesSlotResults();
    passed &= TestPetBattleFinalizeLocationPacketLayout();
    passed &= TestPetBattleFacingLooksAtTeamCenter();
    passed &= TestPetBattleFacingPreservesClientValue();
    passed &= TestPetBattleInitialUpdatePacketStartsWithEnvironmentAndTeamBits();
    passed &= TestPetBattleInitialUpdatePacketWritesPetAbilityIds();
    passed &= TestPetBattleInitialUpdatePacketWritesMultipleTeamPets();
    passed &= TestInitialUpdatePetDataUsesSpeciesAbilityLookup();
    passed &= TestWildInitialUpdatePetDataUsesGeneratedBreedAndQuality();
    passed &= TestBattlePetAbilityJournalFlagSlots();
    passed &= TestBattlePetHealthPercentUsesCeilingForNonZeroHeals();
    passed &= TestBattlePetExperienceLevelsAtThreshold();
    passed &= TestBattlePetExperienceCapsAtMaxLevel();
    passed &= TestBattlePetExperienceRewardScalesByLevelDifference();
    passed &= TestBattlePetCapturedLevelPenalty();
    passed &= TestBattlePetTrapAbilityForLoadoutFlags();
    passed &= TestBattlePetAchievementLoadoutRewards();
    passed &= TestBattlePetSpeciesFlagsAllowWildCapture();
    passed &= TestBattlePetAchievementCriteriaIds();
    passed &= TestBattlePetAchievementCriteriaPayload();
    passed &= TestBattlePetTimestampMarksPetForSave();
    passed &= TestPetBattleInitialUpdatePacketAllowsEmptySecondTeam();
    passed &= TestPetBattleFirstRoundPacketSelectsFrontPets();
    passed &= TestPetBattleFirstRoundPacketWritesTrapStatuses();
    passed &= TestPetBattleInputReadsAbilityMove();
    passed &= TestPetBattleInputReadsSwapAndInterruption();
    passed &= TestPetBattleInputCommandBuildsAbilityCommand();
    passed &= TestPetBattleInputCommandPrefersSwapCommand();
    passed &= TestPetBattleInputCommandBuildsForfeitCommand();
    passed &= TestPetBattleInputCommandBuildsTrapCommand();
    passed &= TestPetBattleInputCommandBuildsMoveTypeForfeitCommand();
    passed &= TestPetBattleInputCommandRequiresRound();
    passed &= TestPetBattleFirstPetSelectionReadsPetId();
    passed &= TestPetBattleRequestUpdateReadsGuidAndCancelled();
    passed &= TestPetBattleFinishedPacketIsEmpty();
    passed &= TestPetBattleRoundResultPacketWritesEmptyRound();
    passed &= TestPetBattleRoundResultPacketWritesCooldowns();
    passed &= TestPetBattleRoundResultPacketWritesNonzeroLockdown();
    passed &= TestPetBattleRoundResultPacketWritesMultipleDamageEffects();
    passed &= TestPetBattleRoundResultPacketWritesDamageEffect();
    passed &= TestPetBattleDamageRoundHelperMarksDeadTarget();
    passed &= TestPetBattleSwapEffectHelperSelectsNewFrontPet();
    passed &= TestPetBattleFinalRoundHelperMarksWinnerAndAbandoned();
    passed &= TestPetBattleFinalRoundHelperPreservesMultiplePets();
    passed &= TestActivePetBattleStartsWithHealthState();
    passed &= TestActivePetBattleSelectsInitialFrontPetWithoutAdvancingRound();
    passed &= TestActivePetBattleRejectsUnregisteredFrontPet();
    passed &= TestActivePetBattleRejectsDeadSwapPet();
    passed &= TestActivePetBattleStartsWithRequestedFrontPet();
    passed &= TestActivePetBattleDamageFinishesEnemyWithoutUnderflow();
    passed &= TestActivePetBattleAllyDeathRequiresReplacementWhenBackupAlive();
    passed &= TestActivePetBattleAllyDeathFinishesWhenNoBackupAlive();
    passed &= TestActivePetBattleEnemyAbilityTurnRequiresReplacement();
    passed &= TestActivePetBattleDeadPetReplacementClearsWait();
    passed &= TestActivePetBattleFindsAllyPetHealthByPetId();
    passed &= TestActivePetBattleAdvanceRoundStopsAfterFinished();
    passed &= TestActivePetBattleAbilityTurnBuildsRoundAndFinalState();
    passed &= TestActivePetBattleAbilityCooldownBlocksOnlyUntilExpired();
    passed &= TestActivePetBattleAbilityChannelLockdownContinues();
    passed &= TestActivePetBattleUntargetableSkipsAllyDamage();
    passed &= TestActivePetBattleTeamHealTargetsAllySlots();
    passed &= TestActivePetBattleWeatherBoostsMatchingFamilyDamage();
    passed &= TestActivePetBattleRejectsStaleRoundInput();
    passed &= TestActivePetBattleSwapTurnSelectsNewFrontPet();
    passed &= TestActivePetBattleForfeitFinishesForEnemy();
    passed &= TestPetBattleRoundResultHelperBuildsFromResolvedDamageTurn();
    passed &= TestPetBattleRoundResultHelperBuildsFromResolvedSwapTurn();
    passed &= TestPetBattleRoundResultHelperFlagsAllyReplacement();
    passed &= TestPetBattleFinalRoundPacketWritesWinnerAndPetState();
    passed &= TestPetBattleFinalRoundPacketMarksCapturedPet();
    passed &= TestActivePetBattleTrapFinishesAndMarksCapture();
    passed &= TestActivePetBattleTrapRejectsHighHealthEnemy();
    passed &= TestActivePetBattleTrapStatusHandlesCaptureFailures();
    passed &= TestActivePetBattleFailedTrapAllowsEnemyCounterattack();
    passed &= TestActivePetBattleFailedTrapCounterattackCanForceReplacement();
    std::cout << (passed ? "Battle pet packet tests passed" : "Battle pet packet tests failed") << std::endl;
    return passed ? 0 : 1;
}
