/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"
#include "InstanceScript.h"
#include "Player.h"

enum eShaOfFearSpells
{
    SPELL_REACHING_ATTACK           = 119775,
    SPELL_EERIE_SKULL               = 119519,
    SPELL_BREATH_OF_FEAR            = 119414,
    SPELL_THRASH_AURA               = 131996,
    SPELL_THRASH_EXTRA_ATTACKS      = 131994,
    SPELL_CONJURE_TERROR_SPAWN_TICK = 119108,
    SPELL_CONJURE_TERROR_SPAWN_01   = 119312,
    SPELL_CONJURE_TERROR_SPAWN_02   = 119370,
    SPELL_CONJURE_TERROR_SPAWN_03   = 119371,
    SPELL_CONJURE_TERROR_SPAWN_04   = 119372,
    SPELL_CUSTOM_ENERGY_REGEN       = 119417,
    SPELL_OMINOUS_CACKLE_MORPH      = 129147,

    SPELL_NAKED_AND_AFRAID          = 120669,
    SPELL_FADING_LIGHT              = 129378,
    SPELL_IMPLACABLE_STRIKE         = 120672,
    SPELL_DREAD_THRASH              = 132007,
    SPELL_SUBMERGE                  = 120455,
    SPELL_EMERGE                    = 120458,
    SPELL_EMERGE_STUN               = 120475,
    SPELL_HUDDLE_IN_TERROR          = 120629,
    SPELL_WATERSPOUT                = 120519,
    SPELL_WATERSPOUT_DAMAGE         = 120521,
    SPELL_WATERSPOUT_DUMMY          = 120556,
    SPELL_CHAMPION_OF_THE_LIGHT     = 120268,
    SPELL_TRANSFER_LIGHT            = 120285,
    SPELL_DREAD_EXPANSE             = 120289,
    SPELL_ETERNAL_DARKNESS          = 120394,
    SPELL_TELEPORT_SHA              = 120191,
    SPELL_TELEPORT_PLAYER           = 120192,
    SPELL_TELEPORT_TO_DREAD_EXPANSE = 120221,
    SPELL_PHASE2_ENERGY_REGEN       = 120284,
    SPELL_HEROIC_TRANSITION         = 114936,
    SPELL_HEROIC_TRANSITION_SCREEN  = 120193,
    SPELL_HC_CONJURE_DREAD_SPAWN    = 120453,
    SPELL_HC_TRASH_EXTRA_ATTACK     = 132000,
    SPELL_SHA_SPINE                 = 120437,
    SPELL_SHA_SPINE_EFF             = 120438,
    SPELL_GATHERING_SPEED           = 128419,

    SPELL_LIGHT_WALL                = 117865,
    SPELL_CHAMPION_OF_LIGHT         = 117866,
    SPELL_LIGHT_WALL_READY          = 117770,
    SPELL_LIGHT_WALL_VISUAL         = 107141,
    SPELL_FEARLESS                  = 118977,
    SPELL_WALL_OF_LIGHT_BUFF        = 117999,
    SPELL_PENETRATING_BOLT          = 129075,
    SPELL_PENETRATING_BOLT_MISSILE  = 129077,
    SPELL_DARK_BULWARK              = 119083
};

enum eShaOfFearEvents
{
    EVENT_CHECK_MELEE              = 1,
    EVENT_EERIE_SKULL              = 2,
    EVENT_CHECK_ENERGY             = 3,
    EVENT_FIRST_TERRORS            = 4,
    EVENT_PENETRATING_BOLT         = 5,
    EVENT_OMINOUS_CACKLE           = 6,
    EVENT_EVADE                    = 7,
    EVENT_BERSERK                  = 8,
    EVENT_SET_EVADE_TRUE           = 10,
    EVENT_HUDDLE_IN_TERROR         = 11,
    EVENT_WATERSPOUT               = 12,
    EVENT_NAKED_AND_AFRAID         = 13,
    EVENT_IMPLACABLE_STRIKE        = 14,
    EVENT_COSMETIC_BATTLE          = 15,
    EVENT_IMPLACABLE_STRIKE_REMOVE = 16,
    EVENT_DREAD_SPAWN              = 17,
    EVENT_SUBMERGE                 = 18
};

enum eShaOfFearActions
{
    ACTION_ACTIVATE_WALL_OF_LIGHT,
    ACTION_DESACTIVATE_WALL_OF_LIGHT,
    ACTION_SPAWN_TERROR,
    ACTION_BERSERK,
    ACTION_TRANSFER_LIGHT,
    ACTION_EMOTE,
    ACTION_SELECT_NEW_TARGET
};

enum eShaOfFearSays
{
    SAY_INTRO,
    SAY_AGGRO,
    SAY_BREATH_OF_FEAR,
    SAY_SLAY,
    SAY_PHASE_2,
    SAY_SUBMERGE,
    SAY_HUDDLE,
    EMOTE_PHASE_2
};

enum Entities
{
    NPC_YANG_GUOSHI = 61038,
    NPC_JINLUN_KUN  = 61046,
    NPC_CHENG_KANG  = 61042
};

enum Misc
{
    ACTION_BOWMAN_AGGRESSIVE = 10,
    ACTION_SUBMERGE_REMOVE   = 11
};

struct BowmanData
{
    uint32 entry;
    uint32 spellId;
    float x;
    float y;
    float z;
    float o;
    bool active;
};

static BowmanData bowmenData[] =
{
    { NPC_YANG_GUOSHI, 119593, -1214.795f, -2824.823f, 41.24303f, 3.506719f },
    { NPC_CHENG_KANG, 119693, -1075.198f, -2577.711f, 15.828019f, 1.725f },
    { NPC_JINLUN_KUN, 119692, -832.0764f, -2745.405f, 31.67757f, 0.1583484f }
};

Player* GetChampionOfLight(Creature* me)
{
    if (!me)
        return NULL;

    Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
    for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
        if (Player* player = itr->GetSource())
            if (player->HasAura(SPELL_CHAMPION_OF_LIGHT))
                return player;

    return NULL;
}

static Position const spawnTerrorPos[4] =
{
    { -1052.588f, -2788.164f, 38.268f, 0.0f },
    { -983.4550f, -2787.942f, 38.269f, 0.0f },
    { -989.6860f, -2772.245f, 38.303f, 0.0f },
    { -1046.274f, -2772.215f, 38.303f, 0.0f }
};

enum eShaPhases
{
    PHASE_TERRACE = 1,
    PHASE_DREAD_EXPANSE
};

class DpsSelectPredicate
{
public:
    bool operator()(WorldObject* target) const
    {
        Player* player = target ? target->ToPlayer() : NULL;
        if (!player)
            return false;

        switch (player->GetTalentSpecialization(player->GetActiveSpec()))
        {
            case SPEC_MAGE_ARCANE:
            case SPEC_MAGE_FIRE:
            case SPEC_MAGE_FROST:
            case SPEC_PALADIN_RETRIBUTION:
            case SPEC_WARRIOR_ARMS:
            case SPEC_WARRIOR_FURY:
            case SPEC_DRUID_BALANCE:
            case SPEC_DRUID_FERAL:
            case SPEC_DEATH_KNIGHT_FROST:
            case SPEC_DEATH_KNIGHT_UNHOLY:
            case SPEC_HUNTER_BEAST_MASTERY:
            case SPEC_HUNTER_MARKSMANSHIP:
            case SPEC_HUNTER_SURVIVAL:
            case SPEC_PRIEST_SHADOW:
            case SPEC_ROGUE_ASSASSINATION:
            case SPEC_ROGUE_COMBAT:
            case SPEC_ROGUE_SUBTLETY:
            case SPEC_SHAMAN_ELEMENTAL:
            case SPEC_SHAMAN_ENHANCEMENT:
            case SPEC_WARLOCK_AFFLICTION:
            case SPEC_WARLOCK_DEMONOLOGY:
            case SPEC_WARLOCK_DESTRUCTION:
            case SPEC_MONK_WINDWALKER:
                return false;
            default:
                return true;
        }
    }
};

class notValidTargetPredicate
{
public:
    explicit notValidTargetPredicate(Unit* caster) : caster(caster) { }

    bool operator()(WorldObject* target) const
    {
        Player* player = target ? target->ToPlayer() : NULL;
        return player && (player->HasAura(SPELL_CHAMPION_OF_LIGHT) ||
            player->GetExactDist2d(caster) > 70.0f);
    }

private:
    Unit* caster;
};

static Position const lightPos = { -1017.835f, -2771.984f, 38.65444f, 4.718282f };

