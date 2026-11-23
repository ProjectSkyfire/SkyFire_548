-- Vashj'ir, "Fallen But Not Forgotten" (25964): rescue six Injured Assault Volunteers. The spell 77398 credits the
-- objective 41281 to the targeted player and nothing cast it, so the quest could not be completed. The volunteer (41784,
-- 38 spawns) has the gossip option of menu 11481 ("The way to the rendezvous point is mostly clear ..."); picking it
-- casts the spell and the volunteer leaves, so the same one is not counted twice. Row 0 of the entry is kept.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 41784 AND `id` IN (1, 2, 3);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41784, 0, 1, 2, 62, 0, 100, 0, 11481, 0, 0, 0, 0, 11, 77398, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Option 0 Selected - Cast Rescue Injured Soldier Credit on Invoker'),
(41784, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Close Gossip'),
(41784, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Link - Despawn');
