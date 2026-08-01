/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BattlePetPackets.h"

#include "BattlePet.h"
#include "BattlePetBattleState.h"
#include "DB2Stores.h"

#include <algorithm>
#include <cmath>

namespace Skyfire::BattlePetPackets
{
    namespace
    {
        uint8 constexpr PetBattleEffectTargetStatChange = 0;
        uint8 constexpr PetBattleEffectTargetState = 1;
        uint8 constexpr PetBattleEffectTargetAbilityChange = 2;
        uint8 constexpr PetBattleEffectTargetPet = 3;
        uint8 constexpr PetBattleEffectTargetNone = 4;
        uint8 constexpr PetBattleEffectTargetTriggerAbility = 5;
        uint8 constexpr PetBattleEffectTargetNpcEmote = 6;
        uint8 constexpr PetBattleEffectTargetAura = 7;
        uint8 constexpr PetBattleEffectSetHealth = 0;
        uint8 constexpr PetBattleEffectAuraApply = 1;
        uint8 constexpr PetBattleEffectAuraCancel = 2;
        uint8 constexpr PetBattleEffectActivePet = 4;
        uint8 constexpr PetBattleEffectCatch = 5;
        uint8 constexpr PetBattleEffectTriggerAbility = 7;
        uint16 constexpr PetBattleEffectFlagMiss = 0x0002;
        uint8 constexpr PetBattleRoundResultNormal = 2;
        uint8 constexpr PetBattleRoundResultCatchOrKill = 3;

        void WriteRoundEffectBits(WorldPacket& data, BattlePetRoundEffect const& effect)
        {
            data.WriteBit(!effect.AbilityEffectID);
            data.WriteBit(!effect.PetBattleEffectType);
            data.WriteBits(effect.Targets.size(), 25);
            data.WriteBit(!effect.SourceAuraInstanceID);

            for (BattlePetRoundEffectTarget const& target : effect.Targets)
            {
                data.WriteBits(target.Type, 3);

                if (target.Type == PetBattleEffectTargetStatChange)
                    data.WriteBit(!target.HasStatus);

                if (target.Type == PetBattleEffectTargetAura)
                {
                    data.WriteBit(!target.HasAuraInstanceID);
                    data.WriteBit(!target.HasAuraAbilityID);
                    data.WriteBit(!target.HasRoundsRemaining);
                    data.WriteBit(!target.HasCurrentRound);
                }

                if (target.Type == PetBattleEffectTargetNpcEmote)
                    data.WriteBit(1);

                if (target.Type == PetBattleEffectTargetAbilityChange)
                {
                    data.WriteBit(1);
                    data.WriteBit(1);
                    data.WriteBit(1);
                }

                if (target.Type == PetBattleEffectTargetTriggerAbility)
                    data.WriteBit(1);

                if (target.Type == PetBattleEffectTargetState)
                {
                    data.WriteBit(!target.HasStateValue);
                    data.WriteBit(!target.HasStateID);
                }

                data.WriteBit(target.PetX == -1);

                if (target.Type == PetBattleEffectTargetPet)
                    data.WriteBit(!target.HasRemainingHealth);
            }

            data.WriteBit(effect.CasterPBOID == -1);
            data.WriteBit(!effect.TurnInstanceID);
            data.WriteBit(!effect.StackDepth);
            data.WriteBit(!effect.Flags);
        }

        void WriteRoundEffectBytes(WorldPacket& data, BattlePetRoundEffect const& effect)
        {
            if (effect.Flags)
                data << uint16(effect.Flags);

            if (effect.AbilityEffectID)
                data << uint32(effect.AbilityEffectID);

            for (BattlePetRoundEffectTarget const& target : effect.Targets)
            {
                if (target.PetX != -1)
                    data << uint8(target.PetX);

                if (target.Type == PetBattleEffectTargetStatChange && target.HasStatus)
                    data << uint32(target.Status);

                if (target.Type == PetBattleEffectTargetState)
                {
                    if (target.HasStateID)
                        data << uint32(target.StateID);

                    if (target.HasStateValue)
                        data << uint32(target.StateValue);
                }

                if (target.Type == PetBattleEffectTargetAbilityChange)
                {
                    data << uint32(0);
                    data << uint32(0);
                    data << uint32(0);
                }

                if (target.Type == PetBattleEffectTargetPet && target.HasRemainingHealth)
                    data << int32(target.RemainingHealth);

                if (target.Type == PetBattleEffectTargetAura)
                {
                    if (target.HasAuraInstanceID)
                        data << uint16(target.AuraInstanceID);
                    if (target.HasAuraAbilityID)
                        data << uint32(target.AuraAbilityID);
                    if (target.HasRoundsRemaining)
                        data << uint32(target.RoundsRemaining);
                    if (target.HasCurrentRound)
                        data << uint32(target.CurrentRound);
                }

                if (target.Type == PetBattleEffectTargetTriggerAbility)
                    data << uint32(0);

                if (target.Type == PetBattleEffectTargetNpcEmote)
                    data << uint32(0);
            }

            if (effect.PetBattleEffectType)
                data << uint8(effect.PetBattleEffectType);

            // Match the original MoP round-result value order: TurnInstance before Caster.
            if (effect.TurnInstanceID)
                data << uint16(effect.TurnInstanceID);

            if (effect.CasterPBOID != -1)
                data << uint8(effect.CasterPBOID);

            if (effect.StackDepth)
                data << uint8(effect.StackDepth);

            if (effect.SourceAuraInstanceID)
                data << uint16(effect.SourceAuraInstanceID);
        }
    }

