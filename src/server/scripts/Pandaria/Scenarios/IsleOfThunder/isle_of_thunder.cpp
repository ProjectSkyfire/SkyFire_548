/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "isle_of_thunder.h"
#include "CreatureTextMgr.h"
#include "Vehicle.h"
#include "LFGMgr.h"

enum Spells
{
    /*Zebtula | Shaolmara*/
    SPELL_LIGHTNING_THREE         = 140043,
    SPELL_LIGHTNING_STRIKE        = 137516, // vehicle breaker
    SPELL_PERMANENT_FEIGN_DEATH   = 70628,
    SPELL_BEAST_WITHIN            = 140110,
    SPELL_TWO_HANDED_STRIKE       = 140112,
    SPELL_DUST_CLOUD              = 140102,
    SPELL_DUST_CLOUD_EFF          = 140103,
    SPELL_EARTH_SHATTER           = 140222,
    SPELL_THROW_AXE               = 126375,
    SPELL_LIGHTNING_CHARGED       = 140268,
    SPELL_ARCANITALS_SPARK        = 140324,
    SPELL_WAVES_OF_LIGHT          = 140327,
    SPELL_WINGS_OF_VENGEANCE      = 140331,
    SPELL_HYMN_OF_SILENCE         = 140315,
    SPELL_BARRIER_STANCE          = 140117,
    SPELL_SERRATED_SLASH          = 87395,
    SPELL_SHIELD_BASH             = 135620,
    SPELL_THUNDERCLEAVE           = 139035,
    SPELL_LAUNCH_SPEAR            = 137673,
    SPELL_ARCANITAL_BARRIER       = 140662,
    SPELL_ARCANITAL_BARRIER_EFF   = 140661,
    SPELL_BERSERKER_RUSH          = 140079,
                                  
    /*Tear Down This Wall*/       
    SPELL_LAUNCH_INTRO_SCENE      = 137570,
    SPELL_LAUNCH_WALL_SCENE       = 137490,
    SPELL_WALL_EXPLOSIVE          = 137525,
    SPELL_LIGHTNING_POLE          = 139185,
    SPELL_LIGHTNING_POLE_AT       = 139187,
    SPELL_ALCHUKLA_LIGHTNING_BALL = 139562,
    SPELL_BLOODY_STRIKE           = 77558,
    SPELL_RAGE_ZONE               = 90385,
    SPELL_VOID_SOUL               = 140521,
    SPELL_BODY_OF_STONE           = 55633,
    SPELL_SPIRIT_CLEAVE           = 115530,
    SPELL_LEGACY_OF_BLOOD         = 140497,
    SPELL_LEGACY_OF_BLOOD_EFF     = 140498,
    SPELL_STARFALL                = 140530,
    SPELL_CURSE_OF_BABBLING_TONG  = 140496,

    /*In Sky!*/
    SPELL_CAPTURE                 = 138627,
    SPELL_DEADLY_KICK             = 138632,
    SPELL_LIGHTNING_CHARGE        = 136495,
    SPELL_SHANZE_LIGHTNING_STRIKE = 136441,
    SPELL_SHANZE_CHAIN_LIGHTNING  = 128123,
    SPELL_LIGHTNING_BLAST         = 136439,
    SPELL_ELECTRIFIED_CHAINS      = 136695,
    SPELL_BROKEN_CHAINS           = 137522,
    SPELL_FROST_SHOT              = 137435,
    SPELL_FROST_SHOT_EFF          = 137434,
    SPELL_SHOT                    = 140592,
    SPELL_HAIL_OF_ARROWS          = 137424,
    SPELL_HAIL_OF_ARROWS_EFF      = 137509,
    SPELL_THUNDER_INNERVATION_EFF = 137476,
    SPELL_THUNDER_INNERVATION     = 137477,
    SPELL_JUMP                    = 137263,
    SPELL_FORCE_JUMP_BACK         = 137258,
    SPELL_JUMP_OVERRIDE           = 137264,
    SPELL_JUMP_OVERRIDE_TECH      = 137265,
    SPELL_SUMMON_TAOSHI           = 138405, // like pet
    SPELL_THROW                   = 137438,
    SPELL_RIDE_VEHICLE            = 137253,
    SPELL_POWER_SURGE_SKY         = 136842,
    SPELL_POWER_SURGE_SKY_AT      = 137536,

    /*Assault on Stormsea Landing*/
    SPELL_INFILT_STORMSEA_LANDING = 137309,
    SPELL_INFILTRATING_EFF        = 140639, // invis
    SPELL_SMOKE_BOMB_AVAILABLE    = 140467,
    SPELL_SENTRY                  = 137254,
    SPELL_SENTRY_BEAM             = 138812,
    SPELL_SUMMON_SENTRY_LASER     = 138722,
    SPELL_COSMETIC_SLEEP          = 55474,
    SPELL_MARK_OF_TAOSHI          = 137282,
    SPELL_MARK_OF_TAOSHI_EFF      = 137311,
    SPELL_ABMUSH_OVERRIDE_AURA    = 137312,
    SPELL_WELL_FED                = 140410,
    SPELL_WALLOP                  = 140398,
    SPELL_KEG_TOSS                = 140391,
    SPELL_KEG_TOSS_EFF            = 140392,
    SPELL_BUBBLING_BREW           = 140393,
    SPELL_HANGOVER                = 140432,
    SPELL_EMPOWER_AXE             = 140543,
    SPELL_DESIST                  = 140531,
    SPELL_STURDY                  = 140532, // just visual
    SPELL_CEASE                   = 140511,
    SPELL_CEASE_EFF               = 140519,
    SPELL_HUSENG_ASLEEP           = 139538,
    SPELL_BASH_SKULL              = 129111,
    SPELL_THUNDER_CLAP            = 140445,
    SPELL_SPIRIT_TWISTING         = 136506,
    SPELL_SPIRIT_BOLT             = 136509,
    SPELL_SPOTTED                 = 138954,
    SPELL_GATEKEEPERS_ORB         = 139274,
    SPELL_FREEZE_STATE            = 89476,
    SPELL_BOULDER_THROW           = 139280,
    SPELL_THUNDERING_SLAM         = 139379,
    SPELL_CRUSHING_STOMP          = 139378,
    SPELL_FIXATED_LEAP            = 124112,
    SPELL_STONE_CLAW              = 126793,
    SPELL_VISIOUS_REND            = 127171,
    SPELL_STONE                   = 138155,
    SPELL_DRAW_SHADOWS            = 138868,
    SPELL_SHADOW_CRASH            = 138862,
    SPELL_RESSURECT_NALAK         = 138648,
    SPELL_COSMETIC_SHADOW_NOVA    = 118437,
    SPELL_REBITH                  = 138776,
    SPELL_ELECTRICAL_STORM        = 137301,
    SPELL_BLOOD_DRIP              = 139222,
    SPELL_LIGHTNING_BARRIER       = 137546,

    /*Thunder Forge*/
    SPELL_FIREBALL                = 136333,
    SPELL_CLEAVE                  = 138794,
    SPELL_MORTAL_STRIKE           = 120436,
    SPELL_METEOR                  = 138168,
    SPELL_METEOR_AT               = 140660,
    SPELL_METEOR_AT_EFF           = 138167,
    SPELL_LIGHTNING_SMASH         = 138831,
    SPELL_ELECTRFIED              = 138821,
    SPELL_THUNDER_FORGE_CHARGING  = 140334,
    SPELL_THUNDER_FORGE_SMITHING  = 140487,
    SPELL_THUNDER_FORGE_CHANNEL   = 137599,
    SPELL_THUNDER_FORGE_CHARGE    = 140382,
    SPELL_THUNDER_FORGE_CHARGE_2  = 140489,
    SPELL_FORGEMASTER_SPAWN       = 142594,
    SPELL_HEALING_ORB_AT          = 139249,
    SPELL_HEALING_ORB_AT_EFF      = 132744,
    SPELL_OVERCHARGE_EFF          = 139397, // not found creator
    SPELL_POWER_SURGE_EFF         = 140068,
    SPELL_POWER_SURGE_AT          = 140067,
    SPELL_SHA_COSMETIC_SPAWN      = 139444,
    SPELL_SHA_BLAST               = 138681,
    SPELL_INSANITY                = 139382,
    SPELL_METEOR_STORM            = 139602,
    SPELL_SHADOW_CRASH_2          = 139312,
    SPELL_THUNDER_SURGE           = 138834,

    // Misc
    SPELL_SUMMON_LORTHEMAR        = 139721,
};

enum Events
{
    EVENT_BEAST_WITHIN,
    EVENT_TWO_HANDED_STRIKE,
    EVENT_DUST_CLOUD,
    EVENT_EARTH_SHATTER,
    EVENT_THROW_AXE,
    EVENT_LIGHTNING_CHARGED,
    EVENT_ARCANITAL_SPARK,
    EVENT_WAVES_OF_LIGHT,
    EVENT_HYMN_OF_SILENCE,
    EVENT_SPECIAL_ATTACK,
    EVENT_SHIELD_BASH,
    EVENT_BARRIER_STANCE,
    EVENT_LAUNCH_SPEAR,
    EVENT_MISC,
    EVENT_BERSERKER_RUSH,
    EVENT_LIGHTNING_POLE,
    EVENT_BLOODY_STRIKE,
    EVENT_RAGE_ZONE,
    EVENT_VOID_SOUL,
    EVENT_BODY_OF_STONE,
    EVENT_SPIRIT_CLEAVE,
    EVENT_LEGACY_OF_BLOOD,
    EVENT_STARFALL,
    EVENT_CURSE_OF_BABBLING_TONG,
    EVENT_CAPTURE,
    EVENT_DEADLY_KICK,
    EVENT_CHAIN_LIGHTNING,
    EVENT_LIGHTNING_STRIKE,
    EVENT_ELECTRIFIED_CHAINS,
    EVENT_HAIL_OF_ARROWS,
    EVENT_SHOT,
    EVENT_FROST_SHOT,
    EVENT_WELL_FED,
    EVENT_WALLOP,
    EVENT_KEG_TOSS,
    EVENT_EMPOWER_AXE,
    EVENT_CEASE,
    EVENT_BASH_SKULL,
    EVENT_THUNDER_CLAP,
    EVENT_SPIRIT_TWISTING,
    EVENT_SPIRIT_BOLT,
    EVENT_BOULDER_THROW,
    EVENT_THUNDERING_SLAM,
    EVENT_CRUSHING_STOMP,
    EVENT_STONE_CLAW,
    EVENT_VISIOUS_REND,
    EVENT_DRAW_SHADOWS,
    EVENT_SHADOW_CRASH,
    EVENT_CLEAVE,
    EVENT_MORTAL_STRIKE,
    EVENT_FIREBALL,
    EVENT_METEOR,
    EVENT_HEALING_ORB,
    EVENT_SHA_BLAST,
    EVENT_INSANITY,
    EVENT_METEOR_STORM,
    EVENT_LIGHTNING_SMASH,
    EVENT_ELECTRIFIED,
    EVENT_SWITCH_WEAPONS, // just announce, doesn`t know what should`ve happened.
    EVENT_POWER_SURGE,
};

enum Yells
{
    TALK_INTRO,
    TALK_SPECIAL_1,
    TALK_SPECIAL_2,
    TALK_SPECIAL_3,
    TALK_SPECIAL_4,
    TALK_SPECIAL_5,
    TALK_SPECIAL_6,
    TALK_SPECIAL_7,
    TALK_SPECIAL_8,
    TALK_SPECIAL_9,
    TALK_SPECIAL_10,
    TALK_SPECIAL_11,
    TALK_SPECIAL_12,
    TALK_SPECIAL_13,
    TALK_SPECIAL_14,
    TALK_SPECIAL_15,
    TALK_SPECIAL_16,
    TALK_SPECIAL_17,
    TALK_SPECIAL_18,
    TALK_SPECIAL_19,
    TALK_SPECIAL_20,
};

// Lady Jaina Proudmoore 70131
class npc_shaolmara_lady_jaina : public CreatureScript
{
    public:
        npc_shaolmara_lady_jaina() : CreatureScript("npc_shaolmara_lady_jaina") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver() && creature->GetMapId() == 1064)
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(QUEST_THE_ASSAULT_ON_SHAOLMARA) == QUEST_STATUS_INCOMPLETE && creature->GetMapId() == 1064)
                player->ADD_GOSSIP_ITEM_DB(LADY_JAINA_GOSSIP_MENU, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            if (player->GetQuestStatus(QUEST_TEAR_DOWN_THIS_WALL_A) == QUEST_STATUS_INCOMPLETE && creature->GetMapId() == 1064)
                player->ADD_GOSSIP_ITEM_DB(LADY_JAINA_GOSSIP_MENU, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

            if (player->GetQuestStatus(QUEST_TO_THE_SKIES_A) == QUEST_STATUS_INCOMPLETE && creature->GetMapId() == 1064)
                player->ADD_GOSSIP_ITEM_DB(LADY_JAINA_GOSSIP_MENU, 2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

            if (player->GetQuestStatus(QUEST_FALL_OF_SHANBU_A) == QUEST_STATUS_INCOMPLETE && creature->GetMapId() == 1064)
                player->ADD_GOSSIP_ITEM_DB(LADY_JAINA_GOSSIP_MENU, 3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                {
                    lfg::LfgDungeonSet scenario = { LFG_DUNGEON_ASSAULT_ON_SHAOLMARA };
                    sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
                    player->KilledMonsterCredit(69599);
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 2:
                {
                    lfg::LfgDungeonSet scenario = { LFG_DUNGEON_BREAK_THIS_WALL_A };
                    sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
                    player->KilledMonsterCredit(70282);
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 3:
                {
                    lfg::LfgDungeonSet scenario = { LFG_DUNGEON_TO_SKIES_A };
                    sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
                    player->KilledMonsterCredit(70112);
                    break;
                }

                case GOSSIP_ACTION_INFO_DEF + 4:
                {
                    lfg::LfgDungeonSet scenario = { LFG_DUNGEON_FALL_OF_SHANBU_A };
                    sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
                    player->KilledMonsterCredit(67992);
                    break;
                }
            }

            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        struct npc_shaolmara_lady_jainaAI : public ScriptedAI
        {
            npc_shaolmara_lady_jainaAI(Creature* creature) : ScriptedAI(creature) { }

            TaskScheduler scheduler;
            float x, y;

            void InitializeAI() OVERRIDE
            {
                if (me->GetMapId() == 1064)
                    return;

                if (!me->GetDBTableGUIDLow())
                    return;

                scheduler.Schedule(Seconds(3), [this](TaskContext context)
                {
                    Talk(TALK_INTRO);

                    Movement::MoveSplineInit init(me);

                    for (auto&& itr : jainaPath)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    init.SetWalk(true);
                    init.Launch();

                    uint32 delay = me->GetSplineDuration();

                    scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
                    {
                        Talk(TALK_SPECIAL_1);
                    });

                    scheduler.Schedule(Milliseconds(delay + 5 * IN_MILLISECONDS), [this](TaskContext context)
                    {
                        Talk(TALK_SPECIAL_2);
                    });
                });
            }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                uint32 delay = 2500;
                scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_3);
                });

                scheduler.Schedule(Milliseconds(delay+=7800), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_4);
                });

                scheduler.Schedule(Milliseconds(delay += 6500), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_5);
                });

                scheduler.Schedule(Milliseconds(delay += 8000), [this](TaskContext context)
                {
                    // @Todo - complete Scenario credit

                    if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                        target->TeleportTo(1064, 6147.86f, 5035.56f, 36.89f, 0.0f, 0);
                });
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                scheduler.Update(diff);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_shaolmara_lady_jainaAI(creature);
        }
};

// Player Covenant Hippogryph 70234
struct npc_shaolmara_player_covenant_hippogryph : public ScriptedAI
{
    npc_shaolmara_player_covenant_hippogryph(Creature* creature) : ScriptedAI(creature) 
    {
        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        me->SetSpeed(MOVE_RUN, 3.35f);
    }

    TaskScheduler scheduler;

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        clicker->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, false);
        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_MEET_WITH_RANGER, IN_PROGRESS);

        // Delay for init
        scheduler.Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            Movement::MoveSplineInit init(me);

            for (auto&& itr : flyPath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.SetFly();
            init.SetUncompressed();
            init.Launch();

            scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
            {
                DoCast(me, SPELL_LIGHTNING_STRIKE, true);
            });
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_FLY_FALL)
            me->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetMap()->GetHeight(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), true, 100.0f), 20.0f, 20.0f, EVENT_JUMP);
    }

    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        if (pointId == EVENT_JUMP)
        {
            if (me->GetVehicleKit() && me->GetVehicleKit()->GetPassenger(0))
                me->GetVehicleKit()->GetPassenger(0)->ExitVehicle();

            // Start next scenario step
            if (Creature* vereesa = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_VEREESA_WINDRUNNER) : 0))
                vereesa->AI()->DoAction(ACTION_FLY_FALL);

            me->DisappearAndDie();
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Covenant Hippogryph 70233, 70231
struct npc_shaolmara_covenant_hippogryph : public ScriptedAI
{
    npc_shaolmara_covenant_hippogryph(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        me->SetSpeed(MOVE_RUN, 3.35f);

        // Delay for init
        scheduler.Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            if (Creature* ranger = me->SummonCreature(me->GetEntry() == NPC_VEERISA_HIPPOGRYPH ? NPC_VEREESA_WINDRUNNER : NPC_SILVER_COVENANT_RANGER, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                ranger->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            // Delay for init
            scheduler.Schedule(Milliseconds(1500), [this](TaskContext context)
            {
                Movement::MoveSplineInit init(me);

                for (auto&& itr : flyPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX() + frand(-5.0f, 5.0f), itr.GetPositionY() + frand(-10.0f, 10.0f), itr.GetPositionZ() + frand(-1.5f, 1.5f)));

                init.SetFly();
                init.SetUncompressed();
                init.Launch();

                uint32 delay = me->GetEntry() == NPC_VEERISA_HIPPOGRYPH ? me->GetSplineDuration() : me->GetSplineDuration() - rand() % 8;
                scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
                {
                    DoCast(me, SPELL_LIGHTNING_STRIKE, true);
                });
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Vereesa Windrunner 70041
struct npc_shaolmara_vereesa_windrunner : public ScriptedAI
{
    npc_shaolmara_vereesa_windrunner(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    EventMap events;
    uint32 delay;
    bool thirdStep;

    void DoAction(int32 actionId) OVERRIDE
    {
        switch (actionId)
        {
            case ACTION_START_INTRO:
                Talk(TALK_INTRO);

                delay = 5 * IN_MILLISECONDS;
                scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_1);
                });

                scheduler.Schedule(Milliseconds(delay += 10800), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_2);
                });

                scheduler.Schedule(Milliseconds(delay += 12250), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_3);
                });