uint8 const ominousCaclkePathLenght = 8;
Position const ominousCaclkePath[3][ominousCaclkePathLenght] =
{
    {
        { -1046.99f, -2783.15f, 61.8138f, 0.0f },
        { -1099.02f, -2793.58f, 54.7547f, 0.0f },
        { -1140.58f, -2803.74f, 50.0972f, 0.0f },
        { -1162.42f, -2840.47f, 62.152f, 0.0f },
        { -1205.59f, -2866.87f, 57.9883f, 0.0f },
        { -1236.89f, -2860.34f, 55.5584f, 0.0f },
        { -1249.57f, -2843.74f, 48.606f, 0.0f },
        { -1237.62f, -2833.73f, 41.2698f, 0.0f }
    },
    {
        { -1059.84f, -2777.74f, 69.5133f, 0.0f },
        { -1086.31f, -2715.18f, 57.1774f, 0.0f },
        { -1096.7f, -2666.51f, 43.1586f, 0.0f },
        { -1136.55f, -2603.74f, 42.5406f, 0.0f },
        { -1115.88f, -2560.9f, 31.2417f, 0.0f },
        { -1096.6f, -2541.23f, 25.3045f, 0.0f },
        { -1083.98f, -2538.09f, 22.3872f, 0.0f },
        { -1079.12f, -2553.06f, 15.8787f, 0.0f }
    },
    {
        { -991.503f, -2780.37f, 83.9225f, 0.0f },
        { -942.059f, -2752.11f, 72.1218f, 0.0f },
        { -902.356f, -2729.78f, 63.2266f, 0.0f },
        { -876.343f, -2719.93f, 59.0469f, 0.0f },
        { -821.145f, -2702.31f, 53.7641f, 0.0f },
        { -791.903f, -2717.04f, 47.9356f, 0.0f },
        { -795.439f, -2737.12f, 39.1009f, 0.0f },
        { -807.401f, -2741.74f, 31.7066f, 0.0f }
    }
};

class boss_sha_of_fear : public CreatureScript
{
public:
    boss_sha_of_fear() : CreatureScript("boss_sha_of_fear") { }

    struct boss_sha_of_fearAI : public BossAI
    {
        boss_sha_of_fearAI(Creature* creature) : BossAI(creature, DATA_SHA_OF_FEAR) { }

        EventMap m_mEvents, berserkEvents;

        uint8 attacksCounter;
        uint8 attackCounterDread;
        uint8 terrorCounter;
        uint8 m_uiPhase;

        uint32 delay;
        uint32 prevStep, curStep;
        uint64 VictimGUID;
        std::vector<uint32> m_dEvents;

        bool isDuringP2Transition;
        bool isSecondEvent;

        std::unordered_map<BowmanData*, uint32> pandaMap;

        void RemoveBloodlustDebuffs()
        {
            if (!instance)
                return;

            instance->DoRemoveAurasDueToSpellOnPlayers(95809);
            instance->DoRemoveAurasDueToSpellOnPlayers(80354);
            instance->DoRemoveAurasDueToSpellOnPlayers(57723);
            instance->DoRemoveAurasDueToSpellOnPlayers(57724);
        }

        void InitializeAI() OVERRIDE
        {
            me->setActive(true);
            SetCombatMovement(false);
            introDone = false;
            isDuringP2Transition = false;
            delay = 0;
            prevStep = 0;
            curStep = 1;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);

            for (int i = 0; i < 3; ++i)
            {
                BowmanData* data = new BowmanData();
                data->entry = bowmenData[i].entry;
                data->spellId = bowmenData[i].spellId;
                data->x = bowmenData[i].x;
                data->y = bowmenData[i].y;
                data->z = bowmenData[i].z;
                data->o = bowmenData[i].o;
                data->active = false;

                pandaMap.insert(std::make_pair(data, data->entry));
            }

            if (instance && instance->GetBossState(DATA_PROTECTORS) < DONE)
                m_mEvents.ScheduleEvent(EVENT_COSMETIC_BATTLE, 1500);

            me->GetMap()->SetWorldState(WORLDSTATE_MIND_KILLER, 1);
        }

