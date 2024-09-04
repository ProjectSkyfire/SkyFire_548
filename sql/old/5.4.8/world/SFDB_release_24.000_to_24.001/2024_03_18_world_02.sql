UPDATE `creature_equip_template` SET `itemEntry2`=13606 WHERE `entry`=66290 AND `id`=1;

DELETE FROM `creature_equip_template` WHERE `id`=2 AND `entry` = 66480;
INSERT INTO `creature_equip_template` (`entry`, `id`, `itemEntry1`) VALUES (66480, 2, 13607);

DELETE FROM `creature_equip_template` WHERE `entry` IN (66290, 66398, 66399, 66282);
INSERT INTO `creature_equip_template` (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES
(66290, 3, 6219, 13606, 0),
(66290, 2, 45123, 0, 0),
(66290, 1, 6219, 0, 0),
(66398, 1, 0, 0, 15323),
(66399, 1, 1415, 0, 0),
(66282, 2, 5289, 0, 0),
(66282, 1, 10612, 12452, 0);