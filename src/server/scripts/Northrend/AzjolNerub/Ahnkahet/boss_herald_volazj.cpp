/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ahnkahet.h"
#include "Player.h"
#include "SpellInfo.h"
#include "TemporarySummon.h"

enum Spells
{
    SPELL_INSANITY                                = 57496, //Dummy
    INSANITY_VISUAL                               = 57561,
    SPELL_INSANITY_TARGET                         = 57508,
    SPELL_MIND_FLAY                               = 57941,
    SPELL_SHADOW_BOLT_VOLLEY                      = 57942,
    SPELL_SHIVER                                  = 57949,
    SPELL_CLONE_PLAYER                            = 57507, //casted on player during insanity
    SPELL_INSANITY_PHASING_1                      = 57508,
    SPELL_INSANITY_PHASING_2                      = 57509,
    SPELL_INSANITY_PHASING_3                      = 57510,
    SPELL_INSANITY_PHASING_4                      = 57511,
    SPELL_INSANITY_PHASING_5                      = 57512,

    SPELL_TWISTED_VISAGE_VISUAL                   = 57551,
    SPELL_TWISTED_VISAGE_DEATH                    = 57555,

    // Death Knight
    SPELL_TWISTED_VISAGE_DEATH_GRIP               = 57602,
    SPELL_TWISTED_VISAGE_PLAGUE_STRIKE            = 57599,
    // Druid
    SPELL_TWISTED_VISAGE_WRATH                    = 57648,
    SPELL_TWISTED_VISAGE_MOONFIRE                 = 57647,
    SPELL_TWISTED_VISAGE_LIFEBLOOM                = 57762,
    SPELL_TWISTED_VISAGE_CAT_FORM                 = 57655,
    SPELL_TWISTED_VISAGE_MANGLE                   = 57657,
    SPELL_TWISTED_VISAGE_RIP                      = 57661,
    SPELL_TWISTED_VISAGE_NOURISH                  = 57765,
    // Hunter
    SPELL_TWISTED_VISAGE_SHOOT                    = 57589,
    SPELL_TWISTED_VISAGE_DISENGAGE                = 57635,
    // Mage
    SPELL_TWISTED_VISAGE_FIREBALL                 = 57628,
    SPELL_TWISTED_VISAGE_FROST_NOVA               = 57629,
    // Paladin
    SPELL_TWISTED_VISAGE_CONSECRATION             = 57798,
    SPELL_TWISTED_VISAGE_AVENGERS_SHIELD          = 57799,
    SPELL_TWISTED_VISAGE_SEAL_OF_COMMAND          = 57769,
    SPELL_TWISTED_VISAGE_JUDGEMENT_OF_LIGHT       = 57774,
    // Priest
    SPELL_TWISTED_VISAGE_GREATER_HEAL             = 57775,
    SPELL_TWISTED_VISAGE_RENEW                    = 57777,
    SPELL_TWISTED_VISAGE_SHADOW_WORD_PAIN         = 57778,
    SPELL_TWISTED_VISAGE_MIND_FLAY                = 57779,
    // Rogue
    SPELL_TWISTED_VISAGE_EVISCERATE               = 57641,
    SPELL_TWISTED_VISAGE_SINISTER_STRIKE          = 57640,
    // Shaman
    SPELL_TWISTED_VISAGE_EARTH_SHOCK              = 57783,
    SPELL_TWISTED_VISAGE_LIGHTNING_BOLT           = 57781,
    SPELL_TWISTED_VISAGE_EARTH_SHIELD             = 57802,
    SPELL_TWISTED_VISAGE_HEALING_WAVE             = 57785,
    SPELL_TWISTED_VISAGE_THUNDERSTORM             = 57784,
    // Warlock
    SPELL_TWISTED_VISAGE_CORRUPTION               = 57645,
    SPELL_TWISTED_VISAGE_SHADOW_BOLT              = 57644,
    // Warrior
    SPELL_TWISTED_VISAGE_THUNDER_CLAP             = 57832,
    SPELL_TWISTED_VISAGE_DEVASTATE                = 57795,
    SPELL_TWISTED_VISAGE_MORTAL_STRIKE            = 57789,
    SPELL_TWISTED_VISAGE_INTERCEPT                = 61490,
    SPELL_TWISTED_VISAGE_HAMSTRING                = 9080,
    SPELL_TWISTED_VISAGE_BLOODTHIRST              = 57790
};

