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
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "GridNotifiers.h"
#include "TemporarySummon.h"
#include "throne_of_the_four_winds.h"

enum SlipstreamEnums
{
    // Land Positions
    DIR_WEST_TO_SOUTH,
    DIR_SOUTH_TO_WEST,
    DIR_NORTH_TO_WEST,
    DIR_WEST_TO_NORTH,
    DIR_EAST_TO_NORTH,
    DIR_NORTH_TO_EAST,
    DIR_SOUTH_TO_EAST,
    DIR_EAST_TO_SOUTH,
    DIR_TO_MIDDLE,

    // Spells
    SPELL_SLIPSTREAM_BUFF                   = 87740,
    SPELL_SLIPSTREAM_PLAYER_VISUAL          = 85063,
    SPELL_SLIPSTREAM_TO_MIDDLE_AURA         = 87713,
};

Position const SlipstreamPositions[8] =
{
    {-245.141129f,  861.474060f,    197.386398f,    0},
    {-92.116440f,   1010.796448f,   197.475754f,    0},
    {-5.322055f,    1010.573608f,   197.520096f,    0},
    {144.480469f,   857.187927f,    197.594208f,    0},
    {144.221481f,   770.720154f,    197.629150f,    0},
    {-9.268366f,    620.736328f,    197.567032f,    0},
    {-96.089645f,   621.198730f,    197.499115f,    0},
    {-245.653870f,  774.446472f,    197.507156f,    0},
};

class npc_slipstream_raid : public CreatureScript
{
    public:
        npc_slipstream_raid() : CreatureScript("npc_slipstream_raid") { }

        struct npc_slipstream_raidAI : public ScriptedAI
        {
            npc_slipstream_raidAI(Creature* creature) : ScriptedAI(creature), isActive(true), linkedSlipstreamObjectGuid(), linkedBossGuid(), isUltimate(false)
            {
                instance = creature->GetInstanceScript();
                SlipstreamPosition = DIR_TO_MIDDLE;

                if (creature->ToTempSummon())
                {
                    DoCast(me, SPELL_SLIPSTREAM_TO_MIDDLE_AURA, true);
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    return;
                }

                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                for (uint8 i = 0; i < DIR_TO_MIDDLE; ++i)
                {
                    if (me->GetDistance2d(SlipstreamPositions[i].GetPositionX(), SlipstreamPositions[i].GetPositionY()) < 25)
                    {
                        SlipstreamPosition = i;
                        break;
                    }
                }

                if (SlipstreamPosition >= DIR_TO_MIDDLE)
                    return;

                SlipstreamPosition += (SlipstreamPosition % 2) ? -1 : 1;
            }

            void OnSpellClick(Unit* clicker, bool& result) OVERRIDE
            {
                SetGUID(clicker->GetGUID(), 0);
                result = true;
            }

            void SetGUID(uint64 guid, int32 /*type*/) OVERRIDE
            {
                if (!isActive)
                    return;

                if (Player* target = ObjectAccessor::GetPlayer(*me, guid))
                {
                    DoCast(target, SPELL_SLIPSTREAM_BUFF, true);
                    if (SlipstreamPosition == DIR_TO_MIDDLE)
                    {
                        float x, y, z;
                        me->GetPosition(x, y, z);
                        z = 345;
                        target->GetMotionMaster()->MoveJump(x, y, z, 1, (z - target->GetPositionZ()) * 1.75f);
                        target->m_Events.AddEvent(new MovementEvent(target), target->m_Events.CalculateTime(3000));
                    }
                    else
                    {
                        float x, y, z;
                        SlipstreamPositions[SlipstreamPosition].GetPosition(x, y, z);
                        target->GetMotionMaster()->MoveJump(x, y, 200, 100, 15);
                    }
                }
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!linkedBossGuid)
                {
                    Creature* boss = NULL;
                    if (!(boss = me->FindNearestCreature(BOSS_ANSHAL, 100)))
                        if (!(boss = me->FindNearestCreature(BOSS_NEZIR, 100)))
                            if (!(boss = me->FindNearestCreature(BOSS_ROHASH, 100))) { }
                    if (boss)
                        linkedBossGuid = boss->GetGUID();
                }
                if (!linkedSlipstreamObjectGuid)
                {
                    GameObject* go = NULL;
                    if (!(go = me->FindNearestGameObject(GO_WIND_DRAFTEFFECT_1, 50)))
                        if (!(go = me->FindNearestGameObject(GO_WIND_DRAFTEFFECT_2, 50)))
                            if (!(go = me->FindNearestGameObject(GO_WIND_DRAFTEFFECT_3, 50)))
                                if (!(go = me->FindNearestGameObject(GO_WIND_DRAFTEFFECT_4, 50)))
                                    if (!(go = me->FindNearestGameObject(GO_WIND_DRAFTEFFECT_5, 50)))
                                        if (!(go = me->FindNearestGameObject(GO_WIND_DRAFTEFFECT_6, 50)))
                                            if (!(go = me->FindNearestGameObject(GO_WIND_DRAFTEFFECT_7, 50)))
                                                if (!(go = me->FindNearestGameObject(GO_WIND_DRAFTEFFECT_8, 50))) { }
                    if (go)
                        linkedSlipstreamObjectGuid = go->GetGUID();
                }