    InitialUpdatePetData BuildInitialUpdatePetData(BattlePet const& battlePet)
    {
        InitialUpdatePetData data;
        data.Id = battlePet.GetId();
        data.Level = battlePet.GetLevel();
        data.Quality = battlePet.GetQuality();
        data.Power = battlePet.GetPower();
        data.MaxHealth = battlePet.GetMaxHealth();
        data.Speed = battlePet.GetSpeed();
        data.CurrentHealth = battlePet.GetCurrentHealth();
        data.Xp = battlePet.GetXp();
        data.Flags = battlePet.GetFlags();
        data.Nickname = battlePet.GetNickname();
        data.Species = battlePet.GetSpecies();
        for (uint8 i = 0; i < 3; ++i)
            data.Abilities[i] = BattlePetGetSpeciesAbility(data.Species, i, data.Flags, data.Level);

        return data;
    }

    InitialUpdatePetData BuildWildInitialUpdatePetData(uint64 id, uint16 speciesId, uint8 level, uint8 quality, uint8 breed)
    {
        BattlePet battlePet(id, speciesId, level, quality, breed);
        return BuildInitialUpdatePetData(battlePet);
    }

    BattlePetInput ReadBattlePetInput(WorldPacket& data)
    {
        BattlePetInput input;

        input.HasAbilityID = !data.ReadBit();
        input.HasNewFrontPet = !data.ReadBit();
        input.HasDebugFlags = !data.ReadBit();
        input.IgnoreAbandonPenalty = data.ReadBit();
        input.HasBattleInterrupted = !data.ReadBit();
        input.HasRoundID = !data.ReadBit();
        input.HasMoveType = !data.ReadBit();

        if (input.HasMoveType)
            data >> input.MoveType;

        if (input.HasDebugFlags)
            data >> input.DebugFlags;

        if (input.HasRoundID)
            data >> input.RoundID;

        if (input.HasBattleInterrupted)
            data >> input.BattleInterrupted;

        if (input.HasAbilityID)
            data >> input.AbilityID;

        if (input.HasNewFrontPet)
            data >> input.NewFrontPet;

        return input;
    }

    BattlePetInputCommand BuildBattlePetInputCommand(BattlePetInput const& input)
    {
        BattlePetInputCommand command;
        command.HasRoundID = input.HasRoundID;
        command.RoundID = input.RoundID;
        command.AbilityID = input.AbilityID;
        command.MoveType = input.MoveType;
        command.NewFrontPet = input.NewFrontPet;
        command.IgnoreAbandonPenalty = input.IgnoreAbandonPenalty;
        command.BattleInterrupted = input.BattleInterrupted;

        if (!input.HasRoundID)
            return command;

        if (input.HasMoveType)
        {
            switch (input.MoveType)
            {
                case BATTLE_PET_INPUT_MOVE_ABILITY:
                    if (input.HasAbilityID)
                        command.Action = BATTLE_PET_INPUT_ACTION_ABILITY;
                    break;
                case BATTLE_PET_INPUT_MOVE_SWAP:
                    if (input.HasNewFrontPet)
                        command.Action = BATTLE_PET_INPUT_ACTION_SWAP;
                    break;
                case BATTLE_PET_INPUT_MOVE_TRAP:
                    command.Action = BATTLE_PET_INPUT_ACTION_TRAP;
                    break;
                case BATTLE_PET_INPUT_MOVE_FORFEIT:
                    command.Action = BATTLE_PET_INPUT_ACTION_FORFEIT;
                    break;
                default:
                    break;
            }
        }
        else if (input.HasNewFrontPet)
            command.Action = BATTLE_PET_INPUT_ACTION_SWAP;
        else if (input.HasAbilityID)
            command.Action = BATTLE_PET_INPUT_ACTION_ABILITY;

        if (command.Action == BATTLE_PET_INPUT_ACTION_NONE && input.IgnoreAbandonPenalty)
            command.Action = BATTLE_PET_INPUT_ACTION_FORFEIT;

        return command;
    }