enum Yells
{
    SAY_AGGRO   = 0,
    SAY_SLAY    = 1,
    SAY_DEATH   = 2,
    SAY_PHASE   = 3
};

enum Achievements
{
    ACHIEV_QUICK_DEMISE_START_EVENT               = 20382,
};

enum VisageData
{
    DATA_TWISTED_VISAGE_PLAYER_CLASS              = 1,
    DATA_TWISTED_VISAGE_PLAYER_SPEC               = 2
};

class boss_volazj : public CreatureScript
{
public:
    boss_volazj() : CreatureScript("boss_volazj") { }

    struct boss_volazjAI : public ScriptedAI
    {
        boss_volazjAI(Creature* creature) : ScriptedAI(creature), Summons(me)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 uiMindFlayTimer;
        uint32 uiShadowBoltVolleyTimer;
        uint32 uiShiverTimer;
        uint32 insanityHandled;
        SummonList Summons;

        // returns the percentage of health after taking the given damage.
        uint32 GetHealthPct(uint32 damage)
        {
            if (damage > me->GetHealth())
                return 0;
            return 100*(me->GetHealth()-damage)/me->GetMaxHealth();
        }

        void DamageTaken(Unit* /*pAttacker*/, uint32 &damage) OVERRIDE
        {
            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
                damage = 0;

            if ((GetHealthPct(0) >= 66 && GetHealthPct(damage) < 66)||
                (GetHealthPct(0) >= 33 && GetHealthPct(damage) < 33))
            {
                me->InterruptNonMeleeSpells(false);
                DoCast(me, SPELL_INSANITY, false);
            }
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell) OVERRIDE
        {
            if (spell->Id == SPELL_INSANITY)
            {
                // Not good target or too many players
                if (target->GetTypeId() != TypeID::TYPEID_PLAYER || insanityHandled > 4)
                    return;
                // First target - start channel visual and set self as unnattackable
                if (!insanityHandled)
                {
                    // Channel visual
                    DoCast(me, INSANITY_VISUAL, true);
                    // Unattackable
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->SetControlled(true, UNIT_STATE_STUNNED);
                }
                // phase mask
                target->CastSpell(target, SPELL_INSANITY_TARGET+insanityHandled, true);
                // summon twisted party members for this target
                Map::PlayerList const &players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                {
                    Player* player = i->GetSource();
                    if (!player || !player->IsAlive())
                        continue;
                    // Summon clone
                    if (Creature* summon = me->SummonCreature(NPC_TWISTED_VISAGE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_CORPSE_DESPAWN, 0))
                    {
                        // clone
                        player->CastSpell(summon, SPELL_CLONE_PLAYER, true);
                        summon->AI()->SetData(DATA_TWISTED_VISAGE_PLAYER_CLASS, player->getClass());
                        summon->AI()->SetData(DATA_TWISTED_VISAGE_PLAYER_SPEC, player->GetTalentSpecialization(player->GetActiveSpec()));
                        summon->SetReactState(REACT_AGGRESSIVE);
                        DoZoneInCombat(summon);
                        // set phase
                        summon->SetPhaseMask((1<<(4+insanityHandled)), true);
                    }
                }
                ++insanityHandled;
            }
        }

        void ResetPlayersPhaseMask()
        {
            Map::PlayerList const &players = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
            {
                Player* player = i->GetSource();
                player->RemoveAurasDueToSpell(GetSpellForPhaseMask(player->GetPhaseMask()));
            }
        }

        void Reset() OVERRIDE
        {
            uiMindFlayTimer = 8*IN_MILLISECONDS;
            uiShadowBoltVolleyTimer = 5*IN_MILLISECONDS;
            uiShiverTimer = 15*IN_MILLISECONDS;

            if (instance)
            {
                instance->SetBossState(DATA_HERALD_VOLAZJ, NOT_STARTED);
                instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_QUICK_DEMISE_START_EVENT);
            }

            // Visible for all players in insanity
            me->SetPhaseMask((1|16|32|64|128|256), true);
            // Used for Insanity handling
            insanityHandled = 0;

            ResetPlayersPhaseMask();

            // Cleanup
            Summons.DespawnAll();
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetControlled(false, UNIT_STATE_STUNNED);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);

