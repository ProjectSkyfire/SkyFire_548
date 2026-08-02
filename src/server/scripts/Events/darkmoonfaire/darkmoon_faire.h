/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef DARKMOON_FAIRE_H
#define DARKMOON_FAIRE_H

#include "Define.h"

enum DarkmoonDeathmatch
{
    GAME_EVENT_DARKMOON_DEATHMATCH_ANNOUNCE = 82,
    GAME_EVENT_DARKMOON_DEATHMATCH_RUN = 83,
    NPC_DARKMOON_DEATHMATCH_ANNOUNCER = 55402,
    GO_DARKMOON_DEATHMATCH_CHEST = 209620,
    SPELL_ENTER_DEATHMATCH = 108919,
    SPELL_EXIT_DEATHMATCH = 108923,
};

enum DarkmoonDeathmatchTexts
{
    SAY_ANNOUNCE = 0,
    SAY_RUN = 1,
    SAY_END = 2
};

enum MoonfangSpells
{
    SPELL_FREE_YOUR_MIND_EFF = 145112,
    SPELL_MOONFANG_CURSE = 144590,
    SPELL_MOONFANG_TEARS = 144702,
    SPELL_FANGS_OF_THE_MOON = 144700,
    SPELL_CALL_THE_PACK = 144602,
    SPELL_LEAP_FOR_THE_KILL = 144546,
};

enum MoonfangEvents
{
    EVENT_MOONFANG_CURSE = 1,
    EVENT_FANGS_OF_THE_MOON,
    EVENT_CALL_THE_PACK,
    EVENT_LEAP_FOR_THE_KILL,
    EVENT_MOONFANG_TEARS,
};

namespace DarkmoonFaire
{
    uint16 const GameEventId = 75;
    uint32 const IslandMapId = 974;

    struct EventReturnPosition
    {
        uint32 MapId;
        float X;
        float Y;
        float Z;
        float Orientation;
    };

    inline char const* GetMissingGameTokenText()
    {
        return "You need a Darkmoon Game Token to play this game.";
    }

    inline EventReturnPosition const& GetEventEndReturnPosition(bool horde)
    {
        static EventReturnPosition const allianceReturnPosition = { 0, -9547.84f, 85.0556f, 60.1021f, 6.23083f };
        static EventReturnPosition const hordeReturnPosition = { 1, -1460.82f, 170.258f, -7.44726f, 3.12414f };

        return horde ? hordeReturnPosition : allianceReturnPosition;
    }

    inline bool IsDarkmoonFaireEventEnd(bool start, uint16 eventId)
    {
        return !start && eventId == GameEventId;
    }

    inline bool ShouldTeleportOffIsland(bool start, uint16 eventId, uint32 mapId)
    {
        return IsDarkmoonFaireEventEnd(start, eventId) && mapId == IslandMapId;
    }

    inline bool ShouldTeleportOffIslandWhenEventInactive(bool eventActive, uint32 mapId)
    {
        return !eventActive && mapId == IslandMapId;
    }
}

enum DarkmoonWhackAGnollCreatures
{
    NPC_WHACK_BARREL = 54546,
    NPC_WHACK_GNOLL = 54444,
    NPC_WHACK_DOLL = 54466,
    NPC_WHACK_BOSS = 54549,
    NPC_WHACK_GNOLL_CREDIT = 54505
};

enum DarkmoonWhackAGnollSpells
{
    SPELL_WHACK_OVERRIDE_ACTION = 101612,
    SPELL_WHACK_ENABLE_POWERBAR = 110230,
    SPELL_WHACK_HAMMER = 101604,
    SPELL_WHACK_KILL_CREDIT = 101835,
    SPELL_WHACK_DOLL_STUN = 101679,
    SPELL_WHACK_SPAWN_VISUAL = 102136,
    SPELL_WHACK_OK_TO_HIT = 101996,
    SPELL_WHACK_SUMMON_GNOLL = 102036,
    SPELL_WHACK_SUMMON_DOLL = 102043,
    SPELL_WHACK_SUMMON_BOSS = 102044,
    SPELL_WHACK_FORBIDDEN_ZONE = 109977
};

enum DarkmoonWhackAGnollEvents
{
    EVENT_WHACK_SUMMON_GNOLL = 1,
    EVENT_WHACK_SUMMON_DOLL,
    EVENT_WHACK_SUMMON_BOSS,
    EVENT_WHACK_CHECK_PLAYER_POSITION
};

enum DarkmoonWhackAGnollMisc
{
    QUEST_WHACK_A_GNOLL = 29463,
    ITEM_DARKMOON_GAME_TOKEN = 71083,
    MAX_WHACK_BARRELS = 9
};

