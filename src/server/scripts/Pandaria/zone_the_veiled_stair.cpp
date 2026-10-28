/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "AchievementMgr.h"

#define TONG_MAX_WP 3
#define WRATHION_MAX_WP_Q1 4
#define WRATHION_MAX_WP_Q4 7
#define FEL_METEORS_RANGE 6.0f
#define SHA_TOUCHED_GEM_RANGE 10.0f
#define WRATHION_GOSSIP "Let`s talk..."

// Legend in the making
const Position ScenarioPoint  = { 826.33f, -169.69f, 415.26f, 2.61f };
const Position TongSpawnPos   = { 823.455f, -175.147f, 415.223f, 1.398050f };
const Position TongChestPos   = { 829.24f, -172.20f, 415.21f, 6.22f };
const Position JewelChestPos  = { 820.62f, -174.36f, 416.46f, 0.98f };
const Position jadeWrathion   = { 898.09f, -2648.22f, 184.40f, 4.38f };
const Position suenWrathion   = { 3812.04f, 528.16f, 639.00f, 6.20f };
const Position niuzaoWrathion = { 2277.24f, 4690.68f, 146.31f, 5.29f };
const Position yulonWrathion  = { 897.09f, -2648.51f, 184.40f, 4.37f };
const Position xuenGongPos    = { 3826.55f, 550.48f, 639.83f, 3.71f };
const Position xuenStartChall = { 3795.54f, 533.83f, 639.00f, 0.0f };

const Position WrathionWaypoints_q1[WRATHION_MAX_WP_Q1]
{
    { 827.70f, -170.40f, 415.21f, 2.72f },
    { 828.69f, -170.89f, 415.21f, 2.72f },
    { 823.51f, -168.19f, 415.21f, 2.72f },
    { 821.10f, -164.41f, 415.21f, 4.16f },
};
const Position TongPathToTable[TONG_MAX_WP]
{
    { 824.18f, -174.19f, 415.22f, 2.58f },
    { 821.39f, -172.67f, 415.22f, 2.64f },
    { 821.14f, -173.19f, 415.22f, 4.27f }
};

const Position TongPathToWrathion[TONG_MAX_WP - 1]
{
    { 825.94f, -172.69f, 415.26f, 6.16f },
    { 826.78f, -171.72f, 415.21f, 1.09f },
};

// Breath of the Black Prince
const Position WrathionWaypoints_q4[WRATHION_MAX_WP_Q4]
{
    { 765.68f, -521.95f, 451.46f, 5.48f },
    { 767.85f, -523.97f, 450.70f, 5.39f },
    { 769.61f, -526.42f, 452.58f, 5.34f },
    { 771.23f, -529.14f, 452.36f, 5.25f },
    { 772.64f, -532.15f, 453.69f, 5.14f },
    { 775.47f, -542.00f, 453.71f, 4.93f },
    { 777.64f, -538.04f, 454.06f, 0.64f }
};

const Position wrathionCranePath[7] =
{
    { -1902.27f, 1062.56f,  -4.70f, 1.38f },
    { -1900.03f, 1070.69f,  -9.29f, 1.20f },
    { -1887.61f, 1076.62f,  -9.34f, 0.11f },
    { -1867.02f, 1070.60f, -23.02f, 5.96f },
    { -1853.49f, 1065.84f, -23.57f, 5.96f },
    { -1842.57f, 1062.57f, -31.04f, 5.93f },
    { -1813.73f, 1054.70f, -31.72f, 6.01f },
};

const Position wrathionMassonsPath[3] =
{
    { 754.99f, -515.44f, 447.17f, 5.46f },
    { 773.62f, -530.83f, 453.79f, 5.59f },
    { 777.57f, -538.06f, 454.06f, 2.29f },
};

const Position willyPath[5] =
{
    { 794.75f, -324.72f, 404.02f, 0.0f },
    { 799.79f, -321.45f, 400.78f, 0.0f },
    { 815.12f, -332.57f, 400.78f, 0.0f },
    { 813.91f, -342.96f, 400.78f, 0.0f },
    { 806.91f, -349.23f, 400.78f, 0.0f },
};

enum eCreature
{
    NPC_WRATHION                                         = 64616,
    NPC_GLOBE_OF_AZEROTH                                 = 64814,
    NPC_TONG_THE_FIXER                                   = 64821,
    NPC_WRATHION_Q_01                                    = 64812, // Legend in the making
    NPC_WRATHION_Q_04                                    = 64822, // Breath of the Black Prince
    NPC_YOUR_DRINK                                       = 64817,
    NPC_ALLIANCE_BANNER                                  = 64818,
    NPC_HORDE_BANNER                                     = 64819,
    NPC_TIMELESS_ORB                                     = 54237,
    NPC_FX_BUNNY                                         = 64820, // use fel meteors to azeroth
    NPC_FX_SPELL_BUNNY                                   = 64823,
    NPC_WRATHION_KILLCREDIT_BREATH_OF_THE_BLACK_PRINCE_1 = 66586, // Travelled to Mason`s Folly
    NPC_WRATHION_KILLCREDIT_BREATH_OF_THE_BLACK_PRINCE_2 = 64664,
    NPC_WRATHION_KILLCREDIT_LEGEND_IN_THE_MAKING         = 64649,
    NPC_WRATHION_KILLCREDIT_MEASURE_OF_THE_LEADER        = 68316,
    NPC_WRATHION_KILLCREDIT_FINALE                       = 69053,
    NPC_WRATHION_NAVAL_FLEET_CREDIT                      = 64681,

    // 5.2
    NPC_WRATHION_UPSTAIR                                 = 69782,
    NPC_WRATHION_THUNDER_FORGE                           = 70100,
    NPC_WRATHION_END_MASSONS_FOLLY                       = 70476,
    NPC_UNLOCK_FORGE_KILLCREDIT                          = 70093,
    NPC_COMPLETE_SCENARIO_KILLCREDIT                     = 70094,
    NPC_UNLOCK_FORGE_CREDIT                              = 70093,

    // 5.3
    NPC_CHI_JI_CELESTIAL                                 = 71169,
    NPC_NIUZAO_CELESTIAL                                 = 71144,
    NPC_XUEN_CELESTIAL                                   = 71107,
    NPC_WRATHION_XUEN                                    = 71108,
    NPC_WRATHION_TEMPLE_OF_JADE_SERPENT                  = 71352,
    NPC_YULONG_CREDIT                                    = 70924,
    NPC_CHIJI_CREDIT                                     = 70926,
    NPC_XUEN_CREDIT                                      = 70927,
    NPC_NIUZAO_CREDIT                                    = 70928,
    NPC_CHALLENGE_CREDIT                                 = 70931,
    NPC_XUEN_GONG                                        = 71109,
    NPC_WRATHION_XUEN_CHALLENGE                          = 71037,
    NPC_NELTHARIONS_TEAR                                 = 71041,
    NPC_WRATHION_CLOAK_ACHIEVE                           = 71426,
    NPC_JADE_SERPENT_SPIRIT                              = 71435,
    NPC_BLACK_OX_SPIRIT                                  = 71439,
    NPC_RED_CRANE_SPIRIT                                 = 71441,
    NPC_WHITE_TIGER_SPIRIT                               = 71434,
    NPC_WHITE_TIGER_TEMPLE                               = 64528,

    // 5.4
    NPC_WRATHION_MEET_SEAT_OF_KNOWLEDGE_CREDIT           = 72723,
    NPC_LISTEN_YOUR_STORY_CREDIT                         = 61962,
};

enum eQuest
{
    QUEST_LEGEND_IN_THE_MAKING           = 31454,
    QUEST_BREATH_OF_THE_BLACK_PRINCE     = 31482,
    QUEST_INCOMING                       = 31483,
    QUEST_MEASURE_OF_THE_LEADER_A        = 32373,
    QUEST_MEASURE_OF_THE_LEADER_H        = 32427,
    QUEST_CALL_THE_PACKMASTER            = 32390,
    QUEST_SOUL_OF_THE_HORDE              = 32432,
    QUEST_TEST_OF_VALOR_A                = 32474,
    QUEST_TEST_OF_VALOR_H                = 32476,
    
    // Landing
    QUEST_LIONS_LANDING                  = 32109,
    QUEST_DOMINATION_POINT               = 32108,
    QUEST_GLORY_TO_THE_HORDE             = 32431,
    QUEST_THE_LION_ROARS                 = 32389,

    // Thunder King
    QUEST_MEET_ME_UPSTAIRS               = 32590,
    QUEST_THUNDER_FORGE                  = 32593,
    QUEST_CROWN_OF_THE_HEAVEN            = 32595,

    // Celesial Blessing
    QUEST_CELESTIAL_BLESSING             = 32805,
    QUEST_CLOAK_OF_VIRTUE                = 32861,

    // Judgment of Black Prince
    QUEST_A_PANDAREN_LEGEND              = 33104,

