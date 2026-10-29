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
 * Krik'thir waits behind three packs, each led by one of the Watchers. Engaging
 * a pack wakes the whole pack and Krik'thir starts sending the next Watcher
 * towards the players every 70 seconds; when no Watcher is left he joins the
 * fight himself.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "azjol_nerub.h"
#include "Containers.h"
#include "InstanceScript.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "PassiveAI.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "TemporarySummon.h"

enum Events
{
    // Krik'thir the Gatewatcher
    EVENT_SEND_GROUP = 1,
    EVENT_SWARM,
    EVENT_MIND_FLAY,
    EVENT_FRENZY,

    // Watchers - Shared
    EVENT_WEB_WRAP,
    EVENT_INFECTED_BITE,

    // Watcher Gashra
    EVENT_ENRAGE,
    // Watcher Narjil
    EVENT_BLINDING_WEBS,
    // Watcher Silthik
    EVENT_POISON_SPRAY,

    // Anubar Skirmisher
    EVENT_ANUBAR_CHARGE,
    EVENT_BACKSTAB,

    // Anubar Shadowcaster
    EVENT_SHADOW_BOLT,
    EVENT_SHADOW_NOVA,

    // Anubar Warrior
    EVENT_STRIKE,
    EVENT_CLEAVE
};

enum Spells
{
    // Krik'thir the Gatewatcher
    SPELL_SUBBOSS_AGGRO_TRIGGER     = 52343,
    SPELL_SWARM                     = 52440,
    SPELL_MIND_FLAY                 = 52586,
    SPELL_MIND_FLAY_HEROIC          = 59367,
    SPELL_CURSE_OF_FATIGUE          = 52592,
    SPELL_CURSE_OF_FATIGUE_HEROIC   = 59368,
    SPELL_FRENZY                    = 28747,

    // Watchers - Shared
    SPELL_WEB_WRAP                  = 52086,
    SPELL_WEB_WRAP_WRAPPED          = 52087,
    SPELL_INFECTED_BITE             = 52469,
    SPELL_INFECTED_BITE_HEROIC      = 59364,

    // Watcher Gashra
    SPELL_ENRAGE                    = 52470,
    // Watcher Narjil
    SPELL_BLINDING_WEBS             = 52524,
    SPELL_BLINDING_WEBS_HEROIC      = 59365,
    // Watcher Silthik
    SPELL_POISON_SPRAY              = 52493,
    SPELL_POISON_SPRAY_HEROIC       = 59366,

    // Anub'ar Warrior
    SPELL_CLEAVE                    = 49806,
    SPELL_STRIKE                    = 52532,

    // Anub'ar Skirmisher
    SPELL_CHARGE                    = 52538,
    SPELL_BACKSTAB                  = 52540,
    SPELL_FIXATE_TRIGGER            = 52536,
    SPELL_FIXATE_TRIGGERED          = 52537,

    // Anub'ar Shadowcaster
    SPELL_SHADOW_BOLT               = 52534,
    SPELL_SHADOW_BOLT_HEROIC        = 59357,
    SPELL_SHADOW_NOVA               = 52535,
    SPELL_SHADOW_NOVA_HEROIC        = 59358,

    // Skittering Infector
    SPELL_ACID_SPLASH               = 52446,
    SPELL_ACID_SPLASH_HEROIC        = 59363
};

enum GatewatcherData
{
    DATA_PET_GROUP = 1
};

enum GatewatcherCreatures
{
    NPC_ANUB_AR_WARRIOR     = 28732,
    NPC_ANUB_AR_SHADOWCASTER = 28733,
    NPC_ANUB_AR_SKIRMISHER  = 28734
};

enum Actions
{
    ACTION_GASHRA_DIED = 1,
    ACTION_NARJIL_DIED,
    ACTION_SILTHIK_DIED,
    ACTION_WATCHER_ENGAGED,
    ACTION_PET_ENGAGED,
    ACTION_PET_EVADE
};

