/*
* This file is part of the Legends of Azeroth Pandaria Project. See THANKS file for Copyright information
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
#include "InstanceScript.h"
#include "CreatureTextMgr.h"
#include "Vehicle.h"
#include "VehicleDefines.h"
#include "hour_of_twilight.h"
#include "Player.h"

enum Adds
{
    NPC_TWILIGHT_DRAKE                 = 55636,
    NPC_HARBRINGER_OF_TWILIGHT         = 55969,
    NPC_CHAMPION_OF_LIFE               = 55911,
    NPC_HARBRINGER_OF_DESTRUCTION      = 55967,
    NPC_CHAMPION_OF_TIME               = 55913,
    NPC_DARK_HAZE                      = 54628, // 102287  102255
    NPC_BLOOD                          = 54644,
    NPC_THROW_LOC                      = 54735,
    NPC_SHADOW_BORER                   = 54686,
    NPC_TWILIGHT_PORTAL                = 58233,

    NPC_EARTHEN_SHELL_TARGET           = 55445,
};

enum eEvents
{
    EVENT_INTRO                        = 1,
    EVENT_ESCORT                       = 2,
    EVENT_COMBAT_ARCURION              = 3,
    EVENT_LAVA_BURST                   = 4,
    EVENT_HEALING_WAVE                 = 5,
    EVENT_CHAIN_LIGHTNING              = 6,
    EVENT_ATTACK_FROZEN_SERVITOR       = 7,
    EVENT_ARCURION                     = 8,
    EVENT_BREAK_THE_WALL               = 9,
    EVENT_ESCORT_DIFF                  = 10,
    EVENT_GARROTE                      = 11, // Assassin
    EVENT_BACKSTAB                     = 12,
    EVENT_EVISCREATE                   = 13,
    EVENT_MORTAL_STRIKE                = 14, // Bruiser
    EVENT_STAGGERING_BLOW              = 15,
    EVENT_BEATDOWN                     = 16, // Thug
    EVENT_FREEZING_TRAP                = 17, // Ranger
    EVENT_THUNGERING_SHADOWS           = 18, // Shadow walker
    EVENT_MIND_FLAY                    = 19,
    EVENT_COMBAT                       = 20,
    EVENT_RISING_FIRE_TOTEM            = 21,
    EVENT_SIT_ON_LIFE_WARDEN           = 22,
    EVENT_RESSURECTED                  = 23,
    EVENT_SQUEEZE_LIFELESS             = 24,
    EVENT_TENTACLE_SMASH               = 25,
    EVENT_SHADOW_BORE                  = 26,
    EVENT_SEEKING_SHADOW               = 27,
    EVENT_SHADOW_VALLEY                = 28,
    EVENT_WATER_SHIELD                 = 29,
    EVENT_ESCORT_TO_WALL               = 30,
    EVENT_GALAKROND_REACH              = 31,
};

enum eSpells
{
    // Thrall
    SPELL_LAVA_BURST                   = 102475,
    SPELL_HEALING_WAVE                 = 103641,
    SPELL_CHAIN_LIGHTNING              = 104130,
    SPELL_GHOST_WOLF                   = 2645,
    SPELL_RISING_FIRE_TOTEM            = 108374, // dummy
    SPELL_RISING_FIRE_TOTEM_APPLY_AURA = 103817,
    SPELL_RISING_FIRE_TOTEM_COSMETIC   = 103813,
    SPELL_RESSURECTION                 = 145728,
    SPELL_WATER_SHIELD                 = 103688,
    SPELL_MOLTEN_FURY                  = 103905,

    // Arcurion
    SPELL_APPEARS                      = 104767, // for spawn
    SPELL_IMPALE                       = 104019,

    // Asira
    SPELL_GARROTE                      = 102925,
    SPELL_STEALTH                      = 102921,
    SPELL_EVISCERATE                   = 102967,
    SPELL_BACKSTAB                     = 102924,

    SPELL_CLEAVE                       = 103001,
    SPELL_MORTAL_STRIKE                = 103002,
    SPELL_STAGGERING_BLOW              = 103000,

    SPELL_LEAP                         = 102975,
    SPELL_FREEZING_TRAP                = 43415,

    SPELL_BEATDOWN                     = 102989,
    SPELL_BASH                         = 102990,

    SPELL_HUNGERING_SHADOWS            = 103021,
    SPELL_MIND_FLAY                    = 103024,
    SPELL_SHADOWFORM_COSMETIC          = 107904,

    // Life Warden
    SPELL_SADDLE                       = 103646,

    // Twilight drake
    SPELL_TWILIGHT_BREATH              = 104230,

    // Benedictus
    SPELL_SQUEEZE_LIFELESS             = 102861,
    SPELL_TENTACLE_SMASH               = 102848,

    SPELL_SHADOW_BORE                  = 102995,

    SPELL_SEEKING_SHADOWS              = 102984,
    SPELL_SHADOW_VOLLEY                = 102992,

    // Custom
    SPELL_TWILIGHT_PUDDLE_COSMETIC     = 102254,
    SPELL_GROW_VISUAL_COSMETIC         = 102255,

    // Teleports
    SPELL_TELEPORT_ARCURION_DEAD       = 108928,
    SPELL_TELEPORT_ASIRA_DEAD          = 108929,
    SPELL_TELEPORT_BENEDICTUS          = 108930,
};

enum eActions
{
    ACTION_ACTIVATE                    = 1,
    ACTION_SPAWN                       = 2,
    ACTION_SPAWN_PH2                   = 3,
    ACTION_SPAWN_PH3                   = 4,
    ACTION_MOVEMENT_RANDOM             = 5,
    ACTION_MOLTEN_FURY                 = 6,
};

enum eYells
{
    // Thrall intro
    TALK_INTRO_1                       = 0,
    TALK_INTRO_2                       = 1,
    TALK_INTRO_3                       = 2,
    TALK_INTRO_4                       = 3,
    TALK_INTRO_5                       = 4,

    // Thrall Arcurion
    TALK_ARCURION_1                    = 0,
    TALK_ARCURION_2                    = 1,
    TALK_ARCURION_3                    = 2,
    TALK_ARCURION_4                    = 3,
    TALK_ARCURION_5                    = 4,

    // Thrall Asira
    TALK_ASIRA_1                       = 0,
    TALK_ASIRA_2                       = 1,
    TALK_ASIRA_3                       = 2,
    TALK_ASIRA_4                       = 3,
    TALK_ASIRA_5                       = 4,
    TALK_ASIRA_6                       = 5,
    TALK_ASIRA_7                       = 6,
    TALK_ASIRA_8                       = 7,
    TALK_ASIRA_9                       = 8,

    // Thrall Benedictus
    TALK_BENEDICTUS_1                  = 0,
    TALK_BENEDICTUS_2                  = 1,

    // Twilight drake
    TALK_TWILIGHT_DRAKE                = 0
};

std::list <Creature*> ActivateNearestStalkers(Creature* me, uint64 ownerGUID, uint32 npc_entry)
{
    std::list<Creature*> StalkersInArea;

    Unit* owner = ObjectAccessor::GetUnit(*me, ownerGUID);

    if (!owner)
        return StalkersInArea;

    GetCreatureListWithEntryInGrid(StalkersInArea, owner, npc_entry, 47.0f);

    return StalkersInArea;
}

Creature* GetFrozenServitorTarget(Creature* me, uint64 ownerGUID)
{
    std::list <Creature*> FrozenServitorsInArea;
    Unit* owner = ObjectAccessor::GetUnit(*me, ownerGUID);

    if (!owner)
        return (Creature*)NULL;

    GetCreatureListWithEntryInGrid(FrozenServitorsInArea, owner, NPC_FROZEN_SERVITOR, 200.0f);

    if (FrozenServitorsInArea.empty())
        return (Creature*)NULL;

    return Skyfire::Containers::SelectRandomContainerElement(FrozenServitorsInArea);
}

uint32 GetCurrentPlayersCount(Creature* me, uint64 ownerGUID)
{
    std::list <Player*> PlayersInArea;
    Unit* owner = ObjectAccessor::GetUnit(*me, ownerGUID);

    if (!owner)
        return 0;

    GetPlayerListInGrid(PlayersInArea, owner, 200.0f);
    return PlayersInArea.size();
}

const Position GenerateMovement(Position pos)
{
    return { pos.GetPositionX() + frand(-2.5f, 2.5f), pos.GetPositionY() + frand(-3.0f, 3.0f), pos.GetPositionZ(), pos.GetOrientation() };
}

// Life warden 55549
class npc_hour_of_twilight_life_warden : public CreatureScript
{
    public:
        npc_hour_of_twilight_life_warden() : CreatureScript("npc_hour_of_twilight_life_warden") { }

        struct npc_hour_of_twilight_life_wardenAI : public ScriptedAI
        {
            npc_hour_of_twilight_life_wardenAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap nonCombatEvents;
            uint32 wp;
            bool SetVehicle;

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->SetCanFly(true);
                me->SetSpeed(MOVE_FLIGHT, 2.5f);
                me->SetSpeed(MOVE_RUN, 2.5f);
                me->UpdateSpeed(MOVE_RUN, true);

                Movement::MoveSplineInit init(me);
                for (auto itr : TaxiPathPrepare)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetSmooth();
                init.SetUncompressed();
                init.SetFly();
                init.Launch();
                nonCombatEvents.ScheduleEvent(EVENT_ESCORT_DIFF, me->GetSplineDuration());
            }

            void Reset() OVERRIDE { }

            void InitializeAI() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                wp = 0;
                SetVehicle = false;
                me->SetCanFly(true);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (!SetVehicle)
                    return;

                nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));
                wp++;

                switch (pointId)
                {
                    case 9:
                        me->SummonCreature(NPC_TWILIGHT_DRAKE, GenerateMovement(TwilightDrakePos), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        break;
                    case 11:
                        nonCombatEvents.Reset();
                        me->GetMotionMaster()->MovementExpired();
                        me->GetMotionMaster()->Clear();
                        break;
                    default:
                        break;
                }
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_REMOVE_ALL_PASSENGERS)
                {
                    if (Vehicle* Passengers = me->GetVehicleKit())
                        Passengers->RemoveAllPassengers(/*true*/);

                    me->SetCanFly(false);
                    me->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                }
            }

            void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
            {
                clicker->ToPlayer()->EnterVehicle(me, 0);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 1500);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ESCORT:
                            me->GetMotionMaster()->MovementExpired();
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(wp + 6, GenerateMovement(TaxiPath[wp]));
                            break;
                        case EVENT_ESCORT_DIFF:
                            nonCombatEvents.Reset();
                            wp = 0;
                            SetVehicle = true;
                            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                            break;
                    }
                }

                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_hour_of_twilight_life_wardenAI>(creature);
        }
};

