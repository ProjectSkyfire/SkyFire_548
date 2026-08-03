/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef BATTLE_PET_MGR_H
#define BATTLE_PET_MGR_H

#include "BattlePet.h"
#include "BattlePetBattleState.h"
#include "Common.h"
#include "TemporarySummon.h"

#include <vector>

typedef std::set<BattlePet*> BattlePetSet;

class Player;
class Creature;

enum
{
    BATTLE_PET_MAX_JOURNAL_SPECIES = 3,   // client modifications required to increase
    BATTLE_PET_MAX_JOURNAL_PETS    = 500, // sent to client as 25 bits (theoretical max 33,554,431)
    BATTLE_PET_MAX_LOADOUT_SLOTS   = 3,
    BATTLE_PET_CAGE_ITEM_ID        = 82800,
};

enum BattlePetLoadoutSlots
{
    BATTLE_PET_LOADOUT_SLOT_1 = 0,
    BATTLE_PET_LOADOUT_SLOT_2 = 1,
    BATTLE_PET_LOADOUT_SLOT_3 = 2,
    BATTLE_PET_LOADOUT_SLOT_NONE
};

// custom flags used in 'account_battle_pet_slots' db table
enum BattlePetLoadoutFlags
{
    BATTLE_PET_LOADOUT_SLOT_FLAG_NONE = 0x0,
    BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_1 = 0x1,
    BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_2 = 0x2,
    BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_3 = 0x4,
    BATTLE_PET_LOADOUT_TRAP = 0x08,
    BATTLE_PET_LOADOUT_STRONG_TRAP = 0x10,
    BATTLE_PET_LOADOUT_PRISTINE_TRAP = 0x20,
    BATTLE_PET_LOADOUT_GM_TRAP = 0x40
};

enum BattlePetTrapAbilities
{
    BATTLE_PET_ABILITY_TRAP = 427,
    BATTLE_PET_ABILITY_STRONG_TRAP = 77,
    BATTLE_PET_ABILITY_PRISTINE_TRAP = 135,
    BATTLE_PET_ABILITY_GM_TRAP = 549
};

enum BattlePetAchievementIds
{
    BATTLE_PET_ACHIEVEMENT_JUST_A_PUP = 6566,
    BATTLE_PET_ACHIEVEMENT_NEWBIE = 7433
};

enum BattlePetSpells
{
    SPELL_BATTLE_PET_TRAINING_PASSIVE = 119467,
    SPELL_TRACK_PETS = 122026,
    SPELL_REVIVE_BATTLE_PETS = 125439
};

inline uint32 BattlePetTrapAbilityForLoadoutFlags(uint8 flags)
{
    if (flags & BATTLE_PET_LOADOUT_GM_TRAP)
        return BATTLE_PET_ABILITY_GM_TRAP;

    if (flags & BATTLE_PET_LOADOUT_PRISTINE_TRAP)
        return BATTLE_PET_ABILITY_PRISTINE_TRAP;

    if (flags & BATTLE_PET_LOADOUT_STRONG_TRAP)
        return BATTLE_PET_ABILITY_STRONG_TRAP;

    if (flags & BATTLE_PET_LOADOUT_TRAP)
        return BATTLE_PET_ABILITY_TRAP;

    return 0;
}

inline uint8 BattlePetLoadoutSlotForAchievement(uint32 achievementId)
{
    switch (achievementId)
    {
        case BATTLE_PET_ACHIEVEMENT_NEWBIE:
            return BATTLE_PET_LOADOUT_SLOT_2;
        case BATTLE_PET_ACHIEVEMENT_JUST_A_PUP:
            return BATTLE_PET_LOADOUT_SLOT_3;
        default:
            return BATTLE_PET_LOADOUT_SLOT_NONE;
    }
}

enum PetBattleRequestFailedReason
{
    PET_BATTLE_REQUEST_FAILED_INVALID_TARGET = 0,
    PET_BATTLE_REQUEST_FAILED_ALL_PETS_DEAD = 15,
    PET_BATTLE_REQUEST_FAILED_NO_PETS = 16
};

namespace Skyfire::BattlePetPackets
{
    struct BattlePetFinalRound;
    struct BattlePetRoundResult;
}

class BattlePetMgr
{
public:
    BattlePetMgr(Player* owner)
        : m_owner(owner), m_summon(NULL), m_summonId(0), m_summonLastId(0), m_loadoutSave(false), m_loadoutFlags(0),
        m_activePetBattlePlayerStateApplied(false) { }

    ~BattlePetMgr();

    Player* GetOwner() const { return m_owner; }
    BattlePet* GetBattlePet(uint64 id) const;
    uint32 GetBattlePetCount() const;
    uint8 GetBattlePetCount(uint16 speciesId) const;
    std::string GetBattlePetSpeciesName(BattlePet const* battlePet) const;
    std::vector<BattlePet*> FindBattlePetNameMatches(std::string const& name) const;
    uint8 GetLoadoutSlotForBattlePet(uint64 id) const;

