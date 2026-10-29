/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
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

/*
 * Hadronox starts at the bottom of the tunnel and climbs towards the web doors
 * each time one of the Anub'ar Crushers guarding her path dies. Three packs of
 * crusher-led Anub'ar are summoned around her (one at the start, two more when
 * the first one is engaged), while the world triggers at the web doors keep
 * sending Anub'ar down to fight her until she webs the doors shut.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "azjol_nerub.h"
#include "InstanceScript.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "TemporarySummon.h"

enum Events
{
    // Hadronox
    EVENT_LEECH_POISON = 1,
    EVENT_ACID_CLOUD,
    EVENT_WEB_GRAB,
    EVENT_PIERCE_ARMOR,
    EVENT_PLAYER_CHECK,

    // Anub'ar Crusher
    EVENT_SMASH,

    // Anub'ar foes - Shared
    EVENT_TAUNT,

    // Anub'ar Champion
    EVENT_REND,
    EVENT_PUMMEL,

    // Anub'ar Crypt Guard
    EVENT_CRUSHING_WEBS,
    EVENT_INFECTED_WOUND,

    // Anub'ar Necromancer
    EVENT_SHADOW_BOLT,
    EVENT_ANIMATE_BONES
};

enum Spells
{
    // Hadronox
    SPELL_WEB_FRONT_DOORS                   = 53177,
    SPELL_WEB_SIDE_DOORS                    = 53185,
    SPELL_LEECH_POISON                      = 53030,
    SPELL_LEECH_POISON_HEROIC               = 59417,
    SPELL_LEECH_POISON_HEAL                 = 53800,
    SPELL_ACID_CLOUD                        = 53400,
    SPELL_ACID_CLOUD_HEROIC                 = 59419,
    SPELL_WEB_GRAB                          = 57731,
    SPELL_PIERCE_ARMOR                      = 53418,

    // Anub'ar opponent summoning spells
    SPELL_SUMMON_CHAMPION_PERIODIC          = 53035,
    SPELL_SUMMON_CRYPT_FIEND_PERIODIC       = 53037,
    SPELL_SUMMON_NECROMANCER_PERIODIC       = 53036,
    SPELL_SUMMON_CHAMPION_TOP               = 53064,
    SPELL_SUMMON_CRYPT_FIEND_TOP            = 53065,
    SPELL_SUMMON_NECROMANCER_TOP            = 53066,
    SPELL_SUMMON_CHAMPION_BOTTOM            = 53090,
    SPELL_SUMMON_CRYPT_FIEND_BOTTOM         = 53091,
    SPELL_SUMMON_NECROMANCER_BOTTOM         = 53092,

    // Anub'ar Crusher
    SPELL_SMASH                             = 53318,
    SPELL_SMASH_HEROIC                      = 59346,
    SPELL_FRENZY                            = 53801,

    // Anub'ar foes - Shared
    SPELL_TAUNT                             = 53798,

    // Anub'ar Champion
    SPELL_REND                              = 59343,
    SPELL_PUMMEL                            = 59344,

    // Anub'ar Crypt Guard
    SPELL_CRUSHING_WEBS                     = 59347,
    SPELL_INFECTED_WOUND                    = 59348,

    // Anub'ar Necromancer
    SPELL_SHADOW_BOLT                       = 53333,
    SPELL_ANIMATE_BONES_1                   = 53334,
    SPELL_ANIMATE_BONES_2                   = 53336
};

enum SummonGroups
{
    SUMMON_GROUP_NONE           = 0,
    SUMMON_GROUP_CRUSHER_1      = 1,
    SUMMON_GROUP_CRUSHER_2      = 2,
    SUMMON_GROUP_CRUSHER_3      = 3
};

enum Actions
{
    ACTION_HADRONOX_MOVE = 1,
    ACTION_CRUSHER_ENGAGED,
    ACTION_PACK_WALK,
    ACTION_ENCOUNTER_RESET
};

enum HadronoxData
{
    DATA_CRUSHER_PACK_ID = 1,
    DATA_HADRONOX_ENTERED_COMBAT,
    DATA_HADRONOX_WEBBED_DOORS
};

enum Creatures
{
    NPC_CRUSHER             = 28922,
    NPC_CRUSHER_CHAMPION    = 29117,
    NPC_CRUSHER_CRYPT_FIEND = 29118,
    NPC_CRUSHER_NECROMANCER = 29119,
    NPC_WORLDTRIGGER_LARGE  = 23472
};

enum Talk
{
    CRUSHER_SAY_AGGRO = 1,
    CRUSHER_EMOTE_FRENZY = 2,
    HADRONOX_EMOTE_MOVE = 1
};

enum HadronoxMisc
{
    EVADE_RESPAWN_DELAY     = 25,       // seconds
    PACK_ASSIST_RANGE       = 40,       // yards
    PERIODIC_SUMMON_TOP_Z   = 750,      // triggers above this height use the "top" summon spells
    FOE_HADRONOX_CUTOFF_Z   = 702       // below this height Hadronox is not yet reachable by walking
};

// Movement IDs used by the permanently spawning Anub'ar opponents - they are done in sequence, as one finishes, the next one starts
enum Movements
{
    MOVE_NONE = 0,
    MOVE_OUTSIDE,
    MOVE_DOWNSTAIRS,
    MOVE_DOWNSTAIRS_2,
    MOVE_HADRONOX,       // this one might have us take a detour to avoid pathfinding "through" the floor...
    MOVE_HADRONOX_REAL   // while this one will always make us movechase
};

static uint32 RandTimeMs(uint32 minSeconds, uint32 maxSeconds)
{
    return urand(minSeconds * IN_MILLISECONDS, maxSeconds * IN_MILLISECONDS);
}

// Creatures are removed on evade and come back after a fixed delay, the way BossAI::_DespawnAtEvade does it for the boss.
static void DespawnAtEvade(Creature* creature, uint32 respawnDelay)
{
    uint32 corpseDelay = creature->GetCorpseDelay();
    uint32 oldRespawnDelay = creature->GetRespawnDelay();

    creature->SetCorpseDelay(1);
    creature->SetRespawnDelay(respawnDelay);

    creature->DespawnOrUnsummon();

    creature->SetCorpseDelay(corpseDelay);
    creature->SetRespawnDelay(oldRespawnDelay);
}

static Creature* GetHadronox(Creature* me, InstanceScript* instance)
{
    if (!instance)
        return NULL;
    return Unit::GetCreature(*me, instance->GetData64(DATA_HADRONOX));
}

static const uint8 NUM_STEPS = 4;
static const Position hadronoxStep[NUM_STEPS] =
{
    { 515.5848f, 544.2007f, 673.6272f },
    { 562.191f , 514.068f , 696.4448f },
    { 610.3828f, 518.6407f, 695.9385f },
    { 530.42f  , 560.003f,  733.0308f }
};

struct boss_hadronox : public BossAI
{
    boss_hadronox(Creature* creature) : BossAI(creature, DATA_HADRONOX), _enteredCombat(false), _doorsWebbed(false), _lastPlayerCombatState(false), _summonFirstPack(false), _step(0) { }

    bool IsInCombatWithPlayer() const
    {
        ThreatContainer::StorageType const& threatList = me->getThreatManager().getThreatList();
        for (ThreatContainer::StorageType::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
            if (Unit* target = (*itr)->getTarget())
                if (target->IsControlledByPlayer())
                    return true;
        return false;
    }

    void SetStep(uint8 step)
    {
        if (_lastPlayerCombatState)
            return;

        _step = step;
        me->SetReactState(REACT_PASSIVE);
        me->SetHomePosition(hadronoxStep[step]);
        me->GetMotionMaster()->Clear();
        me->AttackStop();
        me->GetMotionMaster()->MovePoint(0, hadronoxStep[step]);
    }

    void SummonCrusherPack(SummonGroups group)
    {
        std::list<TempSummon*> summoned;
        me->SummonCreatureGroup(group, &summoned);
        for (std::list<TempSummon*>::iterator itr = summoned.begin(); itr != summoned.end(); ++itr)
        {
            (*itr)->AI()->SetData(DATA_CRUSHER_PACK_ID, group);
            (*itr)->AI()->DoAction(ACTION_PACK_WALK);
        }
    }

    void MovementInform(uint32 type, uint32 /*id*/) OVERRIDE
    {
        if (type != POINT_MOTION_TYPE)
            return;
        me->SetReactState(REACT_AGGRESSIVE);
        if (_step < NUM_STEPS - 1)
            return;
        DoCast(me, SPELL_WEB_FRONT_DOORS);
        DoCast(me, SPELL_WEB_SIDE_DOORS);
        _doorsWebbed = true;
        DoZoneInCombat();
    }

    uint32 GetData(uint32 data) const OVERRIDE
    {
        if (data == DATA_HADRONOX_ENTERED_COMBAT)
            return _enteredCombat ? 1 : 0;
        if (data == DATA_HADRONOX_WEBBED_DOORS)
            return _doorsWebbed ? 1 : 0;
        return 0;
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_LEECH_POISON, RandTimeMs(5, 7));
        events.ScheduleEvent(EVENT_ACID_CLOUD, RandTimeMs(7, 13));
        events.ScheduleEvent(EVENT_WEB_GRAB, RandTimeMs(13, 19));
        events.ScheduleEvent(EVENT_PIERCE_ARMOR, RandTimeMs(4, 7));
        events.ScheduleEvent(EVENT_PLAYER_CHECK, 1 * IN_MILLISECONDS);
        me->setActive(true);
    }

    void DoAction(int32 action) OVERRIDE
    {
        switch (action)
        {
            case ACTION_CRUSHER_ENGAGED:
                if (_enteredCombat)
                    break;
                instance->SetBossState(DATA_HADRONOX, IN_PROGRESS);
                _enteredCombat = true;
                SummonCrusherPack(SUMMON_GROUP_CRUSHER_2);
                SummonCrusherPack(SUMMON_GROUP_CRUSHER_3);
                break;
            case ACTION_HADRONOX_MOVE:
                if (_step < NUM_STEPS - 1)
                {
                    SetStep(_step + 1);
                    Talk(HADRONOX_EMOTE_MOVE);
                }
                break;
            case ACTION_ENCOUNTER_RESET:
                ResetEncounter();
                break;
        }
    }

    // Hadronox is also left without targets when she has killed the Anub'ar sent against her,
    // that is no reason to throw the whole encounter away: only drop combat and wait for the next ones.
    void EnterEvadeMode() OVERRIDE
    {
        if (_lastPlayerCombatState)
        {
            ResetEncounter();
            return;
        }

        events.Reset();
        me->DeleteThreatList();
        me->CombatStop(true);
    }

    void ResetEncounter()
    {
        std::list<Creature*> triggers;
        me->GetCreatureListWithEntryInGrid(triggers, NPC_WORLDTRIGGER_LARGE, 150.0f);
        for (std::list<Creature*>::iterator itr = triggers.begin(); itr != triggers.end(); ++itr)
            if ((*itr)->HasAura(SPELL_SUMMON_CHAMPION_PERIODIC) || (*itr)->HasAura(SPELL_WEB_FRONT_DOORS) || (*itr)->HasAura(SPELL_WEB_SIDE_DOORS))
                DespawnAtEvade(*itr, EVADE_RESPAWN_DELAY);

        instance->SetBossState(DATA_HADRONOX, FAIL);
        summons.DespawnAll();
        for (std::list<uint64>::iterator itr = _anubar.begin(); itr != _anubar.end(); ++itr)
            if (Creature* nerubian = Unit::GetCreature(*me, *itr))
                nerubian->DespawnOrUnsummon();
        _anubar.clear();

        DespawnAtEvade(me, EVADE_RESPAWN_DELAY);
    }

    void SetGUID(uint64 guid, int32 /*id*/ = 0) OVERRIDE
    {
        _anubar.push_back(guid);
    }

    void Reset() OVERRIDE
    {
        BossAI::Reset();
        me->SetReactState(REACT_AGGRESSIVE);
        me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 9.0f);
        me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 9.0f);
        SetCombatMovement(true);
        _enteredCombat = false;
        _doorsWebbed = false;
        _lastPlayerCombatState = false;
        _step = 0;
        _anubar.clear();
        // The first crusher pack cannot be summoned while the creature is still being added to the map
        _summonFirstPack = instance->GetBossState(DATA_HADRONOX) != DONE;
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (_summonFirstPack)
        {
            _summonFirstPack = false;
            SummonCrusherPack(SUMMON_GROUP_CRUSHER_1);
        }

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_LEECH_POISON:
                    DoCastAOE(DUNGEON_MODE(SPELL_LEECH_POISON, SPELL_LEECH_POISON_HEROIC));
                    events.ScheduleEvent(EVENT_LEECH_POISON, RandTimeMs(7, 9));
                    break;
                case EVENT_ACID_CLOUD:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f))
                        DoCast(target, DUNGEON_MODE(SPELL_ACID_CLOUD, SPELL_ACID_CLOUD_HEROIC));
                    events.ScheduleEvent(EVENT_ACID_CLOUD, RandTimeMs(16, 23));
                    break;
                case EVENT_WEB_GRAB:
                    DoCastAOE(SPELL_WEB_GRAB);
                    events.ScheduleEvent(EVENT_WEB_GRAB, RandTimeMs(20, 25));
                    break;
                case EVENT_PIERCE_ARMOR:
                    DoCastVictim(SPELL_PIERCE_ARMOR);
                    events.ScheduleEvent(EVENT_PIERCE_ARMOR, RandTimeMs(10, 15));
                    break;
                case EVENT_PLAYER_CHECK:
                    if (IsInCombatWithPlayer() != _lastPlayerCombatState)
                    {
                        _lastPlayerCombatState = !_lastPlayerCombatState;
                        if (_lastPlayerCombatState) // we are now in combat with players
                        {
                            if (!instance->CheckRequiredBosses(DATA_HADRONOX))
                            {
                                ResetEncounter();
                                return;
                            }
                            // cancel current point movement if engaged by players
                            if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                            {
                                me->GetMotionMaster()->Clear();
                                SetCombatMovement(true);
                                AttackStart(me->GetVictim());
                            }
                        }
                        else // we are no longer in combat with players - reset the encounter
                        {
                            ResetEncounter();
                            return;
                        }
                    }
                    events.ScheduleEvent(EVENT_PLAYER_CHECK, 1 * IN_MILLISECONDS);
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }

    // Safeguard to prevent Hadronox dying to NPCs
    void DamageTaken(Unit* who, uint32& damage) OVERRIDE
    {
        if ((!who || !who->IsControlledByPlayer()) && me->HealthBelowPct(70))
        {
            if (me->HealthBelowPctDamaged(5, damage))
                damage = 0;
            else
                damage *= (me->GetHealthPct() - 5.0f) / 65.0f;
        }
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summons.Summon(summon);
        // Do not enter combat with zone
    }

    private:
        bool _enteredCombat;            // has a player entered combat with the first crusher pack? (talk and spawn two more packs)
        bool _doorsWebbed;              // obvious - have we reached the top and webbed the doors shut? (trigger for hadronox denied achievement)
        bool _lastPlayerCombatState;    // was there a player in our threat list the last time we checked (we check every second)
        bool _summonFirstPack;
        uint8 _step;
        std::list<uint64> _anubar;
};