    // Misc
    ITEM_TITAN_RUNESTONE                 = 94594,
    ITEM_JADEFIRE_DRAPE                  = 98150, // range dps cloak
    ITEM_CRANEWING_CLOAK                 = 98149, // healer
    ITEM_OXHORN_BLADEBREAKER             = 98146, // tank (not agility)
    ITEM_TIGERFANG_WRAP                  = 98148, // dps (agility)
    ITEM_OXHOOF_GREATCLOAK               = 98335, // tank (agility)
    ITEM_TIGERCLAW_CAPE                  = 98147, // dps (not agility)

    // Legendary
    ITEM_XING_XO                         = 102246, // range dps cloak
    ITEM_JINA_KANG                       = 102247, // healer
    ITEM_FEN_YU                          = 102248, // dps (agility)
    ITEM_GONG_LU                         = 102249, // dps (not agility)
    ITEM_QIAN_YING                       = 102250, // tank (not agility)
    ITEM_QIAN_LE                         = 102245, // tank (agility)
};

const std::map<uint32, uint32> invCloakType =
{
    { ITEM_JADEFIRE_DRAPE,      ITEM_XING_XO   },
    { ITEM_CRANEWING_CLOAK,     ITEM_JINA_KANG },
    { ITEM_OXHORN_BLADEBREAKER, ITEM_QIAN_YING },
    { ITEM_TIGERFANG_WRAP,      ITEM_FEN_YU    },
    { ITEM_OXHOOF_GREATCLOAK,   ITEM_QIAN_LE   },
    { ITEM_TIGERCLAW_CAPE,      ITEM_GONG_LU   },
};

enum eGameObjects
{
    GO_WRATHION_JEWEL_CHEST = 216837,
};

enum eModelId
{
    MODEL_WRATHION_DRAGON    = 397,
};

enum eScenes
{
    SCENE_LEGEND_IN_THE_MAKING_A      = 71,
    SCENE_LEGEND_IN_THE_MAKING_H      = 86,
    SCENE_BREATH_OF_THE_BLACK_PRINCE  = 93,
    SCENE_MEASURE_OF_A_LEADER_A       = 152,
    SCENE_MEASURE_OF_A_LEADER_H       = 156,
    SCENE_CALL_THE_PACKMASTER         = 157, // Alliance
    SCENE_SOUL_OF_THE_HORDE           = 158, // Horde
};

enum Spells
{
    // A legend in the making
    SPELL_SUMMON_TONG                      = 126673, // by wrathion
    SPELL_HOLDING_A_TRAY                   = 126680, // tong takes the tray from the table
    SPELL_HOLDING_A_WRATHION_CUP           = 126682, // just added cup into tray
    SPELL_TINY_FEL_METEORS                 = 126694, // must hit azeroth plane
    SPELL_AZEROTH_EXPLOSION                = 126689,

    // This possible for quest where wrathion generate sha crystall power (gem powered sha)
    SPELL_FEAR_BOLT                        = 126720,
    SPELL_FEAR_ENERGY                      = 130942,
    SPELL_WISDOM_FX_01                     = 126718,
    SPELL_WISDOM_FX_02                     = 126719,
    SPELL_COSMETIC_FLAME                   = 130950, // used on gem
    SPELL_DEATH_AND_DECAY                  = 126723,
    SPELL_GROWTH                           = 126622,
                                           
    // 5.1                                 
    SPELL_LIONS_LANDING_CINEMATIC          = 135953,
    SPELL_DOMINATION_POINT_CINEMATIC       = 135955,

    // 5.2
    SPELL_SEE_ANDUIN_AT_MASSONS_FOLLY      = 139989,
    SPELL_MASSONS_FOLLY_FINAL_EVENT        = 139999, // crown of the heaven
    SPELL_SUMMON_WRATHION                  = 140019,
    SPELL_EVENT_CLEANUP                    = 140001,

    // 5.3
    SPELL_YULON_SCENE                      = 141313,
    SPELL_SUMMON_WRATHION_OX_TEMPLE        = 141745,
    SPELL_SUMMON_WRATHION_CRANE_TEMPLE     = 141809,
    SPELL_SUMMON_WRATHION_TIGER_TEMPLE     = 141646,
    SPELL_SUMMON_WRATHION_XUEN_CHALLENGE   = 141860,
    SPELL_BLINDSIDE                        = 141623,
    SPELL_BLINDSIDE_EFF                    = 141556,
    SPELL_XUEN_CHALLENGE                   = 141666,
    SPELL_CRUMBLING_ARC                    = 141653,
    SPELL_REIGN_OF_FIRE                    = 141510,
    SPELL_REIGN_OF_FIRE_MISSLE             = 141511,
    SPELL_NELTHARIONS_TEAR                 = 141483,
    SPELL_BLOOD_OF_DEATHBORN               = 141466,
    SPELL_BLOOD_OF_DEATHBORN_MISSLE        = 141500,
    SPELL_BLOOD_OF_DEATHBORN_EFF           = 141469,
    SPELL_NELTHRAIONS_TEAR_BEAM            = 141505,
    SPELL_NELTHARIONS_TEAR_SUMM            = 143303,
    SPELL_CHALLENGE_DONE_CREDIT            = 142213,
    SPELL_SUMMON_WRATHION_CELESTIAL_END    = 142652,
    SPELL_CLOACK_CREATION_STATE            = 142743,
    SPELL_CLOACK_CELESTIAL_SPHERE          = 142763,
    SPELL_CLOACK_SCENE                     = 142541,
    SPELL_SUMMON_JADE_SERPENT              = 142685,
    SPELL_SUMMON_BLACK_OX                  = 142712,
    SPELL_SUMMON_RED_CRANE                 = 142716,
    SPELL_SUMMON_TIGER                     = 142683,
    SPELL_RESSURECTION                     = 24341,

    // 5.4
    SPELL_LEGENDARY_FINALE                 = 146397,
    SPELL_WRATHION_ESSENCE                 = 146428,
                                           
    // Misc                                
    SPELL_LEGEND_IN_THE_MAKING_SCENE_A     = 126670,
    SPELL_LEGEND_IN_THE_MAKING_SCENE_H     = 126671,
    SPELL_MASSONS_FOLLY_FINALE_EVENT       = 126713,
    SPELL_MEASURE_OF_THE_LEADER_HORDE      = 135795,
    SPELL_MEASURE_OF_THE_LEADER_ALLIANCE   = 135794,
    SPELL_CALL_THE_PACKMASTER_SCENE_PLAY   = 135805,
    SPELL_SOUL_OF_THE_HORDE_SCENE_PLAY     = 135806,
    SPELL_KILL_CREDIT_FINALE_5_1           = 135802,
    SPELL_KILL_CREDIT_LEGEND_IN_THE_MAKING = 126698,
    SPELL_KILL_CREDIT_BREATH_OF_PRINCE     = 126726,
    SPELL_KILL_CREDIT_MASSONS_FOLLY        = 126714,
    SPELL_KILL_CREDIT_MEASURE_OF_THE_LEAD  = 135792,
    SPELL_SHADOWBOLT                       = 9613,
    SPELL_LEECHING_FINGERS                 = 125290,
    SPELL_SHOT                             = 6660,
    SPELL_VILE_SPITTLE                     = 124951,
    SPELL_ACID_SPIT                        = 9591,
    SPELL_ENRAGE                           = 8599,
    SPELL_RUSHING_CHARGE                   = 6268,
    SPELL_FLURRY                           = 124977,
    NPC_CLOAK_ACHIEVE_CREDIT               = 71300,
    SPELL_VIOLETS_AND_BLUEBERRIES          = 139031,
    SPELL_VIOLETS_AND_BLUEBERRIES_EFF      = 139033,
    SPELL_STUFFED                          = 139037,

    // Phase Spells
    SPELL_SEE_WRATHION_IN_TAVERN           = 129289, // before take 32590
    SPELL_SEE_WRATHION_UPSTAIRS            = 139244, // on take 32590
    SPELL_DEFENCE_LOCATION                 = 125041, // phase 7
    SPELL_SPIDER_INVISIBILITY              = 82358,  // Phase 14
};

enum Actions
{
    ACTION_LEGEND_IN_THE_MAKING_INTRO = 0,
    ACTION_TONG_MAKE_DRINKS           = 1,
    ACTION_WRATHION_TAKE_CUP          = 2,
    ACTION_TONG_GO_AWAY               = 3,
    ACTION_POWER                      = 4,
    ACTION_WISDOM                     = 5,
    ACTION_TERROR                     = 6,
    ACTION_REIGN_OF_FIRE,
    ACTION_START_XUEN_CHALLENGE,
    ACTION_START_INTRO,
};

