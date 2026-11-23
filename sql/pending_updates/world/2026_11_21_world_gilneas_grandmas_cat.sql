-- Gilneas: Grandma's Cat (14401). Lucius the Cruel stood next to Chance as a harmless bystander (immune to players), so
-- Chance the Cat, his quest item, could not be looted. Clicking Chance (spell 68743) now makes Lucius take the cat
-- ("I'll be taking this cat..."), walk to it and kneel; four seconds later he turns on the nearest player, shooting from range
-- (spell 41440). Chance leaves with him. If the fight is evaded Lucius becomes a bystander again, and he and the cat respawn after 30 seconds. Based on Legends of Azeroth 5.4.8, whose
-- scripts do the same in C++; the Grandma Wahl yell and transformation of that scene are not included.
UPDATE `creature` SET `spawntimesecs`=30 WHERE `map`=654 AND `id` IN (36459, 36461);
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (36459, 36461);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (36459, 36461);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36459, 0, 0, 1, 8, 0, 100, 0, 68743, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chance - On Spell Hit (Catch Cat) - Remove Spell Click Flag'),
(36459, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 36461, 30, 0, 0, 0, 0, 0, 'Chance - Linked - Set Data On Lucius the Cruel'),
(36459, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 6000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chance - Linked - Despawn'),
(36461, 0, 0, 1, 38, 0, 100, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lucius the Cruel - On Data Set - Say Line 0'),
(36461, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 4, 0, 0, 0, 0, 0, 8, 0, 0, 0, -2106.372, 2331.106, 7.360674, 0, 'Lucius the Cruel - Linked - Move To Chance'),
(36461, 0, 2, 3, 34, 0, 100, 0, 8, 4, 0, 0, 0, 5, 68, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lucius the Cruel - On Point Reached - Kneel'),
(36461, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 1, 4000, 4000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lucius the Cruel - Linked - Start Timed Event 1'),
(36461, 0, 4, 5, 59, 0, 100, 0, 1, 0, 0, 0, 0, 19, 33024, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lucius the Cruel - On Timed Event 1 - Become Attackable'),
(36461, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lucius the Cruel - Linked - Set Aggressive'),
(36461, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 17, 0, 40, 0, 0, 0, 0, 0, 'Lucius the Cruel - Linked - Attack Nearest Player'),
(36461, 0, 7, 0, 9, 0, 100, 0, 5, 35, 1500, 2500, 0, 11, 41440, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lucius the Cruel - In Range - Cast Shoot'),
(36461, 0, 8, 9, 7, 0, 100, 0, 0, 0, 0, 0, 0, 18, 33024, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lucius the Cruel - On Evade - Become Immune Again'),
(36461, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lucius the Cruel - Linked - Set Passive');

DELETE FROM `creature_text` WHERE `entry`=36461;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(36461, 0, 0, 'I''ll be taking this cat.  It seems to work as the perfect bait.  Prepare to die now, fool!', 12, 0, 100, 0, 0, 0, 'Lucius the Cruel - takes the cat');