    BattlePetFirstPetSelection ReadBattlePetFirstPetSelection(WorldPacket& data)
    {
        BattlePetFirstPetSelection selection;
        data >> selection.PetID;
        return selection;
    }

    BattlePetRequestUpdate ReadBattlePetRequestUpdate(WorldPacket& data)
    {
        BattlePetRequestUpdate request;

        if (data.rpos() + 2 > data.size())
            return request;

        data.ReadGuidMask(request.Guid, 6, 2, 3, 7, 0, 4);
        request.Cancelled = data.ReadBit();
        data.ReadGuidMask(request.Guid, 5, 1);

        uint8 guidByteCount = 0;
        for (uint8 i = 0; i < 8; ++i)
            if (request.Guid[i])
                ++guidByteCount;

        if (data.rpos() + guidByteCount > data.size())
            return BattlePetRequestUpdate();

        data.ReadGuidBytes(request.Guid, 3, 5, 6, 7, 1, 0, 2, 4);

        return request;
    }

    BattlePetRoundEffect BuildPetSwapEffect(uint8 casterPet, uint8 targetPet)
    {
        BattlePetRoundEffectTarget target;
        target.PetX = targetPet;
        target.Type = PetBattleEffectTargetNone;

        BattlePetRoundEffect effect;
        effect.CasterPBOID = casterPet;
        effect.PetBattleEffectType = PetBattleEffectActivePet;
        effect.Targets.push_back(target);

        return effect;
    }

    BattlePetRoundEffect BuildDamageEffect(uint8 casterPet, uint8 targetPet, int32 remainingHealth,
        uint32 abilityEffectId, uint16 turnInstanceId)
    {
        BattlePetRoundEffectTarget target;
        target.PetX = targetPet;
        target.Type = PetBattleEffectTargetPet;
        target.HasRemainingHealth = true;
        target.RemainingHealth = remainingHealth;

        BattlePetRoundEffect effect;
        effect.CasterPBOID = casterPet;
        effect.TurnInstanceID = turnInstanceId;
        effect.StackDepth = 1;
        effect.AbilityEffectID = abilityEffectId;
        effect.Targets.push_back(target);

        return effect;
    }

    BattlePetRoundEffect BuildAuraEffect(uint8 casterPet, uint8 targetPet, uint32 abilityEffectId,
        uint32 auraAbilityId, uint32 roundsRemaining, uint16 auraInstanceId, bool applying,
        uint16 turnInstanceId)
    {
        // Do not emit SetHealth (type 0) or full aura target payloads — both desync 5.4.8 HP
        // (pets "killed", multi-hundred-thousand heals on the wrong slot). AbilityEffectID +
        // AuraApply/Cancel with an empty target is enough for channel fly-ups; shield buffs use
        // the same minimal shape so the round bitstream stays aligned.
        (void)auraAbilityId;
        (void)roundsRemaining;
        (void)auraInstanceId;

        BattlePetRoundEffectTarget target;
        target.PetX = targetPet;
        target.Type = PetBattleEffectTargetNone;

        BattlePetRoundEffect effect;
        effect.CasterPBOID = casterPet;
        effect.PetBattleEffectType = applying ? PetBattleEffectAuraApply : PetBattleEffectAuraCancel;
        effect.TurnInstanceID = turnInstanceId;
        effect.StackDepth = 1;
        effect.AbilityEffectID = abilityEffectId;
        effect.Targets.push_back(target);
        return effect;
    }

    BattlePetRoundEffect BuildMissEffect(uint8 casterPet, uint8 targetPet, uint32 abilityEffectId,
        int32 remainingHealth, uint16 turnInstanceId)
    {
        // Miss still uses SetHealth + miss flag so the client keeps the real HP and shows "missed".
        BattlePetRoundEffect effect = BuildDamageEffect(casterPet, targetPet, remainingHealth,
            abilityEffectId, turnInstanceId);
        effect.Flags = PetBattleEffectFlagMiss;
        return effect;
    }