struct npc_hadronox_crusherPackAI : public ScriptedAI
{
    npc_hadronox_crusherPackAI(Creature* creature, Position const* positions) : ScriptedAI(creature), _instance(creature->GetInstanceScript()), _positions(positions), _myPack(SUMMON_GROUP_NONE), _doFacing(false), _engaged(false), _proximityTimer(0) { }

    void DoAction(int32 action) OVERRIDE
    {
        if (action == ACTION_PACK_WALK)
        {
            switch (_myPack)
            {
                case SUMMON_GROUP_CRUSHER_1:
                case SUMMON_GROUP_CRUSHER_2:
                case SUMMON_GROUP_CRUSHER_3:
                    me->GetMotionMaster()->MovePoint(ACTION_PACK_WALK, _positions[_myPack - SUMMON_GROUP_CRUSHER_1]);
                    break;
                default:
                    break;
            }
        }
    }

    void MovementInform(uint32 type, uint32 id) OVERRIDE
    {
        if (type == POINT_MOTION_TYPE && id == ACTION_PACK_WALK)
            _doFacing = true;
    }

    void EnterEvadeMode() OVERRIDE
    {
        if (Creature* hadronox = GetHadronox(me, _instance))
            hadronox->AI()->DoAction(ACTION_ENCOUNTER_RESET);
    }

