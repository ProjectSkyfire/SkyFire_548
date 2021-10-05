////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "heart_of_fear.hpp"
#include "Vehicle.h"

enum eTayakSpells
{
    SPELL_VISINTRO_TAYAK        = 128788,
    SPELL_TEMP_SLASH_AURA       = 122854,   // Visual + Periodic trigger aura for SPELL_TEMP_SLASH_DAMAGE.
    SPELL_TEMP_SLASH_DAMAGE     = 122853,   // Aura Damage + Knock back
    SPELL_WIND_STEP_B_DMG       = 123180,   // Bleed damage for 8y targets.
    SPELL_WIND_STEP_DUMMY       = 123459,   // Aura Dummy.
    SPELL_WIND_STEP_TP_BACK     = 123460,   // Teleport back to the main target.
    SPELL_INTENSIFY_NORMAL      = 123470,   // Add spell 123471 every 1 minute, phase 1
    SPELL_INTENSIFY_TORNADO     = 132254,   // Add spell 123471 every 10 secs, phase 2
    SPELL_BLADE_TEMPEST_AUR     = 125310,   // Triggers SPELL_BLADE_TEMPEST_DMG each 0.5s, SPELL_BLADE_TEMPEST_AT.
    SPELL_BLADE_TEMPES_J_FC     = 125325,   // Force Cast SPELL_BLADE_TEMPES_JUMP in 200 yards.
    SPELL_STORM_UNLEASHED_D     = 123814,   // Boss Dummy Visual.
    SPELL_SU_AURA               = 123598,   // Aura for the tornadoes, triggers SPELL_SU_RV_SE each 0.1 secs.
    SPELL_SU_RV                 = 123599,   // Control Vehicle aura.
    SPELL_SU_DUMMY_VIS          = 124024,   // Some dummy visual (for tornadoes?).
    SPELL_SU_DMG_AURA           = 124785,   // Triggers SPELL_SU_DMG every 1 sec.
    SPELL_SU_WIND_GALE          = 123633,
    SPELL_SU_DUMMY_CRAP         = 123616,   // Applies a dummy aura on a target.
    SPELL_TAYAK_BERSERK         = 26662,    // Enrage, 490 seconds, or 8:10 minutes.
};

enum Yells
{
    // Blade Lord Ta'yak
    SAY_AGGRO               = 0,    // On your guard, invaders. I, Ta'yak, Lord of Blades, will be your opponent.
    SAY_DEATH               = 1,    // I'd always hoped to cut someone like that someday, to hear that sound. But to have it happen to my own prothorax is ridiculous.
    SAY_KILL_TRASH          = 2,    // 0 - Mere initiates. ; 1 - They should have studied harder. ; 2 - One technique alone is not enough. ; 3 - They failed their test. Will you?
    SAY_INTRO               = 3,    // They approach? Good. Now, if only my students were here to see and learn from the coming display of martial prowess...
    SAY_SLAY                = 4,    // 0 - A perfect cut. ; 1 - This is the technique of a Blade Lord.
    SAY_STORM_UNLEASHED     = 5,    // Can you follow my blade?
    ANN_UNSEEN              = 6,    // Blade Lord Ta'yak marks $N for [Unseen Strike]!
    SAY_ENTER_ROOM          = 7,    // Now go, impart my techniques to the initiates.
};

enum Events
{
    // Blade Lord Ta'yak
    EVENT_TEMPEST_SLASH     = 1,    // 10 seconds from pull. Every 15.5 seconds.
    EVENT_UNSEEN_STRIKE,            // 30.5 seconds from pull. Every 53 - 61 seconds.
    EVENT_UNSEEN_STRIKE_RETURN,
    EVENT_TAYAK_WIND_STEP,          // 20.5 seconds from pull. Every 25 seconds.
    EVENT_WIND_STEP_RETURN,
    EVENT_OVERWHELMING_ASS,         // 15.5 seconds from pull. Every 20.5 seconds, delayable by up to 15 seconds.

    EVENT_BLADE_TEMPEST,            // Every 60 seconds. Heroic only.
    EVENT_TAYAK_BT_END,

    EVENT_STORM_UNLEASHED,          // 20%
    EVENT_SUMMON_TORNADOES,

    EVENT_TAYAK_BERSERK,            // Enrage at 8 minutes, or, more precisely, 490 seconds.
};

enum TayakPhases
{
    PHASE_NORMAL            = 1,
    PHASE_STORM_UNLEASHED   = 2
};

enum eTayakAdds
{
    NPC_US_TORNADO          = 63278,
    NPC_GALE_WINDS_STALKER  = 63292,
    NPC_SETTHIK_TEMPEST     = 64358,
};

enum eTayakActions
{
    ACTION_WIND             = 1,
    ACTION_STOP_WIND        = 2,
    ACTION_TAYAK_BT_PULL    = 7,
};

enum eTayakPhases
{
    PHASE_TAYAK1                    = 2,
    PHASE_TAYAK2                    = 3,
};

enum eTayakTypes
{
    TYPE_STORM_POINT    = 3,
    TYPE_PHASE_TAYAK    = 4,
    TYPE_STORM_PHASE    = 5,
};

Position TayakStormPoints[2] =
{
    { -2119.072f, 184.985f, 422.162f, 1.585f }, // 20 - 10% TP point
    { -2119.072f, 379.372f, 422.162f, 4.703f }  // 10 -  0% TP point
};

Position PlayerTelePos = { -2124.063f, 281.056f, 420.901f, 0.053f }; // Tele point for players on P2 start.