                scheduler.Schedule(Milliseconds(delay += 10900), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_4);
                });

                scheduler.Schedule(Milliseconds(delay += 9800), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_5);
                });

                scheduler.Schedule(Milliseconds(delay += 7300), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_6);
                });

                scheduler.Schedule(Milliseconds(delay += 4000), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_7);
                });
                break;
            case ACTION_FLY_FALL:
                scheduler.Schedule(Milliseconds(2000), [this](TaskContext context)
                {
                    if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                        me->GetMotionMaster()->MovePoint(0, *target);

                    Talk(TALK_SPECIAL_8);
                });

                scheduler.Schedule(Milliseconds(4800), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_9);

                    if (me->GetInstanceScript())
                    {
                        me->GetInstanceScript()->SetData(STEP_MEET_WITH_RANGER, DONE);
                        me->GetInstanceScript()->SetData(STEP_SURVIVE_THE_AMBUSH, IN_PROGRESS);
                    }
                });
                break;
            case ACTION_SURPRISED:
                scheduler.Schedule(Milliseconds(5000), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_11);
                });
                break;
            case ACTION_AMBUSH_PREVENTED:
                me->GetMotionMaster()->MovePoint(0, vereesaAmbushPoint);

                delay = me->GetSplineDuration();
                scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_12);
                    me->StopMoving();
                });

                scheduler.Schedule(Milliseconds(delay += 5 * IN_MILLISECONDS), [this](TaskContext context)
                {
                    Movement::MoveSplineInit init(me);

                    for (auto&& itr : vereesaBarricadePath)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    init.SetUncompressed();
                    init.Launch();

                    scheduler.Schedule(Milliseconds(me->GetSplineDuration() + 3 * IN_MILLISECONDS), [this](TaskContext context)
                    {
                        thirdStep = false;
                        Talk(TALK_SPECIAL_13);

                        if (me->GetInstanceScript())
                            me->GetInstanceScript()->SetData(STEP_DESTROY_THE_BARRICADES, IN_PROGRESS);

                        if (Creature* arcanital = me->SummonCreature(NPC_ARCANITAL_MARAKAH, arcanitalBarricadePoint, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, Milliseconds(5 * IN_MILLISECONDS)))
                            arcanital->AI()->Talk(TALK_SPECIAL_2);

                        // Wait
                        scheduler
                            .SetValidator([this] { return !thirdStep; })
                            .Schedule(Milliseconds(1000), [this](TaskContext context)
                        {
                            if (me->FindNearestCreature(NPC_ZEBTULA_STONESHIELD, 150.0f, true) || me->FindNearestCreature(NPC_ZANDALARI_BARRICADE_A, 150.0f, true) || me->FindNearestCreature(NPC_ZEBTULA_SPEARANGER, 150.0f, true))
                            {
                                context.Repeat(Milliseconds(1000));
                                return;
                            }

                            thirdStep = true;
                            Talk(TALK_SPECIAL_14);
                            me->StopMoving();

                            if (me->GetInstanceScript())
                                me->GetInstanceScript()->SetData(STEP_DESTROY_THE_BARRICADES, DONE);
                        });
                    });
                });
                break;
            case ACTION_ANKI_DEFEAT:
            {
                Talk(TALK_SPECIAL_16);
                me->StopMoving();

                if (me->GetInstanceScript())
                    me->GetInstanceScript()->SetData(STEP_ANKI_AND_PAKUTESH, DONE);

                Movement::MoveSplineInit init(me);

                for (auto&& itr : vereesaArcanitalPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetUncompressed();
                init.Launch();

                events.ScheduleEvent(EVENT_MISC + 3, me->GetSplineDuration());
                break;
            }
            case ACTION_BEHIND_ANKI:
            {
                me->StopMoving();
                Movement::MoveSplineInit init(me);

                for (auto&& itr : vereesaAnkiPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetUncompressed();
                init.Launch();
                events.ScheduleEvent(EVENT_MISC, me->GetSplineDuration());
                break;
            }
            case ACTION_MARAKAH_DEFEATED:
                Talk(TALK_SPECIAL_17);
                events.ScheduleEvent(EVENT_MISC + 2, 5 * IN_MILLISECONDS);
                break;
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_MISC: // Scheduler not schedule anymore...
                    if (Creature* arcanital = me->SummonCreature(NPC_ARCANITAL_MARAKAH, arcanitalAnkiPoint, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, Milliseconds(5 * IN_MILLISECONDS)))
                        arcanital->AI()->Talk(TALK_SPECIAL_3);
                
                    events.ScheduleEvent(EVENT_MISC + 1, 4500);
                    break;
                case EVENT_MISC + 1:
                    Talk(TALK_SPECIAL_15);
                
                    me->SummonCreature(NPC_ANKI, ankiSpawnPoint, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    break;
                case EVENT_MISC + 2:
                    Talk(TALK_SPECIAL_18);
                    me->SummonCreature(NPC_LADY_JAINA_GUNSHIP, jainaSummPoint, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    break;
                case EVENT_MISC + 3:
                    if (Creature* arcanital = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_ARCANITAL_MARAKAH) : 0))
                    {
                        arcanital->SetVisible(true);
                        arcanital->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                    }
                    break;
            }
            break;
        }
    }
};

// Shaolmara Beastcaller 70286
struct npc_shaolmara_beastcaller : public customCreatureAI
{
    npc_shaolmara_beastcaller(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    float x, y;

    void InitializeAI() OVERRIDE
    {
        Reset();

        if (me->GetDBTableGUIDLow())
            return;

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);
    }
    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        for (uint32 i = 0; i < 2; i++)
        {
            GetPositionWithDistInOrientation(me, 3.0f, Position::NormalizeOrientation(me->GetOrientation() - M_PI / 2 + M_PI * i), x, y);
            me->SummonCreature(NPC_SHAOL_MARA_RAPTOR, x, y, me->GetMap()->GetHeight(x, y, me->GetPositionZ(), true, 5.0f));
        }

        // Init path before combat
        GetPositionWithDistInOrientation(me, 35.0f, me->GetOrientation(), x, y);

        Movement::MoveSplineInit init(me);
        init.MoveTo(x, y, me->GetMap()->GetHeight(x, y, me->GetPositionZ(), true, 20.0f));

        init.Launch();

        scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);

            if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                SetGazeOn(target);
        });
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_TWO_HANDED_STRIKE, 3500);
        events.ScheduleEvent(EVENT_BEAST_WITHIN, 8 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_TWO_HANDED_STRIKE, 7500, EVENT_TWO_HANDED_STRIKE, eventId);
            ExecuteTargetEvent(SPELL_BEAST_WITHIN, 12 * IN_MILLISECONDS, EVENT_BEAST_WITHIN, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Shaolmara Raptor 70288
struct npc_shaolmara_raptor : public customCreatureAI
{
    npc_shaolmara_raptor(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    float x, y;

    void InitializeAI() OVERRIDE
    {
        Reset();

        if (me->GetDBTableGUIDLow())
            return;

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);
    }
    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        // Init path before combat
        GetPositionWithDistInOrientation(me, 35.0f, me->GetOrientation(), x, y);

        Movement::MoveSplineInit init(me);
        init.MoveTo(x, y, me->GetMap()->GetHeight(x, y, me->GetPositionZ(), true, 20.0f));

        init.Launch();

        scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);

            if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                SetGazeOn(target);
        });
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_DUST_CLOUD, urand(3500, 12 * IN_MILLISECONDS));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_DUST_CLOUD, urand(7 * IN_MILLISECONDS, 18 * IN_MILLISECONDS), EVENT_DUST_CLOUD, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Arcanital Marakah 69538
struct npc_arcanital_marakah : public customCreatureAI
{
    npc_arcanital_marakah(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    float x, y;
    bool secondStep;

    void InitializeAI() OVERRIDE
    {
        Reset();

        if (me->GetDBTableGUIDLow())
            return;

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);
    }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (me->GetInstanceScript() && me->GetInstanceScript()->GetData(STEP_SURVIVE_THE_AMBUSH) == DONE)
            return;

        secondStep = false;

        Talk(TALK_INTRO);

        if (Creature* vereesa = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_VEREESA_WINDRUNNER) : 0))
            vereesa->AI()->DoAction(ACTION_SURPRISED);

        scheduler
            .SetValidator([this] { return !secondStep; })
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            if (me->FindNearestCreature(NPC_SHAOL_MARA_BEASTCALLER, 150.0f, true) || me->FindNearestCreature(NPC_SHAOL_MARA_RAPTOR, 150.0f, true))
            {
                context.Repeat(Milliseconds(1000));
                return;
            }

            secondStep = true;
            Talk(TALK_SPECIAL_1);
            me->GetMotionMaster()->MovePoint(0, arcanitalLeave);

            if (me->GetInstanceScript())
                me->GetInstanceScript()->SetData(STEP_SURVIVE_THE_AMBUSH, DONE);

            if (Creature* vereesa = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_VEREESA_WINDRUNNER) : 0))
                vereesa->AI()->DoAction(ACTION_AMBUSH_PREVENTED);
        });
    }

    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        if (pointId == 0)
            me->DespawnOrUnsummon();
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_ARCANITAL_SPARK, urand(3500, 8 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_WAVES_OF_LIGHT, 11 * IN_MILLISECONDS);

        DoCast(me, SPELL_ARCANITAL_BARRIER, true);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        me->RemoveAllAreasTrigger();

        if (Creature* vereesa = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_VEREESA_WINDRUNNER) : 0))
            vereesa->AI()->DoAction(ACTION_MARAKAH_DEFEATED);

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_KILL_ARCANITAL, DONE);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_ARCANITALS_SPARK, urand(7 * IN_MILLISECONDS, 12 * IN_MILLISECONDS), EVENT_ARCANITAL_SPARK, eventId);
            ExecuteTargetEvent(SPELL_WAVES_OF_LIGHT, urand(15 * IN_MILLISECONDS, 20 * IN_MILLISECONDS), EVENT_WAVES_OF_LIGHT, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Anki 69531
struct npc_shaolmara_anki : public customCreatureAI
{
    npc_shaolmara_anki(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    uint64 passengerGUID;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        // Delay for init
        scheduler.Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            if (Creature* pakutesh = me->SummonCreature(NPC_BEASTCALLER_PAKUTESH, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
            {
                passengerGUID = pakutesh->GetGUID();
                pakutesh->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);
            }
        });
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_EARTH_SHATTER, urand(5 * IN_MILLISECONDS, 7 * IN_MILLISECONDS));
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (Creature* pakutesh = ObjectAccessor::GetCreature(*me, passengerGUID))
            pakutesh->AI()->DoAction(ACTION_ANKI_DEFEAT);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_EARTH_SHATTER, urand(9 * IN_MILLISECONDS, 13 * IN_MILLISECONDS), EVENT_EARTH_SHATTER, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Beastcaller Pakutesh 69532
struct npc_shaolmara_beastcaller_pakutesh : public customCreatureAI
{
    npc_shaolmara_beastcaller_pakutesh(Creature* creature) : customCreatureAI(creature) 
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_THROW_AXE, urand(3500, 8 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_LIGHTNING_CHARGED, 10 * IN_MILLISECONDS);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (Creature* vereesa = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_VEREESA_WINDRUNNER) : 0))
            vereesa->AI()->DoAction(ACTION_ANKI_DEFEAT);
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
            ExecuteTargetEvent(SPELL_THROW_AXE, urand(3500, 8 * IN_MILLISECONDS), EVENT_THROW_AXE, eventId);
            ExecuteTargetEvent(SPELL_LIGHTNING_CHARGED, 15 * IN_MILLISECONDS, EVENT_LIGHTNING_CHARGED, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

struct barricadeBaseAI : public customCreatureAI
{
    barricadeBaseAI(Creature* creature) : customCreatureAI(creature) { }

    void DoAction(int32 actionId) OVERRIDE
    {
        switch (actionId)
        {
            case ACTION_BARRICADE_ASSAULT:
                me->SetVisible(true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
            
                if (me->GetEntry() == NPC_ZANDALARI_BARRICADE_A || me->GetEntry() == NPC_ZANDALARI_BARRICADE_H)
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
            
                if (me->GetEntry() == NPC_ZEBTULA_SPEARANGER)
                {
                    SetCombatMovement(false);
                    events.ScheduleEvent(EVENT_LAUNCH_SPEAR, urand(2 * IN_MILLISECONDS, 6 * IN_MILLISECONDS));
                }
                break;
            case ACTION_START_INTRO:
                if (me->GetPositionX() > 91.0f)
                {
                    me->SetVisible(true);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                }
                break;
            case ACTION_START_ASSAULT:
                if (Creature* taranZhu = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_SHANBU_TARAN_ZHU) : 0))
                    me->GetMotionMaster()->MoveJump(*taranZhu, 20.0f, 15.0f);
                break;
        }
    }
};

// Zebtula Stoneshield 69517, 69865, 69865
struct npc_shaolmara_zebtula_stoneshield : public barricadeBaseAI
{
    npc_shaolmara_zebtula_stoneshield(Creature* creature) : barricadeBaseAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SPECIAL_ATTACK, urand(3500, 8 * IN_MILLISECONDS));

        if (me->GetEntry() != NPC_TEARDOWN_ZANDALARI_STONESHIELD)
            events.ScheduleEvent(EVENT_BARRIER_STANCE, urand(10 * IN_MILLISECONDS, 13 * IN_MILLISECONDS));

        events.ScheduleEvent(EVENT_SHIELD_BASH, 9 * IN_MILLISECONDS);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(75) && attacker->GetTypeId() != TypeID::TYPEID_PLAYER)
            damage = 0;
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (me->GetInstanceScript() && me->GetEntry() == NPC_ZANDALARI_STONE_SHIELD_3)
            me->GetInstanceScript()->SetData(STEP_BLOODY_CROSSROAD, SPECIAL);
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
            ExecuteTargetEvent(urand(0, 1) ? SPELL_SERRATED_SLASH : SPELL_THUNDERCLEAVE, urand(3500, 8 * IN_MILLISECONDS), EVENT_SPECIAL_ATTACK, eventId);
            ExecuteTargetEvent(SPELL_BARRIER_STANCE, 15 * IN_MILLISECONDS, EVENT_BARRIER_STANCE, eventId, PRIORITY_CHANNELED);
            ExecuteTargetEvent(SPELL_SHIELD_BASH, 9 * IN_MILLISECONDS, EVENT_SHIELD_BASH, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Barricade 69516, 69518
struct npc_shaolmara_zandalari_barricade : public barricadeBaseAI
{
    npc_shaolmara_zandalari_barricade(Creature* creature) : barricadeBaseAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        me->DisappearAndDie();
    }

    void UpdateAI(uint32 diff) OVERRIDE 
    {
        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_LAUNCH_SPEAR)
            {
                if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                    DoCast(target, SPELL_LAUNCH_SPEAR);

                events.ScheduleEvent(EVENT_LAUNCH_SPEAR, urand(4 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
            }
            break;
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// Lorthemar Theron 67990
class npc_zebtula_lorthemar_theron : public CreatureScript
{
    public:
        npc_zebtula_lorthemar_theron() : CreatureScript("npc_zebtula_lorthemar_theron") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver() && creature->GetMapId() == 1064)
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(QUEST_THE_ASSAULT_ON_ZEB_TULA) == QUEST_STATUS_INCOMPLETE && creature->GetMapId() == 1064)
                player->ADD_GOSSIP_ITEM_DB(LOR_TEMAR_GOSSIP_MENU, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            if (player->GetQuestStatus(QUEST_TEAR_DOWN_THIS_WALL_H) == QUEST_STATUS_INCOMPLETE && creature->GetMapId() == 1064)
                player->ADD_GOSSIP_ITEM_DB(LADY_JAINA_GOSSIP_MENU, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

            if (player->GetQuestStatus(QUEST_TO_THE_SKIES_H) == QUEST_STATUS_INCOMPLETE && creature->GetMapId() == 1064)
                player->ADD_GOSSIP_ITEM_DB(LADY_JAINA_GOSSIP_MENU, 2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

            if (player->GetQuestStatus(QUEST_FALL_OF_SHANBU_H) == QUEST_STATUS_INCOMPLETE && creature->GetMapId() == 1064)
                player->ADD_GOSSIP_ITEM_DB(LADY_JAINA_GOSSIP_MENU, 3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                {
                    lfg::LfgDungeonSet scenario = { LFG_DUNGEON_ASSAULT_ON_ZEB_TULA };
                    sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
                    player->KilledMonsterCredit(69478);
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 2:
                {
                    lfg::LfgDungeonSet scenario = { LFG_DUNGEON_BREAK_THIS_WALL_H };
                    sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
                    player->KilledMonsterCredit(70281);
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 3:
                {
                    lfg::LfgDungeonSet scenario = { LFG_DUNGEON_TO_SKIES_H };
                    sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
                    player->KilledMonsterCredit(70112);
                    break;
                }

                case GOSSIP_ACTION_INFO_DEF + 4:
                {
                    lfg::LfgDungeonSet scenario = { LFG_DUNGEON_FALL_OF_SHANBU_H };
                    sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
                    player->KilledMonsterCredit(67990);
                    break;
                }
            }

            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        struct npc_zebtula_lorthemar_theronAI : public ScriptedAI
        {
            npc_zebtula_lorthemar_theronAI(Creature* creature) : ScriptedAI(creature) { }

            TaskScheduler scheduler;
            float x, y;

            void InitializeAI() OVERRIDE
            {
                if (me->GetMapId() == 1064)
                    return;

                if (!me->GetDBTableGUIDLow())
                    return;

                scheduler.Schedule(Seconds(3), [this](TaskContext context)
                {
                    Talk(TALK_INTRO);

                    scheduler.Schedule(Milliseconds(4000), [this](TaskContext context)
                    {
                        Talk(TALK_SPECIAL_1);

                        Movement::MoveSplineInit init(me);

                        for (auto&& itr : lorthemarPath)
                            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                        init.SetWalk(true);
                        init.Launch();

                        uint32 delay = me->GetSplineDuration();

                        scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_2);
                        });

                        scheduler.Schedule(Milliseconds(delay += 5 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_3);
                        });

                        scheduler.Schedule(Milliseconds(delay += 6 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_4);
                        });
                    });
                });
            }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                uint32 delay = 2500;
                scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_3);
                });
                scheduler.Schedule(Milliseconds(delay += 7800), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_4);
                });
                scheduler.Schedule(Milliseconds(delay += 6500), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_5);
                });
                scheduler.Schedule(Milliseconds(delay += 8000), [this](TaskContext context)
                {
                    // @Todo - complete Scenario credit
                    if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                        target->TeleportTo(1064, 7171.65f, 6294.49f, 12.46f, 2.09f, 0);
                });
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                scheduler.Update(diff);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_zebtula_lorthemar_theronAI(creature);
        }
};

// Player Bloodhawk 69487
struct npc_zebtula_player_bloodhawk : public ScriptedAI
{
    npc_zebtula_player_bloodhawk(Creature* creature) : ScriptedAI(creature)
    {
        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        me->SetSpeed(MOVE_RUN, 3.35f);
    }

