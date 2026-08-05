/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Duskwood
SD%Complete: 100
SDComment: Quest support: 26720, 26760
SDCategory: Duskwood
EndScriptData */

/* ContentData
npc_lurking_potion
npc_oliver_harris
spell_q26720_harris_ampule
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Player.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellAuras.h"
#include "ObjectAccessor.h"

#include <list>

/*######
## npc_lurking_potion - Quest 26720 A Curse We Cannot Lift
######*/

enum LurkingPotion
{
    QUEST_A_CURSE_WE_CANNOT_LIFT    = 26720,

    NPC_LURKING_WORGEN              = 43814,
    NPC_LURKING_WORGEN_KILL_CREDIT  = 43860,

    SPELL_STUNNING_POUNCE           = 81957,
    SPELL_HARRIS_AMPULE             = 82058,

    SAY_HESITATE                    = 0
};

static bool PlayerHasActiveQuest(Player* player)
{
    if (!player)
        return false;

    QuestStatus status = player->GetQuestStatus(QUEST_A_CURSE_WE_CANNOT_LIFT);
    return status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE;
}

static bool IsWeakenedLurkingWorgen(Creature* creature)
{
    if (!creature || creature->GetEntry() != NPC_LURKING_WORGEN)
        return false;

    if (creature->HealthAbovePct(20))
        return false;

    return creature->GetReactState() == REACT_PASSIVE;
}

static Creature* ResolveAmpuleTarget(Player* player, Unit* primaryTarget)
{
    if (!player)
        return NULL;

    if (primaryTarget)
        if (Creature* creature = primaryTarget->ToCreature())
            return creature;

    if (Unit* selected = player->GetSelectedUnit())
        if (Creature* creature = selected->ToCreature())
            return creature;

    if (Unit* victim = player->GetVictim())
        if (Creature* creature = victim->ToCreature())
            return creature;

    return NULL;
}

static bool TryApplyHarrisAmpule(Player* player, Creature* creature)
{
    if (!player || !IsWeakenedLurkingWorgen(creature))
        return false;

    if (!PlayerHasActiveQuest(player))
        return false;

    player->KilledMonsterCredit(NPC_LURKING_WORGEN_KILL_CREDIT, 0);
    creature->DespawnOrUnsummon(1000);
    return true;
}

