-- The Grimtotem Weapon (11169): the Captured Totem (23811) summoned by the quest item (spell 42452) walked after the
-- player for its whole 90 s. The 5.4.8 summon properties (3034, flag 512) make it a guardian of the player, and a
-- guardian that evades is sent to follow its owner (SmartAI evade, MotionMaster::MoveTargetedHome). The totem is a
-- stationary prop whose periodic aura marks the crocolisks around it, so it is rooted (SmartAI root, on spawn and on
-- evade). The quest credit (a crocolisk or daggermaw killed while it carries the totem aura, conditions 22 on 4344 and
-- 4345) is unchanged. Legends of Azeroth 5.4.8 has the same summon and the same credit rows.
-- Safe to run twice.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 23811 AND `id` >= 3;
UPDATE `smart_scripts` SET `link` = 3 WHERE `source_type` = 0 AND `entryorguid` = 23811 AND `id` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(23811, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Totem - Linked To Id 1 - Root'),
(23811, 0, 4, 0, 7, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Totem - On Evade - Root');