    TaskScheduler scheduler;

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        clicker->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, false);
        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_MEET_WITH_RANGER, IN_PROGRESS);

        // Delay for init
        scheduler.Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            Movement::MoveSplineInit init(me);

            for (auto&& itr : hordeFlyPath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.SetFly();
            init.SetUncompressed();
            init.Launch();

            scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
            {
                DoCast(me, SPELL_LIGHTNING_STRIKE, true);
            });
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_FLY_FALL)
            me->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetMap()->GetHeight(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), true, 100.0f), 20.0f, 20.0f, EVENT_JUMP);
    }

    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        if (pointId == EVENT_JUMP)
        {
            if (me->GetVehicleKit() && me->GetVehicleKit()->GetPassenger(0))
                me->GetVehicleKit()->GetPassenger(0)->ExitVehicle();

            // Start next scenario step
            if (Creature* elsia = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_SCOUT_CAPTAIN_ELSIA) : 0))
                elsia->AI()->DoAction(ACTION_FLY_FALL);

            me->DisappearAndDie();
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Sunreaver Bloodhawk 69489, 69486
struct npc_zebtula_bloodhawk : public ScriptedAI
{
    npc_zebtula_bloodhawk(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        me->SetSpeed(MOVE_RUN, 3.35f);

        // Delay for init
        scheduler.Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            if (Creature* ranger = me->SummonCreature(me->GetEntry() == NPC_ELSIA_BLOODHAWK ? NPC_SCOUT_CAPTAIN_ELSIA : NPC_SUNREAVER_LIEUTENANT, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                ranger->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            // Delay for init
            scheduler.Schedule(Milliseconds(1500), [this](TaskContext context)
            {
                Movement::MoveSplineInit init(me);

                for (auto&& itr : hordeFlyPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX() + frand(-5.0f, 5.0f), itr.GetPositionY() + frand(-7.0f, 7.0f), itr.GetPositionZ() + frand(-1.5f, 1.5f)));

                init.SetFly();
                init.SetUncompressed();
                init.Launch();

                uint32 delay = me->GetEntry() == NPC_ELSIA_BLOODHAWK ? me->GetSplineDuration() : me->GetSplineDuration() - rand() % 8;
                scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
                {
                    DoCast(me, SPELL_LIGHTNING_STRIKE, true);
                });
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Scout Captain Elsia 69485
struct npc_zebtula_scout_captain_elsia : public ScriptedAI
{
    npc_zebtula_scout_captain_elsia(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    EventMap events;
    uint32 delay;
    bool thirdStep;

    void DoAction(int32 actionId) OVERRIDE
    {
        switch (actionId)
        {
            case ACTION_START_INTRO:
            {
                Talk(TALK_INTRO);

                delay = 5 * IN_MILLISECONDS;
                scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_1);
                });
                scheduler.Schedule(Milliseconds(delay += 10800), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_2);
                });
                scheduler.Schedule(Milliseconds(delay += 12250), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_3);
                });
                scheduler.Schedule(Milliseconds(delay += 10900), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_4);
                });
                scheduler.Schedule(Milliseconds(delay += 9800), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_5);
                });
                scheduler.Schedule(Milliseconds(delay += 7300), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_6);
                });
                scheduler.Schedule(Milliseconds(delay += 4000), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_7);
                });
                break;
            }
            case ACTION_FLY_FALL:
            {
                scheduler.Schedule(Milliseconds(2100), [this](TaskContext context)
                {
                    if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                        me->GetMotionMaster()->MovePoint(0, *target);

                    Talk(TALK_SPECIAL_8);
                });

                scheduler.Schedule(Milliseconds(4800), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_9);

                    if (me->GetInstanceScript())
                    {
                        me->GetInstanceScript()->SetData(STEP_MEET_WITH_RANGER, DONE);
                        me->GetInstanceScript()->SetData(STEP_SURVIVE_THE_AMBUSH, IN_PROGRESS);
                    }
                });
                break;
            }
            case ACTION_SURPRISED:
            {
                scheduler.Schedule(Milliseconds(5000), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_11);
                });
                break;
            }
            case ACTION_AMBUSH_PREVENTED:
            {
                me->GetMotionMaster()->MovePoint(0, hordeAmbusherSpawnPos[0]);

                delay = me->GetSplineDuration();
                scheduler.Schedule(Milliseconds(delay), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_12);
                    me->StopMoving();
                });

                scheduler.Schedule(Milliseconds(delay += 5 * IN_MILLISECONDS), [this](TaskContext context)
                {
                    Movement::MoveSplineInit init(me);

                    for (auto&& itr : elsiaBarricadePath)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    init.SetUncompressed();
                    init.Launch();

                    scheduler.Schedule(Milliseconds(me->GetSplineDuration() + 3 * IN_MILLISECONDS), [this](TaskContext context)
                    {
                        thirdStep = false;
                        Talk(TALK_SPECIAL_13);

                        if (me->GetInstanceScript())
                            me->GetInstanceScript()->SetData(STEP_DESTROY_THE_BARRICADES, IN_PROGRESS);

                        if (Creature* arcanital = me->SummonCreature(NPC_ARCANITAL_TULACHEK, arcanitalBarricadePoint, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, Milliseconds(5 * IN_MILLISECONDS)))
                            arcanital->AI()->Talk(TALK_SPECIAL_2);

                        // Wait
                        scheduler
                            .SetValidator([this] { return !thirdStep; })
                            .Schedule(Milliseconds(1000), [this](TaskContext context)
                        {
                            if (me->FindNearestCreature(NPC_ZEBTULA_STONESHIELD, 30.0f, true) || me->FindNearestCreature(NPC_ZANDALARI_BARRICADE_A, 30.0f, true) || me->FindNearestCreature(NPC_ZEBTULA_SPEARANGER, 30.0f, true))
                            {
                                context.Repeat(Milliseconds(1000));
                                return;
                            }

                            thirdStep = true;
                            Talk(TALK_SPECIAL_14);
                            me->StopMoving();

                            if (me->GetInstanceScript())
                                me->GetInstanceScript()->SetData(STEP_DESTROY_THE_BARRICADES, DONE);
                        });
                    });
                });
                break;
            }
            case ACTION_ANKI_DEFEAT:
            {
                Talk(TALK_SPECIAL_16);
                me->StopMoving();

                if (me->GetInstanceScript())
                    me->GetInstanceScript()->SetData(STEP_ANKI_AND_PAKUTESH, DONE);

                Movement::MoveSplineInit init(me);

                for (auto&& itr : elsiaArcanitalPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetUncompressed();
                init.Launch();

                events.ScheduleEvent(EVENT_MISC + 3, me->GetSplineDuration());
                break;
            }
            case ACTION_BEHIND_ANKI:
            {
                me->StopMoving();
                Movement::MoveSplineInit init(me);

                for (auto&& itr : elsiaKreshPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetUncompressed();
                init.Launch();
                events.ScheduleEvent(EVENT_MISC, me->GetSplineDuration());
                break;
            }
            case ACTION_MARAKAH_DEFEATED:
            {
                Talk(TALK_SPECIAL_17);
                events.ScheduleEvent(EVENT_MISC + 2, 5 * IN_MILLISECONDS);
                break;
            }
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_MISC: // Scheduler not schedule anymore...
                    if (Creature* arcanital = me->SummonCreature(NPC_ARCANITAL_TULACHEK, arcanitalkreshPoint, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, Milliseconds(5 * IN_MILLISECONDS)))
                        arcanital->AI()->Talk(TALK_SPECIAL_3);

                    events.ScheduleEvent(EVENT_MISC + 1, 4500);
                    break;
                case EVENT_MISC + 1:
                    Talk(TALK_SPECIAL_15);

                    me->SummonCreature(NPC_KRESH_THE_REAPER, krashSpawnPoint, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    break;
                case EVENT_MISC + 2:
                    Talk(TALK_SPECIAL_18);
                    me->SummonCreature(NPC_LORTHEMAR_THERON, lortemarSummPoint, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    break;
                case EVENT_MISC + 3:
                    if (Creature* arcanital = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_ARCANITAL_TULACHEK) : 0))
                    {
                        arcanital->SetVisible(true);
                        arcanital->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                    }
                    break;
                }
                break;
        }
    }
};

// Arcanital Tulachek 69475
struct npc_arcanital_tulachek : public customCreatureAI
{
    npc_arcanital_tulachek(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    float x, y;
    bool secondStep;

    void InitializeAI() OVERRIDE
    {
        Reset();

        if (me->GetDBTableGUIDLow())
            return;

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);
    }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (me->GetInstanceScript() && me->GetInstanceScript()->GetData(STEP_SURVIVE_THE_AMBUSH) == DONE)
            return;

        secondStep = false;

        Talk(TALK_INTRO);

        if (Creature* elsia = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_SCOUT_CAPTAIN_ELSIA) : 0))
            elsia->AI()->DoAction(ACTION_SURPRISED);

        scheduler
            .SetValidator([this] { return !secondStep; })
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            std::list<Creature*> beastsList;
            GetCreatureListWithEntryInGrid(beastsList, me, NPC_SHAOL_MARA_BEASTCALLER, 150.0f);
            GetCreatureListWithEntryInGrid(beastsList, me, NPC_SHAOL_MARA_RAPTOR, 150.0f);
            beastsList.remove_if([=](Creature* target) { return target->GetDBTableGUIDLow() || !target->IsAlive(); });

            if (!beastsList.empty())
            {
                context.Repeat(Milliseconds(1000));
                return;
            }

            secondStep = true;
            Talk(TALK_SPECIAL_1);
            me->GetMotionMaster()->MovePoint(0, tulachekLeave);

            if (me->GetInstanceScript())
                me->GetInstanceScript()->SetData(STEP_SURVIVE_THE_AMBUSH, DONE);

            if (Creature* elsia = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_SCOUT_CAPTAIN_ELSIA) : 0))
                elsia->AI()->DoAction(ACTION_AMBUSH_PREVENTED);
        });
    }

    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        if (pointId == 0)
            me->DespawnOrUnsummon();
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        Talk(TALK_SPECIAL_4);
        events.ScheduleEvent(EVENT_ARCANITAL_SPARK, urand(3500, 8 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_WAVES_OF_LIGHT, 11 * IN_MILLISECONDS);

        DoCast(me, SPELL_ARCANITAL_BARRIER, true);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        me->RemoveAllAreasTrigger();

        Talk(TALK_SPECIAL_5);

        if (Creature* elsia = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_SCOUT_CAPTAIN_ELSIA) : 0))
            elsia->AI()->DoAction(ACTION_MARAKAH_DEFEATED);

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_KILL_ARCANITAL, DONE);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_ARCANITALS_SPARK, urand(7 * IN_MILLISECONDS, 12 * IN_MILLISECONDS), EVENT_ARCANITAL_SPARK, eventId);
            ExecuteTargetEvent(SPELL_WAVES_OF_LIGHT, urand(15 * IN_MILLISECONDS, 20 * IN_MILLISECONDS), EVENT_WAVES_OF_LIGHT, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Kresh the Reaper 69474
struct npc_zebtula_kresh_the_reaper : public customCreatureAI
{
    npc_zebtula_kresh_the_reaper(Creature* creature) : customCreatureAI(creature) { }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
            me->GetMotionMaster()->MoveJump(*target, 20.0f, 15.0f);
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_BERSERKER_RUSH, urand(3500, 8 * IN_MILLISECONDS));
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (Creature* elsia = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_SCOUT_CAPTAIN_ELSIA) : 0))
            elsia->AI()->DoAction(ACTION_ANKI_DEFEAT);
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
            ExecuteTargetEvent(SPELL_BERSERKER_RUSH, urand(10500, 14 * IN_MILLISECONDS), EVENT_BERSERKER_RUSH, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Scout Captain Elsia 69949, 69565
class npc_teardown_scout_captain_elsia : public CreatureScript
{
    public:
        npc_teardown_scout_captain_elsia() : CreatureScript("npc_teardown_scout_captain_elsia") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I`m ready", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                creature->AI()->DoAction(ACTION_BREAK_THE_WALL);

            player->CLOSE_GOSSIP_MENU();

            return true;
        }

        struct npc_teardown_scout_captain_elsiaAI : public ScriptedAI
        {
            npc_teardown_scout_captain_elsiaAI(Creature* creature) : ScriptedAI(creature) 
            {
                if (!me->GetDBTableGUIDLow())
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
    
            TaskScheduler scheduler;
            uint32 delay;
    
            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_START_INTRO:
                    {
                        me->GetMotionMaster()->MovePoint(0, tearDownElsiaIntro);
                    
                        delay = me->GetSplineDuration();
                        scheduler
                            .Schedule(Milliseconds(delay), [this](TaskContext context)
                        {
                            Talk(TALK_INTRO);
                        });
                    
                        scheduler
                            .Schedule(Milliseconds(delay += 6 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_1);
                        });
                        break;
                    }
                    case ACTION_BREAK_THE_WALL:
                    {
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                        me->GetMotionMaster()->MovePoint(0, elsiaExplosivePoint);

                        if (me->GetInstanceScript())
                            me->GetInstanceScript()->SetData(STEP_SEND_EXPLOSIVE, DONE);

                        if (Creature* alchukla = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_WARGOD_ALCHUKLA + 1) : 0))
                            alchukla->AI()->DoAction(ACTION_START_INTRO);

                        scheduler
                            .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                        {
                            me->SummonCreature(NPC_HEAVY_EXPLOSIVES, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                            if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                                me->SetFacingToObject(target);

                            Talk(TALK_SPECIAL_2);
                        });
                        break;
                    }
                    case ACTION_ALCHUKLA_FIND:
                    {
                        scheduler
                            .Schedule(Milliseconds(4000), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_3);
                        });

                        Movement::MoveSplineInit init(me);

                        for (auto&& itr : elsiaAlchuklaPath)
                            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                        init.SetSmooth();
                        init.Launch();

                        scheduler
                            .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                        {
                            if (Creature* alchukla = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_WARGOD_ALCHUKLA) : 0))
                            {
                                alchukla->SetVisible(true);
                                alchukla->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                            }
                        });
                        break;
                    }
                }
            }
    
            void UpdateAI(uint32 diff) OVERRIDE
            {
                scheduler.Update(diff);
            }
        };
    
        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_teardown_scout_captain_elsiaAI(creature);
        }
};

// Heavy Explosives 69606
struct npc_teardown_heavy_explosives : public ScriptedAI
{
    npc_teardown_heavy_explosives(Creature* creature) : ScriptedAI(creature) 
    {
        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    }

    TaskScheduler scheduler;
    uint32 delay;

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_EXPLOSIVE, DONE);

        delay = 3500;
        scheduler
            .Schedule(Milliseconds(delay), [this](TaskContext context)
        {
            if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                target->CastSpell(target, SPELL_LAUNCH_WALL_SCENE, true);
        });

        scheduler
            .Schedule(Milliseconds(delay += 7 * IN_MILLISECONDS), [this](TaskContext context)
        {
            DoCast(me, SPELL_WALL_EXPLOSIVE, true);

            if (GameObject* wall = ObjectAccessor::GetGameObject(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(GO_TEAR_DOWN_DESTRUCT_WALL) : 0))
                wall->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);

            me->DespawnOrUnsummon();
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE 
    {
        scheduler.Update(diff);
    }
};

// Lorthemar Therone 70055
struct npc_teardown_lorthemar_therone : public ScriptedAI
{
    npc_teardown_lorthemar_therone(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint32 delay;

    void InitializeAI() OVERRIDE
    {
        if (!me->GetDBTableGUIDLow())
        {
            me->GetMotionMaster()->MovePoint(0, elsiaAlchuklaPath[6]);

            delay = me->GetSplineDuration();
            scheduler
                .Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_2);
            });

            scheduler
                .Schedule(Milliseconds(delay += 3500), [this](TaskContext context)
            {
                if (Creature* elsia = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TEARDOWN_SCOUT_CAPTAIN_ELSIA + 1) : 0))
                    elsia->AI()->Talk(TALK_SPECIAL_5);
            });

            scheduler
                .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_3);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_4);
            });

            scheduler
                .Schedule(Milliseconds(delay += 6750), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_5);
            });

            return;
        }

        scheduler
            .SetValidator([this] { return me->GetInstanceScript() && me->GetInstanceScript()->GetData(STEP_EXPLOSIVE) != DONE; })
            .Schedule(Milliseconds(30000), [this](TaskContext context)
        {
            Talk(TALK_INTRO);

            context.Repeat(Milliseconds(urand(20000, 25000)));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Wargod Alchukla 69755
struct npc_wargod_alchukla : public customCreatureAI
{
    npc_wargod_alchukla(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        Talk(TALK_SPECIAL_2);
        events.ScheduleEvent(EVENT_LIGHTNING_POLE, urand(3500, 5 * IN_MILLISECONDS));

        if (Creature* elsia = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TEARDOWN_SCOUT_CAPTAIN_ELSIA + 1) : 0))
            elsia->AI()->Talk(TALK_SPECIAL_4);
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            Talk(TALK_INTRO);

            scheduler
                .Schedule(Milliseconds(1000), [this](TaskContext context)
            {
                if (Unit* target = ObjectAccessor::GetUnit(*me, getAnyEnemyGUID()))
                    DoCast(target, SPELL_ALCHUKLA_LIGHTNING_BALL);

                context.Repeat(Milliseconds(3000));
            });
        }
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        me->RemoveAllAreasTrigger();

        Talk(TALK_SPECIAL_5);

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_KILL_WARGOD_ALCHUKLA, DONE);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_LIGHTNING_POLE_AT, 10 * IN_MILLISECONDS, EVENT_LIGHTNING_POLE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }

    private:
        uint64 getAnyEnemyGUID()
        {
            std::list<Creature*> unitList;
            GetCreatureListWithEntryInGrid(unitList, me, NPC_TEARDOWN_SUNREAVER_CAPTAIN, 100.0f);
            GetCreatureListWithEntryInGrid(unitList, me, NPC_TEARDOWN_SUNREAVER_PYRO, 100.0f);
            GetCreatureListWithEntryInGrid(unitList, me, NPC_TEARDOWN_SUNREAVER_SPELLBLADE, 100.0f);
            GetCreatureListWithEntryInGrid(unitList, me, NPC_TEARDOWN_SUNREAVER_ASSASSIN, 100.0f);
            GetCreatureListWithEntryInGrid(unitList, me, NPC_SILVER_COVENANT_ASSASSIN, 100.0f);
            GetCreatureListWithEntryInGrid(unitList, me, NPC_SILVER_COVENANT_SPELLBLADE, 100.0f);
            GetCreatureListWithEntryInGrid(unitList, me, NPC_SILVER_COVENANT_ARCHER, 100.0f);
            GetCreatureListWithEntryInGrid(unitList, me, NPC_TEARDOWN_SUNREAVER_ASSASSIN, 100.0f);

            if (!unitList.empty())
                return Skyfire::Containers::SelectRandomContainerElement(unitList)->GetGUID();

            return 0;
        }
};

// Zandalari Bloodguard 69294
struct npc_teardown_zandalari_bloodguard : public customCreatureAI
{
    npc_teardown_zandalari_bloodguard(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_BLOODY_STRIKE, urand(3500, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_RAGE_ZONE, urand(5 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_BLOODY_STRIKE, 6500, EVENT_BLOODY_STRIKE, eventId);
            ExecuteTargetEvent(SPELL_RAGE_ZONE, 12 * IN_MILLISECONDS, EVENT_RAGE_ZONE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Spirit Bound Sentry 69335
struct npc_teardown_spirit_void_sentry : public customCreatureAI
{
    npc_teardown_spirit_void_sentry(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SPIRIT_CLEAVE, urand(3500, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_VOID_SOUL, urand(5 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_BODY_OF_STONE, 12500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_SPIRIT_CLEAVE, 6500, EVENT_SPIRIT_CLEAVE, eventId);
            ExecuteTargetEvent(SPELL_VOID_SOUL, 12 * IN_MILLISECONDS, EVENT_VOID_SOUL, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_BODY_OF_STONE, 18 * IN_MILLISECONDS, EVENT_BODY_OF_STONE, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Soultwister 69296
struct npc_teardown_zandalari_soultwister : public customCreatureAI
{
    npc_teardown_zandalari_soultwister(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_LEGACY_OF_BLOOD, urand(3500, 12 * IN_MILLISECONDS));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_LEGACY_OF_BLOOD, 13500, EVENT_LEGACY_OF_BLOOD, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Haruspex 69297
struct npc_teardown_zandalari_haruspex : public customCreatureAI
{
    npc_teardown_zandalari_haruspex(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();

        me->m_Events.Schedule(1000, [this]()
        {
            if (Creature* trigger = me->FindNearestCreature(69451, 8.0f))
                DoCast(trigger, 138494);
        });
    }

    void JustReachedHome() OVERRIDE
    {
        me->m_Events.Schedule(1000, [this]()
        {
            if (Creature* trigger = me->FindNearestCreature(69451, 8.0f))
                DoCast(trigger, 138494);
        });
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_CURSE_OF_BABBLING_TONG, urand(3500, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_STARFALL, 7 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_STARFALL, 17 * IN_MILLISECONDS, EVENT_STARFALL, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_CURSE_OF_BABBLING_TONG, 13500, EVENT_CURSE_OF_BABBLING_TONG, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Sunreaver | Covenant Leader 70046, 70044, 69994, 70350, 70045, 70395, 70396, 70397, 70213
struct npc_teardown_covenant_leader : public barricadeBaseAI
{
    npc_teardown_covenant_leader(Creature* creature) : barricadeBaseAI(creature) { }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(75) && attacker->GetTypeId() != TypeID::TYPEID_PLAYER)
            damage = 0;
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// Slavemaster Shiaxu 69923
struct npc_slavemaster_shiaxu : public customCreatureAI
{
    npc_slavemaster_shiaxu(Creature* creature) : customCreatureAI(creature) { }

    bool hasTriggered;

    void Reset() OVERRIDE
    {
        events.Reset();
        hasTriggered = false;
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        Talk(TALK_INTRO);
        events.ScheduleEvent(EVENT_CAPTURE, urand(3500, 6 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_DEADLY_KICK, 7500);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        Talk(TALK_SPECIAL_2);

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_COVER_ME, DONE);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(30) && !hasTriggered)
        {
            hasTriggered = true;

            if (Creature* taoshi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI) : 0))
                taoshi->AI()->Talk(TALK_SPECIAL_4);
        }
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
            if (eventId == EVENT_DEADLY_KICK)
                Talk(TALK_SPECIAL_1);

            ExecuteTargetEvent(SPELL_CAPTURE, 7 * IN_MILLISECONDS, EVENT_CAPTURE, eventId, PRIORITY_CHANNELED);
            ExecuteTargetEvent(SPELL_DEADLY_KICK, 9500, EVENT_DEADLY_KICK, eventId, PRIORITY_CHANNELED);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Shanze Electrocutioner 69216
struct npc_shanze_electrocutioner : public customCreatureAI
{
    npc_shanze_electrocutioner(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();
        scheduler.CancelAll();

        scheduler
            .SetValidator([this] { return !me->GetVictim(); })
            .Schedule(Seconds(2), [this](TaskContext context)
        {
            DoCast(me, SPELL_LIGHTNING_CHARGE);
            context.Repeat(Seconds(6));
        });
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(1500, 2500));
        events.ScheduleEvent(EVENT_LIGHTNING_STRIKE, 7500);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        DoCast(me, SPELL_POWER_SURGE_SKY_AT, true);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_SHANZE_CHAIN_LIGHTNING, 3 * IN_MILLISECONDS, EVENT_CHAIN_LIGHTNING, eventId);
            ExecuteTargetEvent(SPELL_SHANZE_LIGHTNING_STRIKE, 10 * IN_MILLISECONDS, EVENT_LIGHTNING_STRIKE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Lightning Strike Dummy 69192
struct npc_in_sky_lightning_strike_dummy : public customCreatureAI
{
    npc_in_sky_lightning_strike_dummy(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();

        scheduler
            .SetValidator([this] { return !me->GetVictim(); })
            .Schedule(Milliseconds(500), [this](TaskContext context)
        {
            DoCast(me, SPELL_LIGHTNING_BLAST);
            context.Repeat(Seconds(1));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Shanze Slaver 67466
struct npc_shanze_slaver : public customCreatureAI
{
    npc_shanze_slaver(Creature* creature) : customCreatureAI(creature)
    {
        slaveGUID = 0;

        SetCombatMovement(false);
    }

    uint64 slaveGUID;

    void Reset() OVERRIDE
    {
        events.Reset();

        if (!me->GetCreature(*me, slaveGUID))
        {
            me->m_Events.Schedule(3000, [this]()
            {
                if (Creature* slave = me->SummonCreature(69140, me->GetPositionX(), me->GetPositionY() + 5.0f, me->GetPositionZ(), me->GetOrientation()))
                {
                    slaveGUID = slave->GetGUID();
                    me->CastSpell(slave, 136401);

                    slave->GetMotionMaster()->MoveFollow(me, 3.0f, me->GetFollowAngle());
                    slave->ClearUnitState(UNIT_STATE_CASTING);
                }
            });
        }
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (Creature* slave = me->GetCreature(*me, slaveGUID))
        {
            slave->CastSpell(slave, 136703);
            slave->setFaction(35);
            slave->DespawnOrUnsummon(30 * IN_MILLISECONDS);
            slaveGUID = 0;
        }
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_ELECTRIFIED_CHAINS, urand(4500, 12 * IN_MILLISECONDS));
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
            ExecuteTargetEvent(SPELL_ELECTRIFIED_CHAINS, urand(9500, 15 * IN_MILLISECONDS), EVENT_ELECTRIFIED_CHAINS, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Beastmaster Horaki 69559
struct npc_beastmaster_horaki : public customCreatureAI
{
    npc_beastmaster_horaki(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();
        me->RemoveAllAreasTrigger();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        Talk(TALK_SPECIAL_3);
        events.ScheduleEvent(EVENT_SHOT, urand(1500, 4 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_FROST_SHOT, 5500);
        events.ScheduleEvent(EVENT_HAIL_OF_ARROWS, 15 * IN_MILLISECONDS);
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        switch (actionId)
        {
            case ACTION_START_INTRO:
                me->SetVisible(true);
            
                Talk(TALK_INTRO);
                break;
            case ACTION_FLY_FALL:
                if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                    Talk(TALK_SPECIAL_1, target);

                scheduler
                    .Schedule(Seconds(4), [this](TaskContext context)
                {
                    if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                        Talk(TALK_SPECIAL_2, target);
                });

                scheduler
                    .Schedule(Milliseconds(6500), [this](TaskContext context)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PACIFIED);
                });
                break;
        }
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        me->RemoveAllAreasTrigger();

        if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
            Talk(TALK_SPECIAL_3, target);

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_CURB_BEASTMASTER, DONE);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_FROST_SHOT)
                Talk(TALK_SPECIAL_4);
            else if (eventId == EVENT_HAIL_OF_ARROWS)
                Talk(TALK_SPECIAL_5);

            ExecuteTargetEvent(SPELL_SHOT, urand(2 * IN_MILLISECONDS, 5 * IN_MILLISECONDS), EVENT_SHOT, eventId);
            ExecuteTargetEvent(SPELL_FROST_SHOT, 10500, EVENT_FROST_SHOT, eventId);
            ExecuteTargetEvent(SPELL_HAIL_OF_ARROWS, 15500, EVENT_HAIL_OF_ARROWS, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Lorthemar Throne 69481
struct npc_lorthemar_therone_to_skies : public customCreatureAI
{
    npc_lorthemar_therone_to_skies(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        if (me->GetDBTableGUIDLow())
            return;

        scheduler
            .Schedule(Seconds(2), [this](TaskContext context)
        {
            Talk(TALK_INTRO);
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            me->GetMotionMaster()->MovePoint(0, forgeAssaultEntrancePos);

            scheduler
                .Schedule(Seconds(1), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_1);
            });

            scheduler
                .Schedule(Seconds(5), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_2);
            });

            scheduler
                .Schedule(Seconds(7), [this](TaskContext context)
            {
                if (Creature* taoshi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI) : 0))
                {
                    taoshi->SetFacingTo(taoshi->GetAngle(me));
                    taoshi->AI()->Talk(me->GetEntry() == NPC_LADY_JAINA_IN_SKY ? TALK_SPECIAL_8 : TALK_SPECIAL_6);
                }
            });

            scheduler
                .Schedule(Milliseconds(9500), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_3);
            });

            if (me->GetEntry() == NPC_LADY_JAINA_IN_SKY)
            {
                scheduler
                    .Schedule(Seconds(11), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_4);
                });
            }

            scheduler
                .Schedule(Milliseconds(13500), [this](TaskContext context)
            {
                if (Creature* taoshi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI) : 0))
                    taoshi->AI()->Talk(me->GetEntry() == NPC_LADY_JAINA_IN_SKY ? TALK_SPECIAL_9 : TALK_SPECIAL_7);
            });

            scheduler
                .Schedule(Seconds(16), [this](TaskContext context)
            {
                Talk(me->GetEntry() == NPC_LADY_JAINA_IN_SKY ? TALK_SPECIAL_5 : TALK_SPECIAL_4);
            });

            if (me->GetEntry() == NPC_LADY_JAINA_IN_SKY)
            {
                scheduler
                    .Schedule(Seconds(19), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_6);
                });
            }
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Scout Captain Elsia 69745
struct npc_scout_captain_elsia_to_skies : public customCreatureAI
{
    npc_scout_captain_elsia_to_skies(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        if (me->GetDBTableGUIDLow())
            return;

        scheduler
            .Schedule(Seconds(4), [this](TaskContext context)
        {
            Talk(TALK_INTRO);
        });

        scheduler
            .Schedule(Seconds(9), [this](TaskContext context)
        {
            Talk(TALK_SPECIAL_1);

            if (Creature* thunderWing = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_THUNDERWING) : 0))
                thunderWing->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            Talk(TALK_SPECIAL_2);

            scheduler
                .Schedule(Milliseconds(5600), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_3);
            });

            scheduler
                .Schedule(Milliseconds(6200), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_4);

                if (Creature* thunderWing = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_THUNDERWING) : 0))
                    thunderWing->AI()->DoAction(ACTION_START_INTRO);
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Thunderwing 69509
struct npc_thunderwing_to_skies : public customCreatureAI
{
    npc_thunderwing_to_skies(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        me->SetSpeed(MOVE_RUN, 3.35f);
    }

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        clicker->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);
        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        clicker->CastSpell(clicker, SPELL_THUNDER_INNERVATION, true);
        clicker->AddAura(SPELL_RIDE_VEHICLE, clicker);

        if (Creature* elsia = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_SCOUT_CAPTAIN_ELSIA_IN_SKY) : 0))
            elsia->AI()->DoAction(ACTION_START_INTRO);
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        switch (actionId)
        {
            case ACTION_START_INTRO:
            {
                Movement::MoveSplineInit init(me);
            
                bool hasHorde = me->GetInstanceScript() && me->GetInstanceScript()->GetData64(FACTION_DATA);

                if (hasHorde)
                {
                    for (auto&& itr : toSkiesHordePath)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));
                }
                else
                {
                    for (auto&& itr : toSkiesAlliancePath)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));
                }
            
                init.SetFly();
                init.SetUncompressed();
                init.Launch();
            
                scheduler
                    .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                {
                    if (me->GetInstanceScript())
                        me->GetInstanceScript()->SetData(STEP_IN_SKY, DONE);

                    me->StopMoving();

                    Movement::MoveSplineInit newInit(me);
            
                    for (auto&& itr : cycleFlyPath)
                        newInit.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));
            
                    newInit.SetFly();
                    newInit.SetCyclic();
                    newInit.SetUncompressed();
                    newInit.Launch();

                    // 3 cycles of pterrorwings
                    scheduler
                        .Schedule(Milliseconds(4500), [this](TaskContext context)
                    {
                        for (uint8 i = 0; i < 2; i++)
                            me->SummonCreature(NPC_ZANDALARI_PTERRORWING, cycleFlyPath[0].GetPositionX() + frand(-7.5f, 7.5f), cycleFlyPath[0].GetPositionY() + frand(-5.0f, 5.0f), cycleFlyPath[0].GetPositionZ(), 0.0f, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    });

                    scheduler
                        .Schedule(Milliseconds(28000), [this](TaskContext context)
                    {
                        for (uint8 i = 0; i < 3; i++)
                            me->SummonCreature(NPC_ZANDALARI_PTERRORWING, cycleFlyPath[4].GetPositionX() + frand(-7.5f, 7.5f), cycleFlyPath[4].GetPositionY() + frand(-5.0f, 5.0f), cycleFlyPath[4].GetPositionZ(), 0.0f, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    });

                    scheduler
                        .Schedule(Milliseconds(50100), [this](TaskContext context)
                    {
                        for (uint8 i = 0; i < 3; i++)
                            me->SummonCreature(NPC_ZANDALARI_PTERRORWING, cycleFlyPath[7].GetPositionX() + frand(-7.5f, 7.5f), cycleFlyPath[7].GetPositionY() + frand(-5.0f, 5.0f), cycleFlyPath[7].GetPositionZ(), 0.0f, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    });
                });
                break;
            }
            case ACTION_FLY_FALL:
            {
                me->StopMoving();

                if (Creature* Horaki = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_BEASTMASTER_HORAKI) : 0))
                    Horaki->AI()->DoAction(ACTION_START_INTRO);

                Movement::MoveSplineInit init(me);

                for (auto&& itr : thunderwingBackPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetFly();
                init.SetUncompressed();
                init.Launch();

                scheduler
                    .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                {
                    if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                    {
                        target->RemoveAurasDueToSpell(SPELL_THUNDER_INNERVATION);
                        target->RemoveAurasDueToSpell(SPELL_RIDE_VEHICLE);
                        target->ExitVehicle();
                    }

                    if (Creature* Horaki = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_BEASTMASTER_HORAKI) : 0))
                        Horaki->AI()->DoAction(ACTION_FLY_FALL);

                    me->DespawnOrUnsummon();
                });
                break;
            }
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Zandalari Pterrorwing 69511
struct npc_zandalari_pterrorwing_to_skies : public customCreatureAI
{
    npc_zandalari_pterrorwing_to_skies(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        me->SetSpeed(MOVE_RUN, 3.35f);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            if (Creature* beastcaller = me->SummonCreature(NPC_ZANDALARI_BEASTCALLER, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                beastcaller->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);
        });

        scheduler
            .Schedule(Milliseconds(500), [this](TaskContext context)
        {
            if (Creature* thunderWing = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_THUNDERWING) : 0))
                me->GetMotionMaster()->MoveFollow(thunderWing, frand(5.0f, 8.0f), me->GetAngle(thunderWing));

            context.Repeat(Milliseconds(500));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Zandalari Beastcaller 69521
struct npc_zandlari_beastcaller_to_skies : public customCreatureAI
{
    npc_zandlari_beastcaller_to_skies(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    uint64 ownerGUID;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        ownerGUID = summoner->GetGUID();
        DoCast(me, SPELL_JUMP_OVERRIDE_TECH, true);

        scheduler
            .Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                DoCast(target, SPELL_THROW);

            context.Repeat(Milliseconds(2000));
        });
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (Creature* myPterrorwing = ObjectAccessor::GetCreature(*me, ownerGUID))
            myPterrorwing->DespawnOrUnsummon();

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_INTERCEPTION, IN_PROGRESS);
    }

    void UpdateAI(uint32 diff) OVERRIDE 
    {
        scheduler.Update(diff);
    } 
};

// Taoshi 69810
class npc_taoshi_to_skies : public CreatureScript
{
    public:
        npc_taoshi_to_skies() : CreatureScript("npc_taoshi_to_skies") { }
    
        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());
    
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I`m ready", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }
    
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();
    
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                creature->AI()->DoAction(ACTION_BREAK_THE_WALL);
    
            player->CLOSE_GOSSIP_MENU();
    
            return true;
        }
    
        struct npc_taoshi_to_skiesAI : public customCreatureAI
        {
            npc_taoshi_to_skiesAI(Creature* creature) : customCreatureAI(creature) { }
    
            TaskScheduler scheduler;
    
            void InitializeAI() OVERRIDE
            {
                if (me->GetPositionZ() < 45.0f)
                {
                    Talk(TALK_SPECIAL_3);

                    Movement::MoveSplineInit init(me);

                    for (auto&& itr : taoshiOpenDoorPath)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    init.SetFly();
                    init.SetUncompressed();
                    init.Launch();
                }
                else
                {
                    Talk(TALK_SPECIAL_2);
                    me->GetMotionMaster()->MoveJump(taoshiJump[0], 20.0f, 15.0f, EVENT_JUMP);
                }
            }
    
            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                switch (pointId)
                {
                    case EVENT_JUMP:
                        scheduler
                            .Schedule(Milliseconds(200), [this](TaskContext context)
                        {
                            me->GetMotionMaster()->MoveJump(taoshiJump[1], 20.0f, 15.0f, EVENT_JUMP + 1);
                        });
                        break;
                    case EVENT_JUMP + 1:
                        Talk(TALK_INTRO);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        break;
                }
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_BREAK_THE_WALL)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    me->GetMotionMaster()->MovePoint(0, taoshiExplosionPos);

                    scheduler
                        .Schedule(Milliseconds(me->GetSplineDuration() + 5 * IN_MILLISECONDS), [this](TaskContext context)
                    {
                        if (me->GetInstanceScript())
                        {
                            if (GameObject* cavein = ObjectAccessor::GetGameObject(*me, me->GetInstanceScript()->GetData64(GO_CAVEIN)))
                                cavein->UseDoorOrButton();

                            if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                                target->CastSpell(target, SPELL_SUMMON_TAOSHI, true);

                            me->GetInstanceScript()->SetData(STEP_IN_THE_MINE, DONE);
                            me->DespawnOrUnsummon();
                        }
                    });
                }
            }
    
            void UpdateAI(uint32 diff) OVERRIDE
            {
                scheduler.Update(diff);
            }
        };
    
        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_taoshi_to_skiesAI(creature);
        }
};

