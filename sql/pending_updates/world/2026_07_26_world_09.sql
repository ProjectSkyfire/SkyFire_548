-- Player aura while on quest
DELETE FROM `spell_area` WHERE `spell`=69991;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_start_status`, `quest_end_status`) VALUES
(69991, 132, 24489, 8, 0);

-- Soothsayer Shi'kala
DELETE FROM `creature_text` WHERE `entry`=37108;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(37108, 0, 0, 'Da spirits be angry with us.', 12, 0, 100, 396, 0, 0, 'Soothsayer Shi\'kala SAY_0'),
(37108, 1, 0, 'I don\' know why the spirits be rejectin\' us so.', 12, 0, 100, 396, 0, 0, 'Soothsayer Shi\'kala SAY_1'),
(37108, 2, 0, 'Don\' worry, child.  Grik\'nir gonna help us get through this.', 12, 0, 0, 0, 0, 0, 'Soothsayer Shi\'kala SAY_2');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37108;
DELETE FROM `smart_scripts` WHERE `entryorguid`=37108;
DELETE FROM `smart_scripts` WHERE `entryorguid`=3710800;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37108, 0, 0, 0, 8, 0, 100, 0, 69995, 0, 30000, 30000, 0, 80, 3710800, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Shi\'kala - On Spellhit - Run Script'),
(3710800, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Shi\'kala - Script - Say 0'),
(3710800, 9, 1, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Shi\'kala - Script - Say 1'),
(3710800, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 11, 70007, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Shi\'kala - Script - Cast \'Soothsayer Kill Credit SE\''),
(3710800, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Shi\'kala - Script - Say 2');

-- Soothsayer Rikkari
DELETE FROM `creature_text` WHERE `entry`=37173;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(37173, 0, 0, 'What we gon\' do now, you ask?  We wait.', 12, 0, 100, 396, 0, 0, 'Soothsayer Rikkari - SAY_0'),
(37173, 1, 0, 'Grik\'nir says he gon\' talk to the elemental, get it to fight on our side.', 12, 0, 100, 396, 0, 0, 'Soothsayer Rikkari - SAY_1'),
(37173, 2, 0, 'Soon enough we take over dis valley.  Soon enough.', 12, 0, 100, 396, 0, 0, 'Soothsayer Rikkari - SAY_2');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37173;
DELETE FROM `smart_scripts` WHERE `entryorguid`=37173;
DELETE FROM `smart_scripts` WHERE `entryorguid`=3717300;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37173, 0, 0, 0, 8, 0, 100, 0, 69995, 0, 30000, 30000, 0, 80, 3717300, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Rikkari - On Spellhit - Run Script'),
(3717300, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Rikkari - Script - Say 0'),
(3717300, 9, 1, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Rikkari - Script - Say 1'),
(3717300, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 11, 70008, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Rikkari - Script - Cast \'Soothsayer Kill Credit SW\''),
(3717300, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Rikkari - Script - Say 2');

-- Soothsayer Mirim'koa
DELETE FROM `creature_text` WHERE `entry`=37174;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(37174, 0, 0, 'Our land be a land of ice an\' snow.', 12, 0, 100, 396, 0, 0, 'Soothsayer Mirim\'koa - SAY_0'),
(37174, 1, 0, 'But beneath the earth, child, there always be fire.', 12, 0, 100, 396, 0, 0, 'Soothsayer Mirim\'koa - SAY_1'),
(37174, 2, 0, 'De spirit come from deep down to talk with Grik\'nir.', 12, 0, 100, 396, 0, 0, 'Soothsayer Mirim\'koa - SAY_2');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37174;
DELETE FROM `smart_scripts` WHERE `entryorguid`=37174;
DELETE FROM `smart_scripts` WHERE `entryorguid`=3717400;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37174, 0, 0, 0, 8, 0, 100, 0, 69995, 0, 30000, 30000, 0, 80, 3717400, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Mirim\'koa - On Spellhit - Run Script'),
(3717400, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Mirim\'koa - Script - Say 0'),
(3717400, 9, 1, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Mirim\'koa - Script - Say 1'),
(3717400, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 11, 70009, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Mirim\'koa - Script - Cast \'Soothsayer Kill Credit W\''),
(3717400, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Soothsayer Mirim\'koa - Script - Say 2');