class npc_lurking_potion : public CreatureScript
{
public:
    npc_lurking_potion() : CreatureScript("npc_lurking_potion") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_lurking_potionAI(creature);
    }

    struct npc_lurking_potionAI : public ScriptedAI
    {
        npc_lurking_potionAI(Creature* creature) : ScriptedAI(creature),
            _pounced(false), _hesitated(false), _pounceTargetGUID() { }

        bool _pounced;
        bool _hesitated;
        uint64 _pounceTargetGUID;

        bool IsHesitating() const { return _hesitated; }

        void EnterEvadeMode() OVERRIDE
        {
            if (_hesitated || _pounced)
                return;

            ScriptedAI::EnterEvadeMode();
        }

        void JustRespawned() OVERRIDE
        {
            _pounced = false;
            _hesitated = false;
            _pounceTargetGUID = 0;
            me->setRegeneratingHealth(true);
            ScriptedAI::JustRespawned();
        }

        void Reset() OVERRIDE
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE, EMOTE_STATE_NONE);

            if (_hesitated)
            {
                me->setRegeneratingHealth(false);
                me->SetReactState(REACT_PASSIVE);
                me->SetHealth(me->CountPctFromMaxHealth(20));
            }
            else
            {
                _pounced = false;
                _pounceTargetGUID = 0;
                me->setRegeneratingHealth(true);
                me->SetReactState(REACT_AGGRESSIVE);
            }
        }

        void EnterCombat(Unit* who) OVERRIDE
        {
            ScriptedAI::EnterCombat(who);
            me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
        }

        void TriggerHesitate()
        {
            if (_hesitated)
                return;

            _hesitated = true;
            Talk(SAY_HESITATE);
            me->AttackStop();
            me->setRegeneratingHealth(false);
            me->SetReactState(REACT_PASSIVE);
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveIdle();

            uint32 floorHealth = me->CountPctFromMaxHealth(20);
            if (me->GetHealth() > floorHealth)
                me->SetHealth(floorHealth);
        }

        void StartPounce(Player* player)
        {
            if (!player || _pounced || _hesitated)
                return;

            _pounced = true;
            _pounceTargetGUID = player->GetGUID();

            me->SetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE, EMOTE_STATE_NONE);
            me->SetReactState(REACT_PASSIVE);
            me->AttackStop();
            me->CastSpell(player, SPELL_STUNNING_POUNCE, false);
        }

        void EngageQuestPlayer(Player* player)
        {
            if (!PlayerHasActiveQuest(player))
                return;

            _pounceTargetGUID = player->GetGUID();
            me->SetReactState(REACT_AGGRESSIVE);
            AttackStart(player);
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE
        {
            if (!_pounced && !_hesitated && who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (Player* player = who->ToPlayer())
                {
                    if (PlayerHasActiveQuest(player) &&
                        me->IsWithinDistInMap(player, 25.0f) && me->IsWithinLOSInMap(player))
                    {
                        StartPounce(player);
                        return;
                    }
                }
            }

            ScriptedAI::MoveInLineOfSight(who);
        }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (_hesitated || !_pounced)
                return;

            bool pounceComplete = (type == EFFECT_MOTION_TYPE && id == EVENT_JUMP) ||
                                  (type == POINT_MOTION_TYPE && id == EVENT_CHARGE);
            if (!pounceComplete)
                return;

            if (Player* player = ObjectAccessor::GetPlayer(*me, _pounceTargetGUID))
                EngageQuestPlayer(player);
        }

        void DamageTaken(Unit* doneBy, uint32& damage) OVERRIDE
        {
            if (_hesitated)
            {
                damage = 0;
                return;
            }

            if (Player* player = doneBy->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (PlayerHasActiveQuest(player))
                {
                    if (!_pounced)
                    {
                        if (me->IsWithinDistInMap(player, 25.0f) && me->IsWithinLOSInMap(player))
                            StartPounce(player);
                        else
                            EngageQuestPlayer(player);
                    }
                    else if (me->GetReactState() == REACT_PASSIVE)
                        EngageQuestPlayer(player);
                }
            }

            if (me->HealthBelowPctDamaged(21, damage))
            {
                uint32 floorHealth = me->CountPctFromMaxHealth(20);
                if (me->GetHealth() > floorHealth)
                    damage = me->GetHealth() - floorHealth;
                else
                    damage = 0;
            }

            if (!me->HealthAbovePct(20))
                TriggerHesitate();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!_hesitated && !me->HealthAbovePct(20))
                TriggerHesitate();

            if (_hesitated)
                return;

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) OVERRIDE
        {
            if (!_hesitated || !spell || spell->Id != SPELL_HARRIS_AMPULE)
                return;

            Player* player = caster->ToPlayer();
            if (!player)
                return;

            TryApplyHarrisAmpule(player, me);
        }
    };
};

/*######
## npc_oliver_harris - Quest 26760 Cry For The Moon
######*/

enum CryForTheMoon
{
    QUEST_CRY_FOR_THE_MOON          = 26760,

    NPC_MASTER_HARRIS               = 288,
    NPC_EVENT_OLIVER_HARRIS         = 43858,
    NPC_EVENT_JITTERS               = 43859,
    NPC_EVENT_SVEN_WORGEN           = 43950,

    GUID_RP_OLIVER                  = 571930,
    GUID_RP_JITTERS                 = 571929,
    GUID_RP_WORGEN                  = 571931,

    SPELL_WORGEN_INSTOCKS           = 69196,

    DATA_START_SCENE                = 7,
    DATA_SCENE_ENDED                = 8
};

