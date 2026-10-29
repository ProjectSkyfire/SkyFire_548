/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Player.h"
#include "GameObject.h"
#include "naxxramas.h"

//Stalagg
enum StalaggYells
{
    SAY_STAL_AGGRO          = 0,
    SAY_STAL_SLAY           = 1,
    SAY_STAL_DEATH          = 2
};

enum StalagSpells
{
    SPELL_POWERSURGE        = 28134,
    H_SPELL_POWERSURGE      = 54529,
    SPELL_MAGNETIC_PULL     = 28338,
    SPELL_STALAGG_TESLA     = 28097
};

//Feugen
enum FeugenYells
{
    SAY_FEUG_AGGRO          = 0,
    SAY_FEUG_SLAY           = 1,
    SAY_FEUG_DEATH          = 2
};

enum FeugenSpells
{
    SPELL_STATICFIELD       = 28135,
    H_SPELL_STATICFIELD     = 54528,
    SPELL_FEUGEN_TESLA      = 28109
};

// Thaddius DoAction
enum ThaddiusActions
{
    ACTION_FEUGEN_RESET,
    ACTION_FEUGEN_DIED,
    ACTION_STALAGG_RESET,
    ACTION_STALAGG_DIED
};

//generic
enum TeslaSpells
{
    SPELL_STALAGG_CHAIN = 28096,
    SPELL_FEUGEN_CHAIN  = 28111,
    SPELL_TESLA_SHOCK   = 28099,
    SPELL_SHOCK_VISUAL  = 28159,
    SPELL_SPARK_VISUAL  = 28136,
    SPELL_INACTIVE_VISUAL = 28160
};

Creature* GetTeslaCoil(Creature* add)
{
    return add->FindNearestCreature(NPC_TESLA, 1000.0f, true);
}

GameObject* GetTeslaCoilGO(Creature* add, uint32 entry)
{
    return add->FindNearestGameObject(entry, 1000.0f);
}

//Thaddius
enum ThaddiusYells
{
    SAY_GREET               = 0,
    SAY_AGGRO               = 1,
    SAY_SLAY                = 2,
    SAY_ELECT               = 3,
    SAY_DEATH               = 4,
    SAY_SCREAM              = 5
};

enum ThaddiusSpells
{
    SPELL_POLARITY_SHIFT        = 28089,
    SPELL_BALL_LIGHTNING        = 28299,
    SPELL_CHAIN_LIGHTNING       = 28167,
    H_SPELL_CHAIN_LIGHTNING     = 54531,
    SPELL_BERSERK               = 27680,
    SPELL_POSITIVE_CHARGE       = 28062,
    SPELL_POSITIVE_CHARGE_STACK = 29659,
    SPELL_NEGATIVE_CHARGE       = 28085,
    SPELL_NEGATIVE_CHARGE_STACK = 29660,
    SPELL_POSITIVE_POLARITY     = 28059,
    SPELL_NEGATIVE_POLARITY     = 28084,
};

enum Events
{
    EVENT_NONE,
    EVENT_SHIFT,
    EVENT_CHAIN,
    EVENT_BERSERK,
};

enum Achievement
{
    DATA_POLARITY_SWITCH    = 76047605,
};

