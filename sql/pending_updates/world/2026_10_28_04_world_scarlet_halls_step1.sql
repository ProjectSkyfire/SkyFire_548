-- Scarlet Halls: instance and Braun. Entrance events and loot are deferred.
DELETE FROM `instance_template` WHERE `map` = 1001;
INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`) VALUES (1001, 870, 'instance_scarlet_halls', 1);

DELETE FROM `creature` WHERE `map` = 1001 AND `guid` BETWEEN 8300000 AND 8300017;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8300000, 59309, 1001, 262, 0, 0, 0, 0, 1003.86, 495.859, 13.5719, 1.41273, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300001, 59309, 1001, 262, 0, 0, 0, 0, 1005.37, 497.186, 13.5718, 1.4812, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300002, 59309, 1001, 262, 0, 0, 0, 0, 1006.87, 495.929, 13.5719, 1.43431, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300003, 59303, 1001, 262, 0, 0, 0, 0, 1009.56, 513.688, 12.9892, 2.36923, 86400, 0, 0, 1, 0, 0, 0, 0, 0),
(8300004, 59309, 1001, 262, 0, 0, 0, 0, 1026.08, 509.578, 13.5719, 3.20229, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300005, 59309, 1001, 262, 0, 0, 0, 0, 1027.34, 508.005, 13.5718, 3.0664, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300006, 59309, 1001, 262, 0, 0, 0, 0, 1027.41, 511.023, 13.5719, 3.03765, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300007, 59302, 1001, 262, 0, 0, 0, 0, 1036.03, 509.63, 13.573, 3.15905, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300008, 59299, 1001, 262, 0, 0, 0, 0, 1036.18, 510.788, 13.5719, 2.94766, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300009, 59299, 1001, 262, 0, 0, 0, 0, 1036.3, 508.398, 13.572, 3.46588, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300010, 59299, 1001, 262, 0, 0, 0, 0, 1036.6, 507.28, 13.5719, 3.54887, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300011, 59299, 1001, 262, 0, 0, 0, 0, 1036.63, 511.771, 13.5719, 2.8216, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300012, 59299, 1001, 262, 0, 0, 0, 0, 1037.75, 509.639, 13.5616, 3.15905, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300013, 59299, 1001, 262, 0, 0, 0, 0, 1038.26, 511.621, 13.5029, 2.8149, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300014, 59299, 1001, 262, 0, 0, 0, 0, 1038.26, 507.941, 13.4929, 3.47237, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300015, 59299, 1001, 262, 0, 0, 0, 0, 1039.59, 509.674, 12.9499, 3.15905, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300016, 59299, 1001, 262, 0, 0, 0, 0, 1039.96, 511.672, 12.796, 2.92072, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300017, 59299, 1001, 262, 0, 0, 0, 0, 1040.02, 507.785, 12.7723, 3.40571, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- Braun's reference template addon supplies the 114027 aura.
DELETE FROM `creature_addon` WHERE `guid` = 8300003;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (8300003, 0, 0, 0, 1, 0, '114027');

DELETE FROM `gameobject` WHERE `map` = 1001 AND `guid` = 910000;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910000, 210097, 1001, 262, 0, 0, 1040.28, 509.719, 13.4769, 1.5708, 0, 0, 0, 1, 7200, 255, 1);

UPDATE `creature_template` SET `ScriptName` = 'boss_houndmaster_braun' WHERE `entry` = 59303 AND `ScriptName` IN ('', 'boss_houndmaster_braun') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_obediend_hound' WHERE `entry` = 59309 AND `ScriptName` IN ('', 'npc_obediend_hound') AND `AIName` = '';

DELETE FROM `creature_text` WHERE `entry` = 59303;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(59303, 0, 0, 'Ah, fresh meat for the dogs.', 12, 0, 100, 0, 0, 27550, ''),
(59303, 1, 0, 'I bleed... haha... but I won''t die!', 12, 0, 100, 0, 0, 27544, ''),
(59303, 2, 0, 'What? No, no! Stop!  Obey your master!', 12, 0, 100, 0, 0, 27545, ''),
(59303, 3, 0, 'I let the dogs out!', 12, 0, 100, 0, 0, 27551, ''),
(59303, 4, 0, 'Stupid animals!  I''ll put you down after I kill these weaklings.', 12, 0, 100, 0, 0, 27543, '');

DELETE FROM `spell_script_names` WHERE (`spell_id` = 114020 AND `ScriptName` = 'spell_bloody_mess') OR (`spell_id` = 108481 AND `ScriptName` = 'spell_crushing_leap_visual');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (114020, 'spell_bloody_mess'), (108481, 'spell_crushing_leap_visual');
