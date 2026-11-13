-- Four creature templates of the Lost Isles, Deepholm, the Badlands and the Twilight Highlands carried the flags of a
-- corpse (stunned, immune to players and creatures, feign death, dead), so every spawn lay dead and could not be
-- fought or used. They get their living flags from the 5.4.8 data. Safe to run twice.
UPDATE `creature_template` SET `unit_flags` = 32768, `unit_flags2` = 2048, `dynamicflags` = 0 WHERE `entry` = 36621 AND `unit_flags` = 570458880; -- Alliance Wizard
UPDATE `creature_template` SET `unit_flags` = 512, `unit_flags2` = 2048, `dynamicflags` = 0 WHERE `entry` = 43115 AND `unit_flags` = 537133824; -- Dormant Stonebound Elemental
UPDATE `creature_template` SET `unit_flags` = 0, `unit_flags2` = 2048, `dynamicflags` = 0 WHERE `entry` = 46713 AND `unit_flags` = 570687488; -- Stonevault Wanderer
UPDATE `creature_template` SET `unit_flags` = 33280, `unit_flags2` = 2048, `dynamicflags` = 0 WHERE `entry` = 49548 AND `unit_flags` = 570721024; -- Krazzworks Laborer

-- The Dormant Stonebound Elementals keep lying on the ground until they are reformed.
DELETE FROM `creature_template_addon` WHERE `entry` = 43115;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(43115, 0, 0, 7, 1, 0, '');

-- One Alliance Wizard and nine Stonevault Wanderers lie dead in the 5.4.8 data (the same spots); they keep that look
-- through the Permanent Feign Death aura. The dead Krazzworks Laborers already have it from their template addon.
DELETE FROM `creature_addon` WHERE `guid` IN (8184879, 190331, 190332, 190360, 190361, 190362, 190363, 190364, 190365, 190483);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(8184879, 0, 0, 0, 0, 0, '29266'),
(190331, 0, 0, 0, 0, 0, '29266'),
(190332, 0, 0, 0, 0, 0, '29266'),
(190360, 0, 0, 0, 0, 0, '29266'),
(190361, 0, 0, 0, 0, 0, '29266'),
(190362, 0, 0, 0, 0, 0, '29266'),
(190363, 0, 0, 0, 0, 0, '29266'),
(190364, 0, 0, 0, 0, 0, '29266'),
(190365, 0, 0, 0, 0, 0, '29266'),
(190483, 0, 0, 0, 0, 0, '29266');
