/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "PassiveAI.h"
#include "Vehicle.h"
#include "DBCStores.h"
#include "EventProcessor.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Util.h"
#include <functional>
#include <list>

template<class AI>
class gilneas_creature_script : public CreatureScript
{
public:
    gilneas_creature_script(char const* name) : CreatureScript(name) { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new AI(creature);
    }
};

template<class Spell>
class gilneas_spell_script : public SpellScriptLoader
{
public:
    gilneas_spell_script(char const* name) : SpellScriptLoader(name) { }

    SpellScript* GetSpellScript() const override
    {
        return new Spell();
    }
};

class GilneasDelayedEvent : public BasicEvent
{
public:
    explicit GilneasDelayedEvent(std::function<void()> callback) : _callback(callback) { }

    bool Execute(uint64 /*eventTime*/, uint32 /*updateTime*/) override
    {
        _callback();
        return true;
    }

private:
    std::function<void()> _callback;
};

template<class Callable>
void AddGilneasDelayedEvent(Unit* owner, uint32 delay, Callable callback)
{
    owner->m_Events.AddEvent(new GilneasDelayedEvent(std::function<void()>(callback)), owner->m_Events.CalculateTime(delay));
}

static int32 irand(int32 min, int32 max)
{
    if (max <= min)
        return min;

    return min + int32(rand32() % uint32(max - min + 1));
}

static uint32 urand(uint32 min, uint32 max)
{
    if (max <= min)
        return min;

    return min + rand32() % (max - min + 1);
}

void MoveGilneasPointPath(Creature* creature, uint32 pointId, Position const* path, uint32 count, bool generatePath = false)
{
    if (!creature || !path || !count)
        return;

    creature->GetMotionMaster()->MovePoint(pointId, path[count - 1], generatePath);
}

enum Gilneas
{
    SPELL_PING_GILNEAN_CROW = 93275,
    SPELL_SUMMON_RAVENOUS_WORGEN_1 = 66836,
    SPELL_SUMMON_RAVENOUS_WORGEN_2 = 66925,
    SPELL_SHOOT_INSTAKILL = 67593, // Visual problem
    SPELL_COSMETIC_ATTACK = 69873,
    SPELL_PULL_TO = 67357,
    SPELL_GET_SHOT = 67349,

    EVENT_START_TALK_WITH_CITIZEN = 1,
    EVENT_TALK_WITH_CITIZEN_1 = 2,
    EVENT_TALK_WITH_CITIZEN_2 = 3,
    EVENT_TALK_WITH_CITIZEN_3 = 4,

    EVENT_START_DIALOG = 1,
    EVENT_START_TALK_TO_GUARD = 2,
    EVENT_TALK_TO_GUARD_1 = 3,
    EVENT_TALK_TO_GUARD_2 = 4,
    EVENT_RESET_DIALOG = 5,

    PRINCE_LIAM_GREYMANE_TEXT_00 = 0,
    PRINCE_LIAM_GREYMANE_TEXT_01 = 1,
    PRINCE_LIAM_GREYMANE_TEXT_02 = 2,

    EVENT_JUMP_TO_PRISON = 1,
    EVENT_AGGRO_PLAYER = 2,
    EVENT_FORCE_DESPAWN = 3,

    EVENT_COSMETIC_ATTACK = 1,
    EVENT_JUMP_TO_PLAYER = 2,
    EVENT_SHOOT_JOSIAH = 3,

    PHASE_ROOF = 0,
    PHASE_COMBAT = 1,

    WORGEN_ID_ROOF_1 = 0,
    WORGEN_ID_ROOF_2 = 1,
    WORGEN_ID_ROOF_3 = 2,
    WORGEN_ID_ROOF_4 = 3,
    WORGEN_ID_ROOF_5 = 4,
    WORGEN_ID_ROOF_6 = 5,
    WORGEN_ID_ROOF_7 = 6,

    WORGEN_ID_CATHEDRAL_1 = 7,
    WORGEN_ID_CATHEDRAL_2 = 8,
    WORGEN_ID_CATHEDRAL_3 = 9,
    WORGEN_ID_CATHEDRAL_4 = 10,
    WORGEN_ID_CATHEDRAL_5 = 11,
    WORGEN_ID_CATHEDRAL_6 = 12,
    WORGEN_ID_CATHEDRAL_7 = 13,
    WORGEN_ID_CATHEDRAL_8 = 14,
    WORGEN_ID_CATHEDRAL_9 = 15,

    NPC_PANICKED_CITIZEN_GATE = 44086,
    NPC_WORGEN_RUNT = 35456,
    NPC_WORGEN_RUNT_2 = 35188,
    NPC_WORGEN_ALPHA = 35170,
    NPC_WORGEN_ALPHA_2 = 35167,
    NPC_LORNA_CROWLEY = 35378,
    NPC_GENERIC_TRIGGER_LAB = 35374,

    SAY_JOSIAH_AVERY_1 = 0,
    SAY_JOSIAH_AVERY_2 = 1,
    SAY_JOSIAH_AVERY_3 = 2,
    SAY_JOSIAH_AVERY_4 = 3,
    SAY_JOSIAH_AVERY_5 = 4,
    SAY_JOSIAH_AVERY_6 = 5,

    QUEST_FROM_THE_SHADOWS = 14204,

    NPC_GILNEAS_MASTIFF = 35631,
    NPC_BLOODFANG_LURKER = 35463,

    SPELL_SUMMON_MASTIFF = 67807,
    SPELL_ATTACK_LURKER = 67805,
    SPELL_SHADOWSTALKER_STEALTH = 5916,
    SPELL_UNDYING_FRENZY = 80515,
    SPELL_ENRAGE = 8599,

    WORGEN_ENRAGE_EMOTE = 0,

    QUEST_SAVE_KRENNAN_ARANAS = 14293,

    NPC_KRENNAN_ARANAS = 35753,
    NPC_KING_GREYMANES_HORSE = 35905,
    NPC_BLOODFANG_RIPPER = 35505,
    NPC_GILNEAS_CITY_GUARD = 35509,
    NPC_KRENNAN_ARANAS_KILL_CREDIT = 35753,

    TEXT_KRENNAN_ARANAS = 0,
    EMOTE_SAVE_KRENNAN_ARANAS = 0,
    TEXT_SAVED_KRENNAN_ARANAS = 0,

    SPELL_GUARD_SHOOT = 48424,

    SPELL_SAVE_CYNTHIA = 68597,
    SPELL_SAVE_ASHLEY = 68598,
    SPELL_SAVE_JAMES = 68596,

    PLAYER_SAY_CYNTHIA = 0,
    PLAYER_SAY_ASHLEY = 1,
    PLAYER_SAY_JAMES = 2,

    NPC_JAMES = 36289,
    NPC_CYNTHIA = 36287,
    NPC_ASHLEY = 36288,

    EVENT_TALK_TO_PLAYER = 1,
    EVENT_START_RUN = 2,
    EVENT_OPEN_DOOR = 3,
    EVENT_RESUME_RUN = 4,
    EVENT_CRY = 5,

    CHILDREN_TEXT_ID = 0,

    GO_DOOR_TO_THE_BASEMENT = 196411,

    SPELL_CATCH_CAT = 68743,
    SPELL_LUCIUS_SHOOT = 41440,

    NPC_WAHL = 36458,
    NPC_WAHL_WORGEN = 36852,
    NPC_LUCIUS_THE_CRUEL = 36461,

    SAY_THIS_CAT_IS_MINE = 0,
    YELL_DONT_MESS = 0,

    ACTION_SUMMON_LUCIUS = 1,
    ACTION_CHANCE_DESPAWN = 2,
    ACTION_CONTINUE_SCENE = 3,

    POINT_CATCH_CHANCE = 4,

    SPELL_ROUND_UP_HORSE = 68908,
    SPELL_ROPE_CHANNEL = 68940,

    EVENT_CHECK_LORNA = 1,
    EVENT_CHECK_OWNER = 2,

    NPC_LORNA_CROWLEY_2 = 36457,

    QUEST_THE_HUNGRY_ETTIN = 14416,

    QUEST_CREDIT_HORSE = 36560,

    MUSIC_ENTRY_TELESCOPE = 23539,
    PLAY_CINEMATIC_TELESCOPE = 167,

    QUEST_ENTRY_EXODUS = 24438,

    NPC_STAGECOACH_HARNESS = 38755,
    NPC_HARNESS_SUMMONED = 43336,

    EVENT_BOARD_HARNESS_OWNER = 1,

    ACTION_START_WP = 1,

    GO_FIRST_GATE = 196401,
    GO_KINGS_GATE = 196412
};

Position const runt2SummonJumpPos = { -1671.915f, 1446.734f, 52.28712f };
Position const alphaSummonJumpPos = { -1656.723f, 1405.647f, 52.74205f };
Position const alpha2SummonJumpPos = { -1675.44f, 1447.495f, 52.28762f };

Position const josiahJumpPos = { -1796.63f, 1427.73f, 12.4624f };

uint32 const runtHousePathSize1 = 13;

Position const worgenRuntHousePath1[runtHousePathSize1] =
{
    { -1734.77f, 1527.007f, 55.2133f },
    { -1729.345f, 1526.495f, 55.4231f },
    { -1723.921f, 1525.982f, 55.6329f },
    { -1718.885f, 1525.88f, 55.89785f },
    { -1718.002f, 1516.054f, 55.36457f },
    { -1718.162f, 1512.458f, 55.41572f },
    { -1717.852f, 1508.871f, 55.64134f },
    { -1717.868f, 1507.03f, 55.78084f },
    { -1717.939f, 1498.912f, 56.2076f },
    { -1717.975f, 1494.827f, 56.34147f },
    { -1717.666f, 1491.978f, 56.46574f },
    { -1717.708f, 1491.591f, 56.51286f },
    { -1717.708f, 1491.591f, 56.51286f },
};

uint32 const runtHousePathSize2 = 11;

Position const worgenRuntHousePath2[runtHousePathSize2] =
{
    {-1705.29f, 1527.974f, 57.49218f },
    {-1709.63f, 1527.464f, 56.81163f },
    {-1713.971f, 1526.953f, 56.13107f },
    {-1718.249f, 1525.915f, 55.91631f },
    {-1718.002f, 1516.054f, 55.36457f },
    {-1718.162f, 1512.458f, 55.41572f },
    {-1717.852f, 1508.871f, 55.64134f },
    {-1717.866f, 1507.038f, 55.7804f },
    {-1717.928f, 1498.872f, 56.20963f },
    {-1717.947f, 1496.298f, 56.29393f },
    {-1717.947f, 1496.298f, 56.29393f },
};

uint32 const runtHousePathSize3 = 8;

Position const worgenRuntHousePath3[runtHousePathSize3] =
{
    {-1717.74f, 1514.99f, 55.37629f },
    {-1717.75f, 1513.727f, 55.39608f },
    {-1717.76f, 1512.465f, 55.41587f },
    {-1717.787f, 1508.872f, 55.64124f },
    {-1717.799f, 1507.277f, 55.74761f },
    {-1717.864f, 1498.657f, 56.22049f },
    {-1717.887f, 1495.557f, 56.31728f },
    {-1717.887f, 1495.557f, 56.31728f },
};

uint32 const runtHousePathSize4 = 11;

Position const worgenRuntHousePath4[runtHousePathSize4] =
{
    { -1727.101f, 1527.078f, 55.5045f },
    { -1724.719f, 1526.731f, 55.60394f },
    { -1722.337f, 1526.383f, 55.70337f },
    { -1718.885f, 1525.88f, 55.89785f },
    { -1718.002f, 1516.054f, 55.36457f },
    { -1718.162f, 1512.458f, 55.41572f },
    { -1717.852f, 1508.871f, 55.64134f },
    { -1717.845f, 1507.113f, 55.77633f },
    { -1717.81f, 1498.482f, 56.22932f },
    { -1717.793f, 1494.123f, 56.37141f },
    { -1717.793f, 1494.123f, 56.37141f },
};

uint32 const runtHousePathSize5 = 11;

Position const worgenRuntHousePath5[runtHousePathSize5] =
{
    { -1709.699f, 1527.335f, 56.34836f },
    { -1713.974f, 1526.625f, 56.13234f },
    { -1718.249f, 1525.915f, 55.91631f },
    { -1718.002f, 1516.054f, 55.36457f },
    { -1718.162f, 1512.458f, 55.41572f },
    { -1717.852f, 1508.871f, 55.64134f },
    { -1717.891f, 1506.948f, 55.78529f },
    { -1718.047f, 1499.267f, 56.18964f },
    { -1718.125f, 1495.405f, 56.31914f },
    { -1717.693f, 1492.178f, 56.45717f },
    { -1717.693f, 1492.178f, 56.45717f },
};

