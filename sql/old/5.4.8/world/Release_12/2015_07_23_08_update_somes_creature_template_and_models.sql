UPDATE `creature_template` SET `unit_class`=1 WHERE `entry`= 69333;
UPDATE `creature_template` SET `unit_class`=1 WHERE `entry`= 70301;
UPDATE `creature_template` SET `unit_class`=1 WHERE `entry`= 68869;
UPDATE `creature_template` SET `spell1`=0 WHERE  `entry`=61966;

UPDATE `creature_template` SET `faction_H`= 1734 WHERE  `entry`= 69425;
UPDATE `creature_template` SET `faction_H`= 2491 WHERE  `entry`= 70316;
UPDATE `creature_template` SET `faction_H`= 1602 WHERE  `entry`= 67990;
UPDATE `creature_template` SET `faction_H`= 2642 WHERE  `entry`= 70520;
UPDATE `creature_template` SET `faction_H`= 2642 WHERE  `entry`= 69371;
UPDATE `creature_template` SET `faction_H`= 2642 WHERE  `entry`= 67662;
UPDATE `creature_template` SET `faction_H`= 2642 WHERE  `entry`= 67984;
UPDATE `creature_template` SET `faction_H`= 2642 WHERE  `entry`= 67986;
UPDATE `creature_template` SET `faction_H`= 1602 WHERE  `entry`= 67989;
UPDATE `creature_template` SET `faction_H`= 1735 WHERE  `entry`= 69389;
UPDATE `creature_template` SET `faction_H`= 2642 WHERE  `entry`= 69418;
UPDATE `creature_template` SET `faction_H`= 84 WHERE  `entry`= 69572;
UPDATE `creature_template` SET `faction_H`= 2110 WHERE  `entry`= 69742;
UPDATE `creature_template` SET `faction_H`= 2491 WHERE  `entry`= 70160;
UPDATE `creature_template` SET `faction_H`= 2122 WHERE  `entry`= 67660;

INSERT IGNORE INTO `creature_model_info` (`modelid`,`bounding_radius`,`combat_reach`,`gender`,`modelid_other_gender`) VALUES 
(46789, 0, 0, 0, 0),
(47232, 0, 0, 0, 0),
(47361, 0, 0, 0, 0),
(47501, 0, 0, 0, 0),
(47508, 0, 0, 0, 0),
(47514, 0, 0, 0, 0),
(47607, 0, 0, 0, 0);