// Life warden Thrall | Asira 55415
class npc_hour_of_twilight_life_warden_thrall : public CreatureScript
{
    public:
        npc_hour_of_twilight_life_warden_thrall() : CreatureScript("npc_hour_of_twilight_life_warden_thrall") { }

        struct npc_hour_of_twilight_life_warden_thrallAI : public ScriptedAI
        {
            npc_hour_of_twilight_life_warden_thrallAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap nonCombatEvents;
            InstanceScript* instance;
            uint32 wp;

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->AddAura(SPELL_SADDLE, me);
                nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 1000);
            }

            void InitializeAI() OVERRIDE
            {
                instance = me->GetInstanceScript();
                me->SetReactState(REACT_PASSIVE);
                wp = 0;
                me->SetCanFly(true);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                wp++;
                nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));

                switch (pointId)
                {
                    case 2:
                        if (Creature* Thrall = GetClosestCreatureWithEntry(me, NPC_THRALL_3, 150.0f))
                            Thrall->AI()->DoAction(ACTION_SUMMONED_ASIRA);
                        break;
                    case 4:
                        me->SummonCreature(NPC_ASIRA, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        break;
                    case 5:
                        nonCombatEvents.Reset();
                        me->GetMotionMaster()->MovementExpired();
                        me->GetMotionMaster()->Clear();

                        if (instance)
                            if (Creature* Asira = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ASIRA)))
                                Asira->AI()->DoAction(ACTION_ASIRA_FALL);

                            me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), -6.48f, me->GetOrientation());
                            me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                            break;
                    case 6:
                        me->SummonCreature(NPC_THRALL_4, ThrallLastSpawnPoint, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                        if (Creature* Thrall = GetClosestCreatureWithEntry(me, NPC_THRALL_3, 10.0f))
                            Thrall->DespawnOrUnsummon();

                        me->DespawnOrUnsummon();
                        break;
                }
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summon->EnterVehicle(me, 0);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_FLY_TO_DRAGON_SOUL)
                {
                    me->SetCanFly(true);
                    me->GetMotionMaster()->MovementExpired();
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(6, AsiraSpawnPoint);
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_ESCORT)
                    {
                        me->GetMotionMaster()->MovementExpired();
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MovePoint(wp, GenerateMovement(AsiraTaxiPath[wp]));
                    }
                    break;
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_hour_of_twilight_life_warden_thrallAI>(creature);
        }
};

// Twilight drake 55636
class npc_hour_of_twilight_twilight_drake : public CreatureScript
{
    public:
        npc_hour_of_twilight_twilight_drake() : CreatureScript("npc_hour_of_twilight_twilight_drake") { }