uint32 const runtHousePathSize6 = 9;

Position const worgenRuntHousePath6[runtHousePathSize6] =
{
    { -1718.083f, 1532.09f, 56.25435f },
    { -1718.104f, 1524.071f, 55.80854f },
    { -1718.125f, 1516.053f, 55.36273f },
    { -1718.134f, 1512.459f, 55.41573f },
    { -1718.143f, 1508.866f, 55.64067f },
    { -1718.151f, 1506.013f, 55.83052f },
    { -1718.167f, 1499.665f, 56.16953f },
    { -1718.172f, 1497.578f, 56.24289f },
    { -1718.172f, 1497.578f, 56.24289f },
};

uint32 const runtHousePathSize7 = 9;

Position const worgenRuntHousePath7[runtHousePathSize7] =
{
    { -1718.083f, 1532.09f, 56.25435f },
    { -1718.104f, 1524.071f, 55.80854f },
    { -1718.125f, 1516.053f, 55.36273f },
    { -1718.134f, 1512.459f, 55.41573f },
    { -1718.143f, 1508.866f, 55.64067f },
    { -1718.151f, 1506.013f, 55.83052f },
    { -1718.167f, 1499.665f, 56.16953f },
    { -1718.172f, 1497.578f, 56.24289f },
    { -1718.172f, 1497.578f, 56.24289f },
};

uint32 const runtCathedralPathSize1 = 10;

Position const worgenRuntCathedralPath1[runtCathedralPathSize1] =
{
    { -1612.885f, 1492.154f, 67.03599f },
    { -1618.054f, 1489.644f, 68.5475f },
    { -1623.222f, 1487.134f, 70.05901f },
    { -1629.727f, 1483.976f, 72.59077f },
    { -1630.914f, 1483.4f, 72.92308f },
    { -1632.244f, 1482.754f, 72.91827f },
    { -1636.865f, 1480.51f, 68.61356f },
    { -1638.087f, 1479.916f, 67.58314f },
    { -1638.852f, 1479.545f, 66.56925f },
    { -1638.852f, 1479.545f, 66.56925f },
};

uint32 const runtCathedralPathSize2 = 7;

Position const worgenRuntCathedralPath2[runtCathedralPathSize2] =
{
    { -1618.982f, 1489.76f, 68.56043f },
    { -1625.62f, 1487.033f, 71.4378f },
    { -1632.258f, 1484.306f, 74.31516f },
    { -1634.6f, 1483.343f, 72.61462f },
    { -1636.067f, 1482.741f, 70.69682f },
    { -1639.282f, 1481.42f, 66.99659f },
    { -1639.282f, 1481.42f, 66.99659f },
};

uint32 const runtCathedralPathSize3 = 6;

Position const worgenRuntCathedralPath3[runtCathedralPathSize3] =
{
    { -1637.957f, 1493.445f, 67.77746f },
    { -1638.569f, 1489.736f, 68.47077f },
    { -1639.182f, 1486.027f, 69.16409f },
    { -1640.578f, 1477.564f, 64.01109f },
    { -1640.676f, 1476.976f, 63.45144f },
    { -1640.676f, 1476.976f, 63.45144f },
};

uint32 const runtCathedralPathSize4 = 5;

Position const worgenRuntCathedralPath4[runtCathedralPathSize4] =
{
    { -1628.66f, 1482.281f, 71.34027f },
    { -1630.399f, 1481.66f, 71.33196f },
    { -1632.139f, 1481.039f, 71.32365f },
    { -1639.006f, 1478.586f, 65.77306f },
    { -1639.006f, 1478.586f, 65.77306f },
};


uint32 const runtCathedralPathSize5 = 9;

Position const worgenRuntCathedralPath5[runtCathedralPathSize5] =
{
    { -1620.279f, 1484.46f, 67.03528f },
    { -1622.424f, 1483.882f, 68.05564f },
    { -1624.568f, 1483.304f, 69.076f },
    { -1628.933f, 1482.127f, 70.91297f },
    { -1632.153f, 1481.259f, 71.52889f },
    { -1637.776f, 1479.743f, 67.45475f },
    { -1638.956f, 1479.425f, 66.41499f },
    { -1639.354f, 1479.318f, 65.98292f },
    { -1639.354f, 1479.318f, 65.98292f },
};

uint32 const runtCathedralPathSize6 = 10;

Position const worgenRuntCathedralPath6[runtCathedralPathSize6] =
{
    { -1633.998f, 1495.233f, 68.24403f },
    { -1634.344f, 1491.3f, 70.41303f },
    { -1634.689f, 1487.368f, 72.58203f },
    { -1634.735f, 1486.842f, 72.91134f },
    { -1634.837f, 1485.684f, 73.20835f },
    { -1634.987f, 1483.981f, 72.29824f },
    { -1635.084f, 1482.868f, 71.9163f },
    { -1635.166f, 1481.939f, 70.77574f },
    { -1635.809f, 1474.621f, 63.4636f },
    { -1635.809f, 1474.621f, 63.4636f },
};

uint32 const runtCathedralPathSize7 = 7;

Position const worgenRuntCathedralPath7[runtCathedralPathSize7] =
{
    { -1629.975f, 1494.066f, 70.64719f },
    { -1631.979f, 1491.585f, 71.31316f },
    { -1633.984f, 1489.104f, 71.97912f },
    { -1634.991f, 1487.856f, 72.09129f },
    { -1636.631f, 1485.826f, 71.53807f },
    { -1640.601f, 1480.912f, 65.49457f },
    { -1640.601f, 1480.912f, 65.49457f },
};

uint32 const runtCathedralPathSize8 = 8;

Position const worgenRuntCathedralPath8[runtCathedralPathSize8] =
{
    { -1620.879f, 1491.133f, 70.67613f },
    { -1622.665f, 1489.818f, 71.04526f },
    { -1624.451f, 1488.503f, 71.41438f },
    { -1630.469f, 1484.073f, 73.09648f },
    { -1631.376f, 1483.405f, 73.20229f },
    { -1632.245f, 1482.765f, 72.92912f },
    { -1635.605f, 1480.292f, 68.99215f },
    { -1635.605f, 1480.292f, 68.99215f },
};

Position const worgenRuntJumpPos[] =
{
    { -1694.03f, 1466.33f, 52.2872f },
    { -1688.92f, 1455.69f, 52.2871f },
    { -1699.46f, 1468.43f, 52.2871f },
    { -1697.73f, 1469.52f, 52.2871f },
    { -1701.65f, 1470.94f, 52.2871f },
    { -1693.91f, 1468.46f, 52.2872f },
    { -1697.30f, 1464.65f, 52.2871f },

    { -1681.57f, 1455.77f, 52.2871f },
    { -1677.47f, 1454.94f, 52.2871f },
    { -1677.71f, 1452.2f,  52.2871f },
    { -1677.66f, 1450.93f, 52.2871f },
    { -1672.56f, 1448.06f, 52.2871f },
    { -1683.13f, 1455.39f, 52.2871f },
    { -1669.81f, 1442.34f, 52.2871f },
    { -1674.15f, 1448.9f,  52.2871f },
};

Position const WorgenRuntHousePos[] =
{
    { -1729.345f, 1526.495f, 55.47962f, 6.188943f },
    { -1709.63f, 1527.464f, 56.86086f, 3.258752f },
    { -1717.75f, 1513.727f, 55.47941f, 4.704845f },
    { -1724.719f, 1526.731f, 55.66177f, 6.138319f },
    { -1713.974f, 1526.625f, 56.21981f, 3.306195f },
    { -1718.104f, 1524.071f, 55.81641f, 4.709816f },
    { -1718.262f, 1518.557f, 55.55954f, 4.726997f },

    { -1618.054f, 1489.644f, 68.45153f, 3.593639f },
    { -1625.62f, 1487.033f, 71.27762f, 3.531424f },
    { -1638.569f, 1489.736f, 68.55273f, 4.548815f },
    { -1630.399f, 1481.66f, 71.41516f, 3.484555f },
    { -1622.424f, 1483.882f, 67.67381f, 3.404875f },
    { -1634.344f, 1491.3f, 70.10101f, 4.6248f },
    { -1631.979f, 1491.585f, 71.11481f, 4.032866f },
    { -1627.273f, 1499.689f, 68.89395f, 4.251452f },
    { -1622.665f, 1489.818f, 71.03797f, 3.776179f },
};

uint8 const JamesPathLenght = 6;
Position const JamesPath[][JamesPathLenght] =
{
    {
        { -1925.925049f, 2539.176514f, 1.392833f, 0.0f },
        { -1913.658203f, 2545.986328f, 1.465530f, 0.0f },
        { -1904.370728f, 2552.793213f, 1.132485f, 0.0f },
        { -1900.970459f, 2550.849365f, 0.714445f, 0.0f },
        { -1886.868774f, 2540.282471f, 1.706371f, 0.0f },
        { -1882.739746f, 2543.941865f, 1.628683f, 0.0f },
    },
};

uint8 const CynthiaPathLenght = 6;
Position const CynthiaPath[][CynthiaPathLenght] =
{
    {
        { -1947.965088f, 2518.669434f, 1.826697f, 0.0f },
        { -1923.350830f, 2521.841553f, 1.586985f, 0.0f },
        { -1917.197632f, 2520.494385f, 2.297501f, 0.0f },
        { -1890.082031f, 2519.952148f, 1.425827f, 0.0f },
        { -1886.868774f, 2540.282471f, 1.706371f, 0.0f },
        { -1882.739746f, 2543.941865f, 1.628683f, 0.0f },
    },
};

uint8 const AshleyPathLenght = 13;
Position const AshleyPath[][AshleyPathLenght] =
{
    {
        { -1928.023682f, 2558.467285f, 12.733648f, 0.0f },
        { -1928.248901f, 2553.930176f, 12.734390f, 0.0f },
        { -1923.981567f, 2552.113770f, 12.736046f, 0.0f },
        { -1919.301514f, 2563.295166f, 3.579522f, 0.0f },
        { -1930.442017f, 2562.145996f, 3.579824f, 0.0f },
        { -1941.160156f, 2566.118896f, 1.392157f, 0.0f },
        { -1940.852295f, 2543.049072f, 1.392157f, 0.0f },
        { -1919.504517f, 2543.273926f, 1.392157f, 0.0f },
        { -1913.658203f, 2545.986328f, 1.465530f, 0.0f },
        { -1904.370728f, 2552.793213f, 1.132485f, 0.0f },
        { -1900.970459f, 2550.849365f, 0.714445f, 0.0f },
        { -1886.868774f, 2540.282471f, 1.706371f, 0.0f },
        { -1882.739746f, 2543.941865f, 1.628683f, 0.0f },
    },
};

uint8 const childrenBasementPathLenght = 3;
Position const childrenBasementPath[][childrenBasementPathLenght] =
{
    {
        { -1879.062378f, 2546.958984f, -0.130342f, 0.0f },
        { -1873.854980f, 2550.903564f, -5.898719f, 0.0f },
        { -1868.589844f, 2536.521240f, -6.365717f, 0.0f },
    },
};

const std::string PlayerText[3] =
{
    "It's not safe here. Go to the Allens' basement.",
    "Join the others inside the basement next door. Hurry!",
    "Your mother's in the basement next door. Get to her now!",
};

class npc_gilnean_crow : public CreatureScript
{
public:
    npc_gilnean_crow() : CreatureScript("npc_gilneas_crow") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_gilnean_crowAI(creature);
    }

    struct npc_gilnean_crowAI : public ScriptedAI
    {
        npc_gilnean_crowAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 tSpawn;
        bool Move;

        void Reset()
        {
            Move = false;
            tSpawn = 0;
            me->SetPosition(me->GetCreatureData()->posX, me->GetCreatureData()->posY, me->GetCreatureData()->posZ, me->GetCreatureData()->orientation);
        }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_PING_GILNEAN_CROW)
            {
                if (!Move)
                {
                    me->SetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE); // Change our emote state to allow flight
                    me->SetDisableGravity(true);
                    Move = true;
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!Move)
                return;

            if (tSpawn <= diff)
            {
                me->GetMotionMaster()->MovePoint(0, (me->GetPositionX() + irand(-15, 15)), (me->GetPositionY() + irand(-15, 15)), (me->GetPositionZ() + irand(5, 15)));
                tSpawn = urand(500, 1000);
            }
            else tSpawn -= diff;

            if ((me->GetPositionZ() - me->GetCreatureData()->posZ) >= 20.0f)
            {
                me->DisappearAndDie();
                me->RemoveCorpse(true);
                Move = false;
            }
        }
    };
};