    uint32 GetData(uint32 data) const OVERRIDE
    {
        if (data == DATA_CRUSHER_PACK_ID)
            return _myPack;
        return 0;
    }

    void SetData(uint32 data, uint32 value) OVERRIDE
    {
        if (data == DATA_CRUSHER_PACK_ID)
        {
            _myPack = SummonGroups(value);
            me->SetReactState(_myPack ? REACT_PASSIVE : REACT_AGGRESSIVE);
        }
    }

    void AssistPack(Unit* who)
    {
        static const uint32 packEntries[] = { NPC_CRUSHER, NPC_CRUSHER_CHAMPION, NPC_CRUSHER_CRYPT_FIEND, NPC_CRUSHER_NECROMANCER };

        for (uint8 i = 0; i < sizeof(packEntries) / sizeof(packEntries[0]); ++i)
        {
            std::list<Creature*> others;
            me->GetCreatureListWithEntryInGrid(others, packEntries[i], float(PACK_ASSIST_RANGE));
            for (std::list<Creature*>::iterator itr = others.begin(); itr != others.end(); ++itr)
            {
                Creature* other = *itr;
                if (other == me || !other->IsAlive() || !other->AI() || other->AI()->GetData(DATA_CRUSHER_PACK_ID) != uint32(_myPack))
                    continue;
                other->SetReactState(REACT_AGGRESSIVE);
                other->AI()->AttackStart(who);
            }
        }
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        // MoveInLineOfSight starts the pack by hand and the engine calls this again once the attack starts
        if (_engaged)
            return;
        _engaged = true;

        if (me->HasReactState(REACT_PASSIVE))
        {
            AssistPack(who);
            me->SetReactState(REACT_AGGRESSIVE);
            AttackStart(who);
        }
        _EnterCombat();
    }