class boss_thaddius : public CreatureScript
{
public:
    boss_thaddius() : CreatureScript("boss_thaddius") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_thaddiusAI(creature);
    }

    struct boss_thaddiusAI : public BossAI
    {
        boss_thaddiusAI(Creature* creature) : BossAI(creature, BOSS_THADDIUS)
        {
            // init is a bit tricky because thaddius shall track the life of both adds, but not if there was a wipe
            // and, in particular, if there was a crash after both adds were killed (should not respawn)

            // Moreover, the adds may not yet be spawn. So just track down the status if mob is spawn
            // and each mob will send its status at reset (meaning that it is alive)
            uiAddsTimer = 0;
            transitionTimer = 0;
            transitionStage = 0;
            polaritySwitch = false;
            checkFeugenAlive = false;
            if (Creature* pFeugen = me->GetCreature(*me, instance->GetData64(DATA_FEUGEN)))
                checkFeugenAlive = pFeugen->IsAlive();

            checkStalaggAlive = false;
            if (Creature* pStalagg = me->GetCreature(*me, instance->GetData64(DATA_STALAGG)))
                checkStalaggAlive = pStalagg->IsAlive();

            if (!checkFeugenAlive && !checkStalaggAlive)
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);
                me->SetReactState(REACT_AGGRESSIVE);
            }
            else
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);
                me->SetReactState(REACT_PASSIVE);
            }
        }

        bool checkStalaggAlive;
        bool checkFeugenAlive;
        bool polaritySwitch;
        uint32 uiAddsTimer;
        uint32 transitionTimer;
        uint8 transitionStage;

        void CoilTransition(uint8 stage)
        {
            if (!instance)
                return;
            uint32 adds[] = { DATA_FEUGEN, DATA_STALAGG };
            uint32 objects[] = { DATA_FEUGEN_COIL_GO, DATA_STALAGG_COIL_GO };
            for (uint8 i = 0; i < 2; ++i)
            {
                Creature* add = me->GetCreature(*me, instance->GetData64(adds[i]));
                Creature* coil = add ? GetTeslaCoil(add) : NULL;
                if (stage == 1 && coil)
                {
                    coil->CastStop();
                    coil->AI()->Talk(1);
                }
                else if (stage == 2 && coil)
                    coil->CastSpell(me, SPELL_SHOCK_VISUAL, false);
                else if (stage == 3)
                    if (GameObject* go = GameObject::GetGameObject(*me, instance->GetData64(objects[i])))
                        go->SetGoState(GOState::GO_STATE_READY);
            }
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            if (!(rand()%5))
                Talk(SAY_SLAY);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _JustDied();
            Talk(SAY_DEATH);
        }

        void DoAction(int32 action) OVERRIDE
        {
            switch (action)
            {
                case ACTION_FEUGEN_RESET:
                    checkFeugenAlive = true;
                    break;
                case ACTION_FEUGEN_DIED:
                    checkFeugenAlive = false;
                    break;
                case ACTION_STALAGG_RESET:
                    checkStalaggAlive = true;
                    break;
                case ACTION_STALAGG_DIED:
                    checkStalaggAlive = false;
                    break;
            }

            if (!checkFeugenAlive && !checkStalaggAlive)
            {
                if (!transitionStage && !transitionTimer)
                {
                    transitionStage = 1;
                    transitionTimer = 10000;
                }
            }
            else
            {
                transitionStage = 0;
                transitionTimer = 0;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);
                me->SetReactState(REACT_PASSIVE);
            }
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            Talk(SAY_AGGRO);
            events.ScheduleEvent(EVENT_SHIFT, 30000);
            events.ScheduleEvent(EVENT_CHAIN, std::rand() % 20000 + 10000);
            events.ScheduleEvent(EVENT_BERSERK, 360000);
        }

        void DamageTaken(Unit* /*pDoneBy*/, uint32 & /*uiDamage*/) OVERRIDE
        {
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void SetData(uint32 id, uint32 data) OVERRIDE
        {
            if (id == DATA_POLARITY_SWITCH)
                polaritySwitch = data ? true : false;
        }

        uint32 GetData(uint32 id) const OVERRIDE
        {
            if (id != DATA_POLARITY_SWITCH)
                return 0;

            return uint32(polaritySwitch);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (transitionStage)
            {
                if (transitionTimer <= diff)
                {
                    CoilTransition(transitionStage);
                    if (transitionStage == 2 || transitionStage == 3)
                        me->CastSpell(me, SPELL_SPARK_VISUAL, true);
                    if (transitionStage == 3)
                    {
                        transitionStage = 0;
                        transitionTimer = 0;
                        me->RemoveAurasDueToSpell(SPELL_INACTIVE_VISUAL);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);
                        DoZoneInCombat();
                    }
                    else
                    {
                        ++transitionStage;
                        transitionTimer = 2000;
                    }
                }
                else
                    transitionTimer -= diff;
            }

            if (checkFeugenAlive && checkStalaggAlive)
                uiAddsTimer = 0;

            if (checkStalaggAlive != checkFeugenAlive)
            {
                uiAddsTimer += diff;
                if (uiAddsTimer > 5000)
                {
                    if (!checkStalaggAlive)
                    {
                        if (instance)
                            if (Creature* pStalagg = me->GetCreature(*me, instance->GetData64(DATA_STALAGG)))
                                pStalagg->Respawn();
                    }
                    else
                    {
                        if (instance)
                            if (Creature* pFeugen = me->GetCreature(*me, instance->GetData64(DATA_FEUGEN)))
                                pFeugen->Respawn();
                    }
                }
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
                    case EVENT_SHIFT:
                        DoCastAOE(SPELL_POLARITY_SHIFT);
                        events.ScheduleEvent(EVENT_SHIFT, 30000);
                        return;
                    case EVENT_CHAIN:
                        DoCastVictim(RAID_MODE(SPELL_CHAIN_LIGHTNING, H_SPELL_CHAIN_LIGHTNING));
                        events.ScheduleEvent(EVENT_CHAIN, std::rand() % 20000 + 10000);
                        return;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK);
                        return;
                }
            }

            if (events.GetTimer() > 15000 && !me->IsWithinMeleeRange(me->GetVictim()))
                DoCastVictim(SPELL_BALL_LIGHTNING);
            else
                DoMeleeAttackIfReady();
        }
    };
};