struct npc_gilneas_city_guard_gate : public ScriptedAI
{
    npc_gilneas_city_guard_gate(Creature* creature) : ScriptedAI(creature) { }

    EventMap m_events;
    uint8 m_say;
    uint8 m_emote;
    ObjectGuid m_citizenGUID;

    void Reset() override
    {
        if (me->GetDistance2d(-1430.47f, 1345.55f) < 10.0f)
            m_events.ScheduleEvent(EVENT_START_TALK_WITH_CITIZEN, urand(10000, 30000));
    }

    void UpdateAI(uint32 diff) override
    {
        m_events.Update(diff);

        while (uint32 eventId = m_events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_START_TALK_WITH_CITIZEN:
            {
                m_citizenGUID = GetRandomCitizen();
                m_emote = RAND(EMOTE_ONESHOT_COWER, EMOTE_STATE_TALK, EMOTE_ONESHOT_CRY, EMOTE_ONESHOT_BEG, EMOTE_ONESHOT_EXCLAMATION, EMOTE_ONESHOT_POINT);
                m_say = 0;

                if (Creature* npc = ObjectAccessor::GetCreature(*me, m_citizenGUID))
                    npc->HandleEmoteCommand(m_emote);

                m_events.ScheduleEvent(EVENT_TALK_WITH_CITIZEN_1, urand(2200, 3000));
                break;
            }
            case EVENT_TALK_WITH_CITIZEN_1:
            {
                if (Creature* npc = ObjectAccessor::GetCreature(*me, m_citizenGUID))
                    npc->AI()->Talk(m_say);

                m_events.ScheduleEvent(EVENT_TALK_WITH_CITIZEN_2, 5000);
                break;
            }
            case EVENT_TALK_WITH_CITIZEN_2:
            {
                Talk(m_say);
                m_events.ScheduleEvent(EVENT_TALK_WITH_CITIZEN_3, 5000);
                break;
            }
            case EVENT_TALK_WITH_CITIZEN_3:
            {
                if (Creature* npc = ObjectAccessor::GetCreature(*me, m_citizenGUID))
                    npc->HandleEmoteCommand(EMOTE_STATE_NONE);

                m_events.ScheduleEvent(EVENT_START_TALK_WITH_CITIZEN, urand(5000, 30000));
                break;
            }
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void FillCitizenList()
    {
        listOfCitizenGUID.clear();
        std::list<Creature*> listOfCitizen;
        me->GetCreatureListWithEntryInGrid(listOfCitizen, NPC_PANICKED_CITIZEN_GATE, 35.0f);

        for (std::list<Creature*>::iterator itr = listOfCitizen.begin(); itr != listOfCitizen.end(); ++itr)
            listOfCitizenGUID.push_back((*itr)->GetGUID());
    }

    ObjectGuid GetRandomCitizen()
    {
        if (listOfCitizenGUID.empty())
            FillCitizenList();

        uint8 rol = urand(0, listOfCitizenGUID.size() - 1);
        std::list<ObjectGuid>::iterator itr = listOfCitizenGUID.begin();
        std::advance(itr, rol);
        return (*itr);
    }

private:
    std::list<ObjectGuid> listOfCitizenGUID;
};

struct npc_prince_liam_greymane : public ScriptedAI
{
    npc_prince_liam_greymane(Creature* c) : ScriptedAI(c) { }

    EventMap _events;

    void Reset() override
    {
        _events.RescheduleEvent(EVENT_START_DIALOG, 1000);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_START_DIALOG:
            {
                _events.ScheduleEvent(EVENT_RESET_DIALOG, 120000);
                _events.ScheduleEvent(EVENT_START_TALK_TO_GUARD, 1000);
                break;
            }
            case EVENT_START_TALK_TO_GUARD:
            {
                Talk(PRINCE_LIAM_GREYMANE_TEXT_00);
                _events.ScheduleEvent(EVENT_TALK_TO_GUARD_1, 15000);
                break;
            }
            case EVENT_TALK_TO_GUARD_1:
            {
                Talk(PRINCE_LIAM_GREYMANE_TEXT_01);
                _events.ScheduleEvent(EVENT_TALK_TO_GUARD_2, 18000);
                break;
            }
            case EVENT_TALK_TO_GUARD_2:
            {
                Talk(PRINCE_LIAM_GREYMANE_TEXT_02);
                break;
            }
            case EVENT_RESET_DIALOG:
            {
                Reset();
                break;
            }
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

struct npc_worgen_runt : public ScriptedAI
{
    npc_worgen_runt(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _worgenID = 0;
        _wayPointCounter = 0;
        _jumped = false;
        _playerGuid = 0;
    }

    void EnterCombat(Unit* /*who*/) override
    {
        _events.SetPhase(PHASE_COMBAT);
    }

    void IsSummonedBy(Unit* summoner) override
    {
        me->setActive(true);
        _events.SetPhase(PHASE_ROOF);
        _events.ScheduleEvent(EVENT_FORCE_DESPAWN, 70000, 0, PHASE_ROOF);
        _playerGuid = summoner->GetGUID();

        if (me->GetEntry() == NPC_WORGEN_RUNT_2)
            me->GetMotionMaster()->MoveJump(runt2SummonJumpPos, 16.0f, 4.371286f);

        if (me->GetEntry() == NPC_WORGEN_ALPHA)
        {
            me->GetMotionMaster()->MoveJump(alphaSummonJumpPos, 14.0f, 5.395287f);

            AddGilneasDelayedEvent(me, 2500, [this]()
                {
                    me->GetMotionMaster()->MovePoint(0, -1676.767944f, 1442.418457f, 52.287319f, true);
                });
        }

        if (me->GetEntry() == NPC_WORGEN_ALPHA_2)
            me->GetMotionMaster()->MoveJump(alpha2SummonJumpPos, 17.0f, 4.937166f);
    }

    void DoAction(int32 action) override
    {
        _worgenID = action;

        switch (_worgenID)
        {
        case WORGEN_ID_ROOF_1:
            _wayPointCounter = runtHousePathSize1;
            MoveGilneasPointPath(me, runtHousePathSize1, worgenRuntHousePath1, runtHousePathSize1);
            break;
        case WORGEN_ID_ROOF_2:
            _wayPointCounter = runtHousePathSize2;
            MoveGilneasPointPath(me, runtHousePathSize2, worgenRuntHousePath2, runtHousePathSize2);
            break;
        case WORGEN_ID_ROOF_3:
            _wayPointCounter = runtHousePathSize3;
            MoveGilneasPointPath(me, runtHousePathSize3, worgenRuntHousePath3, runtHousePathSize3);
            break;
        case WORGEN_ID_ROOF_4:
            _wayPointCounter = runtHousePathSize4;
            MoveGilneasPointPath(me, runtHousePathSize4, worgenRuntHousePath4, runtHousePathSize4);
            break;
        case WORGEN_ID_ROOF_5:
            _wayPointCounter = runtHousePathSize5;
            MoveGilneasPointPath(me, runtHousePathSize5, worgenRuntHousePath5, runtHousePathSize5);
            break;
        case WORGEN_ID_ROOF_6:
            _wayPointCounter = runtHousePathSize6;
            MoveGilneasPointPath(me, runtHousePathSize6, worgenRuntHousePath6, runtHousePathSize6);
            break;
        case WORGEN_ID_ROOF_7:
            _wayPointCounter = runtHousePathSize7;
            MoveGilneasPointPath(me, runtHousePathSize7, worgenRuntHousePath7, runtHousePathSize7);
            break;
        case WORGEN_ID_CATHEDRAL_1:
            _wayPointCounter = runtCathedralPathSize1;
            MoveGilneasPointPath(me, runtCathedralPathSize1, worgenRuntCathedralPath1, runtCathedralPathSize1);
            break;
        case WORGEN_ID_CATHEDRAL_2:
            _wayPointCounter = runtCathedralPathSize2;
            MoveGilneasPointPath(me, runtCathedralPathSize2, worgenRuntCathedralPath2, runtCathedralPathSize2);
            break;
        case WORGEN_ID_CATHEDRAL_3:
            _wayPointCounter = runtCathedralPathSize3;
            MoveGilneasPointPath(me, runtCathedralPathSize3, worgenRuntCathedralPath3, runtCathedralPathSize3);
            break;
        case WORGEN_ID_CATHEDRAL_4:
            _wayPointCounter = runtCathedralPathSize4;
            MoveGilneasPointPath(me, runtCathedralPathSize4, worgenRuntCathedralPath4, runtCathedralPathSize4);
            break;
        case WORGEN_ID_CATHEDRAL_5:
            _wayPointCounter = runtCathedralPathSize5;
            MoveGilneasPointPath(me, runtCathedralPathSize5, worgenRuntCathedralPath5, runtCathedralPathSize5);
            break;
        case WORGEN_ID_CATHEDRAL_6:
            _wayPointCounter = runtCathedralPathSize6;
            MoveGilneasPointPath(me, runtCathedralPathSize6, worgenRuntCathedralPath6, runtCathedralPathSize6);
            break;
        case WORGEN_ID_CATHEDRAL_7:
            _wayPointCounter = runtCathedralPathSize7;
            MoveGilneasPointPath(me, runtCathedralPathSize7, worgenRuntCathedralPath7, runtCathedralPathSize7);
            break;
        case WORGEN_ID_CATHEDRAL_8:
            _wayPointCounter = runtCathedralPathSize8;
            MoveGilneasPointPath(me, runtCathedralPathSize8, worgenRuntCathedralPath8, runtCathedralPathSize8);
            break;
        }
    }

    void JustDied(Unit* /*killer*/) override
    {
        me->DespawnOrUnsummon(5 * IN_MILLISECONDS);
    }

    void MovementInform(uint32 type, uint32 pointId) override
    {
        if ((type == POINT_MOTION_TYPE || type == EFFECT_MOTION_TYPE) && pointId == _wayPointCounter && !_jumped)
        {
            _jumped = true;
            _events.ScheduleEvent(EVENT_JUMP_TO_PRISON, 1000);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim() && !_events.IsInPhase(PHASE_ROOF))
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_FORCE_DESPAWN:
                me->DespawnOrUnsummon();
                break;
            case EVENT_JUMP_TO_PRISON:
                me->GetMotionMaster()->MoveJump(worgenRuntJumpPos[_worgenID], 16.0f, _worgenID < WORGEN_ID_CATHEDRAL_1 ? 19.2911f : frand(3.945607f, 4.852813f));
                me->SetHomePosition(worgenRuntJumpPos[_worgenID]);
                _events.ScheduleEvent(EVENT_AGGRO_PLAYER, 2000);
                break;
            case EVENT_AGGRO_PLAYER:
                if (Unit* player = ObjectAccessor::GetPlayer(*me, _playerGuid))
                    if (me->IsAIEnabled && me->GetHomePosition().IsInDist(player, 100.0f))
                        me->AI()->AttackStart(player);
                break;
            default:
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    uint32 _worgenID;
    uint32 _wayPointCounter;
    bool _jumped;
    ObjectGuid _playerGuid;
    EventMap _events;
};

class spell_gen_gilneas_prison_periodic_dummy : public SpellScript
{
    PrepareSpellScript(spell_gen_gilneas_prison_periodic_dummy);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        if (!sSpellMgr->GetSpellInfo(SPELL_SUMMON_RAVENOUS_WORGEN_1) ||
            !sSpellMgr->GetSpellInfo(SPELL_SUMMON_RAVENOUS_WORGEN_2))
            return false;
        return true;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
        {
            switch (RAND(0, 1))
            {
            case 0:
                caster->CastSpell(caster, SPELL_SUMMON_RAVENOUS_WORGEN_1, true);
                for (uint8 i = 0; i < 7; i++)
                    if (Creature* runt = caster->SummonCreature(NPC_WORGEN_RUNT, WorgenRuntHousePos[i]))
                        runt->AI()->DoAction(i);
                break;
            case 1:
                caster->CastSpell(caster, SPELL_SUMMON_RAVENOUS_WORGEN_2, true);
                for (uint8 i = 7; i < 16; i++)
                    if (Creature* runt = caster->SummonCreature(NPC_WORGEN_RUNT, WorgenRuntHousePos[i]))
                        runt->AI()->DoAction(i);
                if (RAND(0, 1) == 1)
                    for (uint8 i = 0; i < RAND(1, 3); i++)
                        if (Creature* runt = caster->SummonCreature(NPC_WORGEN_RUNT, WorgenRuntHousePos[i]))
                            runt->AI()->DoAction(i);
                break;
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_gen_gilneas_prison_periodic_dummy::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

struct npc_josiah_avery : public ScriptedAI
{
    npc_josiah_avery(Creature* creature) : ScriptedAI(creature) { }

    uint32 _text_timer;
    uint32 _current_text;

    void Reset() override
    {
        _text_timer = 20 * IN_MILLISECONDS;
        _current_text = 1;
    }

    void UpdateAI(uint32 diff) override
    {
        if (!me->GetVictim() && me->SelectNearestPlayer(20.0f))
        {
            if (_text_timer <= diff)
            {
                switch (_current_text)
                {
                case 1:
                    Talk(SAY_JOSIAH_AVERY_1);
                    _current_text++;
                    break;
                case 2:
                    Talk(SAY_JOSIAH_AVERY_2);
                    _current_text++;
                    break;
                case 3:
                    Talk(SAY_JOSIAH_AVERY_3);
                    _current_text++;
                    break;
                case 4:
                    Talk(SAY_JOSIAH_AVERY_4);
                    _current_text++;
                    break;
                case 5:
                    Talk(SAY_JOSIAH_AVERY_5);
                    _current_text++;
                    break;
                case 6:
                    Talk(SAY_JOSIAH_AVERY_6);
                    _current_text = 1;
                    break;
                }

                _text_timer = 20 * IN_MILLISECONDS;
            }
            else
            {
                _text_timer -= diff;
            }
            return;
        }
    }
};

struct npc_josiah_avery_worgen_form : public PassiveAI
{
    npc_josiah_avery_worgen_form(Creature* creature) : PassiveAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        _playerGuid = summoner->GetGUID();

        AddGilneasDelayedEvent(me, 200, [this, summoner]()
            {
                me->SetFacingToObject(summoner);
        _events.ScheduleEvent(EVENT_COSMETIC_ATTACK, 500);
            });
    }

    void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
    {
        if (spell->Id == SPELL_SHOOT_INSTAKILL)
            me->CastSpell(me, SPELL_GET_SHOT);
    }

    void JustDied(Unit* /*killer*/) override
    {
        me->DespawnOrUnsummon(5 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_COSMETIC_ATTACK:
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGuid))
                {
                    DoCast(player, SPELL_COSMETIC_ATTACK);

                    AddGilneasDelayedEvent(me, 400, [this, player]()
                        {
                            if (Creature* lorna = me->FindNearestCreature(NPC_LORNA_CROWLEY, 30.0f, true))
                            player->GetMotionMaster()->MoveKnockbackFrom(lorna->GetPositionX(), lorna->GetPositionY(), 30, 30);
                        });

                    if (Creature* lorna = me->FindNearestCreature(NPC_LORNA_CROWLEY, 30.0f, true))
                        if (Creature* labTrigger = lorna->FindNearestCreature(NPC_GENERIC_TRIGGER_LAB, 5.0f, true))
                            labTrigger->CastSpell(player, SPELL_PULL_TO);

                    _events.ScheduleEvent(EVENT_JUMP_TO_PLAYER, 1000);
                }
                break;
            case EVENT_JUMP_TO_PLAYER:
                me->GetMotionMaster()->MoveJump(josiahJumpPos, 10.0f, 14.18636f);
                _events.ScheduleEvent(EVENT_SHOOT_JOSIAH, 1200);
                break;
            case EVENT_SHOOT_JOSIAH:
                if (Creature* lorna = me->FindNearestCreature(NPC_LORNA_CROWLEY, 30.0f, true))
                    lorna->CastSpell(me, SPELL_SHOOT_INSTAKILL, true);
                break;
            default:
                break;
            }
        }
    }

private:
    ObjectGuid _playerGuid;
    EventMap _events;
};

class spell_gilneas_pull_to : public SpellScript
{
    PrepareSpellScript(spell_gilneas_pull_to);

    void HandPullEffect(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);

        if (Unit* playerTarget = GetHitPlayer())
        {
            if (Unit* trigger = GetCaster())
            {
                float angle = playerTarget->GetAngle(trigger);
                playerTarget->SendMoveKnockBack(playerTarget->ToPlayer(), 30.0f, -7.361481f, cos(angle), sin(angle));
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_gilneas_pull_to::HandPullEffect, EFFECT_0, SPELL_EFFECT_PULL_TOWARDS);
    }
};

class npc_lorna_crowley_basement : public CreatureScript
{
public:
    npc_lorna_crowley_basement(const char* ScriptName) : CreatureScript(ScriptName) { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_FROM_THE_SHADOWS)
        {
            if (player->getClass() == CLASS_HUNTER)
                player->UnsummonPetTemporaryIfAny();

            player->CastSpell(player, SPELL_SUMMON_MASTIFF, false);
            creature->AI()->Talk(0);
        }
        return true;
    }

    bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/)
    {
        if (quest->GetQuestId() == QUEST_FROM_THE_SHADOWS)
            if (Unit* charm = Unit::GetCreature(*creature, player->GetMinionGUID()))
                if (charm->GetEntry() == NPC_GILNEAS_MASTIFF)
                    if (Creature* mastiff = charm->ToCreature())
                        mastiff->DespawnOrUnsummon();

        if (player->getClass() == CLASS_HUNTER)
            player->ResummonPetTemporaryUnSummonedIfAny();

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lorna_crowley_basementAI(creature);
    }

    struct npc_lorna_crowley_basementAI : public ScriptedAI
    {
        npc_lorna_crowley_basementAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;
        }
    };
};

struct npc_gilnean_mastiff : public ScriptedAI
{
    npc_gilnean_mastiff(Creature* creature) : ScriptedAI(creature)
    {
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_ATTACK_LURKER, true);
    }