        void Reset() OVERRIDE
        {
            _Reset();
            SetCombatMovement(false);

            me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 32.0f);
            me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 32.0f);

            if (isDuringP2Transition)
                return;

            isDuringP2Transition = false;
            me->setRegeneratingHealth(true);

            me->AddAura(72242, me);
            me->SetReactState(REACT_DEFENSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

            summons.DespawnAll();

            SetPhase(0);

            prevStep = 0;
            curStep = 1;
            delay = 0;
            attacksCounter = 0;
            attackCounterDread = 0;
            terrorCounter = 0;
            VictimGUID = 0;
            m_dEvents.clear();
            berserkEvents.Reset();

            for (std::unordered_map<BowmanData*, uint32>::iterator itr = pandaMap.begin(); itr != pandaMap.end(); ++itr)
                itr->first->active = false;

            isSecondEvent = false;

            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                RemoveBloodlustDebuffs();
            }

            me->RemoveAurasDueToSpell(SPELL_BERSERK);
            me->GetMap()->SetWorldState(WORLDSTATE_MIND_KILLER, 1);
        }

        uint32 GetData(uint32 type) const OVERRIDE
        {
            if (type == TYPE_DREAD_TERRACE)
                return m_uiPhase == PHASE_DREAD_EXPANSE ? 1 : 0;

            return 0;
        }

        void SetPhase(uint8 m_phase)
        {
            events.Reset();

            switch (m_phase)
            {
                case PHASE_TERRACE:
                    events.ScheduleEvent(EVENT_CHECK_MELEE, 1000);
                    events.ScheduleEvent(EVENT_EERIE_SKULL, 5000);
                    events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                    events.ScheduleEvent(EVENT_FIRST_TERRORS, 30000);
                    events.ScheduleEvent(EVENT_OMINOUS_CACKLE, 35000);
                    break;
                case PHASE_DREAD_EXPANSE:
                    summons.DespawnAll();
                    events.CancelEvent(EVENT_OMINOUS_CACKLE);
                    events.CancelEvent(EVENT_EERIE_SKULL);
                    berserkEvents.CancelEvent(EVENT_EVADE);
                    me->RemoveAurasDueToSpell(SPELL_THRASH_AURA);
                    me->setRegeneratingHealth(false);

                    if (Unit* vict = me->GetVictim())
                        VictimGUID = vict->GetGUID();

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, [](Unit* candidate)
                    {
                        Player* player = candidate ? candidate->ToPlayer() : NULL;
                        if (!player || player->IsGameMaster())
                            return false;

                        switch (player->GetTalentSpecialization(player->GetActiveSpec()))
                        {
                            case SPEC_PALADIN_HOLY:
                            case SPEC_PRIEST_DISCIPLINE:
                            case SPEC_PRIEST_HOLY:
                            case SPEC_DRUID_RESTORATION:
                            case SPEC_SHAMAN_RESTORATION:
                            case SPEC_MONK_MISTWEAVER:
                                return true;
                            default:
                                return false;
                        }
                    }))
                        target->CastSpell(target, SPELL_CHAMPION_OF_THE_LIGHT, true);
                    else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, [](Unit* candidate)
                    {
                        Player* player = candidate ? candidate->ToPlayer() : NULL;
                        if (!player || player->IsGameMaster())
                            return false;

                        switch (player->GetTalentSpecialization(player->GetActiveSpec()))
                        {
                            case SPEC_MONK_BREWMASTER:
                            case SPEC_DEATH_KNIGHT_BLOOD:
                            case SPEC_WARRIOR_PROTECTION:
                            case SPEC_DRUID_GUARDIAN:
                            case SPEC_PALADIN_PROTECTION:
                                return false;
                            default:
                                return true;
                        }
                    }))
                        target->CastSpell(target, SPELL_CHAMPION_OF_THE_LIGHT, true);
                    else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                        target->CastSpell(target, SPELL_CHAMPION_OF_THE_LIGHT, true);

                    if (instance)
                    {
                        RemoveBloodlustDebuffs();
                        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT);
                        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OMINOUS_CACKLE_MORPH);
                        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WALL_OF_LIGHT_BUFF);
                    }

                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    SetCombatMovement(true);
                    Talk(SAY_PHASE_2);
                    Talk(EMOTE_PHASE_2);
                    me->PrepareChanneledCast(me->GetOrientation());
                    DoCast(me, SPELL_FADING_LIGHT, true);
                    DoCast(me, SPELL_HEROIC_TRANSITION, true);
                    DoCast(me, SPELL_HEROIC_TRANSITION_SCREEN, true);
                    DoCast(me, SPELL_TELEPORT_SHA, true);

                    me->RemoveAurasDueToSpell(SPELL_CUSTOM_ENERGY_REGEN);

                    m_dEvents.clear();
                    m_dEvents = { EVENT_HUDDLE_IN_TERROR, EVENT_WATERSPOUT, EVENT_IMPLACABLE_STRIKE };
                    SetupPhaseTwoEvents();

                    events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                    isDuringP2Transition = true;

                    delay = 0;
                    me->m_Events.Schedule(delay += 5000, 20, [this]()
                    {
                        isDuringP2Transition = false;
                        me->UpdateObjectVisibility();
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        SetCombatMovement(true);
                        me->RemoveChanneledCast(VictimGUID);
                        DoZoneInCombat(me, 400.0f);

                        me->SetPower(POWER_ENERGY, 50);
                        DoCast(me, SPELL_PHASE2_ENERGY_REGEN, true);
                    });
                    break;
            }

            m_uiPhase = m_phase;
        }

        void GetAvailablePandaAndSummon()
        {
            std::unordered_map<BowmanData*, uint32> newPandaMap;
            newPandaMap.reserve(sizeof(pandaMap));
            std::copy(pandaMap.begin(), pandaMap.end(), std::inserter(newPandaMap, newPandaMap.begin()));
            std::unordered_map<BowmanData*, uint32>::iterator itr = newPandaMap.begin();

            for (; itr != newPandaMap.end();)
            {
                if (itr->first->active)
                    itr = newPandaMap.erase(itr);
                else
                    ++itr;
            }

            itr = newPandaMap.begin();

            if (!newPandaMap.empty())
            {
                if (newPandaMap.size() > 1)
                    std::advance(itr, std::rand() % (newPandaMap.size() - 1));

                if (itr != newPandaMap.cend())
                {
                    for (std::unordered_map<BowmanData*, uint32>::const_iterator finder = pandaMap.begin(); finder != pandaMap.end(); ++finder)
                    {
                        if (finder->first->entry == itr->second)
                        {
                            PropagateSummon(finder);
                            return;
                        }
                    }
                }
            }

            itr = pandaMap.begin();
            std::advance(itr, std::rand() % (pandaMap.size() - 1));

            if (itr != pandaMap.cend())
                DoCast(me, itr->first->spellId);
        }

        void PropagateSummon(std::unordered_map<BowmanData*, uint32>::const_iterator itr)
        {
            if (Creature* bowman = me->SummonCreature(itr->first->entry, itr->first->x, itr->first->y, itr->first->z, itr->first->o))
            {
                DoCast(me, itr->first->spellId);
                itr->first->active = true;

                if (bowman->AI())
                    bowman->AI()->DoAction(ACTION_BOWMAN_AGGRESSIVE);
            }
        }

        void JustReachedHome() OVERRIDE
        {
            _JustReachedHome();

            if (instance)
                instance->SetBossState(DATA_SHA_OF_FEAR, FAIL);
        }

        void SetupPhaseTwoEvents()
        {
            if (m_dEvents.empty())
                return;

            if (uint32 nextTypeEvent = Skyfire::Containers::SelectRandomContainerElement(m_dEvents))
            {
                m_dEvents.erase(std::find(m_dEvents.begin(), m_dEvents.end(), nextTypeEvent));
                events.ScheduleEvent(nextTypeEvent, 10 * IN_MILLISECONDS);
            }
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            if (isDuringP2Transition)
                return;

            me->SetPower(POWER_ENERGY, 0);

            DoCast(me, SPELL_CUSTOM_ENERGY_REGEN, true);

            if (instance)
            {
                instance->SetBossState(DATA_SHA_OF_FEAR, IN_PROGRESS);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
                Talk(SAY_AGGRO);
            }

            SetPhase(PHASE_TERRACE);
            berserkEvents.ScheduleEvent(EVENT_BERSERK, 15 * MINUTE * IN_MILLISECONDS);
            berserkEvents.ScheduleEvent(EVENT_EVADE, 1 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
            {
                summons.DespawnAll();
                instance->SetBossState(DATA_SHA_OF_FEAR, DONE);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                RemoveBloodlustDebuffs();
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WALL_OF_LIGHT_BUFF);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_THE_LIGHT);
                _JustDied();
            }
        }

        void EnterEvadeMode() OVERRIDE
        {
            summons.DespawnAll();

            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                RemoveBloodlustDebuffs();
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WALL_OF_LIGHT_BUFF);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_THE_LIGHT);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DREAD_EXPANSE);
                instance->SetBossState(DATA_SHA_OF_FEAR, FAIL);
            }

            _EnterEvadeMode();
            Reset();

            me->RemoveAurasDueToSpell(SPELL_BERSERK);
            me->SetReactState(REACT_DEFENSIVE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            me->setRegeneratingHealth(true);

            if (me->HasUnitState(UNIT_STATE_CANNOT_TURN))
                me->ClearUnitState(UNIT_STATE_CANNOT_TURN);

            if (instance)
            {
                Map::PlayerList const& players = instance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* player = itr->GetSource())
                        if (player->IsAlive() && !player->IsGameMaster())
                            player->Kill(player, false);
                }
            }

            if (m_uiPhase == PHASE_DREAD_EXPANSE)
            {
                float x, y, z, o;
                me->GetHomePosition(x, y, z, o);
                me->NearTeleportTo(x, y, z, o);
                me->UpdateObjectVisibility();
            }

            _DespawnAtEvade();
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            if (summon->GetEntry() == NPC_TERROR_SPAWN)
            {
                summon->SetInCombatWithZone();
                ++terrorCounter;
            }

            summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon) OVERRIDE
        {
            if (summon->GetEntry() == NPC_TERROR_SPAWN)
                --terrorCounter;

            summons.Despawn(summon);
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) OVERRIDE
        {
            switch (summon->GetEntry())
            {
                case NPC_YANG_GUOSHI:
                case NPC_CHENG_KANG:
                case NPC_JINLUN_KUN:
                    for (std::unordered_map<BowmanData*, uint32>::iterator itr = pandaMap.begin(); itr != pandaMap.end(); ++itr)
                        if (itr->first->entry == summon->GetEntry())
                            itr->first->active = false;
                    break;
                default:
                    return;
            }
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(SAY_SLAY);
        }

        void DoAction(int32 actionId) OVERRIDE
        {
            switch (actionId)
            {
                case ACTION_SPAWN_TERROR:
                {
                    if (!terrorCounter)
                    {
                        me->CastSpell(spawnTerrorPos[0].GetPositionX(), spawnTerrorPos[0].GetPositionY(),
                            spawnTerrorPos[0].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_01, true);
                        me->CastSpell(spawnTerrorPos[1].GetPositionX(), spawnTerrorPos[1].GetPositionY(),
                            spawnTerrorPos[1].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_02, true);
                    }
                    else
                    {
                        me->CastSpell(spawnTerrorPos[2].GetPositionX(), spawnTerrorPos[2].GetPositionY(),
                            spawnTerrorPos[2].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_03, true);
                        me->CastSpell(spawnTerrorPos[3].GetPositionX(), spawnTerrorPos[3].GetPositionY(),
                            spawnTerrorPos[3].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_04, true);
                    }
                    break;
                }
                case ACTION_SHA_INTRO:
                    Talk(SAY_INTRO);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);

                    if (instance)
                    {
                        if (GameObject* vortex = ObjectAccessor::GetGameObject(*me, instance->GetData64(GO_SHA_VORTEX)))
                            vortex->SetGoState(GOState::GO_STATE_ACTIVE);

                        if (GameObject* wall = ObjectAccessor::GetGameObject(*me, instance->GetData64(GO_SHA_VORTEX_WALL)))
                            wall->SetGoState(GOState::GO_STATE_ACTIVE);
                    }
                    break;
                case ACTION_BERSERK:
                    DoCast(me, SPELL_BERSERK, true);
                    break;
                case ACTION_SUBMERGE_REMOVE:
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                    me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                    me->RemoveChanneledCast(VictimGUID);
                    break;
                case ACTION_TRANSFER_LIGHT:
                    if (instance && instance->GetBossState(DATA_SHA_OF_FEAR) != IN_PROGRESS)
                        break;

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, [](Unit* candidate)
                    {
                        Player* player = candidate ? candidate->ToPlayer() : NULL;
                        if (!player || player->IsGameMaster())
                            return false;

                        switch (player->GetTalentSpecialization(player->GetActiveSpec()))
                        {
                            case SPEC_PALADIN_HOLY:
                            case SPEC_PRIEST_DISCIPLINE:
                            case SPEC_PRIEST_HOLY:
                            case SPEC_DRUID_RESTORATION:
                            case SPEC_SHAMAN_RESTORATION:
                            case SPEC_MONK_MISTWEAVER:
                                return true;
                            default:
                                return false;
                        }
                    }))
                        target->CastSpell(target, SPELL_CHAMPION_OF_THE_LIGHT, true);
                    else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, [](Unit* candidate)
                    {
                        Player* player = candidate ? candidate->ToPlayer() : NULL;
                        if (!player || player->IsGameMaster())
                            return false;

                        switch (player->GetTalentSpecialization(player->GetActiveSpec()))
                        {
                            case SPEC_MONK_BREWMASTER:
                            case SPEC_DEATH_KNIGHT_BLOOD:
                            case SPEC_WARRIOR_PROTECTION:
                            case SPEC_DRUID_GUARDIAN:
                            case SPEC_PALADIN_PROTECTION:
                                return false;
                            default:
                                return true;
                        }
                    }))
                        target->CastSpell(target, SPELL_CHAMPION_OF_THE_LIGHT, true);
                    else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                        target->CastSpell(target, SPELL_CHAMPION_OF_THE_LIGHT, true);
                    break;
                default:
                    break;
            }
        }

        void DamageDealt(Unit* /*victim*/, uint32& /*damage*/, DamageEffectType damageType) OVERRIDE
        {
            if (damageType == DIRECT_DAMAGE)
            {
                if (m_uiPhase == PHASE_TERRACE)
                {
                    if (attacksCounter >= 3 && !me->m_extraAttacks)
                    {
                        me->CastSpell(me, SPELL_THRASH_EXTRA_ATTACKS, true);
                        attacksCounter = 0;
                    }
                    else if (attacksCounter >= 2 && !me->m_extraAttacks)
                    {
                        me->CastSpell(me, SPELL_THRASH_AURA, true);
                        ++attacksCounter;
                    }
                    else if (!me->m_extraAttacks)
                    {
                        me->RemoveAura(SPELL_THRASH_AURA);
                        ++attacksCounter;
                    }
                }
                else
                {
                    if (attacksCounter >= 3 && !me->m_extraAttacks)
                    {
                        me->CastSpell(me, attackCounterDread >= 4 ? SPELL_HC_TRASH_EXTRA_ATTACK : SPELL_THRASH_EXTRA_ATTACKS, true);
                        attacksCounter = 0;

                        if (attackCounterDread >= 4)
                            attackCounterDread = 0;
                    }
                    else if (attacksCounter >= 2 && !me->m_extraAttacks)
                    {
                        ++attackCounterDread;
                        me->CastSpell(me, attackCounterDread >= 4 ? SPELL_DREAD_THRASH : SPELL_THRASH_AURA, true);
                        ++attacksCounter;
                    }
                    else if (!me->m_extraAttacks)
                    {
                        me->RemoveAura(SPELL_THRASH_AURA);
                        me->RemoveAura(SPELL_DREAD_THRASH);
                        ++attacksCounter;
                    }
                }
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (me->HealthBelowPctDamaged(66, damage) && m_uiPhase == PHASE_TERRACE)
            {
                if (IsHeroic())
                {
                    if (instance && instance->GetData(TYPE_LEIS_HOPE) != DONE)
                        instance->SetData(TYPE_LEIS_HOPE, DONE);

                    SetPhase(PHASE_DREAD_EXPANSE);
                }
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            m_mEvents.Update(diff);

            while (uint32 eventId = m_mEvents.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_EVADE:
                        me->SetVisible(true);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        break;
                    case EVENT_COSMETIC_BATTLE:
                        if (instance && instance->GetBossState(DATA_PROTECTORS) < DONE)
                        {
                            me->HandleEmoteCommand(urand(35, 37));
                            m_mEvents.ScheduleEvent(EVENT_COSMETIC_BATTLE, urand(1500, 2000));
                        }
                        break;
                    default:
                        break;
                }
            }

            if (isDuringP2Transition)
                return;

            if (!UpdateVictim())
                return;

            berserkEvents.Update(diff);

            while (uint32 eventId = berserkEvents.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BERSERK:
                        DoAction(ACTION_BERSERK);
                        break;
                    case EVENT_EVADE:
                        if (instance && instance->IsWipe(70.0f, me))
                            EnterEvadeMode();

                        berserkEvents.ScheduleEvent(EVENT_EVADE, 1 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHECK_ENERGY:
                        if (!instance)
                        {
                            events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                            break;
                        }

                        if (me->GetPower(POWER_ENERGY) >= me->GetMaxPower(POWER_ENERGY))
                        {
                            if (m_uiPhase == PHASE_TERRACE)
                            {
                                if (Creature* pureLight = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_PURE_LIGHT_TERRACE)))
                                    me->CastSpell(pureLight, SPELL_BREATH_OF_FEAR, false);
                                Talk(SAY_BREATH_OF_FEAR);
                            }
                            else
                            {
                                Talk(SAY_SUBMERGE);
                                if (Unit* victim = me->GetVictim())
                                {
                                    me->PrepareChanneledCast(me->GetAngle(victim));
                                    VictimGUID = victim->GetGUID();
                                }

                                DoCast(SPELL_SUBMERGE);
                                m_dEvents.clear();
                                m_dEvents = { EVENT_HUDDLE_IN_TERROR, EVENT_WATERSPOUT, EVENT_IMPLACABLE_STRIKE };
                                SetupPhaseTwoEvents();
                                events.RescheduleEvent(EVENT_NAKED_AND_AFRAID, 8 * IN_MILLISECONDS);
                                events.ScheduleEvent(EVENT_DREAD_SPAWN, 6 * IN_MILLISECONDS);
                                events.ScheduleEvent(EVENT_SUBMERGE, 2 * IN_MILLISECONDS);
                            }
                            me->SetPower(POWER_ENERGY, 0);
                            me->SetInt32Value(UNIT_FIELD_POWER, 0);
                        }

                        events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                        break;
                    case EVENT_CHECK_MELEE:
                        if (Player* target = GetChampionOfLight(me))
                        {
                            if (me->GetVictim() && me->GetVictim()->GetGUID() != target->GetGUID())
                            {
                                DoResetThreat();
                                AttackStart(target);
                                me->AddThreat(target, 5000000.0f);
                            }
                        }

                        if (!me->IsWithinMeleeRange(me->GetVictim(), 2.0f))
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f))
                                me->CastSpell(target, SPELL_REACHING_ATTACK, false);
                        }
                        events.ScheduleEvent(EVENT_CHECK_MELEE, 1000);
                        break;
                    case EVENT_EERIE_SKULL:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 80.0f, true))
                            DoCast(target, SPELL_EERIE_SKULL, false);
                        events.ScheduleEvent(EVENT_EERIE_SKULL, 5000);
                        break;
                    case EVENT_FIRST_TERRORS:
                        me->CastSpell(me, SPELL_CONJURE_TERROR_SPAWN_TICK, true);
                        break;
                    case EVENT_OMINOUS_CACKLE:
                        GetAvailablePandaAndSummon();
                        events.ScheduleEvent(EVENT_OMINOUS_CACKLE, Is25ManRaid() ? 45 * IN_MILLISECONDS : 90 * IN_MILLISECONDS);
                        break;
                    case EVENT_WATERSPOUT:
                        DoCast(me, SPELL_WATERSPOUT);
                        SetupPhaseTwoEvents();
                        break;
                    case EVENT_SUBMERGE:
                        me->CastSpell(me, SPELL_SUBMERGE, true);
                        break;
                    case EVENT_IMPLACABLE_STRIKE:
                        if (Unit* victim = me->GetVictim())
                        {
                            VictimGUID = victim->GetGUID();
                            me->PrepareChanneledCast(me->GetAngle(victim), SPELL_IMPLACABLE_STRIKE);
                        }
                        events.ScheduleEvent(EVENT_IMPLACABLE_STRIKE_REMOVE, 4 * IN_MILLISECONDS);
                        SetupPhaseTwoEvents();
                        break;
                    case EVENT_IMPLACABLE_STRIKE_REMOVE:
                        me->RemoveChanneledCast(VictimGUID);
                        break;
                    case EVENT_NAKED_AND_AFRAID:
                        if (Unit* victim = me->GetVictim())
                            me->CastSpell(victim, SPELL_NAKED_AND_AFRAID, false);

                        events.ScheduleEvent(EVENT_NAKED_AND_AFRAID, 31 * IN_MILLISECONDS);
                        break;
                    case EVENT_HUDDLE_IN_TERROR:
                        Talk(SAY_HUDDLE);
                        DoCast(SPELL_HUDDLE_IN_TERROR);
                        SetupPhaseTwoEvents();
                        break;
                    case EVENT_DREAD_SPAWN:
                    {
                        uint32 spawnCount = GetDreadSpawnSummonCount();

                        for (uint32 i = 0; i < spawnCount; ++i)
                        {
                            float orientation = Position::NormalizeOrientation(frand(0, 2 * M_PI));
                            me->CastSpell(DreadCenter.GetPositionX() + (150.0f * cos(orientation)),
                                DreadCenter.GetPositionY() + (150.0f * sin(orientation)),
                                me->GetPositionZ(), SPELL_HC_CONJURE_DREAD_SPAWN, true);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        bool introDone;

        uint32 GetDreadSpawnSummonCount()
        {
            if (curStep - prevStep == 0)
                ++curStep;
            else
                ++prevStep;

            return curStep;
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_sha_of_fearAI(creature);
    }
};

class npc_pure_light_terrace : public CreatureScript
{
public:
    npc_pure_light_terrace() : CreatureScript("npc_pure_light_terrace") { }

    struct npc_pure_light_terraceAI : public ScriptedAI
    {
        npc_pure_light_terraceAI(Creature* creature) : ScriptedAI(creature),
            instance(creature->GetInstanceScript()), wallActivated(false), lightTimer(0) { }

        InstanceScript* instance;
        bool wallActivated;
        uint32 lightTimer;

        void Reset() OVERRIDE
        {
            lightTimer = 200;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            me->CastSpell(me, SPELL_LIGHT_WALL, true);
            me->CastSpell(me, SPELL_LIGHT_WALL_READY, true);
        }

        void DoAction(int32 actionId) OVERRIDE
        {
            switch (actionId)
            {
                case ACTION_ACTIVATE_WALL_OF_LIGHT:
                {
                    if (wallActivated)
                        break;

                    std::list<Creature*> wallBunnies;
                    GetCreatureListWithEntryInGrid(wallBunnies, me, NPC_GENERAL_BUNNY_JMF, 60.0f);
                    for (Creature* bunny : wallBunnies)
                        bunny->CastSpell(bunny, SPELL_LIGHT_WALL_VISUAL, true);

                    wallActivated = true;
                    me->RemoveAura(SPELL_LIGHT_WALL_READY);
                    break;
                }
                case ACTION_DESACTIVATE_WALL_OF_LIGHT:
                {
                    if (!wallActivated)
                        break;

                    wallActivated = false;
                    me->CastSpell(me, SPELL_LIGHT_WALL_READY, true);

                    std::list<Creature*> wallBunnies;
                    GetCreatureListWithEntryInGrid(wallBunnies, me, NPC_GENERAL_BUNNY_JMF, 60.0f);
                    for (Creature* bunny : wallBunnies)
                        bunny->RemoveAurasDueToSpell(SPELL_LIGHT_WALL_VISUAL);
                    break;
                }
                default:
                    break;
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            damage = 0;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (lightTimer >= diff)
            {
                lightTimer -= diff;
                return;
            }

            lightTimer = 100;

            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (instance && instance->GetBossState(DATA_SHA_OF_FEAR) != IN_PROGRESS)
                    break;

                if (Player* player = itr->GetSource())
                {
                    if (lightPos.HasInArc(M_PI / 3, player) && wallActivated)
                    {
                        if (!player->HasAura(SPELL_WALL_OF_LIGHT_BUFF) &&
                            !player->HasAura(SPELL_CHAMPION_OF_LIGHT))
                            player->CastSpell(player, SPELL_WALL_OF_LIGHT_BUFF, true);
                    }
                    else
                        player->RemoveAura(SPELL_WALL_OF_LIGHT_BUFF);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_pure_light_terraceAI(creature);
    }
};

class npc_terror_spawn : public CreatureScript
{
public:
    npc_terror_spawn() : CreatureScript("npc_terror_spawn") { }

    struct npc_terror_spawnAI : public ScriptedAI
    {
        npc_terror_spawnAI(Creature* creature) : ScriptedAI(creature),
            instance(creature->GetInstanceScript()) { }

        InstanceScript* instance;
        EventMap events;

        void Reset() OVERRIDE
        {
            if (instance)
                if (Creature* pureLight = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_PURE_LIGHT_TERRACE)))
                    me->SetFacingToObject(pureLight);

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_DISABLE_TURN);
            me->CastSpell(me, SPELL_DARK_BULWARK, true);

            events.Reset();
            events.ScheduleEvent(EVENT_PENETRATING_BOLT, 5000);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_PENETRATING_BOLT:
                    me->CastSpell(me, SPELL_PENETRATING_BOLT, false);
                    events.ScheduleEvent(EVENT_PENETRATING_BOLT, 5000);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_terror_spawnAI(creature);
    }
};

class npc_dread_spawn : public CreatureScript
{
public:
    npc_dread_spawn() : CreatureScript("npc_dread_spawn") { }

    enum Events
    {
        EVENT_MOVE = 1,
        EVENT_SPEED = 2
    };

    struct npc_dread_spawnAI : public ScriptedAI
    {
        npc_dread_spawnAI(Creature* creature) : ScriptedAI(creature),
            instance(NULL), InPrepare(false)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
        }

        InstanceScript* instance;
        EventMap events;
        bool InPrepare;

        uint64 LightTargetGUID()
        {
            std::list<Player*> players;
            GetPlayerListInGrid(players, me, 300.0f);

            for (Player* player : players)
                if (player->HasAura(SPELL_CHAMPION_OF_THE_LIGHT))
                    return player->GetGUID();

            return 0;
        }

        void FixateOnLight(Unit* target)
        {
            me->getThreatManager().resetAllAggro();
            me->AddThreat(target, std::numeric_limits<float>::max());
            AttackStart(target);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_THREAT, true);
        }

        void Reset() OVERRIDE
        {
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);

            InPrepare = false;
            instance = me->GetInstanceScript();

            if (Unit* light = ObjectAccessor::GetUnit(*me, LightTargetGUID()))
                FixateOnLight(light);

            events.ScheduleEvent(EVENT_MOVE, 1 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_SPEED, 8 * IN_MILLISECONDS);
        }

        void DoAction(int32 actionId) OVERRIDE
        {
            switch (actionId)
            {
                case ACTION_EMOTE:
                    InPrepare = true;
                    me->InterruptNonMeleeSpells(true, SPELL_ETERNAL_DARKNESS);
                    DoCast(me, SPELL_SHA_SPINE_EFF, true);
                    me->PrepareChanneledCast(me->GetOrientation());
                    me->RemoveAurasDueToSpell(SPELL_GATHERING_SPEED);
                    events.RescheduleEvent(EVENT_SPEED, 8 * IN_MILLISECONDS);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_BATTLE_ROAR);
                    break;
                case ACTION_SELECT_NEW_TARGET:
                    me->SetReactState(REACT_AGGRESSIVE);
                    InPrepare = false;
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOVE:
                        if (!InPrepare)
                        {
                            if (Unit* light = ObjectAccessor::GetUnit(*me, LightTargetGUID()))
                            {
                                FixateOnLight(light);

                                if (me->GetExactDist2d(light) < 4.0f)
                                    DoCast(light, SPELL_ETERNAL_DARKNESS);
                            }
                        }

                        events.ScheduleEvent(EVENT_MOVE, 1 * IN_MILLISECONDS);
                        break;
                    case EVENT_SPEED:
                        me->CastSpell(me, SPELL_GATHERING_SPEED, false);
                        events.ScheduleEvent(EVENT_SPEED, 8 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_dread_spawnAI(creature);
    }
};

class spell_champion_of_light : public SpellScriptLoader
{
public:
    spell_champion_of_light() : SpellScriptLoader("spell_champion_of_light") { }

    class spell_champion_of_light_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_champion_of_light_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                if (Creature* creature = caster->ToCreature())
                    if (creature->AI())
                        creature->AI()->DoAction(ACTION_ACTIVATE_WALL_OF_LIGHT);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                if (Creature* creature = caster->ToCreature())
                    if (creature->AI())
                        creature->AI()->DoAction(ACTION_DESACTIVATE_WALL_OF_LIGHT);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_champion_of_light_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_champion_of_light_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    class spell_champion_of_light_Spellcript : public SpellScript
    {
        PrepareSpellScript(spell_champion_of_light_Spellcript);

        void SelectChampion(std::list<WorldObject*>& targets)
        {
            for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end();)
            {
                Player* target = (*itr)->ToPlayer();
                if (!target)
                {
                    itr = targets.erase(itr);
                    continue;
                }

                if (!target->IsWithinDist2d(&lightPos, 3.0f))
                {
                    if (target->HasAura(SPELL_CHAMPION_OF_LIGHT))
                        target->RemoveAurasDueToSpell(SPELL_CHAMPION_OF_LIGHT);

                    itr = targets.erase(itr);
                }
                else if (target->HasAura(SPELL_CHAMPION_OF_LIGHT))
                {
                    targets.clear();
                    return;
                }
                else
                    ++itr;
            }

            if (!targets.empty())
                Skyfire::Containers::RandomResizeList(targets, 1);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_champion_of_light_Spellcript::SelectChampion, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_champion_of_light_Spellcript();
    }

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_champion_of_light_AuraScript();
    }
};

