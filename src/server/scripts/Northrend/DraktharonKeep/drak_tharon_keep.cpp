/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"

enum SummonRandomDrakkari
{
    SPELL_SUMMON_DRAKKARI_SHAMAN      = 49958,
    SPELL_SUMMON_DRAKKARI_GUARDIAN    = 49959
};

// 49960 - Summon Random Drakkari
class spell_summon_random_drakkari : public SpellScript
{
    PrepareSpellScript(spell_summon_random_drakkari);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetCaster(), RAND(SPELL_SUMMON_DRAKKARI_SHAMAN, SPELL_SUMMON_DRAKKARI_GUARDIAN));
    }

    void Register() OVERRIDE
    {
        OnEffectHit += SpellEffectFn(spell_summon_random_drakkari::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

void AddSC_drak_tharon_keep()
{
    RegisterSpellScript(spell_summon_random_drakkari);
}