// Taoshi 69457
struct npc_taoshi_stormsea_landing : public customCreatureAI
{
    npc_taoshi_stormsea_landing(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    uint32 delay;
    uint32 vehicleAvailable;

    uint32 GetData(uint32 type) const OVERRIDE
    {
        if (type == TAOSHI_DATA)
            return vehicleAvailable;

        return 0;
    }

    void InitializeAI() OVERRIDE
    {
        vehicleAvailable = 0;

        delay = 0;
        scheduler
            .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
        {
            Talk(TALK_INTRO);
        });

        scheduler
            .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
        {
            Talk(TALK_SPECIAL_1);
        });

        scheduler
            .Schedule(Milliseconds(delay += 5200), [this](TaskContext context)
        {
            Talk(TALK_SPECIAL_2);

            vehicleAvailable = 1;
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        switch (actionId)
        {
            case ACTION_START_INTRO:
                scheduler
                    .Schedule(Seconds(3), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_3);
                });
            
                scheduler
                    .Schedule(Seconds(10), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_4);
                });
                break;
            case ACTION_FLY_ASSAULT:
                Talk(TALK_SPECIAL_5);

                delay = 0;
                scheduler
                    .Schedule(Milliseconds(delay += 5500), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_6);
                });
            
                scheduler
                    .Schedule(Milliseconds(delay += 4200), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_7);
                });
            
                scheduler
                    .Schedule(Milliseconds(delay += 4900), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_8);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 10000), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_9);
                });
                break;
            case ACTION_FLY_FALL:
                Talk(TALK_SPECIAL_10);
                break;
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Thunderwing 69458
struct npc_thunderwing_stormsea_landing : public customCreatureAI
{
    npc_thunderwing_stormsea_landing(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        me->SetSpeed(MOVE_RUN, 3.35f);
    }

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        if (Creature* taoshi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI_STORMSEA) : 0))
        {
            if (!taoshi->AI()->GetData(TAOSHI_DATA))
                return;

            clicker->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            taoshi->AI()->DoAction(ACTION_START_INTRO);

            if (Creature* shanbu = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_SHAN_BU_STORMSEA) : 0))
            {
                shanbu->SetVisible(true);
                shanbu->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
            }

            if (Creature* huseng = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_HUSENG_THE_GATEKEEPER) : 0))
            {
                huseng->SetVisible(true);
                huseng->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
            }

            int32 bp = 2;
            taoshi->CastCustomSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, &bp, 0, 0, true);
        }

        // init path
        Movement::MoveSplineInit init(me);

        for (auto&& itr : taoshiStormseaFlypath)
            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

        init.SetFly();
        init.SetUncompressed();
        init.Launch();

        scheduler
            .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
        {
            if (me->GetInstanceScript())
                me->GetInstanceScript()->SetData(STEP_BLENDING_IN, DONE);

            if (Creature* taosi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI_STORMSEA) : 0))
                taosi->AI()->DoAction(ACTION_FLY_ASSAULT);

            if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                target->AddAura(SPELL_SMOKE_BOMB_AVAILABLE, target);

            scheduler
                .Schedule(Milliseconds(3500), [this](TaskContext context)
            {
                me->StopMoving();
                Movement::MoveSplineInit newInit(me);

                for (auto&& itr : StormseaFlypath)
                    newInit.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                newInit.SetSmooth();
                newInit.SetUncompressed();
                newInit.SetFly();
                newInit.SetCyclic();
                newInit.Launch();

                uint32 delay = 0;
                scheduler
                    .Schedule(Milliseconds(delay += me->GetSplineDuration() / 2), [this](TaskContext context)
                {
                    if (Creature* shanbu = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_SHAN_BU_STORMSEA) : 0))
                        shanbu->AI()->Talk(TALK_INTRO);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
                {
                    if (Creature* huseng = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_HUSENG_THE_GATEKEEPER) : 0))
                    {
                        huseng->AI()->Talk(TALK_INTRO);
                        huseng->RemoveAurasDueToSpell(SPELL_HUSENG_ASLEEP);
                        huseng->RemoveAurasDueToSpell(SPELL_COSMETIC_SLEEP);

                        Movement::MoveSplineInit husengInit(huseng);

                        for (auto&& itr : husengIntroPath)
                            husengInit.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                        husengInit.SetSmooth();
                        husengInit.Launch();
                    }
                });
            });
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            me->StopMoving();

            // init path
            Movement::MoveSplineInit init(me);

            for (auto&& itr : StormseaShipFlypath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.SetFly();
            init.SetSmooth();
            init.Launch();

            scheduler
                .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
            {
                if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                    target->ExitVehicle();

                if (Creature* taoshi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI_STORMSEA) : 0))
                    taoshi->DespawnOrUnsummon();

                if (Creature* shipTaoshi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI_STORMSEA_SHIP) : 0))
                    shipTaoshi->AI()->DoAction(ACTION_START_INTRO);

                me->DespawnOrUnsummon();
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Taoshi 69544
struct npc_taoshi_stormsea_landing_ship : public customCreatureAI
{
    npc_taoshi_stormsea_landing_ship(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    bool hasAnnounced;
    bool hasPathEnd;
    uint32 pointId;
    uint32 delay;

    void Reset() OVERRIDE
    {
        hasAnnounced = false;
        hasPathEnd   = false;
        pointId = 0;
        delay   = 0;
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        switch (actionId)
        {
            case ACTION_START_INTRO:
                me->SetVisible(true);
                DoCast(me, SPELL_INFILT_STORMSEA_LANDING, true);
                Talk(TALK_INTRO);
            
                scheduler
                    .Schedule(Milliseconds(4500), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_1);
                    DoAction(ACTION_MOVE_TO);
                });
                break;
            case ACTION_AMBUSH_HIM:
                if (!hasAnnounced)
                {
                    hasAnnounced = true;
                    Talk(TALK_SPECIAL_2); // Ann
                }

                // Do smth
                if (Creature* wingrider = me->FindNearestCreature(NPC_VETERAN_WINGRIDER, 20.0f, true))
                {
                    wingrider->CastSpell(wingrider, SPELL_MARK_OF_TAOSHI, true);
                    wingrider->AI()->DoAction(ACTION_MARKED);
                }
                break;
            case ACTION_MOVE_TO:
            {
                me->StopMoving();

                if (hasPathEnd)
                {
                    if (me->GetInstanceScript())
                        me->GetInstanceScript()->SetData(STEP_SHADOWS_IN_THE_STORM, DONE);

                    // init path
                    Movement::MoveSplineInit newInit(me);

                    for (auto&& itr : TaoshiHoldPath)
                        newInit.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    newInit.Launch();

                    scheduler
                        .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                    {
                        Talk(TALK_SPECIAL_3);
                        Talk(TALK_SPECIAL_4); // Ann

                        if (Creature* captain = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_CAPTAIN_HALUKAL) : 0))
                            captain->AI()->DoAction(ACTION_START_INTRO);
                    });
                    break;
                }

                if (pointId > 3)
                {
                    hasPathEnd = true;

                    // init path
                    Movement::MoveSplineInit newInit(me);

                    for (auto&& itr : TaoshiShipParabolic)
                        newInit.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    newInit.Launch();

                    scheduler
                        .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                    {
                        DoAction(ACTION_AMBUSH_HIM);
                    });
                    break;
                }

                // init path
                Movement::MoveSplineInit init(me);

                init.MoveTo(TaoshiShipPath[pointId].GetPositionX(), TaoshiShipPath[pointId].GetPositionY(), TaoshiShipPath[pointId].GetPositionZ());
                pointId++;
                init.Launch();

                scheduler
                    .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                {
                    DoAction(ACTION_AMBUSH_HIM);
                });
                break;
            }
            case ACTION_LEAVE_SHIP:
            {
                Talk(TALK_SPECIAL_5);

                // init path
                Movement::MoveSplineInit init(me);

                for (auto&& itr : TaoshiLeaveShipPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.Launch();

                delay = 0;
                scheduler
                    .Schedule(Milliseconds(delay += me->GetSplineDuration()), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_6);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 4500), [this](TaskContext context)
                {
                    DoCast(me, SPELL_INFILT_STORMSEA_LANDING, true);
                    Talk(TALK_SPECIAL_7);

                    me->StopMoving();

                    // init path
                    Movement::MoveSplineInit init(me);

                    for (auto&& itr : TaoshiReachGatePath_1)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    init.Launch();

                    delay = 0;
                    scheduler
                        .Schedule(Milliseconds(delay += me->GetSplineDuration()), [this](TaskContext context)
                    {
                        Talk(TALK_SPECIAL_8);
                    });

                    scheduler
                        .Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
                    {
                        me->StopMoving();

                        // init path
                        Movement::MoveSplineInit init(me);

                        for (auto&& itr : TaoshiReachGatePath_2)
                            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                        init.Launch();

                        scheduler
                            .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                        {
                            if (me->GetInstanceScript())
                                me->GetInstanceScript()->SetData(STEP_KEEP_QUIET, DONE);

                            if (Creature* huseng = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_HUSENG_THE_GATEKEEPER) : 0))
                                huseng->AI()->DoAction(ACTION_START_INTRO);

                            me->RemoveAurasDueToSpell(SPELL_INFILT_STORMSEA_LANDING);
                        });
                    });

                });

                break;
            }
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Captain Halukal 69482
struct npc_captain_halukal : public customCreatureAI
{
    npc_captain_halukal(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        DoCast(me, SPELL_COSMETIC_SLEEP, true);
        events.Reset();
        me->RemoveAllAreasTrigger();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        me->RemoveAurasDueToSpell(SPELL_COSMETIC_SLEEP);
        events.ScheduleEvent(EVENT_WELL_FED, urand(1500, 4 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_WALLOP, 5500);
        events.ScheduleEvent(EVENT_KEG_TOSS, 10500);
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            me->RemoveAurasDueToSpell(SPELL_COSMETIC_SLEEP);

            uint32 delay = 0;
            scheduler
                .Schedule(Milliseconds(delay += 1000), [this](TaskContext context)
            {
                Talk(TALK_INTRO);

                if (Creature* shipTaoshi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI_STORMSEA_SHIP) : 0))
                    shipTaoshi->RemoveAurasDueToSpell(SPELL_INFILT_STORMSEA_LANDING);
            });