enum Yells
{
    SAY_AGGRO       = 0,
    SAY_SLAY        = 1,
    SAY_DEATH       = 2,
    SAY_SWARM       = 3,
    SAY_PREFIGHT    = 4,
    SAY_SEND_GROUP  = 5
};

static uint32 const PET_ASSIST_RANGE = 40;  // yards
static uint32 const SEND_GROUP_TIMER = 70;  // seconds

static uint32 RandTimeMs(uint32 minSeconds, uint32 maxSeconds)
{
    return urand(minSeconds * IN_MILLISECONDS, maxSeconds * IN_MILLISECONDS);
}

static Creature* GetGatewatcher(Creature* me, InstanceScript* instance)
{
    if (!instance)
        return NULL;
    return Unit::GetCreature(*me, instance->GetData64(DATA_KRIKTHIR_THE_GATEWATCHER));
}

// Passive creatures get no line of sight callbacks from the grid, so the waiting packs look for players themselves
static Player* FindPlayerInReach(Creature* me)
{
    Map::PlayerList const& players = me->GetMap()->GetPlayers();
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        Player* player = itr->GetSource();
        if (player && player->IsAlive() && me->CanStartAttack(player, false) && me->IsWithinLOSInMap(player))
            return player;
    }
    return NULL;
}

struct boss_krik_thir : public BossAI
{
    boss_krik_thir(Creature* creature) : BossAI(creature, DATA_KRIKTHIR_THE_GATEWATCHER), _hadFrenzy(false), _petsInCombat(false), _summonAdds(false), _watchersActive(0), _proximityTimer(0) { }

    void SummonAdds()
    {
        if (instance->GetBossState(DATA_KRIKTHIR_THE_GATEWATCHER) == DONE)
            return;

        for (uint8 i = 1; i <= 3; ++i)
        {
            std::list<TempSummon*> adds;
            me->SummonCreatureGroup(i, &adds);
            for (std::list<TempSummon*>::iterator itr = adds.begin(); itr != adds.end(); ++itr)
                (*itr)->AI()->SetData(DATA_PET_GROUP, i);
        }
    }

    void Reset() OVERRIDE
    {
        BossAI::Reset();
        _hadFrenzy = false;
        _petsInCombat = false;
        _watchersActive = 0;
        me->SetReactState(REACT_PASSIVE);
        // The packs cannot be summoned while the creature is still being added to the map
        _summonAdds = true;
    }

    void KilledUnit(Unit* victim) OVERRIDE
    {
        if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
            Talk(SAY_SLAY);
    }

    // The Watchers have to stay alive for Watch Him Die, so the summon list is not despawned here
    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        events.Reset();
        instance->SetBossState(DATA_KRIKTHIR_THE_GATEWATCHER, DONE);
        instance->SaveToDB();
        Talk(SAY_DEATH);
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        _petsInCombat = false;
        me->SetReactState(REACT_AGGRESSIVE);
        summons.DoZoneInCombat();

        events.CancelEvent(EVENT_SEND_GROUP);
        events.ScheduleEvent(EVENT_SWARM, 5 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_MIND_FLAY, RandTimeMs(1, 3));