enum DarkmoonShootingGalleryCreatures
{
    NPC_DARKMOON_RINLING = 14841,
    NPC_SHOOTING_GALLERY_TARGET = 54231
};

enum DarkmoonShootingGallerySpells
{
    SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION = 101871,
    SPELL_SHOOTING_GALLERY_SHOOT = 101872,
    SPELL_SHOOTING_GALLERY_TARGET_INDICATOR = 102341
};

enum DarkmoonShootingGalleryEvents
{
    EVENT_SHOOTING_GALLERY_START_WAVE = 1,
    EVENT_SHOOTING_GALLERY_FINISH_GAME,
    EVENT_SHOOTING_GALLERY_CHECK_PLAYER
};

enum DarkmoonShootingGalleryActions
{
    ACTION_SHOOTING_GALLERY_END_GAME = 1,
    ACTION_SHOOTING_GALLERY_COMPLETE_GAME
};

enum DarkmoonShootingGalleryData
{
    DATA_SHOOTING_GALLERY_ACTIVE_PLAYER = 1,
    DATA_SHOOTING_GALLERY_SPAWN_TIME,
    DATA_SHOOTING_GALLERY_HIT_COUNT,
    DATA_SHOOTING_GALLERY_ACTIVE_TARGET
};

enum DarkmoonShootingGalleryMisc
{
    QUEST_SHOOTING_GALLERY = 29438,
    ACHIEVEMENT_QUICK_SHOT = 6022,
    SHOOTING_GALLERY_TARGET_COUNT = 25,
    SHOOTING_GALLERY_QUICK_SHOT_WINDOW_MS = 1000,
    SHOOTING_GALLERY_MAX_HITS_PER_TARGET = 2
};

enum DarkmoonRingTossCreatures
{
    NPC_DARKMOON_JESSICA_ROGERS = 54485,
    NPC_DARKMOON_DUBENKO = 54490,
    NPC_RING_TOSS_CREDIT = 54495
};

enum DarkmoonRingTossSpells
{
    SPELL_RING_TOSS_THROW = 101695,
    SPELL_RING_TOSS_ACTION = 102058
};

enum DarkmoonRingTossEvents
{
    EVENT_RING_TOSS_FINISH_GAME = 1,
    EVENT_RING_TOSS_CHECK_PLAYER
};

enum DarkmoonRingTossActions
{
    ACTION_RING_TOSS_END_GAME = 1,
    ACTION_RING_TOSS_COMPLETE_GAME
};

enum DarkmoonRingTossData
{
    DATA_RING_TOSS_ACTIVE_PLAYER = 1,
    DATA_RING_TOSS_THROW_COUNT
};

enum DarkmoonRingTossMisc
{
    QUEST_TARGET_TURTLE = 29455,
    RING_TOSS_TARGET_COUNT = 3,
    RING_TOSS_MAX_THROWS = 10
};

enum DarkmoonTonkChallengeCreatures
{
    NPC_DARKMOON_FINLAY_COOLSHOT = 54605,
    NPC_DARKMOON_STEAM_TONK = 54588,
    NPC_TONK_CANNON_TARGET = 33081
};

enum DarkmoonTonkChallengeSpells
{
    SPELL_DARKMOON_TONK_CONTROLLER = 100752,
    SPELL_DARKMOON_TONK_CANNON_BLAST = 102121,
    SPELL_DARKMOON_TONK_ACTION = 102178,
    SPELL_DARKMOON_TONK_SHOT = 102292,
    SPELL_DARKMOON_TONK_TARGET_TRACKER = 62265,
    SPELL_DARKMOON_TONK_TARGET_PARTICIPANT = 62280
};

enum DarkmoonTonkChallengeEvents
{
    EVENT_TONK_CHALLENGE_START_WAVE = 1,
    EVENT_TONK_CHALLENGE_PLACE_TONK,
    EVENT_TONK_CHALLENGE_FINISH_GAME,
    EVENT_TONK_CHALLENGE_CHECK_PLAYER,
    EVENT_TONK_CHALLENGE_CANCEL_GAME
};

enum DarkmoonTonkChallengeActions
{
    ACTION_TONK_CHALLENGE_END_GAME = 1,
    ACTION_TONK_CHALLENGE_COMPLETE_GAME,
    ACTION_TONK_CHALLENGE_CANCEL_GAME
};

enum DarkmoonTonkChallengeData
{
    DATA_TONK_CHALLENGE_ACTIVE_PLAYER = 1
};