struct tesla_linkAI : public ScriptedAI
{
    tesla_linkAI(Creature* creature, uint32 chain, uint32 goEntry) : ScriptedAI(creature), chainSpell(chain), coilGOEntry(goEntry), linkTimer(0), linked(false), broken(false) { }

    uint32 chainSpell;
    uint32 coilGOEntry;
    uint32 linkTimer;
    bool linked;
    bool broken;

    void ResetLink()
    {
        linkTimer = 0;
        linked = false;
        broken = false;
        if (GameObject* go = GetTeslaCoilGO(me, coilGOEntry))
            go->SetGoState(GOState::GO_STATE_ACTIVE);
    }

    void StopLink()
    {
        me->RemoveAurasDueToSpell(chainSpell);
        if (Creature* coil = GetTeslaCoil(me))
            coil->CastStop();
        linked = false;
    }

    void UpdateLink(uint32 diff)
    {
        if (linkTimer > diff)
        {
            linkTimer -= diff;
            return;
        }
        // 28098 and 28110 are absent in 5.4.8; check the link directly.
        linkTimer = 1000;
        Creature* coil = GetTeslaCoil(me);
        if (!coil)
        {
            linked = false;
            return;
        }
        if (GameObject* go = GetTeslaCoilGO(me, coilGOEntry))
            if (go->GetGoState() != GOState::GO_STATE_ACTIVE)
                go->SetGoState(GOState::GO_STATE_ACTIVE);
        if (me->IsInCombat() && me->GetDistance(me->GetHomePosition()) > 28.0f)
        {
            if (!broken)
            {
                broken = true;
                linked = false;
                me->RemoveAurasDueToSpell(chainSpell);
                coil->CastStop();
                coil->AI()->Talk(0);
            }
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                coil->CastSpell(target, SPELL_TESLA_SHOCK, true);
        }
        else if (!linked || broken)
        {
            broken = false;
            linked = true;
            coil->CastStop();
            coil->CastSpell(me, chainSpell, true);
        }
    }
};