    void Reset() OVERRIDE
    {
        _events.Reset();
        _engaged = false;
    }

    virtual void _EnterCombat() = 0;
    virtual void DoEvent(uint32 /*eventId*/) = 0;

    // Passive creatures get no line of sight callbacks from the grid, so a waiting pack looks for players itself
    void CheckForPlayers(uint32 diff)
    {
        if (_proximityTimer > diff)
        {
            _proximityTimer -= diff;
            return;
        }
        _proximityTimer = 1 * IN_MILLISECONDS;

        Map::PlayerList const& players = me->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* player = itr->GetSource();
            if (player && player->IsAlive() && me->CanStartAttack(player, false) && me->IsWithinLOSInMap(player))
            {
                EnterCombat(player);
                return;
            }
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (_doFacing)
        {
            _doFacing = false;
            me->SetFacingTo(_positions[_myPack - SUMMON_GROUP_CRUSHER_1].GetOrientation());
        }

        if (!_engaged && me->HasReactState(REACT_PASSIVE))
            CheckForPlayers(diff);

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
            DoEvent(eventId);

        DoMeleeAttackIfReady();
    }

    protected:
        InstanceScript* const _instance;
        EventMap _events;
        Position const* const _positions;
        SummonGroups _myPack;
        bool _doFacing;
        bool _engaged;
        uint32 _proximityTimer;
};