enum DarkmoonTonkChallengeMisc
{
    QUEST_TONK_COMMANDER = 29434,
    TONK_CHALLENGE_TARGET_COUNT = 30
};

enum DarkmoonCannonCreatures
{
    NPC_DARKMOON_MAXIMA_BLASTENHEIMER = 15303,
    NPC_DARKMOON_CANNON_TARGET_CREDIT = 54224,
    NPC_DARKMOON_FOZLEBUB = 57850
};

enum DarkmoonCannonSpells
{
    SPELL_DARKMOON_CANNON_PREPARATION = 102112,
    SPELL_DARKMOON_CANNON_MAGIC_WINGS = 102116,
    SPELL_DARKMOON_CANNON_LAUNCH = 102121,
    SPELL_DARKMOON_CANNON_TARGET_CREDIT = 100962,
    SPELL_DARKMOON_CANNON_TARGET_CENTER = 62173,
    SPELL_DARKMOON_CANNON_TARGET_NEAR = 62175,
    SPELL_DARKMOON_CANNON_TARGET_MISS = 62179,
    SPELL_DARKMOON_CANNON_ROOT = 97936,
    SPELL_DARKMOON_BACK_TO_THE_CANNON = 109244
};

enum DarkmoonCannonMisc
{
    QUEST_HUMANOID_CANNONBALL = 29436,
    DARKMOON_CANNON_TARGET_POINTS = 5,
    ACHIEVEMENT_BLASTENHEIMER_BULLSEYE = 6021
};

namespace DarkmoonRingToss
{
    uint32 const ThrowSpellVisual = 21471;
    float const ThrowDestinationVisualHeight = 1.25f;

    inline uint32 GetThrowSpellVisual(uint32 spellVisual)
    {
        return spellVisual ? spellVisual : ThrowSpellVisual;
    }

    inline float GetThrowDestinationVisualZ(float groundZ)
    {
        return groundZ + ThrowDestinationVisualHeight;
    }
}

namespace DarkmoonCannon
{
    uint32 const MapId = 974;
    uint32 const ReturnCost = 3000;
    float const BullseyeDistance = 3.0f;
    float const NearDistance = 28.0f;
    float const LaunchHorizontalSpeed = 55.0f;
    float const LaunchVerticalSpeed = 48.2f;

    struct CannonPosition
    {
        float X;
        float Y;
        float Z;
        float Orientation;
    };

    inline CannonPosition const& GetLaunchPosition()
    {
        static CannonPosition const launchPosition = { -4018.15f, 6299.57f, 13.11f, 3.30732f };
        return launchPosition;
    }

    inline CannonPosition const& GetLaunchSourcePosition()
    {
        static CannonPosition const sourcePosition = { -4017.1637f, 6299.7350f, 13.11f, 3.30732f };
        return sourcePosition;
    }

    inline CannonPosition const& GetTargetPosition()
    {
        static CannonPosition const targetPosition = { -4479.484863f, 6222.407227f, -1.569462f, 0.0f };
        return targetPosition;
    }

    inline CannonPosition const& GetReturnPosition()
    {
        static CannonPosition const returnPosition = { -4019.0f, 6286.58f, 12.49f, 1.39f };
        return returnPosition;
    }

    inline uint32 GetLandingCredit(float distance)
    {
        if (distance <= BullseyeDistance)
            return 3;

        if (distance <= NearDistance)
            return 1;

        return 0;
    }

    inline uint32 ClampCannonScore(uint32 current, uint32 increment)
    {
        if (current >= DARKMOON_CANNON_TARGET_POINTS)
            return DARKMOON_CANNON_TARGET_POINTS;

        uint32 score = current + increment;
        return score > DARKMOON_CANNON_TARGET_POINTS ? DARKMOON_CANNON_TARGET_POINTS : score;
    }

    inline bool IsCompleteScore(uint32 score)
    {
        return score >= DARKMOON_CANNON_TARGET_POINTS;
    }
}

namespace DarkmoonTonkChallenge
{
    uint32 const MapId = 974;
    uint32 const TonkVehicleId = 1734;

    struct TargetPosition
    {
        float X;
        float Y;
        float Z;
        float Orientation;
    };

    uint32 const TargetPositionCount = 18;

    inline TargetPosition const& GetTonkStartPosition()
    {
        static TargetPosition const startPosition = { -4129.767578f, 6321.413086f, 13.116933f, 4.30912f };
        return startPosition;
    }

    inline TargetPosition const& GetTonkReturnPosition()
    {
        static TargetPosition const returnPosition = { -4128.246582f, 6324.775879f, 13.117409f, 4.30912f };
        return returnPosition;
    }