// StormPoint1 Tornado summon points.
Position const Tornado1[3] =
{
    { -2123.702f, 198.023f, 420.910f, 1.561f }, // Left
    { -2119.503f, 198.023f, 420.910f, 1.561f }, // Center
    { -2114.113f, 198.023f, 420.910f, 1.561f }, // Right
};

// StormPoint2 Tornado summon points.
Position const Tornado2[3] =
{
    { -2114.113f, 363.058f, 420.910f, 4.710f }, // Left
    { -2119.503f, 363.058f, 420.910f, 4.710f }, // Center
    { -2123.702f, 363.058f, 420.910f, 4.710f }, // Right
};

// Zorlok - 62980
class boss_tayak : public CreatureScript
{
    public:
        boss_tayak() : CreatureScript("boss_tayak") { }

        struct boss_tayakAI : public BossAI
        {
            boss_tayakAI(Creature* creature) : BossAI(creature, DATA_TAYAK), summons(me)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            SummonList summons;
            uint64 unseenTank;
            uint64 currentTank;
            bool entranceDone;
            bool introDone;
            bool storm1Done;
            bool unseenReturn;
            uint8 tpPlayers;    // 0 - Phase 1 | 1 - TP Players | 2 - Players have been TP
            uint8 Phase;
            std::list<uint64> playerGuids;

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();

                unseenTank  = 0;
                currentTank = 0;
                tpPlayers   = 0;
                storm1Done   = false;
                entranceDone = false;
                introDone    = false;
                unseenReturn = false;
                Phase = PHASE_NORMAL;
                playerGuids.clear();
                me->CastSpell(me, SPELL_VISINTRO_TAYAK, true);

                std::list<Creature*> stalkerList;
                GetCreatureListWithEntryInGrid(stalkerList, me, NPC_GALE_WINDS_STALKER, 300.0f); // TODO: adjuste searche range
                for (auto stalker : stalkerList)
                    stalker->AI()->DoAction(ACTION_STOP_WIND);

                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TAYAK_MELJARAK);

                _Reset();
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_TAYAK_TALK_TRASH:
                    {
                        Talk(SAY_KILL_TRASH);
                        break;
                    }
                    case ACTION_TAYAK_BT_PULL:
                    {
                        // Slowly pulling players back to Ta'yak
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 200.0f); // TODO: adjuste searche range

                        Position pos = {me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f};

                        for (Player* player : playerList)
                            if (player->isAlive())
                                player->GetForcedMovement().StartPullingTo(pos, 3.0f);