    void Reset() override
    {
        me->GetCharmInfo()->InitEmptyActionBar(false);
        me->GetCharmInfo()->SetActionBar(0, SPELL_ATTACK_LURKER, ACT_PASSIVE);
        me->SetReactState(REACT_DEFENSIVE);
        me->GetCharmInfo()->SetIsFollowing(true);
    }

    void UpdateAI(uint32 /*diff*/) override
    {
        Player* player = me->GetOwner()->ToPlayer();

        if (player->GetQuestStatus(QUEST_FROM_THE_SHADOWS) == QUEST_STATUS_REWARDED)
            me->DespawnOrUnsummon(1);

        if (!UpdateVictim())
        {
            me->GetCharmInfo()->SetIsFollowing(true);
            me->SetReactState(REACT_DEFENSIVE);
            return;
        }

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* /*killer*/) override
    {
        me->DespawnOrUnsummon(1);
    }

    void KilledUnit(Unit* /*victim*/) override
    {
        Reset();
    }
};

struct npc_bloodfang_lurker : public ScriptedAI
{
    npc_bloodfang_lurker(Creature* creature) : ScriptedAI(creature) { }

    bool _enrage;
    bool _frenzy;

    void Reset() override
    {
        _enrage = false;
        _frenzy = false;
        DoCast(me, SPELL_SHADOWSTALKER_STEALTH);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        me->SetReactState(REACT_PASSIVE);
    }

    void StartAttack(Unit* who)
    {
        me->SetReactState(REACT_AGGRESSIVE);
        me->SetInCombatWith(who);
        who->SetInCombatWith(me);
    }

    void DamageTaken(Unit* attacker, uint32& damage) override
    {
        if (me->HasReactState(REACT_PASSIVE))
            StartAttack(attacker);
    }

    void SpellHit(Unit* caster, const SpellInfo* spell) override
    {
        if (spell->Id == SPELL_ATTACK_LURKER)
            StartAttack(caster);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        if (!_frenzy && me->HealthBelowPct(45))
        {
            _frenzy = true;
            DoCast(SPELL_UNDYING_FRENZY);
        }

        if (!_enrage && me->HealthBelowPct(30))
        {
            _enrage = true;
            DoCast(SPELL_ENRAGE);
            Talk(WORGEN_ENRAGE_EMOTE);
        }

        DoMeleeAttackIfReady();
    }
};

class spell_attack_lurker : public SpellScript
{
    PrepareSpellScript(spell_attack_lurker);

    void HandleDummy()
    {
        Unit* caster = GetCaster();

        if (Creature* target = caster->FindNearestCreature(NPC_BLOODFANG_LURKER, 30.0f))
        {
            float x, y, z, o;
            target->GetContactPoint(caster, x, y, z, CONTACT_DISTANCE);
            o = caster->GetOrientation();
            float speedXY, speedZ;
            speedZ = 10.0f;
            speedXY = caster->GetExactDist2d(x, y) * 30.0f / speedZ;
            caster->GetMotionMaster()->MoveCharge(x, y, z, speedXY, speedZ);
            target->RemoveAura(SPELL_SHADOWSTALKER_STEALTH);
            target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            target->SetReactState(REACT_AGGRESSIVE);
            target->AI()->AttackStart(caster);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_attack_lurker::HandleDummy);
    }
};

class npc_king_genn_greymane : public CreatureScript
{
public:
    npc_king_genn_greymane() : CreatureScript("npc_king_genn_greymane") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_SAVE_KRENNAN_ARANAS)
        {
            float x, y;
            creature->GetNearPoint2D(x, y, 2.0f, player->GetOrientation() + M_PI / 2);

            if (Creature* horse = player->SummonCreature(NPC_KING_GREYMANES_HORSE, x, y, creature->GetPositionZ(), creature->GetOrientation()))
            {
                if (npc_escortAI* escort = CAST_AI(npc_escortAI, horse->AI()))
                {
                    escort->AddWaypoint(1, -1799.37f, 1400.21f, 19.8951f);
                    escort->AddWaypoint(2, -1798.23f, 1396.9f, 19.8993f);
                    escort->AddWaypoint(3, -1795.03f, 1388.01f, 19.8898f);
                    escort->AddWaypoint(4, -1790.16f, 1378.7f, 19.8016f);
                    escort->AddWaypoint(5, -1786.41f, 1372.97f, 19.8406f);
                    escort->AddWaypoint(6, -1779.72f, 1364.88f, 19.8131f);
                    escort->AddWaypoint(7, -1774.43f, 1359.87f, 19.7021f);
                    escort->AddWaypoint(8, -1769.0f, 1356.76f, 19.7439f);
                    escort->AddWaypoint(9, -1762.64f, 1356.02f, 19.7979f);
                    escort->AddWaypoint(10, -1758.91f, 1356.08f, 19.8635f);
                    escort->AddWaypoint(11, -1751.95f, 1356.8f, 19.8273f);
                    escort->AddWaypoint(12, -1745.66f, 1357.21f, 19.7993f);
                    escort->AddWaypoint(13, -1738.7f, 1356.64f, 19.7822f);
                    escort->AddWaypoint(14, -1731.79f, 1355.51f, 19.7149f);
                    escort->AddWaypoint(15, -1724.89f, 1354.29f, 19.8661f);
                    escort->AddWaypoint(16, -1718.03f, 1352.93f, 19.7824f);
                    escort->AddWaypoint(17, -1707.68f, 1351.16f, 19.7811f); // Jump
                    escort->AddWaypoint(18, -1673.04f, 1344.91f, 15.1353f, 2000);
                    escort->AddWaypoint(19, -1673.04f, 1344.91f, 15.1353f);
                    escort->AddWaypoint(20, -1669.32f, 1346.55f, 15.1353f);
                    escort->AddWaypoint(21, -1666.45f, 1349.89f, 15.1353f);
                    escort->AddWaypoint(22, -1665.61f, 1353.85f, 15.1353f);
                    escort->AddWaypoint(23, -1666.04f, 1358.01f, 15.1353f);
                    escort->AddWaypoint(24, -1669.79f, 1360.71f, 15.1353f);
                    escort->AddWaypoint(25, -1673.1f, 1362.11f, 15.1353f);
                    escort->AddWaypoint(26, -1677.12f, 1361.57f, 15.1353f);
                    escort->AddWaypoint(27, -1679.9f, 1360.1f, 15.1353f);
                    escort->AddWaypoint(28, -1682.79f, 1357.56f, 15.1353f);
                    escort->AddWaypoint(29, -1682.79f, 1357.56f, 15.1353f);
                    escort->AddWaypoint(30, -1689.07f, 1352.39f, 15.1353f);
                    escort->AddWaypoint(31, -1691.91f, 1351.83f, 15.1353f);
                    escort->AddWaypoint(32, -1703.81f, 1351.82f, 19.7604f);
                    escort->AddWaypoint(33, -1707.26f, 1352.38f, 19.7826f);
                    escort->AddWaypoint(34, -1712.25f, 1353.55f, 19.7826f);
                    escort->AddWaypoint(35, -1718.2f, 1356.51f, 19.7164f);
                    escort->AddWaypoint(36, -1741.5f, 1372.04f, 19.9569f);
                    escort->AddWaypoint(37, -1746.23f, 1375.8f, 19.9817f);
                    escort->AddWaypoint(38, -1751.06f, 1380.53f, 19.8424f);
                    escort->AddWaypoint(39, -1754.97f, 1386.34f, 19.8474f);
                    escort->AddWaypoint(40, -1760.77f, 1394.37f, 19.9282f);
                    escort->AddWaypoint(41, -1765.11f, 1402.07f, 19.8816f);
                    escort->AddWaypoint(42, -1768.24f, 1410.2f, 19.7833f);
                    escort->AddWaypoint(43, -1772.26f, 1420.48f, 19.9029f);
                    escort->AddWaypoint(44, -1776.98f, 1436.13f, 19.632f);
                    player->EnterVehicle(horse, 0);
                }
            }
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_king_genn_greymaneAI(creature);
    }

    struct npc_king_genn_greymaneAI : public ScriptedAI
    {
        npc_king_genn_greymaneAI(Creature* creature) : ScriptedAI(creature), _talkTimer(urand(15000, 35000))
        {
        }

        void UpdateAI(uint32 diff) override
        {
            if (_talkTimer <= diff)
            {
                Talk(0);
                _talkTimer = urand(15000, 35000);
            }
            else
                _talkTimer -= diff;
        }

    private:
        uint32 _talkTimer;
    };
};