enum Events
{
    // A legend in the making
    EVENT_TONG_DRINKS         = 1,
    EVENT_TONG_MAKE_DRINGS    = 2,
    EVENT_TONG_SHALL_DRINKS   = 3,
    EVENT_TONG_MOVE_HOME      = 4,
    EVENT_TONG_TAKE_A_CHEST   = 5,
    EVENT_TONG_MOVE_TO_CHEST  = 6,
    EVENT_TONG_PUT_A_CHEST    = 7,
    EVENT_TONG_CHEST_ON_TABLE = 8,
    EVENT_TONG_MOVE_TO_TABLE  = 9,

    // Breath of Black Princwe
    EVENT_LOOK_AT_PANDARIA    = 1,
    EVENT_POWER               = 2,
    EVENT_WISDOM              = 4,
    EVENT_TERROR              = 5,
    EVENT_BREATH_TO_GEM       = 6,
    EVENT_GIFT_FROM_ME        = 7,
    EVENT_WORK_IS_DONE        = 8,
    EVENT_ESCORT              = 9,

    // Misc
    EVENT_LEECHING_FINGERS    = 1,
    EVENT_SHADOWBOLT,
    EVENT_SHOT,
    EVENT_ACID_SPIT,
    EVENT_VILE_SPIT,
    EVENT_RUSHING_CHARGE,
    EVENT_FLURRY,
    EVENT_BLOOD_OF_DEATHBORN,
    EVENT_ARC,
    EVENT_REIGN_OF_FIRE,
    EVENT_VIOLET_AND_BLUEBERRIES,
};

enum Yells
{
    // A legend in the making
    TALK_INTRO                 = 0,
    TALK_TONG_DRINKS           = 1,
    TALK_ABOUT_DEATHWING_1     = 2,
    TALK_ABOUT_DEATHWING_2     = 3,
    TALK_AZEROTH_SCENE_1       = 4,
    TALK_AZEROTH_SCENE_2       = 5,
    TALK_TONG_HOLDING_CUP      = 6,
    TALK_WRATHION_OPINION      = 7,
    TALK_ABOUT_GARROSH         = 8,
    TALK_AZEROTH_SCENE_3       = 9,
    TALK_AZEROTH_SCENE_4       = 10,
    TALK_ABOUT_WAR             = 11,
    TALK_FACTION_H             = 12,
    TALK_FACTION_A             = 13,
    TALK_ABOUT_CONFLICT        = 14,
    TALK_ABOUT_GEARED          = 15,

    // Breath of the Black Prince
    TALK_INTRO_VENTURE         = 0,
    TALK_ABOUT_PANDARIA        = 1,
    TALK_ABOUT_PROTECT_AZEROTH = 2,
    TALK_GREAT_THINGS          = 3,
    TALK_POWER_OF_ENEMIES      = 4,
    TALK_WISDOM_OF_ENEMIES     = 5,
    TALK_TERROR_OF_ENEMIES     = 6,
    TALK_BREATH_TO_GEM         = 7,
    TALK_WORK_IS_DONE          = 8,

    // Default
    TALK_SPECIAL_1             = 1,
    TALK_SPECIAL_2,
    TALK_SPECIAL_3,
    TALK_SPECIAL_4,
    TALK_SPECIAL_5,
    TALK_SPECIAL_6,
    TALK_SPECIAL_7,
    TALK_SPECIAL_8,
    TALK_SPECIAL_9,
    TALK_SPECIAL_10,
};

enum ePhases
{
    PHASE_MAKING_LEGEND            = 4,
    PHASE_BREATH_OF_BLACK_PRINCE   = 6,
    PHASE_MEASURE_OF_A_LEADER      = 8,
    PHASE_GIFT_OF_THE_BLACK_PRINCE = 10,
};

const std::map<uint32, uint32> invZoneChallengeType = 
{
    { 5785, SPELL_YULON_SCENE                  },
    { 5841, SPELL_SUMMON_WRATHION_TIGER_TEMPLE },
    { 6134, SPELL_SUMMON_WRATHION_CRANE_TEMPLE },
    { 5842, SPELL_SUMMON_WRATHION_OX_TEMPLE    },
};

// Wrathion 64616
class npc_wrathion : public CreatureScript
{
    public:
        npc_wrathion() : CreatureScript("npc_wrathion") { }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
        {
            if (player->GetQuestStatus(QUEST_CALL_THE_PACKMASTER) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_SOUL_OF_THE_HORDE) == QUEST_STATUS_INCOMPLETE)
                player->CastSpell(player, player->GetTeam() == HORDE ? SPELL_SOUL_OF_THE_HORDE_SCENE_PLAY : SPELL_CALL_THE_PACKMASTER_SCENE_PLAY, true);

            // temp hack, i don`t know why OnGossipHello now doesn`t work
            switch (quest->GetQuestId())
            {
                case QUEST_CALL_THE_PACKMASTER:
                case QUEST_SOUL_OF_THE_HORDE:
                    player->CastSpell(player, player->GetTeam() == HORDE ? SPELL_SOUL_OF_THE_HORDE_SCENE_PLAY : SPELL_CALL_THE_PACKMASTER_SCENE_PLAY, true);

                    if (Creature* tong = player->SummonCreature(NPC_TONG_THE_FIXER, TongSpawnPos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, Milliseconds(2 * MINUTE * IN_MILLISECONDS)))
                        tong->SetPhaseMask(2, true);
                    break;
                case QUEST_LEGEND_IN_THE_MAKING:
                    player->CastSpell(player, player->GetTeam() == HORDE ? SPELL_LEGEND_IN_THE_MAKING_SCENE_H : SPELL_LEGEND_IN_THE_MAKING_SCENE_A, true);

                    if (Creature* tong = player->SummonCreature(NPC_TONG_THE_FIXER, TongSpawnPos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, Milliseconds(2 * MINUTE * IN_MILLISECONDS)))
                        tong->SetPhaseMask(2, true);
                    break;
                case QUEST_MEASURE_OF_THE_LEADER_A:
                case QUEST_MEASURE_OF_THE_LEADER_H:
                    player->CastSpell(player, player->GetTeam() == HORDE ? SPELL_MEASURE_OF_THE_LEADER_HORDE : SPELL_MEASURE_OF_THE_LEADER_ALLIANCE, true);
                    break;
                case QUEST_INCOMING:
                    // Check if player complete landing questline
                    if (player->GetQuestStatus(QUEST_LIONS_LANDING) == QUEST_STATUS_REWARDED || player->GetQuestStatus(QUEST_DOMINATION_POINT) == QUEST_STATUS_REWARDED)
                        if (player->GetQuestStatus(QUEST_INCOMING) == QUEST_STATUS_INCOMPLETE)
                            player->KilledMonsterCredit(NPC_WRATHION_NAVAL_FLEET_CREDIT);
                    break;
                case QUEST_TEST_OF_VALOR_H:
                case QUEST_TEST_OF_VALOR_A:
                    if (player->HasAchieved(quest->GetQuestId() == QUEST_TEST_OF_VALOR_A ? 8030 : 8031))
                        player->KilledMonsterCredit(69145);
                    break;
            }

            return false;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    if (player->GetQuestStatus(QUEST_LEGEND_IN_THE_MAKING) == QUEST_STATUS_INCOMPLETE)
                    {
                        player->CastSpell(player, player->GetTeam() == HORDE ? SPELL_LEGEND_IN_THE_MAKING_SCENE_H : SPELL_LEGEND_IN_THE_MAKING_SCENE_A, true);

                        if (Creature* tong = player->SummonCreature(NPC_TONG_THE_FIXER, TongSpawnPos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, Milliseconds(2 * MINUTE * IN_MILLISECONDS)))
                            tong->SetPhaseMask(2, true);
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    if (player->GetQuestStatus(QUEST_MEASURE_OF_THE_LEADER_A) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_MEASURE_OF_THE_LEADER_H) == QUEST_STATUS_INCOMPLETE)
                        player->CastSpell(player, player->GetTeam() == HORDE ? SPELL_MEASURE_OF_THE_LEADER_HORDE : SPELL_MEASURE_OF_THE_LEADER_ALLIANCE, true);
                    break;
            }

            player->CLOSE_GOSSIP_MENU();

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(QUEST_LEGEND_IN_THE_MAKING) == QUEST_STATUS_INCOMPLETE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, WRATHION_GOSSIP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            if (player->GetQuestStatus(QUEST_MEASURE_OF_THE_LEADER_A) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_MEASURE_OF_THE_LEADER_H) == QUEST_STATUS_INCOMPLETE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, WRATHION_GOSSIP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        struct npc_wrathionAI : public ScriptedAI
        {
            npc_wrathionAI(Creature* creature) : ScriptedAI(creature) { }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_wrathionAI(creature);
        }
};

// Tong the fixer Legend in the making 64821
class npc_tong_the_fixer_legend_in_the_making : public CreatureScript
{
    public:
        npc_tong_the_fixer_legend_in_the_making() : CreatureScript("npc_tong_the_fixer_legend_in_the_making") { }

        struct npc_tong_the_fixer_legend_in_the_makingAI : public ScriptedAI
        {
            npc_tong_the_fixer_legend_in_the_makingAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 delay;

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->SetWalk(true);
                me->SetSpeed(MOVE_WALK, 0.4f);
                delay = 0;