            scheduler
                .Schedule(Milliseconds(delay += 3500), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_1);

                if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                    me->RemoveChanneledCast(target->GetGUID());
            });
        }
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        me->RemoveAllAreasTrigger();
        Talk(TALK_SPECIAL_3);

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_WETWORK, DONE);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_WELL_FED, urand(15 * IN_MILLISECONDS, 25 * IN_MILLISECONDS), EVENT_WELL_FED, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_WALLOP, 13500, EVENT_WALLOP, eventId, PRIORITY_CHANNELED);
            ExecuteTargetEvent(SPELL_KEG_TOSS, 10 * IN_MILLISECONDS, EVENT_KEG_TOSS, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Drunken Wingrider 70566
struct npc_drunken_wingrider : public customCreatureAI
{
    npc_drunken_wingrider(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    bool hasTriggered;

    void Reset() OVERRIDE
    {
        me->SetReactState(REACT_PASSIVE);
        DoCast(me, SPELL_COSMETIC_SLEEP, true);
        events.Reset();
        hasTriggered = false;
        me->SetSpeed(MOVE_RUN, 0.35f);
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO && !hasTriggered)
        {
            if (Creature* captain = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_CAPTAIN_HALUKAL) : 0))
                captain->AI()->Talk(TALK_SPECIAL_2);

            hasTriggered = true;
            me->RemoveAurasDueToSpell(SPELL_COSMETIC_SLEEP);
            DoCast(me, SPELL_HANGOVER, true);

            scheduler
                .Schedule(Milliseconds(1500), [this](TaskContext context)
            {
                if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                    me->RemoveChanneledCast(target->GetGUID());
            });

        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// Veteran Wingrider 69552
struct npc_veteran_wingrider : public customCreatureAI
{
    npc_veteran_wingrider(Creature* creature) : customCreatureAI(creature) { }

    bool hasMarked;

    void InitializeAI() OVERRIDE
    {
        hasMarked = false;
    }

    void Reset() OVERRIDE
    {
        events.Reset();

        if (hasMarked)
            DoCast(me, SPELL_MARK_OF_TAOSHI, true);
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_MARKED)
            hasMarked = true;
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_BASH_SKULL, urand(2500, 5 * IN_MILLISECONDS));
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (!hasMarked)
            return;

        // safety if someone wanna kill this instead ambush...
        me->RemoveAurasDueToSpell(SPELL_MARK_OF_TAOSHI);

        if (Creature* taoshi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI_STORMSEA_SHIP) : 0))
            taoshi->AI()->DoAction(ACTION_MOVE_TO);
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
            ExecuteTargetEvent(SPELL_BASH_SKULL, urand(5500, 9500), EVENT_BASH_SKULL, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Golem 70511
struct npc_zandalari_golem : public customCreatureAI
{
    npc_zandalari_golem(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        DoCast(me, SPELL_BODY_OF_STONE, true);
        events.ScheduleEvent(EVENT_THUNDER_CLAP, urand(2500, 5 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_BODY_OF_STONE, urand(12 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
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
            ExecuteTargetEvent(SPELL_THUNDER_CLAP, urand(9500, 13 * IN_MILLISECONDS), EVENT_THUNDER_CLAP, eventId);
            ExecuteTargetEvent(SPELL_BODY_OF_STONE, urand(12 * IN_MILLISECONDS, 15 * IN_MILLISECONDS), EVENT_BODY_OF_STONE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Spirit-Twister 70121
struct npc_zandalari_spirit_twister : public customCreatureAI
{
    npc_zandalari_spirit_twister(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SPIRIT_BOLT, urand(1500, 3 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_SPIRIT_TWISTING, urand(10 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
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
            ExecuteTargetEvent(SPELL_SPIRIT_TWISTING, 10 * IN_MILLISECONDS, EVENT_SPIRIT_TWISTING, eventId);
            ExecuteTargetEvent(SPELL_SPIRIT_BOLT, urand(3500, 4 * IN_MILLISECONDS), EVENT_SPIRIT_BOLT, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Huseng Gatekeeper 69483
struct npc_huseng_gatekeeper : public customCreatureAI
{
    npc_huseng_gatekeeper(Creature* creature) : customCreatureAI(creature), summons(me) { }

    TaskScheduler scheduler;
    SummonList summons;
    bool hasTriggered;

    void Reset() OVERRIDE
    {
        hasTriggered = false;
        events.Reset();
        me->RemoveAllAreasTrigger();
        summons.DespawnAll();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        me->RemoveAurasDueToSpell(SPELL_HUSENG_ASLEEP);
        Talk(TALK_SPECIAL_2);
        events.ScheduleEvent(EVENT_EMPOWER_AXE, urand(9500, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_CEASE, 20 * IN_MILLISECONDS);

        if (Creature* taoshi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI_STORMSEA_SHIP) : 0))
            taoshi->AI()->DoAction(ACTION_HUSENG_COMBAT);
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summon->SetDisplayFromModel(1);

        summons.Summon(summon);

        if (Unit* target = ObjectAccessor::GetUnit(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
        {
            summon->SetInCombatWith(target);
            summon->PrepareChanneledCast(summon->GetOrientation());
            summon->CastSpell(summon, SPELL_CEASE_EFF, true);
        }

        scheduler
            .Schedule(Milliseconds(500), [this](TaskContext context)
        {
            DoCast(me, SPELL_DESIST);
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() - M_PI));
            Talk(TALK_SPECIAL_1);

            if (Creature* taoshi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_TAOSHI_STORMSEA_SHIP) : 0))
                taoshi->RemoveAurasDueToSpell(SPELL_INFILTRATING_EFF);
        }
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        me->RemoveAllAreasTrigger();
        summons.DespawnAll();
        Talk(TALK_SPECIAL_9);

        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_LIGHT_UP_THE_NIGHT, DONE);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(30) && !hasTriggered)
        {
            hasTriggered = true;
            Talk(TALK_SPECIAL_7);
            Talk(TALK_SPECIAL_8); // Ann
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_EMPOWER_AXE)
            {
                Talk(TALK_SPECIAL_3);
                Talk(TALK_SPECIAL_4); // Ann
            }
            else if (eventId == EVENT_CEASE)
            {
                Talk(TALK_SPECIAL_5);
                Talk(TALK_SPECIAL_6); // Ann
            }

            ExecuteTargetEvent(SPELL_EMPOWER_AXE, urand(15 * IN_MILLISECONDS, 25 * IN_MILLISECONDS), EVENT_EMPOWER_AXE, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_CEASE, 21 * IN_MILLISECONDS, EVENT_CEASE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Taoshi 70297
class npc_stormsea_landing_taoshi_queue : public CreatureScript
{
    public:
        npc_stormsea_landing_taoshi_queue() : CreatureScript("npc_stormsea_landing_taoshi_queue") { }
    
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();
    
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                lfg::LfgDungeonSet scenario = { LFG_DUNGEON_STORMSEA_LANDING };
                sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
            }
    
            player->CLOSE_GOSSIP_MENU();
    
            return true;
        }
    
        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());
    
            if (player->GetQuestStatus(QUEST_A_BOLT_IDEA_A) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_DECISIVE_ACTION_H) == QUEST_STATUS_INCOMPLETE)
                player->ADD_GOSSIP_ITEM_DB(player->GetDefaultGossipMenuForSource(creature), 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }
};

// Sentry Totem 69505
struct npc_stormsea_landing_sentry_totem : public ScriptedAI
{
    npc_stormsea_landing_sentry_totem(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 targetGUID;

    void InitializeAI() OVERRIDE
    {
        scheduler
            .Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            DoCast(me, SPELL_SENTRY, true);
        });
    }

    void SetGUID(uint64 guid, int32 /*type*/) OVERRIDE
    {
        targetGUID = guid;
    }

    uint64 GetGUID(int32 /*type*/) const OVERRIDE
    {
        return targetGUID;
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        DoCast(summon, SPELL_SENTRY_BEAM);
        summon->ClearUnitState(UNIT_STATE_CASTING);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Sentry Beam Bunny 70078
struct npc_stormsea_landing_sentry_beam_bunny : public ScriptedAI
{
    npc_stormsea_landing_sentry_beam_bunny(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 targetGUID;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (summoner->ToCreature())
            targetGUID = summoner->ToCreature()->AI()->GetGUID();

        scheduler
            .Schedule(Milliseconds(500), [this](TaskContext context)
        {
            if (Unit* target = ObjectAccessor::GetUnit(*me, targetGUID))
            {
                me->GetMotionMaster()->MoveFollow(target, 0.0f, me->GetAngle(target));

                if (me->GetExactDist2d(target) < 1.5f && !target->HasAura(SPELL_SPOTTED))
                {
                    me->AddAura(SPELL_SPOTTED, target);

                    // Call Help Here!
                    std::list<Creature*> guardianList;
                    GetCreatureListWithEntryInGrid(guardianList, me, NPC_ZANDALARI_GOLEM, 30.0f);
                    GetCreatureListWithEntryInGrid(guardianList, me, NPC_ZANDALARI_SPIRIT_TWISTER, 30.0f);
                    GetCreatureListWithEntryInGrid(guardianList, me, NPC_ZANDALARI_CONSCRIPT, 30.0f);

                    for (auto&& itr : guardianList)
                        itr->RemoveChanneledCast(target->GetGUID());
                }
            }
            context.Repeat(Milliseconds(500));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Zandalari Pterrorwing 69464
struct npc_stormsea_landing_zandalari_pterrorwing : public ScriptedAI
{
    npc_stormsea_landing_zandalari_pterrorwing(Creature* creature) : ScriptedAI(creature) { }

    void InitializeAI() OVERRIDE
    {
        // Init Delay
        me->m_Events.Schedule(1500, [this]()
        {
            if (Creature* passenger = me->SummonCreature(NPC_ZANDALARI_WINGRIDER, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                passenger->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);
        });
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

struct shanbuAssaultBaseAI : public customCreatureAI
{
    shanbuAssaultBaseAI(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    InstanceScript* instance;
    uint32 delay;

    void Reset() OVERRIDE
    {
        instance = me->GetInstanceScript();
        events.Reset();
        delay = 0;

        if (me->GetEntry() == NPC_SHANBU_TARAN_ZHU && instance && instance->GetData(STEP_DEFEAT_SHAN_BU) == DONE)
            DoCast(me, SPELL_BLOOD_DRIP, true);
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->GetMotionMaster()->MoveTargetedHome();
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(75))
            damage = 0;
    }

    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        switch (pointId)
        {
            case EVENT_JUMP:
                me->SetHomePosition(*me);
                Talk(TALK_SPECIAL_5);

                scheduler
                    .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_6);
                });
                break;
            case EVENT_JUMP + 1:
                DoCast(me, SPELL_REBITH, true);
                Talk(TALK_SPECIAL_14);

                delay = 0;
                scheduler
                    .Schedule(Milliseconds(delay += 2800), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_15);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_16);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 5500), [this](TaskContext context)
                {
                    if (Creature* jaina = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_JAINA + 5000) : 0))
                        jaina->AI()->Talk(TALK_SPECIAL_8);

                    if (Creature* lorthemar = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_LORTHEMAR + 5000) : 0))
                        lorthemar->AI()->Talk(TALK_SPECIAL_7);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 1500), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_17);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 2300), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_18);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3500), [this](TaskContext context)
                {
                    if (Creature* lorthemar = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_LORTHEMAR + 5000) : 0))
                        lorthemar->AI()->Talk(TALK_SPECIAL_8);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 1800), [this](TaskContext context)
                {
                    if (Creature* jaina = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_JAINA + 5000) : 0))
                        jaina->AI()->Talk(TALK_SPECIAL_9);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 1800), [this](TaskContext context)
                {
                    if (Creature* vereesa = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_VEREESA + 5000) : 0))
                        vereesa->AI()->Talk(TALK_SPECIAL_4);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 1200), [this](TaskContext context)
                {
                    if (Creature* jaina = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_JAINA + 5000) : 0))
                        jaina->AI()->Talk(TALK_SPECIAL_10);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
                {
                    if (Creature* jaina = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_JAINA + 5000) : 0))
                        jaina->AI()->Talk(TALK_SPECIAL_11);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3200), [this](TaskContext context)
                {
                    if (Creature* lorthemar = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_LORTHEMAR + 5000) : 0))
                        lorthemar->AI()->Talk(TALK_SPECIAL_9);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3500), [this](TaskContext context)
                {
                    if (Creature* lorthemar = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_LORTHEMAR + 5000) : 0))
                        lorthemar->AI()->Talk(TALK_SPECIAL_10);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 1500), [this](TaskContext context)
                {
                    if (Creature* jaina = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_JAINA + 5000) : 0))
                        jaina->AI()->Talk(TALK_SPECIAL_12);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
                {
                    if (Creature* jaina = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_JAINA + 5000) : 0))
                        jaina->AI()->Talk(TALK_SPECIAL_13);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 1200), [this](TaskContext context)
                {
                    if (Creature* lorthemar = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_LORTHEMAR + 5000) : 0))
                        lorthemar->AI()->Talk(TALK_SPECIAL_11);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 2500), [this](TaskContext context)
                {
                    if (Creature* taoshi = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TAOSHI) : 0))
                        taoshi->AI()->Talk(TALK_SPECIAL_2);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 1500), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_19);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 2500), [this](TaskContext context)
                {
                    if (Creature* taoshi = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TAOSHI) : 0))
                        taoshi->AI()->Talk(TALK_SPECIAL_3);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 2350), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_20);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 4500), [this](TaskContext context)
                {
                    if (Creature* taoshi = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TAOSHI) : 0))
                        taoshi->AI()->Talk(TALK_SPECIAL_4);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
                {
                    if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                        me->SetFacingToObject(target);

                    if (Creature* taoshi = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TAOSHI) : 0))
                        taoshi->AI()->Talk(TALK_SPECIAL_5);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
                {
                    if (Creature* taoshi = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TAOSHI) : 0))
                        taoshi->AI()->Talk(TALK_SPECIAL_6);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3500), [this](TaskContext context)
                {
                    if (Creature* taoshi = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TAOSHI) : 0))
                        taoshi->AI()->Talk(TALK_SPECIAL_7);
                });
                break;
        }
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        switch (actionId)
        {
            case ACTION_START_INTRO:
                me->SetVisible(true);

                if (me->GetEntry() == NPC_SHAN_BU)
                {
                    Talk(TALK_INTRO);

                    delay = 0;
                    scheduler
                        .Schedule(Milliseconds(delay += 3500), [this](TaskContext context)
                    {
                        if (Creature* taranZhu = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TARAN_ZHU) : 0))
                            taranZhu->AI()->Talk(TALK_INTRO);

                        // Summon Zandalars
                        std::list<Creature*> zandalars;
                        GetCreatureListWithEntryInGrid(zandalars, me, NPC_ZANDALARI_STONE_SHIELD_3, 70.0f);

                        if (zandalars.size() > 8)
                            Skyfire::Containers::RandomResizeList(zandalars, 8);

                        for (auto&& itr : zandalars)
                            itr->AI()->DoAction(ACTION_START_ASSAULT);
                    });

                    scheduler
                        .Schedule(Milliseconds(delay += 8000), [this](TaskContext context)
                    {
                        if (Creature* lorthemar = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_LORTHEMAR + 5000) : 0))
                            lorthemar->AI()->Talk(TALK_SPECIAL_4);

                        // Close Gates
                        if (GameObject* moguDoor = ObjectAccessor::GetGameObject(*me, instance ? instance->GetData64(GO_MOGU_GATES_CENTER) : 0))
                            instance->HandleGameObject(moguDoor->GetGUID(), false, NULL);

                        // Leave to Nalak
                        Movement::MoveSplineInit init(me);
                        init.MoveTo(shanbuLeavePos.GetPositionX(), shanbuLeavePos.GetPositionY(), shanbuLeavePos.GetPositionZ());

                        init.Launch();

                        scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                        {
                            me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() - M_PI));

                            DoCast(me, SPELL_RESSURECT_NALAK);
                        });
                    });

                    scheduler
                        .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
                    {
                        if (Creature* jaina = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_JAINA + 5000) : 0))
                            jaina->AI()->Talk(TALK_SPECIAL_4);
                    });
                }
                else
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                break;
            case ACTION_START_ASSAULT:
            {
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                Talk(TALK_SPECIAL_1);

                bool hasHorde = me->GetInstanceScript() && me->GetInstanceScript()->GetData64(FACTION_DATA);

                if (Creature* factionLeader = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(hasHorde ? NPC_SHANBU_LORTHEMAR : NPC_SHANBU_JAINA) : 0))
                    factionLeader->AI()->DoAction(ACTION_BARRICADE_ASSAULT);
                break;
            }
            case ACTION_BARRICADE_ASSAULT:
            {
                delay = 0;

                scheduler
                    .Schedule(Milliseconds(delay += 3500), [this](TaskContext context)
                {
                    Talk(TALK_INTRO);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 2500), [this](TaskContext context)
                {
                    bool hasHorde = instance && instance->GetData64(FACTION_DATA);

                    if (Creature* scoutLeader = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(hasHorde ? NPC_SHANBU_ELSIA : NPC_SHANBU_VEREESA) : 0))
                        scoutLeader->AI()->Talk(TALK_SPECIAL_2);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_2);

                    if (instance)
                    {
                        // Invisible Wall (only horde)
                        instance->HandleGameObject(instance->GetData64(GO_SPIRIT_WALL + 1), true, NULL);

                        instance->HandleGameObject(instance->GetData64(instance->GetData64(FACTION_DATA) ? GO_MOGU_GATES_HORDE : GO_MOGU_GATES_STORMSEA), true, NULL);
                    }

                    DoAction(ACTION_MOVE_TO_BLODY_CROSS);

                    bool hasHorde = instance && instance->GetData64(FACTION_DATA);

                    if (Creature* scoutLeader = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(hasHorde ? NPC_SHANBU_ELSIA : NPC_SHANBU_VEREESA) : 0))
                        scoutLeader->AI()->DoAction(ACTION_MOVE_TO_BLODY_CROSS);

                });

                scheduler
                    .Schedule(Milliseconds(delay += 3500), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_3);
                });
                break;
            }
            case ACTION_MOVE_TO_BLODY_CROSS:
            {
                // Movement
                bool hasHorde = me->GetInstanceScript() && me->GetInstanceScript()->GetData64(FACTION_DATA);

                if (hasHorde)
                {
                    Movement::MoveSplineInit init(me);

                    for (auto&& itr : HordeShanbuPath)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    init.Launch();

                    scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                    {
                        me->DespawnOrUnsummon();
                    });
                }
                else
                {
                    Movement::MoveSplineInit init(me);

                    for (auto&& itr : AllianceShanbuPath)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    init.Launch();

                    scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                    {
                        me->DespawnOrUnsummon();
                    });
                }
                break;
            }
            case ACTION_KEEP_TO_SHANBU:
                delay = 0;
                scheduler
                    .Schedule(Milliseconds(delay += 4000), [this](TaskContext context)
                {
                    if (Creature* scoutLeader = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_VEREESA + 5000) : 0))
                        scoutLeader->AI()->Talk(TALK_SPECIAL_3);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 4500), [this](TaskContext context)
                {
                    if (Creature* jaina = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_JAINA + 5000) : 0))
                        jaina->AI()->Talk(TALK_SPECIAL_5);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 6000), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_2);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 5500), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_3);

                    // Open Gates
                    if (GameObject* moguDoor = ObjectAccessor::GetGameObject(*me, instance ? instance->GetData64(GO_MOGU_GATES_CENTER) : 0))
                        instance->HandleGameObject(moguDoor->GetGUID(), true, NULL);

                    me->GetMotionMaster()->MovePoint(0, taranZhuFollowPos);

                    delay = 0;
                    scheduler
                        .Schedule(Milliseconds(delay += me->GetSplineDuration() + 2000), [this](TaskContext context)
                    {
                        Talk(TALK_SPECIAL_4);
                    });

                    scheduler
                        .Schedule(Milliseconds(delay += 4900), [this](TaskContext context)
                    {
                        if (Creature* shanbu = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHAN_BU) : 0))
                            shanbu->AI()->Talk(TALK_SPECIAL_1);
                    });

                    scheduler
                        .Schedule(Milliseconds(delay += 4500), [this](TaskContext context)
                    {
                        if (Creature* shanbu = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHAN_BU) : 0))
                            shanbu->AI()->Talk(TALK_SPECIAL_2);

                        me->GetMotionMaster()->MoveJump(taranZhuJumpPos, 20.0f, 15.0f, EVENT_JUMP);

                        // Open Invisible Wall
                        if (instance)
                            instance->HandleGameObject(instance->GetData64(GO_SPIRIT_WALL), true, NULL);
                    });
                });
                break;
            case ACTION_ATTACK_SHANBU:
                Talk(TALK_SPECIAL_3);
                me->InterruptNonMeleeSpells(true, SPELL_RESSURECT_NALAK);

                if (Creature* nalak = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_NALAK) : 0))
                    nalak->AI()->DoAction(ACTION_START_INTRO);

                delay = 0;
                scheduler
                    .Schedule(Milliseconds(delay += 3500), [this](TaskContext context)
                {
                    if (Creature* taranZhu = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TARAN_ZHU) : 0))
                        taranZhu->AI()->Talk(TALK_SPECIAL_7);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
                {
                    me->RemoveAurasDueToSpell(SPELL_LIGHTNING_BARRIER);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                    Talk(TALK_SPECIAL_4);

                    // Attack Player
                    if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                        AttackStart(target);
                });
                break;
            case ACTION_SHANBU_DEFEATED:
                Talk(TALK_SPECIAL_10);

                if (Creature* taoshi = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TAOSHI) : 0))
                    taoshi->AI()->DoAction(ACTION_TARAN_BLEEDED);
                break;
            case ACTION_TARAN_BLEEDED:
            {
                Movement::MoveSplineInit init(me);

                for (auto&& itr : TaoshiToTaranPath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.Launch();

                delay = 0;
                scheduler
                    .Schedule(Milliseconds(delay += me->GetSplineDuration()), [this](TaskContext context)
                {
                    Talk(TALK_INTRO);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
                {
                    if (Creature* taranZhu = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TARAN_ZHU) : 0))
                        taranZhu->AI()->Talk(TALK_SPECIAL_11);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 2500), [this](TaskContext context)
                {
                    if (Creature* taranZhu = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TARAN_ZHU) : 0))
                        taranZhu->AI()->Talk(TALK_SPECIAL_12);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3500), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_1);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3200), [this](TaskContext context)
                {
                    if (Creature* taranZhu = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TARAN_ZHU) : 0))
                        taranZhu->AI()->DoAction(ACTION_BREAK_THIS_CONFLICT);
                });
                break;
            }
            case ACTION_BREAK_THIS_CONFLICT:
                Talk(TALK_SPECIAL_13);

                Movement::MoveSplineInit init(me);

                for (auto&& itr : TaranHomePath)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.Launch();

                delay = 0;
                scheduler
                    .Schedule(Milliseconds(delay += me->GetSplineDuration()), [this](TaskContext context)
                {
                    if (Creature* jaina = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_JAINA + 5000) : 0))
                        jaina->AI()->Talk(TALK_SPECIAL_6);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 2500), [this](TaskContext context)
                {
                    if (Creature* lorthemar = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_LORTHEMAR + 5000) : 0))
                        lorthemar->AI()->Talk(TALK_SPECIAL_5);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 4000), [this](TaskContext context)
                {
                    if (Creature* jaina = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_JAINA + 5000) : 0))
                        jaina->AI()->Talk(TALK_SPECIAL_7);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
                {
                    if (Creature* lorthemar = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_LORTHEMAR + 5000) : 0))
                        lorthemar->AI()->Talk(TALK_SPECIAL_6);
                });

                scheduler
                    .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
                {
                    me->GetMotionMaster()->MoveJump(taranZhuBreakPos, 20.0f, 15.0f, EVENT_JUMP + 1);

                    if (Creature* taoshi = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TAOSHI) : 0))
                    {
                        Movement::MoveSplineInit newInit(taoshi);

                        for (auto&& itr : TaranHomePath)
                            newInit.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                        newInit.Launch();
                    }
                });
                break;
        }
    }

    void AttackStart(Unit* target) OVERRIDE 
    {
        if (!target || target->GetTypeId() == TypeID::TYPEID_PLAYER || target->GetEntry() == NPC_SUNREAVER_MAGUS)
            return;

        me->Attack(target, true);
        me->GetMotionMaster()->MoveChase(target, 0.0f, me->GetAngle(target));
    }
};