class spell_breath_of_fear : public SpellScriptLoader
{
public:
    spell_breath_of_fear() : SpellScriptLoader("spell_breath_of_fear") { }

    class spell_breath_of_fear_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_breath_of_fear_AuraScript);

        void OnTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
                if (!GetUnitOwner()->IsWithinDist2d(caster, 60.0f) ||
                    GetUnitOwner()->HasAura(SPELL_WALL_OF_LIGHT_BUFF) ||
                    GetUnitOwner()->HasAura(SPELL_CHAMPION_OF_LIGHT))
                    PreventDefaultAction();
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_breath_of_fear_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_breath_of_fear_AuraScript();
    }
};

class spell_breath_of_fear_fear : public SpellScriptLoader
{
public:
    spell_breath_of_fear_fear() : SpellScriptLoader("spell_breath_of_fear_fear") { }

    class spell_breath_of_fear_fear_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_breath_of_fear_fear_SpellScript);

        std::list<WorldObject*> m_targets;

        void CorrectRange(std::list<WorldObject*>& targets)
        {
            targets.clear();

            Map::PlayerList const& players = GetCaster()->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (Player* player = itr->GetSource())
                    if (player->IsWithinDist2d(GetCaster(), 60.0f) &&
                        !player->HasAura(SPELL_WALL_OF_LIGHT_BUFF) &&
                        !player->HasAura(SPELL_CHAMPION_OF_LIGHT) &&
                        !player->HasAura(SPELL_OMINOUS_CACKLE_MORPH))
                        targets.push_back(player);

            m_targets = targets;
        }

        void CopyTargets(std::list<WorldObject*>& targets)
        {
            targets = m_targets;
        }

        void HandleFearEff(SpellEffIndex /*effIndex*/)
        {
            if (Player* target = GetHitPlayer())
                target->GetMap()->SetWorldState(WORLDSTATE_MIND_KILLER, 0);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_breath_of_fear_fear_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_breath_of_fear_fear_SpellScript::CopyTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_breath_of_fear_fear_SpellScript::CopyTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_breath_of_fear_fear_SpellScript::HandleFearEff, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_breath_of_fear_fear_SpellScript();
    }
};

