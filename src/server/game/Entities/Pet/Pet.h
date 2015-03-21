/*
 * Copyright (C) 2011-2015 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2015 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRINITYCORE_PET_H
#define TRINITYCORE_PET_H

#include "PetDefines.h"
#include "TemporarySummon.h"

#define PET_FOCUS_REGEN_INTERVAL 4 * IN_MILLISECONDS
#define HAPPINESS_LEVEL_SIZE        333000

struct PetSpell
{
    ActiveStates active;
    PetSpellState state;
    PetSpellType type;
};

typedef UNORDERED_MAP<uint32, PetSpell> PetSpellMap;
typedef std::vector<uint32> AutoSpellList;

class Player;

class Pet : public Guardian
{
    public:
        explicit Pet(Player* owner, PetType type = MAX_PET_TYPE);
        virtual ~Pet();

        void AddToWorld();
        void RemoveFromWorld();

        void SetDisplayId(uint32 modelId);

        PetType getPetType() const { return _petType; }
        void setPetType(PetType type) { _petType = type; }
        bool isControlled() const { return getPetType() == SUMMON_PET || getPetType() == HUNTER_PET; }
        bool isTemporarySummoned() const { return _duration > 0; }

        bool IsPermanentPetFor(Player* owner) const;        // pet have tab in character windows and set UNIT_FIELD_PET_NUMBER

        bool Create(uint32 guidlow, Map* map, uint32 phaseMask, uint32 Entry, uint32 pet_number);
        bool CreateBaseAtCreature(Creature* creature);
        bool CreateBaseAtCreatureInfo(CreatureTemplate const* cinfo, Unit* owner);
        bool CreateBaseAtTamed(CreatureTemplate const* cinfo, Map* map, uint32 phaseMask);
        bool LoadPetFromDB(Player* owner, uint32 petentry = 0, uint32 petnumber = 0, bool current = false);
        bool isBeingLoaded() const { return _loading;}
        void SavePetToDB(PetSaveMode mode);
        void Remove(PetSaveMode mode, bool returnreagent = false);
        static void DeleteFromDB(uint32 guidlow);

        void setDeathState(DeathState s);                   // overwrite virtual Creature::setDeathState and Unit::setDeathState
        void Update(uint32 diff);                           // overwrite virtual Creature::Update and Unit::Update

        uint8 GetPetAutoSpellSize() const { return m_autospells.size(); }
        uint32 GetPetAutoSpellOnPos(uint8 pos) const
        {
            if (pos >= m_autospells.size())
                return 0;
            else
                return m_autospells[pos];
        }

        void GivePetXP(uint32 xp);
        void GivePetLevel(uint8 level);
        void SynchronizeLevelWithOwner();
        bool HaveInDiet(ItemTemplate const* item) const;
        uint32 GetCurrentFoodBenefitLevel(uint32 itemlevel) const;
        void SetDuration(int32 dur) { _duration = dur; }
        int32 GetDuration() const { return _duration; }

        uint8 GetActiveSpecialization() const { return _activeSpecialization; }
        void SetActiveSpecialization(uint8 spec){ _activeSpecialization = spec; }
        uint8 GetSpecializationCount() const { return _specializationCount; }
        void SetSpecializationCount(uint8 count) { _specializationCount = count; }
        
        uint32 GetSpecialization() { return GetSpecialization(GetActiveSpecialization()); }
        uint32 GetSpecialization(uint8 specIdx) { return _specializations[specIdx]; }
        void SetSpecialization(uint32 specGroup) { _specializations[GetActiveSpecialization()] = specGroup; };
        void SetSpecialization(uint8 specIdx, uint32 specGroup) { _specializations[specIdx] = specGroup; };

        void SendPetSpecialization();
        void LearnSpecializationSpells();
        void UnlearnSpecializationSpells();

        /*
        bool UpdateStats(Stats stat);
        bool UpdateAllStats();
        void UpdateResistances(uint32 school);
        void UpdateArmor();
        void UpdateMaxHealth();
        void UpdateMaxPower(Powers power);
        void UpdateAttackPowerAndDamage(bool ranged = false);
        void UpdateDamagePhysical(WeaponAttackType attType);
        */

        void ToggleAutocast(SpellInfo const* spellInfo, bool apply);

        bool HasSpell(uint32 spell) const;

        void LearnPetPassives();
        void CastPetAuras(bool current);
        void CastPetAura(PetAura const* aura);
        bool IsPetAura(Aura const* aura);

        void _LoadSpellCooldowns();
        void _SaveSpellCooldowns(SQLTransaction& trans);
        void _LoadAuras(uint32 timediff);
        void _SaveAuras(SQLTransaction& trans);
        void _LoadSpells();
        void _SaveSpells(SQLTransaction& trans);

        bool AddSpell(uint32 spellId, ActiveStates active = ACT_DECIDE, PetSpellState state = PETSPELL_NEW, PetSpellType type = PETSPELL_NORMAL);
        bool LearnSpell(uint32 spellId);
        void LearnSpells(std::list<uint32> learnSpells);
        void InitLevelupSpellsForLevel();
        void UnlearnSpells(std::list<uint32> unlearnSpells, bool clear_ab = true);
        bool UnlearnSpell(uint32 spell_id, bool learn_prev, bool clear_ab = true);
        bool RemoveSpell(uint32 spell_id, bool learn_prev, bool clear_ab = true);
        void CleanupActionBar();
        virtual void ProhibitSpellSchool(SpellSchoolMask idSchoolMask, uint32 unTimeMs);

        PetSpellMap     m_spells;
        AutoSpellList   m_autospells;

        void InitPetCreateSpells();

        uint64 GetAuraUpdateMaskForRaid() const { return _auraRaidUpdateMask; }
        void SetAuraUpdateMaskForRaid(uint8 slot) { _auraRaidUpdateMask |= (uint64(1) << slot); }
        void ResetAuraUpdateMaskForRaid() { _auraRaidUpdateMask = 0; }

        DeclinedName const* GetDeclinedNames() const { return _declinedname; }

        bool    m_removed;                                  // prevent overwrite pet state in DB at next Pet::Update if pet already removed(saved)

        Player* GetOwner() const;

    protected:
        PetType _petType;
        int32   _duration;                                 // time until unsummon (used mostly for summoned guardians and not used for controlled pets)
        uint64  _auraRaidUpdateMask;
        bool    _loading;
        uint32  _regenTimer;
        uint8   _specializationCount;
        uint8   _activeSpecialization;
        uint32  _specializations[MAX_TALENT_SPECS];

        DeclinedName *_declinedname;

    private:
        void SaveToDB(uint32, uint8, uint32)                // override of Creature::SaveToDB     - must not be called
        {
            ASSERT(false);
        }
        void DeleteFromDB()                                 // override of Creature::DeleteFromDB - must not be called
        {
            ASSERT(false);
        }
};
#endif
 