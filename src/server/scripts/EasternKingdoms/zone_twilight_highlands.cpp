/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "SpellInfo.h"
#include "Vehicle.h"

enum Quests
{
    QUEST_SCENT_OF_BATTLE       = 27811,
    QUEST_BLOODEYE_BRUISER      = 27863,
    QUEST_DEADLY_DRAGONMAW      = 27864,
    QUEST_WAYWARD_WILDHUMMER    = 27865,
    QUEST_CALDERS_CREATION      = 27866,
    QUEST_EARL_OF_EVILSCERATION = 27867,
    QUEST_TWILIGHT_TERROR       = 27868,
    QUEST_ONLY_HOMES_WE_HAVE    = 27804,
    QUEST_TOTAL_WAR                = 27747,
};

enum Creatures
{
    NPC_TWILIGHT_TRAPPER   = 46984,
    NPC_GURGTHOCK          = 46935,
    NPC_HURP_DERP          = 46944,
    NPC_SULLY_KNEECAPPER   = 46946,
    NPC_TORG_DRAKEFLAYER   = 46945,
    NPC_CALDER_GRAY        = 47512,
    NPC_COLDERS_CREATION   = 46947,
    NPC_LORD_TULVAN        = 46948,
    NPC_EMBERSCAR_DEVOURER = 46949,
    NPC_POOL_OF_LAVA       = 48549,
    NPC_FIREBEADS_PATROL   = 46849,
};

enum Spells
{
    SPELL_MULLAN_GRYPHON_CREDIT   = 87513,

    // Crucible of Carnage
    SPELL_OVERHEAD_SMASH          = 88482,
    SPELL_INTIMIDATING_ROAR       = 91933,
    SPELL_WHIRLWIND               = 83016,
    SPELL_WHIRLWIND_EFF           = 33239,
    SPELL_CHARGE_MISC             = 88288,
    SPELL_UPPERCUT                = 80182,
    SPELL_BELCH                   = 88607,
    SPELL_BELCH_EFF               = 88609,
    SPELL_INHALE                  = 88615,
    SPELL_PLAGUE_EXPLOSION        = 88614,
    SPELL_POISON_CLOUD            = 90447,
    SPELL_POISON_CLOUD_TRIGGERING = 90449,
    SPELL_POISON_CLOUD_EFF        = 90448,
    SPELL_REPULSIVE_KICK          = 88605,
    SPELL_DEATH_BY_PEASANT        = 88619,
    SPELL_WORGEN_TRANSFORM        = 84798,
    SPELL_UNDYING_FRENZY          = 80515,
    SPELL_UNDYING_FRENZY_EFF      = 80514,
    SPELL_ACTIVATE_POOLS          = 90409,
    SPELL_FIREBALL                = 90446,
    SPELL_MAGMATIC_FAULT          = 90333,
    SPELL_LAVA_POOL               = 90391,
    SPELL_LAVA_POOL_EFF           = 90406,
    SPELL_VOLCANIC_SKYBOX         = 90418,
    SPELL_BUILDING_FIRE           = 87252,
};

enum Actions
{
    ACTION_START_INTRO,
    ACTION_BRUISER,
    ACTION_FACTION_CHAMPION_A,
    ACTION_FACTION_CHAMPION_H,
    ACTION_ABOMINATION,
    ACTION_LORD,
    ACTION_TWILIGHT_TERROR,
    ACTION_CADAVER_OUTRO,
    ACTION_LORD_OUTRO,
};

enum Types
{
    TYPE_TRIAL,
};

enum CrucibleYells
{
    TALK_MISC_INTRO, // for whole
    TALK_BRUISER_INTRO_1,
    TALK_BRUISER_INTRO_2,
    TALK_BRUISER_DEFEAT,
    TALK_WILDHUMMER_INTRO_1,
    TALK_WILDHUMMER_INTRO_2,
    TALK_WILDHUMMER_INTRO_3,
    TALK_WILDHUMMER_DEFEAT,
    TALK_DRAGONMAW_INTRO,
    TALK_DRAGONMAW_DEFEAT,
    TALK_ABOMINATION_INTRO_1,
    TALK_ABOMINATION_INTRO_2,
    TALK_ABOMINATION_DEFEAT_1,
    TALK_ABOMINATION_DEFEAT_2,
    TALK_LORD_INTRO,
    TALK_LORD_DEFEAT_1,
    TALK_LORD_DEFEAT_2,
    TALK_TWILIGHT_TERROR_INTRO_1,
    TALK_TWILIGHT_TERROR_INTRO_2,
    TALK_TWILIGHT_TERROR_DEFEAT,

    TALK_WILDHUMMER_SELF_INTRO = 0,
    TALK_DRAGONMAW_SELF_INTRO = 0,
    TALK_DRAGONMAW_SPELL,