class npc_stalagg : public CreatureScript
{
public:
    npc_stalagg() : CreatureScript("npc_stalagg") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_stalaggAI(creature);
    }

    struct npc_stalaggAI : public tesla_linkAI
    {
        npc_stalaggAI(Creature* creature) : tesla_linkAI(creature, SPELL_STALAGG_CHAIN, GO_CONS_NOX_TESLA_STALAGG)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 powerSurgeTimer;
        uint32 magneticPullTimer;

        void Reset() OVERRIDE
        {
            ResetLink();
            if (instance)
                if (Creature* pThaddius = me->GetCreature(*me, instance->GetData64(DATA_THADDIUS)))
                    if (pThaddius->AI())
                        pThaddius->AI()->DoAction(ACTION_STALAGG_RESET);
            powerSurgeTimer = std::rand() % 25000 + 20000;
            magneticPullTimer = 20000;
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            if (!(rand()%5))
                Talk(SAY_STAL_SLAY);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_STAL_AGGRO);
            DoCast(SPELL_STALAGG_TESLA);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_STAL_DEATH);
            StopLink();
            if (instance)
                if (Creature* pThaddius = me->GetCreature(*me, instance->GetData64(DATA_THADDIUS)))
                    if (pThaddius->AI())
                        pThaddius->AI()->DoAction(ACTION_STALAGG_DIED);
        }

        void UpdateAI(uint32 uiDiff) OVERRIDE
        {
            UpdateLink(uiDiff);
            if (!UpdateVictim())
                return;

            if (magneticPullTimer <= uiDiff)
            {
                if (Creature* pFeugen = me->GetCreature(*me, instance->GetData64(DATA_FEUGEN)))
                {
                    Unit* pStalaggVictim = me->GetVictim();
                    Unit* pFeugenVictim = pFeugen->GetVictim();

                    if (pFeugenVictim && pStalaggVictim)
                    {
                        // magnetic pull is not working. So just jump.

                        // reset aggro to be sure that feugen will not follow the jump
                        pFeugen->getThreatManager().modifyThreatPercent(pFeugenVictim, -100);
                        pFeugenVictim->JumpTo(me, 0.3f);

                        me->getThreatManager().modifyThreatPercent(pStalaggVictim, -100);
                        pStalaggVictim->JumpTo(pFeugen, 0.3f);
                    }
                }

                magneticPullTimer = 20000;
            }
            else magneticPullTimer -= uiDiff;

            if (powerSurgeTimer <= uiDiff)
            {
                DoCast(me, RAID_MODE(SPELL_POWERSURGE, H_SPELL_POWERSURGE));
                powerSurgeTimer = std::rand() % 20000 + 15000;
            } else powerSurgeTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
};

class npc_feugen : public CreatureScript
{
public:
    npc_feugen() : CreatureScript("npc_feugen") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_feugenAI(creature);
    }

    struct npc_feugenAI : public tesla_linkAI
    {
        npc_feugenAI(Creature* creature) : tesla_linkAI(creature, SPELL_FEUGEN_CHAIN, GO_CONS_NOX_TESLA_FEUGEN)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 staticFieldTimer;

        void Reset() OVERRIDE
        {
            ResetLink();
            if (instance)
                if (Creature* pThaddius = me->GetCreature(*me, instance->GetData64(DATA_THADDIUS)))
                    if (pThaddius->AI())
                        pThaddius->AI()->DoAction(ACTION_FEUGEN_RESET);
            staticFieldTimer = 5000;
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            if (!(rand()%5))
                Talk(SAY_FEUG_SLAY);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_FEUG_AGGRO);
            DoCast(SPELL_FEUGEN_TESLA);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_FEUG_DEATH);
            StopLink();
            if (instance)
                if (Creature* pThaddius = me->GetCreature(*me, instance->GetData64(DATA_THADDIUS)))
                    if (pThaddius->AI())
                        pThaddius->AI()->DoAction(ACTION_FEUGEN_DIED);
        }

        void UpdateAI(uint32 uiDiff) OVERRIDE
        {
            UpdateLink(uiDiff);
            if (!UpdateVictim())
                return;

            if (staticFieldTimer <= uiDiff)
            {
                DoCast(me, RAID_MODE(SPELL_STATICFIELD, H_SPELL_STATICFIELD));
                staticFieldTimer = 5000;
            } else staticFieldTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
};

class npc_tesla : public CreatureScript
{
public:
    npc_tesla() : CreatureScript("npc_tesla") { }

