-- Postponing the Inevitable (27420) and Amidst Death, Life (27421) cannot be finished. The Overcharged Mote (spell
-- 84973) and Rayne's Seeds (spell 84964) could be used on any target, and neither the Plague Disseminators nor the
-- three ziggurat flower controllers reacted to them, so no credit was given. The spells are now limited to their
-- targets, a hit Disseminator takes the player's Overcharge Effect (which gives the credit) and dies, and a hit
-- controller grows the flowers and gives its credit. The Disseminators lose the immunity to players, which the source
-- does not have and which keeps the quest spell from landing.
-- Rows and conditions are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (45475, 45486, 45487, 45488) AND `ScriptName` = '';
UPDATE `creature_template` SET `unit_flags` = 512 WHERE `entry` = 45475 AND `unit_flags` = 768;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (45475, 45486, 45487, 45488) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45475, 0, 0, 0, 8, 0, 100, 0, 84973, 0, 5000, 5000, 0, 86, 84972, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Disseminator - On Spellhit Overcharge - Cross Cast Overcharge Effect'),
(45475, 0, 1, 0, 8, 0, 100, 0, 84972, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Disseminator - On Spellhit Overcharge Effect - Die'),
(45486, 0, 0, 1, 8, 0, 100, 0, 84964, 0, 10000, 10000, 0, 11, 84961, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Necropolis Flower Controller - On Spellhit Rayne''s Seed - Cast Flower Aura'),
(45486, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 45486, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Necropolis Flower Controller - Linked To Id 0 - Quest Credit'),
(45487, 0, 0, 1, 8, 0, 100, 0, 84964, 0, 10000, 10000, 0, 11, 84961, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Necropolis Flower Controller - On Spellhit Rayne''s Seed - Cast Flower Aura'),
(45487, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 45487, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Necropolis Flower Controller - Linked To Id 0 - Quest Credit'),
(45488, 0, 0, 1, 8, 0, 100, 0, 84964, 0, 10000, 10000, 0, 11, 84961, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Necropolis Flower Controller - On Spellhit Rayne''s Seed - Cast Flower Aura'),
(45488, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 45488, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Necropolis Flower Controller - Linked To Id 0 - Quest Credit');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (84964, 84973);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 84964, 0, 0, 31, 0, 3, 45486, 0, 0, 0, 0, '', 'Rayne''s Seeds can hit the southeastern flower controller'),
(13, 1, 84964, 0, 1, 31, 0, 3, 45487, 0, 0, 0, 0, '', 'Rayne''s Seeds can hit the northeastern flower controller'),
(13, 1, 84964, 0, 2, 31, 0, 3, 45488, 0, 0, 0, 0, '', 'Rayne''s Seeds can hit the western flower controller'),
(13, 1, 84973, 0, 0, 31, 0, 3, 45475, 0, 0, 0, 0, '', 'Overcharge can hit Plague Disseminators');
