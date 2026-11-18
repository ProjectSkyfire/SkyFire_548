/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "MoveSplineInit.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Vehicle.h"

enum RealmOfEarth
{
    SPELL_DEEPHOLM_INTRO_TELEPORT   = 84073,
    SPELL_DEEPHOLM_INTRO_TAXI       = 84101,

    NPC_AGGRA                       = 45006,

    MAP_DEEPHOLM                    = 646,

    TALK_INTRO                      = 0,
    TALK_SPECIAL_1                  = 1,
    TALK_SPECIAL_2                  = 2,
    TALK_SPECIAL_3                  = 3,
    TALK_SPECIAL_4                  = 4,
    TALK_SPECIAL_5                  = 5,
    TALK_SPECIAL_6                  = 6,
    TALK_SPECIAL_7                  = 7,
    TALK_SPECIAL_8                  = 8,
    TALK_SPECIAL_9                  = 9,
    TALK_SPECIAL_10                 = 10
};

Position const WyvernMaelstromPath[10] =
{
    { 810.61f, 1009.42f, 17.96f, 0.0f },
    { 788.45f, 857.77f, -33.96f, 0.0f },
    { 850.80f, 705.77f, -60.97f, 0.0f },
    { 968.77f, 741.15f, -83.58f, 0.0f },
    { 940.87f, 828.13f, -102.28f, 0.0f },
    { 864.19f, 839.58f, -125.39f, 0.0f },
    { 868.62f, 759.38f, -145.01f, 0.0f },
    { 933.94f, 762.14f, -171.41f, 0.0f },
    { 910.86f, 808.24f, -197.10f, 0.0f },
    { 893.38f, 789.12f, -247.18f, 0.0f }
};

Position const WyvernDeepholmPath[8] =
{
    { 1040.91f, 544.689f, 681.86f, 3.97935f },
    { 1098.07f, 627.78f, 485.69f, 0.0f },
    { 1100.67f, 655.00f, 362.34f, 0.0f },
    { 974.18f, 664.47f, 294.96f, 0.0f },
    { 893.63f, 633.28f, 196.87f, 0.0f },
    { 858.82f, 573.17f, 148.96f, 0.0f },
    { 846.16f, 496.09f, -1.66f, 0.0f },
    { 915.32f, 503.90f, -35.16f, 0.0f }
};

// Wyvern 45024
struct npc_deepholm_wyvern_ride : public ScriptedAI
{
    npc_deepholm_wyvern_ride(Creature* creature) : ScriptedAI(creature), targetGUID(0), aggraGUID(0) { }

    TaskScheduler scheduler;
    uint64 targetGUID;
    uint64 aggraGUID;

    void StartPath(Position const* path, uint32 count)
    {
        me->StopMoving();
        Movement::MoveSplineInit init(me);
        for (uint32 i = 0; i < count; ++i)
            init.Path().push_back(G3D::Vector3(path[i].GetPositionX(), path[i].GetPositionY(), path[i].GetPositionZ()));

        init.SetSmooth();
        init.SetFly();
        init.Launch();
    }

    void SummonAggra()
    {
        if (Creature* aggra = me->SummonCreature(NPC_AGGRA, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
        {
            aggraGUID = aggra->GetGUID();
            aggra->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);
        }
    }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        targetGUID = summoner->GetGUID();
        me->SetCanFly(true);
        me->SetDisableGravity(true);
        me->SetAnimTier(ANIM_TIER_FLY);
        me->SetSpeed(MOVE_FLIGHT, 2.5f);

        if (me->GetMapId() != MAP_DEEPHOLM)
        {
            me->SetSpeed(MOVE_FLIGHT, 3.1f);
            SummonAggra();
            StartPath(WyvernMaelstromPath, 10);

            scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext /*context*/)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                {
                    target->ExitVehicle();
                    target->CastSpell(target, SPELL_DEEPHOLM_INTRO_TELEPORT, true);
                }

                if (Creature* aggra = ObjectAccessor::GetCreature(*me, aggraGUID))
                    aggra->DespawnOrUnsummon(500);

                me->DespawnOrUnsummon(500);
            });
        }
        else
        {
            me->SetSpeed(MOVE_FLIGHT, 2.6f);

            scheduler.Schedule(Milliseconds(4000), [this](TaskContext /*context*/)
            {
                SummonAggra();
                StartPath(WyvernDeepholmPath, 8);

                scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext /*context*/)
                {
                    if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                        target->ExitVehicle();

                    if (Creature* aggra = ObjectAccessor::GetCreature(*me, aggraGUID))
                    {
                        aggra->AI()->Talk(TALK_SPECIAL_10);
                        aggra->DespawnOrUnsummon(5000);
                    }

                    me->DespawnOrUnsummon(500);
                });
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Aggra 45006
struct npc_maelstrom_aggra : public ScriptedAI
{
    npc_maelstrom_aggra(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        uint32 delay;
        if (me->GetMapId() != MAP_DEEPHOLM)
        {
            delay = 3200;
            ScheduleTalk(delay, TALK_INTRO);
            ScheduleTalk(delay += 10000, TALK_SPECIAL_1);
            ScheduleTalk(delay += 10500, TALK_SPECIAL_2);
            ScheduleTalk(delay += 8500, TALK_SPECIAL_3);
        }
        else
        {
            delay = 1250;
            ScheduleTalk(delay, TALK_SPECIAL_4);
            ScheduleTalk(delay += 3900, TALK_SPECIAL_5);
            ScheduleTalk(delay += 6000, TALK_SPECIAL_6);
            ScheduleTalk(delay += 14400, TALK_SPECIAL_7);
            ScheduleTalk(delay += 14600, TALK_SPECIAL_8);
            ScheduleTalk(delay += 9800, TALK_SPECIAL_9);
        }
    }

    void ScheduleTalk(uint32 delay, uint8 group)
    {
        scheduler.Schedule(Milliseconds(delay), [this, group](TaskContext /*context*/)
        {
            Talk(group);
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// 84073 - Deepholm Intro Teleport
class spell_deepholm_intro_teleport : public AuraScript
{
    PrepareAuraScript(spell_deepholm_intro_teleport);

    bool Load() OVERRIDE
    {
        taxiCast = false;
        return true;
    }

    void OnUpdate(uint32 /*diff*/, AuraEffect* /*aurEff*/)
    {
        Unit* owner = GetUnitOwner();
        if (!owner || taxiCast || owner->GetMapId() != MAP_DEEPHOLM)
            return;

        taxiCast = true;
        owner->CastSpell(owner, SPELL_DEEPHOLM_INTRO_TAXI, true);
    }

    void Register() OVERRIDE
    {
        OnEffectUpdate += AuraEffectUpdateFn(spell_deepholm_intro_teleport::OnUpdate, EFFECT_0, SPELL_AURA_DUMMY);
    }

    bool taxiCast;
};

void AddSC_deepholm()
{
    new creature_script<npc_deepholm_wyvern_ride>("npc_deepholm_wyvern_ride");
    new creature_script<npc_maelstrom_aggra>("npc_maelstrom_aggra");
    RegisterAuraScript(spell_deepholm_intro_teleport);
}