class spell_conjure_terror_spawn : public SpellScriptLoader
{
public:
    spell_conjure_terror_spawn() : SpellScriptLoader("spell_conjure_terror_spawn") { }

    class spell_conjure_terror_spawn_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_conjure_terror_spawn_AuraScript);

        void OnTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
                if (Creature* creature = caster->ToCreature())
                    creature->AI()->DoAction(ACTION_SPAWN_TERROR);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_conjure_terror_spawn_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_conjure_terror_spawn_AuraScript();
    }
};

class spell_penetrating_bolt : public SpellScriptLoader
{
public:
    spell_penetrating_bolt() : SpellScriptLoader("spell_penetrating_bolt") { }

    class spell_penetrating_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_penetrating_bolt_SpellScript);

        uint64 targetGuid;

        void CorrectRange(std::list<WorldObject*>& targets)
        {
            targetGuid = 0;

            if (!targets.empty())
                Skyfire::Containers::RandomResizeList(targets, 1);

            for (WorldObject* target : targets)
                if (target->GetGUID())
                    targetGuid = target->GetGUID();
        }

        void HandleDummy(SpellEffIndex /*index*/)
        {
            if (Unit* caster = GetCaster())
                if (InstanceScript* instance = caster->GetInstanceScript())
                    if (Player* target = ObjectAccessor::GetPlayer(*caster, targetGuid))
                        caster->CastSpell(target, SPELL_PENETRATING_BOLT_MISSILE, true, NULL, NULL, instance->GetData64(NPC_SHA_OF_FEAR));
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_penetrating_bolt_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectLaunch += SpellEffectFn(spell_penetrating_bolt_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_penetrating_bolt_SpellScript();
    }
};