                        // Won't reach the event until 6-7 secs as Ta'yak has UNIT_STATE_CASTING
                        events.ScheduleEvent(EVENT_TAYAK_BT_END, 100);
                        break;
                    }
                    default:
                        break;
                }
            }

            bool CheckTrash()
            {
                // -- Trashs --
                Creature* GaleSlicer = GetClosestCreatureWithEntry(me, NPC_SETTHIK_GALESLICER, 100.0f, true);
                Creature* Silentwing = GetClosestCreatureWithEntry(me, NPC_KORTHIK_SILENTWING, 100.0f, true);
                Creature* Swarmer    = GetClosestCreatureWithEntry(me, NPC_KORTHIK_SWARMER,    100.0f, true);
                Creature* Tempest    = GetClosestCreatureWithEntry(me, NPC_SETTHIK_TEMPEST,    100.0f, true);

                // -- Instructors --
                Creature* Klithak    = GetClosestCreatureWithEntry(me, NPC_INSTRUCTOR_KLITHAK, 100.0f, true);
                Creature* Takthok    = GetClosestCreatureWithEntry(me, NPC_INSTRUCTOR_TAKTHOK, 100.0f, true);
                Creature* Maltik     = GetClosestCreatureWithEntry(me, NPC_INSTRUCTOR_MALTIK,  100.0f, true);
                Creature* Zarik      = GetClosestCreatureWithEntry(me, NPC_INSTRUCTOR_ZARIK,   100.0f, true);

                if (GaleSlicer || Silentwing || Swarmer || Tempest || Klithak || Takthok || Maltik || Zarik)
                    return false;

                return true;
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!entranceDone && me->IsWithinDistInMap(who, 50) && who->GetTypeId() == TYPEID_PLAYER)
                {
                    Talk(SAY_ENTER_ROOM);
                    entranceDone = true;
                }

                if (entranceDone && !introDone && me->IsWithinDistInMap(who, 30) && who->GetTypeId() == TYPEID_PLAYER && CheckTrash())
                {
                    Talk(SAY_INTRO);
                    introDone = true;
                    me->RemoveAura(SPELL_VISINTRO_TAYAK);
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                if (pInstance)
                {
                    if (pInstance->GetBossState(DATA_TAYAK) == IN_PROGRESS)
                        return;

                    if (!pInstance->CheckRequiredBosses(DATA_TAYAK) || !CheckTrash())
                    {
                        EnterEvadeMode();
                        return;
                    }

                    if (GameObject* entranceDoor = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_QUARTERS_DOOR_ENTRANCE)))
                        entranceDoor->SetGoState(GO_STATE_READY);
                }

                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_TEMPEST_SLASH, urand(9500, 11000));
                events.ScheduleEvent(EVENT_UNSEEN_STRIKE, urand(29500, 31500));
                events.ScheduleEvent(EVENT_TAYAK_WIND_STEP, urand(19500, 21500));
                events.ScheduleEvent(EVENT_OVERWHELMING_ASS, urand(14500, 16500));
                //events.ScheduleEvent(EVENT_INTENSIFY, 60000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_BLADE_TEMPEST, 60000);

                me->AddAura(SPELL_INTENSIFY_NORMAL, me);

                // 8:15 minutes Enrage timer
                events.ScheduleEvent(EVENT_TAYAK_BERSERK, 495000);

                if (pInstance)
                {
                    pInstance->SetData(DATA_TAYAK, IN_PROGRESS);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me); // Add
                }

                if (me->HasAura(SPELL_VISINTRO_TAYAK))
                    me->RemoveAura(SPELL_VISINTRO_TAYAK);

                me->DisableEvadeMode();
                me->DisableHealthRegen();
                _EnterCombat();
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            uint32 GetData(uint32 type)
            {
                if (type == TYPE_PHASE_TAYAK)
                    return Phase;
                else if (type == TYPE_STORM_PHASE)
                    return storm1Done ? 1 : 0;
                return 0;
            }

            void EnterEvadeMode()
            {
                me->RemoveAllAuras();
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();

                if (pInstance)
                {
                    pInstance->SetData(DATA_TAYAK, FAIL);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me); // Remove

                    if (GameObject* entranceDoor = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_QUARTERS_DOOR_ENTRANCE)))
                        entranceDoor->SetGoState(GO_STATE_ACTIVE);
                }

                _EnterEvadeMode();
            }

            void JustDied(Unit* /*killer*/)
            {
                Talk(SAY_DEATH);
                events.Reset();
                me->RemoveAllAuras();

                if (pInstance)
                {
                    pInstance->SetData(DATA_TAYAK, DONE);

                    if (IsHeroic())
                        pInstance->DoCompleteAchievement(ACHIEVEMENT_HEROIC_TAYAK);

                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me); // Remove

                    if (GameObject* entranceDoor = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_QUARTERS_DOOR_ENTRANCE)))
                        entranceDoor->SetGoState(GO_STATE_ACTIVE);
                }

                // Stop Wind Gales
                std::list<Creature*> windList;
                GetCreatureListWithEntryInGrid(windList, me, NPC_GALE_WINDS_STALKER, 300.0f); // TODO: adjuste searche range
                for (auto stalker : windList)
                    stalker->AI()->DoAction(ACTION_STOP_WIND);

                summons.DespawnAll();

                _JustDied();

                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                }
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                summon->setActive(true);

                if (me->isInCombat())
                    summon->SetInCombatWithZone();
            }

            // Phase 2 player teleportation.
            void TeleportPlayers()
            {
                Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    if (Player* playr = i->getSource())
                        if (playr->isAlive())
                            playr->TeleportTo(me->GetMapId(), PlayerTelePos.GetPositionX(), PlayerTelePos.GetPositionY(), PlayerTelePos.GetPositionZ(), PlayerTelePos.GetOrientation(), TELE_TO_NOT_LEAVE_COMBAT | TELE_TO_NOT_UNSUMMON_PET);
            }

            void ActivateGaleWinds()
            {
                std::list<Creature*> GWStalkersList;
                GetCreatureListWithEntryInGrid(GWStalkersList, me, NPC_GALE_WINDS_STALKER, 300.0f); // TODO: adjuste searche range
                for (auto stalker : GWStalkersList)
                    stalker->AI()->DoAction(ACTION_WIND);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (id)
                {
                    // Coming to center of the room
                    case 2:
                    {
                        // Beginning of phase 2, Ta'yak at room's center
                        TeleportPlayers();
                        events.ScheduleEvent(EVENT_STORM_UNLEASHED, 100);

                        // Moving to the other side
                        me->SetSpeed(MOVE_RUN, 20.0f, true);
                        me->SetSpeed(MOVE_WALK, 20.0f, true);
                        me->SetSpeed(MOVE_FLIGHT, 20.0f, true);
                        me->GetMotionMaster()->MovementExpired();
                        me->GetMotionMaster()->Clear();
                        tpPlayers = 1;
                        break;
                    }
                    // Coming to an edge
                    case 3:
                    {
                        me->GetMotionMaster()->MovementExpired();
                        me->GetMotionMaster()->Clear();
                        events.ScheduleEvent(EVENT_SUMMON_TORNADOES, 1000);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->EvadeModeIsDisable() && pInstance && pInstance->IsWipe())
                {
                    me->ReenableEvadeMode();
                    me->ReenableHealthRegen();
                    EnterEvadeMode();
                    return;
                }

                if ((!UpdateVictim() && !unseenReturn) || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                // Storm Unleashed 20 - 10%.
                if (me->HealthBelowPct(20) && Phase == PHASE_NORMAL)
                {
                    // Set phase
                    Phase = PHASE_STORM_UNLEASHED;

                    // Intensify
                    me->RemoveAura(SPELL_INTENSIFY_NORMAL);
                    me->AddAura(SPELL_INTENSIFY_TORNADO, me);

                    Talk(SAY_STORM_UNLEASHED);

                    // Cancel the P1 events except intensify
                    events.CancelEvent(EVENT_TEMPEST_SLASH);
                    events.CancelEvent(EVENT_UNSEEN_STRIKE);
                    events.CancelEvent(EVENT_TAYAK_WIND_STEP);
                    events.CancelEvent(EVENT_OVERWHELMING_ASS);
                    if (IsHeroic())
                        events.CancelEvent(EVENT_BLADE_TEMPEST);

                    me->SetReactState(REACT_PASSIVE);

                    // Teleport players and move to P2 first point.
                    me->SetSpeed(MOVE_RUN, 20.0f, true);
                    me->SetSpeed(MOVE_WALK, 20.0f, true);
                    me->SetSpeed(MOVE_FLIGHT, 20.0f, true);
                    me->GetMotionMaster()->MovePoint(2, PlayerTelePos);

                    DoCast(me, SPELL_STORM_UNLEASHED_D);
                    me->AddAura(SPELL_SU_DMG_AURA, me);
                }

                // Storm Unleashed 10 - 0%.
                if (me->HealthBelowPct(10) && Phase == PHASE_STORM_UNLEASHED && !storm1Done)
                {
                    storm1Done = true;
                    ActivateGaleWinds();

                    Talk(SAY_STORM_UNLEASHED);

                    events.CancelEvent(EVENT_SUMMON_TORNADOES);

                    // Move to P2 second point.
                    me->SetSpeed(MOVE_RUN, 20.0f, true);
                    me->SetSpeed(MOVE_WALK, 20.0f, true);
                    me->SetSpeed(MOVE_FLIGHT, 20.0f, true);
                    me->GetMotionMaster()->MovePoint(3, TayakStormPoints[1]);
                }

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TEMPEST_SLASH:
                        {
                            DoCast(me, SPELL_TEMPEST_SLASH);
                            events.ScheduleEvent(EVENT_TEMPEST_SLASH, urand(14500, 16500));
                            break;
                        }
                        case EVENT_UNSEEN_STRIKE:
                        {
                            unseenTank = me->getVictim() ? me->getVictim()->GetGUID() : 0;
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            {
                                Talk(ANN_UNSEEN, target->GetGUID());
                                me->CastSpell(target, SPELL_UNSEEN_STRIKE_TR, true);
                                me->AddAura(SPELL_UNSEEN_STRIKE_MKR, target);
                                me->GetMotionMaster()->MoveChase(target);
                                me->SetReactState(REACT_PASSIVE);
                                unseenReturn = true;
                            }
                            events.ScheduleEvent(EVENT_UNSEEN_STRIKE_RETURN, 5500);
                            events.ScheduleEvent(EVENT_UNSEEN_STRIKE, urand(53000, 61000));
                            break;
                        }
                        case EVENT_UNSEEN_STRIKE_RETURN:
                        {
                            if (unseenTank)
                            {
                                me->SetReactState(REACT_AGGRESSIVE);
                                if (Player* unseenTarget = ObjectAccessor::FindPlayer(unseenTank))
                                {
                                    if (unseenTarget->isAlive())
                                        AttackStart(unseenTarget);
                                    else
                                        SetAggro();
                                }
                                else
                                    SetAggro();
                                unseenTank = 0;
                            }
                            else
                                SetAggro();
                            break;
                        }
                        case EVENT_TAYAK_WIND_STEP:
                        {
                            // Store current victim to return to it afterwards
                            currentTank = me->getVictim() ? me->getVictim()->GetGUID() : 0;
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 50.0f, true))
                                DoCast(target, SPELL_WIND_STEP_TP);
                            events.ScheduleEvent(EVENT_WIND_STEP_RETURN, 1000);
                            events.ScheduleEvent(EVENT_TAYAK_WIND_STEP, urand(24000, 26000));
                            break;
                        }
                        case EVENT_WIND_STEP_RETURN:
                        {
                            // Return to old target
                            if (Player* currentVictim = ObjectAccessor::FindPlayer(currentTank))
                            {
                                if (currentVictim->isAlive())
                                    DoCast(currentVictim, SPELL_WIND_STEP_TP_BACK);
                                else
                                    SetAggro();
                                currentTank = 0;
                            }
                            else
                                SetAggro();
                            break;
                        }
                        case EVENT_OVERWHELMING_ASS:
                        {
                            // On tank
                            DoCastVictim(SPELL_OVERWHELMING_ASS);
                            events.ScheduleEvent(EVENT_OVERWHELMING_ASS, urand(20500, 35500));
                            break;
                        }
                        // Heroic
                        case EVENT_BLADE_TEMPEST:
                        {
                            DoCast(me, SPELL_BLADE_TEMPEST_AUR);
                            DoCast(SPELL_BLADE_TEMPES_J_FC);
                            events.ScheduleEvent(EVENT_BLADE_TEMPEST, 60000);
                            break;
                        }
                        case EVENT_TAYAK_BT_END:
                        {
                            // Remove ForceMovement on players
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 200.0f); // TODO: adjuste searche range

                            Position pos = {me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f};

                            for (Player* player : playerList)
                                player->GetForcedMovement().Stop();

                            break;
                        }
                        // Phase 2
                        case EVENT_STORM_UNLEASHED:
                        {
                            // Players have just been TP : now creating list of the players.
                            if (tpPlayers == 1)
                            {
                                std::list<Player*> playerList;
                                GetPlayerListInGrid(playerList, me, 10.0f);

                                // Working with GUID, not players directly
                                for (auto player : playerList)
                                    playerGuids.push_back(player->GetGUID());

                                // Scheduling creation of storms to bring players on the side
                                if (!playerGuids.empty())
                                {
                                    events.ScheduleEvent(EVENT_STORM_UNLEASHED, 100);
                                    me->GetMotionMaster()->MovePoint(3, TayakStormPoints[0]);
                                    tpPlayers = 2;
                                }
                                // No player : maybe they're not arrived from TP : we'll try to redo this part later
                                else
                                    events.ScheduleEvent(EVENT_STORM_UNLEASHED, 100);
                                // Nothing more to do for now
                                return;
                            }

                            if (playerGuids.empty())
                                break;

                            // Creating storm to bring each player to a side of the room
                            uint64 plGuid = *playerGuids.begin();
                            if (Player* player = ObjectAccessor::GetPlayer(*me, plGuid))
                                if (Creature* storm = me->SummonCreature(NPC_US_TORNADO, PlayerTelePos))
                                    storm->AI()->SetData(TYPE_STORM_POINT, 1);

                            playerGuids.remove(plGuid);

                            if (!playerGuids.empty())
                                events.ScheduleEvent(EVENT_STORM_UNLEASHED, 100);
                            else
                                ActivateGaleWinds();
                            break;
                        }
                        case EVENT_SUMMON_TORNADOES:
                        {
                            // Summon the tornado and increase the counter
                            uint32 choice = urand(0, 2);
                            if (Creature* storm = me->SummonCreature(NPC_US_TORNADO, !storm1Done ? Tornado1[choice] : Tornado2[choice]))
                                storm->AI()->SetData(TYPE_STORM_POINT, 3 * storm1Done + choice);

                            events.ScheduleEvent(EVENT_SUMMON_TORNADOES, 3000);
                            break;
                        }
                        case EVENT_TAYAK_BERSERK: // Nasty if you're here!
                        {
                            me->AddAura(SPELL_TAYAK_BERSERK, me);
                            break;
                        }
                        default:
                            break;
                    }
                }
                // No melee in P2
                if (Phase == PHASE_NORMAL)
                    DoMeleeAttackIfReady();
            }

            void SetAggro()
            {
                if (Unit* victim = SelectTarget(SELECT_TARGET_TOPAGGRO))
                {
                    AttackStart(victim);
                    me->SetInCombatWith(victim);
                }
            }
        };

        BossAI* GetAI(Creature* creature) const
        {
            return new boss_tayakAI(creature);
        }
};