struct npc_shanbu_assault_base : public shanbuAssaultBaseAI
{
    npc_shanbu_assault_base(Creature* creature) : shanbuAssaultBaseAI(creature) { }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// Elsia, Vereesa 69620, 69615
class npc_shanbu_fall_elsia : public CreatureScript
{
    public:
        npc_shanbu_fall_elsia() : CreatureScript("npc_shanbu_fall_elsia") { }
    
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();
    
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                creature->AI()->DoAction(ACTION_START_ASSAULT);
    
            player->CLOSE_GOSSIP_MENU();
    
            return true;
        }
    
        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());
    
            if (!creature->GetDBTableGUIDLow())
                player->ADD_GOSSIP_ITEM_DB(player->GetDefaultGossipMenuForSource(creature), 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }
    
        struct npc_shanbu_fall_elsiaAI : public shanbuAssaultBaseAI
        {
            npc_shanbu_fall_elsiaAI(Creature* creature) : shanbuAssaultBaseAI(creature) { }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                scheduler
                    .Schedule(Milliseconds(2000), [this](TaskContext context)
                {
                    Talk(TALK_INTRO);
                });
            }

            void EnterEvadeMode() OVERRIDE
            {
                ScriptedAI::EnterEvadeMode();
                me->GetMotionMaster()->MoveTargetedHome();
            }
    
            void UpdateAI(uint32 diff) OVERRIDE
            {
                scheduler.Update(diff);
            }
        };
    
        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_shanbu_fall_elsiaAI(creature);
        }
};

// Palace Gatekeeper 69510
struct npc_shanbu_palace_gatekeeper : public customCreatureAI
{
    npc_shanbu_palace_gatekeeper(Creature* creature) : customCreatureAI(creature) { }