        BossAI::EnterCombat(who);
    }

    void EnterEvadeMode() OVERRIDE
    {
        summons.DespawnAll();
        _DespawnAtEvade();
    }

    void DoAction(int32 action) OVERRIDE
    {
        if (!me->IsAlive())
            return;

        switch (action)
        {
            case ACTION_GASHRA_DIED:
            case ACTION_NARJIL_DIED:
            case ACTION_SILTHIK_DIED:
                if (!_watchersActive) // something is wrong
                {
                    EnterEvadeMode();
                    return;
                }
                if (!--_watchersActive) // if there are no watchers currently in combat...
                    events.RescheduleEvent(EVENT_SEND_GROUP, 5 * IN_MILLISECONDS); // ...send the next watcher after the targets sooner
                break;
            case ACTION_WATCHER_ENGAGED:
                ++_watchersActive;
                break;
            case ACTION_PET_ENGAGED:
                if (_petsInCombat || me->IsInCombat())
                    break;
                _petsInCombat = true;
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_SEND_GROUP, SEND_GROUP_TIMER * IN_MILLISECONDS);
                break;
            case ACTION_PET_EVADE:
                EnterEvadeMode();
                break;
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (_summonAdds)
        {
            _summonAdds = false;
            SummonAdds();
        }

        if (me->HasReactState(REACT_PASSIVE) && !me->IsInCombat())
        {
            if (_proximityTimer > diff)
                _proximityTimer -= diff;
            else
            {
                _proximityTimer = 1 * IN_MILLISECONDS;
                if (Player* player = FindPlayerInReach(me))
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    AttackStart(player);
                }
            }
        }

        if (!UpdateVictim() && !_petsInCombat)
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        if (me->HealthBelowPct(10) && !_hadFrenzy)
        {
            _hadFrenzy = true;
            events.ScheduleEvent(EVENT_FRENZY, 1 * IN_MILLISECONDS);
        }

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_SEND_GROUP:
                    DoCastAOE(SPELL_SUBBOSS_AGGRO_TRIGGER, true);
                    events.ScheduleEvent(EVENT_SEND_GROUP, SEND_GROUP_TIMER * IN_MILLISECONDS);
                    break;

                case EVENT_SWARM:
                    DoCastAOE(SPELL_SWARM);
                    Talk(SAY_SWARM);
                    break;

                case EVENT_MIND_FLAY:
                    DoCastVictim(DUNGEON_MODE(SPELL_MIND_FLAY, SPELL_MIND_FLAY_HEROIC));
                    events.ScheduleEvent(EVENT_MIND_FLAY, RandTimeMs(9, 11));
                    break;

                case EVENT_FRENZY:
                    DoCast(me, SPELL_FRENZY);
                    DoCastVictim(DUNGEON_MODE(SPELL_CURSE_OF_FATIGUE, SPELL_CURSE_OF_FATIGUE_HEROIC));
                    events.ScheduleEvent(EVENT_FRENZY, 15 * IN_MILLISECONDS);
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spellInfo) OVERRIDE
    {
        if (spellInfo->Id == SPELL_SUBBOSS_AGGRO_TRIGGER)
            DoZoneInCombat();
    }

    void SpellHitTarget(Unit* /*target*/, SpellInfo const* spellInfo) OVERRIDE
    {
        if (spellInfo->Id == SPELL_SUBBOSS_AGGRO_TRIGGER)
            Talk(SAY_SEND_GROUP);
    }

    private:
        bool _hadFrenzy;
        bool _petsInCombat;
        bool _summonAdds;
        uint8 _watchersActive;
        uint32 _proximityTimer;
};

struct npc_gatewatcher_petAI : public ScriptedAI
{
    npc_gatewatcher_petAI(Creature* creature, bool isWatcher) : ScriptedAI(creature), _instance(creature->GetInstanceScript()), _petGroup(0), _isWatcher(isWatcher), _engaged(false), _proximityTimer(0) { }

    virtual void _EnterCombat() = 0;

    void AssistPack(Unit* who)
    {
        static const uint32 packEntries[] = { NPC_WATCHER_NARJIL, NPC_WATCHER_GASHRA, NPC_WATCHER_SILTHIK, NPC_ANUB_AR_WARRIOR, NPC_ANUB_AR_SHADOWCASTER, NPC_ANUB_AR_SKIRMISHER };

        for (uint8 i = 0; i < sizeof(packEntries) / sizeof(packEntries[0]); ++i)
        {
            std::list<Creature*> others;
            me->GetCreatureListWithEntryInGrid(others, packEntries[i], float(PET_ASSIST_RANGE));
            for (std::list<Creature*>::iterator itr = others.begin(); itr != others.end(); ++itr)
            {
                Creature* other = *itr;
                if (other == me || !other->IsAlive() || !other->AI() || other->AI()->GetData(DATA_PET_GROUP) != _petGroup)
                    continue;
                other->SetReactState(REACT_AGGRESSIVE);
                other->AI()->AttackStart(who);
            }
        }
    }

