-- Halga Bloodeye: the quest event action was refused at load (quest 13949 is not an event quest), which broke the
-- link to closing the gossip. The kill credit alone completes the objective.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 34258 AND `id` = 1;
UPDATE `smart_scripts` SET `link` = 2 WHERE `source_type` = 0 AND `entryorguid` = 34258 AND `id` = 0;
