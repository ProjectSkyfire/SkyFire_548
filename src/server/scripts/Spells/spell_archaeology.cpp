/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "GameObject.h"
#include "Map.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "Unit.h"

enum ArchaeologySurvey
{
    SPELL_SURVEY                 = 80451,

    GO_SURVEY_TOOL_GREEN         = 204272,
    GO_SURVEY_TOOL_YELLOW        = 206589,
    GO_SURVEY_TOOL_RED           = 206590,

    SURVEY_OBJECT_SLOT           = 1 // SPELL_EFFECT_SUMMON_OBJECT_SLOT2 - SLOT1
};

// Retail ranges: red > 100yd, yellow 50-100yd, green < 50yd, find within ~8yd.
static float const SURVEY_FIND_DISTANCE   = 8.0f;
static float const SURVEY_GREEN_DISTANCE  = 50.0f;
static float const SURVEY_YELLOW_DISTANCE = 100.0f;
static int32 const SURVEY_TOOL_LIFETIME   = 5; // seconds

static uint32 GetSurveyToolEntry(float distance)
{
    if (distance <= SURVEY_GREEN_DISTANCE)
        return GO_SURVEY_TOOL_GREEN;
    if (distance <= SURVEY_YELLOW_DISTANCE)
        return GO_SURVEY_TOOL_YELLOW;
    return GO_SURVEY_TOOL_RED;
}

static void ReplaceSurveyObjectSlot(Unit* caster, GameObject* go)
{
    if (uint64 guid = caster->m_ObjectSlot[SURVEY_OBJECT_SLOT])
    {
        if (GameObject* old = caster->GetMap()->GetGameObject(guid))
        {
            if (old->GetSpellId() == SPELL_SURVEY)
                old->SetSpellId(0);
            caster->RemoveGameObject(old, true);
        }
        caster->m_ObjectSlot[SURVEY_OBJECT_SLOT] = 0;
    }

    if (!go)
        return;

    caster->AddGameObject(go);
    caster->m_ObjectSlot[SURVEY_OBJECT_SLOT] = go->GetGUID();
}

static GameObject* SpawnSurveyGameObject(Player* player, uint32 entry, float x, float y, float z, float orientation, int32 lifetimeSecs)
{
    Map* map = player->GetMap();
    if (!map)
        return NULL;

    GameObject* go = new GameObject();
    if (!go->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT), entry, map,
        x, y, z, orientation, 0.0f, 0.0f, 0.0f, 0.0f, 0, GOState::GO_STATE_READY))
    {
        delete go;
        return NULL;
    }

    go->SetRespawnTime(lifetimeSecs > 0 ? lifetimeSecs : 0);
    go->SetSpellId(SPELL_SURVEY);
    go->SetOwnerGUID(player->GetGUID());
    for (auto phase : player->GetPhases())
        go->SetPhased(phase, false, true);

    map->AddToMap(go);
    return go;
}

// 80451 - Survey
// DBC MiscValue is 1 (placeholder GO "unk name"); spawn the correct survey tool / find ourselves.
class spell_archaeology_survey : public SpellScriptLoader
{
public:
    spell_archaeology_survey() : SpellScriptLoader("spell_archaeology_survey") { }

    class spell_archaeology_survey_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_archaeology_survey_SpellScript);

        float _findOrientation = 0.0f;

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        SpellCastResult CheckCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player->HasSkill(SKILL_ARCHAEOLOGY))
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            if (!player->GetCurrentResearchDigsite())
                return SpellCastResult::SPELL_FAILED_NOT_HERE;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void SetTelescopeOrientation()
        {
            Player* player = GetCaster()->ToPlayer();
            ResearchDigsite* digsite = player->GetCurrentResearchDigsite();
            if (!digsite)
                return;

            ArchaeologyFindInfo const* find = digsite->GetArchaeologyFind();
            if (!find)
                return;

            // Survey tool orientation only — do not turn the player toward the find.
            _findOrientation = player->GetAngle(find->x, find->y);
        }

        void HandleSummon(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            Player* player = GetCaster()->ToPlayer();
            ResearchDigsite* digsite = player->GetCurrentResearchDigsite();
            if (!digsite)
                return;

            ArchaeologyFindInfo const* find = digsite->GetArchaeologyFind();
            if (!find)
            {
                player->SendSurveyCastInfo(digsite, false);
                return;
            }

            float distance = player->GetDistance(find->x, find->y, find->z);
            bool success = distance <= SURVEY_FIND_DISTANCE;
            float orientation = _findOrientation;
            if (!orientation)
                orientation = player->GetAngle(find->x, find->y);

            if (success)
            {
                // Clear previous survey tool; dig find is not tracked in the object slot.
                ReplaceSurveyObjectSlot(player, NULL);

                if (GameObject* go = SpawnSurveyGameObject(player, find->goEntry, find->x, find->y, find->z, orientation, 60))
                {
                    // Finds are looted via Use/SendLoot; keep owner for archaeology credit checks.
                    go->SetSpellId(0);
                }
            }
            else
            {
                float x, y, z;
                player->GetClosePoint(x, y, z, DEFAULT_WORLD_OBJECT_SIZE);
                uint32 toolEntry = GetSurveyToolEntry(distance);
                if (GameObject* go = SpawnSurveyGameObject(player, toolEntry, x, y, z, orientation, SURVEY_TOOL_LIFETIME))
                    ReplaceSurveyObjectSlot(player, go);
            }

            player->SendSurveyCastInfo(digsite, success);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_archaeology_survey_SpellScript::CheckCast);
            BeforeCast += SpellCastFn(spell_archaeology_survey_SpellScript::SetTelescopeOrientation);
            OnEffectHit += SpellEffectFn(spell_archaeology_survey_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON_OBJECT_SLOT2);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_archaeology_survey_SpellScript();
    }
};

void AddSC_archaeology_spell_scripts()
{
    new spell_archaeology_survey();
}