// Heart of Fear - Armsmaster Ta'yak Tempest Stalker (LTD): 62908.
class npc_tempest_slash_tornado : public CreatureScript
{
    public:
        npc_tempest_slash_tornado() : CreatureScript("npc_tempest_slash_tornado") { }

        struct npc_tempest_slash_tornadoAI : public ScriptedAI
        {
            npc_tempest_slash_tornadoAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void IsSummonedBy(Unit* summoner)
            {
                events.Reset();
                me->SetReactState(REACT_PASSIVE);

                if (summoner)
                {
                    // Replacing at 5.0f on the right of Ta'yak (summoner)
                    float leftOri = (me->GetOrientation() + M_PI * 0.5f > M_PI * 2.0f) ? me->GetOrientation() - 1.5f * M_PI : me->GetOrientation() + M_PI * 0.5f;

                    // Random orientation in front of Ta'yak
                    float ori = summoner->GetOrientation() + ((urand(0, 1) ? 1 : -1) * frand(M_PI/3, M_PI/2));
                    me->SetOrientation(ori);
                    me->SetFacingTo(ori);

                    // Applying auras and moving
                    me->SetInCombatWithZone();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);

                    me->AddAura(SPELL_TEMP_SLASH_AURA, me);     // Visual aura

                    float x, y, z;
                    me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, frand(5.0f, 30.0f));
                    me->GetMotionMaster()->MovePoint(8, x, y, z);
                }

            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (!id || type != POINT_MOTION_TYPE)
                    return;