    // Called by the engine when the pet enters combat and by the player check below for a passive pack
    void EnterCombat(Unit* who) OVERRIDE
    {
        if (_engaged)
            return;
        _engaged = true;

        Creature* gatewatcher = GetGatewatcher(me, _instance);

        if (_isWatcher)
        {
            _isWatcher = false;
            if (gatewatcher)
                gatewatcher->AI()->DoAction(ACTION_WATCHER_ENGAGED);
        }

        if (me->HasReactState(REACT_PASSIVE))
        {
            AssistPack(who);
            me->SetReactState(REACT_AGGRESSIVE);
            if (gatewatcher)
                gatewatcher->AI()->DoAction(ACTION_PET_ENGAGED);
            AttackStart(who);
        }
        _EnterCombat();
    }

    void Reset() OVERRIDE
    {
        _events.Reset();
        _engaged = false;
    }

    void SetData(uint32 data, uint32 value) OVERRIDE
    {
        if (data == DATA_PET_GROUP)
        {
            _petGroup = value;
            me->SetReactState(_petGroup ? REACT_PASSIVE : REACT_AGGRESSIVE);
        }
    }

    uint32 GetData(uint32 data) const OVERRIDE
    {
        if (data == DATA_PET_GROUP)
            return _petGroup;
        return 0;
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spellInfo) OVERRIDE
    {
        if (spellInfo->Id == SPELL_SUBBOSS_AGGRO_TRIGGER)
            DoZoneInCombat();
    }

    void EnterEvadeMode() OVERRIDE
    {
        if (TempSummon* meSummon = me->ToTempSummon())
        {
            Creature* summoner = meSummon->GetSummonerCreatureBase();
            if (summoner && summoner->IsAlive())
                summoner->AI()->DoAction(ACTION_PET_EVADE);
            else
                me->DespawnOrUnsummon();
            return;
        }
        ScriptedAI::EnterEvadeMode();
    }

    void CheckForPlayers(uint32 diff)
    {
        if (_proximityTimer > diff)
        {
            _proximityTimer -= diff;
            return;
        }
        _proximityTimer = 1 * IN_MILLISECONDS;

        if (Player* player = FindPlayerInReach(me))
            EnterCombat(player);
    }

    // Returns false while the pet has nothing to fight
    bool UpdateCombat(uint32 diff)
    {
        if (!_engaged && me->HasReactState(REACT_PASSIVE))
            CheckForPlayers(diff);

        if (!UpdateVictim())
            return false;

        _events.Update(diff);

        return !me->HasUnitState(UNIT_STATE_CASTING);
    }

    protected:
        EventMap _events;
        InstanceScript* _instance;
        uint32 _petGroup;
        bool _isWatcher;
        bool _engaged;
        uint32 _proximityTimer;
};

struct npc_watcher_gashra : public npc_gatewatcher_petAI
{
    npc_watcher_gashra(Creature* creature) : npc_gatewatcher_petAI(creature, true) { }

    void _EnterCombat() OVERRIDE
    {
        _events.ScheduleEvent(EVENT_ENRAGE, RandTimeMs(3, 5));
        _events.ScheduleEvent(EVENT_WEB_WRAP, RandTimeMs(16, 19));
        _events.ScheduleEvent(EVENT_INFECTED_BITE, RandTimeMs(7, 11));
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        Creature* krikthir = GetGatewatcher(me, _instance);
        if (krikthir && krikthir->IsAlive())
            krikthir->AI()->DoAction(ACTION_GASHRA_DIED);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateCombat(diff))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_ENRAGE:
                    DoCast(me, SPELL_ENRAGE);
                    _events.ScheduleEvent(EVENT_ENRAGE, RandTimeMs(12, 20));
                    break;
                case EVENT_WEB_WRAP:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f))
                        DoCast(target, SPELL_WEB_WRAP);
                    _events.ScheduleEvent(EVENT_WEB_WRAP, RandTimeMs(13, 19));
                    break;
                case EVENT_INFECTED_BITE:
                    DoCastVictim(DUNGEON_MODE(SPELL_INFECTED_BITE, SPELL_INFECTED_BITE_HEROIC));
                    _events.ScheduleEvent(EVENT_INFECTED_BITE, RandTimeMs(23, 27));
                    break;
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }
};