    uint64 GetCurrentSummonId() const { return m_summonId; }
    TempSummon* GetCurrentSummon() const { return m_summon; }
    void SetCurrentSummonId(uint64 summonId) { m_summonId = summonId; }
    void SetCurrentSummon(TempSummon* summon) { m_summon = summon; }

    void UnSummonCurrentBattlePet(bool temporary);
    void ResummonLastBattlePet();

    void LoadFromDb(PreparedQueryResult result);
    void LoadSlotsFromDb(PreparedQueryResult result);
    void SaveToDb();
    void SaveToDb(SQLTransaction& trans);
    void SaveSlotsToDb(SQLTransaction& trans);

    void UnlockLoadoutSlot(uint8 slot);
    bool HasLoadoutSlot(uint8 slot) const;
    bool HasLoadoutBattlePet() const;
    BattlePet* GetFirstAliveLoadoutBattlePet() const;
    uint64 GetLoadoutSlot(uint8 slot) const;
    void SetLoadoutSlot(uint8 slot, uint64 id, bool save = false);

    bool HasLoadoutFlag(uint8 flag) const { return (m_loadoutFlags & flag) != 0; };
    uint8 GetLoadoutFlags() const { return m_loadoutFlags; };
    void SetLoadoutFlag(uint8 flag);
    uint32 GetTrapAbility() const;
    void OnAchievementEarned(uint32 achievementId);

    BattlePet* Create(uint16 speciesId);
    BattlePet* Create(uint16 speciesId, uint8 level, uint8 quality, bool notification = true);
    BattlePet* Create(uint16 speciesId, uint8 level, uint8 quality, uint8 breed, bool notification = true);
    bool CanCreateBattlePet(uint16 speciesId) const;
    void Delete(BattlePet* battlePet);
    void CageBattlePet(ObjectGuid guid);
    void HealBattlePets(uint8 percent);
    void StartWildPetBattle(uint64 enemyGuid, uint64 allyPetId, uint32 allyMaxHealth, uint32 allyHealth,
        uint64 enemyPetId, uint32 enemyMaxHealth, uint32 enemyHealth,
        uint16 enemySpecies = 0, uint8 enemyLevel = 0, uint8 enemyQuality = 0,
        uint8 enemyBreed = 0, uint8 allyFrontPet = 0);
    void StartPvpPetBattle(uint64 opponentGuid, uint64 allyPetId, uint32 allyMaxHealth, uint32 allyHealth,
        uint64 enemyPetId, uint32 enemyMaxHealth, uint32 enemyHealth,
        uint16 enemySpecies = 0, uint8 enemyLevel = 0, uint8 enemyQuality = 0,
        uint8 allyFrontPet = 0, uint8 enemyFrontPet = 3,
        BattlePetAchievementSource source = BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_DUEL);
    void HideActivePetBattleWorldObject(Creature* creature);
    bool ShouldHideActivePetBattleWorldObject(uint64 guid) const
    {
        return m_activePetBattleWorldObjectHidden && m_activePetBattleWorldObjectGuid == guid;
    }
    void ApplyActivePetBattlePlayerState(float faceX, float faceY);
    void ClearActivePetBattle();
    bool HasActivePetBattle() const { return m_activePetBattle.IsActive(); }
    bool IsPetBattlePvpQueued() const { return m_petBattlePvpQueued; }
    void SetPetBattlePvpQueued(bool queued) { m_petBattlePvpQueued = queued; }
    ActivePetBattle const& GetActivePetBattle() const { return m_activePetBattle; }
    uint8 GetActivePetBattleTrapStatus() const;
    bool SetActivePetBattleAllyPet(uint8 petIndex, uint64 petId, uint32 maxHealth, uint32 health);
    bool SelectActivePetBattleFrontPet(uint8 frontPet);
    bool ApplyEnemyBattlePetDamage(uint32 damage, uint32 abilityEffectId,
        Skyfire::BattlePetPackets::BattlePetRoundResult& round);
    bool ApplyBattlePetAbilityInput(uint32 roundId, uint32 damage, uint32 abilityEffectId,
        uint8 abilitySlot, uint32 abilityId, uint16 abilityCooldown,
        Skyfire::BattlePetPackets::BattlePetRoundResult& round,
        Skyfire::BattlePetPackets::BattlePetFinalRound* finalRound = NULL);
    bool ApplyBattlePetAbilityExchangeInput(uint32 roundId,
        uint32 allyDamage, uint32 allyAbilityEffectId,
        uint8 allyAbilitySlot, uint32 allyAbilityId, uint16 allyAbilityCooldown,
        uint32 enemyDamage, uint32 enemyAbilityEffectId,
        uint32 allyIncomingDamageReduction, uint8 allyIncomingDamageReductionRounds,
        uint32 enemyIncomingDamageReduction, uint8 enemyIncomingDamageReductionRounds,
        Skyfire::BattlePetPackets::BattlePetRoundResult& round,
        Skyfire::BattlePetPackets::BattlePetFinalRound* finalRound = NULL);
    bool ApplyBattlePetAbilityTurnExchangeInput(uint32 roundId,
        uint8 allyAbilitySlot, uint32 allyAbilityId, uint8 turnIndex,
        uint16 power, uint8 level,
        uint32 enemyDamage, uint32 enemyAbilityEffectId,
        uint32 allyIncomingDamageReduction, uint8 allyIncomingDamageReductionRounds,
        uint32 enemyIncomingDamageReduction, uint8 enemyIncomingDamageReductionRounds,
        Skyfire::BattlePetPackets::BattlePetRoundResult& round,
        Skyfire::BattlePetPackets::BattlePetFinalRound* finalRound = NULL);
    bool ApplyBattlePetSwapInput(uint32 roundId, uint8 newFrontPet,
        Skyfire::BattlePetPackets::BattlePetRoundResult& round);
    bool ApplyBattlePetForfeitInput(uint32 roundId, Skyfire::BattlePetPackets::BattlePetFinalRound& finalRound);
    bool ApplyBattlePetTrapInput(uint32 roundId, uint32 trapAbilityEffectId,
        uint32 enemyDamage, uint32 enemyAbilityEffectId,
        Skyfire::BattlePetPackets::BattlePetRoundResult& round,
        Skyfire::BattlePetPackets::BattlePetFinalRound& finalRound);
    void FinishActivePetBattle(uint8 winner);
    bool BuildActivePetBattleFinalRound(bool abandoned, Skyfire::BattlePetPackets::BattlePetFinalRound& finalRound,
        bool captured = false);
    BattlePetAchievementContext BuildActivePetBattleAchievementContext(
        uint16 speciesId, uint8 quality, bool won, bool captured) const;