    BattlePetRoundEffect BuildAbilityVisualEffect(uint8 casterPet, uint8 targetPet, uint32 abilityEffectId,
        uint16 turnInstanceId)
    {
        BattlePetRoundEffectTarget target;
        target.PetX = targetPet;
        target.Type = PetBattleEffectTargetNone;

        BattlePetRoundEffect effect;
        effect.CasterPBOID = casterPet;
        effect.PetBattleEffectType = PetBattleEffectTriggerAbility;
        effect.TurnInstanceID = turnInstanceId;
        effect.StackDepth = 1;
        effect.AbilityEffectID = abilityEffectId;
        effect.Targets.push_back(target);
        return effect;
    }

    BattlePetRoundEffect BuildCatchEffect(uint8 casterPet, uint8 targetPet, uint32 abilityEffectId,
        bool captured, uint16 turnInstanceId)
    {
        BattlePetRoundEffectTarget target;
        target.PetX = targetPet;
        target.Type = PetBattleEffectTargetStatChange;
        target.HasStatus = true;
        target.Status = captured ? 1 : 0;

        BattlePetRoundEffect effect;
        effect.CasterPBOID = casterPet;
        effect.PetBattleEffectType = PetBattleEffectCatch;
        effect.Flags = captured ? PetBattleEffectFlagMiss : 0;
        effect.TurnInstanceID = turnInstanceId;
        effect.StackDepth = 1;
        effect.AbilityEffectID = abilityEffectId;
        effect.Targets.push_back(target);

        return effect;
    }

    void AppendDamageRoundEffect(BattlePetRoundResult& round, uint8 casterPet, uint8 targetPet,
        int32 remainingHealth, uint32 abilityEffectId, bool targetDied)
    {
        round.Effects.push_back(BuildDamageEffect(casterPet, targetPet, remainingHealth, abilityEffectId));

        if (targetDied)
            round.DeadPets.push_back(targetPet);
    }

    BattlePetRoundResult BuildDamageRoundResult(uint32 roundId, uint8 casterPet, uint8 targetPet,
        int32 remainingHealth, uint32 abilityEffectId, bool targetDied)
    {
        BattlePetRoundResult round;
        round.RoundID = roundId;
        AppendDamageRoundEffect(round, casterPet, targetPet, remainingHealth, abilityEffectId, targetDied);

        return round;
    }

    BattlePetRoundResult BuildRoundResultFromTurn(ActivePetBattleTurn const& turn, uint32 abilityEffectId)
    {
        BattlePetRoundResult round;
        round.RoundID = turn.RoundID;

        if (!turn.Accepted || !turn.HasRoundResult)
            return round;

        AppendRoundCooldowns(round, turn);

        switch (turn.EffectKind)
        {
            case ACTIVE_PET_BATTLE_TURN_EFFECT_DAMAGE:
                AppendDamageRoundEffect(round, turn.CasterPet, turn.TargetPet, int32(turn.RemainingHealth),
                    abilityEffectId, turn.TargetDied);

                if (turn.RequiresFrontPet)
                    round.InputFlags[0] = BATTLE_PET_ROUND_INPUT_FLAG_SELECT_NEW_FRONT_PET;
                break;
            case ACTIVE_PET_BATTLE_TURN_EFFECT_SWAP:
                round.Effects.push_back(BuildPetSwapEffect(turn.CasterPet, turn.TargetPet));
                break;
            default:
                break;
        }

        return round;
    }

    void AppendRoundCooldowns(BattlePetRoundResult& round, ActivePetBattleTurn const& turn)
    {
        for (ActivePetBattleCooldown const& source : turn.Cooldowns)
        {
            BattlePetRoundCooldown cooldown;
            cooldown.AbilityID = source.AbilityID;
            cooldown.Cooldown = source.Cooldown;
            // Client GetAbilityState treats Lockdown > 0 as "ability unusable" (Nevermore-style).
            // Multi-turn channels are tracked server-side via NextTurnIndex; never advertise lockdown.
            cooldown.Lockdown = 0;
            cooldown.AbilitySlot = source.AbilitySlot;
            cooldown.PetPBOID = source.PetPBOID;
            cooldown.HasPetPBOID = true;
            round.Cooldowns.push_back(cooldown);
        }
    }

    void AppendChannelInputFlags(BattlePetRoundResult& round, uint8 abilitySlot)
    {
        // Intentionally leave abilities unlocked. Locking slots (especially ability 3, which has no
        // dedicated unlock flag) prevents the player from sending the input that continues a channel.
        (void)round;
        (void)abilitySlot;
    }