                if (id == 8)
                {
                    Movement::MoveSplineInit init(*me);
                    // Selecting the center 2.0f yds forward as center of the circle path
                    Position pos = GetTargetPoint(me, 2.0f);
                    // Creating the circle path from the center
                    FillCirclePath(pos, 2.0f, pos.GetPositionZ(), init.Path(), true);
                    init.SetWalk(true);
                    init.SetCyclic();
                    init.Launch();
                }
            }

            Position GetTargetPoint(Creature* mob, float dist)
            {
                /*
                 * The main idea is : a circle has 4 quarters; the principle is to define a point at the specified distance forward according the orientation,
                 * and use this point as a destination for MovePoint(). To calculate this point, we use the orientation to get a ratio between X and Y in the
                 * main "quarter" (both positive x and y): the point is that "ratio x" is a proportion of orientation/(pi/2), that is: if orientation = 0,
                 * orientation/(pi/2) = 0 and so ratio-x = 0%. If orientation = pi/2, then orientation/(pi/2) = (pi/2)/(pi/2) = 1, ratio-x = 100%. And
                 * ratio-y = 100% - ratio-x. So if ratio-x = 60%, ratio-y = 40%. That means that when you walk 1 yd in the orientation direction, you'll
                 * made 0.6yd forward and 0.4yd sidewards (more or less). To be precise, we should use pythagorean theorem, but the difference would be
                 * small.
                 *
                 * The range orientation from 0 to pi/2 represents a quarter circle where x and y will be both positives, and we use this quarter circle
                 * to determine general ratio between x and y. Then, we just have to "rotate" to apply this to the right orientation. According to this
                 * initial orientation, we may need to switch x and y ratio (when turned on left or right, moving forward is moving on y-axis, and not
                 * on x-axis, for instance), and/or to apply negatives values (if orientation is pi, we're moving backwards, so the x-value decreases,
                 * while if orientation is 0.0, we're moving forwards, and so, the x-value increases, but we're still on the same axis).
                 */

                float posX = mob->GetPositionX();
                float posY = mob->GetPositionY();
                float posZ = mob->GetPositionZ();
                float orientation = mob->GetOrientation();

                // Retrieving absolute orientation
                float absOri = orientation;
                uint8 turn = 0;
                while (absOri > (M_PI / 2))
                {
                    absOri -= (M_PI / 2);
                    turn = ++turn % 4;
                }

                // Looking for ratio between X and Y
                float percentX = ((M_PI / 2) - absOri) / (M_PI / 2);
                float percentY = 1.0f - percentX;

                // Applying negatives directions according to orientation
                if (orientation > (M_PI / 2))
                {
                    if (orientation > M_PI)
                        percentY = -percentY;

                    if (orientation > (M_PI / 2) && orientation < (1.5f * M_PI))
                        percentX = -percentX;
                }

                // if turned, we need to switch X & Y
                switch (turn)
                {
                    // -x / +y / switch
                    case 1:
                    {
                        float tmpVal = percentX;
                        percentX = -percentY;
                        percentY = tmpVal;
                        break;
                    }
                    // -x / -y / no switch
                    case 2:
                    {
                        percentX = -percentX;
                        percentY = -percentY;
                        break;
                    }
                    // +x / -y / switch
                    case 3:
                    {
                        float tmpVal = percentX;
                        percentX = percentY;
                        percentY = -tmpVal;
                        break;
                    }
                    // +x / +y / no switch : no change
                    default:
                        break;
                }

                // Calculating reaching point
                float pointX = posX;
                float pointY = posY;

                Position reachPoint = {pointX, pointY, posZ, orientation};

                do
                {
                    pointX += percentX;
                    pointY += percentY;
                    reachPoint.Relocate(pointX, pointY);
                } while (mob->GetDistance(reachPoint) < dist);

                return reachPoint;
            }

            void FillCirclePath(Position const& centerPos, float radius, float z, Movement::PointsArray& path, bool clockwise)
            {
                float step = clockwise ? -M_PI / 8.0f : M_PI / 8.0f;
                float angle = centerPos.GetAngle(me->GetPositionX(), me->GetPositionY());

                for (uint8 i = 0; i < 16; angle += step, ++i)
                {
                    G3D::Vector3 point;
                    point.x = centerPos.GetPositionX() + radius * cosf(angle);
                    point.y = centerPos.GetPositionY() + radius * sinf(angle);
                    point.z = me->GetMap()->GetHeight(me->GetPhaseMask(), point.x, point.y, z + 5.0f);
                    path.push_back(point);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                // Despawn on Ta'yak's phase 2
                if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                    if (tayak->AI()->GetData(TYPE_PHASE_TAYAK) == PHASE_STORM_UNLEASHED)
                        me->DespawnOrUnsummon();

                events.Update(diff);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tempest_slash_tornadoAI(creature);
        }
};