                if (summoner && summoner->ToPlayer())
                {
                    if (summoner->ToPlayer()->GetQuestStatus(QUEST_LEGEND_IN_THE_MAKING) == QUEST_STATUS_INCOMPLETE)
                    {
                        me->m_Events.Schedule(delay += 9000, EVENT_TONG_DRINKS, [this]()
                        {
                            me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                            // move towards the upper platform
                            Movement::MoveSplineInit init(me);
                            for (auto itr : TongPathToTable)
                                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                            init.SetSmooth();
                            init.SetUncompressed();
                            init.Launch();

                            delay = 0;
                            me->m_Events.Schedule(delay += me->GetSplineDuration(), EVENT_TONG_MAKE_DRINGS, [this]()
                            {
                                me->HandleEmoteStateCommand(EMOTE_STATE_USE_STANDING);
                                me->StopMoving();
                            });
                            me->m_Events.Schedule(delay += 29500, EVENT_TONG_SHALL_DRINKS, [this]()
                            {
                                me->HandleEmoteStateCommand(EMOTE_STATE_NONE);
                                DoCast(me, SPELL_HOLDING_A_TRAY);
                                DoCast(me, SPELL_HOLDING_A_WRATHION_CUP);

                                // move towards the upper platform
                                Movement::MoveSplineInit init(me);
                                for (auto itr : TongPathToWrathion)
                                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                                init.SetSmooth();
                                init.SetUncompressed();
                                init.Launch();

                                delay = 0;
                                me->m_Events.Schedule(delay += me->GetSplineDuration() + 2500, EVENT_TONG_MOVE_HOME, [this]()
                                {
                                    me->GetMotionMaster()->MovePoint(0, me->GetHomePosition());
                                });
                            });
                        });
                    }
                    else
                    {
                        uint32 delay = 0;
                        me->m_Events.Schedule(delay += 53000, EVENT_TONG_TAKE_A_CHEST, [this]()
                        {
                            me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                        });

                        me->m_Events.Schedule(delay += 4000, EVENT_TONG_MOVE_TO_CHEST, [this]()
                        {
                            me->GetMotionMaster()->MovePoint(1, TongChestPos);
                        });
                    }
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (pointId)
                {
                    case 0:
                        me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() + M_PI));
                        me->RemoveAurasDueToSpell(SPELL_HOLDING_A_TRAY);
                        me->RemoveAurasDueToSpell(SPELL_HOLDING_A_WRATHION_CUP);
                        break;
                    case 1:
                    {
                        uint32 delay = 0;
                        me->HandleEmoteStateCommand(EMOTE_STATE_USE_STANDING);
                        me->m_Events.Schedule(delay += 5000, EVENT_TONG_CHEST_ON_TABLE, [this]()
                        {
                            me->HandleEmoteStateCommand(EMOTE_STATE_NONE);
                            Movement::MoveSplineInit init(me);
                            for (auto itr : TongPathToTable)
                                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                            init.SetSmooth();
                            init.SetUncompressed();
                            init.Launch();
                        });
                        me->m_Events.Schedule(delay += me->GetSplineDuration(), EVENT_TONG_MOVE_TO_TABLE, [this]()
                        {
                            if (GameObject* goChest = me->SummonGameObject(GO_WRATHION_JEWEL_CHEST, JewelChestPos.GetPositionX(), JewelChestPos.GetPositionY(), JewelChestPos.GetPositionZ(), JewelChestPos.GetOrientation(), { }, 0))
                                goChest->SetPhaseMask(2, true);
                        });
                        break;
                    }
                }
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_tong_the_fixer_legend_in_the_makingAI(creature);
        }
};

// Wrathion breath of the black prince 64822
class npc_wrathion_breath_of_the_black_prince : public CreatureScript
{
    public:
        npc_wrathion_breath_of_the_black_prince() : CreatureScript("npc_wrathion_breath_of_the_black_prince") { }

        struct npc_wrathion_breath_of_the_black_princeAI : public ScriptedAI
        {
            npc_wrathion_breath_of_the_black_princeAI(Creature* creature) : ScriptedAI(creature)
            {
                wp = 1;
            }

            EventMap events, nonCombatEvents;
            uint32 wp;
            uint64 SummonerGUID;

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                SummonerGUID = summoner->GetGUID();
                me->SetWalk(true);
                me->SetSpeed(MOVE_WALK, 0.6f);
                nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));
            }

            void MovementInform(uint32 type, uint32 id) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                wp++;

                switch (id)
                {
                    case 1:
                        Talk(TALK_INTRO_VENTURE);
                        nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));
                        break;
                    case 5:
                        Talk(TALK_ABOUT_PANDARIA);
                        nonCombatEvents.ScheduleEvent(EVENT_LOOK_AT_PANDARIA, 6000);
                        break;
                    case 6:
                        Talk(TALK_GREAT_THINGS);
                        me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() - M_PI / 2));
                        me->CastSpell(me, SPELL_DEATH_AND_DECAY, false);

                        if (Unit* summoner = ObjectAccessor::GetPlayer(*me, SummonerGUID))
                            summoner->CastSpell(summoner, SPELL_MASSONS_FOLLY_FINALE_EVENT, false);

                        nonCombatEvents.ScheduleEvent(EVENT_POWER, 7500);
                        break;
                    default:
                        nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));
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
                            me->GetMotionMaster()->MovementExpired();
                            me->GetMotionMaster()->MovePoint(wp, WrathionWaypoints_q4[wp]);
                            break;
                        case EVENT_LOOK_AT_PANDARIA:
                            Talk(TALK_ABOUT_PROTECT_AZEROTH);
                            nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 4000);
                            break;
                        case EVENT_POWER:
                            Talk(TALK_POWER_OF_ENEMIES);
                            nonCombatEvents.ScheduleEvent(EVENT_WISDOM, 4500);
                            break;
                        case EVENT_WISDOM:
                            Talk(TALK_WISDOM_OF_ENEMIES);
                            nonCombatEvents.ScheduleEvent(EVENT_TERROR, 3500);
                            break;
                        case EVENT_TERROR:
                            Talk(TALK_TERROR_OF_ENEMIES);
                            nonCombatEvents.ScheduleEvent(EVENT_BREATH_TO_GEM, 8500);
                            break;
                        case EVENT_BREATH_TO_GEM:
                            DoCast(me, SPELL_COSMETIC_FLAME);
                            Talk(TALK_BREATH_TO_GEM);
                            nonCombatEvents.ScheduleEvent(EVENT_GIFT_FROM_ME, 9000);
                            break;
                        case EVENT_GIFT_FROM_ME:
                            Talk(TALK_WORK_IS_DONE);
                            me->RemoveAllAuras();
                            nonCombatEvents.ScheduleEvent(EVENT_WORK_IS_DONE, 8000);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_wrathion_breath_of_the_black_princeAI(creature);
        }
};