    void MarkRoundResultAsCatchOrKill(BattlePetRoundResult& round)
    {
        round.NextPetBattleState = PetBattleRoundResultCatchOrKill;
    }

    BattlePetFinalRound BuildFinalRoundState(bool allyWon, bool abandoned,
        std::vector<BattlePetFinalRoundPet> const& pets)
    {
        BattlePetFinalRound finalRound;
        finalRound.Winners[0] = allyWon;
        finalRound.Winners[1] = !allyWon;
        finalRound.Abandoned = abandoned;
        finalRound.Pets = pets;

        return finalRound;
    }

    BattlePetFinalRound BuildFinalRoundState(bool allyWon, bool abandoned,
        BattlePetFinalRoundPet const& allyPet, BattlePetFinalRoundPet const& enemyPet)
    {
        std::vector<BattlePetFinalRoundPet> pets;
        pets.push_back(allyPet);
        pets.push_back(enemyPet);
        return BuildFinalRoundState(allyWon, abandoned, pets);
    }

    WorldPacket BuildInitialUpdatePacket(ObjectGuid wildBattlePetGuid, InitialUpdateTeamData const* teams,
        uint8 teamCount, bool isPvP)
    {
        WorldPacket data(SMSG_PET_BATTLE_INITIAL_UPDATE, 1000);

        bool const hasWaitingForFrontPetsMaxSecs = true;
        bool const hasPvPMaxRoundTime = true;
        bool const hasForfeitPenalty = true;
        bool const hasCreatureId = false;
        bool const hasDisplayId = false;
        ObjectGuid battlePetGuid = isPvP ? ObjectGuid(0) : wildBattlePetGuid;

        for (uint8 i = 0; i < 3; i++)
        {
            data.WriteBits(0, 21);
            data.WriteBits(0, 21);
        }

        for (uint8 i = 0; i < 2; i++)
        {
            InitialUpdateTeamData const* team = teams && i < teamCount ? &teams[i] : NULL;
            uint8 const battlePetCount = team
                ? std::min<uint8>(team->PetCount, INITIAL_UPDATE_MAX_TEAM_PETS)
                : 0;
            bool const hasRoundTimeSec = false;
            bool const hasFrontPet = battlePetCount != 0;
            bool const trapStatus = team && team->TrapStatus != 0;

            ObjectGuid characterGuid = team ? team->OwnerGuid : 0;

            data.WriteBit(!trapStatus);
            data.WriteBits(battlePetCount, 2);
            data.WriteBit(characterGuid[2]);

            for (uint8 j = 0; j < battlePetCount; j++)
            {
                InitialUpdatePetData const& activePet = team->Pets[j];
                ObjectGuid petEntry = activePet.Id;

                data.WriteBits(0, 21);
                data.WriteBit(petEntry[3]);
                data.WriteBits(0, 21);

                uint8 abilityCount = 0;
                for (uint8 k = 0; k < 3; k++)
                    if (activePet.Abilities[k])
                        abilityCount++;

                data.WriteBit(petEntry[0]);
                data.WriteBit(!activePet.Flags);
                data.WriteGuidMask(petEntry, 5, 1);

                data.WriteBits(abilityCount, 20);
                data.WriteBit(0);
                data.WriteBits(activePet.Nickname.size(), 7);
                data.WriteGuidMask(petEntry, 2, 4);

                for (uint8 l = 0; l < 3; l++)
                    if (activePet.Abilities[l])
                        data.WriteBit(0);

                data.WriteGuidMask(petEntry, 6, 7);
            }

            data.WriteBit(!hasFrontPet);
            data.WriteBit(!hasRoundTimeSec);
            data.WriteGuidMask(characterGuid, 5, 3, 4, 6, 7, 0, 1);
        }

        data.WriteBit(!hasForfeitPenalty);

        data.WriteBit(0);
        data.WriteBit(isPvP);

        data.WriteBit(!hasDisplayId);
        data.WriteBit(!hasCreatureId);
        data.WriteBit(!hasWaitingForFrontPetsMaxSecs);
        data.WriteBit(uint64(battlePetGuid) == 0);

        data.WriteGuidMask(battlePetGuid, 2, 4, 5, 1, 3, 6, 7, 0);

        data.WriteBit(!hasPvPMaxRoundTime);
        data.WriteBit(0);

        data.FlushBits();

        for (uint8 i = 0; i < 2; i++)
        {
            InitialUpdateTeamData const* team = teams && i < teamCount ? &teams[i] : NULL;
            uint8 const battlePetCount = team ? std::min<uint8>(team->PetCount, INITIAL_UPDATE_MAX_TEAM_PETS) : 0;
            ObjectGuid characterGuid = team ? team->OwnerGuid : 0;

            bool const hasRoundTimeSec = false;
            bool const hasFrontPet = battlePetCount != 0;
            bool const trapStatus = team && team->TrapStatus != 0;

            uint8 battlePetTeamIndex = 0;
            for (uint8 j = 0; j < battlePetCount; j++)
            {
                data << battlePetTeamIndex;
                battlePetTeamIndex++;

                InitialUpdatePetData const& activePet = team->Pets[j];
                ObjectGuid petEntry = activePet.Id;
                uint8 const petBattleObjectId = i == 0 ? j : uint8(3 + j);

                for (uint8 k = 0; k < 3; k++)
                {
                    if (!activePet.Abilities[k])
                        continue;

                    data << uint32(activePet.Abilities[k]);
                    data << uint8(k);
                    data << uint8(petBattleObjectId);
                    data << uint16(0);
                    data << uint16(0);
                }

                data << uint32(0);
                data.WriteByteSeq(petEntry[4]);
                data << uint16(activePet.Level);
                data.WriteByteSeq(petEntry[7]);
                data << uint16(activePet.Quality);
                data.WriteByteSeq(petEntry[6]);
                data << uint32(activePet.Power);
                data.WriteByteSeq(petEntry[0]);
                data << uint32(activePet.MaxHealth);
                data.WriteByteSeq(petEntry[5]);
                data.WriteByteSeq(petEntry[2]);
                data << uint32(activePet.Speed);
                data << uint32(activePet.CurrentHealth);
                data.WriteByteSeq(petEntry[3]);
                data << uint32(0);
                data.WriteByteSeq(petEntry[1]);
                data << uint32(0);
                data << uint16(activePet.Xp);
                if (activePet.Flags)
                    data << uint16(activePet.Flags);

                data.WriteString(activePet.Nickname);
                data << uint32(activePet.Species);
            }

            if (trapStatus)
                data << uint32(team->TrapStatus);

            data << uint32(team ? team->TrapAbility : 0);
            data.WriteGuidBytes(characterGuid, 5, 7, 6, 1, 4, 0);

            if (hasFrontPet)
                data << uint8(team ? team->FrontPet : 0);

            data << uint8(6);

            if (hasRoundTimeSec)
                data << uint16(0);

            data.WriteGuidBytes(characterGuid, 3, 2);
        }

        if (hasForfeitPenalty)
            data << uint8(10);

        data << uint8(1);

        data.WriteGuidBytes(battlePetGuid, 5, 4, 3, 2, 7, 0, 1, 6);

        if (hasDisplayId)
            data << uint32(0);

        if (hasPvPMaxRoundTime)
            data << uint16(30);

        data << uint32(0);

        if (hasWaitingForFrontPetsMaxSecs)
            data << uint16(30);

        if (hasCreatureId)
            data << uint32(0);

        return data;
    }