// Heart of Fear - Armsmaster Ta'yak - Storm Unleashed West 1 Tornado (LTD): 63278.
class npc_storm_unleashed_tornado : public CreatureScript
{
    public:
        npc_storm_unleashed_tornado() : CreatureScript("npc_storm_unleashed_tornado") { }

        struct npc_storm_unleashed_tornadoAI : public ScriptedAI
        {
            npc_storm_unleashed_tornadoAI(Creature* creature) : ScriptedAI(creature), vehicle(creature->GetVehicleKit())
            {
                ASSERT(vehicle);
                pInstance = creature->GetInstanceScript();
            }

            Vehicle* vehicle;
            InstanceScript* pInstance;
            bool storm1;

            void Reset()
            {
                storm1 = true;
                me->AddAura(SPELL_SU_AURA, me);         // Control vehicle aura.
                me->AddAura(SPELL_SU_DUMMY_VIS, me);    // Visual aura.
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);

                me->SetSpeed(MOVE_WALK, 1.1f, true);
                me->SetSpeed(MOVE_RUN, 1.1f, true);
                Movement::MoveSplineInit init(*me);
                init.SetOrientationFixed(true);
                init.Launch();
            }

            void Eject()
            {
                if (vehicle)
                {
                    int8 seat = 0;
                    do
                    {
                        if (Unit* passenger = vehicle->GetPassenger(seat))
                            passenger->ExitVehicle();

                        ++seat;

                    } while (seat < 8);
                }

                me->RemoveAura(SPELL_SU_AURA);
                me->RemoveAura(SPELL_SU_DUMMY_VIS);
                me->DespawnOrUnsummon();
            }

            void SetData(uint32 type, uint32 value)
            {
                if (type == TYPE_STORM_POINT)
                {
                    storm1 = value < 3 ? true : false;
                    Position reachPoint = (value < 3 ? Tornado2[2 - value] : Tornado1[5 - value]);
                    me->GetMotionMaster()->MovePoint(2, reachPoint);
                }
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (!id || type != POINT_MOTION_TYPE)
                    return;

                Eject();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!pInstance)
                {
                    Eject();
                    return;
                }

                // Removing storm from the 1st storm phase (20-10%) when switching on 2nd storm phase (10-0%)
                if (storm1)
                    if (pInstance)
                        if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                            if (tayak->AI()->GetData(TYPE_STORM_PHASE))
                                Eject();

            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_storm_unleashed_tornadoAI(creature);
        }
};

// Gale Winds Stalker - 63292
class mob_gale_winds_stalker : public CreatureScript
{
    public :
        mob_gale_winds_stalker() : CreatureScript("mob_gale_winds_stalker") { }

        struct mob_gale_winds_stalkerAI : public ScriptedAI
        {
            mob_gale_winds_stalkerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            bool firstWind;
            bool isActive;