    void InitializeAI() OVERRIDE
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
        DoCast(me, SPELL_FREEZE_STATE, true);
        Reset();
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_CRUSHING_STOMP, urand(4500, 14500));
        events.ScheduleEvent(EVENT_BOULDER_THROW, urand(15, 25));
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            me->RemoveAurasDueToSpell(SPELL_FREEZE_STATE);

            // Attack Player
            if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                AttackStart(target);
        }
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_KILL_THE_GUARDS, IN_PROGRESS);
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
            ExecuteTargetEvent(urand(0, 1) ? SPELL_CRUSHING_STOMP : SPELL_THUNDERING_SLAM, urand(8500, 17 * IN_MILLISECONDS), EVENT_CRUSHING_STOMP, eventId, PRIORITY_CHANNELED);
            ExecuteTargetEvent(SPELL_BOULDER_THROW, 11 * IN_MILLISECONDS, EVENT_BOULDER_THROW, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Shanze Electrocutioner 69525
struct npc_shanbu_shanze_electrocutioner : public customCreatureAI
{
    npc_shanbu_shanze_electrocutioner(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(1500, 2500));
        events.ScheduleEvent(EVENT_LIGHTNING_STRIKE, 7500);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_TO_LEI_SHEN_PALACE, IN_PROGRESS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_SHANZE_CHAIN_LIGHTNING, 3 * IN_MILLISECONDS, EVENT_CHAIN_LIGHTNING, eventId);
            ExecuteTargetEvent(SPELL_SHANZE_LIGHTNING_STRIKE, 10 * IN_MILLISECONDS, EVENT_LIGHTNING_STRIKE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Nalak 69562
struct npc_shanbu_nalak : public customCreatureAI
{
    npc_shanbu_nalak(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            me->RemoveAurasDueToSpell(SPELL_STONE);
            DoCast(me, SPELL_COSMETIC_SHADOW_NOVA, true);

            me->GetMotionMaster()->MovePoint(0, nalakLeavePos);

            scheduler
                .Schedule(Milliseconds(me->GetSplineDuration() / 2), [this](TaskContext context)
            {
                Talk(TALK_INTRO);
            });

            scheduler
                .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                    DoCast(target, SPELL_ELECTRICAL_STORM);

                context.Repeat(Seconds(5));
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Quilen Guardian 69506
struct npc_shanbu_quilen_guardian : public customCreatureAI
{
    npc_shanbu_quilen_guardian(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_TO_LEI_SHEN_PALACE, IN_PROGRESS);
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        DoCast(who, SPELL_FIXATED_LEAP);
        events.ScheduleEvent(EVENT_STONE_CLAW, urand(1500, 2500));
        events.ScheduleEvent(EVENT_VISIOUS_REND, 7500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_STONE_CLAW, urand(3 * IN_MILLISECONDS, 8 * IN_MILLISECONDS), EVENT_STONE_CLAW, eventId);
            ExecuteTargetEvent(SPELL_VISIOUS_REND, 8500, EVENT_VISIOUS_REND, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Shan Bu 69530
struct npc_shan_bu : public shanbuAssaultBaseAI
{
    npc_shan_bu(Creature* creature) : shanbuAssaultBaseAI(creature) { }

    bool hasTriggered;

    void Reset() OVERRIDE
    {
        instance = me->GetInstanceScript();
        events.Reset();
        hasTriggered = false;
    }

    void AttackStart(Unit* target) OVERRIDE
    {
        if (target)
        {
            me->Attack(target, true);
            me->GetMotionMaster()->MoveChase(target, 0.0f, me->GetAngle(target));
        }
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        Talk(TALK_SPECIAL_7);

        if (instance)
        {
            instance->SetData(STEP_DEFEAT_SHAN_BU, DONE);

            if (Creature* nalak = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_NALAK)))
                nalak->DespawnOrUnsummon();

            if (Player* target = ObjectAccessor::GetPlayer(*me, instance->GetData64(PLAYER_DATA)))
                target->KilledMonsterCredit(NPC_SHANBU_CREDIT);
        }
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SHADOW_CRASH, urand(1500, 2500));
        events.ScheduleEvent(EVENT_DRAW_SHADOWS, 7500);

        if (Creature* taranZhu = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TARAN_ZHU) : 0))
            taranZhu->AI()->Talk(TALK_SPECIAL_8);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(25) && !hasTriggered)
        {
            hasTriggered = true;

            Talk(TALK_SPECIAL_6);

            scheduler
                .Schedule(Milliseconds(1500), [this](TaskContext context)
            {
                if (Creature* taranZhu = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHANBU_TARAN_ZHU) : 0))
                {
                    taranZhu->AI()->Talk(TALK_SPECIAL_9);
                    taranZhu->CastSpell(taranZhu, SPELL_BLOOD_DRIP, true);
                }
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_DRAW_SHADOWS)
                Talk(TALK_SPECIAL_5);

            ExecuteTargetEvent(SPELL_SHADOW_CRASH, urand(3 * IN_MILLISECONDS, 8 * IN_MILLISECONDS), EVENT_SHADOW_CRASH, eventId);
            ExecuteTargetEvent(SPELL_DRAW_SHADOWS, 12 * IN_MILLISECONDS, EVENT_DRAW_SHADOWS, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Wrathion at Thunder forge 70100
class npc_wrathion_thunder_forge : public CreatureScript
{
    public:
        npc_wrathion_thunder_forge() : CreatureScript("npc_wrathion_thunder_forge") { }
    
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    creature->AI()->DoAction(ACTION_START_INTRO);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    creature->AI()->DoAction(ACTION_LAUNCH_FORGE);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3:
                {
                    lfg::LfgDungeonSet scenario = { LFG_THUNDER_FORGE };
                    sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
                    player->KilledMonsterCredit(NPC_UNLOCK_FORGE_CREDIT);
                    break;
                }
            }
    
            player->CLOSE_GOSSIP_MENU();
    
            return true;
        }
    
        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (creature->GetInstanceScript())
            {
                if (creature->GetInstanceScript()->GetData(STEP_IN_FORGE) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I`m ready!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                else
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I`m ready!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            }
            else if (player->GetQuestStatus(QUEST_THE_THUNDER_FORGE) == QUEST_STATUS_INCOMPLETE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I`m ready!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
        {
            if (quest->GetQuestId() == QUEST_SPIRIT_OF_THE_STORM_LORD)
                player->KilledMonsterCredit(NPC_UNLOCK_COURTYARD_CREDIT);

            return false;
        }
    
        struct npc_wrathion_thunder_forgeAI : public ScriptedAI
        {
            npc_wrathion_thunder_forgeAI(Creature* creature) : ScriptedAI(creature) { }

            InstanceScript* instance;
            TaskScheduler scheduler;
            uint32 delay;

            void InitializeAI() OVERRIDE
            {
                delay = 0;
                instance = me->GetInstanceScript();
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                if (me->GetMapId() == 1064)
                {
                    me->SetDisplayId(497); // Dragon Form
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                }
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_START_INTRO:
                    {
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        Talk(TALK_INTRO);
                    
                        if (instance)
                            instance->SetData(STEP_TALK_WITH_WRATHION, DONE);
                    
                        Movement::MoveSplineInit init(me);
                    
                        for (auto&& itr : WrathionCruciblePath)
                            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));
                    
                        init.SetWalk(true);
                        init.Launch();
                    
                        scheduler
                            .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_1);

                            if (GameObject* moguCrucible = ObjectAccessor::GetGameObject(*me, instance ? instance->GetData64(GO_MOGU_CRUCIBLE) : 0))
                                moguCrucible->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        });
                    
                        break;
                    }
                    case ACTION_FORGE_INFLUSION:
                        delay = 0;

                        scheduler
                            .Schedule(Milliseconds(delay += 1500), [this](TaskContext context)
                        {
                            // Summon Shado-Pan and jump to area
                            me->SummonCreature(NPC_SHADO_PAN_DEFENDER, ShadoPanForgeAssault[0], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                            for (uint8 i = 1; i < 3; i++)
                                me->SummonCreature(NPC_SHADO_PAN_WARRIOR, ShadoPanForgeAssault[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_2);
                        });
                        break;
                    case ACTION_FORGE_FIRST_WAVE_DONE:
                        delay = 0;
                        Talk(TALK_SPECIAL_3);
                        scheduler
                            .Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_3);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 4500), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_4);

                            // Progress bar and waves
                            DoCast(me, SPELL_THUNDER_FORGE_CHARGING, true);

                            if (Creature* thunderForge = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_THUNDER_FORGE) : 0))
                                thunderForge->AI()->DoAction(ACTION_START_INTRO);
                        });
                        break;
                    case ACTION_MOVE_INNER_FORGE:
                    {
                        if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                            target->RemoveAurasDueToSpell(SPELL_THUNDER_FORGE_CHARGING);

                        Talk(TALK_SPECIAL_5);
                        me->StopMoving();

                        scheduler
                            .Schedule(Milliseconds(4500), [this](TaskContext context)
                        {
                            if (Creature* shadopanDefender = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHADO_PAN_DEFENDER) : 0))
                                shadopanDefender->AI()->Talk(TALK_SPECIAL_2);
                        });

                        Movement::MoveSplineInit init(me);

                        for (auto&& itr : InnerForgePath)
                            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                        init.SetWalk(true);
                        init.SetUncompressed();
                        init.SetSmooth();
                        init.Launch();

                        scheduler
                            .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                        {
                            if (instance)
                                instance->SetData(STEP_IN_FORGE, DONE);
                        });
                        break;
                    }
                    case ACTION_INNER_FORGE:
                    {
                        delay = 0;
                        Talk(TALK_SPECIAL_6);

                        scheduler
                            .Schedule(Milliseconds(delay += 10 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_7);

                            // Summon Celestials
                            me->SummonCreature(NPC_CELESTIAL_DEFENDER, celesialsSpawnPos[1], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            me->SummonCreature(NPC_CELESTIAL_BLACKSMITH, celesialsSpawnPos[0], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 9400), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_8);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 12 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_9);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 13 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        });
                        break;
                    }
                    case ACTION_LAUNCH_FORGE:
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        Talk(TALK_SPECIAL_10);
                        DoCast(me, SPELL_THUNDER_FORGE_SMITHING);

                        scheduler
                            .Schedule(Milliseconds(6 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_11);

                            if (Creature* anvil = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_THUNDER_FORGE_2) : 0))
                                anvil->AI()->DoAction(ACTION_START_INTRO);
                        });

                        scheduler
                            .Schedule(Milliseconds(16 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_11);
                        });
                        break;
                    case ACTION_SHA_DEFEATED:
                        if (Player* target = ObjectAccessor::GetPlayer(*me, instance ? instance->GetData64(PLAYER_DATA) : 0))
                            target->KilledMonsterCredit(NPC_COMPLETE_SCENARIO_CREDIT);

                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                        Talk(TALK_SPECIAL_16);

                        scheduler
                            .Schedule(Milliseconds(7500), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_17);
                        });
                        break;
                    case ACTION_ALMAGMATION_INCOMING:
                        Talk(TALK_SPECIAL_13);

                        delay = 0;
                        scheduler
                            .Schedule(Milliseconds(delay += 6200), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_14);

                            if (Player* target = ObjectAccessor::GetPlayer(*me, instance ? instance->GetData64(PLAYER_DATA) : 0))
                                target->RemoveAurasDueToSpell(SPELL_THUNDER_FORGE_SMITHING);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 8000), [this](TaskContext context)
                        {
                            Talk(TALK_SPECIAL_15);

                            // Summon Almagmation
                            if (Creature* anvil = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_THUNDER_FORGE_2) : 0))
                                anvil->CastSpell(anvil, SPELL_SHA_COSMETIC_SPAWN, true);
                        });
                        break;
                }
            }
    
            void UpdateAI(uint32 diff) OVERRIDE 
            {
                scheduler.Update(diff);
            }
        };
    
        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_wrathion_thunder_forgeAI(creature);
        }
};

// Shado-Pan Defender, Shado-Pan Warrior 70099, 70106
struct npc_thunder_forge_shado_pan_warrior : public ScriptedAI
{
    npc_thunder_forge_shado_pan_warrior(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    bool hasEvade;
    float x, y;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        hasEvade = false;
        x = 0.0f; y = 0.0f;
        GetPositionWithDistInOrientation(me, 28.0f, me->GetOrientation(), x, y);
        me->GetMotionMaster()->MoveJump(x, y, 65.98f, 20.0f, 15.0f, EVENT_JUMP);
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        if (me->GetEntry() == NPC_SHADO_PAN_DEFENDER)
            events.ScheduleEvent(EVENT_HEALING_ORB, 10 * IN_MILLISECONDS);
    }

    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        if (pointId == EVENT_JUMP)
        {
            me->SetHomePosition(*me);

            if (me->GetEntry() != NPC_SHADO_PAN_DEFENDER)
                return;

            // Summon Mogu Here
            for (uint8 i = 0; i < 2; i++)
                me->SummonCreature(NPC_SHANZE_WARRIOR, moguAssaultSide[1].GetPositionX() + frand(-8.5, 8.5f), moguAssaultSide[1].GetPositionY() + frand(-5.5, 5.5f), moguAssaultSide[1].GetPositionZ(), moguAssaultSide[1].GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

            // Set in combat with this
            if (Creature* shanzePyro = me->SummonCreature(NPC_SHANZE_PYROMANCER, moguAssaultSide[1], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
            {
                me->GetMotionMaster()->MoveChase(shanzePyro, 0, me->GetAngle(shanzePyro));
                me->Attack(shanzePyro, true);

                std::list<Creature*> warriorsList;
                GetCreatureListWithEntryInGrid(warriorsList, me, NPC_SHADO_PAN_WARRIOR, 100.0f);

                for (auto&& itr : warriorsList)
                {
                    itr->GetMotionMaster()->MoveChase(shanzePyro, 0, itr->GetAngle(shanzePyro));
                    itr->Attack(shanzePyro, true);
                }
            }
        }
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(45) && attacker->GetTypeId() != TypeID::TYPEID_PLAYER)
            damage = 0;
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->GetMotionMaster()->MoveTargetedHome();

        if (!hasEvade && me->GetEntry() == NPC_SHADO_PAN_DEFENDER)
        {
            hasEvade = true;
            Talk(TALK_SPECIAL_1);

            if (Creature* wrathion = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_WRATHION) : 0))
                wrathion->AI()->DoAction(ACTION_FORGE_FIRST_WAVE_DONE);
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_HEALING_ORB)
            {
                Talk(TALK_INTRO);
                DoCast(me, SPELL_HEALING_ORB_AT, true);
                events.ScheduleEvent(EVENT_HEALING_ORB, 10 * IN_MILLISECONDS);
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Thunder Forge Bunny 70061
struct npc_thunder_forge_bunny : public ScriptedAI
{
    npc_thunder_forge_bunny(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint32 powerValue;

    void InitializeAI() OVERRIDE
    {
        powerValue = 0;
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            if (me->GetInstanceScript())
                me->GetInstanceScript()->SetData(STEP_CHARGING, IN_PROGRESS);

            // Periodic Energize
            scheduler
                .Schedule(Milliseconds(1000), [this](TaskContext context)
            {
                DoCast(me, SPELL_THUNDER_FORGE_CHARGE, true);

                if (++powerValue > 199)
                {
                    if (me->GetInstanceScript())
                        me->GetInstanceScript()->SetData(STEP_CHARGING, SPECIAL);

                    return;
                }

                context.Repeat(Milliseconds(1000));
            });

            // Waves Controller
            scheduler
                .SetValidator([this] { return me->GetInstanceScript() && me->GetInstanceScript()->GetData(STEP_CHARGING) == IN_PROGRESS; })
                .Schedule(Milliseconds(8500), [this](TaskContext context)
            {
                // additional check
                if (Creature* vulkon = me->FindNearestCreature(NPC_FORGEMASTER_VULKON, 200.0f, true))
                    return;

                // Summon Waves here
                uint8 index = urand(0, 2);

                for (uint8 i = 0; i < 2; i++)
                    me->SummonCreature(NPC_SHANZE_WARRIOR, moguAssaultSide[index].GetPositionX() + frand(-8.5, 8.5f), moguAssaultSide[index].GetPositionY() + frand(-5.5, 5.5f), moguAssaultSide[index].GetPositionZ(), moguAssaultSide[index].GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                // Set in combat with this
                if (Creature* shanzePyro = me->SummonCreature(NPC_SHANZE_PYROMANCER, moguAssaultSide[index], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    std::list<Creature*> warriorsList;
                    GetCreatureListWithEntryInGrid(warriorsList, me, NPC_SHADO_PAN_WARRIOR, 200.0f);
                    GetCreatureListWithEntryInGrid(warriorsList, me, NPC_SHADO_PAN_DEFENDER, 200.0f);

                    for (auto&& itr : warriorsList)
                    {
                        itr->GetMotionMaster()->MoveChase(shanzePyro, 0, itr->GetAngle(shanzePyro));
                        itr->Attack(shanzePyro, true);
                    }
                }

                context.Repeat(Seconds(45), Seconds(50));
            });

            // Extra Help
            scheduler
                .SetValidator([this] { return me->GetInstanceScript() && (me->GetInstanceScript()->GetData(STEP_CHARGING) == IN_PROGRESS || me->GetInstanceScript()->GetData(STEP_CHARGING) == SPECIAL); })
                .Schedule(Milliseconds(25000), [this](TaskContext context)
            {
                Talk(TALK_INTRO);

                if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
                    DoCast(target, SPELL_OVERCHARGE_EFF, true);

                context.Repeat(Seconds(35), Seconds(45));
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Forgemaster Vulkon 70074
struct npc_forgemaster_vulkon : public customCreatureAI
{
    npc_forgemaster_vulkon(Creature* creature) : customCreatureAI(creature) { }

    void InitializeAI() OVERRIDE
    {
        DoCast(me, SPELL_FORGEMASTER_SPAWN, true);
        Reset();

        if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
        {
            me->GetMotionMaster()->MoveChase(target, 0, me->GetAngle(target));
            me->Attack(target, true);
        }
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_LIGHTNING_SMASH, urand(3500, 9 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_ELECTRIFIED, 15 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_SWITCH_WEAPONS, 10 * IN_MILLISECONDS);
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->GetMotionMaster()->MoveTargetedHome();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_CHARGING, DONE);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        // Allow to death from creature if player que as healer
        if (HealthBelowPct(45) && attacker->GetTypeId() != TypeID::TYPEID_PLAYER && me->GetInstanceScript() && !me->GetInstanceScript()->GetData(ROLE_DATA))
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
            ExecuteTargetEvent(SPELL_LIGHTNING_SMASH, urand(7500, 13 * IN_MILLISECONDS), EVENT_LIGHTNING_SMASH, eventId);
            ExecuteTargetEvent(SPELL_ELECTRFIED, 14500, EVENT_ELECTRIFIED, eventId, PRIORITY_SELF);

            if (eventId == EVENT_SWITCH_WEAPONS)
            {
                // just announce
                Talk(urand(0, 1));
                events.ScheduleEvent(EVENT_SWITCH_WEAPONS, urand(25 * IN_MILLISECONDS, 30 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Shanze Warrior 69833
struct npc_thunder_forge_shanze_warrior : public customCreatureAI
{
    npc_thunder_forge_shanze_warrior(Creature* creature) : customCreatureAI(creature) { }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
        {
            me->GetMotionMaster()->MoveChase(target, 0, me->GetAngle(target));
            me->Attack(target, true);
        }
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(3500, 9 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_CLEAVE, 5 * IN_MILLISECONDS);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        // Allow to death from creature if player que as healer
        if (HealthBelowPct(45) && attacker->GetTypeId() != TypeID::TYPEID_PLAYER && me->GetInstanceScript() && !me->GetInstanceScript()->GetData(ROLE_DATA))
            damage = 0;
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->GetMotionMaster()->MoveTargetedHome();
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
            ExecuteTargetEvent(SPELL_MORTAL_STRIKE, urand(3500, 9 * IN_MILLISECONDS), EVENT_MORTAL_STRIKE, eventId);
            ExecuteTargetEvent(SPELL_CLEAVE, urand(4500, 6 * IN_MILLISECONDS), EVENT_CLEAVE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Shanze Pyromancer 69824
struct npc_thunder_forge_shanze_pyromancer : public customCreatureAI
{
    npc_thunder_forge_shanze_pyromancer(Creature* creature) : customCreatureAI(creature) { }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
        {
            me->GetMotionMaster()->MoveChase(target, 0, me->GetAngle(target));
            me->Attack(target, true);
        }
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_FIREBALL, urand(1500, 2 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_METEOR, 9500);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        // Allow to death from creature if player que as healer
        if (HealthBelowPct(45) && attacker->GetTypeId() != TypeID::TYPEID_PLAYER && me->GetInstanceScript() && !me->GetInstanceScript()->GetData(ROLE_DATA))
            damage = 0;
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->GetMotionMaster()->MoveTargetedHome();
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
            ExecuteTargetEvent(SPELL_FIREBALL, urand(3500, 4 * IN_MILLISECONDS), EVENT_FIREBALL, eventId);
            ExecuteTargetEvent(SPELL_METEOR, 10 * IN_MILLISECONDS, EVENT_METEOR, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Thunder Forge 2 Bunny 70283
struct npc_thunder_forge_second_bunny : public ScriptedAI
{
    npc_thunder_forge_second_bunny(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint32 powerValue;
    float x, y;

    void InitializeAI() OVERRIDE
    {
        powerValue = 0;
        x = 0.0f; y = 0.0f;
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            // Periodic Energize
            scheduler
                .Schedule(Milliseconds(1000), [this](TaskContext context)
            {
                DoCast(me, SPELL_THUNDER_FORGE_CHARGE, true);

                if (++powerValue > 159)
                {
                    if (me->GetInstanceScript())
                        me->GetInstanceScript()->SetData(STEP_FILTH_FOUND, DONE);

                    return;
                }

                context.Repeat(Milliseconds(1000));
            });

            // Waves Controller
            scheduler
                .SetValidator([this] { return me->GetInstanceScript() && me->GetInstanceScript()->GetData(STEP_FILTH_FOUND) != DONE; })
                .Schedule(Milliseconds(3000), [this](TaskContext context)
            {
                // Summon Waves here
                for (uint8 i = 0; i < 2; i++)
                {
                    GetPositionWithDistInOrientation(me, frand(17.0f, 25.0f), frand(0, 2 * M_PI), x, y);
                    me->SummonCreature(NPC_SHA_FIEND, x, y, me->GetMap()->GetHeight(x, y, me->GetPositionZ(), true, 6.0f), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                }

                if (roll_chance_i(45))
                {
                    GetPositionWithDistInOrientation(me, frand(17.0f, 25.0f), frand(0, 2 * M_PI), x, y);
                    me->SummonCreature(NPC_SHA_BEAST, x, y, me->GetMap()->GetHeight(x, y, me->GetPositionZ(), true, 6.0f), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                }

                context.Repeat(Seconds(10), Seconds(20));
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Sha Fiend, Sha Beast 70039, 68030
struct npc_thunder_forge_sha_fiend : public customCreatureAI
{
    npc_thunder_forge_sha_fiend(Creature* creature) : customCreatureAI(creature) { }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (Creature* target = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_CELESTIAL_BLACKSMITH) : 0))
        {
            me->GetMotionMaster()->MoveChase(target, 0, me->GetAngle(target));
            me->Attack(target, true);
        }
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        if (me->GetEntry() == NPC_SHA_FIEND)
            events.ScheduleEvent(EVENT_SHA_BLAST, urand(1500, 6 * IN_MILLISECONDS));
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->GetMotionMaster()->MoveTargetedHome();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        me->DisappearAndDie();
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
            ExecuteTargetEvent(SPELL_SHA_BLAST, urand(4500, 10 * IN_MILLISECONDS), EVENT_SHA_BLAST, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Sha Almagmation 70228
struct npc_sha_almagmation : public customCreatureAI
{
    npc_sha_almagmation(Creature* creature) : customCreatureAI(creature) { }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (Player* target = ObjectAccessor::GetPlayer(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(PLAYER_DATA) : 0))
        {
            me->GetMotionMaster()->MoveChase(target, 0, me->GetAngle(target));
            me->Attack(target, true);
        }
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_INSANITY, 20 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_METEOR_STORM, urand(9500, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_SHADOW_CRASH, urand(3500, 6 * IN_MILLISECONDS));

        if (Creature* anvil = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_THUNDER_FORGE_2) : 0))
            anvil->AI()->Talk(TALK_SPECIAL_1);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        // Allow to death from creature if player que as healer
        if (HealthBelowPct(45) && attacker->GetTypeId() != TypeID::TYPEID_PLAYER && me->GetInstanceScript() && !me->GetInstanceScript()->GetData(ROLE_DATA))
            damage = 0;
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->GetMotionMaster()->MoveTargetedHome();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (me->GetInstanceScript())
            me->GetInstanceScript()->SetData(STEP_MANIFESTATION_OF_FILTH, DONE);
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
            if (eventId == EVENT_INSANITY)
                Talk(TALK_INTRO);

            ExecuteTargetEvent(SPELL_INSANITY, 25 * IN_MILLISECONDS, EVENT_INSANITY, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_METEOR_STORM, urand(10500, 15 * IN_MILLISECONDS), EVENT_METEOR_STORM, eventId);
            ExecuteTargetEvent(SPELL_SHADOW_CRASH_2, urand(8500, 12 * IN_MILLISECONDS), EVENT_SHADOW_CRASH, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Celestial Defender 69837
struct npc_thunder_forge_celestial_defender : public ScriptedAI
{
    npc_thunder_forge_celestial_defender(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    TaskScheduler scheduler;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->GetMotionMaster()->MovePoint(0, almagmationSummPos);

        Movement::MoveSplineInit init(me);
        init.MoveTo(almagmationSummPos.GetPositionX(), almagmationSummPos.GetPositionY(), almagmationSummPos.GetPositionZ());

        init.SetWalk(true);
        init.Launch();

        scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
        {
            me->SetHomePosition(*me);
        });
    }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_POWER_SURGE, 15 * IN_MILLISECONDS);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(45) && attacker->GetTypeId() != TypeID::TYPEID_PLAYER)
            damage = 0;
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->GetMotionMaster()->MoveTargetedHome();
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_POWER_SURGE)
            {
                Talk(TALK_INTRO);
                DoCast(me, SPELL_POWER_SURGE_AT, true);
                events.ScheduleEvent(EVENT_POWER_SURGE, 20 * IN_MILLISECONDS);
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Celestial Blacksmith 69828
struct npc_thunder_forge_celestial_blacksmith : public ScriptedAI
{
    npc_thunder_forge_celestial_blacksmith(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint32 anvilIndex;
    bool hasBegun;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        anvilIndex = 0;
        hasBegun = false;
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
        me->PrepareChanneledCast(me->GetOrientation());

        scheduler
            .Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            if (hasBegun)
            {
                if (GameObject* nearAnvil = me->FindNearestGameObject(GO_THUNDER_ANVIL, 20.0f))
                    nearAnvil->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);

                if (Creature* anvil = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_THUNDER_FORGE_2) : 0))
                    anvil->AI()->Talk(TALK_INTRO);
            }
            else
                hasBegun = true;

            me->StopMoving();
            me->HandleEmoteCommand(EMOTE_STATE_NONE);
            me->GetMotionMaster()->MovePoint(0, anvilsPathPos[anvilIndex]);

            scheduler.Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
            {
                if (Creature* anvil = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_THUNDER_FORGE_2) : 0))
                    me->SetFacingToObject(anvil);

                me->HandleEmoteStateCommand(EMOTE_STATE_WORK);
            });

            if (++anvilIndex > 6)
                anvilIndex = 0;

            context.Repeat(Seconds(25));
        });
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(45) && attacker->GetTypeId() != TypeID::TYPEID_PLAYER)
            damage = 0;
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Heavy Explosives 218196
class go_teardown_heavy_explosives : public GameObjectScript
{
    public:
        go_teardown_heavy_explosives() : GameObjectScript("go_teardown_heavy_explosives") { }
    
        bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
        {
            go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);

            if (player->GetInstanceScript())
                player->GetInstanceScript()->SetData(STEP_FIND_MISSED_EXPLOSIVE, DONE);

            return true;
        }
};

// Mogu Crucible 218910
class go_thunder_forge_mogu_crucible : public GameObjectScript
{
    public:
        go_thunder_forge_mogu_crucible() : GameObjectScript("go_thunder_forge_mogu_crucible") { }
    
        bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
        {
            go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
    
            if (player->GetInstanceScript())
                player->GetInstanceScript()->SetData(STEP_FORGED_FROM_TRILLIUM, DONE);
    
            return true;
        }
};

// Thunder Forge Anvil 218701
class go_thunder_forge_anvil : public GameObjectScript
{
    public:
        go_thunder_forge_anvil() : GameObjectScript("go_thunder_forge_anvil") { }
    
        bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
        {
            if (go->GetInstanceScript())
            {
                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);

                if (Creature* anvil = ObjectAccessor::GetCreature(*go, go->GetInstanceScript() ? go->GetInstanceScript()->GetData64(NPC_THUNDER_FORGE_2) : 0))
                    anvil->CastSpell(anvil, SPELL_THUNDER_SURGE, true);
            }
    
            return true;
        }
};

// Lightning Strike 137516
class spell_shaolmara_lightning_strike : public SpellScript
{
    PrepareSpellScript(spell_shaolmara_lightning_strike);

    void HandleEffectHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Creature* caster = GetCaster()->ToCreature())
        {
            if (Vehicle* veh = caster->GetVehicleKit())
            {
                if (veh->GetPassenger(0) && veh->GetPassenger(0)->ToPlayer())
                    caster->AI()->DoAction(ACTION_FLY_FALL);
                else
                    caster->Kill(caster);
            }
        }
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_shaolmara_lightning_strike::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Legacy of Blood 140497
class spell_teardown_legacy_of_blood : public AuraScript
{
    PrepareAuraScript(spell_teardown_legacy_of_blood);

    void HandleOnProc(ProcEventInfo& eventInfo)
    {
        if (Unit* owner = GetOwner()->ToUnit())
        {
            int32 damage = int32(eventInfo.GetDamageInfo()->GetDamage());
            owner->CastCustomSpell(owner, SPELL_LEGACY_OF_BLOOD_EFF, &damage, 0, 0, true);
        }
    }

    void Register() OVERRIDE
    {
        OnProc += AuraProcFn(spell_teardown_legacy_of_blood::HandleOnProc);
    }
};

// Lightning Charge 136495
class spell_in_sky_lightning_charge : public SpellScript
{
    PrepareSpellScript(spell_in_sky_lightning_charge);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != NPC_MANFRED; });
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_in_sky_lightning_charge::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
    }
};

// Electrified Chains 136695
class spell_in_sky_electrified_chains : public AuraScript
{
    PrepareAuraScript(spell_in_sky_electrified_chains);

    void OnTrigger(AuraEffect const* /*aurEff*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* owner = GetOwner()->ToUnit())
            {
                if (caster->GetExactDist2d(owner) > 25.0f)
                {
                    owner->RemoveAurasDueToSpell(SPELL_ELECTRIFIED_CHAINS, caster->GetGUID());
                    caster->CastSpell(caster, SPELL_BROKEN_CHAINS, true);
                }
            }
        }
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_in_sky_electrified_chains::OnTrigger, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// Jump on Pterrowing 137263
class spell_jump_on_pterrorwing : public SpellScript
{
    PrepareSpellScript(spell_jump_on_pterrorwing);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != NPC_ZANDALARI_BEASTCALLER; });

        if (targets.size() > 1)
            Skyfire::Containers::RandomResizeList(targets, 1);
    }

    void HandleEffectHitTarget(SpellEffIndex /*effIndex*/)
    {
        // Large hack cuz we can`t jump - vehicle issue.
        // Player`ll drop to ground while try to jump = die
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                caster->Kill(target);
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_jump_on_pterrorwing::SelectTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
        OnEffectHitTarget += SpellEffectFn(spell_jump_on_pterrorwing::HandleEffectHitTarget, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
    }
};

// Smoke Bomb Deactivated 137249
class spell_stormsea_landing_smoke_bomb_deactivated : public SpellScript
{
    PrepareSpellScript(spell_stormsea_landing_smoke_bomb_deactivated);

    void HandleEffectHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Creature* target = GetHitCreature())
        {
            target->RemoveAurasDueToSpell(SPELL_SENTRY);

            if (target->GetInstanceScript())
                target->GetInstanceScript()->SetData(STEP_PATH_OF_SMOKE, IN_PROGRESS); // counter
        }
    }

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != NPC_SENTRY_TOTEM; });
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_stormsea_landing_smoke_bomb_deactivated::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_stormsea_landing_smoke_bomb_deactivated::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
    }
};

