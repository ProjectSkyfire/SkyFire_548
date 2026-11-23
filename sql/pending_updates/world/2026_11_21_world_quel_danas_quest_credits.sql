-- Isle of Quel'Danas: "Know Your Ley Lines" (11547). The three Sunwell quest bunnies (Shrine, Portal, Sunwell) had no
-- script, so sampling the ley line field (spell 45191) gave no credit. The Dawnblade Reservists of "Intercept the
-- Reinforcements" (11542) and "Keeping the Enemy at Bay" (11543) fight from range and cast Raptor Strike in melee.
-- Based on Legends of Azeroth 5.4.8.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (25087, 25154, 25156, 25157);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (25087, 25154, 25156, 25157);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(25087, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 79, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dawnblade Reservist - On Respawn - Set Ranged Movement'),
(25087, 0, 1, 0, 9, 0, 100, 0, 1, 25, 1000, 4000, 0, 11, 32915, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dawnblade Reservist - Within Range - Cast Raptor Strike'),
(25154, 0, 0, 0, 8, 0, 100, 0, 45191, 0, 0, 0, 0, 33, 25154, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sunwell Quest Bunny Shrine - On Spell Hit (Sample Ley Line Field) - Give Kill Credit'),
(25156, 0, 0, 0, 8, 0, 100, 0, 45191, 0, 0, 0, 0, 33, 25156, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sunwell Quest Bunny Portal - On Spell Hit (Sample Ley Line Field) - Give Kill Credit'),
(25157, 0, 0, 0, 8, 0, 100, 0, 45191, 0, 0, 0, 0, 33, 25157, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sunwell Quest Bunny Sunwell - On Spell Hit (Sample Ley Line Field) - Give Kill Credit');