            if (instance)
            {
                instance->SetBossState(DATA_HERALD_VOLAZJ, IN_PROGRESS);
                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_QUICK_DEMISE_START_EVENT);
            }
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            summon->SetReactState(REACT_PASSIVE);
            Summons.Summon(summon);
        }

        uint32 GetSpellForPhaseMask(uint32 phase)
        {
            uint32 spell = 0;
            switch (phase)
            {
                case 16:
                    spell = SPELL_INSANITY_PHASING_1;
                    break;
                case 32:
                    spell = SPELL_INSANITY_PHASING_2;
                    break;
                case 64:
                    spell = SPELL_INSANITY_PHASING_3;
                    break;
                case 128:
                    spell = SPELL_INSANITY_PHASING_4;
                    break;
                case 256:
                    spell = SPELL_INSANITY_PHASING_5;
                    break;
            }
            return spell;
        }

        void SummonedCreatureDespawn(Creature* summon) OVERRIDE
        {
            uint32 phase = summon->GetPhaseMask();
            uint32 nextPhase = 0;
            Summons.Despawn(summon);

            // Check if all summons in this phase killed
            for (SummonList::const_iterator iter = Summons.begin(); iter != Summons.end(); ++iter)
            {
                if (Creature* visage = Unit::GetCreature(*me, *iter))
                {
                    // Not all are dead
                    if (phase == visage->GetPhaseMask())
                        return;
                    else
                        nextPhase = visage->GetPhaseMask();
                }
            }

            // Roll Insanity
            uint32 spell = GetSpellForPhaseMask(phase);
            uint32 spell2 = GetSpellForPhaseMask(nextPhase);
            Map* map = me->GetMap();
            if (!map)
                return;

            Map::PlayerList const &PlayerList = map->GetPlayers();
            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (Player* player = i->GetSource())
                    {
                        if (player->HasAura(spell))
                        {
                            player->RemoveAurasDueToSpell(spell);
                            if (spell2) // if there is still some different mask cast spell for it
                                player->CastSpell(player, spell2, true);
                        }
                    }
                }
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (insanityHandled)
            {
                if (!Summons.empty())
                    return;

                insanityHandled = 0;
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetControlled(false, UNIT_STATE_STUNNED);
                me->RemoveAurasDueToSpell(INSANITY_VISUAL);
            }

            if (uiMindFlayTimer <= diff)
            {
                DoCastVictim(SPELL_MIND_FLAY);
                uiMindFlayTimer = 20*IN_MILLISECONDS;
            } else uiMindFlayTimer -= diff;

            if (uiShadowBoltVolleyTimer <= diff)
            {
                DoCastVictim(SPELL_SHADOW_BOLT_VOLLEY);
                uiShadowBoltVolleyTimer = 5*IN_MILLISECONDS;
            } else uiShadowBoltVolleyTimer -= diff;

            if (uiShiverTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_SHIVER);
                uiShiverTimer = 15*IN_MILLISECONDS;
            } else uiShiverTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);

            if (instance)
                instance->SetBossState(DATA_HERALD_VOLAZJ, DONE);

            Summons.DespawnAll();
            ResetPlayersPhaseMask();
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            Talk(SAY_SLAY);
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_volazjAI(creature);
    }
};

enum VisageCast
{
    VISAGE_CAST_VICTIM,
    VISAGE_CAST_SELF,
    VISAGE_CAST_FRIEND,     // lowest health friendly within 40 yards
    VISAGE_CAST_VICTIM_FAR, // only when the victim is farther than the range
    VISAGE_CAST_VICTIM_NEAR // only when the victim is within the range
};

struct VisageSpell
{
    uint32 spellId;
    uint32 delay;
    uint32 minRepeat;       // 0 for a spell cast once
    uint32 maxRepeat;
    uint8 castType;
    float range;
};