                // The Slipstreams are Deactivated before each Ultimate ability
                if (instance && instance->GetBossState(DATA_ALAKIR) == IN_PROGRESS && SlipstreamPosition != DIR_TO_MIDDLE)
                    isUltimate = true;
                else if (Creature* boss = ObjectAccessor::GetCreature(*me, linkedBossGuid))
                    isUltimate = boss->AI()->GetData(0);
                else if (isUltimate)
                    isUltimate = false;

                if (!isUltimate != isActive)
                {
                    isActive = !isActive;
                    if (GameObject* go = ObjectAccessor::GetGameObject(*me, linkedSlipstreamObjectGuid))
                        go->SetGoState(isActive ? GOState::GO_STATE_READY : GOState::GO_STATE_ACTIVE);
                }
            }

        private:
            InstanceScript* instance;
            uint8 SlipstreamPosition;
            bool isUltimate;
            bool isActive;

            uint64 linkedSlipstreamObjectGuid;
            uint64 linkedBossGuid;

            class MovementEvent : public BasicEvent
            {
                public:
                    MovementEvent(Unit* target) : _target(target) { }

                    bool Execute(uint64 execTime, uint32 diff)
                    {
                        const float distance = 50;
                        float angle = conclaveHomePositions[4].GetAngle(_target);
                        float x, y, z;
                        conclaveHomePositions[4].GetPosition(x, y, z);
                        x += cosf(angle) * distance;
                        y += sinf(angle) * distance;
                        _target->GetMotionMaster()->MoveJump(x, y, z, 100, 20);
                        return true;
                    }

                private:
                    Unit* _target;
            };
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_slipstream_raidAI(creature);
        }
};

class npc_fall_catcher : public CreatureScript
{
    public:
        npc_fall_catcher() : CreatureScript("npc_fall_catcher") { }

        struct npc_fall_catcherAI : public ScriptedAI
        {
            npc_fall_catcherAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                summoner->SetDisableGravity(true, true);

                float x, y, z, o;
                summoner->GetPosition(x, y, z, o);
                summoner->NearTeleportTo(x, y, z + 1, o);
                summoner->CastSpell(me, SPELL_CATCH_FALL_VEHICLE, true);
                me->GetPosition(x, y, z);
                me->GetMotionMaster()->MovePoint(1, x, y, 210);

                summoner->SetDisableGravity(false, true);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE && pointId == 1)
                {
                    if (TempSummon* summon = me->ToTempSummon())
                    {
                        if (Unit* summoner = summon->GetSummoner())
                        {
                            summoner->ExitVehicle();
                            summoner->CastSpell(summoner, SPELL_SLIPSTREAM_BUFF, true);
                            summoner->CastSpell(summoner, SPELL_CATCH_FALL_JUMP_TRIGGER, true);
                        }
                    }
                }
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_fall_catcherAI(creature);
        }
};

class at_slipstream : public AreaTriggerScript
{
    public:
        at_slipstream() : AreaTriggerScript("at_slipstream") { }

        bool OnTrigger(Player* player, const AreaTriggerEntry* /*trigger*/) OVERRIDE
        {
            if (!player->HasAura(SPELL_SLIPSTREAM_BUFF) && !player->HasAura(SPELL_CATCH_FALL_SUMMON_AURA))
                if (Creature* slipstream = player->FindNearestCreature(NPC_SLIPSTREAM, 100))
                    slipstream->AI()->SetGUID(player->GetGUID());
            return true;
        }
};

class at_catch_fall : public AreaTriggerScript
{
    public:
        at_catch_fall() : AreaTriggerScript("at_catch_fall") { }

        bool OnTrigger(Player* player, const AreaTriggerEntry* /*trigger*/) OVERRIDE
        {
            if (player->HasAura(SPELL_CATCH_FALL_SUMMON_AURA) || player->HasAura(SPELL_EYE_OF_THE_STORM))
                return true;

            if (Creature* trigger = player->FindNearestCreature(NPC_WORLD_TRIGGER_CATCH_FALL, 50000))
                trigger->CastSpell(player, SPELL_CATCH_FALL_SUMMON_TRIGGER, true);

            return true;
        }
};

void AddSC_throne_of_the_four_winds()
{
    new npc_slipstream_raid();
    new npc_fall_catcher();
    new at_slipstream();
    new at_catch_fall();
}