            void Reset()
            {
                isActive  = false;
                firstWind = false;
                if (me->GetOrientation() < 3)
                    firstWind = true;
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_WIND:
                    {
                        if (pInstance)
                        {
                            if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                            {
                                bool firstPos = tayak->AI()->GetData(TYPE_STORM_PHASE);
                                if ((firstPos && firstWind) || (!firstPos && !firstWind))
                                {
                                    DoCast(SPELL_SU_WIND_GALE);
                                    tayak->CastSpell(me, SPELL_SU_DUMMY_CRAP, false);
                                }
                                else
                                    if (me->HasAura(SPELL_SU_WIND_GALE))
                                        me->RemoveAura(SPELL_SU_WIND_GALE);

                                if (!isActive)
                                    isActive = true;
                            }
                        }
                        break;
                    }
                    case ACTION_STOP_WIND:
                    {
                        if (me->HasAura(SPELL_SU_WIND_GALE))
                            me->RemoveAura(SPELL_SU_WIND_GALE);

                        if (pInstance)
                        {
                            if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                            {
                                Position pos = {tayak->GetPositionX(), tayak->GetPositionY(), tayak->GetPositionZ(), 0.0f};

                                std::list<Player*> playerList;
                                GetPlayerListInGrid(playerList, me, 500.0f); // TODO: adjuste searche range

                                for (auto player : playerList)
                                    player->GetForcedMovement().Stop();
                            }

                            isActive = false;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                // Check force
                if (!pInstance || !isActive)
                    return;

                if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                {
                    Position pos = {tayak->GetPositionX(), tayak->GetPositionY(), tayak->GetPositionZ(), 0.0f};

                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 20.0f);

                    for (auto player : playerList)
                    {
                        // if player is in wind gale
                        if (player->GetPositionX() > -2109.51f || player->GetPositionX() < -2129.05f)
                        {
                            // Player doesn't have forcedMovement
                            if (player->isAlive() && !player->GetForcedMovement().IsActive())
                                player->GetForcedMovement().StartPushingFrom(pos, 7.0f);
                            // Dead player has forcedMovement
                            else if (!player->isAlive() && player->GetForcedMovement().IsActive())
                                player->GetForcedMovement().Stop();
                        }
                        // player not in wind gale
                        else if (player->GetForcedMovement().IsActive())
                            player->GetForcedMovement().Stop();
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_gale_winds_stalkerAI(creature);
        }
};

// 123175 - Wind Step
class spell_wind_step : public SpellScriptLoader
{
    public:
        spell_wind_step() : SpellScriptLoader("spell_wind_step") { }

        class spell_wind_stepSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_wind_stepSpellScript);

            void Apply()
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, caster, 10.0f);

                    for (auto player : playerList)
                        caster->AddAura(SPELL_WIND_STEP_B_DMG, player);

                    caster->CastSpell(caster, SPELL_WIND_STEP_DUMMY, false);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_wind_stepSpellScript::Apply);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_wind_stepSpellScript();
        }
};

// Wind Step: 123459
class spell_tayak_wind_step: public SpellScriptLoader
{
    public:
        spell_tayak_wind_step() : SpellScriptLoader("spell_tayak_wind_step") { }

        class spell_tayak_wind_stepAuraScript: public AuraScript
        {
            PrepareAuraScript(spell_tayak_wind_stepAuraScript);

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_WIND_STEP_DUMMY, caster);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_tayak_wind_stepAuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_tayak_wind_stepAuraScript();
        }
};

// 122982 - Unseen Strike
class spell_unseen_strike_aura : public SpellScriptLoader
{
    public:
        spell_unseen_strike_aura() : SpellScriptLoader("spell_unseen_strike") { }

        class spell_unseen_strike_auraAuraScript : public AuraScript
        {
            PrepareAuraScript(spell_unseen_strike_auraAuraScript);

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetTarget())
                        caster->AddAura(SPELL_UNSEEN_STRIKE_TR, target);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_unseen_strike_auraAuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_unseen_strike_auraAuraScript();
        }
};

// 122994 - Unseen Strike
class spell_unseen_strike_dmg : public SpellScriptLoader
{
    public :
        spell_unseen_strike_dmg() : SpellScriptLoader("spell_unseen_strike_dmg") { }

        class spell_unseen_strike_dmgSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_unseen_strike_dmgSpellScript);

            int32 numTargets;

            void CountTargets(std::list<WorldObject*>& targets)
            {
                numTargets = targets.size();
            }

            void DealDamages(SpellEffIndex effIndex)
            {
                if (numTargets)
                    SetHitDamage(GetSpellInfo()->Effects[effIndex].BasePoints / numTargets);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_unseen_strike_dmgSpellScript::CountTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
                OnEffectHitTarget += SpellEffectFn(spell_unseen_strike_dmgSpellScript::DealDamages, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_unseen_strike_dmgSpellScript();
        }
};

// Storm Unleashed Ride Me: 124258.
class spell_tayak_storms_vehicle: public SpellScriptLoader
{
    public:
        spell_tayak_storms_vehicle() : SpellScriptLoader("spell_tayak_storms_vehicle") { }

        class spell_tayak_storms_vehicleSpellScript: public SpellScript
        {
            PrepareSpellScript(spell_tayak_storms_vehicleSpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.clear();

                // Find the nearest player in 100 yards, and that will be the target.
                WorldObject* target = GetCaster()->ToCreature()->SelectNearestPlayer(100.0f);

                if (!target->ToPlayer()->GetVehicle())
                    targets.push_back(target);
            }