struct npc_watcher_narjil : public npc_gatewatcher_petAI
{
    npc_watcher_narjil(Creature* creature) : npc_gatewatcher_petAI(creature, true) { }

    void _EnterCombat() OVERRIDE
    {
        _events.ScheduleEvent(EVENT_BLINDING_WEBS, RandTimeMs(13, 18));
        _events.ScheduleEvent(EVENT_WEB_WRAP, RandTimeMs(3, 5));
        _events.ScheduleEvent(EVENT_INFECTED_BITE, RandTimeMs(7, 11));
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        Creature* krikthir = GetGatewatcher(me, _instance);
        if (krikthir && krikthir->IsAlive())
            krikthir->AI()->DoAction(ACTION_NARJIL_DIED);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateCombat(diff))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_BLINDING_WEBS:
                    DoCastVictim(DUNGEON_MODE(SPELL_BLINDING_WEBS, SPELL_BLINDING_WEBS_HEROIC));
                    _events.ScheduleEvent(EVENT_BLINDING_WEBS, RandTimeMs(23, 27));
                    break;
                case EVENT_WEB_WRAP:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        DoCast(target, SPELL_WEB_WRAP);
                    _events.ScheduleEvent(EVENT_WEB_WRAP, RandTimeMs(13, 19));
                    break;
                case EVENT_INFECTED_BITE:
                    DoCastVictim(DUNGEON_MODE(SPELL_INFECTED_BITE, SPELL_INFECTED_BITE_HEROIC));
                    _events.ScheduleEvent(EVENT_INFECTED_BITE, RandTimeMs(20, 25));
                    break;
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }
};

struct npc_watcher_silthik : public npc_gatewatcher_petAI
{
    npc_watcher_silthik(Creature* creature) : npc_gatewatcher_petAI(creature, true) { }

    void _EnterCombat() OVERRIDE
    {
        _events.ScheduleEvent(EVENT_POISON_SPRAY, RandTimeMs(16, 19));
        _events.ScheduleEvent(EVENT_WEB_WRAP, RandTimeMs(7, 11));
        _events.ScheduleEvent(EVENT_INFECTED_BITE, RandTimeMs(3, 5));
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        Creature* krikthir = GetGatewatcher(me, _instance);
        if (krikthir && krikthir->IsAlive())
            krikthir->AI()->DoAction(ACTION_SILTHIK_DIED);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateCombat(diff))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_POISON_SPRAY:
                    DoCastVictim(DUNGEON_MODE(SPELL_POISON_SPRAY, SPELL_POISON_SPRAY_HEROIC));
                    _events.ScheduleEvent(EVENT_POISON_SPRAY, RandTimeMs(13, 19));
                    break;
                case EVENT_WEB_WRAP:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        DoCast(target, SPELL_WEB_WRAP);
                    _events.ScheduleEvent(EVENT_WEB_WRAP, RandTimeMs(13, 17));
                    break;
                case EVENT_INFECTED_BITE:
                    DoCastVictim(DUNGEON_MODE(SPELL_INFECTED_BITE, SPELL_INFECTED_BITE_HEROIC));
                    _events.ScheduleEvent(EVENT_INFECTED_BITE, RandTimeMs(20, 24));
                    break;
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }
};

struct npc_anub_ar_warrior : public npc_gatewatcher_petAI
{
    npc_anub_ar_warrior(Creature* creature) : npc_gatewatcher_petAI(creature, false) { }

    void _EnterCombat() OVERRIDE
    {
        _events.ScheduleEvent(EVENT_CLEAVE, RandTimeMs(7, 9));
        _events.ScheduleEvent(EVENT_STRIKE, RandTimeMs(5, 10));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateCombat(diff))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CLEAVE:
                    DoCastVictim(SPELL_CLEAVE);
                    _events.ScheduleEvent(EVENT_CLEAVE, RandTimeMs(10, 16));
                    break;
                case EVENT_STRIKE:
                    DoCastVictim(SPELL_STRIKE);
                    _events.ScheduleEvent(EVENT_STRIKE, RandTimeMs(15, 19));
                    break;
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }
};