// Darkhatched Soulbleeder 62928
struct npc_darkhatched_soulbleeder : public ScriptedAI
{
    npc_darkhatched_soulbleeder(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SHADOWBOLT, urand(3500, 5500));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_SHADOWBOLT)
            {
                if (Unit* target = me->GetVictim())
                    DoCast(target, SPELL_SHADOWBOLT);

                events.ScheduleEvent(EVENT_SHADOWBOLT, urand(3500, 6 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Leechfingers 63494
struct npc_leechfingers : public ScriptedAI
{
    npc_leechfingers(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SHADOWBOLT, urand(3500, 5500));
        events.ScheduleEvent(EVENT_LEECHING_FINGERS, 8 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_LEECHING_FINGERS:
                    me->InterruptNonMeleeSpells(true);

                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_LEECHING_FINGERS);

                    events.ScheduleEvent(EVENT_LEECHING_FINGERS, 19 * IN_MILLISECONDS);
                    break;
                case EVENT_SHADOWBOLT:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_SHADOWBOLT);

                    events.ScheduleEvent(EVENT_SHADOWBOLT, urand(3500, 6 * IN_MILLISECONDS));
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Hatescale Spitter 63490
struct npc_hatescale_spitter : public ScriptedAI
{
    npc_hatescale_spitter(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_SHOT, 2 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_ACID_SPIT, urand(3500, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_VILE_SPIT, 8 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_SHOT:
                    if (Unit* target = me->GetVictim())
                        if (target->GetExactDist2d(me) > 8.0f)
                            DoCast(target, SPELL_SHOT);

                    events.ScheduleEvent(EVENT_SHOT, 2 * IN_MILLISECONDS);
                    break;
                case EVENT_ACID_SPIT:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_ACID_SPIT);

                    events.ScheduleEvent(EVENT_ACID_SPIT, urand(3500, 12 * IN_MILLISECONDS));
                    break;
                case EVENT_VILE_SPIT:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_VILE_SPITTLE);

                    events.ScheduleEvent(EVENT_VILE_SPIT, 12 * IN_MILLISECONDS);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Hatescale Ironface 63495
struct npc_hatescale_ironface : public ScriptedAI
{
    npc_hatescale_ironface(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_RUSHING_CHARGE, 500);
        events.ScheduleEvent(EVENT_FLURRY, 10 * IN_MILLISECONDS);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (HealthBelowPct(31) && !me->HasAura(SPELL_ENRAGE))
            DoCast(me, SPELL_ENRAGE);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_RUSHING_CHARGE:
                    if (Unit* target = me->GetVictim())
                        if (me->GetExactDist2d(target) > 6.0f)
                            DoCast(target, SPELL_RUSHING_CHARGE);

                    events.ScheduleEvent(EVENT_RUSHING_CHARGE, urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
                    break;
                case EVENT_FLURRY:
                    DoCast(me, SPELL_FLURRY);
                    events.ScheduleEvent(EVENT_FLURRY, 25 * IN_MILLISECONDS);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Wrathion 69782
class npc_wrathion_thunder_king : public CreatureScript
{
    public:
        npc_wrathion_thunder_king() : CreatureScript("npc_wrathion_thunder_king") { }
    
        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
        {
            if (quest->GetQuestId() == QUEST_THUNDER_FORGE)
                player->KilledMonsterCredit(NPC_UNLOCK_FORGE_CREDIT);
    
            return false;
        }
};

// Image of Wrathion 70516
struct npc_image_of_wrathion : public ScriptedAI
{
    npc_image_of_wrathion(Creature* creature) : ScriptedAI(creature) { }

    uint32 runestoneIndex;
    uint64 ownerGUID;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        ownerGUID = summoner->GetGUID();
        runestoneIndex = DoFindRunestoneIndex(12, ownerGUID);

        if (runestoneIndex >= 1 && runestoneIndex <= 12)
            Talk(runestoneIndex - 1);

        me->DespawnOrUnsummon(15 * IN_MILLISECONDS);
    }

    uint32 DoFindRunestoneIndex(int n, uint64 ownerGUID)
    {
        if (n < 1)
            return 0;

        if (Player* owner = ObjectAccessor::GetPlayer(*me, ownerGUID))
            if (owner->HasItemCount(ITEM_TITAN_RUNESTONE, n))
                return n;

        return DoFindRunestoneIndex(n - 1, ownerGUID);
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

// Wrathion Suen Celesial 71108
struct npc_wrathion_suen_celestial : public ScriptedAI
{
    npc_wrathion_suen_celestial(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 ownerGUID;
    uint32 delay;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (!summoner)
            return;

        delay = 0;
        me->SetPhaseMask(4, true);
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());
        ownerGUID = summoner->GetGUID();

        if (summoner->ToPlayer())
            summoner->ToPlayer()->KilledMonsterCredit(NPC_XUEN_CREDIT);

        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            Talk(TALK_INTRO);
            me->SetWalk(true);
            me->GetMotionMaster()->MovePoint(0, suenWrathion);

            delay = me->GetSplineDuration() + 2500;
            scheduler
                .Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_1);
            });

            scheduler
                .Schedule(Milliseconds(delay += 3000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_2);
            });

            scheduler
                .Schedule(Milliseconds(delay += 4000), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_XUEN_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_INTRO, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 14000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_3);
            });

            scheduler
                .Schedule(Milliseconds(delay += 3800), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_XUEN_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_1, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5800), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_4);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5200), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_XUEN_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_2, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 4600), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_5);
            });

            scheduler
                .Schedule(Milliseconds(delay += 4000), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_XUEN_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_3, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 3200), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_6);
            });

            scheduler
                .Schedule(Milliseconds(delay += 4100), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_XUEN_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_4, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 14500), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_XUEN_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_5, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 11000), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_XUEN_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_6, target);

                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    target->SummonCreature(NPC_XUEN_GONG, xuenGongPos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            });
        });

        // Evade Check
        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                if (me->GetExactDist2d(target) > 90.0f)
                    me->DespawnOrUnsummon();

            context.Repeat(Milliseconds(1000));
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_XUEN_CHALLENGE)
        {
            delay = 2 * IN_MILLISECONDS;
            scheduler
                .Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_XUEN_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_7, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 9400), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_7);
            });

            scheduler
                .Schedule(Milliseconds(delay += 6000), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_XUEN_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_8, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5400), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_8);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5600), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_XUEN_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_9, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 10000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_9);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
            {
                me->GetMotionMaster()->MovePoint(0, xuenStartChall);

                scheduler
                    .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
                {
                    Talk(TALK_SPECIAL_10);
                });

                scheduler
                    .Schedule(Milliseconds(me->GetSplineDuration() + 4500), [this](TaskContext context)
                {
                    if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                        target->CastSpell(target, SPELL_SUMMON_WRATHION_XUEN_CHALLENGE, true);

                    me->DespawnOrUnsummon();
                });
            });
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE 
    {
        scheduler.Update(diff);
    }
};

// Wrathion Chi-Ji Celesial 71166
struct npc_wrathion_chiji_celestial : public ScriptedAI
{
    npc_wrathion_chiji_celestial(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 ownerGUID;
    uint32 delay;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (!summoner)
            return;

        delay = 0;
        me->SetPhaseMask(4, true);
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());
        ownerGUID = summoner->GetGUID();

        if (summoner->ToPlayer())
            summoner->ToPlayer()->KilledMonsterCredit(NPC_CHIJI_CREDIT);

        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            Talk(TALK_INTRO);

            Movement::MoveSplineInit init(me);
            for (auto itr : wrathionCranePath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.SetSmooth();
            init.Launch();

            delay = me->GetSplineDuration() + 2500;
            scheduler
                .Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_1);
            });

            scheduler
                .Schedule(Milliseconds(delay += 3200), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_2);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5400), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_CHI_JI_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_INTRO, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 11000), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_CHI_JI_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_1, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 8000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_3);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5250), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_CHI_JI_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_2, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 6200), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_4);
            });

            scheduler
                .Schedule(Milliseconds(delay += 4400), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_CHI_JI_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_3, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 2800), [this](TaskContext context)
            {
                me->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
                Talk(TALK_SPECIAL_5);
            });

            scheduler
                .Schedule(Milliseconds(delay += 8500), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_6);
            });

            scheduler
                .Schedule(Milliseconds(delay += 10300), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_7);
            });

            scheduler
                .Schedule(Milliseconds(delay += 12500), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_CHI_JI_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_4, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 11000), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_CHI_JI_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_5, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5700), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_CHI_JI_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_6, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 9600), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_8);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5200), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_CHI_JI_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_7, target);
            });
        });

        // Evade Check
        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                if (me->GetExactDist2d(target) > 90.0f)
                    me->DespawnOrUnsummon();

            context.Repeat(Milliseconds(1000));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Wrathion Niuzao Celesial 71142
struct npc_wrathion_niuzao_celestial : public ScriptedAI
{
    npc_wrathion_niuzao_celestial(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 ownerGUID;
    uint32 delay;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (!summoner)
            return;

        delay = 0;
        me->SetPhaseMask(4, true);
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());
        ownerGUID = summoner->GetGUID();

        if (summoner->ToPlayer())
            summoner->ToPlayer()->KilledMonsterCredit(NPC_NIUZAO_CREDIT);

        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            Talk(TALK_INTRO);
        });

        scheduler
            .Schedule(Milliseconds(3500), [this](TaskContext context)
        {
            me->GetMotionMaster()->MovePoint(0, niuzaoWrathion);

            delay = me->GetSplineDuration() + 2500;
            scheduler
                .Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_1);
            });

            scheduler
                .Schedule(Milliseconds(delay += 4700), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_2);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5300), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_NIUZAO_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_INTRO, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 7300), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_NIUZAO_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_1, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 7200), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_3);
            });

            scheduler
                .Schedule(Milliseconds(delay += 5400), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_NIUZAO_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_2, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 9700), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_NIUZAO_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_3, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 11000), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_NIUZAO_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_4, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 9400), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_4);
            });

            scheduler
                .Schedule(Milliseconds(delay += 6500), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_NIUZAO_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_5, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_5);
            });

            scheduler
                .Schedule(Milliseconds(delay += 2700), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_NIUZAO_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_6, target);
            });

            scheduler
                .Schedule(Milliseconds(delay += 8600), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_NIUZAO_CELESTIAL, 50.0f, true))
                        tiger->AI()->Talk(TALK_SPECIAL_7, target);
            });
        });

        // Evade Check
        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            if (Player* target = ObjectAccessor::GetPlayer(*me, ownerGUID))
                if (me->GetExactDist2d(target) > 90.0f)
                    me->DespawnOrUnsummon();

            context.Repeat(Milliseconds(1000));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Wrathion Yulon Celestial 71352