    void SendBattlePetDeleted(uint64 id);
    void SendBattlePetJournalLock();
    void SendBattlePetJournal();
    void SendBattlePetSlotUpdate(uint8 slot, bool notification, uint64 id = 0);
    void SendBattlePetUpdate(BattlePet* battlePet, bool notification);
    void SendPetBattleRequestFailed(uint8 reason);
    void UpdateBattlePetLevelAchievement(BattlePet const& battlePet, uint8 initialLevel, uint8 finalLevel);

private:
    bool ApplyAchievementLoadoutReward(uint32 achievementId);
    void SyncAchievementLoadoutRewards();
    void ApplyActivePetBattleRoundState(Skyfire::BattlePetPackets::BattlePetRoundResult& round) const;
    void DespawnResolvedWildPetBattleWorldObject();
    void ClearActivePetBattleWorldObjectState();
    void ClearActivePetBattlePlayerState();
    void PersistActivePetBattleHealth();
    uint16 RewardActivePetBattlePet(BattlePet* battlePet, bool awardExperience);
    uint32 GetUniqueBattlePetSpeciesCount() const;
    void UpdateBattlePetCollectionAchievements(BattlePet const* battlePet = NULL);
    void UpdateCapturedBattlePetAchievements(BattlePet const& battlePet);
    void UpdateFinishedPetBattleAchievements();

    Player* m_owner;
    BattlePetSet m_battlePetSet;

    TempSummon* m_summon;
    uint64 m_summonId;
    uint64 m_summonLastId;

    bool m_loadoutSave;
    uint8 m_loadoutFlags;
    uint64 m_loadout[BATTLE_PET_MAX_LOADOUT_SLOTS] = { };
    ActivePetBattle m_activePetBattle;
    bool m_activePetBattlePlayerStateApplied;
    bool m_activePetBattleWorldObjectHidden = false;
    uint64 m_activePetBattleWorldObjectGuid = 0;
    bool m_petBattlePvpQueued = false;
};

struct PetBattleRequest
{
    enum PetBattleType
    {
        PET_BATTLE_TYPE_PVE = 0,
        PET_BATTLE_TYPE_PVP_DUEL = 1,
        PET_BATTLE_TYPE_PVP_MATCHMAKING = 2
    };
    uint64 EnemyGUID;
    uint32 LocationResult;                              // name dumped from client, use unknown

    G3D::Vector3 Origin;
    G3D::Vector3 Positions[2];
    float Orientation;

    // Data for creation of pet battle
    PetBattleType Type;
    Player* Challenger;
    Unit* Enemy;
};


#endif