// The visage fights with the abilities of the class and specialization it copies
static std::vector<VisageSpell> GetVisageSpells(uint32 playerClass, uint32 spec)
{
    switch (playerClass)
    {
        case CLASS_WARRIOR:
            switch (spec)
            {
                case SPEC_WARRIOR_ARMS:
                    return { { SPELL_TWISTED_VISAGE_MORTAL_STRIKE, 3000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f },
                             { SPELL_TWISTED_VISAGE_HAMSTRING, 5000, 5000, 10000, VISAGE_CAST_VICTIM, 0.0f } };
                case SPEC_WARRIOR_PROTECTION:
                    return { { SPELL_TWISTED_VISAGE_THUNDER_CLAP, 5000, 5000, 10000, VISAGE_CAST_SELF, 0.0f },
                             { SPELL_TWISTED_VISAGE_DEVASTATE, 3000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
                default:
                    return { { SPELL_TWISTED_VISAGE_INTERCEPT, 2000, 12000, 12000, VISAGE_CAST_VICTIM_FAR, 8.0f },
                             { SPELL_TWISTED_VISAGE_BLOODTHIRST, 3000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
            }
        case CLASS_PALADIN:
            if (spec == SPEC_PALADIN_PROTECTION)
                return { { SPELL_TWISTED_VISAGE_CONSECRATION, 5000, 5000, 10000, VISAGE_CAST_SELF, 0.0f },
                         { SPELL_TWISTED_VISAGE_AVENGERS_SHIELD, 2000, 5000, 10000, VISAGE_CAST_VICTIM, 0.0f } };
            return { { SPELL_TWISTED_VISAGE_CONSECRATION, 5000, 5000, 10000, VISAGE_CAST_SELF, 0.0f },
                     { SPELL_TWISTED_VISAGE_SEAL_OF_COMMAND, 2000, 0, 0, VISAGE_CAST_SELF, 0.0f },
                     { SPELL_TWISTED_VISAGE_JUDGEMENT_OF_LIGHT, 3000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
        case CLASS_HUNTER:
            return { { SPELL_TWISTED_VISAGE_SHOOT, 2000, 1000, 4000, VISAGE_CAST_VICTIM, 0.0f },
                     { SPELL_TWISTED_VISAGE_DISENGAGE, 5000, 10000, 20000, VISAGE_CAST_VICTIM_NEAR, 4.0f } };
        case CLASS_ROGUE:
            return { { SPELL_TWISTED_VISAGE_EVISCERATE, 5000, 5000, 10000, VISAGE_CAST_VICTIM, 0.0f },
                     { SPELL_TWISTED_VISAGE_SINISTER_STRIKE, 2000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
        case CLASS_PRIEST:
            if (spec == SPEC_PRIEST_SHADOW)
                return { { SPELL_TWISTED_VISAGE_SHADOW_WORD_PAIN, 5000, 5000, 10000, VISAGE_CAST_VICTIM, 0.0f },
                         { SPELL_TWISTED_VISAGE_MIND_FLAY, 2000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
            return { { SPELL_TWISTED_VISAGE_RENEW, 2000, 2000, 5000, VISAGE_CAST_FRIEND, 0.0f },
                     { SPELL_TWISTED_VISAGE_GREATER_HEAL, 4000, 4000, 6000, VISAGE_CAST_FRIEND, 0.0f } };
        case CLASS_DEATH_KNIGHT:
            return { { SPELL_TWISTED_VISAGE_DEATH_GRIP, 5000, 12000, 12000, VISAGE_CAST_VICTIM_FAR, 3.0f },
                     { SPELL_TWISTED_VISAGE_PLAGUE_STRIKE, 2000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
        case CLASS_SHAMAN:
            switch (spec)
            {
                case SPEC_SHAMAN_ENHANCEMENT:
                    return { { SPELL_TWISTED_VISAGE_EARTH_SHOCK, 2000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
                case SPEC_SHAMAN_RESTORATION:
                    return { { SPELL_TWISTED_VISAGE_EARTH_SHIELD, 2000, 4000, 6000, VISAGE_CAST_FRIEND, 0.0f },
                             { SPELL_TWISTED_VISAGE_HEALING_WAVE, 4000, 4000, 6000, VISAGE_CAST_FRIEND, 0.0f } };
                default:
                    return { { SPELL_TWISTED_VISAGE_THUNDERSTORM, 5000, 5000, 10000, VISAGE_CAST_SELF, 0.0f },
                             { SPELL_TWISTED_VISAGE_LIGHTNING_BOLT, 2000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
            }
        case CLASS_MAGE:
            return { { SPELL_TWISTED_VISAGE_FROST_NOVA, 5000, 5000, 10000, VISAGE_CAST_SELF, 0.0f },
                     { SPELL_TWISTED_VISAGE_FIREBALL, 2000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
        case CLASS_WARLOCK:
            return { { SPELL_TWISTED_VISAGE_CORRUPTION, 2000, 6000, 10000, VISAGE_CAST_VICTIM, 0.0f },
                     { SPELL_TWISTED_VISAGE_SHADOW_BOLT, 3000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
        case CLASS_DRUID:
            switch (spec)
            {
                case SPEC_DRUID_BALANCE:
                    return { { SPELL_TWISTED_VISAGE_MOONFIRE, 2000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f },
                             { SPELL_TWISTED_VISAGE_WRATH, 3000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
                case SPEC_DRUID_FERAL:
                case SPEC_DRUID_GUARDIAN:
                    return { { SPELL_TWISTED_VISAGE_CAT_FORM, 1, 0, 0, VISAGE_CAST_SELF, 0.0f },
                             { SPELL_TWISTED_VISAGE_MANGLE, 2000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f },
                             { SPELL_TWISTED_VISAGE_RIP, 3000, 3000, 5000, VISAGE_CAST_VICTIM, 0.0f } };
                default:
                    return { { SPELL_TWISTED_VISAGE_LIFEBLOOM, 2000, 4000, 6000, VISAGE_CAST_FRIEND, 0.0f },
                             { SPELL_TWISTED_VISAGE_NOURISH, 4000, 4000, 6000, VISAGE_CAST_FRIEND, 0.0f } };
            }
        default:
            return { };
    }
}

struct npc_twisted_visage : public ScriptedAI
{
    npc_twisted_visage(Creature* creature) : ScriptedAI(creature), playerClass(0), playerSpec(0) { }

    EventMap events;
    std::vector<VisageSpell> spells;
    uint32 playerClass;
    uint32 playerSpec;

    void Reset() OVERRIDE
    {
        DoCast(me, SPELL_TWISTED_VISAGE_VISUAL, true);
    }

    bool IsCaster() const
    {
        switch (playerClass)
        {
            case CLASS_PRIEST:
            case CLASS_HUNTER:
            case CLASS_MAGE:
            case CLASS_WARLOCK:
                return true;
            case CLASS_SHAMAN:
                return playerSpec == SPEC_SHAMAN_ELEMENTAL || playerSpec == SPEC_SHAMAN_RESTORATION;
            case CLASS_DRUID:
                return playerSpec == SPEC_DRUID_BALANCE || playerSpec == SPEC_DRUID_RESTORATION;
            default:
                return false;
        }
    }

    void AttackStart(Unit* who) OVERRIDE
    {
        if (IsCaster())
            AttackStartCaster(who, 25.0f);
        else
            ScriptedAI::AttackStart(who);
    }

    void SetData(uint32 type, uint32 data) OVERRIDE
    {
        if (type == DATA_TWISTED_VISAGE_PLAYER_CLASS)
        {
            if (data >= CLASS_WARRIOR && data <= CLASS_DRUID)
                playerClass = data;
        }
        else if (type == DATA_TWISTED_VISAGE_PLAYER_SPEC && playerClass)
        {
            playerSpec = data;

            if (playerClass == CLASS_ROGUE)
                me->SetCanDualWield(true);

            spells = GetVisageSpells(playerClass, playerSpec);
            for (uint8 i = 0; i < spells.size(); ++i)
                events.ScheduleEvent(i + 1, spells[i].delay);
        }
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        DoCast(me, SPELL_TWISTED_VISAGE_DEATH, true);
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
            VisageSpell const& visageSpell = spells[eventId - 1];
            Unit* victim = me->GetVictim();
            bool cast = false;

            switch (visageSpell.castType)
            {
                case VISAGE_CAST_VICTIM:
                    cast = victim != NULL;
                    if (cast)
                        DoCast(victim, visageSpell.spellId);
                    break;
                case VISAGE_CAST_SELF:
                    cast = true;
                    DoCast(me, visageSpell.spellId);
                    break;
                case VISAGE_CAST_FRIEND:
                    if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                    {
                        cast = true;
                        DoCast(target, visageSpell.spellId);
                    }
                    break;
                case VISAGE_CAST_VICTIM_FAR:
                    cast = victim && !me->IsWithinCombatRange(victim, visageSpell.range);
                    if (cast)
                        DoCast(victim, visageSpell.spellId);
                    break;
                case VISAGE_CAST_VICTIM_NEAR:
                    cast = victim && me->IsWithinCombatRange(victim, visageSpell.range);
                    if (cast)
                        DoCast(victim, visageSpell.spellId);
                    break;
            }

            if (!cast)
                events.ScheduleEvent(eventId, 1000);
            else if (visageSpell.minRepeat)
                events.ScheduleEvent(eventId, urand(visageSpell.minRepeat, visageSpell.maxRepeat));

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }
};

void AddSC_boss_volazj()
{
    new boss_volazj();
    RegisterCreatureAI(npc_twisted_visage);
}