        struct npc_hour_of_twilight_twilight_drakeAI : public ScriptedAI
        {
            npc_hour_of_twilight_twilight_drakeAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap nonCombatEvents;
            uint32 wp;
            Creature* LifeWarden;

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 1500);
                LifeWarden = summoner->ToCreature();
                me->SetSpeed(MOVE_FLIGHT, 2.0f);
            }

            void Reset() OVERRIDE { }

            void InitializeAI() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                wp = 0;
                me->SetCanFly(true);
                me->SetDisableGravity(true);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                wp++;
                nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));

                if (pointId == 2)
                {
                    nonCombatEvents.Reset();
                    me->GetMotionMaster()->Clear();

                    if (LifeWarden)
                    {
                        me->CastSpell(LifeWarden, SPELL_TWILIGHT_BREATH, false);
                        LifeWarden->AI()->DoAction(ACTION_REMOVE_ALL_PASSENGERS);
                    }

                    Talk(TALK_TWILIGHT_DRAKE);
                    nonCombatEvents.ScheduleEvent(EVENT_ESCORT_DIFF, 2500);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ESCORT:
                            me->GetMotionMaster()->MovementExpired();
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(wp, GenerateMovement(TwilightDrakeTaxi[wp]));
                            break;
                        case EVENT_ESCORT_DIFF:
                            me->DespawnOrUnsummon();
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_hour_of_twilight_twilight_drakeAI>(creature);
        }
};

// Thrall Intro 54548
class npc_thrall_intro : public CreatureScript
{
    public:
        npc_thrall_intro() : CreatureScript("npc_thrall_intro") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                creature->AI()->DoAction(ACTION_ACTIVATE);

            player->CLOSE_GOSSIP_MENU();

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, THRALL_INTRO, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(18287, creature->GetGUID());
            return true;
        }

        struct npc_thrall_introAI : public ScriptedAI
        {
            npc_thrall_introAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

            InstanceScript* instance;
            EventMap events, nonCombatEvents;
            uint32 timer;
            uint32 wp;

            void Reset() OVERRIDE
            {
                events.Reset();
                nonCombatEvents.Reset();
                nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 1500);
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_ACTIVATE:
                        Talk(TALK_INTRO_1);
                        SetEquipmentSlots(false, DoomHammer, 0, EQUIP_NO_CHANGE);
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        nonCombatEvents.ScheduleEvent(EVENT_INTRO, 6500);
                        break;
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_LAVA_BURST, 1500);
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 30000);
                events.ScheduleEvent(EVENT_HEALING_WAVE, 50000);
            }

            void InitializeAI() OVERRIDE
            {
                wp = 0;
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                wp++;
                nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));

                switch (pointId)
                {
                    case 2:
                        Talk(TALK_INTRO_3);
                        Talk(TALK_INTRO_4);

                        for (auto itr : ActivateNearestStalkers(me, me->GetGUID(), NPC_FROZEN_SERVITOR_SPAWN_STALKER_2))
                            itr->AI()->DoAction(ACTION_SPAWN);

                        for (auto itr : ActivateNearestStalkers(me, me->GetGUID(), NPC_FROZEN_SERVITOR_SPAWN_STALKER))
                            itr->AI()->DoAction(ACTION_SPAWN);
                        break;
                    case 4:
                        me->GetMotionMaster()->Clear();
                        nonCombatEvents.Reset();
                        Talk(TALK_INTRO_5);
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                        me->SummonCreature(NPC_THRALL_2, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO:
                        {
                            for (auto itr : ActivateNearestStalkers(me, me->GetGUID(), NPC_FROZEN_SERVITOR_STALKER_ENTRANCE))
                                itr->AI()->DoAction(ACTION_SPAWN);
                            me->SetFacingTo(me->GetOrientation() + M_PI);
                            Talk(TALK_INTRO_2);
                            nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 2500);
                            break;
                        }
                        case EVENT_ESCORT:
                        {
                            if (me->IsInCombat())
                                return;
                            me->GetMotionMaster()->MovementExpired();
                            me->GetMotionMaster()->Clear();
                            if (me->GetUnitMovementFlags() == MOVEMENTFLAG_WALKING)
                                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                            me->GetMotionMaster()->MovePoint(wp, ThrallWaypoints_Arcurion[wp]);
                        }
                            break;
                        default:
                            break;
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
                        case EVENT_LAVA_BURST:
                            if (Unit* itr = me->GetVictim())
                                 me->CastSpell(itr, SPELL_LAVA_BURST, false);
                            events.ScheduleEvent(EVENT_LAVA_BURST, 10000);
                            break;
                        case EVENT_CHAIN_LIGHTNING:
                            if (Unit* itr = me->GetVictim())
                                  me->CastSpell(itr, SPELL_CHAIN_LIGHTNING, false);
                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 30000);
                            break;
                        case EVENT_HEALING_WAVE:
                            if (HealthBelowPct(50))
                                me->CastSpell(me, SPELL_HEALING_WAVE, false);
                            events.ScheduleEvent(EVENT_HEALING_WAVE, 50000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();

            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_thrall_introAI>(creature);
        }
};

// Thrall Arcurion 55779
class npc_thrall_arcurion : public CreatureScript
{
    public:
        npc_thrall_arcurion() : CreatureScript("npc_thrall_arcurion") { }

        bool activated = false;

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                creature->AI()->DoAction(ACTION_ACTIVATE);