    TALK_COLDER_INTRO = 0,
    TALK_COLDER_ULTIMATE,
    TALK_COLDER_OUTRO_1,
    TALK_COLDER_OUTRO_2,
    TALK_COLDER_OUTRO_3,
    TALK_ABOMINATION_SELF_INTRO = 0,
    TALK_ABOMINATION_SPELL,
    TALK_ABOMINATION_OUTRO_1,
    TALK_ABOMINATION_OUTRO_2,

    TALK_LORD_SELF_INTRO = 0,
    TALK_LORD_WOLF,
    TALK_LORD_ENRAGE,

    TALK_TWILIGHT_TERROR_SELF_INTRO = 0,
    TALK_TWILIGHT_TERROR_SPELL_LIQUID,
    TALK_TWILIGHT_TERROR_SKY,
};

class dunwald_victim_credit_store
{
public:
    uint64 player_guid;
    int32 repeat_timer;
};

struct npc_dunwald_victim : public ScriptedAI
{
    npc_dunwald_victim(Creature* creature) : ScriptedAI(creature) { }

    bool OnGossipHello(Player* player) OVERRIDE
    {
        if (player->GetQuestStatus(27642) != QUEST_STATUS_INCOMPLETE && !plrs_credited.empty() && std::find_if(plrs_credited.begin(), plrs_credited.end(), std::bind(std::equal_to<uint64>(), std::bind(&dunwald_victim_credit_store::player_guid, std::placeholders::_1), player->GetGUID())) != plrs_credited.end())
            return false;

        dunwald_victim_credit_store temp_player;
        temp_player.player_guid = player->GetGUID();
        temp_player.repeat_timer = 60000;
        plrs_credited.push_back(temp_player);

        player->KilledMonsterCredit(46609);
        return true;
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (plrs_credited.empty())
            return;

        // Update timer for each player
        for (auto&& itr : plrs_credited)
            itr.repeat_timer -= diff;

        // Erase if timer expired
        plrs_credited.remove_if([](dunwald_victim_credit_store const &i) { return i.repeat_timer <= 0; });
    }

    uint64 player_guid;
    int32 repeat_timer;

    private:
        std::list<dunwald_victim_credit_store> plrs_credited;
};

// Mullan Gryphon 46969
class npc_quest_mullan_gryphon : public CreatureScript
{
    public:
        npc_quest_mullan_gryphon() : CreatureScript("npc_quest_mullan_gryphon") { }
    
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();
    
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                creature->AI()->DoAction(ACTION_START_INTRO);
    
            player->CLOSE_GOSSIP_MENU();
    
            return true;
        }
    
        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(QUEST_SCENT_OF_BATTLE) == QUEST_STATUS_INCOMPLETE)
                player->ADD_GOSSIP_ITEM_DB(player->GetDefaultGossipMenuForSource(creature), 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        struct npc_quest_mullan_gryphonAI : public ScriptedAI
        {
            npc_quest_mullan_gryphonAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                me->SetAnimTier(ANIM_TIER_GROUND);
                me->OverrideInhabitType(INHABIT_GROUND);
                me->UpdateMovementFlags();
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_START_INTRO)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    DoCast(me, SPELL_MULLAN_GRYPHON_CREDIT, true);

                    // Fly Away
                    me->SetAnimTier(ANIM_TIER_FLY);
                    me->OverrideInhabitType(INHABIT_AIR);
                    me->UpdateMovementFlags();

                    float x, y;
                    GetPositionWithDistInOrientation(me, 10.0f, frand(0, 2 * M_PI), x, y);
                    me->GetMotionMaster()->MovePoint(0, x, y, me->GetPositionZ() + 16.0f);
                    me->DespawnOrUnsummon(me->GetSplineDuration());
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_quest_mullan_gryphonAI(creature);
        }
};

const std::map<uint32, uint32> invChallengeType =
{
    { QUEST_BLOODEYE_BRUISER,      ACTION_BRUISER            },
    { QUEST_DEADLY_DRAGONMAW,      ACTION_FACTION_CHAMPION_A },
    { QUEST_WAYWARD_WILDHUMMER,    ACTION_FACTION_CHAMPION_H },
    { QUEST_CALDERS_CREATION,      ACTION_ABOMINATION        },
    { QUEST_EARL_OF_EVILSCERATION, ACTION_LORD               },
    { QUEST_TWILIGHT_TERROR,       ACTION_TWILIGHT_TERROR    },
};

const Position championsSummonPos[4] =
{
    { -4131.70f, -5193.46f, -9.56f, 2.46f }, // Bruiser & Tulvan & Twilight Lord
    { -4231.34f, -5139.36f, -7.44f, 5.64f }, // Alliance Champion
    { -4135.14f, -5138.86f, 40.82f, 3.22f }, // Horde Champion
    { -4172.73f, -5087.46f, -10.60f, 4.12f }, // Calder`s Creation
};

