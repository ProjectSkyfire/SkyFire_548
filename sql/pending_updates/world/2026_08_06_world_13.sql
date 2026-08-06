-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144763;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144763, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144764;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144764, 1, 234);

-- USE HAMMER
UPDATE `creature` SET `equipment_id`=2 WHERE `guid`=144765;

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144765;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144765, 1, 234);

-- USE HAMMER
UPDATE `creature` SET `equipment_id`=2 WHERE `guid`=144766;

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144766;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144766, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144782;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144782, 1, 234);

-- USE HAMMER
UPDATE `creature` SET `equipment_id`=2 WHERE `guid`=144783;

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144783;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144783, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144787;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144787, 1, 234);

-- USE HAMMER
UPDATE `creature` SET `equipment_id`=2 WHERE `guid`=144789;

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144789;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144789, 1, 234);

-- USE HAMMER
UPDATE `creature` SET `equipment_id`=2 WHERE `guid`=144792;

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144792;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144792, 1, 234);

-- EMOTE_STATE_USE_STANDING
DELETE FROM `creature_addon` WHERE `guid`=144793;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144793, 1, 69);

-- EMOTE_STATE_USE_STANDING
DELETE FROM `creature_addon` WHERE `guid`=144794;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144794, 1, 69);

-- USE HAMMER
UPDATE `creature` SET `equipment_id`=2 WHERE `guid`=144795;

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144795;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144795, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144823;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144823, 1, 234);

-- USE HAMMER
UPDATE `creature` SET `equipment_id`=2 WHERE `guid`=144829;

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144829;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144829, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144832;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144832, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144836;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144836, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144839;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144839, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144840;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144840, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144841;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144841, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144846;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144846, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144862;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144862, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144864;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144864, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144873;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144873, 1, 234);

-- EMOTE_STATE_WORK_CHOPWOOD
DELETE FROM `creature_addon` WHERE `guid`=144874;
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES
(144874, 1, 234);

-- Frostmane Builder
UPDATE `creature` SET `spawndist`=0 WHERE `id`=41251;
UPDATE `creature` SET `MovementType`=0 WHERE `id`=41251;

DELETE FROM `creature_text` WHERE `entry`=41251;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(41251, 0, 0, 'No make me small!', 12, 0, 100, 0, 0, 0, 'Frostmane Builder SAY_0_0'),
(41251, 0, 1, 'Why everyone else get so big?', 12, 0, 100, 0, 0, 0, 'Frostmane Builder - SAY_0_1'),
(41251, 0, 2, 'Take curse away!', 12, 0, 100, 0, 0, 0, 'Frostmane Builder - SAY_0_2'),
(41251, 0, 3, 'Can\'t... lift... tools...', 12, 0, 100, 0, 0, 0, 'Frostmane Builder - SAY_0_3');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=41251;

DELETE FROM `smart_scripts` WHERE `entryorguid`=41251;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41251, 0, 0, 0, 8, 0, 100, 0, 93773, 0, 0, 0, 0, 80, 4125100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Builder - On Spellhit - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4125100;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4125100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Builder - On Script - Emote'),
(4125100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Builder - On Script - Set Orientation'),
(4125100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Builder - On Script - Talk 0'),
(4125100, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 33, 50606, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Builder - On Script - Give Credit'),
(4125100, 9, 4, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Builder - On Script - Despawn');

-- The View from Down Here
UPDATE `quest_template` SET `OfferRewardText`='Well done, $n. That\'ll teach them not to mess with us in our home!' WHERE `Id`=28868;