            player->CLOSE_GOSSIP_MENU();

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, THRALL_ARCURION, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(18555, creature->GetGUID());
            return true;
        }

        struct npc_thrall_arcurionAI : public ScriptedAI
        {
            npc_thrall_arcurionAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()), summons(me) { }

            InstanceScript* instance;
            EventMap events, nonCombatEvents;
            SummonList summons;
            uint32 timer;
            uint32 wp;
            bool phase;

            void InitializeAI() OVERRIDE
            {
                SetEquipmentSlots(true, DoomHammer, 0, EQUIP_NO_CHANGE);
                wp           = 0;
                phase        = false;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                DoZoneInCombat();
                me->HandleEmoteStateCommand(EMOTE_STATE_READY1H);
            }

            void Reset() OVERRIDE
            {
                if (instance && instance->GetBossState(DATA_ARCURION) == DONE)
                {
                    MovementFlagWithoutWalking();
                    me->GetMotionMaster()->MovePoint(21, BreakTheWall);
                    nonCombatEvents.ScheduleEvent(EVENT_ESCORT_TO_WALL, me->GetSplineDuration() + 500);
                }
            }

            void MovementFlagWithoutWalking()
            {
                me->GetMotionMaster()->MovementExpired();
                me->GetMotionMaster()->Clear();

                if (me->GetUnitMovementFlags() == MOVEMENTFLAG_WALKING)
                    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);

                if (summon->GetEntry() == NPC_RISING_FIRE_TOTEM)
                    summon->CastSpell(summon, SPELL_RISING_FIRE_TOTEM_COSMETIC, false);
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_ACTIVATE:
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                        nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 1000);
                        break;
                    case ACTION_ARCURION:
                        Talk(TALK_ARCURION_2);
                        nonCombatEvents.ScheduleEvent(EVENT_ARCURION, 1000);
                        break;
                    case ACTION_ICY_TOMB:
                        me->AddUnitState(UNIT_STATE_STUNNED);
                        events.Reset();
                        break;
                    case ACTION_ICY_TOMB_REMOVE:
                        me->ClearUnitState(UNIT_STATE_STUNNED);
                        events.ScheduleEvent(EVENT_ATTACK_FROZEN_SERVITOR, 1500);
                        break;
                    case ACTION_TALK_FINISH_HIM:
                        events.CancelEvent(EVENT_ATTACK_FROZEN_SERVITOR);
                        events.ScheduleEvent(EVENT_LAVA_BURST, 2000);
                        Talk(TALK_ARCURION_3);
                        break;
                    case ACTION_TALK_ARCURION_DEATH:
                        summons.DespawnEntry(NPC_ICY_TOMB);
                        events.Reset();
                        me->getHostileRefManager().deleteReferences();
                        me->getThreatManager().clearReferences();

                        if (instance)
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                        Talk(TALK_ARCURION_4);
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == 20)
                {
                    me->GetMotionMaster()->Clear();
                    me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                    nonCombatEvents.Reset();
                    Talk(TALK_ARCURION_1);
                    if (Creature* ArcurionStalker = GetClosestCreatureWithEntry(me, NPC_ARCURION_SPAWN_VISIAL_STALKER, 100.0f, true))
                    {
                        ArcurionStalker->CastSpell(ArcurionStalker, SPELL_APPEARS, false);
                        ArcurionStalker->SummonCreature(NPC_ARCURION, ArcurionStalker->GetPositionX(), ArcurionStalker->GetPositionY(), ArcurionStalker->GetPositionZ(), ArcurionStalker->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    }
                    me->HandleEmoteStateCommand(EMOTE_STATE_READY1H);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ESCORT:
                            MovementFlagWithoutWalking();
                            me->GetMotionMaster()->MovePoint(20, ThrallWaypoints_ArcurionPrepare);
                            break;
                        case EVENT_ESCORT_DIFF:
                        {
                            DoCast(me, SPELL_GHOST_WOLF);

                            Movement::MoveSplineInit init(me);
                            for (auto itr : ThrallWaypoints_Asira)
                                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                            init.SetSmooth();
                            init.SetUncompressed();
                            init.Launch();
                            nonCombatEvents.ScheduleEvent(EVENT_GALAKROND_REACH, me->GetSplineDuration());
                            break;
                        }
                        case EVENT_ARCURION:
                            me->SetReactState(REACT_PASSIVE);
                            DoZoneInCombat();
                            events.ScheduleEvent(EVENT_ATTACK_FROZEN_SERVITOR, 1500);
                            break;
                        case EVENT_ESCORT_TO_WALL:
                            me->GetMotionMaster()->Clear();
                            DoCast(me, SPELL_MOLTEN_FURY);
                            phase = true;
                            wp = 0;
                            Talk(TALK_ARCURION_5);
                            nonCombatEvents.ScheduleEvent(EVENT_ESCORT_DIFF, 11000);
                            break;
                        case EVENT_GALAKROND_REACH:
                            me->SummonCreature(NPC_THRALL_3, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            me->DespawnOrUnsummon();
                            break;
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
                        case EVENT_LAVA_BURST:
                            if (Creature* Arc = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ARCURION)))
                                me->CastSpell(Arc, SPELL_LAVA_BURST, false);

                            events.ScheduleEvent(EVENT_LAVA_BURST, 6000);
                            break;
                        case EVENT_ATTACK_FROZEN_SERVITOR:
                            if (Creature* FrozenServitor = GetFrozenServitorTarget(me, me->GetGUID()))
                                me->CastSpell(FrozenServitor, SPELL_LAVA_BURST, false);

                            events.ScheduleEvent(EVENT_ATTACK_FROZEN_SERVITOR, 5000);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_thrall_arcurionAI>(creature);
        }
};

// Thrall Asira 54972
class npc_thrall_asira : public CreatureScript
{
    public:
        npc_thrall_asira() : CreatureScript("npc_thrall_asira") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                creature->AI()->DoAction(ACTION_ACTIVATE);