struct npc_anub_ar_skirmisher : public npc_gatewatcher_petAI
{
    npc_anub_ar_skirmisher(Creature* creature) : npc_gatewatcher_petAI(creature, false) { }

    void _EnterCombat() OVERRIDE
    {
        _events.ScheduleEvent(EVENT_ANUBAR_CHARGE, RandTimeMs(6, 8));
        _events.ScheduleEvent(EVENT_BACKSTAB, RandTimeMs(7, 9));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateCombat(diff))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_ANUBAR_CHARGE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        DoCast(target, SPELL_CHARGE);
                    _events.ScheduleEvent(EVENT_ANUBAR_CHARGE, RandTimeMs(20, 25));
                    break;
                case EVENT_BACKSTAB:
                    if (me->GetVictim() && me->GetVictim()->isInBack(me))
                        DoCastVictim(SPELL_BACKSTAB);
                    _events.ScheduleEvent(EVENT_BACKSTAB, RandTimeMs(10, 13));
                    break;
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }

    void SpellHitTarget(Unit* target, SpellInfo const* spellInfo) OVERRIDE
    {
        if (target && spellInfo->Id == SPELL_CHARGE)
            DoCast(target, SPELL_FIXATE_TRIGGER);
    }
};

struct npc_anub_ar_shadowcaster : public npc_gatewatcher_petAI
{
    npc_anub_ar_shadowcaster(Creature* creature) : npc_gatewatcher_petAI(creature, false) { }

    void _EnterCombat() OVERRIDE
    {
        _events.ScheduleEvent(EVENT_SHADOW_BOLT, 4 * IN_MILLISECONDS);
        _events.ScheduleEvent(EVENT_SHADOW_NOVA, RandTimeMs(10, 14));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateCombat(diff))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_SHADOW_BOLT:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        DoCast(target, DUNGEON_MODE(SPELL_SHADOW_BOLT, SPELL_SHADOW_BOLT_HEROIC));
                    _events.ScheduleEvent(EVENT_SHADOW_BOLT, RandTimeMs(2, 4));
                    break;
                case EVENT_SHADOW_NOVA:
                    DoCastVictim(DUNGEON_MODE(SPELL_SHADOW_NOVA, SPELL_SHADOW_NOVA_HEROIC));
                    _events.ScheduleEvent(EVENT_SHADOW_NOVA, RandTimeMs(10, 16));
                    break;
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }
};

struct npc_skittering_swarmer : public ScriptedAI
{
    npc_skittering_swarmer(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        if (Creature* gatewatcher = GetGatewatcher(me, me->GetInstanceScript()))
        {
            if (Unit* target = gatewatcher->getAttackerForHelper())
                AttackStart(target);
            gatewatcher->AI()->JustSummoned(me);
        }
    }
};

struct npc_skittering_infector : public ScriptedAI
{
    npc_skittering_infector(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        if (Creature* gatewatcher = GetGatewatcher(me, me->GetInstanceScript()))
        {
            if (Unit* target = gatewatcher->getAttackerForHelper())
                AttackStart(target);
            gatewatcher->AI()->JustSummoned(me);
        }
    }

    void JustDied(Unit* killer) OVERRIDE
    {
        DoCastAOE(DUNGEON_MODE(SPELL_ACID_SPLASH, SPELL_ACID_SPLASH_HEROIC));
        ScriptedAI::JustDied(killer);
    }
};

struct npc_gatewatcher_web_wrap : public NullCreatureAI
{
    npc_gatewatcher_web_wrap(Creature* creature) : NullCreatureAI(creature) { }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (TempSummon* meSummon = me->ToTempSummon())
            if (Unit* summoner = meSummon->GetSummoner())
                summoner->RemoveAurasDueToSpell(SPELL_WEB_WRAP_WRAPPED);
    }
};

