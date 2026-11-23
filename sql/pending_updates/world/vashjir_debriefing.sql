-- Vashj'ir, "Debriefing" (26007): debrief Gurrok. The credit spell 78108 credits the targeted player, and nothing cast
-- it; Gurrok (41885) had no gossip menu. The debriefing is the chain of menus 11555, 11557, 11558, 11560, 11559 and
-- 11562 of the 5.4.8 data; the credit comes with the last question. Decompression follows the quest.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 41885;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `gossip_menu_id` = 11555 WHERE `entry` = 41885;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41885, 0, 0, 0, 62, 0, 100, 0, 11562, 0, 0, 0, 0, 11, 78108, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Option 0 Selected - Cast Quest Credit Gurrok on Invoker');

UPDATE `quest_template` SET `PrevQuestId` = 25887 WHERE `Id` = 26008;