class npc_vehicle_genn_horse : public CreatureScript
{
public:
    npc_vehicle_genn_horse() : CreatureScript("npc_vehicle_genn_horse") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_vehicle_genn_horseAI(creature);
    }

    struct npc_vehicle_genn_horseAI : public npc_escortAI
    {
        npc_vehicle_genn_horseAI(Creature* creature) : npc_escortAI(creature)
        {
            _aranasSaved = false;
            _playerSeated = false;
        }

        bool _playerSeated;
        bool _aranasSaved;

        void AttackStart(Unit* /*who*/) override { }
        void EnterCombat(Unit* /*who*/) override { }
        void EnterEvadeMode() override { }

        void Reset() override
        {
            _playerSeated = false;
        }

        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
        {
            if (who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                _playerSeated = true;

                if (apply)
                {
                    Start(false, true, who->GetGUID());
                    me->SetSpeed(MOVE_WALK, 1.0f, true);
                    me->SetSpeed(MOVE_RUN, 1.3f, true);
                }
            }
            else
            {
                if (apply)
                {
                    SetEscortPaused(false);
                    _aranasSaved = true;
                    me->SetSpeed(MOVE_WALK, 1.0f, true);
                    me->SetSpeed(MOVE_RUN, 1.3f, true);
                }
                else
                    me->DespawnOrUnsummon(1);
            }
        }

        void WaypointReached(uint32 i)
        {
            Player* player = GetPlayerForEscort();

            switch (i)
            {
            case 1:
            {
                if (Unit* passenger = me->GetVehicleKit()->GetPassenger(0))
                {
                    if (Vehicle* vehicle = me->GetVehicleKit())
                        if (Unit* passenger = vehicle->GetPassenger(0))
                            if (Player* player = passenger->ToPlayer())
                                player->SetClientControl(me, 0);
                }
                break;
            }
            case 17:
            {
                if (Unit* passenger = me->GetVehicleKit()->GetPassenger(0))
                {
                    if (Creature* aranas = passenger->FindNearestCreature(NPC_KRENNAN_ARANAS, 50.0f))
                        if (Vehicle* vehicle = me->GetVehicleKit())
                            if (Unit* passenger = vehicle->GetPassenger(0))
                                if (Player* player = passenger->ToPlayer())
                                    aranas->AI()->Talk(TEXT_KRENNAN_ARANAS);

                    me->GetMotionMaster()->MoveJump(-1673.04f, 1344.91f, 15.1353f, 25.0f, 15.0f);

                    AddGilneasDelayedEvent(me, 2000, [this, passenger]()
                        {
                            Talk(EMOTE_SAVE_KRENNAN_ARANAS, passenger);
                        });
                }
                break;
            }
            case 40:
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                {
                    if (Unit* passenger = vehicle->GetPassenger(0))
                    {
                        if (Player* player = passenger->ToPlayer())
                        {
                            std::list<Creature*> guards;
                            me->GetCreatureListWithEntryInGrid(guards, NPC_GILNEAS_CITY_GUARD, 90.0f);

                            if (!guards.empty())
                                for (std::list<Creature*>::const_iterator itr = guards.begin(); itr != guards.end(); ++itr)
                                    if ((*itr)->IsAlive())
                                        if (Creature* worgen = (*itr)->FindNearestCreature(NPC_BLOODFANG_RIPPER, 90.0f))
                                            (*itr)->CastSpell(worgen, SPELL_GUARD_SHOOT, false);
                        }
                    }
                }
                break;
            }
            case 41:
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->CombatStop();
                break;
            }
            case 42:
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                    if (Unit* passenger = vehicle->GetPassenger(0))
                        if (Player* player = passenger->ToPlayer())
                        {
                            player->KilledMonsterCredit(NPC_KRENNAN_ARANAS_KILL_CREDIT, 0);

                            if (Unit* passenger_2 = vehicle->GetPassenger(1))
                                if (Creature* aranas = passenger_2->ToCreature())
                                    aranas->AI()->Talk(TEXT_SAVED_KRENNAN_ARANAS);
                        }
                break;
            }
            case 44:
            {
                if (Unit* passenger = me->GetVehicleKit()->GetPassenger(0))
                {
                    if (Vehicle* vehicle = me->GetVehicleKit())
                    {
                        if (Unit* passenger = vehicle->GetPassenger(0))
                            if (Player* player = passenger->ToPlayer())
                                player->SetClientControl(me, 1);

                        if (Unit* passenger = vehicle->GetPassenger(1))
                            if (Creature* aranas = passenger->ToCreature())
                                aranas->DespawnOrUnsummon();

                        vehicle->RemoveAllPassengers();
                    }
                }
                me->DespawnOrUnsummon(1);
                break;
            }
            }
        }

        void OnCharmed(bool /*apply*/) { }

        void UpdateAI(uint32 diff) override
        {
            npc_escortAI::UpdateAI(diff);
            Player* player = GetPlayerForEscort();

            if (_playerSeated)
            {
                player->SetClientControl(me, 0);
                _playerSeated = false;
            }
        }
    };
};

struct npc_saved_aranas : public ScriptedAI
{
    npc_saved_aranas(Creature* creature) : ScriptedAI(creature)
    {
        me->SetReactState(REACT_PASSIVE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (me->IsSummon())
        {
            if (Creature* horse = me->FindNearestCreature(NPC_KING_GREYMANES_HORSE, 20.0f))
                DoCast(horse, 84275, true);

            if (Creature* aranas = me->FindNearestCreature(NPC_KRENNAN_ARANAS, 50.0f))
                aranas->DespawnOrUnsummon();
        }
    }
};

class npc_gilneas_children : public CreatureScript
{
public:
    npc_gilneas_children(const char* scriptName, uint32 spellId, uint8 playerSayId) : CreatureScript(scriptName), _spellId(spellId), _playerSayId(playerSayId) { }

private:
    uint32 _spellId;
    uint8 _playerSayId;

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new creature_script(creature, _spellId, _playerSayId);
    }

    struct creature_script : public ScriptedAI
    {
        creature_script(Creature* creature, uint32 spellId, uint8 playerSayId) : ScriptedAI(creature), _spellId(spellId), _playerSayId(playerSayId) { }

        EventMap events;
        ObjectGuid playerGUID;
        uint32 _spellId;
        uint8 _playerSayId;
        bool activated;

        void Reset() override
        {
            events.Reset();
            playerGUID = 0;
            activated = false;
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

            if (me->GetEntry() == NPC_CYNTHIA)
                events.ScheduleEvent(EVENT_CRY, 1000);
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (!activated && spell->Id == _spellId)
            {
                if (Player* player = caster->ToPlayer())
                {
                    activated = true;
                    playerGUID = player->GetGUID();
                    player->Say(PlayerText[_playerSayId], Language::LANG_UNIVERSAL);
                    player->KilledMonsterCredit(me->GetEntry(), 0);
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    events.ScheduleEvent(EVENT_TALK_TO_PLAYER, 3500);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_TALK_TO_PLAYER:
                {
                    events.CancelEvent(EVENT_CRY);

                    if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                        Talk(CHILDREN_TEXT_ID, player);

                    events.ScheduleEvent(EVENT_START_RUN, 5000);
                }
                break;
                case EVENT_START_RUN:
                {
                    switch (me->GetEntry())
                    {
                    case NPC_JAMES:
                        MoveGilneasPointPath(me, 0, JamesPath[0], JamesPathLenght);
                        break;
                    case NPC_CYNTHIA:
                        MoveGilneasPointPath(me, 0, CynthiaPath[0], CynthiaPathLenght);
                        break;
                    case NPC_ASHLEY:
                        MoveGilneasPointPath(me, 0, AshleyPath[0], AshleyPathLenght);
                        break;
                    default:
                        return;
                    }

                    events.ScheduleEvent(EVENT_OPEN_DOOR, 5000);
                }
                break;
                case EVENT_OPEN_DOOR:
                {
                    if (GameObject* door = me->FindNearestGameObject(GO_DOOR_TO_THE_BASEMENT, 10.0f))
                    {
                        if (door->GetGoState() == GOState::GO_STATE_READY)
                        {
                            door->UseDoorOrButton();
                            events.ScheduleEvent(EVENT_RESUME_RUN, 2000);
                        }
                        else
                            events.ScheduleEvent(EVENT_RESUME_RUN, 0);
                    }
                }
                break;
                case EVENT_RESUME_RUN:
                    MoveGilneasPointPath(me, 0, childrenBasementPath[0], childrenBasementPathLenght);
                    me->DespawnOrUnsummon(5000);
                    break;
                case EVENT_CRY:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_CRY);
                    events.ScheduleEvent(EVENT_CRY, urand(1000, 1500));
                    break;
                }
            }
        }
    };
};

class npc_wahl : public CreatureScript
{
public:
    npc_wahl(const char* ScriptName) : CreatureScript(ScriptName) { }

    struct npc_wahlAI : public npc_escortAI
    {
        npc_wahlAI(Creature* creature) : npc_escortAI(creature)
        {
            creature->SetReactState(REACT_PASSIVE);
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void DoAction(int32 const action) override
        {
            if (action == ACTION_CONTINUE_SCENE)
            {
                AddGilneasDelayedEvent(me, 4000, [this]()
                    {
                        SetEscortPaused(false);
                    });
            }
        }

        void WaypointReached(uint32 point)
        {
            if (point == 1)
                if (me->IsSummon())
                    if (Unit* summoner = me->ToTempSummon()->GetSummoner())
                    {
                        SetEscortPaused(true);
                        me->SetDisplayId(NPC_WAHL_WORGEN);
                        Talk(YELL_DONT_MESS);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);

                        AddGilneasDelayedEvent(me, 800, [this, summoner]()
                            {
                                AttackStart(summoner);
                            });
                    }
        }

        void UpdateAI(uint32 diff) override
        {
            npc_escortAI::UpdateAI(diff);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_wahlAI(creature);
    }
};

class npc_lucius_the_cruel : public CreatureScript
{
public:
    npc_lucius_the_cruel(const char* ScriptName) : CreatureScript(ScriptName) { }

    struct npc_lucius_the_cruelAI : public ScriptedAI
    {
        npc_lucius_the_cruelAI(Creature* creature) : ScriptedAI(creature)
        {
            SetCombatMovement(false);
            Catch = false;
            Summon = false;
            uiCatchTimer = 1000;
            uiShootTimer = 500;
            uiPlayerGUID = 0;
            uiSummonTimer = 1500;
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        }

        ObjectGuid uiPlayerGUID;
        uint32 uiCatchTimer;
        uint32 uiShootTimer;
        uint32 uiSummonTimer;
        bool Catch;
        bool Summon;