// 52343 - Krik'Thir Subboss Aggro Trigger
class spell_gatewatcher_subboss_trigger : public SpellScript
{
    PrepareSpellScript(spell_gatewatcher_subboss_trigger);

    void HandleTargets(std::list<WorldObject*>& targetList)
    {
        // Remove any Watchers that are already in combat
        std::list<WorldObject*>::iterator it = targetList.begin();
        while (it != targetList.end())
        {
            if (Creature* creature = (*it)->ToCreature())
                if (creature->IsAlive() && !creature->IsInCombat())
                {
                    ++it;
                    continue;
                }
            it = targetList.erase(it);
        }

        // Default to Krik'thir himself if he isn't engaged
        WorldObject* target = NULL;
        if (GetCaster() && !GetCaster()->IsInCombat())
            target = GetCaster();
        // Unless there are Watchers that aren't engaged yet
        if (!targetList.empty())
        {
            // If there are, pick one of them at random
            target = Skyfire::Containers::SelectRandomContainerElement(targetList);
        }
        // And hit only that one
        targetList.clear();
        if (target)
            targetList.push_back(target);
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gatewatcher_subboss_trigger::HandleTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

// 52536 - Fixate Trigger
class spell_anub_ar_skirmisher_fixate : public SpellScript
{
    PrepareSpellScript(spell_anub_ar_skirmisher_fixate);

    bool Validate(SpellInfo const* /*spell*/) OVERRIDE
    {
        return sSpellMgr->GetSpellInfo(SPELL_FIXATE_TRIGGERED) != NULL;
    }

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            target->CastSpell(GetCaster(), SPELL_FIXATE_TRIGGERED, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_anub_ar_skirmisher_fixate::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 52086 - Web Wrap
class spell_gatewatcher_web_wrap : public AuraScript
{
    PrepareAuraScript(spell_gatewatcher_web_wrap);

    bool Validate(SpellInfo const* /*spell*/) OVERRIDE
    {
        return sSpellMgr->GetSpellInfo(SPELL_WEB_WRAP_WRAPPED) != NULL;
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
            return;

        if (Unit* target = GetTarget())
            target->CastSpell(target, SPELL_WEB_WRAP_WRAPPED, true);
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_gatewatcher_web_wrap::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_ROOT, AURA_EFFECT_HANDLE_REAL);
    }
};

class achievement_watch_him_die : public AchievementCriteriaScript
{
    public:
        achievement_watch_him_die() : AchievementCriteriaScript("achievement_watch_him_die")
        {
        }

        bool OnCheck(Player* /*player*/, Unit* target) OVERRIDE
        {
            if (!target)
                return false;

            InstanceScript* instance = target->GetInstanceScript();
            if (!instance)
                return false;

            for (uint8 n = 0; n < 3; ++n)
            {
                if (Creature* watcher = ObjectAccessor::GetCreature(*target, instance->GetData64(DATA_WATCHER_GASHRA + n)))
                    if (watcher->IsAlive())
                        continue;
                return false;
            }

            return true;
        }
};

void AddSC_boss_krik_thir()
{
    RegisterCreatureAI(boss_krik_thir);

    RegisterCreatureAI(npc_watcher_gashra);
    RegisterCreatureAI(npc_watcher_narjil);
    RegisterCreatureAI(npc_watcher_silthik);

    RegisterCreatureAI(npc_anub_ar_warrior);
    RegisterCreatureAI(npc_anub_ar_skirmisher);
    RegisterCreatureAI(npc_anub_ar_shadowcaster);

    RegisterCreatureAI(npc_skittering_swarmer);
    RegisterCreatureAI(npc_skittering_infector);
    RegisterCreatureAI(npc_gatewatcher_web_wrap);

    RegisterSpellScript(spell_gatewatcher_subboss_trigger);
    RegisterSpellScript(spell_anub_ar_skirmisher_fixate);
    RegisterAuraScript(spell_gatewatcher_web_wrap);

    new achievement_watch_him_die();
}
