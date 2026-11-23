-- The Risen Riflemen that Cannon Master Willey summons stood in melee and never shot: the template has Shoot (17353)
-- in its spell list but neither an AI name nor any SmartAI rows. TrinityCore 3.3.5 and the Cataclysm Preservation
-- Project 4.3.4 give the entry one row that casts Shoot at the target every 2.3 to 3.9 seconds, as the dungeon
-- journal describes ("shooting at random players"). Stratholme has no heroic mode, so the row takes no difficulty
-- flag.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 11054 AND `AIName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` = 11054 AND `source_type` = 0 AND `id` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11054, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 17353, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Risen Rifleman - In Combat - Cast Shoot');