    inline TargetPosition const* GetTargetPositions()
    {
        static TargetPosition const targetPositions[TargetPositionCount] =
        {
            { -4140.77f, 6315.62f, 13.1177f, 0.26241f },
            { -4148.82f, 6303.68f, 13.1177f, 0.36451f },
            { -4143.79f, 6299.65f, 13.1177f, 0.32523f },
            { -4142.58f, 6309.06f, 13.1177f, 0.64332f },
            { -4134.22f, 6303.87f, 13.1177f, 1.51511f },
            { -4134.84f, 6293.24f, 13.1169f, 1.60936f },
            { -4130.75f, 6299.12f, 13.1169f, 2.49686f },
            { -4129.41f, 6307.05f, 13.1169f, 2.14343f },
            { -4126.29f, 6314.47f, 13.1175f, 2.36158f },
            { -4129.05f, 6302.49f, 13.1166f, 2.30239f },
            { -4131.39f, 6296.11f, 13.1165f, 2.38578f },
            { -4140.39f, 6298.14f, 13.1165f, 1.21451f },
            { -4148.82f, 6306.59f, 13.1165f, 0.34664f },
            { -4139.23f, 6312.75f, 13.1165f, 0.99067f },
            { -4140.27f, 6307.18f, 13.1165f, 1.17917f },
            { -4137.38f, 6303.69f, 13.1165f, 1.47371f },
            { -4128.18f, 6308.92f, 13.1165f, 2.96988f },
            { -4127.59f, 6313.02f, 13.1165f, 2.84422f }
        };

        return targetPositions;
    }

    inline bool IsTonkTarget(uint32 entry)
    {
        return entry == NPC_TONK_CANNON_TARGET;
    }

    inline bool IsTonkHitSpell(uint32 spellId)
    {
        return spellId == SPELL_DARKMOON_TONK_SHOT;
    }

    inline uint32 ClampTargetScore(uint32 current, uint32 increment)
    {
        if (current >= TONK_CHALLENGE_TARGET_COUNT)
            return TONK_CHALLENGE_TARGET_COUNT;

        uint32 score = current + increment;
        return score > TONK_CHALLENGE_TARGET_COUNT ? TONK_CHALLENGE_TARGET_COUNT : score;
    }

    inline bool IsCompleteScore(uint32 score)
    {
        return score >= TONK_CHALLENGE_TARGET_COUNT;
    }

    inline bool IsTargetInsideArena(TargetPosition const& position)
    {
        return position.X >= -4152.0f && position.X <= -4124.0f
            && position.Y >= 6290.0f && position.Y <= 6323.0f
            && position.Z >= 13.0f && position.Z <= 13.3f;
    }
}

namespace DarkmoonShootingGallery
{
    struct TargetPosition
    {
        float X;
        float Y;
        float Z;
        float Orientation;
    };

    uint32 const TargetPositionCount = 3;

    inline TargetPosition const* GetTargetPositions()
    {
        static TargetPosition const targetPositions[TargetPositionCount] =
        {
            { -4068.08f, 6353.26f, 13.4977f, 4.04916f },
            { -4069.91f, 6355.00f, 13.4849f, 4.13643f },
            { -4072.09f, 6356.73f, 13.4903f, 4.31096f }
        };

        return targetPositions;
    }

    inline bool HasVisibleTargetHeight(TargetPosition const& position)
    {
        return position.Z >= 13.45f;
    }

    inline bool IsTargetCreditEntry(uint32 entry)
    {
        return entry == NPC_SHOOTING_GALLERY_TARGET;
    }

    inline bool CanAwardTargetCredit(uint32 entry, bool active)
    {
        return active && IsTargetCreditEntry(entry);
    }

    inline uint32 GetHitCreditCount(uint32 elapsedMs)
    {
        return elapsedMs <= SHOOTING_GALLERY_QUICK_SHOT_WINDOW_MS ? 2 : 1;
    }

    inline uint32 ClampTargetScore(uint32 current, uint32 increment)
    {
        if (current >= SHOOTING_GALLERY_TARGET_COUNT)
            return SHOOTING_GALLERY_TARGET_COUNT;

        uint32 score = current + increment;
        return score > SHOOTING_GALLERY_TARGET_COUNT ? SHOOTING_GALLERY_TARGET_COUNT : score;
    }

    inline bool IsCompleteScore(uint32 score)
    {
        return score >= SHOOTING_GALLERY_TARGET_COUNT;
    }
}

#endif
