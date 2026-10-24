/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "temple_of_jade_serpent.h"

enum ShaOfDoubtSpells
{
    SPELL_WITHER_WILL                    = 106736,
    SPELL_TOUCH_OF_NOTHINGNESS           = 106113,
    SPELL_BOUNDS_OF_REALITY              = 117665,
    SPELL_FIGMENT_OF_DOUBT               = 106936,
    SPELL_INVISIBILITY_DETECTION         = 126839,

    SPELL_SHADOWFORM                     = 41408,
    SPELL_GROW                           = 104921,
    SPELL_DROWNED_STATE                  = 117509,
    SPELL_DRAW_DOUBT                     = 106290,
    SPELL_GATHERING_DOUBT                = 117570,
    SPELL_GATHERING_DOUBT_STACK          = 117571,
    SPELL_RELEASE_DOUBT                  = 106112,
    SPELL_QUIET_SUICIDE                  = 115372,

    // Figment abilities, picked by the role of the player it copies
    SPELL_PHANTOM_STRIKE                 = 9806,
    SPELL_ARMOR_BUFF                     = 34199,
    SPELL_FURY                           = 15494,
    SPELL_SANCTUARY                      = 69207,
    SPELL_SIPHON_ESSENCE                 = 40291,
    SPELL_STUN                           = 23454,
    SPELL_BLADE_SONG                     = 38282,
    SPELL_GLIMPSE_OF_MADNESS             = 26108,

    SPELL_SEED_OF_DOUBT_ACHIEVEMENT_DONE = 123944,
};

enum ShaOfDoubtEvents
{
    EVENT_WITHER_WILL = 1,
    EVENT_TOUCH_OF_NOTHINGNESS,
    EVENT_BOUNDS_OF_REALITY,

    EVENT_GATHERING_DOUBT,
    EVENT_PHANTOM_STRIKE,
    EVENT_ARMOR_BUFF,
    EVENT_FURY,
    EVENT_SANCTUARY,
    EVENT_SIPHON_ESSENCE,
    EVENT_STUN,
    EVENT_BLADE_SONG,
    EVENT_GLIMPSE_OF_MADNESS,
};

enum ShaOfDoubtTexts
{
    SAY_AGGRO   = 0,
    SAY_DEATH   = 1,
    SAY_FIGMENT = 2,
    SAY_RESET   = 3,
    SAY_SLAY    = 4,
};

enum ShaOfDoubtMisc
{
    ACTION_FIGMENT_DIED      = 0,
    GATHERING_DOUBT_MAX      = 30,
};

float const ShaOfDoubtCombatRange = 53.0f;

enum FigmentRole
{
    FIGMENT_ROLE_DAMAGE,
    FIGMENT_ROLE_TANK,
    FIGMENT_ROLE_HEALER,
};

static FigmentRole GetFigmentRole(Player const* player)
{
    switch (player->GetTalentSpecialization(player->GetActiveSpec()))
    {
        case 66:  // Protection Paladin
        case 73:  // Protection Warrior
        case 104: // Guardian Druid
        case 250: // Blood Death Knight
        case 268: // Brewmaster Monk
            return FIGMENT_ROLE_TANK;
        case 65:  // Holy Paladin
        case 105: // Restoration Druid
        case 256: // Discipline Priest
        case 257: // Holy Priest
        case 264: // Restoration Shaman
        case 270: // Mistweaver Monk
            return FIGMENT_ROLE_HEALER;
        default:
            return FIGMENT_ROLE_DAMAGE;
    }
}

static void DespawnFigments(Creature* sha)
{
    std::list<Creature*> figments;
    sha->GetCreatureListWithEntryInGrid(figments, NPC_FIGMENT_OF_DOUBT, 100.0f);
    for (Creature* figment : figments)
        figment->DespawnOrUnsummon();
}

class boss_sha_of_doubt : public CreatureScript
{
public:
    boss_sha_of_doubt() : CreatureScript("boss_sha_of_doubt") { }

    struct boss_sha_of_doubtAI : public BossAI
    {
        boss_sha_of_doubtAI(Creature* creature) : BossAI(creature, DATA_SHA_OF_DOUBT)
        {
            _figmentsAlive = 0;
        }

        void InitializeAI() OVERRIDE
        {
            DoCast(me, SPELL_INVISIBILITY_DETECTION);
            if (instance->GetBossState(DATA_LIU) != DONE)
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            BossAI::InitializeAI();
        }