static Creature* FindSceneActor(Creature* ref, uint32 spawnGuid, uint32 entry)
{
    if (!ref)
        return NULL;

    Map* map = ref->GetMap();
    map->RemoveCreatureRespawnTime(spawnGuid);

    std::list<Creature*> creatures;
    ref->GetCreatureListWithEntryInGrid(creatures, entry, 150.0f);
    for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
    {
        if ((*itr)->GetDBTableGUIDLow() == spawnGuid)
        {
            if (!(*itr)->IsAlive())
                (*itr)->Respawn(true);
            return (*itr)->IsAlive() ? *itr : NULL;
        }
    }

    if (Creature* creature = map->GetCreature(MAKE_NEW_GUID(spawnGuid, entry, HIGHGUID_UNIT)))
    {
        if (!creature->IsAlive())
            creature->Respawn(true);
        return creature->IsAlive() ? creature : NULL;
    }

    Creature* creature = new Creature();
    if (!creature->LoadCreatureFromDB(spawnGuid, map))
    {
        delete creature;
        return NULL;
    }

    creature->SetVisible(false);
    creature->AI()->Reset();
    return creature;
}

static void ResetCryForTheMoonActor(Creature* actor, float x, float y, float z, float o, bool restoreStocks = false)
{
    if (!actor)
        return;

    Map* map = actor->GetMap();
    if (uint32 dbGuid = actor->GetDBTableGUIDLow())
        map->RemoveCreatureRespawnTime(dbGuid);

    if (!actor->IsAlive())
        actor->Respawn(true);

    actor->CombatStop();
    actor->StopMoving();
    actor->GetMotionMaster()->Clear(false);
    actor->GetMotionMaster()->MoveIdle();
    actor->NearTeleportTo(x, y, z, o);

    if (restoreStocks)
    {
        actor->RemoveAurasDueToSpell(SPELL_WORGEN_INSTOCKS);
        actor->AddAura(SPELL_WORGEN_INSTOCKS, actor);
        actor->SetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE, 490);
    }
    else
        actor->SetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE, 0);

    actor->SetVisible(false);
    actor->AI()->Reset();
}

class npc_oliver_harris : public CreatureScript
{
public:
    npc_oliver_harris() : CreatureScript("npc_oliver_harris") { }

    bool OnQuestAccept(Player* /*player*/, Creature* creature, Quest const* quest) OVERRIDE
    {
        if (!creature || !quest || quest->GetQuestId() != QUEST_CRY_FOR_THE_MOON)
            return false;

        npc_oliver_harrisAI::StartSceneImpl(creature);
        return false;
    }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_oliver_harrisAI(creature);
    }

    struct npc_oliver_harrisAI : public ScriptedAI
    {
        npc_oliver_harrisAI(Creature* creature) : ScriptedAI(creature), _sceneActive(false) { }

        static void StartSceneImpl(Creature* questGiver)
        {
            if (!questGiver)
                return;

            npc_oliver_harrisAI* qgAi = CAST_AI(npc_oliver_harrisAI, questGiver->AI());
            if (qgAi && qgAi->_sceneActive && !questGiver->IsVisible())
                return;

            if (qgAi)
                qgAi->_sceneActive = false;

            Creature* eventOliver = FindSceneActor(questGiver, GUID_RP_OLIVER, NPC_EVENT_OLIVER_HARRIS);
            Creature* jitters = FindSceneActor(questGiver, GUID_RP_JITTERS, NPC_EVENT_JITTERS);
            Creature* worgen = FindSceneActor(questGiver, GUID_RP_WORGEN, NPC_EVENT_SVEN_WORGEN);

            if (!eventOliver || !jitters || !worgen)
                return;

            ResetCryForTheMoonActor(eventOliver, -10754.7f, 338.823f, 37.3415f, 3.56867f);
            ResetCryForTheMoonActor(jitters, -10767.1f, 330.665f, 37.6074f, 5.31068f);
            ResetCryForTheMoonActor(worgen, -10747.4375f, 331.902496f, 37.739048f, 4.55199f, true);

            eventOliver->AI()->Reset();
            jitters->AI()->Reset();
            worgen->AI()->Reset();

            if (qgAi)
                qgAi->_sceneActive = true;

            questGiver->SetVisible(false);
            if (Creature* masterHarris = questGiver->FindNearestCreature(NPC_MASTER_HARRIS, 60.0f))
                masterHarris->SetVisible(false);

            eventOliver->AI()->SetData(0, DATA_START_SCENE);
            jitters->AI()->SetData(0, DATA_START_SCENE);
            worgen->AI()->SetData(0, DATA_START_SCENE);
        }

        void StartScene()
        {
            StartSceneImpl(me);
        }

        void SceneEnded()
        {
            _sceneActive = false;
            me->SetVisible(true);
            if (Creature* masterHarris = me->FindNearestCreature(NPC_MASTER_HARRIS, 60.0f))
                masterHarris->SetVisible(true);

            ResetCryForTheMoonActor(FindSceneActor(me, GUID_RP_OLIVER, NPC_EVENT_OLIVER_HARRIS),
                -10754.7f, 338.823f, 37.3415f, 3.56867f);
            ResetCryForTheMoonActor(FindSceneActor(me, GUID_RP_JITTERS, NPC_EVENT_JITTERS),
                -10767.1f, 330.665f, 37.6074f, 5.31068f);
            ResetCryForTheMoonActor(FindSceneActor(me, GUID_RP_WORGEN, NPC_EVENT_SVEN_WORGEN),
                -10747.4375f, 331.902496f, 37.739048f, 4.55199f, true);
        }

        void SetData(uint32 type, uint32 data) OVERRIDE
        {
            if (type == 0 && data == DATA_SCENE_ENDED)
                SceneEnded();
        }

    private:
        bool _sceneActive;
    };
};

