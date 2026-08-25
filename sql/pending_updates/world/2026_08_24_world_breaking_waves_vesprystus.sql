-- Quests 26383 / 26385 Breaking Waves of Change: missing speak-to objective
-- for Vesprystus (3838). Sniffed as QUEST_OBJECTIVE_TYPE_DUMMY (10); credit on
-- gossip hello via AreaExploredOrEventHappens. Taxi option "I need a ride."
-- remains the lift dialogue (OptionType=4).

DELETE FROM `quest_objective` WHERE `questId` IN (26383, 26385);
INSERT INTO `quest_objective` (`questId`, `id`, `index`, `type`, `objectId`, `amount`, `flags`, `description`) VALUES
(26383, 2638300, 0, 10, 0, 0, 0, 'Use the teleportation tree in western Darnassus to reach Rut''theran Village, then speak to Vesprystus to secure a ride to Lor''danel.'),
(26385, 2638500, 0, 10, 0, 0, 0, 'Speak to Vesprystus in Rut''theran Village to secure a ride to Lor''danel.');

UPDATE `quest_template` SET `SpecialFlags`=`SpecialFlags`|2 WHERE `Id` IN (26383, 26385);

DELETE FROM `smart_scripts` WHERE `entryorguid`=3838 AND `source_type`=0 AND `id` IN (3, 4);
INSERT INTO `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
 `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
 `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
 `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3838, 0, 3, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 15, 26383, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Vesprystus - On Gossip Hello - Credit quest 26383'),
(3838, 0, 4, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 15, 26385, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Vesprystus - On Gossip Hello - Credit quest 26385');