    WorldPacket BuildInitialUpdatePacket(ObjectGuid wildBattlePetGuid, InitialUpdatePetData const* teamPets,
        uint8 teamPetCount, bool isPvP)
    {
        InitialUpdateTeamData teams[2];
        for (uint8 i = 0; teamPets && i < 2 && i < teamPetCount; i++)
        {
            teams[i].Pets[0] = teamPets[i];
            teams[i].PetCount = 1;
        }

        return BuildInitialUpdatePacket(wildBattlePetGuid, teams, 2, isPvP);
    }

    WorldPacket BuildInitialUpdatePacket(ObjectGuid wildBattlePetGuid, InitialUpdatePetData const& activePet,
        bool isPvP)
    {
        InitialUpdatePetData teamPets[2] = { activePet, activePet };
        return BuildInitialUpdatePacket(wildBattlePetGuid, teamPets, 2, isPvP);
    }

    WorldPacket BuildFirstRoundPacket(uint32 roundId, uint8 allyFrontPet, uint8 enemyFrontPet,
        uint8 allyTrapStatus, uint8 enemyTrapStatus)
    {
        uint8 const frontPets[2] = { allyFrontPet, enemyFrontPet };
        uint8 const trapStatuses[2] = { allyTrapStatus, enemyTrapStatus };

        WorldPacket data(SMSG_PET_BATTLE_FIRST_ROUND, 64);
        for (uint8 i = 0; i < 2; ++i)
        {
            data << uint16(0);                     // RoundTimeSecs
            data << uint8(trapStatuses[i]);        // NextTrapStatus
            data << uint8(0);                      // NextInputFlags
        }

        data << uint32(roundId);

        data.WriteBit(0);                          // has NextPetBattleState
        data.WriteBits(0, 3);                      // PetX died count
        data.WriteBits(0, 20);                     // Cooldown count
        data.WriteBits(2, 22);                     // Front pet effects

        for (uint8 i = 0; i < 2; ++i)
        {
            data.WriteBit(1);                      // no SourceAuraInstanceID
            data.WriteBit(1);                      // no AbilityEffectID
            data.WriteBit(1);                      // no TurnInstanceID
            data.WriteBit(1);                      // no StackDepth
            data.WriteBit(0);                      // has CasterPBOID
            data.WriteBits(1, 25);                 // Target count
            data.WriteBit(0);                      // has target PetX
            data.WriteBits(PetBattleEffectTargetNone, 3);
            data.WriteBit(1);                      // no Flags
            data.WriteBit(0);                      // has PetBattleEffectType
        }

        data.FlushBits();

        for (uint8 i = 0; i < 2; ++i)
        {
            data << uint8(frontPets[i]);           // CasterPBOID
            data << uint8(frontPets[i]);           // Target PetX
            data << uint8(PetBattleEffectActivePet);
        }

        data << uint8(PetBattleRoundResultNormal);
        return data;
    }