class npc_wrathion_yulon_celestial : public CreatureScript
{
    public:
        npc_wrathion_yulon_celestial() : CreatureScript("npc_wrathion_yulon_celestial") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(QUEST_CELESTIAL_BLESSING) == QUEST_STATUS_INCOMPLETE)
                player->CastSpell(player, invZoneChallengeType.find(creature->GetZoneId())->second, true);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        struct npc_wrathion_yulon_celestialAI : public ScriptedAI
        {
            npc_wrathion_yulon_celestialAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* summoner) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_wrathion_yulon_celestialAI(creature);
        }
};

// Wrathion Xuen Challenge 71037
struct npc_wrathion_xuen_challenge : public customCreatureAI
{
    npc_wrathion_xuen_challenge(Creature* creature) : customCreatureAI(creature) { }

    TaskScheduler scheduler;
    uint64 targetGUID;
    bool hasReign;
    bool hasDefeat;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());
        me->SetPhaseMask(4, true);
        targetGUID = summoner->GetGUID();
    }

    void Reset() OVERRIDE
    {
        me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID, 45498);
        SetCombatMovement(false);
        events.Reset();
        hasReign = false;
        hasDefeat = false;
        summons.DespawnAll();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        DoCast(me, SPELL_BLINDSIDE, true);
        events.ScheduleEvent(EVENT_ARC, urand(1500, 3 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_REIGN_OF_FIRE, 11 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_BLOOD_OF_DEATHBORN, 35 * IN_MILLISECONDS);

        // Evade Check
        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                if (me->GetExactDist2d(target) > 90.0f)
                    me->DespawnOrUnsummon();

            context.Repeat(Milliseconds(1000));
        });
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summons.Summon(summon);

        if (me->GetVictim())
            summon->SetInCombatWith(me->GetVictim());
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_REIGN_OF_FIRE)
        {
            me->RemoveChanneledCast(targetGUID);

            if (hasReign)
                return;

            hasReign = true;
            DoCast(me, SPELL_NELTHARIONS_TEAR, true); // only once, next time i`ll reborn
        }
    }

    void KilledUnit(Unit* victim) OVERRIDE
    {
        if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
        {
            scheduler
                .Schedule(Milliseconds(1000), [this](TaskContext context)
            {
                if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                    if (Creature* tiger = me->FindNearestCreature(NPC_WHITE_TIGER_TEMPLE, 200.0f, true))
                        tiger->CastSpell(target, SPELL_RESSURECTION, true);

                me->DespawnOrUnsummon(500);
            });

            return;
        }
    }

    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        if (pointId == EVENT_JUMP)
        {
            if (Unit* target = ObjectAccessor::GetUnit(*me, targetGUID))
                me->PrepareChanneledCast(me->GetAngle(target), SPELL_CRUMBLING_ARC);

            scheduler
                .Schedule(Milliseconds(3500), [this](TaskContext context)
            {
                me->RemoveChanneledCast(targetGUID);
            });
        }
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (hasDefeat)
            damage = 0;

        if ((HealthBelowPct(5) || damage >= me->GetHealth()) && !hasDefeat)
        {
            damage = 0;
            hasDefeat = true;
            summons.DespawnAll();
            events.Reset();
            me->setFaction(35);
            DoCast(me, SPELL_CHALLENGE_DONE_CREDIT, true);
            me->DespawnOrUnsummon(5 * IN_MILLISECONDS);
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
            switch (eventId)
            {
                case EVENT_REIGN_OF_FIRE:
                    me->PrepareChanneledCast(me->GetOrientation(), SPELL_REIGN_OF_FIRE);
                    events.ScheduleEvent(EVENT_REIGN_OF_FIRE, 35 * IN_MILLISECONDS);
                    break;
                case EVENT_BLOOD_OF_DEATHBORN:
                    if (Unit* target = ObjectAccessor::GetUnit(*me, targetGUID))
                        DoCast(target, SPELL_BLOOD_OF_DEATHBORN_MISSLE);

                    events.ScheduleEvent(EVENT_BLOOD_OF_DEATHBORN, 25 * IN_MILLISECONDS);
                    break;
                case EVENT_ARC: // jump behind before cast
                    if (Unit* target = ObjectAccessor::GetUnit(*me, targetGUID))
                        me->GetMotionMaster()->MoveJump(target->GetPositionX() + frand(-5.0f, 5.0f), target->GetPositionY() + frand(-5.0f, 5.0f), target->GetPositionZ(), 20.0f, 15.0f, EVENT_JUMP);

                    events.ScheduleEvent(EVENT_ARC, urand(4500, 9500));
                    break;
            }
        }
    }
};

// Neltharion Tear 71041
struct npc_neltharions_tear : public ScriptedAI
{
    npc_neltharions_tear(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    bool hasTriggered;
    uint64 targetGUID;
    uint64 summonerGUID;

    void Reset() OVERRIDE
    {
        hasTriggered = false;
        targetGUID = 0;
    }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        summonerGUID = summoner->GetGUID();
        me->SetPhaseMask(4, true);
    }

    void KilledUnit(Unit* victim) OVERRIDE
    {
        if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
        {
            if (Creature* wrathion = ObjectAccessor::GetCreature(*me, summonerGUID))
                wrathion->AI()->KilledUnit(victim);

            return;
        }
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (hasTriggered)
            damage = 0;

        if (damage >= me->GetHealth() && !hasTriggered)
        {
            damage = 0;
            hasTriggered = true;

            if (attacker)
                targetGUID = attacker->GetGUID();

            me->PrepareChanneledCast(me->GetOrientation());
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            me->SetStandState(UNIT_STAND_STATE_DEAD);

            uint32 delay = urand(30 * IN_MILLISECONDS, 35 * IN_MILLISECONDS);
            scheduler
                .Schedule(Milliseconds(delay), [this](TaskContext context)
            {
                DoCast(me, SPELL_NELTHRAIONS_TEAR_BEAM, true);
            });

            scheduler
                .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
            {
                me->SetHealth(me->GetMaxHealth());
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->RemoveChanneledCast(targetGUID);
                hasTriggered = false;
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

// Xuen Challenge Gong 71109
struct npc_xuen_challenge_gong : public ScriptedAI
{
    npc_xuen_challenge_gong(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());
        me->SetPhaseMask(4, true);
        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    }

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        me->DespawnOrUnsummon(1500);
        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        clicker->CastSpell(clicker, SPELL_XUEN_CHALLENGE, true);

        std::list<TempSummon*> wrathions;
        clicker->GetSummons(wrathions, NPC_WRATHION_XUEN);

        if (!wrathions.empty())
            wrathions.front()->AI()->DoAction(ACTION_START_XUEN_CHALLENGE);
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

// Wrathion Cloak Achieve 71426
struct npc_wrathion_cloak_achieve : public ScriptedAI
{
    npc_wrathion_cloak_achieve(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 targetGUID;
    uint32 delay;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        targetGUID = summoner->GetGUID();
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());

        // Here missed scene launch (celesial spirits in sky , launch by 142541, but miss PackageID)
        me->SetFacingTo(me->GetAngle(summoner));
        Talk(TALK_INTRO);
        delay = 4500;

        scheduler
            .Schedule(Milliseconds(delay), [this](TaskContext context)
        {
            Talk(TALK_SPECIAL_1);

            delay = 0;
            scheduler
                .Schedule(Milliseconds(delay += 7400), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_2);
            });

            scheduler
                .Schedule(Milliseconds(delay += 6600), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_3);
            });

            scheduler
                .Schedule(Milliseconds(delay += 6100), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_4);

                // Summon Celestial Spirits
                if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                {
                    target->CastSpell(target, SPELL_SUMMON_JADE_SERPENT, true);
                    target->CastSpell(target, SPELL_SUMMON_BLACK_OX, true);
                    target->CastSpell(target, SPELL_SUMMON_RED_CRANE, true);
                    target->CastSpell(target, SPELL_SUMMON_TIGER, true);
                }
            });

