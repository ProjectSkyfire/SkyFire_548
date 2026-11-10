-- King of the Foulweald (6621): Senani's Banner needs spell focus 1063 (the Foulweald Totem Mound) within 7 yards, and no
-- object with it was spawned, so the banner could not be used. Using it sends event 6721, which summons ten Enraged
-- Foulweald that walk to the banner, then Chief Murgut. The event summoned other creatures, the Enraged Foulweald had no
-- walk, fifteen of them stood piled on the mound all the time, and Murgut's death script never ran in the open world.

-- Spell focus object at the mound.
DELETE FROM `gameobject_template` WHERE `entry` = 178206;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(178206, 8, 327, 'Foulweald Totem Mound', 1, 1063, 7, 1);

DELETE FROM `gameobject` WHERE `guid` = 1062010;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1062010, 178206, 1, 1, 0, 0, 2231.89, -1550.41, 93.1996, 0.645454, 0, 0, 0.317154, 0.948374, 300, 100, 1);

-- The banner has no script of its own, the event does the summoning.
UPDATE `gameobject_template` SET `AIName` = '' WHERE `entry` = 178205;
DELETE FROM `smart_scripts` WHERE (`source_type` = 1 AND `entryorguid` = 178205) OR (`source_type` = 9 AND `entryorguid` = 17820500);

-- The permanent Enraged Foulweald on the mound.
DELETE FROM `creature` WHERE `id` = 12921 AND `guid` BETWEEN 176840 AND 176854;

-- Event 6721: ten Enraged Foulweald six seconds apart, then Chief Murgut a minute in.
DELETE FROM `event_scripts` WHERE `id` = 6721;
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(6721, 0, 10, 12921, 120000, 0, 2235.62, -1513.57, 90.4863, 4.65774),
(6721, 6, 10, 12921, 120000, 0, 2235.66, -1587.98, 86.2744, 1.68378),
(6721, 12, 10, 12921, 120000, 0, 2276.14, -1536.84, 90.5515, 3.50258),
(6721, 18, 10, 12921, 120000, 0, 2195.59, -1550.55, 86.5951, 6.276),
(6721, 24, 10, 12921, 120000, 0, 2235.62, -1513.57, 90.4863, 4.65774),
(6721, 30, 10, 12921, 120000, 0, 2235.66, -1587.98, 86.2744, 1.68378),
(6721, 36, 10, 12921, 120000, 0, 2276.14, -1536.84, 90.5515, 3.50258),
(6721, 42, 10, 12921, 120000, 0, 2195.59, -1550.55, 86.5951, 6.276),
(6721, 48, 10, 12921, 120000, 0, 2235.62, -1513.57, 90.4863, 4.65774),
(6721, 54, 10, 12921, 120000, 0, 2235.66, -1587.98, 86.2744, 1.68378),
(6721, 60, 10, 12918, 120000, 0, 2276.14, -1536.84, 90.5515, 3.50258);

-- Enraged Foulweald walk to the banner and attack it; Chief Murgut speaks and drops the basket with the totem.
DELETE FROM `waypoints` WHERE `entry` = 12921;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(12921, 1, 2231.89, -1550.41, 93.1996, 'Enraged Foulweald');

DELETE FROM `creature_text` WHERE `entry` IN (12918, 12921);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(12918, 0, 0, 'You are a little gnat to the Foulweald!  Die!', 12, 0, 100, 0, 0, 0, 'on Aggro Text'),
(12918, 1, 0, 'No!  You cannot be stronger than the Foulweald!  No!!', 12, 0, 100, 0, 0, 0, 'on Death Text'),
(12921, 0, 0, '%s attacks the banner!', 16, 0, 100, 0, 0, 0, 'Enraged Foulweald');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (12918, 12921);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (12918, 12921);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(12918, 0, 0, 1, 54, 0, 100, 1, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 100, 0, 0, 0, 0, 0, 0, 'Chief Murgut - On Just Summoned - Attack Closest Player'),
(12918, 0, 1, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Murgut - Linked To Id 0 - Say Line 0'),
(12918, 0, 2, 3, 6, 0, 100, 0, 0, 0, 0, 0, 0, 11, 20818, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Murgut - On Just Died - Cast Summon Foulweald Totem Basket'),
(12918, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Murgut - Linked To Id 2 - Say Line 1'),
(12921, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 11, 6821, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enraged Foulweald - On Just Summoned - Cast Corrupted Strength Passive'),
(12921, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 12921, 0, 6621, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 'Enraged Foulweald - Linked To Id 0 - Waypoint Start'),
(12921, 0, 2, 3, 58, 0, 100, 0, 1, 12921, 0, 0, 0, 11, 20786, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enraged Foulweald - On Waypoint Ended - Cast Destroy Banner'),
(12921, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enraged Foulweald - Linked To Id 2 - Say Line 0');
