-- Quest 28729 Teldrassil: Crown of Azeroth
-- Shadowglen Moonwell Bunny (guid 138449) forcecasts 65657 while the quest is taken.
-- That triggers summon 65656 (Shade of the Kaldorei 34574) and its RP path/dialogue.
-- Summon 65656 uses TARGET_DEST_DB, so spell_target_position is required.

UPDATE `creature_template` SET `AIName`='SmartAI', `unit_flags`=33544 WHERE `entry`=34574;

DELETE FROM `spell_target_position` WHERE `id`=65656;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(65656, 0, 1, 10711.6, 758.853, 1321.633, 2.44346);

DELETE FROM `creature_text` WHERE `entry`=34574;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34574, 0, 0, '%s fades into existence as you approach, nodding a subtle greeting.', 16, 0, 100, 0, 0, 0, 'Shade of the Kaldorei'),
(34574, 1, 0, 'Much has changed for our people since the Battle of Mount Hyjal.', 12, 0, 100, 1, 0, 0, 'Shade of the Kaldorei'),
(34574, 2, 0, 'Nordrassil lies a pale shadow of what it once was, its power used to defeat Archimonde and drive back the Burning Legion.', 12, 0, 100, 1, 0, 0, 'Shade of the Kaldorei'),
(34574, 3, 0, 'Our immortality-the very essence of our beings!-was lost.', 12, 0, 100, 1, 0, 0, 'Shade of the Kaldorei'),
(34574, 4, 0, 'The Betrayer was freed from his prison, and Shan''do Stormrage disappeared into the emerald dream.', 12, 0, 100, 1, 0, 0, 'Shade of the Kaldorei'),
(34574, 5, 0, 'A dark time for all.', 12, 0, 100, 1, 0, 0, 'Shade of the Kaldorei'),
(34574, 6, 0, 'The Shade of the Kaldorei closes its eyes and fades away.', 16, 0, 100, 0, 0, 0, 'Shade of the Kaldorei');

DELETE FROM `waypoints` WHERE `entry`=34574;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(34574, 1, 10702.9, 761.374, 1322.91, 'Shade of the Kaldorei'),
(34574, 2, 10704.4, 769.148, 1322.6, 'Shade of the Kaldorei'),
(34574, 3, 10706.4, 768.36, 1322.77, 'Shade of the Kaldorei'),
(34574, 4, 10714.0, 761.775, 1321.59, 'Shade of the Kaldorei');

-- Moonwell Bunny at Shadowglen moonwell (guid 138449)
-- Trigger NPCs default to REACT_PASSIVE; SkyFire only runs MoveInLineOfSight for REACT_AGGRESSIVE.
DELETE FROM `smart_scripts` WHERE `entryorguid`=-138449 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-138449, 0, 0, 1, 25, 0, 100, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonwell Bunny - On Reset - Set React Aggressive'),
(-138449, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonwell Bunny - On Reset - Set Invisible'),
(-138449, 0, 2, 0, 10, 0, 100, 0, 1, 10, 1000, 1000, 11, 65657, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Moonwell Bunny - On OOC Los - Cast Forcecast Summon Shade of the Kaldorei');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`=-138449 AND `SourceId`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=17 AND `SourceEntry`=65657;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 3, -138449, 0, 0, 9, 0, 28729, 0, 0, 0, 0, 0, '', 'Moonwell Bunny OOC Los requires quest 28729 taken'),
(22, 3, -138449, 0, 0, 29, 1, 34574, 30, 0, 1, 0, 0, '', 'Moonwell Bunny OOC Los requires Shade 34574 not already nearby'),
(17, 0, 65657, 0, 0, 9, 1, 28729, 0, 0, 0, 0, 0, '', 'Forcecast Summon Shade requires quest 28729 on target'),
(17, 0, 65657, 0, 0, 29, 1, 34574, 30, 0, 1, 0, 0, '', 'Forcecast Summon Shade requires Shade 34574 not already nearby');

-- Shade of the Kaldorei (34574)
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (34574, 3457400, 3457401, 3457402) AND `source_type` IN (0, 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34574, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 80, 3457400, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - On Just Summoned - Run Script'),
(34574, 0, 1, 0, 40, 0, 100, 0, 1, 34574, 0, 0, 54, 4000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - On Waypoint 1 - Pause Path'),
(34574, 0, 2, 0, 40, 0, 100, 0, 1, 34574, 0, 0, 1, 2, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - On Waypoint 1 - Say Line 2'),
(34574, 0, 3, 0, 40, 0, 100, 0, 3, 34574, 0, 0, 80, 3457401, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - On Waypoint 3 - Run Script'),
(34574, 0, 4, 0, 40, 0, 100, 0, 4, 34574, 0, 0, 80, 3457402, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - On Waypoint 4 - Run Script'),
(3457400, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - Script - Say Line 0'),
(3457400, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - Script - Say Line 1'),
(3457400, 9, 2, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 53, 0, 34574, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - Script - Start Waypoint'),
(3457401, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 54, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - Script - Pause Path'),
(3457401, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - Script - Say Line 3'),
(3457401, 9, 2, 0, 0, 0, 100, 0, 7000, 7000, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - Script - Say Line 4'),
(3457402, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 10714, 761.775, 1321.59, 2.957, 'Shade of the Kaldorei - Script - Set Orientation'),
(3457402, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - Script - Say Line 5'),
(3457402, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - Script - Say Line 6'),
(3457402, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of the Kaldorei - Script - Despawn');