            player->CLOSE_GOSSIP_MENU();

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, THRALL_ASIRA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(18287, creature->GetGUID());
            return true;
        }

        struct npc_thrall_asiraAI : public ScriptedAI
        {
            npc_thrall_asiraAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

            EventMap events, nonCombatEvents;
            uint32 timer;
            uint32 wp;

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->HandleEmoteStateCommand(EMOTE_STATE_CANNIBALIZE);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                nonCombatEvents.Reset();
                instance->GetBossState(DATA_ASIRA) == DONE ? nonCombatEvents.ScheduleEvent(EVENT_ESCORT_DIFF, 1500) : nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 1500);
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_ACTIVATE:
                        SetEquipmentSlots(false, DoomHammer, 0, EQUIP_NO_CHANGE);
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        me->HandleEmoteStateCommand(EMOTE_STATE_NONE);

                        if (instance)
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                        nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 1000);
                        break;
                    case ACTION_MOVEMENT_RANDOM:
                        Talk(TALK_ASIRA_5);
                        me->GetMotionMaster()->MoveRandom(4.0f);
                        me->SummonCreature(NPC_LIFE_WARDEN_1, AsiraSpawnPoint, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        break;
                    case ACTION_SUMMONED_ASIRA:
                        me->GetMotionMaster()->MoveIdle();
                        me->GetMotionMaster()->Clear();

                        if (Creature* LifeWarden = GetClosestCreatureWithEntry(me, NPC_LIFE_WARDEN_1, 150.0f))
                            me->SetFacingTo(me->GetAngle(LifeWarden));

                        Talk(TALK_ASIRA_6);
                        me->HandleEmoteStateCommand(EMOTE_STATE_READY1H);
                        break;
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_LAVA_BURST, 1500);
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 30000);
                events.ScheduleEvent(EVENT_HEALING_WAVE, 50000);
                events.ScheduleEvent(EVENT_RISING_FIRE_TOTEM, 5500);
            }

            void InitializeAI() OVERRIDE
            {
                wp = 0;
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (instance->GetBossState(DATA_ASIRA) != DONE)
                {
                    wp++;
                    nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));
                }

                switch (pointId)
                {
                    case 3:
                        Talk(TALK_ASIRA_1);
                        if (Creature* stalker = GetClosestCreatureWithEntry(me, NPC_ASIRA_TRASH_STALKER_PH1, 50.0f))
                            stalker->AI()->DoAction(ACTION_SPAWN);
                        break;
                    case 4:
                        Talk(TALK_ASIRA_2);
                        break;
                    case 7:
                        Talk(TALK_ASIRA_3);
                        if (Creature* stalker = GetClosestCreatureWithEntry(me, NPC_ASIRA_TRASH_STALKER_PH1, 50.0f))
                            stalker->AI()->DoAction(ACTION_SPAWN_PH2);
                        break;
                    case 9:
                        Talk(TALK_ASIRA_4);
                        if (Creature* stalker = GetClosestCreatureWithEntry(me, NPC_ASIRA_TRASH_STALKER_PH1, 70.0f))
                            stalker->AI()->DoAction(ACTION_SPAWN_PH3);
                        break;
                    case 10:
                        me->GetMotionMaster()->Clear();
                        me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                        nonCombatEvents.Reset();
                        DoAction(ACTION_MOVEMENT_RANDOM);
                        break;
                    case 11:
                        if (Creature* LifeWarden = GetClosestCreatureWithEntry(me, NPC_LIFE_WARDEN_1, 10.0f))
                            me->CastSpell(LifeWarden, SPELL_RESSURECTION, false);

                        nonCombatEvents.ScheduleEvent(EVENT_RESSURECTED, 10000);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ESCORT:
                            if (me->IsInCombat())
                                return;
                            // Moving Without Walk...
                            me->GetMotionMaster()->MovementExpired();
                            me->GetMotionMaster()->Clear();

                            if (me->GetUnitMovementFlags() == MOVEMENTFLAG_WALKING)
                                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);

                            me->GetMotionMaster()->MovePoint(wp, ThrallWaypoints_Asira_Prepare[wp]);
                            break;
                        case EVENT_ESCORT_DIFF:
                            if (Creature* LifeWarden = GetClosestCreatureWithEntry(me, NPC_LIFE_WARDEN_1, 150.0f))
                            {
                                Position pos = { LifeWarden->GetPositionX(), LifeWarden->GetPositionY(), LifeWarden->GetPositionZ(), LifeWarden->GetOrientation() };
                                me->GetMotionMaster()->MovePoint(11, GenerateMovement(pos));
                            }
                            break;
                        case EVENT_RESSURECTED:
                            if (Creature* LifeWarden = GetClosestCreatureWithEntry(me, NPC_LIFE_WARDEN_1, 20.0f))
                            {
                                LifeWarden->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                LifeWarden->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                LifeWarden->SetStandState(UNIT_STAND_STATE_STAND);

                                if (!me->IsOnVehicle())
                                    me->EnterVehicle(LifeWarden, 0);

                                me->HandleEmoteStateCommand(EMOTE_STATE_MOUNT);
                            }
                            nonCombatEvents.ScheduleEvent(EVENT_SIT_ON_LIFE_WARDEN, 1500);
                            break;
                        case EVENT_SIT_ON_LIFE_WARDEN:
                            Talk(TALK_ASIRA_9);
                            if (Creature* LifeWarden = GetClosestCreatureWithEntry(me, NPC_LIFE_WARDEN_1, 20.0f))
                                LifeWarden->AI()->DoAction(ACTION_FLY_TO_DRAGON_SOUL);

                            for (uint32 i = 0; i < GetCurrentPlayersCount(me, me->GetGUID()); i++)
                                me->SummonCreature(NPC_LIFE_WARDEN_2, GenerateMovement(drakePos), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            break;
                        default:
                            break;
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
                        case EVENT_LAVA_BURST:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_LAVA_BURST, false);

                            events.ScheduleEvent(EVENT_LAVA_BURST, 10000);
                            break;
                        case EVENT_CHAIN_LIGHTNING:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_CHAIN_LIGHTNING, false);

                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 30000);
                            break;
                        case EVENT_HEALING_WAVE:
                            if (HealthBelowPct(50))
                                me->CastSpell(me, SPELL_HEALING_WAVE, false);

                            events.ScheduleEvent(EVENT_HEALING_WAVE, 50000);
                            break;
                        case EVENT_RISING_FIRE_TOTEM:
                            me->CastSpell(me, SPELL_RISING_FIRE_TOTEM, false);
                            break;
                    }
                }

                DoMeleeAttackIfReady();

            }

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_thrall_asiraAI>(creature);
        }
};

// Thrall Trash Benedictus 54634
class npc_thrall_trash_benedictus : public CreatureScript
{
    public:
        npc_thrall_trash_benedictus() : CreatureScript("npc_thrall_trash_benedictus") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                creature->AI()->DoAction(ACTION_ACTIVATE);

            player->CLOSE_GOSSIP_MENU();

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, THRALL_BEFORE_BENEDICTUS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(18293, creature->GetGUID());
            return true;
        }

        struct npc_thrall_trash_benedictusAI : public ScriptedAI
        {
            npc_thrall_trash_benedictusAI(Creature* creature) : ScriptedAI(creature) { }
            EventMap nonCombatEvents;
            uint32 timer, wp, tentacle;
            bool ToDragonSoul, LastStalker;

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                LastStalker = false;
                ToDragonSoul = false;
                me->SetReactState(REACT_PASSIVE);
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_ACTIVATE:
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        SetEquipmentSlots(false, DoomHammer, 0, EQUIP_NO_CHANGE);
                        nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 1500);
                        break;
                    case ACTION_BENEDICTUS:
                        wp = 0;
                        ToDragonSoul = true;
                        nonCombatEvents.ScheduleEvent(EVENT_ESCORT_DIFF, 3500);
                        break;
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE { }

            void InitializeAI() OVERRIDE
            {
                wp = 0;
                tentacle = 0;
                ToDragonSoul = false;
                me->SetReactState(REACT_PASSIVE);  // temp
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); // temp
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                wp++;

                switch (pointId)
                {
                    case 2:
                    case 3:
                        for (auto itr : ActivateNearestStalkers(me, me->GetGUID(), NPC_DARK_HAZE_STALKER))
                            itr->AI()->DoAction(ACTION_SPAWN);

                        me->SummonCreature(NPC_FLAILING_TENTACLE, TentacleSpawnPos[tentacle], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        tentacle++;
                        me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                        nonCombatEvents.ScheduleEvent(EVENT_COMBAT, 1000);
                        break;
                    case 6:
                        for (auto itr : ActivateNearestStalkers(me, me->GetGUID(), NPC_DARK_HAZE_STALKER))
                            itr->AI()->DoAction(ACTION_SPAWN);

                        me->SummonCreature(NPC_FLAILING_TENTACLE, TentacleSpawnPos[tentacle], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        tentacle++;
                        me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                        LastStalker = true;
                        nonCombatEvents.ScheduleEvent(EVENT_COMBAT, 1000);
                        break;
                    case 13:
                        me->SummonCreature(NPC_THRALL_5, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        ToDragonSoul ? nonCombatEvents.ScheduleEvent(EVENT_ESCORT_DIFF, urand(100, 200)) : nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));
                        break;
                }
            }

            bool HasInCombat()
            {
                Creature* Brute = GetClosestCreatureWithEntry(me, NPC_FACELESS_BRUTE, 200.0f, true);
                Creature* Borer = GetClosestCreatureWithEntry(me, NPC_FACELESS_BORER, 200.0f, true);
                Creature* ShadowWeaver = GetClosestCreatureWithEntry(me, NPC_FACELESS_SHADOW_WEAVER, 200.0f, true);
                Creature* Tentacle = GetClosestCreatureWithEntry(me, NPC_FLAILING_TENTACLE, 200.0f, true);

                if (Brute || Borer || ShadowWeaver || Tentacle)
                    return true;

                return false;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ESCORT:
                        {
                            if (HasInCombat())
                                return;

                            me->GetMotionMaster()->MovementExpired();
                            me->GetMotionMaster()->Clear();

                            if (me->GetUnitMovementFlags() == MOVEMENTFLAG_WALKING)
                                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);

                            me->GetMotionMaster()->MovePoint(wp, ThrallWaypoints_Benedict_Prepare[wp]);
                            break;
                        }
                        case EVENT_COMBAT:
                        {
                            Creature* Brute = GetClosestCreatureWithEntry(me, NPC_FACELESS_BRUTE, 200.0f, true);
                            Creature* Borer = GetClosestCreatureWithEntry(me,NPC_FACELESS_BORER, 200.0f, true);
                            Creature* ShadowWeaver = GetClosestCreatureWithEntry(me, NPC_FACELESS_SHADOW_WEAVER, 200.0f, true);
                            Creature* Tentacle = GetClosestCreatureWithEntry(me, NPC_FLAILING_TENTACLE, 200.0f, true);

                            if (HasInCombat())
                            {
                                nonCombatEvents.ScheduleEvent(EVENT_COMBAT, urand(100, 200));
                                return;
                            }
                            nonCombatEvents.Reset();

                            if (LastStalker)
                                me->SummonCreature(NPC_BENEDICTUS, BenedictusSpawnPoint, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            else
                                nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 1000);
                            break;
                        }
                        case EVENT_ESCORT_DIFF:
                        {
                            if (me->IsInCombat())
                                return;

                            me->GetMotionMaster()->MovementExpired();
                            me->GetMotionMaster()->Clear();

                            if (me->GetUnitMovementFlags() == MOVEMENTFLAG_WALKING)
                                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);

                            me->GetMotionMaster()->MovePoint(wp + 7, ThrallWaypoints_To_Dragon_Soul[wp]);
                            break;
                        }
                        default:
                            break;
                    }
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_thrall_trash_benedictusAI>(creature);
        }
};