/*######
## spell_q26720_harris_ampule - Quest 26720 Harris's Ampule
######*/

class spell_q26720_harris_ampule : public SpellScriptLoader
{
public:
    spell_q26720_harris_ampule() : SpellScriptLoader("spell_q26720_harris_ampule") { }

    class spell_q26720_harris_ampule_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_q26720_harris_ampule_SpellScript);

        Creature* GetAmpuleCreatureFromCaster(Player* player)
        {
            return ResolveAmpuleTarget(player, GetExplTargetUnit());
        }

        SpellCastResult CheckCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return SpellCastResult::SPELL_CAST_OK;

            if (!PlayerHasActiveQuest(player))
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            if (!IsWeakenedLurkingWorgen(GetAmpuleCreatureFromCaster(player)))
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleAmpule(SpellEffIndex /*effIndex*/)
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            Creature* creature = GetHitCreature();
            if (!creature)
                creature = GetAmpuleCreatureFromCaster(player);

            TryApplyHarrisAmpule(player, creature);
        }

        void HandleAmpuleCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            TryApplyHarrisAmpule(player, GetAmpuleCreatureFromCaster(player));
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_q26720_harris_ampule_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_q26720_harris_ampule_SpellScript::HandleAmpuleCast);
            // MoP DBC: EFFECT_0 is SPELL_EFFECT_DUMMY only.
            OnEffectHitTarget += SpellEffectFn(spell_q26720_harris_ampule_SpellScript::HandleAmpule, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_q26720_harris_ampule_SpellScript();
    }
};

/*######
## spell_q26674_call_stalvan
######*/

class spell_q26674_call_stalvan : public SpellScriptLoader
{
public:
    spell_q26674_call_stalvan() : SpellScriptLoader("spell_q26674_call_stalvan") { }

    class spell_q26674_call_stalvan_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_q26674_call_stalvan_SpellScript);

        void HandleScript()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            // Summon Stalvan and Tobias.
            if (Creature* stalvan = player->SummonCreature(315, -10371.669922f, -1251.914185f, 35.9100023f, 5.536726f, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000))
            {
                stalvan->SetReactState(REACT_PASSIVE);
                stalvan->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            }
            if (Creature* tobias = player->SummonCreature(35124, -10361.395508f, -1256.723022f, 35.859238f, 3.17049f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 60000))
            {
                tobias->SetReactState(REACT_PASSIVE);
                tobias->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                
                // Scale the summoned Tobias so he doesn't get instantly one-shot by Stalvan during the RP combat
                tobias->SetLevel(32);
                tobias->SetMaxHealth(25000);
                tobias->SetFullHealth();
            }
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_q26674_call_stalvan_SpellScript::HandleScript);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_q26674_call_stalvan_SpellScript();
    }
};

void AddSC_duskwood()
{
    new npc_lurking_potion();
    new npc_oliver_harris();
    new spell_q26720_harris_ampule();
    new spell_q26674_call_stalvan();
}