        void EnterEvadeMode() override
        {
            me->DespawnOrUnsummon();
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* wahl = me->FindNearestCreature(NPC_WAHL, 30.f))
                wahl->AI()->DoAction(ACTION_CONTINUE_SCENE);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == POINT_CATCH_CHANCE)
            {
                me->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);

                if (me->IsSummon())
                    if (Unit* summoner = me->ToTempSummon()->GetSummoner())
                        if (Creature* chance = summoner->ToCreature())
                        {
                            AddGilneasDelayedEvent(me, 4000, [this, chance]()
                                {
                                    Catch = true;
                                    Summon = true;
                                    chance->AI()->DoAction(ACTION_CHANCE_DESPAWN);
                                });
                        }
            }
        }

        void DoAction(int32 const action) override
        {
            if (action == ACTION_SUMMON_LUCIUS)
            {
                me->GetMotionMaster()->MovePoint(POINT_CATCH_CHANCE, -2106.372f, 2331.106f, 7.360674f);

                AddGilneasDelayedEvent(me, 1000, [this]()
                    {
                        Talk(SAY_THIS_CAT_IS_MINE);
                    });
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (Catch)
            {
                if (uiCatchTimer <= diff)
                {
                    Catch = false;
                    uiCatchTimer = 1000;
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                    me->SetReactState(REACT_AGGRESSIVE);

                    if (Player* player = Unit::GetPlayer(*me, uiPlayerGUID))
                        AttackStart(player);
                }
                else
                    uiCatchTimer -= diff;
            }

            if (Summon)
            {
                if (uiSummonTimer <= diff)
                {
                    Summon = false;
                    uiSummonTimer = 1500;

                    if (Creature* wahl = me->SummonCreature(NPC_WAHL, -2098.366f, 2352.075f, 7.160643f))
                    {
                        if (npc_escortAI* npc_escort = CAST_AI(npc_wahl::npc_wahlAI, wahl->AI()))
                        {
                            npc_escort->AddWaypoint(0, -2106.54f, 2342.69f, 6.93668f);
                            npc_escort->AddWaypoint(1, -2106.12f, 2334.90f, 7.36691f);
                            npc_escort->AddWaypoint(2, -2117.80f, 2357.15f, 5.88139f);
                            npc_escort->AddWaypoint(3, -2111.46f, 2366.22f, 7.17151f);
                            npc_escort->Start(false, true);
                        }
                    }
                }
                else
                    uiSummonTimer -= diff;
            }

            if (!UpdateVictim())
                return;

            if (uiShootTimer <= diff)
            {
                uiShootTimer = 1000;

                if (me->GetDistance(me->GetVictim()) > 2.0f)
                    DoCastVictim(SPELL_LUCIUS_SHOOT);
            }
            else
                uiShootTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lucius_the_cruelAI(creature);
    }
};

struct npc_chance_the_cat : public ScriptedAI
{
    npc_chance_the_cat(Creature* creature) : ScriptedAI(creature)
    {
        Despawn = false;
        uiDespawnTimer = 500;
    }

    uint32 uiDespawnTimer;
    bool Despawn;

    void DoAction(int32 const action) override
    {
        if (action == ACTION_CHANCE_DESPAWN)
            Despawn = true;
    }

    void SpellHit(Unit* caster, const SpellInfo* spell) override
    {
        if (spell->Id == SPELL_CATCH_CAT && caster->GetTypeId() == TypeID::TYPEID_PLAYER)
        {
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

            if (Creature* lucius = me->SummonCreature(NPC_LUCIUS_THE_CRUEL, -2111.533f, 2329.95f, 7.390349f))
            {
                lucius->AI()->DoAction(ACTION_SUMMON_LUCIUS);
                CAST_AI(npc_lucius_the_cruel::npc_lucius_the_cruelAI, lucius->AI())->uiPlayerGUID = caster->GetGUID();
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (Despawn)
        {
            if (uiDespawnTimer <= diff)
            {
                uiDespawnTimer = 500;
                Despawn = false;
                me->DespawnOrUnsummon();
            }
            else
                uiDespawnTimer -= diff;
        }
    }
};

class npc_mountain_horse : public CreatureScript
{
public:
    npc_mountain_horse(const char* ScriptName) : CreatureScript(ScriptName) { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_THE_HUNGRY_ETTIN) == QUEST_STATUS_INCOMPLETE)
        {
            player->EnterVehicle(creature, 0);
            creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            creature->GetMotionMaster()->Clear();
            return true;
        }
        return true;
    }

    struct npc_mountain_horseAI : public ScriptedAI
    {
        npc_mountain_horseAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        void Reset() override
        {
            if (!me->GetVehicleKit()->GetPassenger(0))
                me->GetMotionMaster()->MoveRandom(8.0f);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            switch (spell->Id)
            {
            case SPELL_ROUND_UP_HORSE:
            {
                if (me->GetVehicleKit()->GetPassenger(0))
                    break;

                me->DespawnOrUnsummon(1);
                break;
            }
            default:
                break;
            }
        }

        void PassengerBoarded(Unit* /*passenger*/, int8 /*SeatId*/, bool apply)
        {
            if (!apply)
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->SetWalk(true);
                me->AI()->EnterEvadeMode();
            }
        }

        void OnCharmed(bool apply) { }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_mountain_horseAI(creature);
    }
};

struct npc_mountain_horse_summoned : public ScriptedAI
{
    npc_mountain_horse_summoned(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void IsSummonedBy(Unit* summoner) override
    {
        me->GetMotionMaster()->MoveFollow(summoner, 6.0f, 0);
        me->CastSpell(summoner, SPELL_ROPE_CHANNEL, true);
        me->ClearUnitState(UNIT_STATE_CASTING);
        events.ScheduleEvent(EVENT_CHECK_LORNA, 2000);
        events.ScheduleEvent(EVENT_CHECK_OWNER, 2000);
        me->SetWalk(false);
        me->SetSpeed(MOVE_RUN, 2.0f, true);
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_CHECK_LORNA:
            {
                if (Creature* lornaCrowley = me->FindNearestCreature(NPC_LORNA_CROWLEY_2, 8.0f, true))
                {
                    if (Unit* owner = me->GetCharmerOrOwner())
                    {
                        if (owner->GetTypeId() == TypeID::TYPEID_PLAYER)
                            owner->ToPlayer()->KilledMonsterCredit(QUEST_CREDIT_HORSE);
                    }
                    events.CancelEvent(EVENT_CHECK_LORNA);
                    me->DespawnOrUnsummon(1);
                }
                else
                    events.RescheduleEvent(EVENT_CHECK_LORNA, 2000);
                break;
            }
            case EVENT_CHECK_OWNER:
            {
                if (Unit* owner = me->GetCharmerOrOwner())
                {
                    if (!owner->IsAlive() || !owner->IsInWorld() || !owner->GetVehicleBase())
                        me->DespawnOrUnsummon(1);

                    events.CancelEvent(EVENT_CHECK_OWNER);
                }
                else
                    events.RescheduleEvent(EVENT_CHECK_OWNER, 2000);
                break;
            }
            default:
                break;
            }
        }
    }
};

class spell_gilneas_test_telescope : public SpellScript
{
    PrepareSpellScript(spell_gilneas_test_telescope);

    void StartCinematic()
    {
        if (Unit* caster = GetCaster())
        {
            if (caster->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                caster->PlayDirectSound(MUSIC_ENTRY_TELESCOPE, caster->ToPlayer());
                caster->ToPlayer()->SendCinematicStart(PLAY_CINEMATIC_TELESCOPE);
            }
        }
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_gilneas_test_telescope::StartCinematic);
    }
};

class npc_stagecoach_carriage_exodus : public CreatureScript
{
public:
    npc_stagecoach_carriage_exodus(const char* ScriptName) : CreatureScript(ScriptName) { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_ENTRY_EXODUS) == QUEST_STATUS_COMPLETE)
        {
            if (player->GetVehicleBase())
                return true;

            if (Creature* harness = player->FindNearestCreature(NPC_STAGECOACH_HARNESS, 30.0f, true))
            {
                Position summonPos(harness->GetPositionX(), harness->GetPositionY(), harness->GetPositionZ(), harness->GetOrientation());
                player->GetMap()->SummonCreature(NPC_HARNESS_SUMMONED, summonPos, sSummonPropertiesStore.LookupEntry(3105), 600000, player);
            }
            return true;
        }

        return true;
    }

    struct npc_stagecoach_carriage_exodusAI : public ScriptedAI
    {
        npc_stagecoach_carriage_exodusAI(Creature* creature) : ScriptedAI(creature)
        {
            events.ScheduleEvent(EVENT_BOARD_HARNESS_OWNER, 1);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
        }

        EventMap events;

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BOARD_HARNESS_OWNER:
                {
                    if (Unit* harness = me->GetVehicleCreatureBase())
                    {
                        if (Unit* harnessOwner = harness->ToTempSummon()->GetSummoner())
                        {
                            if (harnessOwner->IsAlive() && harnessOwner->IsInWorld())
                            {
                                harnessOwner->EnterVehicle(me, 1);
                                events.CancelEvent(EVENT_BOARD_HARNESS_OWNER);
                                break;
                            }
                        }
                    }
                    events.CancelEvent(EVENT_BOARD_HARNESS_OWNER);
                    break;
                }
                default:
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_stagecoach_carriage_exodusAI(creature);
    }
};

class npc_stagecoach_harness : public CreatureScript
{
public:
    npc_stagecoach_harness(const char* ScriptName) : CreatureScript(ScriptName) { }

    struct npc_stagecoach_harnessAI : public npc_escortAI
    {
        npc_stagecoach_harnessAI(Creature* creature) : npc_escortAI(creature) { }

        void OnCharmed(bool apply) { }

        void IsSummonedBy(Unit* owner)
        {
            DoAction(ACTION_START_WP);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
        }

        void DoAction(int32 action)
        {
            switch (action)
            {
            case ACTION_START_WP:
            {
                Start(false, true, NULL, NULL, false, false, true);
                SetDespawnAtEnd(true);

                if (GameObject* gate = me->FindNearestGameObject(GO_FIRST_GATE, 80.0f))
                    gate->UseDoorOrButton(0, false, me);

                me->SetWalk(false);
                me->SetSpeed(MOVE_RUN, 1.34f, true);
                break;
            }
            default:
                break;
            }
        }

        void WaypointReached(uint32 point) override
        {
            switch (point)
            {
            case 16:
            {
                if (GameObject* gate = me->FindNearestGameObject(GO_KINGS_GATE, 80.0f))
                    gate->UseDoorOrButton(0, false, me);
                break;
            }
            case 24:
            {
                if (Unit* caravan = me->GetVehicleKit()->GetPassenger(2))
                {
                    if (Unit* lorna = caravan->GetVehicleKit()->GetPassenger(6))
                    {
                        if (lorna->ToCreature())
                            lorna->ToCreature()->AI()->Talk(0);
                    }
                }
                break;
            }
            case 30:
            {
                if (Unit* caravan = me->GetVehicleKit()->GetPassenger(2))
                {
                    if (Unit* player = caravan->GetVehicleKit()->GetPassenger(1))
                        player->ExitVehicle();
                }
                break;
            }
            default:
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_stagecoach_harnessAI(creature);
    }
};

enum eZoneGilneas
{
    NPC_WORGEN_ALPHA_C2 = 35167,
    NPC_WORGEN_ALPHA_C1 = 35170,
    NPC_WORGEN_RUNT_C1 = 35188,
    NPC_WORGEN_RUNT_C2 = 35456,

    SPELL_BY_THE_SKIN = 66914,
    SPELL_ENRAGE_8599 = 8599,
    SPELL_GILNEAS_PRISON_PERIODIC_1 = 66894,
    SPELL_GILNEAS_PRISON_PERIODIC_2 = 68218,
};

/* 35077 - QUEST: 14154 - By The Skin of His Teeth - START */
class npc_lord_darius_crowley_35077 : public CreatureScript
{
public:
    npc_lord_darius_crowley_35077() : CreatureScript("npc_lord_darius_crowley_35077") { }

    enum eNpc
    {
        ACTION_START_EVENT = 101,
    };

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == 14154)
            if (CAST_AI(npc_lord_darius_crowley_35077AI, creature->AI())->m_playerGUID == 0)
            {
                creature->AI()->SetGUID(player->GetGUID());
                creature->AI()->DoAction(ACTION_START_EVENT);
                creature->CastSpell(player, SPELL_BY_THE_SKIN, true);
            }
        return true;
    }