static const Position crusherWaypoints[] =
{
    { 529.6913f, 547.1257f, 731.9155f, 4.799650f },
    { 517.51f  , 561.439f , 734.0306f, 4.520403f },
    { 543.414f , 551.728f , 732.0522f, 3.996804f }
};
struct npc_anub_ar_crusher : public npc_hadronox_crusherPackAI
{
    npc_anub_ar_crusher(Creature* creature) : npc_hadronox_crusherPackAI(creature, crusherWaypoints), _hadFrenzy(false) { }

    void Reset() OVERRIDE
    {
        npc_hadronox_crusherPackAI::Reset();
        _hadFrenzy = false;
    }

    void _EnterCombat() OVERRIDE
    {
        _events.ScheduleEvent(EVENT_SMASH, RandTimeMs(8, 12));

        if (_myPack != SUMMON_GROUP_CRUSHER_1)
            return;

        if (Creature* hadronox = GetHadronox(me, _instance))
        {
            if (hadronox->AI()->GetData(DATA_HADRONOX_ENTERED_COMBAT))
                return;
            hadronox->AI()->DoAction(ACTION_CRUSHER_ENGAGED);
        }

        Talk(CRUSHER_SAY_AGGRO);
    }

    void DamageTaken(Unit* /*source*/, uint32& damage) OVERRIDE
    {
        if (_hadFrenzy || !me->HealthBelowPctDamaged(25, damage))
            return;
        _hadFrenzy = true;
        Talk(CRUSHER_EMOTE_FRENZY);
        DoCast(me, SPELL_FRENZY);
    }

    void DoEvent(uint32 eventId) OVERRIDE
    {
        switch (eventId)
        {
            case EVENT_SMASH:
                DoCastVictim(DUNGEON_MODE(SPELL_SMASH, SPELL_SMASH_HEROIC));
                _events.ScheduleEvent(EVENT_SMASH, RandTimeMs(13, 21));
                break;
        }
    }

    void JustDied(Unit* killer) OVERRIDE
    {
        if (Creature* hadronox = GetHadronox(me, _instance))
            hadronox->AI()->DoAction(ACTION_HADRONOX_MOVE);
        ScriptedAI::JustDied(killer);
    }

    private:
        bool _hadFrenzy;
};

static const Position championWaypoints[] =
{
    { 539.2076f, 549.7539f, 732.8668f, 4.55531f  },
    { 527.3098f, 559.5197f, 732.9407f, 4.742493f },
    { }
};
struct npc_anub_ar_crusher_champion : public npc_hadronox_crusherPackAI
{
    npc_anub_ar_crusher_champion(Creature* creature) : npc_hadronox_crusherPackAI(creature, championWaypoints) { }

    void DoEvent(uint32 eventId) OVERRIDE
    {
        switch (eventId)
        {
            case EVENT_REND:
                DoCastVictim(SPELL_REND);
                _events.ScheduleEvent(EVENT_REND, RandTimeMs(12, 16));
                break;
            case EVENT_PUMMEL:
                DoCastVictim(SPELL_PUMMEL);
                _events.ScheduleEvent(EVENT_PUMMEL, RandTimeMs(12, 17));
                break;
        }
    }

    void _EnterCombat() OVERRIDE
    {
        _events.ScheduleEvent(EVENT_REND, RandTimeMs(4, 8));
        _events.ScheduleEvent(EVENT_PUMMEL, RandTimeMs(15, 19));
    }
};

static const Position cryptFiendWaypoints[] =
{
    { 520.3911f, 548.7895f, 732.0118f, 5.0091f   },
    { },
    { 550.9611f, 545.1674f, 731.9031f, 3.996804f }
};
struct npc_anub_ar_crusher_crypt_fiend : public npc_hadronox_crusherPackAI
{
    npc_anub_ar_crusher_crypt_fiend(Creature* creature) : npc_hadronox_crusherPackAI(creature, cryptFiendWaypoints) { }

    void DoEvent(uint32 eventId) OVERRIDE
    {
        switch (eventId)
        {
            case EVENT_CRUSHING_WEBS:
                DoCastVictim(SPELL_CRUSHING_WEBS);
                _events.ScheduleEvent(EVENT_CRUSHING_WEBS, RandTimeMs(12, 16));
                break;
            case EVENT_INFECTED_WOUND:
                DoCastVictim(SPELL_INFECTED_WOUND);
                _events.ScheduleEvent(EVENT_INFECTED_WOUND, RandTimeMs(16, 25));
                break;
        }
    }

    void _EnterCombat() OVERRIDE
    {
        _events.ScheduleEvent(EVENT_CRUSHING_WEBS, RandTimeMs(4, 8));
        _events.ScheduleEvent(EVENT_INFECTED_WOUND, RandTimeMs(15, 19));
    }
};