            void EffectScriptEffect(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->CastSpell(GetCaster(), SPELL_SU_RV, true); // Enter the vehicle.
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_tayak_storms_vehicleSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_tayak_storms_vehicleSpellScript::EffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_tayak_storms_vehicleSpellScript();
        }
};

// Storm Unleashed: 124783.
class spell_tayak_storm_unleashed_dmg: public SpellScriptLoader
{
    public:
        spell_tayak_storm_unleashed_dmg() : SpellScriptLoader("spell_tayak_storm_unleashed_dmg") { }

        class spell_tayak_storm_unleashed_dmgSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_tayak_storm_unleashed_dmgSpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();

                if (Unit* caster = GetCaster())
                {
                    std::list<Player*> targetList;
                    GetPlayerListInGrid(targetList, caster, 300.0f); // TODO: adjuste searche range

                    for (auto player : targetList)
                        if (player->IsOnVehicle())
                            targets.push_back(player);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_tayak_storm_unleashed_dmgSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_tayak_storm_unleashed_dmgSpellScript();
        }
};

// 122853 - Tempest Slash (damage aura)
class spell_tempest_slash : public SpellScriptLoader
{
    public :
        spell_tempest_slash() : SpellScriptLoader("spell_tempest_slash") { }

        class spell_tempest_slash_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_tempest_slash_AuraScript);

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_TEMP_SLASH_DAMAGE, caster);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_tempest_slash_AuraScript::Apply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_tempest_slash_AuraScript();
        }
};

// 123814 - Storm unleashed - Boss visual effect
class spell_tayak_su_visual : public SpellScriptLoader
{
    public:
        spell_tayak_su_visual() : SpellScriptLoader("spell_tayak_su_visual") { }

        class spell_tayak_su_visualAuraScript : public AuraScript
        {
            PrepareAuraScript(spell_tayak_su_visualAuraScript);

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_STORM_UNLEASHED_D, caster);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_tayak_su_visualAuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_tayak_su_visualAuraScript();
        }
};

// 124024 - Storm unleashed dummy visual
class spell_su_dummy_visual : public SpellScriptLoader
{
    public:
        spell_su_dummy_visual() : SpellScriptLoader("spell_su_dummy_visual") { }

        class spell_su_dummy_visualAuraScript : public AuraScript
        {
            PrepareAuraScript(spell_su_dummy_visualAuraScript);

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_SU_DUMMY_VIS, caster);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_su_dummy_visualAuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScrpt() const
        {
            return new spell_su_dummy_visualAuraScript();
        }
};

// 123633 - Gale Winds
class spell_gale_winds : public SpellScriptLoader
{
    public:
        spell_gale_winds() : SpellScriptLoader("spell_gale_winds") { }

        class spell_gale_windsAuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gale_windsAuraScript);

            void Apply(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_GALE_FORCE_WINDS, caster);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gale_windsAuraScript::Apply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gale_windsAuraScript();
        }
};

// 123600 - Storm unleashed
class spell_su_dummy : public SpellScriptLoader
{
    public:
        spell_su_dummy() : SpellScriptLoader("spell_su_dummy") { }

        class spell_su_dummyAuraScript : public AuraScript
        {
            PrepareAuraScript(spell_su_dummyAuraScript);

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Creature*> galeList;
                    GetCreatureListWithEntryInGrid(galeList, caster, NPC_GALE_WINDS_STALKER, 15.0f);
                    for (auto gale : galeList)
                        caster->AddAura(SPELL_SU_DUMMY_CRAP, gale);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_su_dummyAuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_su_dummyAuraScript();
        }

};

// 123616 - Storm unleashed
class spell_su_dumaura : public SpellScriptLoader
{
    public:
        spell_su_dumaura() : SpellScriptLoader("spell_su_dumaura") { }

        class spell_su_dumauraAuraScript : public AuraScript
        {
            PrepareAuraScript(spell_su_dumauraAuraScript);

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, caster, 40.0f);

                    for (auto player : playerList)
                        caster->AddAura(SPELL_SU_DUMMY_CRAP, player);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_su_dumauraAuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_su_dumauraAuraScript();
        }
};

// 125310
class spell_blade_tempest : public SpellScriptLoader
{
    public:
        spell_blade_tempest() : SpellScriptLoader("spell_blade_tempest") { }

        class spell_blade_tempest_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_blade_tempest_SpellScript);

            void PullRaid()
            {
                if (Unit* Tayak = GetCaster())
                    if (Tayak->GetEntry() == NPC_TAYAK)
                        Tayak->GetAI()->DoAction(ACTION_TAYAK_BT_PULL);
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_blade_tempest_SpellScript::PullRaid);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_blade_tempest_SpellScript();
        }
};

void AddSC_boss_blade_lord_tayak()
{
    new boss_tayak();                       ///< 62543
    new npc_tempest_slash_tornado();        ///< 62908
    new npc_storm_unleashed_tornado();      ///< 63278
    new mob_gale_winds_stalker();           ///< 63292
    new spell_wind_step();                  ///< 123175
    new spell_tayak_wind_step();            ///< 123459
    new spell_tayak_storms_vehicle();       ///< 124258
    new spell_tayak_storm_unleashed_dmg();  ///< 124783
    new spell_tempest_slash();              ///< 122853
    new spell_unseen_strike_aura();         ///< 122982
    new spell_unseen_strike_dmg();          ///< 122994
    new spell_tayak_su_visual();            ///< 123814
    new spell_su_dummy_visual();            ///< 124024
    new spell_gale_winds();                 ///< 123633
    new spell_su_dummy();                   ///< 123600
    new spell_su_dumaura();                 ///< 123616
    new spell_blade_tempest();              ///< 125310
}
