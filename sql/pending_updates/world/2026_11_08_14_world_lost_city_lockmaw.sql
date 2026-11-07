-- Lost City of the Tol'vir: Lockmaw, his crocolisks and Augh (the intro, the three ability versions and, on heroic, the boss).
UPDATE `creature_template` SET `ScriptName` = 'boss_lockmaw' WHERE `entry` = 43614;
UPDATE `creature_template` SET `ScriptName` = 'npc_frenzied_croc' WHERE `entry` = 43658;
UPDATE `creature_template` SET `ScriptName` = 'npc_augh_dragons_breath' WHERE `entry` = 44895;
UPDATE `creature_template` SET `ScriptName` = 'npc_augh_intro' WHERE `entry` = 45377;
UPDATE `creature_template` SET `ScriptName` = 'npc_augh_whirlwind' WHERE `entry` = 45378;
UPDATE `creature_template` SET `ScriptName` = 'npc_augh_blow_dart' WHERE `entry` = 45379;
UPDATE `creature_template` SET `ScriptName` = 'boss_augh' WHERE `entry` = 49045;

DELETE FROM `spell_script_names` WHERE `spell_id` = 81642 AND `ScriptName` = 'spell_dust_flail';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (81642, 'spell_dust_flail');

DELETE FROM `creature` WHERE `guid` IN (9240000, 9240001);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9240000, 44895, 755, 6, 0, 0, 37339, 0, -10987.3, -1659.64, 2.35191, 1.76278, 7200, 0, 0, 995928, 0, 0, 0, 0, 0),
(9240001, 49045, 755, 4, 0, 0, 37339, 0, -11058.9, -1625.34, -0.130499, 4.7822, 86400, 0, 0, 2074850, 0, 0, 0, 0, 0);

DELETE FROM `creature_text` WHERE `entry` IN (43614, 45377, 49045);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43614, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Lockmaw - Frenzy'),
(45377, 0, 0, 'Bwaaaah! You no take da treasure!!!', 12, 0, 100, 0, 0, 0, 'Augh - Intro'),
(45377, 1, 0, 'Bwagauugh!!! Augh feed you to da crocs!!!', 12, 0, 100, 0, 0, 0, 'Augh - Intro'),
(49045, 0, 0, 'Augh appears from the distance!', 41, 0, 100, 0, 0, 0, 'Augh - Appears'),
(49045, 1, 0, 'GAAAH! How you kill croc?!', 12, 0, 100, 0, 0, 0, 'Augh - Lockmaw dead'),
(49045, 2, 0, 'Augh smart! Augh already steal treasure while you no looking!', 12, 0, 100, 0, 0, 0, 'Augh - Treasure'),
(49045, 3, 0, 'Augh da boss! Oh yeah!', 12, 0, 100, 0, 0, 0, 'Augh - Boss'),
(49045, 4, 0, 'Augh steal your treasure. Uhn uhn uhnnn!', 12, 0, 100, 0, 0, 0, 'Augh - Steal'),
(49045, 5, 0, 'Who bad?! Augh bad!! Ugn!', 12, 0, 100, 0, 0, 0, 'Augh - Bad'),
(49045, 6, 0, 'Gwaaaaaaahhh!!!', 14, 0, 100, 0, 0, 0, 'Augh - Yell');