class spell_ominous_caclke_target : public SpellScript
{
    PrepareSpellScript(spell_ominous_caclke_target);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        std::list<WorldObject*> tanks;
        std::list<WorldObject*> healers;

        if (Unit* caster = GetCaster())
        {
            targets.remove_if(notValidTargetPredicate(caster));

            for (WorldObject* target : targets)
            {
                if (Player* player = target->ToPlayer())
                {
                    switch (player->GetTalentSpecialization(player->GetActiveSpec()))
                    {
                        case SPEC_MONK_BREWMASTER:
                        case SPEC_DEATH_KNIGHT_BLOOD:
                        case SPEC_WARRIOR_PROTECTION:
                        case SPEC_DRUID_GUARDIAN:
                        case SPEC_PALADIN_PROTECTION:
                            tanks.push_back(player);
                            break;
                        case SPEC_PALADIN_HOLY:
                        case SPEC_DRUID_RESTORATION:
                        case SPEC_PRIEST_DISCIPLINE:
                        case SPEC_PRIEST_HOLY:
                        case SPEC_SHAMAN_RESTORATION:
                        case SPEC_MONK_MISTWEAVER:
                            healers.push_back(player);
                            break;
                        default:
                            break;
                    }
                }
            }

            targets.remove_if(DpsSelectPredicate());

            if (targets.size() > 3)
                targets.resize(3);

            std::list<WorldObject*>::iterator tank = tanks.begin();
            std::list<WorldObject*>::iterator healer = healers.begin();

            if (tanks.size() > 1)
                std::advance(tank, urand(0, tanks.size() - 1));

            if (tank != tanks.end())
                targets.insert(targets.begin(), *tank);

            if (healers.size() > 1)
                std::advance(healer, urand(0, healers.size() - 1));

            if (healer != healers.end())
                targets.insert(targets.begin(), *healer);

            if (targets.empty())
                if (Creature* creature = caster->ToCreature())
                    if (creature->AI())
                        creature->AI()->DoAction(ACTION_BERSERK);
        }
    }

    void ApplyMorph(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Player* victim = GetHitPlayer();

        if (!caster || !victim)
            return;

        victim->AddAura(SPELL_OMINOUS_CACKLE_MORPH, victim);

        uint32 path = 0;
        switch (GetSpellInfo()->Id)
        {
            case 119593:
                path = 0;
                break;
            case 119693:
                path = 1;
                break;
            case 119692:
                path = 2;
                break;
            default:
                return;
        }

        if (victim->isMoving())
            victim->StopMoving();

        Movement::MoveSplineInit init(victim);
        init.Path().push_back(G3D::Vector3(victim->GetPositionX(), victim->GetPositionY(), victim->GetPositionZ()));
        for (uint32 i = 0; i < ominousCaclkePathLenght; ++i)
        {
            Position const& point = ominousCaclkePath[path][i];
            init.Path().push_back(G3D::Vector3(point.GetPositionX(), point.GetPositionY(), point.GetPositionZ()));
        }
        init.SetWalk(false);
        init.SetFly();
        init.SetVelocity(20.0f);
        init.Launch();
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ominous_caclke_target::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_ominous_caclke_target::ApplyMorph, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

class npc_sha_of_fear_bowman : public CreatureScript
{
    enum
    {
        SPELL_DEATH_BLOSSOM    = 119888,
        SPELL_DREAD_SPRAY      = 119862,
        SPELL_SHOOT            = 119862,
        SPELL_SHA_GLOBE        = 129178,
        SPELL_DREAD_SPRAY_INIT = 120047,
        SPELL_FEARLESS_ENABLER = 131504,
        SPELL_SHA_CORRUPTION   = 120000
    };

    enum
    {
        EVENT_DEATH_BLOSSOM = 1,
        EVENT_DREAD_SPRAY,
        EVENT_DREAD_SPRAY_END,
        EVENT_SHOOT
    };

    enum
    {
        SAY_AGGRO,
        SAY_DEATH,
        SAY_UNK
    };

public:
    npc_sha_of_fear_bowman() : CreatureScript("npc_sha_of_fear_bowman") { }

    struct npc_sha_of_fear_bowmanAI : public ScriptedAI
    {
        npc_sha_of_fear_bowmanAI(Creature* creature) : ScriptedAI(creature)
        {
            SetCombatMovement(false);
        }

        EventMap events;
        EventMap m_mLowEvents;

        void Reset() OVERRIDE
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->m_SightDistance = 40.0f;
            me->m_CombatDistance = 40.0f;
            lastHealthPct = 99;
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
            events.ScheduleEvent(EVENT_DEATH_BLOSSOM, 5000);
            events.ScheduleEvent(EVENT_SHOOT, 500);
            events.ScheduleEvent(EVENT_DREAD_SPRAY, 8000);
        }

        void EnterEvadeMode() OVERRIDE { }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);

            if (Creature* terrace = GetClosestCreatureWithEntry(me, NPC_RETURN_TO_TERRACE, 30.0f))
            {
                terrace->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                terrace->UpdateObjectVisibility();
                terrace->AddAura(120216, terrace);
            }
        }

        void DoAction(int32 actionId) OVERRIDE
        {
            if (actionId == ACTION_BOWMAN_AGGRESSIVE)
            {
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->setActive(true);

                if (Creature* terrace = GetClosestCreatureWithEntry(me, NPC_RETURN_TO_TERRACE, 30.0f))
                {
                    terrace->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    terrace->RemoveAura(120216);
                }
            }
        }

        void HealReceived(Unit* /*doneBy*/, uint32& /*heal*/) OVERRIDE
        {
            lastHealthPct = me->GetHealthPct();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            if (me->GetHealthPct() <= 5)
                return;

            if (me->HealthBelowPctDamaged(lastHealthPct, damage))
                for (; lastHealthPct > me->GetHealthPct(); lastHealthPct -= 4)
                    DoCast(me, SPELL_SHA_GLOBE, true);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_DREAD_SPRAY:
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        DoCast(me, SPELL_DREAD_SPRAY_INIT, false);
                        events.DelayEvents(9000);
                        events.ScheduleEvent(EVENT_DREAD_SPRAY, 20000);
                        events.ScheduleEvent(EVENT_DREAD_SPRAY_END, 8000);
                        break;
                    case EVENT_DREAD_SPRAY_END:
                        me->SetReactState(REACT_AGGRESSIVE);
                        break;
                    case EVENT_SHOOT:
                        DoCastVictim(SPELL_SHOOT, false);
                        events.ScheduleEvent(EVENT_SHOOT, 1000);
                        break;
                    case EVENT_DEATH_BLOSSOM:
                        if (me->GetHealthPct() < 25.1f)
                        {
                            Talk(SAY_UNK);
                            DoCast(me, SPELL_DEATH_BLOSSOM, false);
                            events.ScheduleEvent(EVENT_DEATH_BLOSSOM, 13000);
                        }
                        else
                            events.ScheduleEvent(EVENT_DEATH_BLOSSOM, 5000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        uint32 lastHealthPct;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_sha_of_fear_bowmanAI(creature);
    }
};