        void Reset() OVERRIDE
        {
            _Reset();
            _figmentsAlive = 0;
            me->SetReactState(REACT_DEFENSIVE);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            Talk(SAY_AGGRO);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            instance->SetData(DATA_SHA_OF_DOUBT, IN_PROGRESS);

            events.ScheduleEvent(EVENT_WITHER_WILL, 5 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_TOUCH_OF_NOTHINGNESS, 1 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_BOUNDS_OF_REALITY, urand(8, 12) * IN_MILLISECONDS);
        }

        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_FIGMENT_DIED && _figmentsAlive && --_figmentsAlive == 0)
                me->RemoveAurasDueToSpell(SPELL_BOUNDS_OF_REALITY);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(SAY_SLAY);
        }

        void EnterEvadeMode() OVERRIDE
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            DespawnFigments(me);
            Talk(SAY_RESET);
            BossAI::EnterEvadeMode();
            instance->SetData(DATA_SHA_OF_DOUBT, FAIL);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _JustDied();
            Talk(SAY_DEATH);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            instance->SetData(DATA_SHA_OF_DOUBT, DONE);
            DespawnFigments(me);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (me->GetDistance(me->GetHomePosition()) > ShaOfDoubtCombatRange)
            {
                EnterEvadeMode();
                return;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            bool inFigmentPhase = me->HasAura(SPELL_BOUNDS_OF_REALITY);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WITHER_WILL:
                        if (!inFigmentPhase)
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_WITHER_WILL);
                        events.ScheduleEvent(EVENT_WITHER_WILL, urand(4, 7) * IN_MILLISECONDS);
                        break;
                    case EVENT_TOUCH_OF_NOTHINGNESS:
                        if (!inFigmentPhase)
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_TOUCH_OF_NOTHINGNESS);
                        events.ScheduleEvent(EVENT_TOUCH_OF_NOTHINGNESS, 30 * IN_MILLISECONDS);
                        break;
                    case EVENT_BOUNDS_OF_REALITY:
                    {
                        Talk(SAY_FIGMENT);
                        DoCast(me, SPELL_BOUNDS_OF_REALITY);

                        _figmentsAlive = 0;
                        Map::PlayerList const& players = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        {
                            Player* player = itr->GetSource();
                            if (player->IsAlive() && !player->IsGameMaster())
                            {
                                player->CastSpell(player, SPELL_FIGMENT_OF_DOUBT, false);
                                ++_figmentsAlive;
                            }
                        }
                        events.ScheduleEvent(EVENT_BOUNDS_OF_REALITY, 60 * IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }
            }

            if (!inFigmentPhase)
                DoMeleeAttackIfReady();
        }

    private:
        uint32 _figmentsAlive;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return GetJadeSerpentAI<boss_sha_of_doubtAI>(creature);
    }
};

class mob_figment_of_doubt : public CreatureScript
{
public:
    mob_figment_of_doubt() : CreatureScript("mob_figment_of_doubt") { }

    struct mob_figment_of_doubtAI : public ScriptedAI
    {
        mob_figment_of_doubtAI(Creature* creature) : ScriptedAI(creature)
        {
            _instance = creature->GetInstanceScript();
        }

        void InitializeAI() OVERRIDE
        {
            DoCast(me, SPELL_SHADOWFORM);
            DoCast(me, SPELL_GROW);

            if (me->IsSummon())
                DoCast(SPELL_DRAW_DOUBT);

            ScriptedAI::InitializeAI();
        }