static const Position necromancerWaypoints[] =
{
    { },
    { 507.6937f, 563.3471f, 734.8986f, 4.520403f },
    { 535.1049f, 552.8961f, 732.8441f, 3.996804f }
};
struct npc_anub_ar_crusher_necromancer : public npc_hadronox_crusherPackAI
{
    npc_anub_ar_crusher_necromancer(Creature* creature) : npc_hadronox_crusherPackAI(creature, necromancerWaypoints) { }

    void DoEvent(uint32 eventId) OVERRIDE
    {
        switch (eventId)
        {
            case EVENT_SHADOW_BOLT:
                DoCastVictim(SPELL_SHADOW_BOLT);
                _events.ScheduleEvent(EVENT_SHADOW_BOLT, RandTimeMs(2, 5));
                break;
            case EVENT_ANIMATE_BONES:
                DoCastVictim(urand(0, 1) ? SPELL_ANIMATE_BONES_2 : SPELL_ANIMATE_BONES_1);
                _events.ScheduleEvent(EVENT_ANIMATE_BONES, RandTimeMs(35, 50));
                break;
        }
    }

    void _EnterCombat() OVERRIDE
    {
        _events.ScheduleEvent(EVENT_SHADOW_BOLT, RandTimeMs(2, 4));
        _events.ScheduleEvent(EVENT_ANIMATE_BONES, RandTimeMs(37, 45));
    }
};

static const uint8 NUM_SPAWNS = 3;
static const Position initialMoves[NUM_SPAWNS] =
{
    { 485.314606f, 611.418640f, 771.428406f },
    { 575.760437f, 611.516418f, 771.427368f },
    { 588.930725f, 598.233276f, 739.142151f }
};
static const Position downstairsMoves[NUM_SPAWNS] =
{
    { 513.574341f, 587.022156f, 736.229065f },
    { 537.920410f, 580.436157f, 732.796692f },
    { 601.289246f, 583.259644f, 725.443054f }
};
static const Position downstairsMoves2[NUM_SPAWNS] =
{
    { 571.498718f, 576.978333f, 727.582947f },
    { 571.498718f, 576.978333f, 727.582947f },
    { }
};
struct npc_hadronox_foeAI : public ScriptedAI
{
    npc_hadronox_foeAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()), _nextMovement(MOVE_OUTSIDE), _mySpawn(0) { }

    void InitializeAI() OVERRIDE
    {
        ScriptedAI::InitializeAI();
        if (Creature* hadronox = GetHadronox(me, _instance))
            hadronox->AI()->SetGUID(me->GetGUID());
    }

    void MovementInform(uint32 type, uint32 id) OVERRIDE
    {
        if (type == POINT_MOTION_TYPE)
            _nextMovement = Movements(id + 1);
    }

    void EnterEvadeMode() OVERRIDE
    {
        me->DespawnOrUnsummon();
    }

    virtual void DoEvent(uint32 /*eventId*/) = 0;

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (_nextMovement)
        {
            Movements movement = _nextMovement;
            _nextMovement = MOVE_NONE;

            // A skipped optional waypoint falls through to the walk towards Hadronox
            if (movement == MOVE_DOWNSTAIRS_2 && downstairsMoves2[_mySpawn].GetPositionX() <= 0.0f)
                movement = MOVE_HADRONOX;

            switch (movement)
            {
                case MOVE_OUTSIDE:
                {
                    float dist = FLT_MAX;
                    for (uint8 spawn = 0; spawn < NUM_SPAWNS; ++spawn)
                    {
                        float thisDist = initialMoves[spawn].GetExactDistSq(me);
                        if (thisDist < dist)
                        {
                            _mySpawn = spawn;
                            dist = thisDist;
                        }
                    }
                    me->GetMotionMaster()->MovePoint(MOVE_OUTSIDE, initialMoves[_mySpawn], false); // do not pathfind here, we have to pass through a "wall" of webbing
                    break;
                }
                case MOVE_DOWNSTAIRS:
                    me->GetMotionMaster()->MovePoint(MOVE_DOWNSTAIRS, downstairsMoves[_mySpawn]);
                    break;
                case MOVE_DOWNSTAIRS_2:
                    me->GetMotionMaster()->MovePoint(MOVE_DOWNSTAIRS_2, downstairsMoves2[_mySpawn]);
                    break;
                case MOVE_HADRONOX:
                case MOVE_HADRONOX_REAL:
                {
                    Creature* hadronox = GetHadronox(me, _instance);
                    if (hadronox && hadronox->IsAlive())
                    {
                        if (movement != MOVE_HADRONOX_REAL)
                            if (hadronox->GetPositionZ() < float(FOE_HADRONOX_CUTOFF_Z))
                            {
                                me->GetMotionMaster()->MovePoint(MOVE_HADRONOX, hadronoxStep[2]);
                                break;
                            }
                        AttackStart(hadronox);
                    }
                    break;
                }
                default:
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
            DoEvent(eventId);

        DoMeleeAttackIfReady();
    }

    protected:
        EventMap _events;
        InstanceScript* const _instance;

    private:
        Movements _nextMovement;
        uint8 _mySpawn;
};