    struct npc_lord_darius_crowley_35077AI : public ScriptedAI
    {
        npc_lord_darius_crowley_35077AI(Creature* creature) : ScriptedAI(creature), m_summons(me) { Init(); }

        enum eQ14154
        {
            Event120Secounds = 1,
            EventCheckPlayerIsAlive,
            EventSummonNextWave,
            EventHelpPlayer,
        };

        ObjectGuid m_playerGUID;
        EventMap m_events;
        SummonList m_summons;
        uint32 m_phase;

        void Init()
        {
            m_events.Reset();
            m_summons.DespawnAll();
            m_playerGUID = 0;
            m_phase = 0;
        }

        void JustSummoned(Creature* summoned) override
        {
            m_summons.Summon(summoned);
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) override
        {
            m_summons.Despawn(summon);
        }

        void DoAction(int32 /*action*/) override
        {
            m_phase = 1;
            m_events.ScheduleEvent(EventCheckPlayerIsAlive, 1000);
            m_events.ScheduleEvent(EventSummonNextWave, 1000);
            m_events.ScheduleEvent(Event120Secounds, 120000);
            m_events.ScheduleEvent(EventHelpPlayer, 250);
        }

        void SetGUID(uint64 guid, int32 /*type = 0*/) override
        {
            m_playerGUID = guid;
        }

        void DamageTaken(Unit* attacker, uint32& /*damage*/) override
        {
            if (/*Creature* worgen = */attacker->ToCreature())
                if (!me->IsInCombat())
                {
                    //me->Attack(worgen, true);
                }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case Event120Secounds:
                    Init();
                    break;
                case EventCheckPlayerIsAlive: // check every sec player is alive
                    if (!m_playerGUID == 0 && m_phase)
                        if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                            if (!player->IsInWorld() || !player->IsAlive())
                                Init();

                    m_events.ScheduleEvent(EventCheckPlayerIsAlive, 1000);
                    break;
                case EventSummonNextWave:
                {
                    for (int i = 0; i < 4; i++)
                    {
                        // Entry 35456 is used by the prison periodic wave handler, so keep
                        // the Darius event on entries that use the Darius wave movement AI.
                        uint32 w1 = RAND(NPC_WORGEN_RUNT_C1, NPC_WORGEN_ALPHA_C1, NPC_WORGEN_ALPHA_C2);
                        uint32 w2 = RAND(NPC_WORGEN_RUNT_C1, NPC_WORGEN_ALPHA_C1, NPC_WORGEN_ALPHA_C2);

                        if (Creature* creature1 = me->SummonCreature(w1, -1610.39f, 1507.16f, 74.99f, 3.94f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 120000))
                        {
                            m_summons.Summon(creature1);
                            creature1->AI()->SetGUID(m_playerGUID);
                            creature1->AI()->DoAction(1);
                        }

                        if (Creature* creature2 = me->SummonCreature(w2, -1718.01f, 1516.81f, 55.40f, 4.6f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 120000))
                        {
                            m_summons.Summon(creature2);
                            creature2->AI()->SetGUID(m_playerGUID);
                            creature2->AI()->DoAction(2);
                        }
                    }

                    m_events.ScheduleEvent(EventSummonNextWave, 30000); // every 30 secounds one wave
                    break;
                }
                case EventHelpPlayer:
                {
                    if (!me->IsInCombat())
                    {
                        Creature* creature = nullptr;
                        creature = me->FindNearestCreature(NPC_WORGEN_RUNT_C1, 5.0f);
                        if (!creature)
                            creature = me->FindNearestCreature(NPC_WORGEN_RUNT_C2, 5.0f);
                        if (!creature)
                            creature = me->FindNearestCreature(NPC_WORGEN_ALPHA_C1, 5.0f);
                        if (!creature)
                            creature = me->FindNearestCreature(NPC_WORGEN_ALPHA_C2, 5.0f);
                        if (creature)
                        {
                            me->Attack(creature, true);
                            // creature->Attack(me, true);
                        }
                    }

                    m_events.ScheduleEvent(EventHelpPlayer, 250);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lord_darius_crowley_35077AI(creature);
    }
};

// 35124
class npc_tobias_mistmantle_35124 : public CreatureScript
{
public:
    npc_tobias_mistmantle_35124() : CreatureScript("npc_tobias_mistmantle_35124") { }

    struct npc_tobias_mistmantle_35124AI : public ScriptedAI
    {
        npc_tobias_mistmantle_35124AI(Creature* creature) : ScriptedAI(creature) { }

        void UpdateAI(uint32 /*diff*/) override
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_tobias_mistmantle_35124AI(creature);
    }
};

// 35188
class npc_worgen_runt_35188 : public CreatureScript
{
public:
    npc_worgen_runt_35188() : CreatureScript("npc_worgen_runt_35188") {}

    struct npc_worgen_runt_35188AI : public ScriptedAI
    {
        npc_worgen_runt_35188AI(Creature* creature) : ScriptedAI(creature) { Init(); }

        ObjectGuid m_playerGUID;
        EventMap m_events;
        uint32 m_phase;
        Position jump;
        Position JumpW1[3];
        Position LandingW1[3];
        Position LandingW2[4];

        void Init()
        {
            m_phase = 0;
            JumpW1[0] = Position{ -1643.91f, 1482.96f, 63.22f, 3.95f };
            JumpW1[1] = Position{ -1638.83f, 1478.07f, 65.36f, 3.84f };
            JumpW1[2] = Position{ -1631.49f, 1475.02f, 65.64f, 3.84f };

            LandingW1[0] = Position{ -1677.39f, 1455.52f, 52.29f, 4.06f };
            LandingW1[1] = Position{ -1671.89f, 1449.27f, 52.29f, 3.84f };
            LandingW1[2] = Position{ -1664.82f, 1443.62f, 52.29f, 3.84f };

            LandingW2[0] = Position{ -1704.93f, 1469.07f, 52.29f, 5.34f };
            LandingW2[1] = Position{ -1698.90f, 1472.92f, 52.29f, 5.41f };
            LandingW2[2] = Position{ -1703.37f, 1470.66f, 52.29f, 5.49f };
            LandingW2[3] = Position{ -1700.37f, 1473.32f, 52.29f, 5.41f };
        }

        void DoAction(int32 action) override
        {
            if (!m_playerGUID == 0)
            {
                m_events.ScheduleEvent(1, 500);
                m_phase = action;
                me->SetSpeed(MOVE_RUN, frand(1.2f, 1.6f));
            }
        }