// Thrall Benedictus 54971
class npc_thrall_benedictus : public CreatureScript
{
    public:
        npc_thrall_benedictus() : CreatureScript("npc_thrall_benedictus") { }

        struct npc_thrall_benedictusAI : public ScriptedAI
        {
            npc_thrall_benedictusAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events, nonCombatEvents;
            uint32 timer;

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void Reset() OVERRIDE { }

            void InitializeAI() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_LAVA_BURST, 1500);
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 30000);
                events.ScheduleEvent(EVENT_HEALING_WAVE, 50000);
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_BENEDICTUS:
                        Talk(TALK_BENEDICTUS_1);
                        break;
                    case ACTION_TALK_FINISH_HIM:
                        Talk(TALK_BENEDICTUS_2);
                        break;
                    case ACTION_COMBAT:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->SetReactState(REACT_AGGRESSIVE);
                        DoZoneInCombat();
                        break;
                    case ACTION_WATER_SHIELD:
                        events.ScheduleEvent(EVENT_WATER_SHIELD, 1000);
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = 0;
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
                    switch (eventId)
                    {
                        case EVENT_LAVA_BURST:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_LAVA_BURST, false);
                            events.ScheduleEvent(EVENT_LAVA_BURST, 10000);
                            break;
                        case EVENT_CHAIN_LIGHTNING:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_CHAIN_LIGHTNING, false);
                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 30000);
                            break;
                        case EVENT_HEALING_WAVE:
                            if (HealthBelowPct(50))
                                me->CastSpell(me, SPELL_HEALING_WAVE, false);
                            events.ScheduleEvent(EVENT_HEALING_WAVE, 50000);
                            break;
                        case EVENT_WATER_SHIELD:
                            DoCastAOE(SPELL_WATER_SHIELD);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_thrall_benedictusAI>(creature);
        }
};

enum Texts
{
    EMOTE_DARK_HAZE                     = 0,
};

// Frozen Servitors stalker 55389, 54560, 55384, 55385, 54598
// Asira stalkers 53488, 3570621, 3570622
// Faceless stalkers 54628, 3570623
class npc_frozen_servitor_stalker : public CreatureScript
{
    public:
        npc_frozen_servitor_stalker() : CreatureScript("npc_frozen_servitor_stalker") { }

        struct npc_frozen_servitor_stalkerAI : public ScriptedAI
        {
            npc_frozen_servitor_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap nonCombatEvents;

