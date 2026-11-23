-- Redridge Mountains: the quest givers of the Cataclysm chain that had no spawn. John J. Keeshan (26567 in the Lakeshire
-- Inn basement, 26693 and Detonation at the Tower of Ilgalar), Foreman Oslow (26569), Messner, Krakauer and Danforth at
-- Camp Everstill (26636 to 26638, 26692), Brubaker (26639), Colonel Troteman (26708), Darkblaze and the Blackrock
-- Invaders were not spawned, and their templates were friendly placeholders. Based on Legends of Azeroth 5.4.8; the
-- positions match the spawn maps on Wowhead MoP Classic. Messner, Jorgensen, Krakauer and Danforth of the rescue chain
-- (26560 to 26562) are summoned by script in 5.4.8 and are not spawned here.

DELETE FROM `creature` WHERE `guid` BETWEEN 9310100 AND 9310124;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9310100, 43184, 0, 1, 0, 0, 0, 0, -9201.86, -2154.75, 57.1866, 4.5931, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310101, 43194, 0, 1, 0, 0, 0, 0, -9277.43, -2240.89, 64.1159, 5.0614, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310102, 43458, 0, 1, 0, 0, 0, 0, -9472.03, -2833.19, 65.279, 1.0339, 600, 0, 0, 0, 0, 0, 0, 0, 0),
(9310103, 43459, 0, 1, 0, 0, 0, 0, -9476.13, -2845.2, 65.8312, 6.1268, 600, 0, 0, 0, 0, 0, 0, 0, 0),
(9310104, 43461, 0, 1, 0, 0, 0, 0, -9472.54, -2835.76, 65.2782, 0.5258, 600, 0, 0, 0, 0, 0, 0, 0, 0),
(9310105, 43462, 0, 1, 0, 0, 0, 0, -9472.85, -2841.93, 65.3127, 0.6696, 600, 0, 0, 0, 0, 0, 0, 0, 0),
(9310106, 43496, 0, 1, 0, 0, 0, 0, -9475.67, -2996.85, 133.689, 2.9446, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310107, 43508, 0, 1, 0, 0, 0, 0, -9676.09, -2842, 51.9081, 0.071, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310108, 43607, 0, 1, 0, 0, 0, 0, -9639.26, -3474.67, 121.956, 5.9143, 600, 0, 0, 0, 0, 0, 0, 0, 0),
(9310109, 43611, 0, 1, 0, 0, 0, 0, -9638.71, -3461.92, 119.412, 4.9858, 600, 0, 0, 0, 0, 0, 0, 0, 0),
(9310110, 43733, 0, 1, 0, 0, 0, 0, -9649.82, -3464.61, 120.077, 4.4304, 600, 0, 0, 0, 0, 0, 0, 0, 0),
(9310111, 43775, 0, 1, 0, 0, 0, 0, -9218.06, -3202.91, 105.464, 0.6161, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310112, 43775, 0, 1, 0, 0, 0, 0, -9275.61, -3209.39, 103.793, 5.9685, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310113, 43775, 0, 1, 0, 0, 0, 0, -9321.78, -3226.87, 99.6671, 0.2508, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310114, 43775, 0, 1, 0, 0, 0, 0, -9368.14, -3266.82, 88.3341, 0.997, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310115, 43775, 0, 1, 0, 0, 0, 0, -9176.83, -3335.72, 104.413, 3.145, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310116, 43775, 0, 1, 0, 0, 0, 0, -9112.03, -3334.22, 100.343, 1.1383, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310117, 43775, 0, 1, 0, 0, 0, 0, -9127.01, -3329.94, 100.979, 1.4643, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310118, 43775, 0, 1, 0, 0, 0, 0, -9104.8, -3292.58, 102.875, 4.221, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310119, 43775, 0, 1, 0, 0, 0, 0, -9353.91, -3322.1, 92.2777, 1.8177, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310120, 43775, 0, 1, 0, 0, 0, 0, -9351.4, -3343.01, 90.66, 3.3414, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310121, 43775, 0, 1, 0, 0, 0, 0, -9315.97, -3335.83, 103.075, 3.3414, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310122, 43775, 0, 1, 0, 0, 0, 0, -9301.92, -3356.51, 107.112, 3.9815, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310123, 43775, 0, 1, 0, 0, 0, 0, -9249.97, -3389.91, 103.763, 4.5666, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310124, 43775, 0, 1, 0, 0, 0, 0, -9198.5, -3301.6, 99.5434, 2.5756, 60, 0, 0, 0, 0, 0, 0, 0, 0);

UPDATE `creature_template` SET `faction_A`=1934, `faction_H`=1934 WHERE `entry`=43184;
UPDATE `creature_template` SET `faction_A`=1802, `faction_H`=1802 WHERE `entry` IN (43458, 43459, 43461, 43462, 43508, 43607, 43611, 43733);
UPDATE `creature_template` SET `faction_A`=1801, `faction_H`=1801 WHERE `entry` IN (43496, 43775);
UPDATE `creature_template` SET `minlevel`=25, `maxlevel`=25 WHERE `entry`=43611;
UPDATE `creature_template` SET `minlevel`=17, `maxlevel`=17 WHERE `entry`=43508;

-- Detonation: Keeshan gives the quest credit when it is accepted.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=43611;
DELETE FROM `smart_scripts` WHERE `entryorguid`=43611 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43611, 0, 0, 0, 19, 0, 100, 0, 26668, 0, 0, 0, 0, 15, 26668, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'John J. Keeshan - On Quest Accepted - Area Explored Or Event Happens');

-- Surveying Equipment: the Blackrock Spyglass.
DELETE FROM `creature_loot_template` WHERE `entry`=615 AND `item`=58952;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(615, 58952, -56.1333, 1, 0, 1, 1);
