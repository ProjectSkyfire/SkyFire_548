-- Adds and summons of the Cataclysm dungeons were left with the friendly level 1 placeholder faction, so they
-- spawned friendly and, with no melee damage, hit for nothing. They get the faction and unit flags of Legends of
-- Azeroth 5.4.8 and melee damage by the convention of the other dungeon creatures (the levels are set
-- separately). Each statement checks the old faction.
-- Based on Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `unit_flags` = 0 WHERE `entry` IN (
    43658, 54191, 56036
) AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `unit_flags` = 320 WHERE `entry` = 55400 AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 0 WHERE `entry` IN (
    40630, 43934
) AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 16 WHERE `entry` = 54646 AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 64 WHERE `entry` = 45269 AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 32768 WHERE `entry` = 54952 AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33554432 WHERE `entry` IN (
    40622, 44713
) AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33587968 WHERE `entry` = 54641 AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33816640 WHERE `entry` = 40620 AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 90, `faction_H` = 90, `unit_flags` = 0 WHERE `entry` IN (
    56001, 56002, 57410
) AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 91, `faction_H` = 91, `unit_flags` = 64 WHERE `entry` IN (
    39369, 39370
) AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 1965, `faction_H` = 1965, `unit_flags` = 0 WHERE `entry` = 58234 AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 2102, `faction_H` = 2102, `unit_flags` = 0 WHERE `entry` = 54600 AND `faction_A` = 35 AND `faction_H` = 35;
UPDATE `creature_template` SET `faction_A` = 2324, `faction_H` = 2324, `unit_flags` = 0 WHERE `entry` = 54696 AND `faction_A` = 35 AND `faction_H` = 35;

UPDATE `creature_template` SET `mindmg` = 2568, `maxdmg` = 4366, `attackpower` = 1004, `dmg_multiplier` = 1 WHERE `entry` IN (
    40622, 40630, 43658, 43934, 54952, 56001
) AND `maxdmg` = 0;
UPDATE `creature_template` SET `mindmg` = 2568, `maxdmg` = 4366, `attackpower` = 1004, `dmg_multiplier` = 2.25 WHERE `entry` IN (
    39369, 39370, 40620, 45269, 54191, 54600, 56002, 57410, 58234
) AND `maxdmg` = 0;
UPDATE `creature_template` SET `mindmg` = 3278, `maxdmg` = 5573, `attackpower` = 0, `dmg_multiplier` = 2.25 WHERE `entry` = 56036 AND `maxdmg` = 0;
UPDATE `creature_template` SET `mindmg` = 4184, `maxdmg` = 7114, `attackpower` = 0, `dmg_multiplier` = 1 WHERE `entry` = 54646 AND `maxdmg` = 0;
UPDATE `creature_template` SET `mindmg` = 4184, `maxdmg` = 7114, `attackpower` = 0, `dmg_multiplier` = 2.25 WHERE `entry` IN (
    44713, 54641, 54696
) AND `maxdmg` = 0;
UPDATE `creature_template` SET `mindmg` = 5340, `maxdmg` = 9080, `attackpower` = 0, `dmg_multiplier` = 2.25 WHERE `entry` = 55400 AND `maxdmg` = 0;