class npc_sha_globe : public CreatureScript
{
    enum
    {
        SPELL_SHA_GLOBE_VISUAL     = 129187,
        SPELL_SHA_GLOBE_PRE_VISUAL = 132214,
        SPELL_SHA_GLOBE_HEAL       = 129190,
        SPELL_SHA_GLOBE_DMG        = 129189
    };

public:
    npc_sha_globe() : CreatureScript("npc_sha_globe") { }

    struct npc_sha_globeAI : public ScriptedAI
    {
        npc_sha_globeAI(Creature* creature) : ScriptedAI(creature)
        {
            SetCombatMovement(false);
        }

        void Reset() OVERRIDE
        {
            DoCast(me, SPELL_SHA_GLOBE_VISUAL, true);
            visualTimer = 5000;
            healTimer = 8000;
            me->SetReactState(REACT_PASSIVE);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell) OVERRIDE
        {
            if (spell->Id == SPELL_SHA_GLOBE_DMG)
            {
                visualTimer = 0;
                healTimer = 0;
                me->RemoveAllAuras();
                me->DespawnOrUnsummon(500);
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (visualTimer)
            {
                if (visualTimer <= diff)
                {
                    DoCast(me, SPELL_SHA_GLOBE_PRE_VISUAL);
                    visualTimer = 0;
                }
                else
                    visualTimer -= diff;
            }
            else if (healTimer)
            {
                if (healTimer <= diff)
                {
                    DoCast(me, SPELL_SHA_GLOBE_HEAL, true);
                    me->RemoveAllAuras();
                    me->DespawnOrUnsummon(1000);
                    healTimer = 0;
                }
                else
                    healTimer -= diff;
            }
        }

    private:
        uint32 visualTimer;
        uint32 healTimer;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_sha_globeAI(creature);
    }
};

class spell_sha_globe_regen : public SpellScriptLoader
{
public:
    spell_sha_globe_regen() : SpellScriptLoader("spell_sha_globe_regen") { }

    class spell_sha_globe_regen_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_globe_regen_SpellScript);

        void RemoveOldAura()
        {
            if (Unit* victim = GetHitUnit())
                victim->RemoveAurasDueToSpell(129189);
        }

        void Register() OVERRIDE
        {
            BeforeHit += SpellHitFn(spell_sha_globe_regen_SpellScript::RemoveOldAura);
        }

        bool prevented;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_globe_regen_SpellScript();
    }
};

class spell_dread_spray_stackable : public SpellScriptLoader
{
    enum
    {
        SPELL_DREAD_AURA = 119983,
        SPELL_FEAR       = 119985
    };

public:
    spell_dread_spray_stackable() : SpellScriptLoader("spell_dread_spray_stackable") { }

    class spell_dread_spray_stackable_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dread_spray_stackable_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetStackAmount() == 2)
                if (Unit* owner = GetUnitOwner())
                    owner->CastSpell(owner, SPELL_FEAR, true);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dread_spray_stackable_AuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAPPLY);
        }
    };

    class spell_dread_spray_stackable_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dread_spray_stackable_SpellScript);

        std::list<WorldObject*> m_targets;

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* caster = GetCaster())
                targets.remove_if([caster](WorldObject* target)
                {
                    return target->ToUnit()->HasAura(SPELL_FEAR) ||
                        !target->IsWithinLOSInMap(caster) ||
                        !caster->HasInLine(target, 35.0f);
                });

            m_targets = targets;
        }

        void CopyTargets(std::list<WorldObject*>& targets)
        {
            targets = m_targets;
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_spray_stackable_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_spray_stackable_SpellScript::CopyTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_spray_stackable_SpellScript::CopyTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dread_spray_stackable_AuraScript();
    }

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dread_spray_stackable_SpellScript();
    }
};

class spell_death_blossom : public SpellScriptLoader
{
public:
    spell_death_blossom() : SpellScriptLoader("spell_death_blossom") { }

    class spell_death_blossom_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_death_blossom_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* caster = GetCaster())
                targets.remove_if([caster](WorldObject* target)
                {
                    return !target->IsWithinLOSInMap(caster);
                });
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_death_blossom_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_death_blossom_SpellScript();
    }
};

class spell_dread_spray : public SpellScriptLoader
{
public:
    spell_dread_spray() : SpellScriptLoader("spell_dread_spray") { }

    class spell_dread_spray_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dread_spray_AuraScript);

        void OnTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* owner = GetUnitOwner())
            {
                owner->SetTarget(0);
                float orientation = float(rand_norm()) * static_cast<float>(2 * M_PI);
                owner->SetOrientation(orientation);
                owner->SetFacingTo(orientation);
                owner->CastSpell(owner, 119958, true);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dread_spray_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dread_spray_AuraScript();
    }
};

class notPlayerPredicate
{
public:
    bool operator()(WorldObject* target) const
    {
        return target && !target->ToPlayer();
    }
};

class spell_teleport_to_dread_expanse : public SpellScript
{
    PrepareSpellScript(spell_teleport_to_dread_expanse);

    void HandleOnHit()
    {
        if (Unit* unit = GetHitUnit())
        {
            unit->CastSpell(unit, SPELL_TELEPORT_PLAYER, true);
            unit->CastSpell(unit, SPELL_DREAD_EXPANSE, true);
            unit->RemoveAurasDueToSpell(SPELL_LEIS_HOPE);
        }
    }

    void SelectTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(notPlayerPredicate());
    }

    void Register() OVERRIDE
    {
        OnHit += SpellHitFn(spell_teleport_to_dread_expanse::HandleOnHit);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_teleport_to_dread_expanse::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

class spell_huddle_in_terror : public SpellScript
{
    PrepareSpellScript(spell_huddle_in_terror);

    std::list<WorldObject*> m_targets;

    void SelectTargets(std::list<WorldObject*>& targets)
    {
        if (Unit* caster = GetCaster())
        {
            std::list<Player*> players;
            GetPlayerListInGrid(players, caster, 300.0f);

            players.remove_if([](Player* player)
            {
                switch (player->GetTalentSpecialization(player->GetActiveSpec()))
                {
                    case SPEC_MONK_BREWMASTER:
                    case SPEC_DEATH_KNIGHT_BLOOD:
                    case SPEC_WARRIOR_PROTECTION:
                    case SPEC_DRUID_GUARDIAN:
                    case SPEC_PALADIN_PROTECTION:
                        return true;
                    default:
                        return false;
                }
            });
            players.remove_if([](Player* player)
            {
                return player->HasAura(SPELL_CHAMPION_OF_THE_LIGHT);
            });

            uint32 maxVictims = caster->GetInstanceScript()->instance->Is25ManRaid() ? 5 : 2;

            if (players.size() >= maxVictims)
            {
                targets.clear();

                for (Player* player : players)
                    targets.push_back(player);

                if (players.size() > maxVictims)
                    Skyfire::Containers::RandomResizeList(targets, maxVictims);

                m_targets = targets;
                return;
            }

            targets.remove_if([](WorldObject* target)
            {
                return target && target->ToPlayer() &&
                    target->ToPlayer()->HasAura(SPELL_CHAMPION_OF_THE_LIGHT);
            });

            if (targets.size() > maxVictims)
                Skyfire::Containers::RandomResizeList(targets, maxVictims);

            m_targets = targets;
        }
    }

    void CopyTargets(std::list<WorldObject*>& targets)
    {
        targets = m_targets;
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_huddle_in_terror::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_huddle_in_terror::CopyTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_huddle_in_terror::CopyTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_sha_waterspout : public SpellScriptLoader
{
public:
    spell_sha_waterspout() : SpellScriptLoader("spell_sha_waterspout") { }

    class spell_sha_waterspout_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_waterspout_SpellScript);

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                Player* player = target ? target->ToPlayer() : NULL;
                if (!player)
                    return false;

                if (player->HasAura(SPELL_CHAMPION_OF_THE_LIGHT))
                    return true;

                if (player->IsGameMaster())
                    return false;

                uint32 spec = player->GetTalentSpecialization(player->GetActiveSpec());
                switch (spec)
                {
                    case SPEC_MONK_BREWMASTER:
                    case SPEC_DEATH_KNIGHT_BLOOD:
                    case SPEC_WARRIOR_PROTECTION:
                    case SPEC_DRUID_GUARDIAN:
                    case SPEC_PALADIN_PROTECTION:
                    case SPEC_PALADIN_RETRIBUTION:
                    case SPEC_MONK_WINDWALKER:
                    case SPEC_DRUID_FERAL:
                    case SPEC_SHAMAN_ENHANCEMENT:
                        return true;
                    default:
                        break;
                }

                return player->getClass() == CLASS_ROGUE ||
                    (player->getClass() == CLASS_WARRIOR && spec != SPEC_WARRIOR_PROTECTION) ||
                    (player->getClass() == CLASS_DEATH_KNIGHT && spec != SPEC_DEATH_KNIGHT_BLOOD);
            });
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_waterspout_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    class spell_sha_waterspout_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_waterspout_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* owner = GetOwner()->ToPlayer())
            {
                if (TempSummon* waterspout = owner->SummonCreature(NPC_WATERSPOUT, *owner, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 6 * IN_MILLISECONDS))
                {
                    waterspout->AddAura(SPELL_WATERSPOUT, waterspout);
                    waterspout->CastSpell(waterspout, SPELL_WATERSPOUT_DUMMY, true);
                }
            }
            else if (Creature* owner = GetOwner()->ToCreature())
                owner->CastSpell(owner, SPELL_WATERSPOUT_DAMAGE, false);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_sha_waterspout_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_waterspout_AuraScript();
    }

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_waterspout_SpellScript();
    }
};