        void SetGUID(uint64 guid, int32 /*type*/) override
        {
            m_playerGUID = guid;
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE || type == EFFECT_MOTION_TYPE)
                m_phase = pointId;
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            uint32 eventId = m_events.ExecuteEvent();
            switch (eventId)
            {
            case 1:
            {
                m_events.ScheduleEvent(1, 500);
                DoWalk();
                break;
            }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void DoWalk()
        {
            switch (m_phase)
            {
            case 1:
            {
                m_phase = 3;
                uint8 rol = urand(0, 2);
                jump = JumpW1[rol];
                me->GetMotionMaster()->MovePoint(11 + rol, jump);
                break;
            }
            case 2:
                m_phase = 3;
                jump = Position{ -1717.73f, 1486.27f, 57.23f, 5.45f };
                me->GetMotionMaster()->MovePoint(21, jump);
                break;
            case 11:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[0], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 12:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[1], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 13:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[2], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 21:
            {
                m_phase = 5;
                uint8 rol = urand(0, 3);
                jump = LandingW2[rol];
                me->GetMotionMaster()->MoveJump(jump, frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            }
            case 25:
                m_phase = 6;
                if (!m_playerGUID == 0)
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        if (player->IsInWorld() || player->IsAlive())
                        {
                            Position pos;
                            player->GetNearPosition(pos, frand(2.0f, 4.0f), frand(3.14f, 6.28f));
                            me->GetMotionMaster()->MovePoint(26, pos);
                        }
                break;
            case 26:
                m_phase = 7;
                me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_worgen_runt_35188AI(creature);
    }
};

// 35456
class npc_worgen_runt_35456 : public CreatureScript
{
public:
    npc_worgen_runt_35456() : CreatureScript("npc_worgen_runt_35456") { }

    struct npc_worgen_runt_35456AI : public ScriptedAI
    {
        npc_worgen_runt_35456AI(Creature* creature) : ScriptedAI(creature) { Init(); }

        ObjectGuid m_playerGUID;
        EventMap m_events;
        uint32 m_phase;
        Position jump;
        Position JumpW1[3];
        Position LandingW1[3];
        Position LandingW2[4];

        void Init()
        {
            m_phase = 0;
            JumpW1[0] = Position{ -1643.91f, 1482.96f, 63.22f, 3.95f };
            JumpW1[1] = Position{ -1638.83f, 1478.07f, 65.36f, 3.84f };
            JumpW1[2] = Position{ -1631.49f, 1475.02f, 65.64f, 3.84f };

            LandingW1[0] = Position{ -1677.39f, 1455.52f, 52.29f, 4.06f };
            LandingW1[1] = Position{ -1671.89f, 1449.27f, 52.29f, 3.84f };
            LandingW1[2] = Position{ -1664.82f, 1443.62f, 52.29f, 3.84f };

            LandingW2[0] = Position{ -1704.93f, 1469.07f, 52.29f, 5.34f };
            LandingW2[1] = Position{ -1698.90f, 1472.92f, 52.29f, 5.41f };
            LandingW2[2] = Position{ -1703.37f, 1470.66f, 52.29f, 5.49f };
            LandingW2[3] = Position{ -1700.37f, 1473.32f, 52.29f, 5.41f };
        }

        void DoAction(int32 action) override
        {
            if (!m_playerGUID == 0)
            {
                m_events.ScheduleEvent(1, 500);
                m_phase = action;
                me->SetSpeed(MOVE_RUN, frand(1.2f, 1.6f));
            }
        }

        void SetGUID(uint64 guid, int32 /*type*/) override
        {
            m_playerGUID = guid;
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE || type == EFFECT_MOTION_TYPE)
                m_phase = pointId;
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            uint32 eventId = m_events.ExecuteEvent();
            switch (eventId)
            {
            case 1:
            {
                m_events.ScheduleEvent(1, 500);
                DoWalk();
                break;
            }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void DoWalk()
        {
            switch (m_phase)
            {
            case 1:
            {
                m_phase = 3;
                uint8 rol = urand(0, 2);
                jump = JumpW1[rol];
                me->GetMotionMaster()->MovePoint(11 + rol, jump);
                break;
            }
            case 2:
                m_phase = 3;
                jump = Position{ -1717.73f, 1486.27f, 57.23f, 5.45f };
                me->GetMotionMaster()->MovePoint(21, jump);
                break;
            case 11:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[0], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 12:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[1], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 13:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[2], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 21:
            {
                m_phase = 5;
                uint8 rol = urand(0, 3);
                jump = LandingW2[rol];
                me->GetMotionMaster()->MoveJump(jump, frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            }
            case 25:
                m_phase = 6;
                if (!m_playerGUID == 0)
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        if (player->IsInWorld() || player->IsAlive())
                        {
                            Position pos;
                            player->GetNearPosition(pos, frand(2.0f, 4.0f), frand(3.14f, 6.28f));
                            me->GetMotionMaster()->MovePoint(26, pos);
                        }
                break;
            case 26:
                m_phase = 7;
                me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_worgen_runt_35456AI(creature);
    }
};

// 35170
class npc_worgen_alpha_35170 : public CreatureScript
{
public:
    npc_worgen_alpha_35170() : CreatureScript("npc_worgen_alpha_35170") {}

    struct npc_worgen_alpha_35170AI : public ScriptedAI
    {
        npc_worgen_alpha_35170AI(Creature* creature) : ScriptedAI(creature) { Init(); }

        ObjectGuid m_playerGUID;
        EventMap m_events;
        uint32 m_phase;
        Position jump;
        Position JumpW1[3];
        Position LandingW1[3];
        Position LandingW2[4];

        void Init()
        {
            m_phase = 0;
            JumpW1[0] = Position{ -1643.91f, 1482.96f, 63.22f, 3.95f };
            JumpW1[1] = Position{ -1638.83f, 1478.07f, 65.36f, 3.84f };
            JumpW1[2] = Position{ -1631.49f, 1475.02f, 65.64f, 3.84f };

            LandingW1[0] = Position{ -1677.39f, 1455.52f, 52.29f, 4.06f };
            LandingW1[1] = Position{ -1671.89f, 1449.27f, 52.29f, 3.84f };
            LandingW1[2] = Position{ -1664.82f, 1443.62f, 52.29f, 3.84f };

            LandingW2[0] = Position{ -1704.93f, 1469.07f, 52.29f, 5.34f };
            LandingW2[1] = Position{ -1698.90f, 1472.92f, 52.29f, 5.41f };
            LandingW2[2] = Position{ -1703.37f, 1470.66f, 52.29f, 5.49f };
            LandingW2[3] = Position{ -1700.37f, 1473.32f, 52.29f, 5.41f };
        }

        void DoAction(int32 action) override
        {
            if (!m_playerGUID == 0)
            {
                m_events.ScheduleEvent(1, 500);
                m_phase = action;
                me->SetSpeed(MOVE_RUN, frand(1.2f, 1.6f));
            }
        }

        void SetGUID(uint64 guid, int32 /*type*/) override
        {
            m_playerGUID = guid;
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE || type == EFFECT_MOTION_TYPE)
                m_phase = pointId;
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            uint32 eventId = m_events.ExecuteEvent();
            switch (eventId)
            {
            case 1:
            {
                m_events.ScheduleEvent(1, 500);
                DoWalk();
                break;
            }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void DoWalk()
        {
            switch (m_phase)
            {
            case 1:
            {
                m_phase = 3;
                uint8 rol = urand(0, 2);
                jump = JumpW1[rol];
                me->GetMotionMaster()->MovePoint(11 + rol, jump);
                break;
            }
            case 2:
                m_phase = 3;
                jump = Position{ -1717.73f, 1486.27f, 57.23f, 5.45f };
                me->GetMotionMaster()->MovePoint(21, jump);
                break;
            case 11:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[0], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 12:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[1], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 13:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[2], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 21:
            {
                m_phase = 5;
                uint8 rol = urand(0, 3);
                jump = LandingW2[rol];
                me->GetMotionMaster()->MoveJump(jump, frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            }
            case 25:
                m_phase = 6;
                if (!m_playerGUID == 0)
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        if (player->IsInWorld() || player->IsAlive())
                        {
                            Position pos;
                            player->GetNearPosition(pos, frand(2.0f, 4.0f), frand(3.14f, 6.28f));
                            me->GetMotionMaster()->MovePoint(26, pos);
                        }
                break;
            case 26:
                m_phase = 7;
                me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_worgen_alpha_35170AI(creature);
    }
};

// 35167
class npc_worgen_alpha_35167 : public CreatureScript
{
public:
    npc_worgen_alpha_35167() : CreatureScript("npc_worgen_alpha_35167") {}

    struct npc_worgen_alpha_35167AI : public ScriptedAI
    {
        npc_worgen_alpha_35167AI(Creature* creature) : ScriptedAI(creature) { Init(); }

        ObjectGuid m_playerGUID;
        EventMap m_events;
        uint32 m_phase;
        Position jump;
        Position JumpW1[3];
        Position LandingW1[3];
        Position LandingW2[4];

        void Init()
        {
            m_phase = 0;
            JumpW1[0] = Position{ -1643.91f, 1482.96f, 63.22f, 3.95f };
            JumpW1[1] = Position{ -1638.83f, 1478.07f, 65.36f, 3.84f };
            JumpW1[2] = Position{ -1631.49f, 1475.02f, 65.64f, 3.84f };

            LandingW1[0] = Position{ -1677.39f, 1455.52f, 52.29f, 4.06f };
            LandingW1[1] = Position{ -1671.89f, 1449.27f, 52.29f, 3.84f };
            LandingW1[2] = Position{ -1664.82f, 1443.62f, 52.29f, 3.84f };

            LandingW2[0] = Position{ -1704.93f, 1469.07f, 52.29f, 5.34f };
            LandingW2[1] = Position{ -1698.90f, 1472.92f, 52.29f, 5.41f };
            LandingW2[2] = Position{ -1703.37f, 1470.66f, 52.29f, 5.49f };
            LandingW2[3] = Position{ -1700.37f, 1473.32f, 52.29f, 5.41f };
        }

        void DoAction(int32 action) override
        {
            if (!m_playerGUID == 0)
            {
                m_events.ScheduleEvent(1, 500);
                m_phase = action;
                me->SetSpeed(MOVE_RUN, frand(1.2f, 1.6f));
            }
        }

        void SetGUID(uint64 guid, int32 /*type = 0*/) override
        {
            m_playerGUID = guid;
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE || type == EFFECT_MOTION_TYPE)
                m_phase = pointId;
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            uint32 eventId = m_events.ExecuteEvent();
            switch (eventId)
            {
            case 1:
            {
                m_events.ScheduleEvent(1, 500);
                DoWalk();
                break;
            }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void DoWalk()
        {
            switch (m_phase)
            {
            case 1:
            {
                m_phase = 3;
                uint8 rol = urand(0, 2);
                jump = JumpW1[rol];
                me->GetMotionMaster()->MovePoint(11 + rol, jump);
                break;
            }
            case 2:
                m_phase = 3;
                jump = Position{ -1717.73f, 1486.27f, 57.23f, 5.45f };
                me->GetMotionMaster()->MovePoint(21, jump);
                break;
            case 11:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[0], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 12:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[1], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 13:
                m_phase = 4;
                me->GetMotionMaster()->MoveJump(LandingW1[2], frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            case 21:
            {
                m_phase = 5;
                uint8 rol = urand(0, 3);
                jump = LandingW2[rol];
                me->GetMotionMaster()->MoveJump(jump, frand(20.0f, 25.0f), frand(15.0f, 20.0f), 25);
                break;
            }
            case 25:
                m_phase = 6;
                if (!m_playerGUID == 0)
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        if (player->IsInWorld() || player->IsAlive())
                        {
                            Position pos;
                            player->GetNearPosition(pos, frand(2.0f, 4.0f), frand(3.14f, 6.28f));
                            me->GetMotionMaster()->MovePoint(26, pos);
                        }
                break;
            case 26:
                m_phase = 7;
                me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_worgen_alpha_35167AI(creature);
    }
};

// 35112
class npc_king_genn_greymane_35112 : public CreatureScript
{
public:
    npc_king_genn_greymane_35112() : CreatureScript("npc_king_genn_greymane_35112") { }

    struct npc_king_genn_greymane_35112AI : public ScriptedAI
    {
        npc_king_genn_greymane_35112AI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_king_genn_greymane_35112AI(creature);
    }
};

// 35115
class npc_lord_godfrey_35115 : public CreatureScript
{
public:
    npc_lord_godfrey_35115() : CreatureScript("npc_lord_godfrey_35115") { }

    struct npc_lord_godfrey_35115AI : public ScriptedAI
    {
        npc_lord_godfrey_35115AI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lord_godfrey_35115AI(creature);
    }
};

// 35118 showfight <> guard 34916
class npc_bloodfang_worgen_35118 : public CreatureScript
{
public:
    npc_bloodfang_worgen_35118() : CreatureScript("npc_bloodfang_worgen_35118") {}

    enum eNpc
    {
        EVENT_ENRAGE_COOLDOWN = 101,
    };

    struct npc_bloodfang_worgen_35118AI : public ScriptedAI
    {
        npc_bloodfang_worgen_35118AI(Creature* creature) : ScriptedAI(creature) {}

        EventMap m_events;
        bool m_enrage;

        void Reset() override
        {
            m_enrage = false;
        }

        void DamageTaken(Unit* /*who*/, uint32& /*damage*/) override
        {
            if (!m_enrage && me->GetHealthPct() < 50.0f)
            {
                me->CastSpell(me, SPELL_ENRAGE_8599);
                m_enrage = true;
                m_events.ScheduleEvent(EVENT_ENRAGE_COOLDOWN, 20000);
            }
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if (Player* player = caster->ToPlayer())
            {
                if (player->GetQuestStatus(14276) == QUEST_STATUS_INCOMPLETE)
                    if (spell->Id == 56641)
                        player->KilledMonsterCredit(44175);

                if (player->GetQuestStatus(14281) == QUEST_STATUS_INCOMPLETE)
                    if (spell->Id == 5143)
                        player->KilledMonsterCredit(44175);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_ENRAGE_COOLDOWN:
                {
                    m_enrage = false;
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_bloodfang_worgen_35118AI(creature);
    }
};

class spell_gilneas_prison_periodic_dummy : public SpellScriptLoader
{
public:
    spell_gilneas_prison_periodic_dummy() : SpellScriptLoader("spell_gilneas_prison_periodic_dummy") { }

private:
    class spell_script_impl : public SpellScript
    {
        PrepareSpellScript(spell_script_impl)

            void ExtraEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            Unit* caster = GetCaster();

            if (caster)
            {
                caster->RemoveAura(SPELL_GILNEAS_PRISON_PERIODIC_1);
                caster->RemoveAura(SPELL_GILNEAS_PRISON_PERIODIC_2);

                if (Player* player = caster->ToPlayer())
                    player->FailQuest(14154);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_script_impl::ExtraEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_script_impl();
    }
};
/* QUEST - 14154 - By The Skin of His Teeth - END */

struct npc_rampaging_worgen : public ScriptedAI
{
    npc_rampaging_worgen(Creature* creature) : ScriptedAI(creature) { }

    void DamageTaken(Unit* attacker, uint32& damage) override
    {
        if (attacker->GetTypeId() != TypeID::TYPEID_PLAYER && !HealthAbovePct(75))
            damage = 0;

        if (!HealthAbovePct(10) && !me->HasAura(8599))
        {
            Talk(0);
            DoCast(me, 8599);
        }
    }
};

void AddSC_gilneas()
{
    new npc_gilnean_crow();
    new gilneas_creature_script<npc_gilneas_city_guard_gate>("npc_gilneas_city_guard_gate");
    new gilneas_creature_script<npc_prince_liam_greymane>("npc_prince_liam_greymane");
    new gilneas_creature_script<npc_worgen_runt>("npc_worgen_runt");
    new gilneas_spell_script<spell_gen_gilneas_prison_periodic_dummy>("spell_gen_gilneas_prison_periodic_dummy");
    new gilneas_creature_script<npc_josiah_avery>("npc_josiah_avery");
    new gilneas_creature_script<npc_josiah_avery_worgen_form>("npc_josiah_avery_worgen_form");
    new gilneas_spell_script<spell_gilneas_pull_to>("spell_gilneas_pull_to");
    new npc_lorna_crowley_basement("npc_lorna_crowley_basement");
    new gilneas_creature_script<npc_gilnean_mastiff>("npc_gilnean_mastiff");
    new gilneas_creature_script<npc_bloodfang_lurker>("npc_bloodfang_lurker");
    new gilneas_spell_script<spell_attack_lurker>("spell_attack_lurker");
    new npc_king_genn_greymane();
    new npc_vehicle_genn_horse();
    new gilneas_creature_script<npc_saved_aranas>("npc_saved_aranas");
    new npc_gilneas_children("npc_james", SPELL_SAVE_JAMES, PLAYER_SAY_JAMES);
    new npc_gilneas_children("npc_ashley", SPELL_SAVE_ASHLEY, PLAYER_SAY_ASHLEY);
    new npc_gilneas_children("npc_cynthia", SPELL_SAVE_CYNTHIA, PLAYER_SAY_CYNTHIA);
    new npc_wahl("npc_wahl");
    new npc_lucius_the_cruel("npc_lucius_the_cruel");
    new gilneas_creature_script<npc_chance_the_cat>("npc_chance_the_cat");
    new npc_mountain_horse("npc_mountain_horse");
    new gilneas_creature_script<npc_mountain_horse_summoned>("npc_mountain_horse_summoned");
    new gilneas_spell_script<spell_gilneas_test_telescope>("spell_gilneas_test_telescope");
    new npc_stagecoach_carriage_exodus("npc_stagecoach_carriage_exodus");
    new npc_stagecoach_harness("npc_stagecoach_harness");
    new npc_king_genn_greymane_35112();
    new npc_bloodfang_worgen_35118();
    new npc_tobias_mistmantle_35124();
    new npc_lord_darius_crowley_35077();
    new npc_worgen_runt_35188();
    new npc_worgen_alpha_35170();
    new npc_worgen_runt_35456();
    new npc_worgen_alpha_35167();
    new npc_lord_godfrey_35115();
    new spell_gilneas_prison_periodic_dummy();
    new gilneas_creature_script<npc_rampaging_worgen>("npc_rampaging_worgen");
}