struct npc_anub_ar_champion : public npc_hadronox_foeAI
{
    npc_anub_ar_champion(Creature* creature) : npc_hadronox_foeAI(creature) { }

    void DoEvent(uint32 eventId) OVERRIDE
    {
        switch (eventId)
        {
            case EVENT_REND:
                DoCastVictim(SPELL_REND);
                _events.ScheduleEvent(EVENT_REND, RandTimeMs(12, 16));
                break;
            case EVENT_PUMMEL:
                DoCastVictim(SPELL_PUMMEL);
                _events.ScheduleEvent(EVENT_PUMMEL, RandTimeMs(12, 17));
                break;
            case EVENT_TAUNT:
                DoCastVictim(SPELL_TAUNT);
                _events.ScheduleEvent(EVENT_TAUNT, RandTimeMs(15, 50));
                break;
        }
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        _events.ScheduleEvent(EVENT_REND, RandTimeMs(4, 8));
        _events.ScheduleEvent(EVENT_PUMMEL, RandTimeMs(15, 19));
        _events.ScheduleEvent(EVENT_TAUNT, RandTimeMs(15, 50));
    }
};

struct npc_anub_ar_crypt_fiend : public npc_hadronox_foeAI
{
    npc_anub_ar_crypt_fiend(Creature* creature) : npc_hadronox_foeAI(creature) { }

    void DoEvent(uint32 eventId) OVERRIDE
    {
        switch (eventId)
        {
            case EVENT_CRUSHING_WEBS:
                DoCastVictim(SPELL_CRUSHING_WEBS);
                _events.ScheduleEvent(EVENT_CRUSHING_WEBS, RandTimeMs(12, 16));
                break;
            case EVENT_INFECTED_WOUND:
                DoCastVictim(SPELL_INFECTED_WOUND);
                _events.ScheduleEvent(EVENT_INFECTED_WOUND, RandTimeMs(16, 25));
                break;
            case EVENT_TAUNT:
                DoCastVictim(SPELL_TAUNT);
                _events.ScheduleEvent(EVENT_TAUNT, RandTimeMs(15, 50));
                break;
        }
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        _events.ScheduleEvent(EVENT_CRUSHING_WEBS, RandTimeMs(4, 8));
        _events.ScheduleEvent(EVENT_INFECTED_WOUND, RandTimeMs(15, 19));
        _events.ScheduleEvent(EVENT_TAUNT, RandTimeMs(15, 50));
    }
};

struct npc_anub_ar_necromancer : public npc_hadronox_foeAI
{
    npc_anub_ar_necromancer(Creature* creature) : npc_hadronox_foeAI(creature) { }

    void DoEvent(uint32 eventId) OVERRIDE
    {
        switch (eventId)
        {
            case EVENT_SHADOW_BOLT:
                DoCastVictim(SPELL_SHADOW_BOLT);
                _events.ScheduleEvent(EVENT_SHADOW_BOLT, RandTimeMs(2, 5));
                break;
            case EVENT_ANIMATE_BONES:
                DoCastVictim(urand(0, 1) ? SPELL_ANIMATE_BONES_2 : SPELL_ANIMATE_BONES_1);
                _events.ScheduleEvent(EVENT_ANIMATE_BONES, RandTimeMs(35, 50));
                break;
            case EVENT_TAUNT:
                DoCastVictim(SPELL_TAUNT);
                _events.ScheduleEvent(EVENT_TAUNT, RandTimeMs(15, 50));
                break;
        }
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        _events.ScheduleEvent(EVENT_SHADOW_BOLT, RandTimeMs(2, 4));
        _events.ScheduleEvent(EVENT_ANIMATE_BONES, RandTimeMs(37, 45));
        _events.ScheduleEvent(EVENT_TAUNT, RandTimeMs(15, 50));
    }
};

// 53035 - Summon Anub'ar Champion Periodic
// 53036 - Summon Anub'ar Necromancer Periodic
// 53037 - Summon Anub'ar Crypt Fiend Periodic
class spell_hadronox_periodic_summon : public AuraScript
{
    PrepareAuraScript(spell_hadronox_periodic_summon);

    bool Validate(SpellInfo const* /*spell*/) OVERRIDE
    {
        return sSpellMgr->GetSpellInfo(SPELL_SUMMON_CHAMPION_TOP) && sSpellMgr->GetSpellInfo(SPELL_SUMMON_CHAMPION_BOTTOM)
            && sSpellMgr->GetSpellInfo(SPELL_SUMMON_CRYPT_FIEND_TOP) && sSpellMgr->GetSpellInfo(SPELL_SUMMON_CRYPT_FIEND_BOTTOM)
            && sSpellMgr->GetSpellInfo(SPELL_SUMMON_NECROMANCER_TOP) && sSpellMgr->GetSpellInfo(SPELL_SUMMON_NECROMANCER_BOTTOM);
    }