    WorldPacket BuildFinishedPacket()
    {
        return WorldPacket(SMSG_PET_BATTLE_FINISHED, 0);
    }

    float CalculateBattlePetFacing(G3D::Vector3 const& origin, G3D::Vector3 const* positions)
    {
        float const centerX = (positions[0].x + positions[1].x) * 0.5f;
        float const centerY = (positions[0].y + positions[1].y) * 0.5f;
        float facing = std::atan2(centerY - origin.y, centerX - origin.x);
        if (facing < 0.0f)
            facing += 6.28318530717958647692f;

        return facing;
    }

    float ResolveBattlePetFacing(bool hasClientOrientation, float clientOrientation,
        G3D::Vector3 const& origin, G3D::Vector3 const* positions)
    {
        if (hasClientOrientation)
            return clientOrientation;

        return CalculateBattlePetFacing(origin, positions);
    }

    WorldPacket BuildRoundResultPacket(BattlePetRoundResult const& round)
    {
        WorldPacket data(SMSG_PET_BATTLE_ROUND_RESULT, 128);

        for (uint8 i = 0; i < 2; ++i)
        {
            data << uint8(round.TrapStatus[i]);
            data << uint8(round.InputFlags[i]);
            data << uint16(0);                    // round time seconds
        }

        data << uint32(round.RoundID);
        data.WriteBits(round.Effects.size(), 22);

        for (BattlePetRoundEffect const& effect : round.Effects)
            WriteRoundEffectBits(data, effect);

        data.WriteBit(!round.NextPetBattleState);
        data.WriteBits(round.DeadPets.size(), 3);
        data.WriteBits(round.Cooldowns.size(), 20);

        for (BattlePetRoundCooldown const& cooldown : round.Cooldowns)
            data.WriteBit(!cooldown.HasPetPBOID);

        data.FlushBits();

        for (BattlePetRoundCooldown const& cooldown : round.Cooldowns)
        {
            data << uint16(cooldown.Cooldown);
            data << uint16(cooldown.Lockdown);
            data << uint32(cooldown.AbilityID);
            data << uint8(cooldown.AbilitySlot);

            if (cooldown.HasPetPBOID)
                data << uint8(cooldown.PetPBOID);
        }

        for (BattlePetRoundEffect const& effect : round.Effects)
            WriteRoundEffectBytes(data, effect);

        if (round.NextPetBattleState)
            data << uint8(round.NextPetBattleState);

        return data;
    }

    WorldPacket BuildFinalRoundPacket(BattlePetFinalRound const& finalRound)
    {
        WorldPacket data(SMSG_PET_BATTLE_FINAL_ROUND, 64);

        data.WriteBit(finalRound.PvPBattle);
        data.WriteBit(finalRound.Winners[0]);
        data.WriteBit(finalRound.Winners[1]);
        data.WriteBits(finalRound.Pets.size(), 20);

        for (BattlePetFinalRoundPet const& pet : finalRound.Pets)
        {
            data.WriteBit(0);                     // has new level
            data.WriteBit(pet.Caged);
            data.WriteBit(pet.SeenAction);
            data.WriteBit(0);                     // has xp
            data.WriteBit(pet.Captured);
            data.WriteBit(0);                     // has initial level
            data.WriteBit(pet.AwardedXP);
        }

        data.WriteBit(1);                         // abandoned/result ack

        for (BattlePetFinalRoundPet const& pet : finalRound.Pets)
        {
            data << uint32(pet.RemainingHealth);
            data << uint16(pet.InitialLevel);
            data << uint16(pet.Xp);
            data << uint8(pet.Pboid);
            data << uint16(pet.NewLevel);
            data << uint32(pet.MaxHealth);
        }

        for (uint8 i = 0; i < 2; ++i)
            data << uint32(finalRound.NpcCreatureID[i]);

        return data;
    }