            scheduler
                .Schedule(Milliseconds(delay += 12100), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_5);
                DoCast(me, SPELL_CLOACK_CREATION_STATE, true);
                me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() - M_PI));

                // Tiger Spirit
                if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                {
                    std::list<TempSummon*> celestials;
                    target->GetSummons(celestials, NPC_WHITE_TIGER_SPIRIT);

                    if (!celestials.empty())
                        celestials.front()->AI()->DoAction(ACTION_START_INTRO);
                }
            });

            scheduler
                .Schedule(Milliseconds(delay += 6000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_6);

                // Serpent Spirit
                if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                {
                    std::list<TempSummon*> celestials;
                    target->GetSummons(celestials, NPC_JADE_SERPENT_SPIRIT);

                    if (!celestials.empty())
                        celestials.front()->AI()->DoAction(ACTION_START_INTRO);
                }
            });

            scheduler
                .Schedule(Milliseconds(delay += 6000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_7);

                // Ox Spirit
                if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                {
                    std::list<TempSummon*> celestials;
                    target->GetSummons(celestials, NPC_BLACK_OX_SPIRIT);

                    if (!celestials.empty())
                        celestials.front()->AI()->DoAction(ACTION_START_INTRO);
                }
            });

            scheduler
                .Schedule(Milliseconds(delay += 6000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_8);

                // Crane Spirit
                if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                {
                    std::list<TempSummon*> celestials;
                    target->GetSummons(celestials, NPC_RED_CRANE_SPIRIT);

                    if (!celestials.empty())
                        celestials.front()->AI()->DoAction(ACTION_START_INTRO);
                }
            });

            scheduler
                .Schedule(Milliseconds(delay += 5000), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_9);
                DoCast(me, SPELL_CLOACK_CELESTIAL_SPHERE, true);
            });

            scheduler
                .Schedule(Milliseconds(delay += 7500), [this](TaskContext context)
            {
                Talk(TALK_SPECIAL_10);
                me->RemoveAurasDueToSpell(SPELL_CLOACK_CELESTIAL_SPHERE);
                me->RemoveAurasDueToSpell(SPELL_CLOACK_CREATION_STATE);

                if (Player* target = ObjectAccessor::GetPlayer(*me, targetGUID))
                    target->KilledMonsterCredit(NPC_CLOAK_ACHIEVE_CREDIT);

                me->DespawnOrUnsummon(10 * IN_MILLISECONDS);
            });

        });

        scheduler
            .Schedule(Milliseconds(delay += 2000), [this](TaskContext context)
        {
            // Path
            Movement::MoveSplineInit init(me);
            for (auto itr : wrathionMassonsPath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.SetSmooth();
            init.SetWalk(true);
            init.Launch();
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE 
    {
        scheduler.Update(diff);
    }
};

// Wrathion 71424
class npc_wrathion_at_massons_folly : public CreatureScript
{
    public:
        npc_wrathion_at_massons_folly() : CreatureScript("npc_wrathion_at_massons_folly") { }
    
        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
        {
            if (quest->GetQuestId() == QUEST_CLOAK_OF_VIRTUE)
                player->CastSpell(player, SPELL_SUMMON_WRATHION_CELESTIAL_END, true);

            return false;
        }

        struct npc_wrathion_at_massons_follyAI : public ScriptedAI
        {
            npc_wrathion_at_massons_follyAI(Creature* creature) : ScriptedAI(creature) { }
    
            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };
    
        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_wrathion_at_massons_follyAI(creature);
        }
};

// Celestial Spirits 71434, 71435, 71439, 71441
struct npc_wrathion_celestial_spirits_cloak : public ScriptedAI
{
    npc_wrathion_celestial_spirits_cloak(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        me->ToTempSummon()->SetPrivateObjectOwner(summoner->GetGUID());
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        if (actionId == ACTION_START_INTRO)
        {
            me->GetMotionMaster()->MovePoint(0, wrathionMassonsPath[2]);
            me->DespawnOrUnsummon(me->GetSplineDuration());
        }
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

// Wrathion Pandaren Legend 72723
class npc_wrathion_pandaren_legend : public CreatureScript
{
    public:
        npc_wrathion_pandaren_legend() : CreatureScript("npc_wrathion_pandaren_legend") { }
    
        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());
    
            if (player->GetQuestStatus(QUEST_A_PANDAREN_LEGEND) == QUEST_STATUS_INCOMPLETE)
                player->CastSpell(player, SPELL_LEGENDARY_FINALE, true);
    
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }
    
        struct npc_wrathion_pandaren_legendAI : public ScriptedAI
        {
            npc_wrathion_pandaren_legendAI(Creature* creature) : ScriptedAI(creature) { }
    
            void IsSummonedBy(Unit* summoner) OVERRIDE { }
        };
    
        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_wrathion_pandaren_legendAI(creature);
        }
};

// Willy Wilder 70126
struct npc_willy_wilder : public ScriptedAI
{
    npc_willy_wilder(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();

        me->setFaction(35); // by default

        scheduler
            .Schedule(Milliseconds(3000), [this](TaskContext context)
        {
            Movement::MoveSplineInit init(me);
            for (auto&& itr : willyPath)
                init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

            init.SetWalk(true);
            init.Launch();

            scheduler
                .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
            {
                me->HandleEmoteCommand(EMOTE_ONESHOT_CHEER);
                me->SetHomePosition(*me);
                me->setFaction(7);
                Talk(TALK_INTRO);
            });
        });
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_VIOLET_AND_BLUEBERRIES, urand(4 * IN_MILLISECONDS, 5 * IN_MILLISECONDS));
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();

        uint32 corpseDelay = me->GetCorpseDelay();
        uint32 respawnDelay = me->GetRespawnDelay();

        me->SetCorpseDelay(1);
        me->SetRespawnDelay(29);

        me->DespawnOrUnsummon();

        me->SetCorpseDelay(corpseDelay);
        me->SetRespawnDelay(respawnDelay);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        Talk(TALK_SPECIAL_1);
    }

    void UpdateAI(uint32 diff) OVERRIDE 
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_VIOLET_AND_BLUEBERRIES)
            {
                DoCast(me, SPELL_VIOLETS_AND_BLUEBERRIES);
                events.ScheduleEvent(EVENT_VIOLET_AND_BLUEBERRIES, 12 * IN_MILLISECONDS);
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Wrathion Jewel Chest 216837
class go_wrathion_jewel_chest : public GameObjectScript
{
    public:
        go_wrathion_jewel_chest() : GameObjectScript("go_wrathion_jewel_chest") { }

        bool OnQuestReward(Player* player, GameObject* go, Quest const* quest, uint32 /*item*/) OVERRIDE
        {
            player->SetPhaseMask(1, true);
            go->Delete();

            return true;
        }
};

// Holding Tray 126680
class spell_wrathion_holding_a_cup : public SpellScript
{
    PrepareSpellScript(spell_wrathion_holding_a_cup)

    void HandleAfterCast()
    {
        if (Unit* caster = GetCaster())
            caster->GetAI()->DoAction(ACTION_WRATHION_TAKE_CUP);
    }

    void Register() OVERRIDE
    {
        AfterCast += SpellCastFn(spell_wrathion_holding_a_cup::HandleAfterCast);
    }
};

// Summon Wrathion Celestials 141745, 141809, 141646
class spell_summon_wrathion_celestials : public AuraScript
{
    PrepareAuraScript(spell_summon_wrathion_celestials);

    void HandleOnApply(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* owner = GetOwner()->ToPlayer())
            owner->SetPhaseMask(4, true);
    }

    void HandleOnRemove(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* owner = GetOwner()->ToPlayer())
            owner->SetPhaseMask(1, true);
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_summon_wrathion_celestials::HandleOnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_summon_wrathion_celestials::HandleOnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Reign of Fire 141510 
class spell_wrathion_reign_of_fire : public AuraScript
{
    PrepareAuraScript(spell_wrathion_reign_of_fire);

    void OnTrigger(AuraEffect const* /*aurEff*/)
    {
        if (Unit* owner = GetOwner()->ToUnit())
            owner->CastSpell(owner, SPELL_REIGN_OF_FIRE_MISSLE, true);
    }

    void HandleOnRemove(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Creature* owner = GetOwner()->ToCreature())
            owner->AI()->DoAction(ACTION_REIGN_OF_FIRE);
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_wrathion_reign_of_fire::OnTrigger, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_wrathion_reign_of_fire::HandleOnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class BlindsidePredicate 
{
    public:
        BlindsidePredicate(Unit* const m_caster) : _caster(m_caster) { }
    
        bool operator()(WorldObject* object)
        {
            return object && object->ToUnit() && !_caster->isInBack(object->ToUnit());
    
            return false;
        }
    
    private:
        Unit const* _caster;
};

// Blindside 141556
class spell_wrathion_blindside : public SpellScript
{
    PrepareSpellScript(spell_wrathion_blindside);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(BlindsidePredicate(GetCaster()));
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_wrathion_blindside::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// Legendary Finale 146397
class spell_wrathion_legendary_finale : public AuraScript
{
    PrepareAuraScript(spell_wrathion_legendary_finale);

    void HandleOnRemove(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* owner = GetOwner()->ToPlayer())
            owner->KilledMonsterCredit(NPC_LISTEN_YOUR_STORY_CREDIT);
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_wrathion_legendary_finale::HandleOnRemove, EFFECT_0, SPELL_AURA_PLAY_SCENE, AURA_EFFECT_HANDLE_REAL);
    }
};

// Wrathion Essence 146428
class spell_legendary_wrathion_essence : public SpellScript
{
    PrepareSpellScript(spell_legendary_wrathion_essence);

    void HandleOnEffectHit(SpellEffIndex effIdx)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            uint32 cloackId = 0;
            
            for (auto&& itr : invCloakType)
            {
                if (caster->HasItemCount(itr.first, 1))
                {
                    cloackId = itr.first;
                    break;
                }
            }

            if (!cloackId) // play haven`t any cloak to upgrade
                return;

            caster->DestroyItemCount(cloackId, 1, true);
            caster->AddItem(invCloakType.find(cloackId)->second, 1);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_legendary_wrathion_essence::HandleOnEffectHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Violets And Blueberriers Eff 139033
class spell_violets_and_blueberries : public AuraScript
{
    PrepareAuraScript(spell_violets_and_blueberries);

    void HandleAuraEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetStackAmount() > 4 && GetOwner() && GetOwner()->ToUnit())
        {
            GetOwner()->ToUnit()->RemoveAurasDueToSpell(SPELL_VIOLETS_AND_BLUEBERRIES);
            GetOwner()->ToUnit()->RemoveAurasDueToSpell(SPELL_VIOLETS_AND_BLUEBERRIES_EFF);
            GetOwner()->ToUnit()->CastSpell(GetOwner()->ToUnit(), SPELL_STUFFED, true);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_violets_and_blueberries::HandleAuraEffectApply, EFFECT_1, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAPPLY);
    }
};

