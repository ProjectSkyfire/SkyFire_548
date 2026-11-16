-- Dragonblight: quest credit given by a spell hit on a creature that had no SmartAI here.
-- 26321 Lothalor Ancient (12092, 12096) and 27450 Neltharion's Flame Control Bunny (12267).
-- SmartAI of 26321, 27450
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (26321,27450) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (26321,27450);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(26321, 0, 0, 0, 8, 0, 100, 1, 47530, 0, 0, 0, 0, 33, 26321, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lothalor Ancient - On Spellhit ''Bark of the Walkers'' - Quest Credit ''Strengthen the Ancients'' (Phase 1) (No Repeat)'),
(27450, 0, 0, 1, 8, 0, 100, 0, 48790, 0, 0, 0, 0, 33, 27450, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Neltharion''s Flame Control Bunny - On SpellHit Neltharion''s Flame - Quest Credit'),
(27450, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 27355, 10, 0, 0, 0, 0, 0, 'Neltharion''s Flame Control Bunny - Linked To Id 0 - Set Data 0 1');