    WorldPacket BuildFinalizeLocationPacket(G3D::Vector3 const& origin, G3D::Vector3 const* positions,
        bool hasOrientation, float orientation, bool hasLocationResult, uint32 locationResult)
    {
        WorldPacket data(SMSG_PET_BATTLE_FINALIZE_LOCATION, 100);
        data << origin.x;
        data << origin.y;

        for (uint8 i = 0; i < 2; i++)
        {
            data << positions[i].y;
            data << positions[i].x;
            data << positions[i].z;
        }

        data << origin.z;

        data.WriteBit(!hasOrientation);
        data.WriteBit(!hasLocationResult);

        if (hasLocationResult)
            data << uint32(locationResult);

        if (hasOrientation)
            data << float(orientation);

        return data;
    }

    WorldPacket BuildPvpChallengePacket(ObjectGuid challengerGuid, G3D::Vector3 const& origin, G3D::Vector3 const* positions,
        bool hasOrientation, float orientation, bool hasLocationResult, uint32 locationResult)
    {
        WorldPacket data(SMSG_PET_BATTLE_PVP_CHALLENGE, 100);

        data.WriteGuidMask(challengerGuid, 4);
        data.WriteBit(!hasOrientation);
        data.WriteGuidMask(challengerGuid, 2, 5, 7, 3, 6, 0, 1);
        data.WriteBit(!hasLocationResult);
        data.FlushBits();

        if (hasOrientation)
            data << float(orientation);

        data.WriteGuidBytes(challengerGuid, 1);
        data << origin.x;
        data.WriteGuidBytes(challengerGuid, 0, 5);
        data << origin.y;

        if (hasLocationResult)
            data << uint32(locationResult);

        data.WriteGuidBytes(challengerGuid, 2);

        for (uint8 i = 0; i < 2; ++i)
        {
            data << positions[i].y;
            data << positions[i].x;
            data << positions[i].z;
        }

        data << origin.z;
        data.WriteGuidBytes(challengerGuid, 7, 3, 4, 6);

        return data;
    }

    WorldPacket BuildQueueStatusPacket(ObjectGuid requesterGuid, uint32 status, uint32 const* slotResults)
    {
        WorldPacket data(SMSG_PET_BATTLE_QUEUE_STATUS, 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 8 + 1);

        data.WriteGuidMask(requesterGuid, 7, 2, 6, 1);
        data.WriteBit(0); // hasAverageWaitTime
        data.WriteGuidMask(requesterGuid, 4);
        data.WriteBits(0, 22);
        data.WriteGuidMask(requesterGuid, 0);
        data.WriteBit(0); // hasClientWaitTime
        data.WriteGuidMask(requesterGuid, 3, 5);

        data.FlushBits();
        data.WriteGuidBytes(requesterGuid, 2, 4);
        data << uint32(0); // CliRideTicket.Time
        data.WriteGuidBytes(requesterGuid, 3);
        data << uint32(status);
        data.WriteGuidBytes(requesterGuid, 6);
        data.WriteGuidBytes(requesterGuid, 1);
        data << uint32(0); // CliRideTicket.Type
        data.WriteGuidBytes(requesterGuid, 5, 7);
        data << uint32(0); // CliRideTicket.Id
        data.WriteGuidBytes(requesterGuid, 0);

        for (uint8 i = 0; i < PET_BATTLE_QUEUE_SLOT_COUNT; i++)
            data << uint32(slotResults ? slotResults[i] : PET_BATTLE_QUEUE_STATUS_QUEUED);

        return data;
    }

    WorldPacket BuildQueueProposeMatchPacket()
    {
        return WorldPacket(SMSG_PET_BATTLE_QUEUE_PROPOSE_MATCH, 0);
    }

    WorldPacket BuildRequestFailedPacket(uint8 reason)
    {
        WorldPacket data(SMSG_PET_BATTLE_REQUEST_FAILED, 2);
        data.WriteBit(0);
        data.FlushBits();
        data << uint8(reason);

        return data;
    }
}