        void Reset() OVERRIDE
        {
            _events.Reset();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            DoCast(me, SPELL_GATHERING_DOUBT);
            _events.ScheduleEvent(EVENT_GATHERING_DOUBT, 1 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_SIPHON_ESSENCE, 8 * IN_MILLISECONDS);

            Player* summoner = GetSummoner();
            if (!summoner)
                return;

            switch (GetFigmentRole(summoner))
            {
                case FIGMENT_ROLE_HEALER:
                    _events.ScheduleEvent(EVENT_PHANTOM_STRIKE, 20 * IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_STUN, 7 * IN_MILLISECONDS);
                    break;
                case FIGMENT_ROLE_TANK:
                    _events.ScheduleEvent(EVENT_ARMOR_BUFF, 10 * IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_SANCTUARY, urand(8, 12) * IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_STUN, 7 * IN_MILLISECONDS);
                    break;
                default:
                    _events.ScheduleEvent(EVENT_FURY, 5 * IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_BLADE_SONG, 13 * IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_GLIMPSE_OF_MADNESS, 8 * IN_MILLISECONDS);
                    break;
            }
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            DoCast(me, SPELL_DROWNED_STATE, true);
            me->RemoveAurasDueToSpell(SPELL_GATHERING_DOUBT);
            me->DespawnOrUnsummon(5 * IN_MILLISECONDS);

            if (Player* summoner = GetSummoner())
                summoner->RemoveAurasDueToSpell(SPELL_DRAW_DOUBT);

            if (_instance)
                if (Creature* sha = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SHA_OF_DOUBT)))
                    if (sha->IsAIEnabled)
                        sha->AI()->DoAction(ACTION_FIGMENT_DIED);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_GATHERING_DOUBT:
                        if (me->GetAuraCount(SPELL_GATHERING_DOUBT_STACK) >= GATHERING_DOUBT_MAX)
                        {
                            me->RemoveAurasDueToSpell(SPELL_GATHERING_DOUBT);
                            DoCast(me, SPELL_RELEASE_DOUBT, true);
                            DoCast(me, SPELL_QUIET_SUICIDE, true);
                        }
                        else
                            _events.ScheduleEvent(EVENT_GATHERING_DOUBT, 1 * IN_MILLISECONDS);
                        break;
                    case EVENT_PHANTOM_STRIKE:
                        DoCastVictim(SPELL_PHANTOM_STRIKE);
                        _events.ScheduleEvent(EVENT_PHANTOM_STRIKE, 20 * IN_MILLISECONDS);
                        break;
                    case EVENT_ARMOR_BUFF:
                        DoCast(me, SPELL_ARMOR_BUFF);
                        _events.ScheduleEvent(EVENT_ARMOR_BUFF, 10 * IN_MILLISECONDS);
                        break;
                    case EVENT_FURY:
                        DoCast(me, SPELL_FURY);
                        _events.ScheduleEvent(EVENT_FURY, 5 * IN_MILLISECONDS);
                        break;
                    case EVENT_SANCTUARY:
                        DoCast(me, SPELL_SANCTUARY);
                        _events.ScheduleEvent(EVENT_SANCTUARY, 10 * IN_MILLISECONDS);
                        break;
                    case EVENT_SIPHON_ESSENCE:
                        DoCast(me, SPELL_SIPHON_ESSENCE);
                        _events.ScheduleEvent(EVENT_SIPHON_ESSENCE, 8 * IN_MILLISECONDS);
                        break;
                    case EVENT_STUN:
                        DoCastVictim(SPELL_STUN);
                        _events.ScheduleEvent(EVENT_STUN, 7 * IN_MILLISECONDS);
                        break;
                    case EVENT_BLADE_SONG:
                        DoCast(me, SPELL_BLADE_SONG);
                        _events.ScheduleEvent(EVENT_BLADE_SONG, 13 * IN_MILLISECONDS);
                        break;
                    case EVENT_GLIMPSE_OF_MADNESS:
                        DoCastVictim(SPELL_GLIMPSE_OF_MADNESS);
                        _events.ScheduleEvent(EVENT_GLIMPSE_OF_MADNESS, 8 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        Player* GetSummoner() const
        {
            if (TempSummon* summon = me->ToTempSummon())
                if (Unit* summoner = summon->GetSummoner())
                    return summoner->ToPlayer();
            return NULL;
        }

        InstanceScript* _instance;
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return GetJadeSerpentAI<mob_figment_of_doubtAI>(creature);
    }
};

class NonPlayerCheck
{
public:
    bool operator()(WorldObject* object) const
    {
        return object->GetTypeId() != TypeID::TYPEID_PLAYER;
    }
};

class NotShaOfDoubtCheck
{
public:
    bool operator()(WorldObject* object) const
    {
        return object->GetEntry() != NPC_SHA_OF_DOUBT;
    }
};

// 106112 - Release Doubt
class spell_sod_release_doubt : public SpellScriptLoader
{
public:
    spell_sod_release_doubt() : SpellScriptLoader("spell_sod_release_doubt") { }

    class spell_sod_release_doubt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sod_release_doubt_SpellScript);

        void FilterPlayers(std::list<WorldObject*>& targets)
        {
            targets.remove_if(NonPlayerCheck());
        }

        void FilterSha(std::list<WorldObject*>& targets)
        {
            targets.remove_if(NotShaOfDoubtCheck());
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sod_release_doubt_SpellScript::FilterPlayers, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sod_release_doubt_SpellScript::FilterSha, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sod_release_doubt_SpellScript();
    }
};

// 123916 - Seed of Doubt
class spell_tjs_seed_of_doubt : public SpellScriptLoader
{
public:
    spell_tjs_seed_of_doubt() : SpellScriptLoader("spell_tjs_seed_of_doubt") { }

    class spell_tjs_seed_of_doubt_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_tjs_seed_of_doubt_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetUnitOwner();
            if (owner && GetStackAmount() > 3 && !owner->HasAura(SPELL_SEED_OF_DOUBT_ACHIEVEMENT_DONE))
                owner->CastSpell(owner, SPELL_SEED_OF_DOUBT_ACHIEVEMENT_DONE, true);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_tjs_seed_of_doubt_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_tjs_seed_of_doubt_AuraScript();
    }
};

void AddSC_boss_sha_of_doubt()
{
    new boss_sha_of_doubt();
    new mob_figment_of_doubt();
    new spell_sod_release_doubt();
    new spell_tjs_seed_of_doubt();
}