// Mark of Taoshi Eff 137311
class spell_stormsea_landing_mark_of_taoshi_eff : public AuraScript
{
    PrepareAuraScript(spell_stormsea_landing_mark_of_taoshi_eff);

    void HandleAuraEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetOwner()->ToUnit())
        {
            owner->RemoveAurasDueToSpell(SPELL_MARK_OF_TAOSHI);

            if (Creature* taoshi = ObjectAccessor::GetCreature(*owner, owner->GetInstanceScript() ? owner->GetInstanceScript()->GetData64(NPC_TAOSHI_STORMSEA_SHIP) : 0))
                taoshi->AI()->DoAction(ACTION_MOVE_TO);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_stormsea_landing_mark_of_taoshi_eff::HandleAuraEffectApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
    }
};

// Stormsea Infiltrating Eff 140639
class spell_stormsea_landing_infiltrating_eff : public SpellScript
{
    PrepareSpellScript(spell_stormsea_landing_infiltrating_eff);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target && target->ToUnit() && (target->ToUnit()->HasAura(SPELL_SPOTTED) || target->ToUnit()->IsInCombat()); });
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_stormsea_landing_infiltrating_eff::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_stormsea_landing_infiltrating_eff::SelectTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ALLY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_stormsea_landing_infiltrating_eff::SelectTarget, EFFECT_2, TARGET_UNIT_SRC_AREA_ALLY);
    }
};

// Gatekeepers Orb 139274
class spell_stormsea_landing_gatekeepers_orb : public SpellScript
{
    PrepareSpellScript(spell_stormsea_landing_gatekeepers_orb);

    void HandleAfterCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            caster->KilledMonsterCredit(NPC_GATE_BUNNY_CREDIT);

            if (GameObject* moguGate = ObjectAccessor::GetGameObject(*caster, caster->GetInstanceScript() ? caster->GetInstanceScript()->GetData64(GO_MOGU_GATES_STORMSEA) : 0))
                moguGate->SetGoState(GOState::GO_STATE_ACTIVE);

            if (Creature* taoshi = ObjectAccessor::GetCreature(*caster, caster->GetInstanceScript() ? caster->GetInstanceScript()->GetData64(NPC_TAOSHI_STORMSEA_SHIP) : 0))
                taoshi->AI()->Talk(TALK_SPECIAL_11);
        }
    }

    void Register() OVERRIDE
    {
        AfterCast += SpellCastFn(spell_stormsea_landing_gatekeepers_orb::HandleAfterCast);
    }
};

// Sha Cosmetic Spawn 139444
class spell_thunder_forge_sha_cosmetic_spawn : public AuraScript
{
    PrepareAuraScript(spell_thunder_forge_sha_cosmetic_spawn);

    void HandleAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetOwner()->ToUnit())
            owner->SummonCreature(NPC_SHA_ALMAGAMATION, almagmationSummPos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_thunder_forge_sha_cosmetic_spawn::HandleAuraEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

// Thunder Surge 138834
class spell_thunder_forge_thunder_surge : public SpellScript
{
    PrepareSpellScript(spell_thunder_forge_thunder_surge);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != NPC_SHA_BEAST && target->GetEntry() != NPC_SHA_FIEND && target->GetEntry() != NPC_SHA_ALMAGAMATION; });
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_thunder_forge_thunder_surge::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_thunder_forge_thunder_surge::SelectTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_thunder_forge_thunder_surge::SelectTarget, EFFECT_2, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

// 873. Summoned by 140102 - Dust Cloud
class sat_shaolmara_dust_cloud : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_DUST_CLOUD_EFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_DUST_CLOUD_EFF);
    }
};

// 891. Summoned by 140662 - Arcanitals Barrier
class sat_arcanitals_barrier : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_ARCANITAL_BARRIER_EFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_ARCANITAL_BARRIER_EFF);
    }
};

// 834, 835. Summoned by 139187 - Lightning Pole
class sat_lightning_pole : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            if (Unit* caster = GetCaster())
                caster->CastSpell(itr, SPELL_LIGHTNING_POLE, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_LIGHTNING_POLE);
    }
};

// 745. Summoned by 137424 - Hail of Arrows
class sat_hail_of_arrows : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_HAIL_OF_ARROWS_EFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_HAIL_OF_ARROWS_EFF);
    }
};

// 732. Summoned by 137254 - Sentry
class sat_assault_stormsea_sentry : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* pTriggering) OVERRIDE
    {
        return pTriggering && (pTriggering->ToPlayer() || pTriggering->GetEntry() == NPC_SENTRY_LASER_BUNNY);
    }

    void OnTriggeringApply(WorldObject* pTriggering) OVERRIDE
    {
        if (Player* target = pTriggering->ToPlayer())
        {
            if (Creature* caster = GetCaster()->ToCreature())
            {
                caster->AI()->SetGUID(target->GetGUID());
                caster->CastSpell(caster, SPELL_SUMMON_SENTRY_LASER, true);
            }
        }
    }

    void OnTriggeringRemove(WorldObject* pTriggering) OVERRIDE
    {
        if (Creature* target = pTriggering->ToCreature())
            target->DespawnOrUnsummon();
    }
};

// 735. Summoned by 137282 - Mark of Taoshi
class sat_stormsea_landing_mark_of_taoshi : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_ABMUSH_OVERRIDE_AURA, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_ABMUSH_OVERRIDE_AURA);
    }
};

// 880. Summoned by 140392 - Keg Toss
class sat_stormsea_landing_keg_toss : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && (triggering->ToPlayer() || triggering->GetEntry() == NPC_DRUNKEN_WINGRIDER);
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_BUBBLING_BREW, true);
        else if (Creature* itr = triggering->ToCreature())
            itr->AI()->DoAction(ACTION_START_INTRO);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_BUBBLING_BREW);
    }
};

// 783. Summoned by 140660 - Meteor
class sat_thunder_forge_meteor : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_METEOR_AT_EFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_METEOR_AT_EFF);
    }
};

// 503. Summoned by 139249 - Healing Orb
class sat_thunder_forge_healing_orb : public IAreaTriggerOnce
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer();
    }

    void OnTrigger(WorldObject* target) OVERRIDE
    {
        if (Player* itr = target->ToPlayer())
            itr->CastSpell(itr, SPELL_HEALING_ORB_AT_EFF, true);
    }
};

// 868. Summoned by 140067 - Power Surge
class sat_thunder_forge_power_surge : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->CastSpell(itr, SPELL_POWER_SURGE_EFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* itr = triggering->ToPlayer())
            itr->RemoveAurasDueToSpell(SPELL_POWER_SURGE_EFF);
    }
};

// 747. Summoned by 137536 - Power Surge
class sat_in_sky_power_surge : public IAreaTriggerOnce
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer() && triggering->ToPlayer()->IsAlive();
    }

    void OnTrigger(WorldObject* target) OVERRIDE
    {
        if (Unit* caster = GetCaster())
            if (Player* itr = target->ToPlayer())
                caster->CastSpell(caster, SPELL_POWER_SURGE_SKY, true);
    }
};

// Areatrigger 8901
class AreaTrigger_at_thunder_forge_to_skies : public AreaTriggerScript
{
    public:
        AreaTrigger_at_thunder_forge_to_skies() : AreaTriggerScript("AreaTrigger_at_thunder_forge_to_skies") { }
    
        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) OVERRIDE
        {
            if (player->GetInstanceScript() && player->GetInstanceScript()->GetData(STEP_WAS_YOURS_BECOME_MINE) != DONE)
            {
                player->GetInstanceScript()->SetData(STEP_WAS_YOURS_BECOME_MINE, DONE);

                // Follower, not executed in instance script
                if (Creature* taoshi = player->FindNearestCreature(NPC_TAOSHI_CAVE, 200.0f, true))
                    taoshi->DespawnOrUnsummon();
            }
    
            return false;
        }
};

// Areatrigger 8914, 8920
class AreaTrigger_at_shanbu_fall : public AreaTriggerScript
{
    public:
        AreaTrigger_at_shanbu_fall() : AreaTriggerScript("AreaTrigger_at_shanbu_fall") { }
    
        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) OVERRIDE
        {
            if (player->GetInstanceScript() && player->GetInstanceScript()->GetData(STEP_BLOODY_CROSSROAD) < IN_PROGRESS && player->GetInstanceScript()->GetData64(PHASE_DATA) == SCENARIO_FALL_OF_SHANBU)
                player->GetInstanceScript()->SetData(STEP_BLOODY_CROSSROAD, IN_PROGRESS);
    
            return false;
        }
};

// By Spell 137311
class cond_mark_of_taoshi : public ConditionScript
{
    public:
        cond_mark_of_taoshi() : ConditionScript("cond_mark_of_taoshi") { }
    
        bool OnConditionCheck(Condition* cond, ConditionSourceInfo& source) OVERRIDE
        {
            return source.mConditionTargets[0] && source.mConditionTargets[0]->ToCreature() && source.mConditionTargets[0]->ToCreature()->HasAura(SPELL_MARK_OF_TAOSHI);
        }
};

void AddSC_scenario_isle_of_thunder()
{
    new npc_shaolmara_lady_jaina();
    new creature_script<npc_shaolmara_player_covenant_hippogryph>("npc_shaolmara_player_covenant_hippogryph");
    new creature_script<npc_shaolmara_covenant_hippogryph>("npc_shaolmara_covenant_hippogryph");
    new creature_script<npc_shaolmara_vereesa_windrunner>("npc_shaolmara_vereesa_windrunner");
    new creature_script<npc_shaolmara_beastcaller>("npc_shaolmara_beastcaller");
    new creature_script<npc_shaolmara_raptor>("npc_shaolmara_raptor");
    new creature_script<npc_arcanital_marakah>("npc_arcanital_marakah");
    new creature_script<npc_shaolmara_anki>("npc_shaolmara_anki");
    new creature_script<npc_shaolmara_beastcaller_pakutesh>("npc_shaolmara_beastcaller_pakutesh");
    new creature_script<npc_shaolmara_zebtula_stoneshield>("npc_shaolmara_zebtula_stoneshield");
    new creature_script<npc_shaolmara_zandalari_barricade>("npc_shaolmara_zandalari_barricade");
    new npc_zebtula_lorthemar_theron();
    new creature_script<npc_zebtula_player_bloodhawk>("npc_zebtula_player_bloodhawk");
    new creature_script<npc_zebtula_bloodhawk>("npc_zebtula_bloodhawk");
    new creature_script<npc_zebtula_scout_captain_elsia>("npc_zebtula_scout_captain_elsia");
    new creature_script<npc_arcanital_tulachek>("npc_arcanital_tulachek");
    new creature_script<npc_zebtula_kresh_the_reaper>("npc_zebtula_kresh_the_reaper");
    new npc_teardown_scout_captain_elsia();
    new creature_script<npc_teardown_heavy_explosives>("npc_teardown_heavy_explosives");
    new creature_script<npc_teardown_lorthemar_therone>("npc_teardown_lorthemar_therone");
    new creature_script<npc_wargod_alchukla>("npc_wargod_alchukla");
    new creature_script<npc_teardown_zandalari_bloodguard>("npc_teardown_zandalari_bloodguard");
    new creature_script<npc_teardown_spirit_void_sentry>("npc_teardown_spirit_void_sentry");
    new creature_script<npc_teardown_zandalari_soultwister>("npc_teardown_zandalari_soultwister");
    new creature_script<npc_teardown_zandalari_haruspex>("npc_teardown_zandalari_haruspex");
    new creature_script<npc_teardown_covenant_leader>("npc_teardown_covenant_leader");
    new creature_script<npc_slavemaster_shiaxu>("npc_slavemaster_shiaxu");
    new creature_script<npc_shanze_electrocutioner>("npc_shanze_electrocutioner");
    new creature_script<npc_in_sky_lightning_strike_dummy>("npc_in_sky_lightning_strike_dummy");
    new creature_script<npc_shanze_slaver>("npc_shanze_slaver");
    new creature_script<npc_beastmaster_horaki>("npc_beastmaster_horaki");
    new creature_script<npc_lorthemar_therone_to_skies>("npc_lorthemar_therone_to_skies");
    new creature_script<npc_scout_captain_elsia_to_skies>("npc_scout_captain_elsia_to_skies");
    new creature_script<npc_thunderwing_to_skies>("npc_thunderwing_to_skies");
    new creature_script<npc_zandalari_pterrorwing_to_skies>("npc_zandalari_pterrorwing_to_skies");
    new creature_script<npc_zandlari_beastcaller_to_skies>("npc_zandlari_beastcaller_to_skies");
    new npc_taoshi_to_skies();
    new creature_script<npc_taoshi_stormsea_landing>("npc_taoshi_stormsea_landing");
    new creature_script<npc_thunderwing_stormsea_landing>("npc_thunderwing_stormsea_landing");
    new creature_script<npc_taoshi_stormsea_landing_ship>("npc_taoshi_stormsea_landing_ship");
    new creature_script<npc_captain_halukal>("npc_captain_halukal");
    new creature_script<npc_drunken_wingrider>("npc_drunken_wingrider");
    new creature_script<npc_veteran_wingrider>("npc_veteran_wingrider");
    new creature_script<npc_zandalari_golem>("npc_zandalari_golem");
    new creature_script<npc_zandalari_spirit_twister>("npc_zandalari_spirit_twister");
    new creature_script<npc_huseng_gatekeeper>("npc_huseng_gatekeeper");
    new npc_stormsea_landing_taoshi_queue();
    new creature_script<npc_stormsea_landing_sentry_totem>("npc_stormsea_landing_sentry_totem");
    new creature_script<npc_stormsea_landing_sentry_beam_bunny>("npc_stormsea_landing_sentry_beam_bunny");
    new creature_script<npc_stormsea_landing_zandalari_pterrorwing>("npc_stormsea_landing_zandalari_pterrorwing");
    new creature_script<npc_shanbu_assault_base>("npc_shanbu_assault_base");
    new npc_shanbu_fall_elsia();
    new creature_script<npc_shanbu_palace_gatekeeper>("npc_shanbu_palace_gatekeeper");
    new creature_script<npc_shanbu_shanze_electrocutioner>("npc_shanbu_shanze_electrocutioner");
    new creature_script<npc_shanbu_quilen_guardian>("npc_shanbu_quilen_guardian");
    new creature_script<npc_shan_bu>("npc_shan_bu");
    new creature_script<npc_shanbu_nalak>("npc_shanbu_nalak");
    new npc_wrathion_thunder_forge();
    new creature_script<npc_thunder_forge_shado_pan_warrior>("npc_thunder_forge_shado_pan_warrior");
    new creature_script<npc_thunder_forge_bunny>("npc_thunder_forge_bunny");
    new creature_script<npc_forgemaster_vulkon>("npc_forgemaster_vulkon");
    new creature_script<npc_thunder_forge_shanze_warrior>("npc_thunder_forge_shanze_warrior");
    new creature_script<npc_thunder_forge_shanze_pyromancer>("npc_thunder_forge_shanze_pyromancer");
    new creature_script<npc_thunder_forge_second_bunny>("npc_thunder_forge_second_bunny");
    new creature_script<npc_thunder_forge_sha_fiend>("npc_thunder_forge_sha_fiend");
    new creature_script<npc_sha_almagmation>("npc_sha_almagmation");
    new creature_script<npc_thunder_forge_celestial_defender>("npc_thunder_forge_celestial_defender");
    new creature_script<npc_thunder_forge_celestial_blacksmith>("npc_thunder_forge_celestial_blacksmith");
    new go_teardown_heavy_explosives();
    new go_thunder_forge_mogu_crucible();
    new go_thunder_forge_anvil();

    new GenericSpellScriptLoader<spell_shaolmara_lightning_strike>("spell_shaolmara_lightning_strike");
    new GenericAuraScriptLoader<spell_teardown_legacy_of_blood>("spell_teardown_legacy_of_blood");
    new GenericSpellScriptLoader<spell_in_sky_lightning_charge>("spell_in_sky_lightning_charge");
    new GenericAuraScriptLoader<spell_in_sky_electrified_chains>("spell_in_sky_electrified_chains");
    new GenericSpellScriptLoader<spell_jump_on_pterrorwing>("spell_jump_on_pterrorwing");
    new GenericSpellScriptLoader<spell_stormsea_landing_smoke_bomb_deactivated>("spell_stormsea_landing_smoke_bomb_deactivated");
    new GenericAuraScriptLoader<spell_stormsea_landing_mark_of_taoshi_eff>("spell_stormsea_landing_mark_of_taoshi_eff");
    new GenericSpellScriptLoader<spell_stormsea_landing_infiltrating_eff>("spell_stormsea_landing_infiltrating_eff");
    new GenericSpellScriptLoader<spell_stormsea_landing_gatekeepers_orb>("spell_stormsea_landing_gatekeepers_orb");
    new GenericAuraScriptLoader<spell_thunder_forge_sha_cosmetic_spawn>("spell_thunder_forge_sha_cosmetic_spawn");
    new GenericSpellScriptLoader<spell_thunder_forge_thunder_surge>("spell_thunder_forge_thunder_surge");
    new atrigger_script<sat_shaolmara_dust_cloud>("sat_shaolmara_dust_cloud");
    new atrigger_script<sat_arcanitals_barrier>("sat_arcanitals_barrier");
    new atrigger_script<sat_lightning_pole>("sat_lightning_pole");
    new atrigger_script<sat_hail_of_arrows>("sat_hail_of_arrows");
    new atrigger_script<sat_assault_stormsea_sentry>("sat_assault_stormsea_sentry");
    new atrigger_script<sat_stormsea_landing_mark_of_taoshi>("sat_stormsea_landing_mark_of_taoshi");
    new atrigger_script<sat_stormsea_landing_keg_toss>("sat_stormsea_landing_keg_toss");
    new atrigger_script<sat_thunder_forge_meteor>("sat_thunder_forge_meteor");
    new atrigger_script<sat_thunder_forge_healing_orb>("sat_thunder_forge_healing_orb");
    new atrigger_script<sat_thunder_forge_power_surge>("sat_thunder_forge_power_surge");
    new atrigger_script<sat_in_sky_power_surge>("sat_in_sky_power_surge");
    new AreaTrigger_at_thunder_forge_to_skies();
    new AreaTrigger_at_shanbu_fall();
    new cond_mark_of_taoshi();
}