const Position sullyPath[3] =
{
    { -4202.10f, -5160.25f, -7.73f, 0.0f },
    { -4181.07f, -5159.61f, -7.73f, 0.0f },
    { -4181.64f, -5158.51f, -7.73f, 0.0f },
};

const Position cadaverPath[4] =
{
    { -4173.43f, -5097.15f, -7.67f, 0.0f },
    { -4182.70f, -5122.06f, -7.73f, 0.0f },
    { -4175.26f, -5153.65f, -7.73f, 0.0f },
    { -4176.00f, -5152.72f, -7.73f, 0.0f },
};

const Position chargePos            = { -4157.56f, -5141.31f, -7.74f, 0.0f };
const Position hordeChampionJumpPos = { -4161.16f, -5141.31f, -7.73f, 0.0f };

// Gurgthock 46935
class npc_gurgthock_twilands : public CreatureScript
{
    public:
        npc_gurgthock_twilands() : CreatureScript("npc_gurgthock_twilands") { }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
        {
            creature->AI()->SetGUID(player->GetGUID());
            creature->AI()->DoAction(invChallengeType.find(quest->GetQuestId())->second);

            return false;
        }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver() && !creature->AI()->GetData(TYPE_TRIAL)) // not allow to accept new quest while anybody on arena
                player->PrepareQuestMenu(creature->GetGUID());

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        struct npc_gurgthock_twilandsAI : public ScriptedAI
        {
            npc_gurgthock_twilandsAI(Creature* creature) : ScriptedAI(creature), summons(me) { }

            TaskScheduler scheduler;
            SummonList summons;
            uint64 championGUID;
            bool hasTrialBegun;
            uint32 delay;

            void Reset() OVERRIDE
            {
                hasTrialBegun = false;
                championGUID = 0;
                delay         = 0;
                scheduler.CancelAll();

                std::list<Creature*> lavaPools;
                GetCreatureListWithEntryInGrid(lavaPools, me, NPC_POOL_OF_LAVA, 200.0f);

                for (auto&& itr : lavaPools)
                    itr->RemoveAurasDueToSpell(SPELL_LAVA_POOL);
            }

            void SetGUID(uint64 guid, int32 /*type*/) OVERRIDE
            {
                championGUID = guid;
            }

            uint64 GetGUID(int32 /*type*/) const OVERRIDE
            {
                return championGUID;
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                if (type == TYPE_TRIAL)
                    return hasTrialBegun ? 1 : 0;

                return 0;
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (hasTrialBegun)
                    return;

                if (actionId < ACTION_CADAVER_OUTRO)
                {
                    hasTrialBegun = true;

                    delay = 3 * IN_MILLISECONDS;
                    scheduler
                        .Schedule(Milliseconds(delay), [this](TaskContext context)
                    {
                        if (Player* champion = ObjectAccessor::GetPlayer(*me, championGUID))
                            Talk(TALK_MISC_INTRO, champion);
                    });
                }

                switch (actionId)
                {
                    case ACTION_BRUISER:
                        scheduler
                            .Schedule(Milliseconds(delay += 5 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            if (Player* champion = ObjectAccessor::GetPlayer(*me, championGUID))
                                Talk(TALK_BRUISER_INTRO_1, champion);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 8 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_BRUISER_INTRO_2);

                            // Summon first champion (only if player is in World)
                            if (Player* champion = ObjectAccessor::GetPlayer(*me, championGUID))
                                me->SummonCreature(NPC_HURP_DERP, championsSummonPos[0], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            else
                                Reset();
                        });
                        break;
                    case ACTION_FACTION_CHAMPION_A:
                        scheduler
                            .Schedule(Milliseconds(delay += 6 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            if (Player* champion = ObjectAccessor::GetPlayer(*me, championGUID))
                                Talk(TALK_DRAGONMAW_INTRO, champion);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
                        {
                            // Summon second champion (only if player is in World)
                            if (Player* champion = ObjectAccessor::GetPlayer(*me, championGUID))
                                me->SummonCreature(NPC_TORG_DRAKEFLAYER, championsSummonPos[2], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            else
                                Reset();
                        });
                        break;
                    case ACTION_FACTION_CHAMPION_H:
                        scheduler
                            .Schedule(Milliseconds(delay += 6 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            if (Player* champion = ObjectAccessor::GetPlayer(*me, championGUID))
                                Talk(TALK_WILDHUMMER_INTRO_2, champion);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 6200), [this](TaskContext context)
                        {
                            Talk(TALK_WILDHUMMER_INTRO_3);

                            // Summon second champion (only if player is in World)
                            if (Player* champion = ObjectAccessor::GetPlayer(*me, championGUID))
                                me->SummonCreature(NPC_SULLY_KNEECAPPER, championsSummonPos[1], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            else
                                Reset();
                        });
                        break;
                    case ACTION_ABOMINATION:
                        scheduler
                            .Schedule(Milliseconds(delay += 6 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_ABOMINATION_INTRO_1);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 8700), [this](TaskContext context)
                        {
                            Talk(TALK_ABOMINATION_INTRO_2);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 1000), [this](TaskContext context)
                        {
                            if (Creature* calderGray = me->FindNearestCreature(NPC_CALDER_GRAY, 100.0f, true))
                                calderGray->AI()->Talk(TALK_COLDER_INTRO);

                            // Summon third champion (only if player is in World)
                            if (Player* champion = ObjectAccessor::GetPlayer(*me, championGUID))
                                me->SummonCreature(NPC_COLDERS_CREATION, championsSummonPos[3], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            else
                                Reset();
                        });
                        break;
                    case ACTION_LORD:
                        scheduler
                            .Schedule(Milliseconds(delay += 6 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_LORD_INTRO);

                            // Summon four champion (only if player is in World)
                            if (Player* champion = ObjectAccessor::GetPlayer(*me, championGUID))
                                me->SummonCreature(NPC_LORD_TULVAN, championsSummonPos[0], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            else
                                Reset();
                        });
                        break;
                    case ACTION_TWILIGHT_TERROR:
                        scheduler
                            .Schedule(Milliseconds(delay += 5 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_TWILIGHT_TERROR_INTRO_1);
                        });

                        scheduler
                            .Schedule(Milliseconds(delay += 8 * IN_MILLISECONDS), [this](TaskContext context)
                        {
                            Talk(TALK_TWILIGHT_TERROR_INTRO_2);

                            // Summon last champion (only if player is in World)
                            if (Player* champion = ObjectAccessor::GetPlayer(*me, championGUID))
                                me->SummonCreature(NPC_EMBERSCAR_DEVOURER, championsSummonPos[0], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            else
                                Reset();
                        });
                        break;
                    case ACTION_CADAVER_OUTRO:
                        scheduler
                            .Schedule(Milliseconds(2350), [this](TaskContext context)
                        {
                            Talk(TALK_ABOMINATION_DEFEAT_2);
                        });

                        scheduler
                            .Schedule(Milliseconds(6000), [this](TaskContext context)
                        {
                            if (Creature* colder = me->FindNearestCreature(NPC_CALDER_GRAY, 150.0f, true))
                                colder->AI()->Talk(TALK_COLDER_OUTRO_2);
                        });

                        scheduler
                            .Schedule(Milliseconds(14000), [this](TaskContext context)
                        {
                            if (Creature* colder = me->FindNearestCreature(NPC_CALDER_GRAY, 150.0f, true))
                                colder->AI()->Talk(TALK_COLDER_OUTRO_3);
                        });
                        break;
                    case ACTION_LORD_OUTRO:
                        scheduler
                            .Schedule(Milliseconds(5500), [this](TaskContext context)
                        {
                            if (Player* champion = ObjectAccessor::GetPlayer(*me, GetGUID(0)))
                                Talk(TALK_LORD_DEFEAT_2, champion);
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
            return new npc_gurgthock_twilandsAI(creature);
        }
};

// Hurp`Derp 46944
struct npc_twilight_hurp_derp : public customCreatureAI
{
    npc_twilight_hurp_derp(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        Reset();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

        float x, y, z;
        GetPositionWithDistInOrientation(me, 50.0f, me->GetOrientation(), x, y);
        z = me->GetPositionZ();
        me->UpdateAllowedPositionZ(x, y, z);
        me->GetMotionMaster()->MovePoint(0, x, y, z);

        scheduler
            .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
        {
            me->SetHomePosition(*me);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

            // Special Check - if we not pulled in 15s - remove from world for not spam
            scheduler
                .Schedule(Milliseconds(15 * IN_MILLISECONDS), [this](TaskContext context)
            {
                if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
                    gurgthock->AI()->Reset();

                me->DisappearAndDie();
            });
        });
    }

    void Reset() OVERRIDE
    {
        scheduler.CancelAll();
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        scheduler.CancelAll();

        scheduler
            .SetValidator([this] { return !me->HasUnitState(UNIT_STATE_CASTING) && !me->HasAura(SPELL_WHIRLWIND); })
            .Schedule(Seconds(7), [this](TaskContext context)
        {
            DoCast(me, SPELL_OVERHEAD_SMASH);
            context.Repeat(Seconds(12));
        });

        scheduler
            .SetValidator([this] { return !me->HasUnitState(UNIT_STATE_CASTING) && !me->HasAura(SPELL_WHIRLWIND); })
            .Schedule(Seconds(10), [this](TaskContext context)
        {
            if (Unit* target = me->GetVictim())
                DoCast(target, SPELL_INTIMIDATING_ROAR);

            scheduler
                .Schedule(Seconds(1), [this](TaskContext context)
            {
                DoCast(me, SPELL_WHIRLWIND);
            });

            context.Repeat(Seconds(25));
        });
    }

    void EnterEvadeMode() OVERRIDE
    {
        if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
            gurgthock->AI()->Reset();

        ScriptedAI::EnterEvadeMode();
        summons.DespawnAll();
        me->DisappearAndDie();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
        {
            if (Player* champion = ObjectAccessor::GetPlayer(*me, gurgthock->AI()->GetGUID()))
            {
                champion->GroupEventHappens(QUEST_BLOODEYE_BRUISER, me);
                gurgthock->AI()->Talk(TALK_BRUISER_DEFEAT, champion);
            }

            gurgthock->AI()->Reset();
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

// Sully 46946, Torg 46945
struct npc_twilight_faction_champions : public customCreatureAI
{
    npc_twilight_faction_champions(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        Reset();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

        if (IsHordeQuest())
        {
            Movement::MoveSplineInit init(me);
            for (auto&& itr : sullyPath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.Launch();
            init.SetWalk(true);

            scheduler
                .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
            {
                Talk(TALK_WILDHUMMER_SELF_INTRO);

                scheduler
                    .Schedule(Milliseconds(1500), [this](TaskContext context)
                {
                    me->SetHomePosition(*me);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);
                });

                // Special Check - if we not pulled in 15s - remove from world for not spam
                scheduler
                    .Schedule(Milliseconds(16 * IN_MILLISECONDS), [this](TaskContext context)
                {
                    if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
                        gurgthock->AI()->Reset();

                    me->DisappearAndDie();
                });
            });
        }
        else
        {
            me->GetMotionMaster()->MoveJump(hordeChampionJumpPos, 25.0f, 25.0f, EVENT_JUMP);

            // Special Check - if we not pulled in 15s - remove from world for not spam
            scheduler
                .Schedule(Milliseconds(20 * IN_MILLISECONDS), [this](TaskContext context)
            {
                if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
                    gurgthock->AI()->Reset();

                me->DisappearAndDie();
            });
        }
    }

    void Reset() OVERRIDE
    {
        scheduler.CancelAll();
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        scheduler.CancelAll();

        scheduler
            .SetValidator([this] { return !me->HasUnitState(UNIT_STATE_CASTING); })
            .Schedule(Seconds(7), [this](TaskContext context)
        {
            if (Unit* target = me->GetVictim())
                DoCast(target, SPELL_UPPERCUT);

            context.Repeat(Seconds(6));
        });

        scheduler
            .SetValidator([this] { return !me->HasUnitState(UNIT_STATE_CASTING); })
            .Schedule(Seconds(10), [this](TaskContext context)
        {
            if (me->GetEntry() == NPC_TORG_DRAKEFLAYER)
                Talk(TALK_DRAGONMAW_SPELL);

            if (Unit* target = me->GetVictim())
                DoCast(target, SPELL_CHARGE_MISC);

            scheduler
                .Schedule(Milliseconds(1 * IN_MILLISECONDS), [this](TaskContext context)
            {
                me->GetMotionMaster()->MoveCharge(chargePos.GetPositionX(), chargePos.GetPositionY(), chargePos.GetPositionZ(), 30.0f, EVENT_CHARGE);
            });

            context.Repeat(Seconds(30));
        });
    }

    void MovementInform(uint32 /*type*/, uint32 pointId) OVERRIDE
    {
        switch (pointId)
        {
            case EVENT_CHARGE:
                if (Unit* target = me->GetVictim())
                {
                    target->ExitVehicle();

                    // Vehicle Issue
                    scheduler
                        .Schedule(Milliseconds(1000), [this](TaskContext context)
                    {
                        if (Unit* victim = me->GetVictim())
                            DoCast(victim, SPELL_UPPERCUT);
                    });
                }
                break;
            case EVENT_JUMP:
                Talk(TALK_DRAGONMAW_SELF_INTRO);

                scheduler
                    .Schedule(Milliseconds(1500), [this](TaskContext context)
                {
                    me->SetHomePosition(*me);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);
                });
                break;
        }
    }

    void EnterEvadeMode() OVERRIDE
    {
        if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
            gurgthock->AI()->Reset();

        ScriptedAI::EnterEvadeMode();
        summons.DespawnAll();
        me->DisappearAndDie();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
        {
            if (Player* champion = ObjectAccessor::GetPlayer(*me, gurgthock->AI()->GetGUID()))
            {
                champion->GroupEventHappens(IsHordeQuest() ? QUEST_WAYWARD_WILDHUMMER : QUEST_DEADLY_DRAGONMAW, me);
                gurgthock->AI()->Talk(IsHordeQuest() ? TALK_WILDHUMMER_DEFEAT : TALK_DRAGONMAW_DEFEAT, champion);
            }

            gurgthock->AI()->Reset();
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    private:
        bool IsHordeQuest()
        {
            return me->GetEntry() != NPC_TORG_DRAKEFLAYER;
        }
};

// Calder`s Creation 46947
struct npc_twilight_calders_creation : public customCreatureAI
{
    npc_twilight_calders_creation(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    bool hasOutro;

    void InitializeAI() OVERRIDE
    {
        Reset();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

        Movement::MoveSplineInit init(me);
        for (auto&& itr : cadaverPath)
            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

        init.Launch();
        init.SetSmooth();
        init.SetWalk(true);

        scheduler
            .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
        {
            Talk(TALK_ABOMINATION_SELF_INTRO);

            scheduler
                .Schedule(Milliseconds(1500), [this](TaskContext context)
            {
                me->SetHomePosition(*me);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);
            });

            // Special Check - if we not pulled in 15s - remove from world for not spam
            scheduler
                .Schedule(Milliseconds(16 * IN_MILLISECONDS), [this](TaskContext context)
            {
                if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
                    gurgthock->AI()->Reset();

                me->DisappearAndDie();
            });
        });
    }

    void Reset() OVERRIDE
    {
        scheduler.CancelAll();
        hasOutro = false;
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summons.Summon(summon);

        if (Unit* target = me->GetVictim())
            summon->SetInCombatWith(target);

        summon->CastSpell(summon, SPELL_POISON_CLOUD_TRIGGERING, true);
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        scheduler.CancelAll();

        scheduler
            .SetValidator([this] { return !me->HasUnitState(UNIT_STATE_CASTING); })
            .Schedule(Seconds(7), [this](TaskContext context)
        {
            Talk(TALK_ABOMINATION_SPELL);

            if (Unit* target = me->GetVictim())
                DoCast(target, SPELL_REPULSIVE_KICK);

            context.Repeat(Seconds(20), Seconds(22));
        });

        scheduler
            .SetValidator([this] { return !me->HasUnitState(UNIT_STATE_CASTING); })
            .Schedule(Seconds(6), [this](TaskContext context)
        {
            DoCast(me, SPELL_POISON_CLOUD);
            context.Repeat(Seconds(10));
        });

        scheduler
            .SetValidator([this] { return !me->HasUnitState(UNIT_STATE_CASTING); })
            .Schedule(Seconds(12), [this](TaskContext context)
        {
            if (Unit* target = me->GetVictim())
                DoCast(target, SPELL_BELCH);

            context.Repeat(Seconds(20));
        });
    }

    void EnterEvadeMode() OVERRIDE
    {
        if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
            gurgthock->AI()->Reset();

        ScriptedAI::EnterEvadeMode();
        summons.DespawnAll();
        me->DisappearAndDie();
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (hasOutro)
            damage = 0;

        if (damage >= me->GetHealth() && !hasOutro)
        {
            hasOutro = true;
            damage = 0;
            me->PrepareChanneledCast(me->GetOrientation());
            scheduler.CancelAll();

            if (Creature* calder = me->FindNearestCreature(NPC_CALDER_GRAY, 150.0f, true))
                calder->AI()->Talk(TALK_COLDER_ULTIMATE);

            scheduler
                .Schedule(Seconds(2), [this](TaskContext context)
            {
                Talk(TALK_ABOMINATION_OUTRO_1);
                DoCast(me, SPELL_INHALE);

                scheduler
                    .Schedule(Seconds(7), [this](TaskContext context)
                {
                    Talk(TALK_ABOMINATION_OUTRO_2);

                    // Fly into air!
                    me->HandleEmoteStateCommand(EMOTE_STATE_DROWNED);
                    me->OverrideInhabitType(INHABIT_AIR);
                    me->UpdateMovementFlags();

                    scheduler
                        .Schedule(Seconds(1), [this](TaskContext context)
                    {
                        me->SetWalk(true);
                        me->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 20.0f);

                        scheduler
                            .Schedule(Milliseconds(me->GetSplineDuration() + 1000), [this](TaskContext context)
                        {
                            DoCast(me, SPELL_PLAGUE_EXPLOSION);

                            scheduler
                                .Schedule(Seconds(2), [this](TaskContext context)
                            {
                                // Complete event
                                summons.DespawnAll();

                                if (Creature* calder = me->FindNearestCreature(NPC_CALDER_GRAY, 150.0f, true))
                                    calder->AI()->Talk(TALK_COLDER_OUTRO_1);

                                if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
                                {
                                    if (Player* champion = ObjectAccessor::GetPlayer(*me, gurgthock->AI()->GetGUID()))
                                        champion->GroupEventHappens(QUEST_CALDERS_CREATION, me);

                                    gurgthock->AI()->Reset();
                                    gurgthock->AI()->Talk(TALK_ABOMINATION_DEFEAT_1);
                                    gurgthock->AI()->DoAction(ACTION_CADAVER_OUTRO);
                                }

                                me->DisappearAndDie();
                            });
                        });
                    });

                });
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

// Lord Tulvan 46948
struct npc_twilight_lord_tulvan : public customCreatureAI
{
    npc_twilight_lord_tulvan(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    bool firstEnrage;
    bool secondEnrage;

    void InitializeAI() OVERRIDE
    {
        Reset();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

        float x, y, z;
        GetPositionWithDistInOrientation(me, 50.0f, me->GetOrientation(), x, y);
        z = me->GetPositionZ();
        me->UpdateAllowedPositionZ(x, y, z);
        me->GetMotionMaster()->MovePoint(0, x, y, z);

        scheduler
            .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
        {
            Talk(TALK_LORD_SELF_INTRO);
            me->SetHomePosition(*me);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

            // Special Check - if we not pulled in 15s - remove from world for not spam
            scheduler
                .Schedule(Milliseconds(15 * IN_MILLISECONDS), [this](TaskContext context)
            {
                if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
                    gurgthock->AI()->Reset();

                me->DisappearAndDie();
            });
        });
    }

    void Reset() OVERRIDE
    {
        scheduler.CancelAll();
        firstEnrage  = false;
        secondEnrage = false;
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        scheduler.CancelAll();

        scheduler
            .Schedule(Seconds(8), [this](TaskContext context)
        {
            DoCast(me, SPELL_DEATH_BY_PEASANT);
            context.Repeat(Seconds(21));
        });

    }

    void EnterEvadeMode() OVERRIDE
    {
        if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
            gurgthock->AI()->Reset();

        ScriptedAI::EnterEvadeMode();
        summons.DespawnAll();
        me->DisappearAndDie();
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(60) && !firstEnrage)
        {
            firstEnrage = true;
            scheduler.CancelAll();
            Talk(TALK_LORD_WOLF);
            DoCast(me, SPELL_WORGEN_TRANSFORM, true);

            scheduler
                .Schedule(Seconds(4), [this](TaskContext context)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_UNDYING_FRENZY);

                context.Repeat(Seconds(17));
            });
        }

        if (HealthBelowPct(40) && !secondEnrage)
        {
            secondEnrage = true;
            Talk(TALK_LORD_ENRAGE);

            scheduler
                .SetValidator([this] { return !me->HasUnitState(UNIT_STATE_CASTING); })
                .Schedule(Seconds(7), [this](TaskContext context)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_UPPERCUT);

                context.Repeat(Seconds(6));
            });
        }
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summons.Summon(summon);

        if (Unit* target = me->GetVictim())
            summon->SetInCombatWith(target);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
        {
            if (Player* champion = ObjectAccessor::GetPlayer(*me, gurgthock->AI()->GetGUID()))
            {
                champion->GroupEventHappens(QUEST_EARL_OF_EVILSCERATION, me);
                gurgthock->AI()->Talk(TALK_LORD_DEFEAT_1, champion);
                gurgthock->AI()->Talk(ACTION_LORD_OUTRO);
            }

            gurgthock->AI()->Reset();
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

// Emberscar Devourer 46949
struct npc_twilight_emberscar_devourer : public customCreatureAI
{
    npc_twilight_emberscar_devourer(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    bool hasEnrage;

    void InitializeAI() OVERRIDE
    {
        Reset();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

        float x, y, z;
        GetPositionWithDistInOrientation(me, 50.0f, me->GetOrientation(), x, y);
        z = me->GetPositionZ();
        me->UpdateAllowedPositionZ(x, y, z);
        me->GetMotionMaster()->MovePoint(0, x, y, z);

        scheduler
            .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
        {
            Talk(TALK_TWILIGHT_TERROR_SELF_INTRO);
            me->SetHomePosition(*me);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

            // Special Check - if we not pulled in 15s - remove from world for not spam
            scheduler
                .Schedule(Milliseconds(15 * IN_MILLISECONDS), [this](TaskContext context)
            {
                if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
                    gurgthock->AI()->Reset();

                me->DisappearAndDie();
            });
        });
    }

    void Reset() OVERRIDE
    {
        scheduler.CancelAll();
        hasEnrage = false;
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        scheduler.CancelAll();

        scheduler
            .SetValidator([this] { return !me->HasUnitState(UNIT_STATE_CASTING); })
            .Schedule(Seconds(1), [this](TaskContext context)
        {
            if (Unit* target = me->GetVictim())
                DoCast(target, SPELL_FIREBALL);

            context.Repeat(Seconds(3));
        });

        scheduler
            .SetValidator([this] { return !me->HasUnitState(UNIT_STATE_CASTING); })
            .Schedule(Seconds(20), [this](TaskContext context)
        {
            Talk(TALK_TWILIGHT_TERROR_SPELL_LIQUID);
            DoCast(me, SPELL_ACTIVATE_POOLS);
            context.Repeat(Seconds(35));
        });

    }

    void EnterEvadeMode() OVERRIDE
    {
        if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
            gurgthock->AI()->Reset();

        ScriptedAI::EnterEvadeMode();
        summons.DespawnAll();
        me->DisappearAndDie();
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(30) && !hasEnrage)
        {
            hasEnrage = true;
            Talk(TALK_TWILIGHT_TERROR_SKY);

            for (auto&& ref : me->getThreatManager().GetUnsortedThreatList())
                if (Player* target = ObjectAccessor::GetPlayer(*me, ref->GetVictim()->GetGUID()))
                    target->CastSpell(target, SPELL_VOLCANIC_SKYBOX, true);
        }
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (Creature* gurgthock = me->FindNearestCreature(NPC_GURGTHOCK, 150.0f, true))
        {
            if (Player* champion = ObjectAccessor::GetPlayer(*me, gurgthock->AI()->GetGUID()))
            {
                champion->GroupEventHappens(QUEST_TWILIGHT_TERROR, me);
                gurgthock->AI()->Talk(TALK_TWILIGHT_TERROR_DEFEAT, champion);
            }

            gurgthock->AI()->Reset();
        }

        // Remove Sky Aura from Players
        std::list<Player*> targets;
        GetPlayerListInGrid(targets, me, 200.0f);

        for (auto&& itr : targets)
            itr->RemoveAurasDueToSpell(SPELL_VOLCANIC_SKYBOX);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// Firebeads Patrol 46849
struct npc_twilight_firebeads_patrol : public customCreatureAI
{
    npc_twilight_firebeads_patrol(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    bool inRestore;

    void Reset() OVERRIDE
    {
        scheduler.CancelAll();
        inRestore = false;
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO && !inRestore)
        {
            inRestore = true;

            scheduler
                .Schedule(Seconds(20), [this](TaskContext context)
            {
                inRestore = false;
                DoCast(me, SPELL_BUILDING_FIRE);
            });
        }
    }
    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Activate Pools 90409
class spell_twilight_activate_pools : public SpellScript
{
    PrepareSpellScript(spell_twilight_activate_pools);

    void HandleHit(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
            target->CastSpell(target, SPELL_LAVA_POOL, true);
    }

    void SelectTargets(std::list<WorldObject*>&targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != NPC_POOL_OF_LAVA; });
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_twilight_activate_pools::HandleHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_twilight_activate_pools::SelectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
    }
};

// Douse Fire 87250
class spell_twilight_douse_fire : public SpellScript
{
    PrepareSpellScript(spell_twilight_douse_fire);

    void HandleHit(SpellEffIndex effIndex)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (Creature* target = GetHitCreature())
                caster->KilledMonsterCredit(target->GetEntry());
    }

    void HandleAfterCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (caster->GetQuestStatus(QUEST_ONLY_HOMES_WE_HAVE) == QUEST_STATUS_INCOMPLETE)
                caster->AddItem(62508, 1);
    }

    void SelectTargets(std::list<WorldObject*>&targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != NPC_FIREBEADS_PATROL || target->ToCreature() && target->ToCreature()->GetEntry()== NPC_FIREBEADS_PATROL && !target->ToCreature()->HasAura(SPELL_BUILDING_FIRE); });
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_twilight_douse_fire::HandleHit, EFFECT_0, SPELL_EFFECT_REMOVE_AURA);
        AfterCast += SpellCastFn(spell_twilight_douse_fire::HandleAfterCast);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_twilight_douse_fire::SelectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
    }
};

// 206195 Thundermar Ale Keg
class gob_thundermar_ale_keg : public GameObjectScript
{
    public:
        gob_thundermar_ale_keg() : GameObjectScript("gob_thundermar_ale_keg") { }

        bool OnReportUse(Player* player, GameObject* go) OVERRIDE
        {
            if (player->GetQuestStatus(QUEST_TOTAL_WAR) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(46551);
                go->ForcedDespawn();
                return true;
            }

            go->CastSpell(player, 86855);
            return true;
        }
};
 
void AddSC_twilight_highlands()
{
    new creature_script<npc_dunwald_victim>("npc_dunwald_victim");
    new npc_quest_mullan_gryphon();
    new npc_gurgthock_twilands();

    new creature_script<npc_twilight_hurp_derp>("npc_twilight_hurp_derp");
    new creature_script<npc_twilight_faction_champions>("npc_twilight_faction_champions");
    new creature_script<npc_twilight_calders_creation>("npc_twilight_calders_creation");
    new creature_script<npc_twilight_lord_tulvan>("npc_twilight_lord_tulvan");
    new creature_script<npc_twilight_emberscar_devourer>("npc_twilight_emberscar_devourer");
    new creature_script<npc_twilight_firebeads_patrol>("npc_twilight_firebeads_patrol");

    new GenericSpellScriptLoader<spell_twilight_activate_pools>("spell_twilight_activate_pools");
    new GenericSpellScriptLoader<spell_twilight_douse_fire>("spell_twilight_douse_fire");
    new gob_thundermar_ale_keg();
}