            void Reset() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void InitializeAI() OVERRIDE
            {
                if (me->GetEntry() == NPC_DARK_HAZE_STALKER)
                    nonCombatEvents.ScheduleEvent(EVENT_INTRO, urand(8 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));

                Reset();
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_SPAWN:
                    {
                        switch (me->GetEntry())
                        {
                            case NPC_FROZEN_SERVITOR_STALKER_ENTRANCE:
                            case NPC_FROZEN_SERVITOR_SPAWN_STALKER_2:
                                me->CastSpell(me, SPELL_APPEARS, false);
                                me->SummonCreature(NPC_FROZEN_SERVITOR, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                break;
                            case NPC_FROZEN_SERVITOR_SPAWN_STALKER:
                                me->SummonCreature(NPC_CRYSTALLINE_ELEMENTAL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                break;
                            case NPC_ASIRA_TRASH_STALKER_PH1:
                                me->SummonCreature(NPC_TWILIGHT_ASSASSIN, me->GetPositionX() + frand(-15.0f, 15.0f), me->GetPositionY() + frand(-5.0f, 5.0f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                me->SummonCreature(NPC_TWILIGHT_BRUISER, me->GetPositionX() + frand(-15.0f, 15.0f), me->GetPositionY() + frand(-5.0f, 5.0f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                me->SummonCreature(NPC_TWILIGHT_RANGER, me->GetPositionX() + frand(-15.0f, 15.0f), me->GetPositionY() + frand(-5.0f, 5.0f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                me->SummonCreature(NPC_TWILIGHT_THUG, me->GetPositionX() + frand(-15.0f, 15.0f), me->GetPositionY() + frand(-5.0f, 5.0f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                me->SummonCreature(NPC_TWILIGHT_SHADOW_WALKER, me->GetPositionX() + frand(-15.0f, 15.0f), me->GetPositionY() + frand(-5.0f, 5.0f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                break;
                            case NPC_DARK_HAZE_STALKER:
                                if (me->HasAura(SPELL_GROW_VISUAL_COSMETIC))
                                    me->RemoveAura(SPELL_GROW_VISUAL_COSMETIC);
                                Talk(EMOTE_DARK_HAZE);
                                me->SummonCreature(NPC_FACELESS_BORER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                me->SummonCreature(NPC_FACELESS_SHADOW_WEAVER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                me->SummonCreature(NPC_FACELESS_BRUTE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                break;
                        }
                        break;
                    }
                    case ACTION_SPAWN_PH2:
                    {
                        me->SummonCreature(NPC_TWILIGHT_ASSASSIN, me->GetPositionX() + frand(-3.0f, 3.0f), me->GetPositionY() + frand(-1.5f, 1.5f), me->GetPositionZ() + frand(1.0f, 2.0f), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        me->SummonCreature(NPC_TWILIGHT_SHADOW_WALKER, me->GetPositionX() + frand(-3.0f, 3.0f), me->GetPositionY() + frand(-1.5f, 1.5f), me->GetPositionZ() + frand(1.0f, 2.0f), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        me->SummonCreature(NPC_TWILIGHT_BRUISER, me->GetPositionX() + frand(-3.0f, 3.0f), me->GetPositionY() + frand(-1.5f, 1.5f), me->GetPositionZ() + frand(1.0f, 2.0f), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        break;
                    }
                    case ACTION_SPAWN_PH3:
                    {
                        me->SummonCreature(NPC_TWILIGHT_BRUISER, me->GetPositionX() + frand(-1.5f, -1.0f), me->GetPositionY() + frand(-3.0f, 3.0f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        me->SummonCreature(NPC_TWILIGHT_RANGER, me->GetPositionX() + frand(-1.5f, -1.0f), me->GetPositionY() + frand(-3.0f, 3.0f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        me->SummonCreature(NPC_TWILIGHT_SHADOW_WALKER, me->GetPositionX() + frand(-1.5f, -1.0f), me->GetPositionY() + frand(-3.0f, 3.0f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        me->SummonCreature(NPC_TWILIGHT_SHADOW_WALKER, me->GetPositionX() + frand(-1.5f, -1.0f), me->GetPositionY() + frand(-3.0f, 3.0f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        break;
                    }
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_INTRO)
                    {
                        me->SummonCreature(NPC_CORRUPTED_SLIME, me->GetPositionX() + frand(-1.5f, -1.0f), me->GetPositionY() + frand(-3.0f, 3.0f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        nonCombatEvents.ScheduleEvent(EVENT_INTRO, urand(8 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                    }
                    break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_frozen_servitor_stalkerAI>(creature);
        }
};

class npc_twilights_hammer : public CreatureScript
{
    public:
        npc_twilights_hammer() : CreatureScript("npc_twilights_hammer") { }

        struct npc_twilights_hammerAI : public ScriptedAI
        {
            npc_twilights_hammerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events, nonCombatEvents;
            uint32 timer;

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                if (summoner->GetPositionZ()<12.0f)
                    nonCombatEvents.ScheduleEvent(EVENT_COMBAT, 1000);
                else
                {
                    me->SetHomePosition(me->GetPositionX() + frand(15.0f, 20.0f), me->GetPositionY() + frand(15.0f, 20.0f), -6.36f, me->GetOrientation());
                    me->GetMotionMaster()->MoveJump(me->GetPositionX() + frand(15.0f, 20.0f), me->GetPositionY() + frand(15.0f, 20.0f), -6.36f, 30.0f, 20.0f, EVENT_JUMP);
                    nonCombatEvents.ScheduleEvent(EVENT_COMBAT, 4000);
                    if (me->GetEntry() == NPC_TWILIGHT_RANGER)
                        Talk(0);
                }
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                switch (me->GetEntry())
                {
                    case NPC_TWILIGHT_ASSASSIN:
                        events.ScheduleEvent(EVENT_GARROTE, 1000);
                        events.ScheduleEvent(EVENT_BACKSTAB, 5000);
                        events.ScheduleEvent(EVENT_EVISCREATE, 7000);
                        break;
                    case NPC_TWILIGHT_BRUISER:
                        events.ScheduleEvent(EVENT_MORTAL_STRIKE, 5000);
                        events.ScheduleEvent(EVENT_STAGGERING_BLOW, 10000);
                        break;
                    case NPC_TWILIGHT_RANGER:
                        events.ScheduleEvent(EVENT_FREEZING_TRAP, 1);
                        break;
                    case NPC_TWILIGHT_THUG:
                        events.ScheduleEvent(EVENT_BEATDOWN, urand(7000, 10000));
                        break;
                    case NPC_TWILIGHT_SHADOW_WALKER:
                        events.ScheduleEvent(EVENT_MIND_FLAY, 1000);
                        events.ScheduleEvent(EVENT_THUNGERING_SHADOWS, urand(9000, 13000));
                        break;
                    default:
                        break;
                }
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void InitializeAI() OVERRIDE
            {
                if (me->GetEntry() == NPC_TWILIGHT_ASSASSIN)
                    SetEquipmentSlots(false, AsiraDaggers, AsiraDaggers, EQUIP_NO_CHANGE);
            }

            void DoAction(int32 /*action*/) OVERRIDE { }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_COMBAT)
                    {
                        if (!me->IsInCombat())
                            if (Creature* Thrall = GetClosestCreatureWithEntry(me, NPC_THRALL_3, 80.0f))
                            {
                                me->SetInCombatWith(Thrall);
                                me->Attack(Thrall, true);

                                if (me->GetDistance(Thrall) > 7)
                                    me->GetMotionMaster()->MoveChase(Thrall);
                            }
                    }
                    break;
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
                        case EVENT_GARROTE:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_GARROTE, true);
                            break;
                        case EVENT_BACKSTAB:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_BACKSTAB, true);

                            events.ScheduleEvent(EVENT_BACKSTAB, 10000);
                            break;
                        case EVENT_EVISCREATE:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_EVISCERATE, true);

                            events.ScheduleEvent(EVENT_EVISCREATE, urand(6000, 7000));
                            break;
                        case EVENT_MORTAL_STRIKE:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_MORTAL_STRIKE, true);

                            events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(6000, 7000));
                            break;
                        case EVENT_STAGGERING_BLOW:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_STAGGERING_BLOW, true);

                            events.ScheduleEvent(EVENT_STAGGERING_BLOW, 10000);
                            break;
                        case EVENT_BEATDOWN:
                            if (Unit* itr = me->GetVictim())
                            {
                                me->CastSpell(itr, SPELL_BEATDOWN, true);
                                me->CastSpell(itr, SPELL_BASH, true);
                            }
                            events.ScheduleEvent(EVENT_BEATDOWN, urand(10000, 15000));
                            break;
                        case EVENT_FREEZING_TRAP:
                            break;
                        case EVENT_THUNGERING_SHADOWS:
                            me->CastSpell(me, SPELL_HUNGERING_SHADOWS, false);
                            events.ScheduleEvent(EVENT_THUNGERING_SHADOWS, urand(15000, 25000));
                            break;
                        case EVENT_MIND_FLAY:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_MIND_FLAY, true);

                            events.ScheduleEvent(EVENT_MIND_FLAY, urand(5000, 7000));
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_twilights_hammerAI>(creature);
        }
};

class npc_faceless_minions : public CreatureScript
{
    public:
        npc_faceless_minions() : CreatureScript("npc_faceless_minions") { }

        struct npc_faceless_minionsAI : public ScriptedAI
        {
            npc_faceless_minionsAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events, nonCombatEvents;
            uint32 timer;

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                switch (me->GetEntry())
                {
                    case NPC_FLAILING_TENTACLE:
                    case NPC_FACELESS_BORER:
                        SetCombatMovement(false);
                        me->SetInCombatWithZone();
                    default:
                        nonCombatEvents.ScheduleEvent(EVENT_COMBAT, 4000);
                        break;
                }

            }

            void EnterEvadeMode() OVERRIDE
            {
                ScriptedAI::EnterEvadeMode();
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                switch (me->GetEntry())
                {
                    case NPC_FACELESS_BORER:
                        events.ScheduleEvent(EVENT_SHADOW_BORE, urand(1000, 2500));
                        break;
                    case NPC_FACELESS_BRUTE:
                        events.ScheduleEvent(EVENT_TENTACLE_SMASH, 4000);
                        events.ScheduleEvent(EVENT_SQUEEZE_LIFELESS, urand(7000, 10000));
                        break;
                    case NPC_FLAILING_TENTACLE:
                        break;
                    case NPC_FACELESS_SHADOW_WEAVER:
                        events.ScheduleEvent(EVENT_SHADOW_VALLEY, urand(3000, 6000));
                        events.ScheduleEvent(EVENT_SEEKING_SHADOW, urand(9000, 13000));
                        break;
                    default:
                        break;
                }
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_COMBAT)
                    {
                        if (!me->IsInCombat())
                            if (Player* itr = me->FindNearestPlayer(100.0f))
                            {
                                me->SetInCombatWith(itr);
                                me->Attack(itr, true);
                            }
                    }
                    break;
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
                        case EVENT_SQUEEZE_LIFELESS:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_SQUEEZE_LIFELESS, false);
                            events.ScheduleEvent(EVENT_SQUEEZE_LIFELESS, 20000);
                            break;
                        case EVENT_TENTACLE_SMASH:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_TENTACLE_SMASH, false);
                            events.ScheduleEvent(EVENT_TENTACLE_SMASH, urand(8000, 11000));
                            break;
                        case EVENT_SHADOW_BORE:
                            if (Unit* rnd = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(rnd, SPELL_SHADOW_BORE, false);
                            events.ScheduleEvent(EVENT_SHADOW_BORE, urand(4000, 5000));
                            break;
                        case EVENT_SEEKING_SHADOW:
                            if (Unit* itr = SelectTarget(SELECT_TARGET_FARTHEST))
                                me->CastSpell(itr, SPELL_SEEKING_SHADOWS, false);
                            events.ScheduleEvent(EVENT_SEEKING_SHADOW, urand(10000, 15000));
                            break;
                        case EVENT_SHADOW_VALLEY:
                            if (Unit* itr = me->GetVictim())
                                me->CastSpell(itr, SPELL_SHADOW_VOLLEY, false);
                            events.ScheduleEvent(EVENT_SHADOW_VALLEY, urand(6000, 8000));
                            break;
                    }
                }

                if (me->GetEntry() == NPC_FACELESS_BORER || me->GetEntry() == NPC_FLAILING_TENTACLE)
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_faceless_minionsAI>(creature);
        }
};

// Crystalline elemental 55559
class npc_crystalline_elemental : public CreatureScript
{
    public:
        npc_crystalline_elemental() : CreatureScript("npc_crystalline_elemental") { }

        struct npc_crystalline_elementalAI : public ScriptedAI
        {
            npc_crystalline_elementalAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            uint32 timer;

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                timer = 3000;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (timer <= diff)
                {
                    if (Unit* itr = me->GetVictim())
                        me->CastSpell(itr, SPELL_IMPALE, false);

                    timer = urand(5000, 8000);
                }
                else timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_crystalline_elementalAI>(creature);
        }
};

// Ice wall exit stalker 55728
class npc_ice_wall_exit_stalker : public CreatureScript
{
    public:
        npc_ice_wall_exit_stalker() : CreatureScript("npc_ice_wall_exit_stalker") { }

        struct npc_ice_wall_exit_stalkerAI : public ScriptedAI
        {
            npc_ice_wall_exit_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->SetDisplayFromModel(1);
                me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_ice_wall_exit_stalkerAI>(creature);
        }
};
class MoltenFuryPredicate
{
    public:
        MoltenFuryPredicate() { }

        bool operator()(WorldObject* object)
        {
            return object && object->ToCreature() && object->ToCreature()->GetEntry() != NPC_ICE_WALL_EXIT_STALKER;
        }
};

// Molten Fury 103905
class spell_hour_of_twilight_molten_fury : public SpellScript
{
    PrepareSpellScript(spell_hour_of_twilight_molten_fury);

    void HandleHit(SpellEffIndex effIndex)
    {
        if (Creature* target = GetHitCreature())
        {
            if (GameObject* go = target->FindNearestGameObject(GO_ICEWALL_2, 50.0f))
                go->UseDoorOrButton();

            target->DespawnOrUnsummon();
        }
    }

    void CheckTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != NPC_ICE_WALL_EXIT_STALKER; });
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_hour_of_twilight_molten_fury::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hour_of_twilight_molten_fury::CheckTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

/// 210026
class go_hot_time_transit_device : public GameObjectScript
{
public:
    go_hot_time_transit_device() : GameObjectScript("go_hot_time_transit_device") {}

    bool OnReportUse(Player* player, GameObject* go) OVERRIDE
    {
        if (player->IsInCombat())
            return false;

        if (InstanceScript* instance = go->GetInstanceScript())
        {
            if (instance->GetBossState(DATA_BENEDICTUS) == DONE)
            {
                player->CastSpell(player, SPELL_TELEPORT_BENEDICTUS, true);
                return true;
            }
            else if (instance->GetBossState(DATA_ASIRA) == DONE)
            {
                player->CastSpell(player, SPELL_TELEPORT_ASIRA_DEAD, true);
                return true;
            }
            else if (instance->GetBossState(DATA_ARCURION) == DONE)
            {
                player->CastSpell(player, SPELL_TELEPORT_ARCURION_DEAD, true);
                return true;
            }
        }

        return false;
    }
};

void AddSC_hour_of_twilight()
{
    new npc_hour_of_twilight_life_warden();
    new npc_hour_of_twilight_life_warden_thrall();
    new npc_hour_of_twilight_twilight_drake();
    new npc_thrall_intro();
    new npc_thrall_arcurion();
    new npc_thrall_asira();
    new npc_thrall_trash_benedictus();
    new npc_thrall_benedictus();
    new npc_frozen_servitor_stalker();
    new npc_twilights_hammer();
    new npc_faceless_minions();
    new npc_crystalline_elemental();
    new npc_ice_wall_exit_stalker();
    RegisterSpellScript(spell_hour_of_twilight_molten_fury);
    new go_hot_time_transit_device();
}