    void HandleApply(AuraEffect const* /*eff*/, AuraEffectHandleModes /*mode*/)
    {
        if (AuraEffect* effect = GetAura()->GetEffect(EFFECT_0))
            effect->SetPeriodicTimer(urand(2, 17) * IN_MILLISECONDS);
    }

    void HandlePeriodic(AuraEffect const* /*eff*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;
        InstanceScript* instance = caster->GetInstanceScript();
        if (!instance)
            return;
        if (!instance->instance->HavePlayers())
            return;
        if (instance->GetBossState(DATA_HADRONOX) == DONE)
        {
            GetAura()->Remove();
            return;
        }

        uint32 topSpellId = 0;
        uint32 bottomSpellId = 0;
        switch (GetId())
        {
            case SPELL_SUMMON_CHAMPION_PERIODIC:
                topSpellId = SPELL_SUMMON_CHAMPION_TOP;
                bottomSpellId = SPELL_SUMMON_CHAMPION_BOTTOM;
                break;
            case SPELL_SUMMON_CRYPT_FIEND_PERIODIC:
                topSpellId = SPELL_SUMMON_CRYPT_FIEND_TOP;
                bottomSpellId = SPELL_SUMMON_CRYPT_FIEND_BOTTOM;
                break;
            case SPELL_SUMMON_NECROMANCER_PERIODIC:
                topSpellId = SPELL_SUMMON_NECROMANCER_TOP;
                bottomSpellId = SPELL_SUMMON_NECROMANCER_BOTTOM;
                break;
            default:
                return;
        }

        caster->CastSpell(caster, caster->GetPositionZ() >= float(PERIODIC_SUMMON_TOP_Z) ? topSpellId : bottomSpellId, true);
    }

    void Register() OVERRIDE
    {
        AfterEffectApply += AuraEffectApplyFn(spell_hadronox_periodic_summon::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_hadronox_periodic_summon::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 53030, 59417 - Leech Poison
class spell_hadronox_leeching_poison : public AuraScript
{
    PrepareAuraScript(spell_hadronox_leeching_poison);

    bool Validate(SpellInfo const* /*spell*/) OVERRIDE
    {
        return sSpellMgr->GetSpellInfo(SPELL_LEECH_POISON_HEAL) != NULL;
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
            return;

        if (GetTarget()->IsGuardian())
            return;

        if (Unit* caster = GetCaster())
            caster->CastSpell(caster, SPELL_LEECH_POISON_HEAL, true);
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_hadronox_leeching_poison::HandleEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_LEECH, AURA_EFFECT_HANDLE_REAL);
    }
};

// 53177 - Web Front Doors
// 53185 - Web Side Door
class spell_hadronox_web_doors : public SpellScript
{
    PrepareSpellScript(spell_hadronox_web_doors);

    bool Validate(SpellInfo const* /*spell*/) OVERRIDE
    {
        return sSpellMgr->GetSpellInfo(SPELL_SUMMON_CHAMPION_PERIODIC) && sSpellMgr->GetSpellInfo(SPELL_SUMMON_CRYPT_FIEND_PERIODIC)
            && sSpellMgr->GetSpellInfo(SPELL_SUMMON_NECROMANCER_PERIODIC);
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
        {
            target->RemoveAurasDueToSpell(SPELL_SUMMON_CHAMPION_PERIODIC);
            target->RemoveAurasDueToSpell(SPELL_SUMMON_CRYPT_FIEND_PERIODIC);
            target->RemoveAurasDueToSpell(SPELL_SUMMON_NECROMANCER_PERIODIC);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_hadronox_web_doors::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    }
};

class achievement_hadronox_denied : public AchievementCriteriaScript
{
    public:
        achievement_hadronox_denied() : AchievementCriteriaScript("achievement_hadronox_denied") { }

        bool OnCheck(Player* /*player*/, Unit* target) OVERRIDE
        {
            if (!target)
                return false;

            // The criteria data type that limits an achievement to heroic is not available in this core
            if (!target->GetMap()->IsHeroic())
                return false;

            if (Creature* cTarget = target->ToCreature())
                if (cTarget->IsAIEnabled && !cTarget->AI()->GetData(DATA_HADRONOX_WEBBED_DOORS))
                    return true;

            return false;
        }
};

void AddSC_boss_hadronox()
{
    RegisterCreatureAI(boss_hadronox);

    RegisterCreatureAI(npc_anub_ar_crusher);
    RegisterCreatureAI(npc_anub_ar_crusher_champion);
    RegisterCreatureAI(npc_anub_ar_crusher_crypt_fiend);
    RegisterCreatureAI(npc_anub_ar_crusher_necromancer);

    RegisterCreatureAI(npc_anub_ar_champion);
    RegisterCreatureAI(npc_anub_ar_crypt_fiend);
    RegisterCreatureAI(npc_anub_ar_necromancer);

    RegisterAuraScript(spell_hadronox_periodic_summon);
    RegisterAuraScript(spell_hadronox_leeching_poison);
    RegisterSpellScript(spell_hadronox_web_doors);

    new achievement_hadronox_denied();
}