// 959. Summoned By 141466 - Blood of the Deathborn
class sat_blood_of_deathborn : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* target = triggering->ToPlayer())
            target->CastSpell(target, SPELL_BLOOD_OF_DEATHBORN_EFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* target = triggering->ToPlayer())
            target->RemoveAurasDueToSpell(SPELL_BLOOD_OF_DEATHBORN_EFF);
    }
};

// Areatrigger 8503
class AreaTrigger_at_massons_folly : public AreaTriggerScript
{
    public:
        AreaTrigger_at_massons_folly() : AreaTriggerScript("at_massons_folly") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            if (player->IsAlive() && !player->IsInCombat())
            {
                // 5.0
                if (player->GetQuestStatus(QUEST_BREATH_OF_THE_BLACK_PRINCE) == QUEST_STATUS_INCOMPLETE && player->GetPhaseMask() == 1)
                {
                    player->CastSpell(player, SPELL_KILL_CREDIT_MASSONS_FOLLY, true);
                    player->SetPhaseMask(3, true);

                    return true;
                }

                // 5.2
                if (player->GetQuestStatus(QUEST_CROWN_OF_THE_HEAVEN) == QUEST_STATUS_INCOMPLETE && !player->HasAura(SPELL_MASSONS_FOLLY_FINAL_EVENT))
                {
                    player->CastSpell(player, SPELL_MASSONS_FOLLY_FINAL_EVENT, true);
                    return true;
                }
            }

            return false;
        }
};

// Areatrigger 9321
class AreaTrigger_at_seat_of_knowledge : public AreaTriggerScript
{
    public:
        AreaTrigger_at_seat_of_knowledge() : AreaTriggerScript("AreaTrigger_at_seat_of_knowledge") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            // 5.4
            if (player->GetQuestStatus(QUEST_A_PANDAREN_LEGEND) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(NPC_WRATHION_MEET_SEAT_OF_KNOWLEDGE_CREDIT);
                return true;
            }

            return false;
        }
};

class scene_wrathion_making_legend : public SceneScript
{
    public:
        scene_wrathion_making_legend() : SceneScript("scene_wrathion_making_legend") { }

        void OnSceneStart(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            player->SetPhaseMask(3, true);
        }

        void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void OnSceneCancel(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void CreditScene(Player* player)
        {
            player->CastSpell(player, SPELL_KILL_CREDIT_LEGEND_IN_THE_MAKING, true);

            player->SetPhaseMask(1, true);
        }
};

class scene_breath_of_the_black_prince : public SceneScript
{
    public:
        scene_breath_of_the_black_prince() : SceneScript("scene_breath_of_the_black_prince") { }

        void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void OnSceneCancel(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void CreditScene(Player* player)
        {
            // why this doesn`t work now? It did work
            //player->CastSpell(player, SPELL_KILL_CREDIT_BREATH_OF_PRINCE, true);
            player->CompleteQuest(QUEST_BREATH_OF_THE_BLACK_PRINCE);
            player->SetPhaseMask(1, true);
        }
};

class scene_wrathion_measure_of_a_leader : public SceneScript
{
    public:
        scene_wrathion_measure_of_a_leader() : SceneScript("scene_wrathion_measure_of_a_leader") { }

        void OnSceneStart(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            player->SetPhaseMask(3, true);
        }

        void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void OnSceneCancel(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void CreditScene(Player* player)
        {
            player->CastSpell(player, SPELL_KILL_CREDIT_MEASURE_OF_THE_LEAD, true);
            player->SetPhaseMask(1, true);
        }
};

class scene_wrathion_gift_of_black_prince : public SceneScript
{
    public:
        scene_wrathion_gift_of_black_prince() : SceneScript("scene_wrathion_gift_of_black_prince") { }

        void OnSceneStart(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            player->SetPhaseMask(3, true);
        }

        void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void OnSceneCancel(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }

        void CreditScene(Player* player)
        {
            player->CastSpell(player, SPELL_KILL_CREDIT_FINALE_5_1, true);
        }
};

class scene_wrathion_crown_of_the_heavens : public SceneScript
{
    public:
        scene_wrathion_crown_of_the_heavens() : SceneScript("scene_wrathion_crown_of_the_heavens") { }
    
        void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }
    
        void OnSceneCancel(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }
    
        void CreditScene(Player* player)
        {
            player->CastSpell(player, SPELL_EVENT_CLEANUP, true);
        }
};

class scene_wrathion_serpent_celestial_scene : public SceneScript
{
    public:
        scene_wrathion_serpent_celestial_scene() : SceneScript("scene_wrathion_serpent_celestial_scene") { }
    
        void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }
    
        void OnSceneCancel(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) OVERRIDE
        {
            CreditScene(player);
        }
    
        void CreditScene(Player* player)
        {
            player->KilledMonsterCredit(NPC_YULONG_CREDIT);
            //player->SummonCreature(NPC_WRATHION_TEMPLE_OF_JADE_SERPENT, yulonWrathion, TEMPSUMMON_MANUAL_DESPAWN);
        }
};

void AddSC_the_veiled_stair()
{
    // black prince questline
    new npc_wrathion();
    new npc_tong_the_fixer_legend_in_the_making();
    new npc_wrathion_breath_of_the_black_prince();
    new go_wrathion_jewel_chest();
    new creature_script<npc_darkhatched_soulbleeder>("npc_darkhatched_soulbleeder");
    new creature_script<npc_leechfingers>("npc_leechfingers");
    new creature_script<npc_hatescale_spitter>("npc_hatescale_spitter");
    new creature_script<npc_hatescale_ironface>("npc_hatescale_ironface");
    new npc_wrathion_thunder_king();
    new creature_script<npc_image_of_wrathion>("npc_image_of_wrathion");
    new creature_script<npc_wrathion_suen_celestial>("npc_wrathion_suen_celestial");
    new creature_script<npc_wrathion_chiji_celestial>("npc_wrathion_chiji_celestial");
    new creature_script<npc_wrathion_niuzao_celestial>("npc_wrathion_niuzao_celestial");
    new npc_wrathion_yulon_celestial();
    new creature_script<npc_wrathion_xuen_challenge>("npc_wrathion_xuen_challenge");
    new creature_script<npc_neltharions_tear>("npc_neltharions_tear");
    new creature_script<npc_xuen_challenge_gong>("npc_xuen_challenge_gong");
    new creature_script<npc_wrathion_cloak_achieve>("npc_wrathion_cloak_achieve");
    new npc_wrathion_at_massons_folly();
    new creature_script<npc_wrathion_celestial_spirits_cloak>("npc_wrathion_celestial_spirits_cloak");
    new npc_wrathion_pandaren_legend();
    new creature_script<npc_willy_wilder>("npc_willy_wilder");

    new GenericSpellScriptLoader<spell_wrathion_holding_a_cup>("spell_wrathion_holding_a_cup");
    new GenericAuraScriptLoader<spell_summon_wrathion_celestials>("spell_summon_wrathion_celestials");
    new GenericAuraScriptLoader<spell_wrathion_reign_of_fire>("spell_wrathion_reign_of_fire");
    new GenericSpellScriptLoader<spell_wrathion_blindside>("spell_wrathion_blindside");
    new GenericAuraScriptLoader<spell_wrathion_legendary_finale>("spell_wrathion_legendary_finale");
    new GenericSpellScriptLoader<spell_legendary_wrathion_essence>("spell_legendary_wrathion_essence");
    new GenericAuraScriptLoader<spell_violets_and_blueberries>("spell_violets_and_blueberries");
    new atrigger_script<sat_blood_of_deathborn>("sat_blood_of_deathborn");
    new AreaTrigger_at_massons_folly();
    new AreaTrigger_at_seat_of_knowledge();
    new scene_wrathion_making_legend();
    new scene_breath_of_the_black_prince();
    new scene_wrathion_measure_of_a_leader();
    new scene_wrathion_gift_of_black_prince();
    new scene_wrathion_crown_of_the_heavens();
    new scene_wrathion_serpent_celestial_scene();
}