    struct npc_teslaAI : public ScriptedAI
    {
        npc_teslaAI(Creature* creature) : ScriptedAI(creature) { me->SetReactState(REACT_PASSIVE); }
        void AttackStart(Unit* /*target*/) OVERRIDE { }
        void EnterCombat(Unit* /*who*/) OVERRIDE { }
        void EnterEvadeMode() OVERRIDE { }
        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE { damage = 0; }
        void UpdateAI(uint32 /*diff*/) OVERRIDE { }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new npc_teslaAI(creature); }
};

class spell_thaddius_pos_neg_charge : public SpellScriptLoader
{
    public:
        spell_thaddius_pos_neg_charge() : SpellScriptLoader("spell_thaddius_pos_neg_charge") { }

        class spell_thaddius_pos_neg_charge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thaddius_pos_neg_charge_SpellScript);

            bool Validate(SpellInfo const* /*spell*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_POSITIVE_CHARGE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_POSITIVE_CHARGE_STACK))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_NEGATIVE_CHARGE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_NEGATIVE_CHARGE_STACK))
                    return false;
                return true;
            }

            bool Load() OVERRIDE
            {
                return GetCaster()->GetTypeId() == TypeID::TYPEID_UNIT;
            }

            void HandleTargets(std::list<WorldObject*>& targets)
            {
                uint8 count = 0;
                for (std::list<WorldObject*>::iterator ihit = targets.begin(); ihit != targets.end(); ++ihit)
                    if ((*ihit)->GetGUID() != GetCaster()->GetGUID())
                        if (Player* target = (*ihit)->ToPlayer())
                            if (target->HasAura(GetTriggeringSpell()->Id))
                                ++count;

                if (count)
                {
                    uint32 spellId = 0;

                    if (GetSpellInfo()->Id == SPELL_POSITIVE_CHARGE)
                        spellId = SPELL_POSITIVE_CHARGE_STACK;
                    else // if (GetSpellInfo()->Id == SPELL_NEGATIVE_CHARGE)
                        spellId = SPELL_NEGATIVE_CHARGE_STACK;

                    GetCaster()->SetAuraStack(spellId, GetCaster(), count);
                }
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (!GetTriggeringSpell())
                    return;

                Unit* target = GetHitUnit();
                Unit* caster = GetCaster();

                if (target->HasAura(GetTriggeringSpell()->Id))
                    SetHitDamage(0);
                else
                {
                    if (target->GetTypeId() == TypeID::TYPEID_PLAYER && caster->IsAIEnabled)
                        caster->ToCreature()->AI()->SetData(DATA_POLARITY_SWITCH, 1);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_thaddius_pos_neg_charge_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_thaddius_pos_neg_charge_SpellScript::HandleTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_thaddius_pos_neg_charge_SpellScript();
        }
};

class spell_thaddius_polarity_shift : public SpellScriptLoader
{
    public:
        spell_thaddius_polarity_shift() : SpellScriptLoader("spell_thaddius_polarity_shift") { }

        class spell_thaddius_polarity_shift_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thaddius_polarity_shift_SpellScript);

            bool Validate(SpellInfo const* /*spell*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_POSITIVE_POLARITY) || !sSpellMgr->GetSpellInfo(SPELL_NEGATIVE_POLARITY))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, roll_chance_i(50) ? SPELL_POSITIVE_POLARITY : SPELL_NEGATIVE_POLARITY, true, NULL, NULL, caster->GetGUID());
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_thaddius_polarity_shift_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_thaddius_polarity_shift_SpellScript();
        }
};

class achievement_polarity_switch : public AchievementCriteriaScript
{
    public:
        achievement_polarity_switch() : AchievementCriteriaScript("achievement_polarity_switch") { }

        bool OnCheck(Player* /*source*/, Unit* target) OVERRIDE
        {
            return target && target->GetAI()->GetData(DATA_POLARITY_SWITCH);
        }
};

void AddSC_boss_thaddius()
{
    new boss_thaddius();
    new npc_stalagg();
    new npc_feugen();
    new npc_tesla();
    new spell_thaddius_pos_neg_charge();
    new spell_thaddius_polarity_shift();
    new achievement_polarity_switch();
}