class spell_sha_submerge : public SpellScriptLoader
{
public:
    spell_sha_submerge() : SpellScriptLoader("spell_sha_submerge") { }

    class spell_sha_submerge_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_submerge_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Creature* owner = GetOwner()->ToCreature())
            {
                owner->SetReactState(REACT_PASSIVE);
                owner->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);

                std::list<Player*> players;
                GetPlayerListInGrid(players, owner, 200.0f);

                if (!players.empty())
                {
                    if (Player* player = Skyfire::Containers::SelectRandomContainerElement(players))
                    {
                        owner->NearTeleportTo(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), owner->GetOrientation());
                        owner->CastSpell(owner, SPELL_EMERGE, false);
                    }
                }
            }
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_sha_submerge_AuraScript::OnApply, EFFECT_1, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_submerge_AuraScript();
    }
};

class spell_sha_emerge : public SpellScriptLoader
{
public:
    spell_sha_emerge() : SpellScriptLoader("spell_sha_emerge") { }

    class spell_sha_emerge_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_emerge_SpellScript);

        void HandleAfterCast()
        {
            if (Creature* caster = GetCaster()->ToCreature())
                caster->AI()->DoAction(ACTION_SUBMERGE_REMOVE);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_sha_emerge_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_emerge_SpellScript();
    }
};

class spell_transfer_light : public SpellScript
{
    PrepareSpellScript(spell_transfer_light);

    SpellCastResult CheckCast()
    {
        if (Unit* caster = GetCaster())
            if (Player* target = ObjectAccessor::GetPlayer(*caster, caster->GetTarget()))
                if (target->IsAlive())
                    return SPELL_CAST_OK;

        return SPELL_FAILED_SPELL_UNAVAILABLE;
    }

    void HandleTransfer()
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = GetHitUnit())
            {
                caster->RemoveAurasDueToSpell(SPELL_CHAMPION_OF_THE_LIGHT);

                if (!target->IsAlive())
                    target->CastSpell(caster, SPELL_TRANSFER_LIGHT, true);
                else
                    target->CastSpell(target, SPELL_CHAMPION_OF_THE_LIGHT, true);

                target->RemoveAurasDueToSpell(SPELL_HUDDLE_IN_TERROR);

                std::list<Creature*> dreadSpawns;
                GetCreatureListWithEntryInGrid(dreadSpawns, caster, NPC_DREAD_SPAWN, 300.0f);

                for (Creature* dreadSpawn : dreadSpawns)
                    dreadSpawn->AI()->DoAction(ACTION_SELECT_NEW_TARGET);
            }
        }
    }

    void HandleOnCast()
    {
        if (Unit* caster = GetCaster())
        {
            std::list<Creature*> dreadSpawns;
            GetCreatureListWithEntryInGrid(dreadSpawns, caster, NPC_DREAD_SPAWN, 300.0f);

            for (Creature* dreadSpawn : dreadSpawns)
                if (dreadSpawn->IsAlive())
                    dreadSpawn->AI()->DoAction(ACTION_EMOTE);
        }
    }

    void Register() OVERRIDE
    {
        OnCheckCast += SpellCheckCastFn(spell_transfer_light::CheckCast);
        OnHit += SpellHitFn(spell_transfer_light::HandleTransfer);
        OnCast += SpellCastFn(spell_transfer_light::HandleOnCast);
    }
};

class LightWallTargetSelector
{
public:
    bool operator()(WorldObject* object) const
    {
        return object && object->GetEntry() != NPC_PURE_LIGHT_TERRACE;
    }
};

class spell_light_wall_active : public SpellScriptLoader
{
public:
    spell_light_wall_active() : SpellScriptLoader("spell_light_wall_active") { }

    class spell_light_wall_active_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_light_wall_active_SpellScript);

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(LightWallTargetSelector());
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_light_wall_active_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_light_wall_active_SpellScript();
    }
};

class spell_ominous_cackle_morph : public AuraScript
{
    PrepareAuraScript(spell_ominous_cackle_morph);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* owner = GetOwner()->ToPlayer())
        {
            owner->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
            owner->UnsummonPetTemporaryIfAny();
        }
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* owner = GetOwner()->ToPlayer())
        {
            owner->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
            owner->ResummonPetTemporaryUnSummonedIfAny();
        }
    }

    void Register() OVERRIDE
    {
        AfterEffectApply += AuraEffectApplyFn(spell_ominous_cackle_morph::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_ominous_cackle_morph::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_heroic_transition_screen_eff : public AuraScript
{
    PrepareAuraScript(spell_heroic_transition_screen_eff);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* owner = GetOwner()->ToPlayer())
            owner->CastSpell(owner, SPELL_TELEPORT_TO_DREAD_EXPANSE, true);
    }

    void Register() OVERRIDE
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_heroic_transition_screen_eff::OnRemove, EFFECT_2, SPELL_AURA_SCREEN_EFFECT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_heroic_sha_spine : public SpellScript
{
    PrepareSpellScript(spell_heroic_sha_spine);

    void CorrectTargets(std::list<WorldObject*>& targets)
    {
        if (Unit* caster = GetCaster())
        {
            uint32 maxVictims = caster->GetMap()->Is25ManRaid() ? 10 : 5;

            if (targets.size() > maxVictims)
                Skyfire::Containers::RandomResizeList(targets, maxVictims);
        }
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_heroic_sha_spine::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_heroic_fading_light : public AuraScript
{
    PrepareAuraScript(spell_heroic_fading_light);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* owner = GetOwner()->ToPlayer())
            owner->RemoveAllSpellCooldown();
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_heroic_fading_light::OnApply, EFFECT_0, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_of_fear_dread_spray_fear : public AuraScript
{
    PrepareAuraScript(spell_sha_of_fear_dread_spray_fear);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* owner = GetOwner()->ToPlayer())
            owner->GetMap()->SetWorldState(WORLDSTATE_MIND_KILLER, 0);
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_of_fear_dread_spray_fear::OnApply, EFFECT_0, SPELL_AURA_MOD_FEAR, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_boss_sha_of_fear()
{
    new boss_sha_of_fear();
    new npc_pure_light_terrace();
    new npc_terror_spawn();
    new npc_dread_spawn();
    new spell_champion_of_light();
    new spell_breath_of_fear();
    new spell_conjure_terror_spawn();
    new spell_penetrating_bolt();
    RegisterSpellScript(spell_ominous_caclke_target);
    new npc_sha_of_fear_bowman();
    new spell_dread_spray_stackable();
    new spell_dread_spray();
    new npc_sha_globe();
    new spell_sha_globe_regen();
    new spell_breath_of_fear_fear();
    new spell_death_blossom();
    RegisterSpellScript(spell_teleport_to_dread_expanse);
    RegisterSpellScript(spell_huddle_in_terror);
    new spell_sha_waterspout();
    new spell_sha_submerge();
    new spell_sha_emerge();
    RegisterSpellScript(spell_transfer_light);
    new spell_light_wall_active();
    RegisterAuraScript(spell_ominous_cackle_morph);
    RegisterAuraScript(spell_heroic_transition_screen_eff);
    RegisterSpellScript(spell_heroic_sha_spine);
    RegisterAuraScript(spell_heroic_fading_light);
    RegisterAuraScript(spell_sha_of_fear_dread_spray_fear);
}
